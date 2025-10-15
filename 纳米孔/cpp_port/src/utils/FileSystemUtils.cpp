#include "FileManager.h"
#include <fstream>
#include <regex>
#include <random>
#include <thread>
#include <map>
#include <sys/stat.h>
#include <dirent.h>
#include <cstdio>
#include <unistd.h>

namespace FileManager {
namespace FileSystemUtils {

bool exists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

bool isFile(const std::string& path) {
    struct stat buffer;
    if (stat(path.c_str(), &buffer) != 0) {
        return false;
    }
    return S_ISREG(buffer.st_mode);
}

bool isDirectory(const std::string& path) {
    struct stat buffer;
    if (stat(path.c_str(), &buffer) != 0) {
        return false;
    }
    return S_ISDIR(buffer.st_mode);
}

size_t getFileSize(const std::string& path) {
    struct stat buffer;
    if (stat(path.c_str(), &buffer) != 0) {
        return 0;
    }
    return buffer.st_size;
}

std::string getFileExtension(const std::string& path) {
    size_t dot_pos = path.find_last_of('.');
    if (dot_pos == std::string::npos) {
        return "";
    }
    return path.substr(dot_pos);
}

std::string getBaseName(const std::string& path) {
    size_t slash_pos = path.find_last_of('/');
    if (slash_pos == std::string::npos) {
        return path;
    }
    return path.substr(slash_pos + 1);
}

std::string getDirectoryName(const std::string& path) {
    size_t slash_pos = path.find_last_of('/');
    if (slash_pos == std::string::npos) {
        return ".";
    }
    return path.substr(0, slash_pos);
}

bool createDirectory(const std::string& path) {
    return mkdir(path.c_str(), 0755) == 0;
}

bool removeFile(const std::string& path) {
    return std::remove(path.c_str()) == 0;
}

bool removeDirectory(const std::string& path, bool recursive) {
    if (recursive) {
        // 简化实现：只删除空目录
        return rmdir(path.c_str()) == 0;
    } else {
        return rmdir(path.c_str()) == 0;
    }
}

bool copyFile(const std::string& source, const std::string& destination) {
    std::ifstream src(source, std::ios::binary);
    std::ofstream dst(destination, std::ios::binary);
    
    if (!src.is_open() || !dst.is_open()) {
        return false;
    }
    
    dst << src.rdbuf();
    return true;
}

bool moveFile(const std::string& source, const std::string& destination) {
    return std::rename(source.c_str(), destination.c_str()) == 0;
}

std::vector<std::string> listFiles(const std::string& directory, 
                                  const std::string& pattern, 
                                  bool recursive) {
    std::vector<std::string> files;
    
    DIR* dir = opendir(directory.c_str());
    if (!dir) {
        return files;
    }
    
    std::regex pattern_regex(pattern == "*" ? ".*" : pattern);
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string name = entry->d_name;
        if (name == "." || name == "..") {
            continue;
        }
        
        std::string full_path = directory + "/" + name;
        if (isFile(full_path)) {
            if (std::regex_match(name, pattern_regex)) {
                files.push_back(full_path);
            }
        } else if (recursive && isDirectory(full_path)) {
            auto sub_files = listFiles(full_path, pattern, true);
            files.insert(files.end(), sub_files.begin(), sub_files.end());
        }
    }
    
    closedir(dir);
    return files;
}

std::vector<std::string> listDirectories(const std::string& directory, 
                                        bool recursive) {
    std::vector<std::string> directories;
    
    DIR* dir = opendir(directory.c_str());
    if (!dir) {
        return directories;
    }
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string name = entry->d_name;
        if (name == "." || name == "..") {
            continue;
        }
        
        std::string full_path = directory + "/" + name;
        if (isDirectory(full_path)) {
            directories.push_back(full_path);
            
            if (recursive) {
                auto sub_dirs = listDirectories(full_path, true);
                directories.insert(directories.end(), sub_dirs.begin(), sub_dirs.end());
            }
        }
    }
    
    closedir(dir);
    return directories;
}

std::string getTemporaryDirectory() {
    const char* temp_dir = getenv("TMPDIR");
    if (temp_dir) {
        return std::string(temp_dir);
    }
    return "/tmp";
}

std::string generateTempFilename(const std::string& prefix, 
                               const std::string& extension) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(1000, 9999);
    
    std::string temp_dir = getTemporaryDirectory();
    std::string filename = prefix + "_" + std::to_string(dis(gen)) + extension;
    
    return temp_dir + "/" + filename;
}

// FileWatcher 实现
struct FileWatcher::Impl {
    std::map<std::string, Callback> watchers;
    std::thread watch_thread;
    std::atomic<bool> running{false};
    std::mutex watchers_mutex;
    
    void watchLoop() {
        while (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            
            std::lock_guard<std::mutex> lock(watchers_mutex);
            for (const auto& [path, callback] : watchers) {
                // 简化的文件监控实现
                // 实际项目中应使用平台特定的文件监控API
                if (exists(path)) {
                    // 检查文件是否被修改
                    // 这里只是一个占位符实现
                }
            }
        }
    }
};

FileWatcher::FileWatcher() : pImpl_(std::make_unique<Impl>()) {}

FileWatcher::~FileWatcher() {
    stopAll();
}

