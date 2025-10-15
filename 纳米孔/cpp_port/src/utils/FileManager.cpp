#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <regex>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <chrono>
#include <cmath>
#include <iomanip>

namespace fs = std::filesystem;

namespace FileManager {

// FileManager::Impl 实现
struct FileManager::Impl {
    std::vector<std::unique_ptr<FileReader>> readers;
    std::vector<std::unique_ptr<FileWriter>> writers;
    
    // 缓存系统
    bool cache_enabled = false;
    size_t max_cache_size = 100;
    std::unordered_map<std::string, Dataset> cache;
    std::mutex cache_mutex;
    
    // 错误处理
    std::string last_error;
    std::vector<std::string> error_log;
    std::mutex error_mutex;
    
    void addError(const std::string& error) {
        std::lock_guard<std::mutex> lock(error_mutex);
        last_error = error;
        error_log.push_back(error);
    }
    
    void clearErrors() {
        std::lock_guard<std::mutex> lock(error_mutex);
        last_error.clear();
        error_log.clear();
    }
};

// TxtReader 实现
bool TxtReader::canRead(const std::string& filepath) const {
    std::string ext = FileSystemUtils::getFileExtension(filepath);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext == ".txt" || ext == ".dat" || ext == ".asc";
}

Dataset TxtReader::read(const std::string& filepath, const ImportOptions& options) {
    Dataset dataset;
    dataset.metadata.filepath = filepath;
    dataset.metadata.filename = FileSystemUtils::getBaseName(filepath);
    dataset.metadata.format = FileFormat::TXT;
    
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件: " + filepath);
    }
    
    std::string line;
    int line_count = 0;
    
    // 跳过指定行数
    for (int i = 0; i < options.skip_rows && std::getline(file, line); ++i) {
        line_count++;
    }
    
    // 读取数据
    std::vector<std::vector<double>> temp_data;
    while (std::getline(file, line) && 
           (options.max_rows == -1 || line_count < options.skip_rows + options.max_rows)) {
        
        // 跳过注释行
        if (!line.empty() && line[0] == options.comment_char) {
            continue;
        }
        
        std::istringstream iss(line);
        std::vector<double> row;
        std::string token;
        
        while (std::getline(iss, token, options.delimiter)) {
            try {
                double value = std::stod(token);
                row.push_back(value);
            } catch (const std::exception&) {
                // 跳过无法转换的值
                continue;
            }
        }
        
        if (!row.empty()) {
            temp_data.push_back(row);
        }
        line_count++;
    }
    
    // 转置数据（从行优先转为列优先）
    if (!temp_data.empty()) {
        size_t num_cols = temp_data[0].size();
        dataset.data.resize(num_cols);
        dataset.column_names.resize(num_cols);
        
        for (size_t col = 0; col < num_cols; ++col) {
            dataset.column_names[col] = "Column_" + std::to_string(col + 1);
            for (const auto& row : temp_data) {
                if (col < row.size()) {
                    dataset.data[col].push_back(row[col]);
                }
            }
        }
    }
    
    dataset.metadata.data_points = temp_data.size();
    dataset.metadata.channels = dataset.data.size();
    
    return dataset;
}

FileMetadata TxtReader::getMetadata(const std::string& filepath) {
    FileMetadata metadata;
    metadata.filepath = filepath;
    metadata.filename = FileSystemUtils::getBaseName(filepath);
    metadata.format = FileFormat::TXT;
    metadata.compression = CompressionType::NONE;
    
    if (FileSystemUtils::exists(filepath)) {
        metadata.file_size = FileSystemUtils::getFileSize(filepath);
        
        // 获取文件时间信息
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        metadata.modification_time = oss.str();
        metadata.creation_time = metadata.modification_time;
    }
    
    return metadata;
}

// CsvReader 实现
bool CsvReader::canRead(const std::string& filepath) const {
    std::string ext = FileSystemUtils::getFileExtension(filepath);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext == ".csv";
}

Dataset CsvReader::read(const std::string& filepath, const ImportOptions& options) {
    ImportOptions csv_options = options;
    csv_options.delimiter = ',';
    
    TxtReader txt_reader;
    Dataset dataset = txt_reader.read(filepath, csv_options);
    dataset.metadata.format = FileFormat::CSV;
    
    return dataset;
}

