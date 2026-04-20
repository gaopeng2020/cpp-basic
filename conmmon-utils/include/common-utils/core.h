//
// Created by gaopeng on 2026/3/29.
//
#pragma once

#include <list>
#include <map>
#include <string>
#include <unordered_set>
#include "common-utils/common-utilsExport.h"

namespace common_utils::core {
/**
 * @brief 核心工具类，提供一些常用的工具函数
 *
 * 该类提供了一系列静态工具函数，主要用于：
 * - 字符串处理（格式化、修剪、分割等）
 * - 文件解析（INI 配置文件解析）
 * - 数据验证（C 变量名验证、数字格式验证）
 * - 系统路径操作（获取可执行文件目录、用户主目录等）
 * - 时间戳生成和格式化
 * - Excel 单元格地址转换
 *
 * @note 所有方法都是静态方法，可以直接通过类名调用
 */
class COMMON_UTILS_EXPORT Core {
public:
    // ... existing code ...
    /**
     * @brief 移除字符串中的所有空白字符（空格、制表符、换行符等）
     *
     * 该函数使用正则表达式匹配所有空白字符（\\s），包括：
     * - 空格 (' ')
     * - 制表符 ('\\t')
     * - 回车符 ('\\r')
     * - 换行符 ('\\n')
     * 并将它们全部移除。
     *
     * @param str 输入的原始字符串
     * @return std::string 移除所有空白字符后的字符串
     *
     * @note 如果输入为空字符串，则返回空字符串
     *
     * @code{.cpp}
     * stringFormat("Hello World") -> "HelloWorld"
     * stringFormat("a\tb\nc") -> "abc"
     * @endcode
     */
    static std::string stringFormat(const std::string& str);

    /**
     * @brief 去除字符串首尾的空白字符
     *
     * 该函数仅移除字符串开头和结尾的空白字符，保留字符串中间的空白字符。
     * 支持的空白字符类型：
     * - 空格 (' ')
     * - 制表符 ('\\t')
     * - 换行符 ('\\n')
     * - 回车符 ('\\r')
     *
     * @param str 输入的原始字符串
     * @return std::string 去除首尾空白后的字符串
     *
     * @note 如果输入为空字符串或全为空白字符，则返回空字符串
     *
     * @code{.cpp}
     * stringTrim("  hello world  ") -> "hello world"
     * stringTrim("\t\nabc def\r\n") -> "abc def"
     * @endcode
     */
    static std::string stringTrim(const std::string& str);

    /**
     * @brief 替换字符串中的指定子串
     *
     * 在输入字符串中查找所有出现的指定子串，并将其替换为新的字符串。
     * 该函数会处理所有匹配的位置，返回替换后的完整字符串。
     *
     * @param str 输入的原始字符串
     * @param o 待查找的子串
     * @param n 用于替换的新字符串
     * @return std::string 替换后的字符串
     *
     * @note 如果输入为空字符串，则返回空字符串
     *       该函数会替换所有匹配的子串，而非仅替换第一个
     *
     * @code{.cpp}
     * replace("hello world", "world", "C++") -> "hello C++"
     * replace("aaa", "a", "b") -> "bbb"
     * @endcode
     */
    static std::string stringReplace(const std::string& str, const std::string& o, const std::string& n);

    /**
     * @brief 按指定分隔符分割字符串
     *
     * 使用输入的分隔符将字符串分割成多个子字符串，并存储在 std::list 中返回。
     * 空的分隔结果会被自动过滤。
     *
     * @param str 待分割的原始字符串
     * @param delimiter 分隔符字符
     * @return std::list<std::string> 分割后的字符串列表
     *
     * @note 如果输入字符串为空或只包含分隔符，则返回空列表
     *       连续的分隔符会被视为一个分隔符处理（跳过空 token）
     *
     * @code{.cpp}
     * splitString("a,b,c", ',') -> {"a", "b", "c"}
     * splitString("hello world", ' ') -> {"hello", "world"}
     * splitString(",,a,,b,,", ',') -> {"a", "b"}
     * @endcode
     */
    static std::list<std::string> splitString(const std::string& str, char delimiter);

    /**
     * @brief 解析 INI 配置文件
     *
     * 读取并解析 INI 格式的配置文件，将配置项存储到 map 中返回。
     * 支持以下 INI 语法：
     * - 节（Section）：[section_name]
     * - 键值对（Key-Value）：key = value
     * - 注释：以 ';' 或 '#' 开头的行
     * - 空行会被自动忽略
     *
     * @param filePath INI 配置文件的路径
     * @return std::map<std::string, std::string> 解析后的配置映射
     *
     * @note 如果文件无法打开，则返回空 map 并记录错误日志
     *       带节的配置项会以 "section.key" 的形式存储
     *       不带节的配置项直接以 "key" 形式存储
     *       所有键值都会自动去除首尾空白字符
     *
     * @code{.cpp}
     * // 配置文件内容：
     * // [database]
     * // host = localhost
     * // port = 3306
     * // [app]
     * // name = MyApp
     * parseIniFile("config.ini") -> {
     *   "database.host": "localhost",
     *   "database.port": "3306",
     *   "app.name": "MyApp"
     * }
     * @endcode
     */
    static std::map<std::string, std::string> parseIniFile(const std::string& filePath);

