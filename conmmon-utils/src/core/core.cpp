//
// Created by gaopeng on 2026/3/29.
//

#include "common-utils/core.h"
#include <glog/logging.h>
#include <regex>
#include <filesystem>
#include <algorithm>
#include <fstream>
#ifdef _WIN32
#include <windows.h>
#endif

namespace common_utils::core{
    std::string Core::stringFormat(const std::string& str) {
        if (str.empty()) return "";
        // 第一步：先替换字面量的 "\n"、"\t"、"\r"（来自配置文件或代码中的字符串字面量）
        std::string result = str;
        size_t pos = 0;
        while ((pos = result.find("\\n", pos)) != std::string::npos) {
            result.replace(pos, 2, "");
        }
        pos = 0;
        while ((pos = result.find("\\t", pos)) != std::string::npos) {
            result.replace(pos, 2, "");
        }
        pos = 0;
        while ((pos = result.find("\\r", pos)) != std::string::npos) {
            result.replace(pos, 2, "");
        }
        // 正则表达式匹配所有空白字符：\s 包含空格、制表符(\t)、回车(\r)、换行(\n)
        const std::regex rex("\\s");
        return std::regex_replace(result, rex, "");
    }

    std::string Core::stringTrim(const std::string& str) {
        if (str.empty()) return "";
        const size_t start = str.find_first_not_of(" \t\n\r");
        if (start == std::string::npos) return ""; // 全是空白字符

        const size_t end = str.find_last_not_of(" \t\n\r");
        std::string substr = str.substr(start, end - start + 1);
        // LOG(INFO)<<"substr= "<<substr;
        // 既然 start != npos，说明有非空白字符，那么 end 也必然 != npos
        return substr;
    }

    std::list<std::string> Core::splitString(const std::string& str, const char delimiter) {
        std::list<std::string> result;
        std::istringstream iss(str);
        std::string token;
        while (std::getline(iss, token, delimiter)) {
            if (!token.empty()) {
                result.push_back(token);
            }
        }
        return result;
    }

    std::map<std::string, std::string> Core::parseIniFile(const std::string& filePath) {
        std::map<std::string, std::string> config;
        std::ifstream file(filePath);

        if (!file.is_open()) {
            LOG(ERROR) << "Cannot open config file: " << filePath;
            return config;
        }

        std::string line;
        std::string currentSection;

        while (std::getline(file, line)) {
            line = stringTrim(line);

            if (line.empty() || line[0] == ';' || line[0] == '#') {
                continue;
            }

            if (line[0] == '[' && line.back() == ']') {
                currentSection = line.substr(1, line.length() - 2);
                continue;
            }

            if (const size_t pos = line.find('='); pos != std::string::npos) {
                std::string key = stringTrim(line.substr(0, pos));
                const std::string value = stringTrim(line.substr(pos + 1));

                if (!currentSection.empty()) {
                    config[currentSection + "." + key] = value;
                } else {
                    config[key] = value;
                }
            }
        }

        file.close();
        return config;
    }

    bool Core::isValidCVariableName(const std::string& name, const int length) {
        // 1. 空字符串直接返回false
        if (name.empty()) return false;

        // 2. 检查是否为C语言关键字
        if (C_KEYWORDS.contains(name)) return false;

        // 3. 检查名称长度
        if (name.length() > static_cast<size_t>(length)) return false;

        // 4. 正则表达式匹配：首字符必须为大写字母，后续为字母、数字或下划线
        const std::regex pattern("^[A-Z][a-zA-Z0-9_]*$");
        return std::regex_match(name, pattern);
    }


    bool Core::isNumber(const std::string& str) {
        if (str.empty()) return false;

        // 正则表达式匹配数字（整数、小数、负数、科学计数法）
        const std::regex rex(R"(^[+-]?(\d+(\.\d*)?|\.\d+)([eE][+-]?\d+)?$)");

        // 去除首尾空格后匹配
        const std::string trimmed = std::regex_replace(str, std::regex("^\\s+|\\s+$"), "");
        return std::regex_match(trimmed, rex);
    }

    std::string Core::getExeDirectory() {
        char exePath[4096];
#ifdef _WIN32
        if (const DWORD ret = GetModuleFileNameA(nullptr, exePath, sizeof(exePath));
            ret == 0 || ret >= sizeof(exePath))
            return "";
#else
        ssize_t len = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
        if (len <= 0) return "";
        // 确保字符串正确终止，标记字符串结束
        exePath[len] = '\0';
#endif
        // 去掉 exe 文件名，只保留目录路径
        std::filesystem::path path(exePath);
        return path.parent_path().string();
    }

    std::string Core::getUserHome() {
#ifdef _WIN32
        const char* homeEnv = std::getenv("USERPROFILE");
#else
        const char* homeEnv = std::getenv("HOME");
#endif
        return homeEnv ? homeEnv : "";
    }

