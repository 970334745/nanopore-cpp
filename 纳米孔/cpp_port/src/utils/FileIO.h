#pragma once
#include <string>
#include <vector>

namespace FileIO {

// 简单TXT读取：两列（x y）或单列（y），x按采样生成
bool readTxt(const std::string& path, std::vector<double>& x, std::vector<double>& y);

}