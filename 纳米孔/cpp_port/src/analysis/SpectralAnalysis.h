#ifndef SPECTRAL_ANALYSIS_H
#define SPECTRAL_ANALYSIS_H

#include <vector>
#include <complex>
#include <string>
#include <memory>
#include <map>

namespace Analysis {

// 复数类型定义
using Complex = std::complex<double>;

// 窗函数类型
enum class WindowType {
    RECTANGULAR,
    HANNING,
    HAMMING,
    BLACKMAN,
    BARTLETT,
    KAISER,
    GAUSSIAN,
    TUKEY
};

// 频谱分析结果
struct SpectrumResult {
    std::vector<double> frequencies;
    std::vector<double> magnitude;
    std::vector<double> phase;
    std::vector<double> power;
    std::vector<double> power_density;
    double sampling_rate = 0.0;
    double frequency_resolution = 0.0;
    int fft_size = 0;
    WindowType window_type = WindowType::HANNING;
};

// 功率谱密度结果
struct PSDResult {
    std::vector<double> frequencies;
    std::vector<double> psd;
    std::vector<double> psd_db;
    double sampling_rate = 0.0;
    double frequency_resolution = 0.0;
    std::string method;
    std::map<std::string, double> parameters;
};

// 短时傅里叶变换结果
struct STFTResult {
    std::vector<std::vector<double>> magnitude;
    std::vector<std::vector<double>> phase;
    std::vector<double> time_axis;
    std::vector<double> frequency_axis;
    double time_resolution = 0.0;
    double frequency_resolution = 0.0;
    int window_size = 0;
    int hop_size = 0;
    WindowType window_type = WindowType::HANNING;
};

// 小波变换结果
struct WaveletResult {
    std::vector<std::vector<double>> coefficients;
    std::vector<double> scales;
    std::vector<double> frequencies;
    std::vector<double> time_axis;
    std::string wavelet_type;
    double sampling_rate = 0.0;
};

// 滤波器设计参数
struct FilterDesign {
    enum Type { LOW_PASS, HIGH_PASS, BAND_PASS, BAND_STOP };
    Type type = LOW_PASS;
    double cutoff_freq = 0.0;
    double low_freq = 0.0;
    double high_freq = 0.0;
    int order = 4;
    double ripple = 0.1;
    double attenuation = 60.0;
    std::string design_method = "butterworth";
};

// 滤波器系数
struct FilterCoefficients {
    std::vector<double> numerator;   // b coefficients
    std::vector<double> denominator; // a coefficients
    std::vector<double> poles;
    std::vector<double> zeros;
    double gain = 1.0;
};

// 频率响应
struct FrequencyResponse {
    std::vector<double> frequencies;
    std::vector<double> magnitude;
    std::vector<double> magnitude_db;
    std::vector<double> phase;
    std::vector<double> group_delay;
};

// 谱分析器类
class SpectralAnalyzer {
public:
    SpectralAnalyzer();
    ~SpectralAnalyzer();

    // FFT相关
    std::vector<Complex> fft(const std::vector<double>& signal);
    std::vector<Complex> fft(const std::vector<Complex>& signal);
    std::vector<Complex> ifft(const std::vector<Complex>& spectrum);
    std::vector<double> rfft(const std::vector<double>& signal);
    std::vector<double> irfft(const std::vector<double>& spectrum);

    // 频谱分析
    SpectrumResult computeSpectrum(const std::vector<double>& signal, 
                                 double sampling_rate,
                                 WindowType window = WindowType::HANNING,
                                 int fft_size = 0);
    
    PSDResult computePSD(const std::vector<double>& signal, 
                        double sampling_rate,
                        const std::string& method = "welch",
                        int nperseg = 0,
                        int noverlap = 0,
                        WindowType window = WindowType::HANNING);

    // 短时傅里叶变换
    STFTResult computeSTFT(const std::vector<double>& signal,
                          double sampling_rate,
                          int window_size = 256,
                          int hop_size = 0,
                          WindowType window = WindowType::HANNING);

    // 小波变换
    WaveletResult computeCWT(const std::vector<double>& signal,
                           double sampling_rate,
                           const std::string& wavelet = "morlet",
                           const std::vector<double>& scales = {});

    // 窗函数
    std::vector<double> generateWindow(WindowType type, int size, 
                                     double parameter = 0.0);
    std::vector<double> applyWindow(const std::vector<double>& signal, 
                                  WindowType window);

    // 滤波器设计
    FilterCoefficients designFilter(const FilterDesign& design, 
                                  double sampling_rate);
    std::vector<double> applyFilter(const std::vector<double>& signal, 
                                  const FilterCoefficients& filter);
    FrequencyResponse computeFrequencyResponse(const FilterCoefficients& filter,
                                             double sampling_rate,
                                             int n_points = 512);

    // 频域滤波
    std::vector<double> frequencyDomainFilter(const std::vector<double>& signal,
                                            double low_freq,
                                            double high_freq,
                                            double sampling_rate);

