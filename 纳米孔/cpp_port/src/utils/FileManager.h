#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>

namespace FileManager {

// 支持的文件格式
enum class FileFormat {
    TXT,        // 纯文本格式
    CSV,        // 逗号分隔值
    TSV,        // 制表符分隔值
    JSON,       // JSON格式
    XML,        // XML格式
    HDF5,       // HDF5科学数据格式
    BINARY,     // 二进制格式
    MATLAB,     // MATLAB .mat格式
    NUMPY,      // NumPy .npy格式
    EXCEL,      // Excel格式
    AUTO        // 自动检测格式
};

// 数据压缩类型
enum class CompressionType {
    NONE,       // 无压缩
    GZIP,       // GZIP压缩
    ZLIB,       // ZLIB压缩
    LZ4,        // LZ4压缩
    ZSTD        // Zstandard压缩
};

// 文件元数据
struct FileMetadata {
    std::string filename;
    std::string filepath;
    FileFormat format;
    CompressionType compression;
    size_t file_size;
    std::string creation_time;
    std::string modification_time;
    std::map<std::string, std::string> custom_attributes;
    
    // 数据相关信息
    size_t data_points;
    size_t channels;
    double sampling_rate;
    std::string data_type;
    std::string description;
};

// 数据集结构
struct Dataset {
    std::string name;
    std::vector<std::string> column_names;
    std::vector<std::vector<double>> data;
    std::map<std::string, std::string> attributes;
    FileMetadata metadata;
};

// 批量处理选项
struct BatchOptions {
    bool parallel_processing = true;
    int max_threads = 4;
    bool skip_errors = true;
    bool create_backup = false;
    std::string output_directory;
    std::string filename_pattern = "{original_name}_processed";
    CompressionType compression = CompressionType::NONE;
};

// 导入/导出选项
struct ImportOptions {
    FileFormat format = FileFormat::AUTO;
    bool has_header = true;
    char delimiter = ',';
    char comment_char = '#';
    std::vector<int> columns_to_read;  // 空表示读取所有列
    int skip_rows = 0;
    int max_rows = -1;  // -1表示读取所有行
    std::string encoding = "utf-8";
    bool auto_detect_types = true;
};

struct ExportOptions {
    FileFormat format = FileFormat::CSV;
    CompressionType compression = CompressionType::NONE;
    bool include_header = true;
    char delimiter = ',';
    int precision = 6;
    std::string encoding = "utf-8";
    bool overwrite_existing = false;
    std::map<std::string, std::string> metadata;
};

// 文件读取器基类
class FileReader {
public:
    virtual ~FileReader() = default;
    virtual bool canRead(const std::string& filepath) const = 0;
    virtual Dataset read(const std::string& filepath, const ImportOptions& options = {}) = 0;
    virtual FileMetadata getMetadata(const std::string& filepath) = 0;
};

// 文件写入器基类
class FileWriter {
public:
    virtual ~FileWriter() = default;
    virtual bool canWrite(FileFormat format) const = 0;
    virtual bool write(const Dataset& dataset, const std::string& filepath, 
                      const ExportOptions& options = {}) = 0;
};

// 具体的读取器实现
class TxtReader : public FileReader {
public:
    bool canRead(const std::string& filepath) const override;
    Dataset read(const std::string& filepath, const ImportOptions& options = {}) override;
    FileMetadata getMetadata(const std::string& filepath) override;
};

class CsvReader : public FileReader {
public:
    bool canRead(const std::string& filepath) const override;
    Dataset read(const std::string& filepath, const ImportOptions& options = {}) override;
    FileMetadata getMetadata(const std::string& filepath) override;
};

class JsonReader : public FileReader {
public:
    bool canRead(const std::string& filepath) const override;
    Dataset read(const std::string& filepath, const ImportOptions& options = {}) override;
    FileMetadata getMetadata(const std::string& filepath) override;
};

class BinaryReader : public FileReader {
public:
    bool canRead(const std::string& filepath) const override;
    Dataset read(const std::string& filepath, const ImportOptions& options = {}) override;
    FileMetadata getMetadata(const std::string& filepath) override;
};

// 具体的写入器实现
class TxtWriter : public FileWriter {
public:
    bool canWrite(FileFormat format) const override;
    bool write(const Dataset& dataset, const std::string& filepath, 
              const ExportOptions& options = {}) override;
};

class CsvWriter : public FileWriter {
public:
    bool canWrite(FileFormat format) const override;
    bool write(const Dataset& dataset, const std::string& filepath, 
              const ExportOptions& options = {}) override;
};

class JsonWriter : public FileWriter {
public:
    bool canWrite(FileFormat format) const override;
    bool write(const Dataset& dataset, const std::string& filepath, 
              const ExportOptions& options = {}) override;
};

class BinaryWriter : public FileWriter {
public:
    bool canWrite(FileFormat format) const override;
    bool write(const Dataset& dataset, const std::string& filepath, 
              const ExportOptions& options = {}) override;
};

// 文件管理器主类
class FileManager {
public:
    FileManager();
    ~FileManager();
    