FileMetadata CsvReader::getMetadata(const std::string& filepath) {
    TxtReader txt_reader;
    FileMetadata metadata = txt_reader.getMetadata(filepath);
    metadata.format = FileFormat::CSV;
    return metadata;
}

// JsonReader 实现
bool JsonReader::canRead(const std::string& filepath) const {
    std::string ext = FileSystemUtils::getFileExtension(filepath);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext == ".json";
}

Dataset JsonReader::read(const std::string& filepath, const ImportOptions& options) {
    Dataset dataset;
    dataset.metadata.filepath = filepath;
    dataset.metadata.filename = FileSystemUtils::getBaseName(filepath);
    dataset.metadata.format = FileFormat::JSON;
    
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开JSON文件: " + filepath);
    }
    
    // 简化的JSON解析（实际项目中应使用专业的JSON库）
    std::string content((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    
    // 这里应该实现完整的JSON解析
    // 为了演示，我们假设JSON格式为简单的数组结构
    dataset.name = "JSON_Data";
    
    return dataset;
}

FileMetadata JsonReader::getMetadata(const std::string& filepath) {
    FileMetadata metadata;
    metadata.filepath = filepath;
    metadata.filename = FileSystemUtils::getBaseName(filepath);
    metadata.format = FileFormat::JSON;
    metadata.compression = CompressionType::NONE;
    
    if (FileSystemUtils::exists(filepath)) {
        metadata.file_size = FileSystemUtils::getFileSize(filepath);
    }
    
    return metadata;
}

// BinaryReader 实现
bool BinaryReader::canRead(const std::string& filepath) const {
    std::string ext = FileSystemUtils::getFileExtension(filepath);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext == ".bin" || ext == ".dat";
}

Dataset BinaryReader::read(const std::string& filepath, const ImportOptions& options) {
    Dataset dataset;
    dataset.metadata.filepath = filepath;
    dataset.metadata.filename = FileSystemUtils::getBaseName(filepath);
    dataset.metadata.format = FileFormat::BINARY;
    
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开二进制文件: " + filepath);
    }
    
    // 读取二进制数据（假设为double类型）
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    size_t num_doubles = file_size / sizeof(double);
    std::vector<double> data(num_doubles);
    
    file.read(reinterpret_cast<char*>(data.data()), file_size);
    
    dataset.data.push_back(data);
    dataset.column_names.push_back("Binary_Data");
    dataset.metadata.data_points = num_doubles;
    dataset.metadata.channels = 1;
    
    return dataset;
}

FileMetadata BinaryReader::getMetadata(const std::string& filepath) {
    FileMetadata metadata;
    metadata.filepath = filepath;
    metadata.filename = FileSystemUtils::getBaseName(filepath);
    metadata.format = FileFormat::BINARY;
    metadata.compression = CompressionType::NONE;
    
    if (FileSystemUtils::exists(filepath)) {
        metadata.file_size = FileSystemUtils::getFileSize(filepath);
        metadata.data_points = metadata.file_size / sizeof(double);
        metadata.channels = 1;
        metadata.data_type = "double";
    }
    
    return metadata;
}

// TxtWriter 实现
bool TxtWriter::canWrite(FileFormat format) const {
    return format == FileFormat::TXT;
}

bool TxtWriter::write(const Dataset& dataset, const std::string& filepath, 
                     const ExportOptions& options) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        return false;
    }
    
    // 写入头部
    if (options.include_header && !dataset.column_names.empty()) {
        for (size_t i = 0; i < dataset.column_names.size(); ++i) {
            if (i > 0) file << options.delimiter;
            file << dataset.column_names[i];
        }
        file << "\n";
    }
    
    // 写入数据
    if (!dataset.data.empty()) {
        size_t max_rows = 0;
        for (const auto& column : dataset.data) {
            max_rows = std::max(max_rows, column.size());
        }
        
        for (size_t row = 0; row < max_rows; ++row) {
            for (size_t col = 0; col < dataset.data.size(); ++col) {
                if (col > 0) file << options.delimiter;
                if (row < dataset.data[col].size()) {
                    file << std::fixed << std::setprecision(options.precision) 
                         << dataset.data[col][row];
                }
            }
            file << "\n";
        }
    }
    
    return true;
}

// CsvWriter 实现
bool CsvWriter::canWrite(FileFormat format) const {
    return format == FileFormat::CSV;
}