    /**
     * @brief 检查字符串是否为有效的 C 语言变量名
     *
     * 验证规则：
     * 1. 不能为空字符串
     * 2. 不能是 C 语言关键字（C11 标准）
     * 3. 长度不能超过指定限制（默认 32 字符）
     * 4. 必须以大写字母开头
     * 5. 后续字符只能是字母、数字或下划线
     *
     * @param name 待检查的变量名字符串
     * @param length 变量名最大长度限制，默认为 32
     * @param isCapital 是否要求首字母大写,默认为 true
     * @return bool 如果是有效的 C 语言变量名返回 true，否则返回 false
     *
     * @note 该函数要求变量名首字母必须大写
     *
     * @code{.cpp}
     * isValidCVariableName("MyVar") -> true
     * isValidCVariableName("myVar") -> false (首字母小写)
     * isValidCVariableName("int") -> false (关键字)
     * isValidCVariableName("9Test") -> false (数字开头)
     * @endcode
     */
    static bool isValidCVariableName(const std::string& name, int length = 32, bool isCapital = true);

    /**
     * @brief 检查字符串是否表示一个有效的数字
     *
     * 支持以下数字格式：
     * - 整数：如 "123", "-456"
     * - 小数：如 "3.14", "-0.5", ".5"
     * - 科学计数法：如 "1e10", "2.5E-3", "-1.2e+5"
     *
     * @param str 待检查的字符串
     * @return bool 如果是有效的数字返回 true，否则返回 false
     *
     * @note 该函数会自动去除字符串首尾的空格后再进行匹配
     *
     * @code{.cpp}
     * isNumber("123") -> true
     * isNumber("-3.14") -> true
     * isNumber("1e10") -> true
     * isNumber("abc") -> false
     * isNumber("  123  ") -> true
     * @endcode
     */
    static bool isNumber(const std::string& str);

    /**
     * @brief 获取或设置最后一次打开的目录路径
     *
     * 该函数具有两种工作模式：
     * 1. **保存模式**（newPath 非空）：
     *    - 从 newPath 中提取目录路径（如果 newPath 是文件，则取其父目录）
     *    - 将目录路径保存到可执行文件所在目录的 conf/lastdir.txt 文件中
     *    - 返回规范化后的目录路径
     *
     * 2. **读取模式**（newPath 为空）：
     *    - 从 conf/lastdir.txt 文件中读取上次保存的目录路径
     *    - 如果文件不存在或读取失败，则返回用户主目录
     *
     * @param newPath 新的文件或目录路径，用于保存；空字符串表示读取模式
     * @return std::string 目录路径
     *
     * @note 如果出现错误（如路径无效、权限不足等），则返回用户主目录
     *       返回的路径会去除首尾空白字符
     *
     * @code{.cpp}
     * getLastOpenDir("") -> 读取上次的目录，若无记录则返回用户主目录
     * getLastOpenDir("C:/Users/test/file.txt") -> 保存并返回 "C:/Users/test"
     * getLastOpenDir("D:/Projects") -> 保存并返回 "D:/Projects"
     * @endcode
     */
    static std::string getLastOpenDir(const std::string& newPath = "");

    /**
     * @brief 获取当前可执行文件所在目录的完整路径
     *
     * 通过操作系统特定的 API 获取正在运行的可执行文件所在目录的绝对路径：
     * - Windows: 使用 GetModuleFileNameA API
     * - Linux/Unix: 读取 /proc/self/exe 符号链接
     *
     * @return std::string 可执行文件所在目录的完整路径
     *
     * @note 如果获取失败（如缓冲区不足、权限问题等），则返回空字符串
     *
     * @code{.cpp}
     * getExeDirectory() -> "C:\\Program Files\\MyApp" (Windows)
     * getExeDirectory() -> "/usr/bin" (Linux)
     * @endcode
     */
    static std::string getExeDirectory();

    /**
     * @brief 获取当前用户的主目录路径
     *
     * 通过读取环境变量获取用户主目录：
     * - Windows: 读取 USERPROFILE 环境变量
     * - Linux/Unix: 读取 HOME 环境变量
     *
     * @return std::string 用户主目录路径
     *
     * @note 如果环境变量未设置，则返回空字符串
     *
     * @code{.cpp}
     * getUserHome() -> "C:\\Users\\username" (Windows)
     * getUserHome() -> "/home/username" (Linux)
     * @endcode
     */
    static std::string getUserHome();