    // 注册读取器和写入器
    void registerReader(std::unique_ptr<FileReader> reader);
    void registerWriter(std::unique_ptr<FileWriter> writer);
    
    // 文件格式检测
    FileFormat detectFormat(const std::string& filepath) const;
    bool isSupported(const std::string& filepath) const;
    std::vector<FileFormat> getSupportedFormats() const;
    
    // 单文件操作
    Dataset loadFile(const std::string& filepath, const ImportOptions& options = {});
    bool saveFile(const Dataset& dataset, const std::string& filepath, 
                  const ExportOptions& options = {});
    
    // 批量操作
    std::vector<Dataset> loadMultiple(const std::vector<std::string>& filepaths, 
                                     const ImportOptions& options = {});
    bool saveMultiple(const std::vector<Dataset>& datasets, 
                     const std::vector<std::string>& filepaths, 
                     const ExportOptions& options = {});
    
    // 文件转换
    bool convertFile(const std::string& input_path, const std::string& output_path, 
                    const ImportOptions& import_opts = {}, 
                    const ExportOptions& export_opts = {});
    
    bool convertMultiple(const std::vector<std::string>& input_paths, 
                        const std::vector<std::string>& output_paths, 
                        const ImportOptions& import_opts = {}, 
                        const ExportOptions& export_opts = {});
    
    // 批量处理
    bool batchProcess(const std::vector<std::string>& input_paths, 
                     std::function<Dataset(const Dataset&)> processor, 
                     const BatchOptions& options = {});
    
    // 文件信息
    FileMetadata getFileInfo(const std::string& filepath);
    std::vector<FileMetadata> getDirectoryInfo(const std::string& directory, 
                                              bool recursive = false);
    
    // 压缩和解压缩
    bool compressFile(const std::string& input_path, const std::string& output_path, 
                     CompressionType type = CompressionType::GZIP);
    bool decompressFile(const std::string& input_path, const std::string& output_path);
    
    // 文件验证
    bool validateFile(const std::string& filepath);
    std::vector<std::string> validateMultiple(const std::vector<std::string>& filepaths);
    
    // 缓存管理
    void enableCache(bool enable, size_t max_cache_size = 100);
    void clearCache();
    size_t getCacheSize() const;
    
    // 错误处理
    std::string getLastError() const;
    std::vector<std::string> getErrorLog() const;
    void clearErrors();

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl_;
};

// 数据压缩工具
class CompressionUtils {
public:
    static std::vector<uint8_t> compress(const std::vector<uint8_t>& data, 
                                        CompressionType type);
    static std::vector<uint8_t> decompress(const std::vector<uint8_t>& data, 
                                          CompressionType type);
    