bool CsvWriter::write(const Dataset& dataset, const std::string& filepath, 
                     const ExportOptions& options) {
    ExportOptions csv_options = options;
    csv_options.delimiter = ',';
    
    TxtWriter txt_writer;
    return txt_writer.write(dataset, filepath, csv_options);
}

// JsonWriter 实现
bool JsonWriter::canWrite(FileFormat format) const {
    return format == FileFormat::JSON;
}

bool JsonWriter::write(const Dataset& dataset, const std::string& filepath, 
                      const ExportOptions& options) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        return false;
    }
    
    file << "{\n";
    file << "  \"name\": \"" << dataset.name << "\",\n";
    file << "  \"columns\": [";
    
    for (size_t i = 0; i < dataset.column_names.size(); ++i) {
        if (i > 0) file << ", ";
        file << "\"" << dataset.column_names[i] << "\"";
    }
    
    file << "],\n";
    file << "  \"data\": [\n";
    
    if (!dataset.data.empty()) {
        size_t max_rows = 0;
        for (const auto& column : dataset.data) {
            max_rows = std::max(max_rows, column.size());
        }
        
        for (size_t row = 0; row < max_rows; ++row) {
            if (row > 0) file << ",\n";
            file << "    [";
            for (size_t col = 0; col < dataset.data.size(); ++col) {
                if (col > 0) file << ", ";
                if (row < dataset.data[col].size()) {
                    file << std::fixed << std::setprecision(options.precision) 
                         << dataset.data[col][row];
                } else {
                    file << "null";
                }
            }
            file << "]";
        }
    }
    
    file << "\n  ]\n";
    file << "}\n";
    
    return true;
}

// BinaryWriter 实现
bool BinaryWriter::canWrite(FileFormat format) const {
    return format == FileFormat::BINARY;
}

bool BinaryWriter::write(const Dataset& dataset, const std::string& filepath, 
                        const ExportOptions& options) {
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    // 写入所有列的数据
    for (const auto& column : dataset.data) {
        file.write(reinterpret_cast<const char*>(column.data()), 
                  column.size() * sizeof(double));
    }
    
    return true;
}

// FileManager 实现
FileManager::FileManager() : pImpl_(std::make_unique<Impl>()) {
    // 注册默认的读取器和写入器
    registerReader(std::make_unique<TxtReader>());
    registerReader(std::make_unique<CsvReader>());
    registerReader(std::make_unique<JsonReader>());
    registerReader(std::make_unique<BinaryReader>());
    
    registerWriter(std::make_unique<TxtWriter>());
    registerWriter(std::make_unique<CsvWriter>());
    registerWriter(std::make_unique<JsonWriter>());
    registerWriter(std::make_unique<BinaryWriter>());
}

FileManager::~FileManager() = default;

void FileManager::registerReader(std::unique_ptr<FileReader> reader) {
    pImpl_->readers.push_back(std::move(reader));
}

void FileManager::registerWriter(std::unique_ptr<FileWriter> writer) {
    pImpl_->writers.push_back(std::move(writer));
}

FileFormat FileManager::detectFormat(const std::string& filepath) const {
    std::string ext = FileSystemUtils::getFileExtension(filepath);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    if (ext == ".txt" || ext == ".dat" || ext == ".asc") return FileFormat::TXT;
    if (ext == ".csv") return FileFormat::CSV;
    if (ext == ".tsv") return FileFormat::TSV;
    if (ext == ".json") return FileFormat::JSON;
    if (ext == ".xml") return FileFormat::XML;
    if (ext == ".bin") return FileFormat::BINARY;
    
    return FileFormat::AUTO;
}

bool FileManager::isSupported(const std::string& filepath) const {
    for (const auto& reader : pImpl_->readers) {
        if (reader->canRead(filepath)) {
            return true;
        }
    }
    return false;
}

std::vector<FileFormat> FileManager::getSupportedFormats() const {
    return {FileFormat::TXT, FileFormat::CSV, FileFormat::JSON, FileFormat::BINARY};
}

