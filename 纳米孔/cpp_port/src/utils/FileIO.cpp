#include "FileIO.h"
#include <fstream>
#include <sstream>

namespace FileIO {

bool readTxt(const std::string& path, std::vector<double>& x, std::vector<double>& y) {
    std::ifstream in(path);
    if (!in.is_open()) return false;
    x.clear(); y.clear();
    std::string line;
    int idx = 0;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        double a, b;
        if (iss >> a) {
            if (iss >> b) {
                x.push_back(a);
                y.push_back(b);
            } else {
                x.push_back(idx * 1.0); // 占位的时间索引
                y.push_back(a);
            }
            ++idx;
        }
    }
    return !y.empty();
}

}