bool FileWatcher::watchFile(const std::string& filepath, Callback callback) {
    if (!exists(filepath)) {
        return false;
    }
    
    std::lock_guard<std::mutex> lock(pImpl_->watchers_mutex);
    pImpl_->watchers[filepath] = callback;
    
    if (!pImpl_->running) {
        pImpl_->running = true;
        pImpl_->watch_thread = std::thread(&Impl::watchLoop, pImpl_.get());
    }
    
    return true;
}

bool FileWatcher::watchDirectory(const std::string& directory, Callback callback, 
                               bool recursive) {
    if (!isDirectory(directory)) {
        return false;
    }
    
    std::lock_guard<std::mutex> lock(pImpl_->watchers_mutex);
    pImpl_->watchers[directory] = callback;
    
    if (!pImpl_->running) {
        pImpl_->running = true;
        pImpl_->watch_thread = std::thread(&Impl::watchLoop, pImpl_.get());
    }
    
    return true;
}

void FileWatcher::stopWatching(const std::string& path) {
    std::lock_guard<std::mutex> lock(pImpl_->watchers_mutex);
    pImpl_->watchers.erase(path);
}

void FileWatcher::stopAll() {
    if (pImpl_->running) {
        pImpl_->running = false;
        if (pImpl_->watch_thread.joinable()) {
            pImpl_->watch_thread.join();
        }
    }
    
    std::lock_guard<std::mutex> lock(pImpl_->watchers_mutex);
    pImpl_->watchers.clear();
}

} // namespace FileSystemUtils

// CompressionUtils 实现
std::vector<uint8_t> CompressionUtils::compress(const std::vector<uint8_t>& data, 
                                               CompressionType type) {
    // 简化的压缩实现
    // 实际项目中应使用专业的压缩库如zlib、lz4等
    switch (type) {
        case CompressionType::NONE:
            return data;
        case CompressionType::GZIP:
        case CompressionType::ZLIB:
        case CompressionType::LZ4:
        case CompressionType::ZSTD:
        default:
            // 占位符实现：简单的RLE压缩
            std::vector<uint8_t> compressed;
            if (data.empty()) return compressed;
            
            uint8_t current = data[0];
            uint8_t count = 1;
            
            for (size_t i = 1; i < data.size(); ++i) {
                if (data[i] == current && count < 255) {
                    count++;
                } else {
                    compressed.push_back(count);
                    compressed.push_back(current);
                    current = data[i];
                    count = 1;
                }
            }
            
            compressed.push_back(count);
            compressed.push_back(current);
            
            return compressed;
    }
}

std::vector<uint8_t> CompressionUtils::decompress(const std::vector<uint8_t>& data, 
                                                 CompressionType type) {
    switch (type) {
        case CompressionType::NONE:
            return data;
        case CompressionType::GZIP:
        case CompressionType::ZLIB:
        case CompressionType::LZ4:
        case CompressionType::ZSTD:
        default:
            // 占位符实现：简单的RLE解压缩
            std::vector<uint8_t> decompressed;
            
            for (size_t i = 0; i < data.size(); i += 2) {
                if (i + 1 < data.size()) {
                    uint8_t count = data[i];
                    uint8_t value = data[i + 1];
                    
                    for (uint8_t j = 0; j < count; ++j) {
                        decompressed.push_back(value);
                    }
                }
            }
            
            return decompressed;
    }
}

bool CompressionUtils::compressFile(const std::string& input_path, 
                                   const std::string& output_path, 
                                   CompressionType type) {
    try {
        std::ifstream input(input_path, std::ios::binary);
        if (!input.is_open()) return false;
        
        std::vector<uint8_t> data((std::istreambuf_iterator<char>(input)),
                                 std::istreambuf_iterator<char>());
        
        std::vector<uint8_t> compressed = compress(data, type);
        
        std::ofstream output(output_path, std::ios::binary);
        if (!output.is_open()) return false;
        
        output.write(reinterpret_cast<const char*>(compressed.data()), 
                    compressed.size());
        
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool CompressionUtils::decompressFile(const std::string& input_path, 
                                     const std::string& output_path, 
                                     CompressionType type) {
    try {
        std::ifstream input(input_path, std::ios::binary);
        if (!input.is_open()) return false;
        
        std::vector<uint8_t> data((std::istreambuf_iterator<char>(input)),
                                 std::istreambuf_iterator<char>());
        
        std::vector<uint8_t> decompressed = decompress(data, type);
        
        std::ofstream output(output_path, std::ios::binary);
        if (!output.is_open()) return false;
        
        output.write(reinterpret_cast<const char*>(decompressed.data()), 
                    decompressed.size());
        
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

CompressionType CompressionUtils::detectCompression(const std::string& filepath) {
    std::string ext = FileSystemUtils::getFileExtension(filepath);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    if (ext == ".gz") return CompressionType::GZIP;
    if (ext == ".z") return CompressionType::ZLIB;
    if (ext == ".lz4") return CompressionType::LZ4;
    if (ext == ".zst") return CompressionType::ZSTD;
    
    return CompressionType::NONE;
}

double CompressionUtils::getCompressionRatio(const std::string& original_path, 
                                            const std::string& compressed_path) {
    size_t original_size = FileSystemUtils::getFileSize(original_path);
    size_t compressed_size = FileSystemUtils::getFileSize(compressed_path);
    
    if (original_size == 0) return 0.0;
    
    return static_cast<double>(compressed_size) / static_cast<double>(original_size);
}

} // namespace FileManager