    /**
     * @brief 获取当前时间戳并格式化为字符串
     *
     * 根据指定的格式字符串获取当前系统时间，并可选择是否包含毫秒部分。
     *
     * @param format 时间格式化字符串，默认为 "%Y%m%d-%H%M"（如：20260402-1430）
     *               支持标准 strftime 格式占位符，如 %Y(年)、%m(月)、%d(日)、%H(时)、%M(分)、%S(秒) 等
     * @param ms 是否包含毫秒，默认为 false。为 true 时在返回值末尾添加 .xxx 毫秒部分
     * @return std::string 格式化后的当前时间戳字符串
     *
     * @code{.cpp}
     * getCurrentTimestamp() -> "20260402-1430"
     * getCurrentTimestamp("%Y-%m-%d %H:%M:%S") -> "2026-04-02 14:30:25"
     * getCurrentTimestamp("%Y%m%d-%H%M%S", true) -> "20260402-143025.123"
     * @endcode
     */
    static std::string getCurrentTimestamp(const std::string& format = "%Y%m%d-%H%M", bool ms = false);

    /**
     * @brief 将 double 类型数值转换为指定精度的字符串表示
     *
     * 该函数支持两种精度模式：
     * 1. **固定精度模式**（precision >= 0）：
     *    - 使用 std::fixed 和 std::setprecision 格式化输出
     *    - 保留指定的小数位数
     *
     * 2. **自动精度模式**（precision < 0，默认）：
     *    - 先使用 12 位精度进行转换
     *    - 自动去除末尾多余的零
     *    - 如果小数部分全为零，则去掉小数点
     *
     * @param d 要转换的双精度浮点数值
     * @param precision 小数位数，默认为 -1（自动判断）
     * @return std::string 格式化后的字符串
     *
     * @note 如果 precision 超过 double 类型的最大精度限制（max_digits10），则返回空字符串
     *
     * @code{.cpp}
     * doublePrecision(3.14159, 2) -> "3.14"
     * doublePrecision(2.5000) -> "2.5"
     * doublePrecision(100.0) -> "100"
     * doublePrecision(1.23e-4) -> "0.000123"
     * @endcode
     */
    static std::string doublePrecision(double d, int precision = -1);

    /**
     * @brief 将 Excel 列字母标识转换为数字
     *
     * 将 Excel 风格的列标识（如 A, B, ..., Z, AA, AB, ...）转换为对应的数字。
     * 转换规则：A=1, B=2, ..., Z=26, AA=27, AB=28, ...
     *
     * @param col 列字母标识字符串（不区分大小写）
     * @return int 对应的列数字
     *
     * @code{.cpp}
     * columnToNumber("A") -> 1
     * columnToNumber("Z") -> 26
     * columnToNumber("AA") -> 27
     * columnToNumber("AB") -> 28
     * @endcode
     */
    static int columnToNumber(const std::string& col);

    /**
     * @brief 将行号和列号转换为 Excel 单元格地址字符串
     *
     * 将行列数字转换为 Excel 风格的单元格地址（如 A1, B2, AA10 等）。
     *
     * @param row 行号（从 1 开始）
     * @param col 列号（从 1 开始）
     * @return std::string Excel 单元格地址字符串
     *
     * @note Excel 的有效范围：
     *       - 行号：[1, 1048576]
     *       - 列号：[1, 16384]
     *       如果超出范围，将记录错误日志并返回空字符串
     *
     * @code{.cpp}
     * numToCellAddress(1, 1) -> "A1"
     * numToCellAddress(2, 2) -> "B2"
     * numToCellAddress(10, 27) -> "AA10"
     * @endcode
     */
    static std::string numToCellAddress(int row, int col);

    /**
     * @brief 将 Excel 单元格地址字符串转换为行号和列号
     *
     * 将 Excel 风格的单元格地址（如 A1, B2, AA10 等）解析为行列数字对。
     *
     * @param addr Excel 单元格地址字符串
     * @return std::pair<int, int> 第一个元素为行号，第二个元素为列号
     *
     * @note 函数会分别提取地址中的字母部分（列）和数字部分（行）
     *
     * @code{.cpp}
     * cellAddressToNum("A1") -> {1, 1}
     * cellAddressToNum("B2") -> {2, 2}
     * cellAddressToNum("AA10") -> {10, 27}
     * @endcode
     */
    static std::pair<int, int> cellAddressToNum(const std::string& addr);

private:
    // C语言关键字集合（C11标准）,C++17 支持内联静态成员，直接在头文件中初始化
    inline static const std::unordered_set<std::string> C_KEYWORDS = {
        "auto",       "break",     "case",           "char",         "const",    "continue", "default",  "do",
        "double",     "else",      "enum",           "extern",       "float",    "for",      "goto",     "if",
        "inline",     "int",       "long",           "register",     "restrict", "return",   "short",    "signed",
        "sizeof",     "static",    "struct",         "switch",       "typedef",  "union",    "unsigned", "void",
        "volatile",   "while",     "_Alignas",       "_Alignof",     "_Atomic",  "_Bool",    "_Complex", "_Generic",
        "_Imaginary", "_Noreturn", "_Static_assert", "_Thread_local"};
};
} // namespace common_utils::core
