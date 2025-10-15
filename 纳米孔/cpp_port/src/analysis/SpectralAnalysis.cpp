#include "SpectralAnalysis.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <complex>
#include <functional>
#include <iostream>

// Ensure M_PI is available on MSVC where it's not defined by default
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Analysis {

// SpectralAnalyzer的PIMPL实现
class SpectralAnalyzer::Impl {
public:
    Impl() {}
    
    // 位反转排列
    void bitReversePermutation(std::vector<Complex>& data) {
        int n = data.size();
        int j = 0;
        
        for (int i = 1; i < n; ++i) {
            int bit = n >> 1;
            while (j & bit) {
                j ^= bit;
                bit >>= 1;
            }
            j ^= bit;
            
            if (i < j) {
                std::swap(data[i], data[j]);
            }
        }
    }
    
    // Cooley-Tukey FFT算法
    void fftCooleyTukey(std::vector<Complex>& data, bool inverse = false) {
        int n = data.size();
        if (n <= 1) return;
        
        // 检查是否为2的幂
        if ((n & (n - 1)) != 0) {
            throw std::invalid_argument("FFT size must be a power of 2");
        }
        
        bitReversePermutation(data);
        
        double sign = inverse ? 1.0 : -1.0;
        
        for (int len = 2; len <= n; len <<= 1) {
            double angle = sign * 2.0 * M_PI / len;
            Complex wlen(std::cos(angle), std::sin(angle));
            
            for (int i = 0; i < n; i += len) {
                Complex w(1.0, 0.0);
                
                for (int j = 0; j < len / 2; ++j) {
                    Complex u = data[i + j];
                    Complex v = data[i + j + len / 2] * w;
                    
                    data[i + j] = u + v;
                    data[i + j + len / 2] = u - v;
                    
                    w *= wlen;
                }
            }
        }
        
        if (inverse) {
            for (auto& x : data) {
                x /= n;
            }
        }
    }
    
    // 零填充到最近的2的幂
    int nextPowerOf2(int n) {
        int power = 1;
        while (power < n) {
            power <<= 1;
        }
        return power;
    }
    
    // 计算汉明窗
    std::vector<double> hammingWindow(int n) {
        std::vector<double> window(n);
        for (int i = 0; i < n; ++i) {
            window[i] = 0.54 - 0.46 * std::cos(2.0 * M_PI * i / (n - 1));
        }
        return window;
    }
    
    // 计算汉宁窗
    std::vector<double> hanningWindow(int n) {
        std::vector<double> window(n);
        for (int i = 0; i < n; ++i) {
            window[i] = 0.5 * (1.0 - std::cos(2.0 * M_PI * i / (n - 1)));
        }
        return window;
    }
    
    // 计算布莱克曼窗
    std::vector<double> blackmanWindow(int n) {
        std::vector<double> window(n);
        for (int i = 0; i < n; ++i) {
            double t = static_cast<double>(i) / (n - 1);
            window[i] = 0.42 - 0.5 * std::cos(2.0 * M_PI * t) + 
                       0.08 * std::cos(4.0 * M_PI * t);
        }
        return window;
    }
    
    // 计算矩形窗
    std::vector<double> rectangularWindow(int n) {
        return std::vector<double>(n, 1.0);
    }
    
    // 计算巴特利特窗
    std::vector<double> bartlettWindow(int n) {
        std::vector<double> window(n);
        for (int i = 0; i < n; ++i) {
            window[i] = 1.0 - 2.0 * std::abs(i - (n - 1) / 2.0) / (n - 1);
        }
        return window;
    }
    
    // 计算凯泽窗
    std::vector<double> kaiserWindow(int n, double beta) {
        std::vector<double> window(n);
        double i0_beta = modifiedBesselI0(beta);
        
        for (int i = 0; i < n; ++i) {
            double t = 2.0 * i / (n - 1) - 1.0;
            double arg = beta * std::sqrt(1.0 - t * t);
            window[i] = modifiedBesselI0(arg) / i0_beta;
        }
        return window;
    }
    
    // 修正贝塞尔函数I0
    double modifiedBesselI0(double x) {
        double sum = 1.0;
        double term = 1.0;
        double x_half = x / 2.0;
        
        for (int k = 1; k < 50; ++k) {
            term *= (x_half * x_half) / (k * k);
            sum += term;
            if (term < 1e-12) break;
        }
        
        return sum;
    }
    
    // 高斯窗
    std::vector<double> gaussianWindow(int n, double sigma) {
        std::vector<double> window(n);
        double center = (n - 1) / 2.0;
        
        for (int i = 0; i < n; ++i) {
            double t = (i - center) / (sigma * center);
            window[i] = std::exp(-0.5 * t * t);
        }
        return window;
    }
    
    // Tukey窗
    std::vector<double> tukeyWindow(int n, double alpha) {
        std::vector<double> window(n);
        int taper_length = static_cast<int>(alpha * (n - 1) / 2.0);
        
        for (int i = 0; i < n; ++i) {
            if (i < taper_length) {
                window[i] = 0.5 * (1.0 + std::cos(M_PI * (2.0 * i / (alpha * (n - 1)) - 1.0)));
            } else if (i >= n - taper_length) {
                window[i] = 0.5 * (1.0 + std::cos(M_PI * (2.0 * (i - n + 1) / (alpha * (n - 1)) + 1.0)));
            } else {
                window[i] = 1.0;
            }
        }
        return window;
    }
    
    // 低通滤波器设计
    std::vector<double> designLowPassFilter(double cutoff_freq, double sampling_rate, int filter_length) {
        std::vector<double> h(filter_length);
        int center = filter_length / 2;
        double wc = 2.0 * M_PI * cutoff_freq / sampling_rate;
        
        for (int n = 0; n < filter_length; ++n) {
            if (n == center) {
                h[n] = wc / M_PI;
            } else {
                double arg = wc * (n - center);
                h[n] = std::sin(arg) / (M_PI * (n - center));
            }
        }
        
        // 应用窗函数
        auto window = hammingWindow(filter_length);
        for (int i = 0; i < filter_length; ++i) {
            h[i] *= window[i];
        }
        
        return h;
    }
    
    // 卷积
    std::vector<double> convolve(const std::vector<double>& signal, 
                                const std::vector<double>& kernel) {
        int signal_len = signal.size();
        int kernel_len = kernel.size();
        int output_len = signal_len + kernel_len - 1;
        
        std::vector<double> output(output_len, 0.0);
        
        for (int i = 0; i < signal_len; ++i) {
            for (int j = 0; j < kernel_len; ++j) {
                output[i + j] += signal[i] * kernel[j];
            }
        }
        
        return output;
    }
};

SpectralAnalyzer::SpectralAnalyzer() : pimpl_(std::make_unique<Impl>()) {}

SpectralAnalyzer::~SpectralAnalyzer() = default;

std::vector<Complex> SpectralAnalyzer::fft(const std::vector<double>& signal) {
    if (signal.empty()) {
        return {};
    }
    
    int n = signal.size();
    if (zero_padding_) {
        n = pimpl_->nextPowerOf2(n);
    }
    
    std::vector<Complex> fft_data(n);
    
    // 复制信号数据
    for (size_t i = 0; i < signal.size(); ++i) {
        fft_data[i] = Complex(signal[i], 0.0);
    }
    
    // 零填充
    for (size_t i = signal.size(); i < n; ++i) {
        fft_data[i] = Complex(0.0, 0.0);
    }
    
    pimpl_->fftCooleyTukey(fft_data, false);
    
    return fft_data;
}

std::vector<Complex> SpectralAnalyzer::fft(const std::vector<Complex>& signal) {
    if (signal.empty()) {
        return {};
    }
    
    std::vector<Complex> fft_data = signal;
    
    int n = fft_data.size();
    if (zero_padding_) {
        n = pimpl_->nextPowerOf2(n);
        fft_data.resize(n, Complex(0.0, 0.0));
    }
    
    pimpl_->fftCooleyTukey(fft_data, false);
    
    return fft_data;
}

std::vector<Complex> SpectralAnalyzer::ifft(const std::vector<Complex>& spectrum) {
    if (spectrum.empty()) {
        return {};
    }
    
    std::vector<Complex> ifft_data = spectrum;
    pimpl_->fftCooleyTukey(ifft_data, true);
    
    return ifft_data;
}

std::vector<double> SpectralAnalyzer::rfft(const std::vector<double>& signal) {
    auto complex_fft = fft(signal);
    
    std::vector<double> real_fft;
    real_fft.reserve(complex_fft.size() / 2 + 1);
    
    for (size_t i = 0; i <= complex_fft.size() / 2; ++i) {
        real_fft.push_back(std::abs(complex_fft[i]));
    }
    
    return real_fft;
}

std::vector<double> SpectralAnalyzer::irfft(const std::vector<double>& spectrum) {
    // 重构复数频谱
    std::vector<Complex> complex_spectrum;
    complex_spectrum.reserve(2 * (spectrum.size() - 1));
    
    // 正频率部分
    for (size_t i = 0; i < spectrum.size(); ++i) {
        complex_spectrum.push_back(Complex(spectrum[i], 0.0));
    }
    
    // 负频率部分（共轭对称）
    for (int i = spectrum.size() - 2; i > 0; --i) {
        complex_spectrum.push_back(Complex(spectrum[i], 0.0));
    }
    
    auto complex_result = ifft(complex_spectrum);
    
    std::vector<double> result;
    result.reserve(complex_result.size());
    
    for (const auto& val : complex_result) {
        result.push_back(val.real());
    }
    
    return result;
}

SpectrumResult SpectralAnalyzer::computeSpectrum(
    const std::vector<double>& signal, 
    double sampling_rate,
    WindowType window,
    int fft_size) {
    
    SpectrumResult result;
    
    if (signal.empty()) {
        return result;
    }
    
    // 应用窗函数
    std::vector<double> windowed_signal = applyWindow(signal, window);
    
    // 计算FFT
    auto fft_result = fft(windowed_signal);
    
    int n = fft_result.size();
    int half_n = n / 2 + 1;
    
    result.frequencies.resize(half_n);
    result.magnitude.resize(half_n);
    result.phase.resize(half_n);
    result.power.resize(half_n);
    result.power_density.resize(half_n);
    
    double freq_resolution = sampling_rate / n;
    
    for (int i = 0; i < half_n; ++i) {
        result.frequencies[i] = i * freq_resolution;
        
        double real = fft_result[i].real();
        double imag = fft_result[i].imag();
        
        result.magnitude[i] = std::sqrt(real * real + imag * imag);
        result.phase[i] = std::atan2(imag, real);
        result.power[i] = result.magnitude[i] * result.magnitude[i];
        result.power_density[i] = result.power[i] / freq_resolution;
        
        // 归一化（除了DC和奈奎斯特频率）
        if (i > 0 && i < half_n - 1) {
            result.magnitude[i] *= 2.0 / n;
            result.power[i] *= 2.0 / n;
            result.power_density[i] *= 2.0 / n;
        } else {
            result.magnitude[i] /= n;
            result.power[i] /= n;
            result.power_density[i] /= n;
        }
    }
    
    result.sampling_rate = sampling_rate;
    result.frequency_resolution = freq_resolution;
    result.fft_size = n;
    result.window_type = window;
    
    return result;
}

PSDResult SpectralAnalyzer::computePSD(
    const std::vector<double>& signal, 
    double sampling_rate,
    const std::string& method,
    int nperseg,
    int noverlap,
    WindowType window) {
    
    PSDResult result;
    
    if (signal.empty()) {
        return result;
    }
    
    if (nperseg <= 0) {
        nperseg = std::min(256, static_cast<int>(signal.size()));
    }
    
    if (noverlap < 0) {
        noverlap = nperseg / 2;
    }
    
    int step = nperseg - noverlap;
    int n_segments = (signal.size() - noverlap) / step;
    
    if (n_segments <= 0) {
        return result;
    }
    
    // 计算第一个段来初始化结果
    std::vector<double> segment(signal.begin(), signal.begin() + nperseg);
    auto spectrum = computeSpectrum(segment, sampling_rate, window);
    
    result.frequencies = spectrum.frequencies;
    result.psd = spectrum.power;
    
    // 累加其他段
    for (int i = 1; i < n_segments; ++i) {
        int start = i * step;
        int end = start + nperseg;
        
        if (end > signal.size()) break;
        
        std::vector<double> current_segment(signal.begin() + start, signal.begin() + end);
        auto current_spectrum = computeSpectrum(current_segment, sampling_rate, window);
        
        for (size_t j = 0; j < result.psd.size(); ++j) {
            result.psd[j] += current_spectrum.power[j];
        }
    }
    
    // 平均化
    for (auto& val : result.psd) {
        val /= n_segments;
    }
    
    // 转换为dB
    result.psd_db.resize(result.psd.size());
    for (size_t i = 0; i < result.psd.size(); ++i) {
        result.psd_db[i] = 10.0 * std::log10(result.psd[i] + 1e-12);
    }
    
    result.sampling_rate = sampling_rate;
    result.frequency_resolution = sampling_rate / nperseg;
    result.method = method;
    result.parameters["nperseg"] = nperseg;
    result.parameters["noverlap"] = noverlap;
    
    return result;
}

STFTResult SpectralAnalyzer::computeSTFT(
    const std::vector<double>& signal,
    double sampling_rate,
    int window_size,
    int hop_size,
    WindowType window) {
    
    STFTResult result;
    
    if (signal.empty()) {
        return result;
    }
    
    if (hop_size <= 0) {
        hop_size = window_size / 2;
    }
    
    int n_frames = (signal.size() - window_size) / hop_size + 1;
    
    if (n_frames <= 0) {
        return result;
    }
    
    // 计算频率轴
    int n_freqs = window_size / 2 + 1;
    result.frequency_axis.resize(n_freqs);
    double freq_resolution = sampling_rate / window_size;
    
    for (int i = 0; i < n_freqs; ++i) {
        result.frequency_axis[i] = i * freq_resolution;
    }
    
    // 计算时间轴
    result.time_axis.resize(n_frames);
    for (int i = 0; i < n_frames; ++i) {
        result.time_axis[i] = (i * hop_size + window_size / 2.0) / sampling_rate;
    }
    
    // 初始化谱图矩阵
    result.magnitude.resize(n_freqs);
    result.phase.resize(n_freqs);
    for (auto& row : result.magnitude) {
        row.resize(n_frames);
    }
    for (auto& row : result.phase) {
        row.resize(n_frames);
    }
    
    // 计算每个时间段的频谱
    for (int i = 0; i < n_frames; ++i) {
        int start = i * hop_size;
        int end = start + window_size;
        
        if (end > signal.size()) break;
        
        std::vector<double> segment(signal.begin() + start, signal.begin() + end);
        auto spectrum = computeSpectrum(segment, sampling_rate, window);
        
        for (int j = 0; j < n_freqs; ++j) {
            result.magnitude[j][i] = spectrum.magnitude[j];
            result.phase[j][i] = spectrum.phase[j];
        }
    }
    
    result.time_resolution = static_cast<double>(hop_size) / sampling_rate;
    result.frequency_resolution = freq_resolution;
    result.window_size = window_size;
    result.hop_size = hop_size;
    result.window_type = window;
    
    return result;
}

std::vector<double> SpectralAnalyzer::generateWindow(WindowType type, int size, double parameter) {
    switch (type) {
        case WindowType::RECTANGULAR:
            return pimpl_->rectangularWindow(size);
        case WindowType::HAMMING:
            return pimpl_->hammingWindow(size);
        case WindowType::HANNING:
            return pimpl_->hanningWindow(size);
        case WindowType::BLACKMAN:
            return pimpl_->blackmanWindow(size);
        case WindowType::BARTLETT:
            return pimpl_->bartlettWindow(size);
        case WindowType::KAISER:
            return pimpl_->kaiserWindow(size, parameter > 0 ? parameter : 8.6);
        case WindowType::GAUSSIAN:
            return pimpl_->gaussianWindow(size, parameter > 0 ? parameter : 0.4);
        case WindowType::TUKEY:
            return pimpl_->tukeyWindow(size, parameter > 0 ? parameter : 0.5);
        default:
            return pimpl_->hanningWindow(size);
    }
}

std::vector<double> SpectralAnalyzer::applyWindow(
    const std::vector<double>& signal, WindowType window) {
    
    auto window_func = generateWindow(window, signal.size());
    
    std::vector<double> windowed_signal(signal.size());
    for (size_t i = 0; i < signal.size(); ++i) {
        windowed_signal[i] = signal[i] * window_func[i];
    }
    
    return windowed_signal;
}

// SpectralUtils命名空间的实现
namespace SpectralUtils {

int nextPowerOfTwo(int n) {
    int power = 1;
    while (power < n) {
        power <<= 1;
    }
    return power;
}

std::vector<double> zeroPad(const std::vector<double>& signal, int target_size) {
    std::vector<double> padded_signal = signal;
    padded_signal.resize(target_size, 0.0);
    return padded_signal;
}

std::vector<double> removeDC(const std::vector<double>& signal) {
    if (signal.empty()) return {};
    
    double mean = std::accumulate(signal.begin(), signal.end(), 0.0) / signal.size();
    
    std::vector<double> dc_removed(signal.size());
    for (size_t i = 0; i < signal.size(); ++i) {
        dc_removed[i] = signal[i] - mean;
    }
    
    return dc_removed;
}

std::vector<double> generateFrequencyAxis(int fft_size, double sampling_rate) {
    std::vector<double> frequencies(fft_size);
    double freq_resolution = sampling_rate / fft_size;
    
    for (int i = 0; i < fft_size; ++i) {
        if (i <= fft_size / 2) {
            frequencies[i] = i * freq_resolution;
        } else {
            frequencies[i] = (i - fft_size) * freq_resolution;
        }
    }
    
    return frequencies;
}

std::vector<double> generateFrequencyAxisRFFT(int signal_size, double sampling_rate) {
    int n_freqs = signal_size / 2 + 1;
    std::vector<double> frequencies(n_freqs);
    double freq_resolution = sampling_rate / signal_size;
    
    for (int i = 0; i < n_freqs; ++i) {
        frequencies[i] = i * freq_resolution;
    }
    
    return frequencies;
}

std::vector<double> magnitudeToDecibels(const std::vector<double>& magnitude) {
    std::vector<double> db(magnitude.size());
    for (size_t i = 0; i < magnitude.size(); ++i) {
        db[i] = 20.0 * std::log10(magnitude[i] + 1e-12);
    }
    return db;
}

std::vector<double> decibelsToMagnitude(const std::vector<double>& db) {
    std::vector<double> magnitude(db.size());
    for (size_t i = 0; i < db.size(); ++i) {
        magnitude[i] = std::pow(10.0, db[i] / 20.0);
    }
    return magnitude;
}

std::vector<double> powerToDecibels(const std::vector<double>& power) {
    std::vector<double> db(power.size());
    for (size_t i = 0; i < power.size(); ++i) {
        db[i] = 10.0 * std::log10(power[i] + 1e-12);
    }
    return db;
}

std::vector<double> decibelsToPower(const std::vector<double>& db) {
    std::vector<double> power(db.size());
    for (size_t i = 0; i < db.size(); ++i) {
        power[i] = std::pow(10.0, db[i] / 10.0);
    }
    return power;
}

std::vector<double> complexMagnitude(const std::vector<Complex>& complex_data) {
    std::vector<double> magnitude(complex_data.size());
    for (size_t i = 0; i < complex_data.size(); ++i) {
        magnitude[i] = std::abs(complex_data[i]);
    }
    return magnitude;
}

std::vector<double> complexPhase(const std::vector<Complex>& complex_data) {
    std::vector<double> phase(complex_data.size());
    for (size_t i = 0; i < complex_data.size(); ++i) {
        phase[i] = std::arg(complex_data[i]);
    }
    return phase;
}

std::vector<double> complexPower(const std::vector<Complex>& complex_data) {
    std::vector<double> power(complex_data.size());
    for (size_t i = 0; i < complex_data.size(); ++i) {
        double magnitude = std::abs(complex_data[i]);
        power[i] = magnitude * magnitude;
    }
    return power;
}

double hzToMel(double hz) {
    return 2595.0 * std::log10(1.0 + hz / 700.0);
}

double melToHz(double mel) {
    return 700.0 * (std::pow(10.0, mel / 2595.0) - 1.0);
}

std::vector<double> hzToMel(const std::vector<double>& hz) {
    std::vector<double> mel(hz.size());
    for (size_t i = 0; i < hz.size(); ++i) {
        mel[i] = hzToMel(hz[i]);
    }
    return mel;
}

std::vector<double> melToHz(const std::vector<double>& mel) {
    std::vector<double> hz(mel.size());
    for (size_t i = 0; i < mel.size(); ++i) {
        hz[i] = melToHz(mel[i]);
    }
    return hz;
}

std::vector<double> smoothSpectrum(const std::vector<double>& spectrum, int window_size) {
    if (spectrum.empty() || window_size <= 0) {
        return spectrum;
    }
    
    std::vector<double> smoothed(spectrum.size());
    int half_window = window_size / 2;
    
    for (size_t i = 0; i < spectrum.size(); ++i) {
        double sum = 0.0;
        int count = 0;
        
        int start = std::max(0, static_cast<int>(i) - half_window);
        int end = std::min(static_cast<int>(spectrum.size()), static_cast<int>(i) + half_window + 1);
        
        for (int j = start; j < end; ++j) {
            sum += spectrum[j];
            count++;
        }
        
        smoothed[i] = count > 0 ? sum / count : spectrum[i];
    }
    
    return smoothed;
}

std::vector<double> medianFilterSpectrum(const std::vector<double>& spectrum, int window_size) {
    if (spectrum.empty() || window_size <= 0) {
        return spectrum;
    }
    
    std::vector<double> filtered(spectrum.size());
    int half_window = window_size / 2;
    
    for (size_t i = 0; i < spectrum.size(); ++i) {
        std::vector<double> window_values;
        
        int start = std::max(0, static_cast<int>(i) - half_window);
        int end = std::min(static_cast<int>(spectrum.size()), static_cast<int>(i) + half_window + 1);
        
        for (int j = start; j < end; ++j) {
            window_values.push_back(spectrum[j]);
        }
        
        std::sort(window_values.begin(), window_values.end());
        filtered[i] = window_values[window_values.size() / 2];
    }
    
    return filtered;
}

} // namespace SpectralUtils

} // namespace Analysis