    /*
     *默认的紧凑格式20250115-1430 "%Y%m%d-%H%M"
     *常规格式2025-01-15-14-30："%Y-%m-%d-%H-%M"
     *中文格式2025 年 01 月 15 日 14 时 30 分 "%Y年%m月%d日 %H时%M分"
     */
    std::string Core::getCurrentTimestamp(const std::string& format, const bool ms) {
        // 获取从纪元（1970-01-01）到现在的秒数（time_t 类型）
        const std::time_t now = std::time(nullptr);
        const std::tm* localTime = std::localtime(&now);

        char buffer[64];
        std::strftime(buffer, sizeof(buffer), format.c_str(), localTime);

        if (!ms) {
            return std::string(buffer);
        } else {
            // std::chrono::system_clock理论上可以获取到纳秒，通过std::chrono::duration_cast转换成毫秒
            const auto now_ms = std::chrono::system_clock::now();
            const auto ms1 = std::chrono::duration_cast<std::chrono::milliseconds>(
                now_ms.time_since_epoch()).count() % 1000; //模运算，获取毫秒数（1s=1000ms）

            // 在末尾添加毫秒，std::setfill('0') 用于设置填充字符，通常与 std::setw() 配合使用，当输出内容宽度不足时，用 '0' 填充剩余空间。
            std::ostringstream oss;
            oss << buffer << "." << std::setfill('0') << std::setw(4) << ms1;
            return oss.str();
        }
    }

    std::string Core::getLastOpenDir(const std::string& newPath) {
        // 获取 exe 所在路径 exePath，以及用户主目录 home
        std::string exeDir = getExeDirectory();
        std::string user_home = getUserHome();

        //获取或创建conf文件夹
        std::filesystem::path confDir = std::filesystem::path(exeDir) / "conf";
        std::error_code ec;
        std::filesystem::create_directories(confDir, ec);
        if (ec) return user_home;

        //LastDir.txt路径
        std::string dir;
        std::filesystem::path filePath = confDir / "lastdir.txt";
        // 保存模式
        if (!newPath.empty()) {
            //从输入的inputPath中提取文件所在目录
            if (std::filesystem::path inputPath(newPath); std::filesystem::is_regular_file(inputPath, ec)) {
                dir = inputPath.parent_path().string();
            } else if (std::filesystem::is_directory(inputPath, ec)) {
                dir = inputPath.string();
            }
            //确认dir路径是否存在
            dir = std::filesystem::weakly_canonical(dir, ec).string();
            if (ec) return user_home;

            //将dir路径写入LastDir.txt
            if (std::ofstream file(filePath.string()); file.is_open()) {
                file << dir;
                file.close();
            }
            return dir;
        }
        // 读取模式
        bool exists = std::filesystem::exists(filePath, ec);
        if (std::ifstream file(filePath.string()); exists && !ec && file.is_open()) {
            std::getline(file, dir);
            file.close();
            // 去除首尾空格
            return stringTrim(dir);
        }

        return user_home;
    }

    std::string Core::doublePrecision(const double d, const int precision) {
        if (precision > std::numeric_limits<double>::max_digits10) {
            LOG(ERROR) << "Invalid precision: " << precision
                << ", maximum allowed is " << std::numeric_limits<double>::max_digits10;
            return "";
        }
        std::ostringstream oss;
        // 指定精度
        if (precision >= 0) {
            oss << std::fixed << std::setprecision(precision) << d;
            return oss.str();
        }
        // 自动判断小数位数,先用 12 位精度转换
        oss << std::setprecision(12) << d;
        std::string str = oss.str();

        // 去除末尾多余的 0, 先找到小数点位置，且保证位置有效
        if (const size_t pointIndex = str.find('.'); pointIndex != std::string::npos) {
            // 找到小数点后最后一个非 0 字符的位置，且保证位置有效
            if (const size_t lastNonZeroIndex = str.find_last_not_of('0');
                lastNonZeroIndex != std::string::npos && lastNonZeroIndex > pointIndex) {
                str = str.substr(0, lastNonZeroIndex + 1); // 截取到该位置
            } else if (lastNonZeroIndex == pointIndex) {
                // 如果小数点后全是 0（如 "2.000"）则 去掉小数点
                str = str.substr(0, pointIndex);
            }
        }
        return str;
    }

    int Core::columnToNumber(const std::string& col) {
        int result = 0;
        for (const char c : col) {
            result = result * 26 + (toupper(c) - 'A' + 1);
        }
        return result;
    }

    std::pair<int, int> Core::cellAddressToNum(const std::string& addr) {
        std::string col, row;
        for (const char c : addr) {
            if (isalpha(c)) col += c;
            else if (isdigit(c)) row += c;
        }
        return {std::stoi(row), columnToNumber(col)};
    }

    std::string Core::numToCellAddress(const int row, int col) {
        if (row < 1 || row >= 1048576 || col < 1 || col >= 16384) {
            LOG(ERROR) << "rowNumber valid range [1;1048576], columnNumber validrange [1;16384]"
                "and the input is: row= " << row << ", column= " << col;
            return "";
        }

        std::string result;
        while (col > 0) {
            // 将列号转换为0-indexed以便计算
            col--; // 因为A对应1，但模运算需要0对应A
            const int remainder = col % 26;
            result.push_back('A' + remainder);
            col /= 26;
        }
        // 上述循环得到的字母顺序是从低位到高位，需要反转
        std::ranges::reverse(result);
        return result + std::to_string(row);
    }
} // namespace common_utils::core