    // 谱特征提取
    double computeSpectralCentroid(const std::vector<double>& magnitude,
                                 const std::vector<double>& frequencies);
    double computeSpectralSpread(const std::vector<double>& magnitude,
                               const std::vector<double>& frequencies);
    double computeSpectralSkewness(const std::vector<double>& magnitude,
                                 const std::vector<double>& frequencies);
    double computeSpectralKurtosis(const std::vector<double>& magnitude,
                                 const std::vector<double>& frequencies);
    double computeSpectralRolloff(const std::vector<double>& magnitude,
                                const std::vector<double>& frequencies,
                                double rolloff_percent = 0.85);
    double computeSpectralFlatness(const std::vector<double>& magnitude);

    // 谱峰检测
    std::vector<int> findSpectralPeaks(const std::vector<double>& magnitude,
                                     double threshold = 0.1,
                                     int min_distance = 1);
    std::vector<double> getPeakFrequencies(const std::vector<int>& peak_indices,
                                         const std::vector<double>& frequencies);

    // 相干性分析
    std::vector<double> computeCoherence(const std::vector<double>& signal1,
                                       const std::vector<double>& signal2,
                                       double sampling_rate,
                                       int nperseg = 0);

    // 交叉功率谱
    std::vector<Complex> computeCrossSpectrum(const std::vector<double>& signal1,
                                            const std::vector<double>& signal2,
                                            double sampling_rate);

    // 谱图生成
    std::vector<std::vector<double>> computeSpectrogram(
        const std::vector<double>& signal,
        double sampling_rate,
        int window_size = 256,
        int hop_size = 0,
        WindowType window = WindowType::HANNING);

    // 倒谱分析
    std::vector<double> computeCepstrum(const std::vector<double>& signal);
    std::vector<double> computeRealCepstrum(const std::vector<double>& signal);

    // 希尔伯特变换
    std::vector<Complex> hilbertTransform(const std::vector<double>& signal);
    std::vector<double> computeInstantaneousAmplitude(const std::vector<double>& signal);
    std::vector<double> computeInstantaneousPhase(const std::vector<double>& signal);
    std::vector<double> computeInstantaneousFrequency(const std::vector<double>& signal,
                                                    double sampling_rate);

    // 谱减法降噪
    std::vector<double> spectralSubtraction(const std::vector<double>& noisy_signal,
                                          const std::vector<double>& noise_spectrum,
                                          double alpha = 2.0,
                                          double beta = 0.01);

    // 维纳滤波
    std::vector<double> wienerFilter(const std::vector<double>& noisy_signal,
                                   const std::vector<double>& noise_psd,
                                   const std::vector<double>& signal_psd);

    // 配置参数
    void setZeroPadding(bool enable) { zero_padding_ = enable; }
    void setFFTSize(int size) { fft_size_ = size; }
    void setOverlapRatio(double ratio) { overlap_ratio_ = ratio; }

private:
    class Impl;
    std::unique_ptr<Impl> pimpl_;
    
    bool zero_padding_ = true;
    int fft_size_ = 0;  // 0 means auto-determine
    double overlap_ratio_ = 0.5;
};

// 工具函数
namespace SpectralUtils {
    // FFT工具
    int nextPowerOfTwo(int n);
    std::vector<double> zeroPad(const std::vector<double>& signal, int target_size);
    std::vector<double> removeDC(const std::vector<double>& signal);
    
    // 频率轴生成
    std::vector<double> generateFrequencyAxis(int fft_size, double sampling_rate);
    std::vector<double> generateFrequencyAxisRFFT(int signal_size, double sampling_rate);
    
    // 单位转换
    std::vector<double> magnitudeToDecibels(const std::vector<double>& magnitude);
    std::vector<double> decibelsToMagnitude(const std::vector<double>& db);
    std::vector<double> powerToDecibels(const std::vector<double>& power);
    std::vector<double> decibelsToPower(const std::vector<double>& db);
    
    // 复数运算
    std::vector<double> complexMagnitude(const std::vector<Complex>& complex_data);
    std::vector<double> complexPhase(const std::vector<Complex>& complex_data);
    std::vector<double> complexPower(const std::vector<Complex>& complex_data);
    
    // 窗函数工具
    double windowNormalizationFactor(WindowType type, int size);
    double windowCoherentGain(WindowType type, int size);
    
    // 频率转换
    double hzToMel(double hz);
    double melToHz(double mel);
    std::vector<double> hzToMel(const std::vector<double>& hz);
    std::vector<double> melToHz(const std::vector<double>& mel);
    
    // 谱平滑
    std::vector<double> smoothSpectrum(const std::vector<double>& spectrum,
                                     int window_size = 3);
    std::vector<double> medianFilterSpectrum(const std::vector<double>& spectrum,
                                           int window_size = 3);
}

} // namespace Analysis

#endif // SPECTRAL_ANALYSIS_H