    static bool compressFile(const std::string& input_path, 
                           const std::string& output_path, 
                           CompressionType type);
    static bool decompressFile(const std::string& input_path, 
                             const std::string& output_path, 
                             CompressionType type);
    
    static CompressionType detectCompression(const std::string& filepath);
    static double getCompressionRatio(const std::string& original_path, 
                                    const std::string& compressed_path);
};

// 文件系统工具
namespace FileSystemUtils {
    bool exists(const std::string& path);
    bool isFile(const std::string& path);
    bool isDirectory(const std::string& path);
    size_t getFileSize(const std::string& path);
    std::string getFileExtension(const std::string& path);
    std::string getBaseName(const std::string& path);
    std::string getDirectoryName(const std::string& path);
    
    bool createDirectory(const std::string& path);
    bool removeFile(const std::string& path);
    bool removeDirectory(const std::string& path, bool recursive = false);
    bool copyFile(const std::string& source, const std::string& destination);
    bool moveFile(const std::string& source, const std::string& destination);
    
    std::vector<std::string> listFiles(const std::string& directory, 
                                      const std::string& pattern = "*", 
                                      bool recursive = false);
    std::vector<std::string> listDirectories(const std::string& directory, 
                                            bool recursive = false);
    
    std::string getTemporaryDirectory();
    std::string generateTempFilename(const std::string& prefix = "temp", 
                                   const std::string& extension = ".tmp");
    
    // 文件监控
    class FileWatcher {
    public:
        enum class Event { CREATED, MODIFIED, DELETED, MOVED };
        using Callback = std::function<void(const std::string&, Event)>;
        
        FileWatcher();
        ~FileWatcher();
        
        bool watchFile(const std::string& filepath, Callback callback);
        bool watchDirectory(const std::string& directory, Callback callback, 
                          bool recursive = false);
        void stopWatching(const std::string& path);
        void stopAll();
        
    private:
        struct Impl;
        std::unique_ptr<Impl> pImpl_;
    };
}

// 数据验证工具
namespace DataValidation {
    struct ValidationResult {
        bool is_valid;
        std::vector<std::string> errors;
        std::vector<std::string> warnings;
        std::map<std::string, double> statistics;
    };
    
    ValidationResult validateDataset(const Dataset& dataset);
    ValidationResult validateNumericData(const std::vector<double>& data);
    ValidationResult validateTimeSeriesData(const std::vector<double>& time, 
                                           const std::vector<double>& values);
    
    bool hasNaN(const std::vector<double>& data);
    bool hasInfinite(const std::vector<double>& data);
    bool isMonotonic(const std::vector<double>& data);
    bool hasOutliers(const std::vector<double>& data, double threshold = 3.0);
    
    std::vector<int> findNaN(const std::vector<double>& data);
    std::vector<int> findInfinite(const std::vector<double>& data);
    std::vector<int> findOutliers(const std::vector<double>& data, double threshold = 3.0);
}

// 数据转换工具
namespace DataConversion {
    // 数据类型转换
    std::vector<double> stringToDouble(const std::vector<std::string>& strings);
    std::vector<std::string> doubleToString(const std::vector<double>& doubles, 
                                          int precision = 6);
    
    // 单位转换
    std::vector<double> convertUnits(const std::vector<double>& data, 
                                   const std::string& from_unit, 
                                   const std::string& to_unit);
    
    // 时间格式转换
    std::vector<double> parseTimeStrings(const std::vector<std::string>& time_strings, 
                                       const std::string& format = "auto");
    std::vector<std::string> formatTimeValues(const std::vector<double>& time_values, 
                                            const std::string& format = "ISO8601");
    
    // 数据重采样
    Dataset resampleDataset(const Dataset& dataset, double new_sampling_rate);
    std::vector<double> interpolate(const std::vector<double>& x_old, 
                                  const std::vector<double>& y_old, 
                                  const std::vector<double>& x_new);
}

} // namespace FileManager