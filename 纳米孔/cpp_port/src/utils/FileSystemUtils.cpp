#include "FileManager.h"
#include <fstream>
#include <regex>
#include <random>
#include <thread>
#include <map>
#include <filesystem>
#include <cstdio>

namespace FileManager {
namespace FileSystemUtils {

bool exists(const std::string& path) {
    return std::filesystem::exists(std::filesystem::path(path));
}

bool isFile(const std::string& path) {
    return std::filesystem::is_regular_file(std::filesystem::path(path));
}

bool isDirectory(const std::string& path) {
    return std::filesystem::is_directory(std::filesystem::path(path));
}

size_t getFileSize(const std::string& path) {
    try {
        return static_cast<size_t>(std::filesystem::file_size(std::filesystem::path(path)));
    } catch (...) {
        return 0;
    }
}

std::string getFileExtension(const std::string& path) {
    return std::filesystem::path(path).extension().string();
}

std::string getBaseName(const std::string& path) {
    return std::filesystem::path(path).filename().string();
}

std::string getDirectoryName(const std::string& path) {
    auto p = std::filesystem::path(path);
    auto parent = p.parent_path();
    return parent.empty() ? std::string(".") : parent.string();
}

bool createDirectory(const std::string& path) {
    try {
        return std::filesystem::create_directories(std::filesystem::path(path));
    } catch (...) {
        return false;
    }
}

bool removeFile(const std::string& path) {
    try {
        return std::filesystem::remove(std::filesystem::path(path));
    } catch (...) {
        return false;
    }
}

bool removeDirectory(const std::string& path, bool recursive) {
    try {
        if (recursive) {
            return std::filesystem::remove_all(std::filesystem::path(path)) > 0;
        } else {
            return std::filesystem::remove(std::filesystem::path(path));
        }
    } catch (...) {
        return false;
    }
}

bool copyFile(const std::string& source, const std::string& destination) {
    try {
        return std::filesystem::copy_file(
            std::filesystem::path(source),
            std::filesystem::path(destination),
            std::filesystem::copy_options::overwrite_existing);
    } catch (...) {
        return false;
    }
}

bool moveFile(const std::string& source, const std::string& destination) {
    try {
        std::filesystem::rename(std::filesystem::path(source), std::filesystem::path(destination));
        return true;
    } catch (...) {
        return false;
    }
}

std::vector<std::string> listFiles(const std::string& directory, 
                                  const std::string& pattern, 
                                  bool recursive) {
    std::vector<std::string> files;
    std::regex pattern_regex(pattern == "*" ? ".*" : pattern);
    try {
        if (recursive) {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
                if (entry.is_regular_file()) {
                    const auto name = entry.path().filename().string();
                    if (std::regex_match(name, pattern_regex)) {
                        files.push_back(entry.path().string());
                    }
                }
            }
        } else {
            for (const auto& entry : std::filesystem::directory_iterator(directory)) {
                if (entry.is_regular_file()) {
                    const auto name = entry.path().filename().string();
                    if (std::regex_match(name, pattern_regex)) {
                        files.push_back(entry.path().string());
                    }
                }
            }
        }
    } catch (...) {
        // ignore
    }
    return files;
}

std::vector<std::string> listDirectories(const std::string& directory, 
                                        bool recursive) {
    std::vector<std::string> directories;
    try {
        if (recursive) {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
                if (entry.is_directory()) {
                    directories.push_back(entry.path().string());
                }
            }
        } else {
            for (const auto& entry : std::filesystem::directory_iterator(directory)) {
                if (entry.is_directory()) {
                    directories.push_back(entry.path().string());
                }
            }
        }
    } catch (...) {
        // ignore
    }
    return directories;
}

std::string getTemporaryDirectory() {
    try {
        return std::filesystem::temp_directory_path().string();
    } catch (...) {
        const char* tmp = getenv("TMPDIR");
        if (!tmp) tmp = getenv("TMP");
        if (!tmp) tmp = getenv("TEMP");
        return tmp ? std::string(tmp) : std::string("/tmp");
    }
}

std::string generateTempFilename(const std::string& prefix, 
                               const std::string& extension) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(1000, 9999);
    
    std::string temp_dir = getTemporaryDirectory();
    std::string filename = prefix + "_" + std::to_string(dis(gen)) + extension;
    return (std::filesystem::path(temp_dir) / filename).string();
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