Dataset FileManager::loadFile(const std::string& filepath, const ImportOptions& options) {
    // 检查缓存
    if (pImpl_->cache_enabled) {
        std::lock_guard<std::mutex> lock(pImpl_->cache_mutex);
        auto it = pImpl_->cache.find(filepath);
        if (it != pImpl_->cache.end()) {
            return it->second;
        }
    }
    
    // 查找合适的读取器
    for (const auto& reader : pImpl_->readers) {
        if (reader->canRead(filepath)) {
            try {
                Dataset dataset = reader->read(filepath, options);
                
                // 添加到缓存
                if (pImpl_->cache_enabled) {
                    std::lock_guard<std::mutex> lock(pImpl_->cache_mutex);
                    if (pImpl_->cache.size() >= pImpl_->max_cache_size) {
                        pImpl_->cache.clear();  // 简单的缓存清理策略
                    }
                    pImpl_->cache[filepath] = dataset;
                }
                
                return dataset;
            } catch (const std::exception& e) {
                pImpl_->addError("读取文件失败: " + std::string(e.what()));
                throw;
            }
        }
    }
    
    throw std::runtime_error("不支持的文件格式: " + filepath);
}

bool FileManager::saveFile(const Dataset& dataset, const std::string& filepath, 
                          const ExportOptions& options) {
    // 查找合适的写入器
    for (const auto& writer : pImpl_->writers) {
        if (writer->canWrite(options.format)) {
            try {
                return writer->write(dataset, filepath, options);
            } catch (const std::exception& e) {
                pImpl_->addError("保存文件失败: " + std::string(e.what()));
                return false;
            }
        }
    }
    
    pImpl_->addError("不支持的输出格式");
    return false;
}

std::vector<Dataset> FileManager::loadMultiple(const std::vector<std::string>& filepaths, 
                                              const ImportOptions& options) {
    std::vector<Dataset> datasets;
    datasets.reserve(filepaths.size());
    
    for (const auto& filepath : filepaths) {
        try {
            datasets.push_back(loadFile(filepath, options));
        } catch (const std::exception& e) {
            pImpl_->addError("加载文件失败: " + filepath + " - " + e.what());
        }
    }
    
    return datasets;
}

bool FileManager::saveMultiple(const std::vector<Dataset>& datasets, 
                              const std::vector<std::string>& filepaths, 
                              const ExportOptions& options) {
    if (datasets.size() != filepaths.size()) {
        pImpl_->addError("数据集数量与文件路径数量不匹配");
        return false;
    }
    
    bool all_success = true;
    for (size_t i = 0; i < datasets.size(); ++i) {
        if (!saveFile(datasets[i], filepaths[i], options)) {
            all_success = false;
        }
    }
    
    return all_success;
}

bool FileManager::convertFile(const std::string& input_path, const std::string& output_path, 
                             const ImportOptions& import_opts, 
                             const ExportOptions& export_opts) {
    try {
        Dataset dataset = loadFile(input_path, import_opts);
        return saveFile(dataset, output_path, export_opts);
    } catch (const std::exception& e) {
        pImpl_->addError("文件转换失败: " + std::string(e.what()));
        return false;
    }
}

FileMetadata FileManager::getFileInfo(const std::string& filepath) {
    for (const auto& reader : pImpl_->readers) {
        if (reader->canRead(filepath)) {
            return reader->getMetadata(filepath);
        }
    }
    
    FileMetadata metadata;
    metadata.filepath = filepath;
    metadata.filename = FileSystemUtils::getBaseName(filepath);
    return metadata;
}

void FileManager::enableCache(bool enable, size_t max_cache_size) {
    std::lock_guard<std::mutex> lock(pImpl_->cache_mutex);
    pImpl_->cache_enabled = enable;
    pImpl_->max_cache_size = max_cache_size;
    if (!enable) {
        pImpl_->cache.clear();
    }
}

void FileManager::clearCache() {
    std::lock_guard<std::mutex> lock(pImpl_->cache_mutex);
    pImpl_->cache.clear();
}

size_t FileManager::getCacheSize() const {
    std::lock_guard<std::mutex> lock(pImpl_->cache_mutex);
    return pImpl_->cache.size();
}

std::string FileManager::getLastError() const {
    std::lock_guard<std::mutex> lock(pImpl_->error_mutex);
    return pImpl_->last_error;
}

std::vector<std::string> FileManager::getErrorLog() const {
    std::lock_guard<std::mutex> lock(pImpl_->error_mutex);
    return pImpl_->error_log;
}

void FileManager::clearErrors() {
    pImpl_->clearErrors();
}

} // namespace FileManager