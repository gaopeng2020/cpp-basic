//
// Created by gaopeng on 2026/3/22.
//

#include "DataTypeConsistencyCheck.h"
#include <chrono>
#include <cmath>
#include <common-utils/core.h>
#include <common-utils/xlsx.h>
#include <filesystem>
#include <fstream>
#include <regex>
#include <sstream>
#include "common-utils/Log.hpp"

using namespace common_utils::core;
using namespace common_utils::xlsx;
using namespace common_utils::log;

namespace {
enum class compuMethodType { TEXT_TABLE, BIT_FIELD, LINEAR, IDENTICAL, NOT_SUPPORTED, ERROR };

bool isUnusedCell(std::string value) {
    value = Core::stringTrim(value);
    if (value.empty() || value == "/" || value == "\\") {
        return true;
    }
    return false;
}

bool tryParseDouble(const std::string& str, double& result) {
    try {
        size_t pos;
        result = std::stod(str, &pos);
        return pos == str.length();
    } catch (...) {
        return false;
    }
}

compuMethodType getCompuMethodType(const std::string& valueTable, const std::string& factor, const std::string& offset) {
    // 去除空格
    const std::string vt = Core::stringTrim(valueTable);
    const std::string f = Core::stringTrim(factor);
    const std::string o = Core::stringTrim(offset);

    // 静态正则表达式
    static const std::regex textTablePattern("^0x[0-9a-f]{1,2}[=:：]\\s*", std::regex_constants::icase);
    static const std::regex bitFieldPattern("^bit[0-9]{1,2}[=:：]\\s*", std::regex_constants::icase);

    const bool vtIsEmpty = isUnusedCell(vt);
    const bool isTextTableFormat = std::regex_search(vt, textTablePattern);
    const bool isBitFieldFormat = std::regex_search(vt, bitFieldPattern);

    // 解析 factor 和 offset
    double factorVal, offsetVal;
    const bool factorOk = tryParseDouble(f, factorVal);
    const bool offsetOk = tryParseDouble(o, offsetVal);
    const bool factorIsOne = factorOk && std::abs(factorVal - 1.0) < 1e-12;
    const bool offsetIsZero = offsetOk && std::abs(offsetVal) < 1e-12;

    // 规则 5: valueTable 满足 1 或 2，但 factor!=1 || offset!=0 但必须是数字 → NOT_SUPPORTED
    if ((isTextTableFormat || isBitFieldFormat) && factorOk && offsetOk && (!factorIsOne || !offsetIsZero)) {
        return compuMethodType::NOT_SUPPORTED;
    }

    // 规则 1: valueTable 以 0x 开头，紧跟=或: → TEXT_TABLE
    if (isTextTableFormat) {
        return compuMethodType::TEXT_TABLE;
    }

    // 规则 2: valueTable 以 Bit 开头，紧跟=或: → BIT_FIELD
    if (isBitFieldFormat) {
        return compuMethodType::BIT_FIELD;
    }

    // 如果 factor 或 offset 不是有效数字，后面的规则无法判断，返回 ERROR
    if (!factorOk || !offsetOk) {
        return compuMethodType::ERROR;
    }

    // 规则 3: valueTable 为空 & factor=1 & offset=0 → IDENTICAL
    if (vtIsEmpty && factorIsOne && offsetIsZero) {
        return compuMethodType::IDENTICAL;
    }

    // 规则 4: valueTable 为空但 (factor!=1 || offset!=0) → LINEAR
    if (vtIsEmpty && (!factorIsOne || !offsetIsZero)) {
        return compuMethodType::LINEAR;
    }

    // 默认情况返回 ERROR
    return compuMethodType::ERROR;
}
std::string compuMethodTypeToString(const compuMethodType type) {
    switch (type) {
        case compuMethodType::TEXT_TABLE:
            return "TEXT_TABLE";
        case compuMethodType::BIT_FIELD:
            return "BIT_FIELD";
        case compuMethodType::LINEAR:
            return "LINEAR";
        case compuMethodType::IDENTICAL:
            return "IDENTICAL";
        case compuMethodType::NOT_SUPPORTED:
            return "NOT_SUPPORTED";
        case compuMethodType::ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}
std::list<int> extractNumbers(const std::string& input, const std::string& prefix) {
    std::list<int> numbers;
    std::string tmp = Core::stringReplace(input, "~", "-");

    const bool isHex = (prefix == "0x" || prefix == "0X");
    const int base = isHex ? 16 : 10;

    const std::string numPattern = (base == 16) ? "[0-9A-Fa-f]+" : (prefix.empty()) ? "[01]{1,3}" : "\\d+";

    // 整个模式 = 第一部分 + (?: 分隔符 + 第二部分 )?
    //        ↑          ↑                    ↑
    //      必有      非捕获分组             整体可选
    // "(?:[-]" + // 非捕获分组开始以及连字符
    //  "(?:" + prefix + ")?" + // prefix 非必选
    //  "(" + numPattern + ")" + // 数字
    //   ")?" + 非捕获分组结束
    const std::string patternStr =
        prefix + "(" + numPattern + ")" + "(?:[-]" + "(?:" + prefix + ")?" + "(" + numPattern + ")" + ")?";

    const std::regex regex(patternStr, std::regex_constants::icase);

    auto searchStart = tmp.cbegin();
    std::smatch match;

    // 使用 regex_search 循环，更可控
    while (std::regex_search(searchStart, tmp.cend(), match, regex)) {
        if (match.empty()) {
            break; // 防止无限循环
        }
        int first = std::stoi(match[1].str(), nullptr, base);
        if (match[2].matched) {
            const int second = std::stoi(match[2].str(), nullptr, base);
            for (int i = first; i <= second; ++i) {
                numbers.push_back(i);
            }
        } else {
            numbers.push_back(first);
        }
        searchStart = match[0].second; // 移动到匹配结束位置
    }

    numbers.sort();
    numbers.unique();
    return numbers;
}

void checkDuplicatedNums(const std::list<int>& numbers, std::list<std::string>& errors, const std::string& cellAddress,
                         const std::string& prefix = "") {
    std::set<int> seen;
    for (int value : numbers) {
        if (seen.contains(value)) {
            std::string errorMsg = "[Error] ";
            errorMsg += cellAddress;
            errorMsg += ": ";
            errorMsg += prefix;
            errorMsg += ": 发现重复的值：";
            errorMsg += std::to_string(value);
            errors.emplace_back(errorMsg);
        }
        seen.insert(value);
    }
}

void checkIncontinuityNums(const std::list<int>& numbers, std::list<std::string>& errors, const std::string& cellAddress,
                           const std::string& prefix = "") {
    if (numbers.empty()) return;
    int expected = numbers.front();
    for (const int value : numbers) {
        if (value != expected) {
            std::string errorMsg = "[Error] ";
            errorMsg += cellAddress;
            errorMsg += ": ";
            errorMsg += prefix;
            errorMsg += ": 发现不连续的值：";
            errorMsg += std::to_string(value);
            errorMsg += "，期望值为：";
            errorMsg += std::to_string(expected);
            errors.emplace_back(std::move(errorMsg));
        }
        expected++;
    }
}

std::list<std::string> extractBitFieldVariable(const std::list<std::string>& bitFieldVars) {
    std::list<std::string> result;
    // 最简单的匹配：分隔符后面跟着非分隔符字符
    static const std::regex regex("[=:：][^=:：]+");
    for (const auto& line : bitFieldVars) {
        const auto begin = std::sregex_iterator(line.begin(), line.end(), regex);
        auto end = std::sregex_iterator();
        for (auto it = begin; it != end; ++it) {
            std::string captured = it->str();
            // 去掉开头的分隔符
            if (!captured.empty() && (captured[0] == '=' || captured[0] == ':' || captured[0] == ':')) {
                captured = captured.substr(1);
            }
            captured = Core::stringTrim(captured);
            if (!captured.empty()) {
                result.push_back(captured);
            }
        }
    }

    return result;
}

void checkBitFieldInside(const std::list<std::string>& symbols, std::list<std::string>& errors, const std::string& cellAddress,
                         const std::string& label) {
    // 检查括号内的位域值定义 (例如：00=no, 01=yes) 是否为二进制
    std::list<int> list;
    for (const auto& varDef : symbols) {
        std::string trimmed = Core::stringTrim(varDef);
        if (trimmed.empty()) continue;

        // 使用正则提取开头的二进制部分，支持 1~3 位二进制
        static const std::regex binRegex("^([01]{1,3})\\s*[=:：]\\s*(.+)$");

        if (std::smatch match; !std::regex_match(trimmed, match, binRegex)) {
            auto msg = "[Error] " + cellAddress;
            msg += ": 位域值定义格式错误：'";
            msg += trimmed;
            msg += "'（应为：二进制值=变量名，如 00=no）";
            errors.emplace_back(msg);
        } else {
            try {
                int value = std::stoi(match.str(1), nullptr, 2); // 使用二进制转换
                list.push_back(value); // 对于临时变量，使用 push_back 代替
                                       // emplace，否则 emplace 可能会构造成 0
            } catch (...) {
                errors.emplace_back("[Error] " + cellAddress + ": 二进制值转换失败：'" + match.str(1) + "'");
            }
        }
    }
    // 检查位域值是否重复
    list.sort();
    checkDuplicatedNums(list, errors, cellAddress, label);
    // 检查位域值是否连续
    checkIncontinuityNums(list, errors, cellAddress, label);
}
} // namespace

DataTypeConsistencyCheck::DataTypeConsistencyCheck() :
    dataTypeNameCol(-1),
    categoryCol(-1),
    memPosiCol(-1),
    memNameCol(-1),
    memTypeRefCol(-1),
    lenTypeCol(-1),
    minLenCol(-1),
    maxLenCol(-1),
    baseTypeCol(-1),
    sigLenCol(-1),
    factorCol(-1),
    offsetCol(-1),
    minPhyCol(-1),
    maxPhyCol(-1),
    unitCol(-1),
    tableCol(-1) /*, textEdit(nullptr)*/ {}

DataTypeConsistencyCheck::DataTypeConsistencyCheck(std::string& filePath) :
    dataTypeNameCol(-1),
    categoryCol(-1),
    memPosiCol(-1),
    memNameCol(-1),
    memTypeRefCol(-1),
    lenTypeCol(-1),
    minLenCol(-1),
    maxLenCol(-1),
    baseTypeCol(-1),
    sigLenCol(-1),
    factorCol(-1),
    offsetCol(-1),
    minPhyCol(-1),
    maxPhyCol(-1),
    unitCol(-1),
    tableCol(-1),
    filePath(std::move(filePath)) /*, textEdit(textEdit)*/ {
    readDataType();
}

void DataTypeConsistencyCheck::readDataType() {
    errors.clear();
    OpenXLSX::XLDocument doc;
    if (!Xlsx::openXlDocument(doc, filePath)) {
        log_error("Adt", "Failed to open xlsx file: " + filePath);
        return;
    }

    auto sheet = doc.workbook().worksheet(1);
    auto sheetName = "[Info] Active Worksheet : " + sheet.name();
    log_info("Adt", sheetName);
    errors.emplace_back(sheetName);

    std::string configPath = Core::getExeDirectory() + "/conf/header_config.ini";
    log_info("Adt", "config Path is: " + configPath);

    auto settings = Core::parseIniFile(configPath);
    if (settings.empty()) {
        log_error("Adt", "Configuration file not found or empty:" + configPath);
        return;
    }

    dataTypeNameCol = Xlsx::getColumnByHeader(sheet, settings["AdtColumnHeaders.DataTypeName"]);
    categoryCol = Xlsx::getColumnByHeader(sheet, settings["AdtColumnHeaders.DataTypeCategory"]);

    memPosiCol = Xlsx::getColumnByHeader(sheet, settings["AdtColumnHeaders.MemberPosition"]);
    memNameCol = Xlsx::getColumnByHeader(sheet, settings["AdtColumnHeaders.MemberName"]);
    memTypeRefCol = Xlsx::getColumnByHeader(sheet, settings["AdtColumnHeaders.MemberDataTypeReference"]);

    lenTypeCol = Xlsx::getColumnByHeader(sheet, settings["AdtColumnHeaders.StringLengthType"]);
    minLenCol = Xlsx::getColumnByHeader(sheet, settings["AdtColumnHeaders.StringLengthMin"]);
    maxLenCol = Xlsx::getColumnByHeader(sheet, settings["AdtColumnHeaders.StringLengthMax"]);

    baseTypeCol = Xlsx::getColumnByHeader(sheet, settings["AdtColumnHeaders.BasicDataType"]);
    sigLenCol = Xlsx::getColumnByHeader(sheet, settings["AdtColumnHeaders.SignalLength"]);
    factorCol = Xlsx::getColumnByHeader(sheet, settings["AdtColumnHeaders.Resolution"]);
    offsetCol = Xlsx::getColumnByHeader(sheet, settings["AdtColumnHeaders.Offset"]);
    minPhyCol = Xlsx::getColumnByHeader(sheet, settings["AdtColumnHeaders.MinPhysicalValue"]);
    maxPhyCol = Xlsx::getColumnByHeader(sheet, settings["AdtColumnHeaders.MaxPhysicalValue"]);
    unitCol = Xlsx::getColumnByHeader(sheet, settings["AdtColumnHeaders.Unit"]);
    tableCol = Xlsx::getColumnByHeader(sheet, settings["AdtColumnHeaders.TableValue"]);

    // 检查输入的表格是否有效
    if (dataTypeNameCol == -1 || categoryCol == -1 || memPosiCol == -1 || memNameCol == -1 || memTypeRefCol == -1 ||
        lenTypeCol == -1 || minLenCol == -1 || maxLenCol == -1 || baseTypeCol == -1 || sigLenCol == -1 || factorCol == -1 ||
        offsetCol == -1 || minPhyCol == -1 || maxPhyCol == -1 || unitCol == -1 || tableCol == -1) {
        const auto msg = "[Error] 输入文件无效，请检查输入的 Excel 是否为 Application Data Type "
                         "表格，且不能修改模板的表头，自定义的 ADT 必须放在第一个 sheet 中";
        // LOG(ERROR) << msg;
        log_error("Adt", msg);
        errors.emplace_back(msg);
        // textEdit->append(msg);
        return;
    }
    // 提示开始检查
    const auto msg = filePath + " 正在执行检查，请等待...";
    // LOG(INFO) << msg;
    log_info("Adt", msg);

    // 获取所有的 data name
    for (int row = 2; row <= sheet.rowCount(); row++) {
        auto name = Xlsx::getCellValue(sheet.cell(row, dataTypeNameCol));
        if (name.empty()) continue;
        dataTypeNames.emplace(name);
    }

    checkDataType(sheet);
}

void DataTypeConsistencyCheck::checkDataType(const OpenXLSX::XLWorksheet& sheet) {
    // 检查data type是否已存在
    std::string libXlsx;
    const std::string confDir = Core::getExeDirectory() + "/conf";
    for (const auto& entry : std::filesystem::directory_iterator(confDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".xlsx") {
            libXlsx = entry.path().string();
            break;
        }
    }
    if (libXlsx.empty()) {
        errors.emplace_back(
            "[Error] 在conf文件夹下未发现excel文件，请从X平台导出最新的Adt清单并放在conf目录，文件名称随意，但后缀必须是xlsx");
    }
    errors.emplace_back("[Info] 请确保conf目录下的excel文件是最新的，否则无法准确识别数据类型是否与库中已有的Adt重复");
    errors.emplace_back("-----------------------------------------------------------------------------------------------");
    std::set<std::string> adtLib;
    if (OpenXLSX::XLDocument doc; Xlsx::openXlDocument(doc, libXlsx)) {
        log_error("Adt", "Failed to open lib xlsx file: " + libXlsx);
        const auto libSheet = doc.workbook().worksheet(1);
        if (Xlsx::getCellValue(libSheet.cell(1, 1)) != "ADT Name")
            errors.emplace_back(
                "[Error] conf目录下的excel文件第一个sheet的第一个单元格必须是 ADT Name, 请确认是否从X平台导出来的ADT清单");
        for (int i = 2; i <= libSheet.rowCount(); i++) {
            adtLib.emplace(Xlsx::getCellValue(libSheet.cell(i, 1)));
        }
    }
    //开始数据类型的检查
    int row = 2;
    log_debug("Adt", "[Info] sheet.rowCount()..." << sheet.rowCount());
    while (row <= sheet.rowCount()) {
        const int endRow = Xlsx::getLastRowNum(const_cast<OpenXLSX::XLWorksheet&>(sheet), row, dataTypeNameCol);

        // data type name check
        auto name = Xlsx::getCellValue(sheet.cell(row, dataTypeNameCol));
        if (name.empty()) {
            // log_debug("Adt", "Skipping empty row " << row << ", next row=" << (endRow + 1));
            row = endRow + 1;
            continue;
        }

        if (!Core::isValidCVariableName(name)) {
            auto msg = "[Error] " + Core::numToCellAddress(row, dataTypeNameCol);
            msg += ": ";
            msg += name;
            msg += ": 数据类型名称不符合要求（大写字母开头，只能包含字母、数字、下划线，长度不超过 32 个字符）";
            errors.emplace_back(msg);
        }
        // 判断是否与库中的Adt重名
        if (adtLib.contains(name)) {
            errors.emplace_back("[Error] " + Core::numToCellAddress(row, dataTypeNameCol) + ": " + name +
                                " 在X平台存在与该数据类型重名的Adt");
        }
        // 检查是否填写了category，以及category的类别是否为Struct, Array, 以及value
        if (auto category = Xlsx::getCellValue(sheet.cell(row, categoryCol)); category == "Value") {
            valueConsistencyCheck(sheet, row);
        } else if (category == "Struct") {
            recordConsistencyCheck(sheet, row, endRow);
        } else if (category == "Array") {
            arrayConsistencyCheck(sheet, row);
        } else {
            errors.emplace_back("[Error] " + Core::numToCellAddress(row, categoryCol) + ": " + category +
                                ": 数据类型类别填写错误，只能是 Struct, Array, Value");
        }
        row = endRow + 1;
    }
}

/*=======================================Value check===========================================*/

void DataTypeConsistencyCheck::valueConsistencyCheck(const OpenXLSX::XLWorksheet& sheet, const int row) {
    // log_debug("Adt", "checkMustDefinedCell called for row " << row);
    // 无需填写的单元格检查（不适用 Value）
    std::string result;
    if (!isUnusedCell(Xlsx::getCellValue(sheet.cell(row, memPosiCol))))
        errors.emplace_back("[Error] " + Core::numToCellAddress(row, memPosiCol) + " value类型此处无需填写或用/或\\填充");

    if (!isUnusedCell(Xlsx::getCellValue(sheet.cell(row, memNameCol))))
        errors.emplace_back("[Error] " + Core::numToCellAddress(row, memNameCol) + " value类型此处无需填写或用/或\\填充");

    if (!isUnusedCell(Xlsx::getCellValue(sheet.cell(row, memTypeRefCol))))
        errors.emplace_back("[Error] " + Core::numToCellAddress(row, memTypeRefCol) + " value类型此处无需填写或用/或\\填充");

    if (!isUnusedCell(Xlsx::getCellValue(sheet.cell(row, lenTypeCol))))
        errors.emplace_back("[Error] " + Core::numToCellAddress(row, lenTypeCol) + " value类型此处无需填写或用/或\\填充");

    if (!isUnusedCell(Xlsx::getCellValue(sheet.cell(row, minLenCol))))
        errors.emplace_back("[Error] " + Core::numToCellAddress(row, minLenCol) + " value类型此处无需填写或用/或\\填充");

    if (!isUnusedCell(Xlsx::getCellValue(sheet.cell(row, maxLenCol))))
        errors.emplace_back("[Error] " + Core::numToCellAddress(row, maxLenCol) + " value类型此处无需填写或用/或\\填充");
    // 必须填写的单元格检查
    checkMustDefinedCell(sheet, row);
}

void DataTypeConsistencyCheck::checkMustDefinedCell(const OpenXLSX::XLWorksheet& sheet, const int row) {
    const auto baseType = Xlsx::getCellValue(sheet.cell(row, baseTypeCol));
    if (!BASIC_TYPES.contains(baseType))
        errors.emplace_back("[Error] " + Core::numToCellAddress(row, baseTypeCol) +
                            " 基础数据类型不在支持的范围内，仅支持"
                            "{bool,uint8,uint16,uint32,uint64,sint8,"
                            "sint16,sint32,sint64,float32,float64,\\}");

    bool isSignal;
    const std::string sigLenStr = Xlsx::getCellValue(sheet.cell(row, sigLenCol));
    int sigLen = 0;
    try {
        sigLen = std::stoi(sigLenStr, nullptr, 10);
        isSignal = true;
    } catch (...) {
        isSignal = false;
    }

    if (!isSignal) errors.emplace_back("[Error] " + Core::numToCellAddress(row, sigLenCol) + " Value类型，信号长度必须填写");

    auto valueTable = Xlsx::getCellValue(sheet.cell(row, tableCol));
    valueTable = Core::stringReplace(valueTable, "：", ":");
    const auto factor = Xlsx::getCellValue(sheet.cell(row, factorCol));
    const auto offset = Xlsx::getCellValue(sheet.cell(row, offsetCol));

    const auto minPhy = Xlsx::getCellValue(sheet.cell(row, minPhyCol));
    const auto maxPhy = Xlsx::getCellValue(sheet.cell(row, maxPhyCol));
    const auto unit = Xlsx::getCellValue(sheet.cell(row, unitCol));

    const auto tableCellAddr = Core::numToCellAddress(row, tableCol);

    const auto methodType = getCompuMethodType(valueTable, factor, offset);
    log_debug("Adt", "Row " << row << " methodType=" << compuMethodTypeToString(methodType));
    if (methodType == compuMethodType::TEXT_TABLE || methodType == compuMethodType::BIT_FIELD) {
        if (!isUnusedCell(factor))
            errors.emplace_back("[Warning] " + Core::numToCellAddress(row, factorCol) +
                                ": 枚举或位域，此处建议保持为空，或者填写/或\\");

        if (!isUnusedCell(factor))
            errors.emplace_back("[Warning] " + Core::numToCellAddress(row, offsetCol) +
                                ": 枚举或位域，此处建议保持为空，或者填写/或\\");

        if (!isUnusedCell(factor))
            errors.emplace_back("[Warning] " + Core::numToCellAddress(row, minPhyCol) +
                                ": 枚举或位域，此处建议保持为空，或者填写/或\\");

        if (!isUnusedCell(factor))
            errors.emplace_back("[Warning] " + Core::numToCellAddress(row, maxPhyCol) +
                                ": 枚举或位域，此处建议保持为空，或者填写/或\\");

        if (!isUnusedCell(factor))
            errors.emplace_back("[Warning] " + Core::numToCellAddress(row, unitCol) +
                                ": 枚举或位域，此处建议保持为空，或者填写/或\\");

        // 枚举和位域校验
        int rawMax;
        if (methodType == compuMethodType::TEXT_TABLE) rawMax = checkValueTable(valueTable, tableCellAddr);

        if (methodType == compuMethodType::BIT_FIELD)
            rawMax = static_cast<int>(std::pow(2, checkBitField(valueTable, tableCellAddr) - 1));

        if (isSignal && rawMax != -1) {
            // checkSignalLength(rawMax, sigLen, row);
            checkSignalLength(rawMax, sigLen, baseType, row);
        }
    } else if (methodType == compuMethodType::IDENTICAL || methodType == compuMethodType::LINEAR) {
        log_debug("Adt", std::to_string(row) + " " + compuMethodTypeToString(methodType));
        bool isMinNum, isMaxNum, isFactor, isOffset;
        double min = 0, max = 0, f = 1, o = 0;

        try {
            min = std::stod(minPhy);
            isMinNum = true;
        } catch (...) {
            isMinNum = false;
        }

        try {
            max = std::stod(maxPhy);
            isMaxNum = true;
        } catch (...) {
            isMaxNum = false;
        }

        try {
            f = std::stod(factor);
            isFactor = true;
        } catch (...) {
            isFactor = false;
        }

        try {
            o = std::stod(offset);
            isOffset = true;
        } catch (...) {
            isOffset = false;
        }

        if (!isMinNum)
            errors.emplace_back("[Error] " + Core::numToCellAddress(row, minPhyCol) +
                                " Value 类型如果没有枚举或位域，物理最小值必须填写数字");
        if (!isMaxNum) {
            errors.emplace_back("[Error] " + Core::numToCellAddress(row, maxPhyCol) +
                                " Value 类型如果没有枚举或位域，物理最大值必须填写数字");
        }
        if (!isFactor) {
            errors.emplace_back("[Error] " + Core::numToCellAddress(row, factorCol) +
                                " Value 类型如果没有枚举或位域，resolution 必须填写数字");
        }
        if (!isOffset) {
            errors.emplace_back("[Error] " + Core::numToCellAddress(row, offsetCol) +
                                " Value 类型如果没有枚举或位域，offset 必须填写数字");
        }
        if (isMaxNum && isMinNum && max <= min)
            errors.emplace_back("[Error] " + Core::numToCellAddress(row, maxPhyCol) + " 物理最大值应大于物理最小值>");
        if (isFactor && isOffset && isMaxNum && isSignal) {
            // phy to raw value (phy = raw*factor + offset)
            const double rawMax = (max - o) / f;
            log_debug("ADT", Core::numToCellAddress(row, maxPhyCol) + " maxValue: " + std::to_string(rawMax));
            // checkSignalLength(rawMax, sigLen, row);
            checkSignalLength(static_cast<int64_t>(rawMax), sigLen, baseType, row);
        }
    } else if (methodType == compuMethodType::NOT_SUPPORTED) {
        errors.emplace_back("[Error] " + tableCellAddr +
                            " 不支持的类型，valueTable 中有枚举或位域的定义，但 "
                            "factor!=1 或 offset!=0");
    }
}

int DataTypeConsistencyCheck::checkValueTable(const std::string& valueTable, const std::string& cellAddress) {
    const std::list<int> numbers = extractNumbers(valueTable, "0x");
    // 检查枚举值是否有重复
    checkDuplicatedNums(numbers, errors, cellAddress);
    // 检查枚举值是否连续，比如 1，2，3，4 不能 1，3，4
    checkIncontinuityNums(numbers, errors, cellAddress);

    // 分隔符正则：等号、英文冒号、中文冒号
    const std::list<std::string> list = Core::splitString(valueTable, '\n');
    static const std::regex separatorRegex("[=:：]");
    int lineNumber = 0;
    for (const auto& str : list) {
        lineNumber++;
        std::string trimmedStr = Core::stringTrim(str);

        // 跳过空行
        if (trimmedStr.empty()) continue;

        // 查找分隔符
        if (std::smatch match; std::regex_search(trimmedStr, match, separatorRegex)) {
            const size_t separatorPos = match.position();
            std::string enumName = Core::stringTrim(trimmedStr.substr(separatorPos + 1));

            // 检查是否以 0x 开头
            if (trimmedStr.size() < 2 || !(trimmedStr[0] == '0' && (trimmedStr[1] == 'x' || trimmedStr[1] == 'X'))) {
                auto msg = "[Error] " + cellAddress;
                msg += ": 第";
                msg += std::to_string(lineNumber);
                msg += "行：'";
                msg += enumName;
                msg += "' 没有以 0x 开头（枚举值必须是十六进制，以 0x 开头）";
                errors.emplace_back(msg);
            }

            // 检查是否符合 C 变量命名规范
            if (!Core::isValidCVariableName(Core::stringFormat(enumName), 16, false)) {
                std::string errorMsg = "[Error] " + cellAddress;
                errorMsg += ": 第";
                errorMsg += std::to_string(lineNumber);
                errorMsg += "行：'";
                errorMsg += enumName;
                errorMsg += "' "
                            "枚举名称不符合要求（大写字母开头，只能包含字母、数字、下划"
                            "线，长度不超过 16 个字符";
                if (enumName.size() > 16) {
                    errorMsg += ",实际:" + std::to_string(enumName.size());
                }
                errorMsg += ")";
                errors.emplace_back(errorMsg);
            }
        }
    }
    return numbers.empty() ? -1 : numbers.back();
}

int DataTypeConsistencyCheck::checkBitField(const std::string& valueTable, const std::string& cellAddress) {
    std::list<int> numbers = extractNumbers(valueTable, "Bit");
    // 检查位域值是否有重复
    checkDuplicatedNums(numbers, errors, cellAddress);
    // 检查位域值是否连续，比如 1，2，3，4 不能 1，3，4
    checkIncontinuityNums(numbers, errors, cellAddress);

    std::list<std::string> list = Core::splitString(valueTable, '\n');
    int lineNumber = 1;
    for (const auto& str : list) {
        std::string trimmedStr = Core::stringTrim(str);

        // 跳过空行
        if (trimmedStr.empty()) continue;

        // 检查是否以 Bit 开头
        if (trimmedStr.size() < 3 || !(trimmedStr[0] == 'B' || trimmedStr[0] == 'b') ||
            !(trimmedStr[1] == 'I' || trimmedStr[1] == 'i') || !(trimmedStr[2] == 'T' || trimmedStr[2] == 't')) {
            auto msg = "[Error] " + cellAddress;
            msg += ": 第";
            msg += std::to_string(lineNumber);
            msg += "行：'";
            msg += trimmedStr;
            msg += "'位域必须以 Bit/bit 开头，如:Bit1:front_right(00=no,01=yes)）";
            errors.emplace_back(msg);
        }

        // 检查是否包含括号
        trimmedStr = Core::stringReplace(trimmedStr, "（", "(");
        trimmedStr = Core::stringReplace(trimmedStr, "）", ")");
        if (const size_t index = trimmedStr.find('('); index == std::string::npos) {
            auto msg = "[Error] " + cellAddress;
            msg += ": 第";
            msg += std::to_string(lineNumber);
            msg += "行：'";
            msg += trimmedStr;
            msg += "' 没有找到左括号（位域定义必须有英文括号，如 Bit1:front_right(00=no,01=yes)）";
            errors.emplace_back(msg);
        } else {
            // 检查Label是否符合 C 变量命名规范
            std::string label;
            static const std::regex varNameRegex(R"([=:：]\s*([^\(]+)\s*\()");
            if (std::smatch match; std::regex_search(trimmedStr, match, varNameRegex)) {
                if (label = Core::stringTrim(match.str(1));
                    !label.empty() && !Core::isValidCVariableName(Core::stringFormat(label), 16, false)) {
                    std::string errorMsg = "[Error] " + cellAddress;
                    errorMsg += ": 第";
                    errorMsg += std::to_string(lineNumber);
                    errorMsg += "行：'";
                    errorMsg += label;
                    errorMsg += "' Label名称不符合要求（大写字母开头，只能包含字母、数字、下划线，长度不超过 16 个字符";

                    if (label.size() > 16) {
                        errorMsg += ",实际:" + std::to_string(label.size());
                    }
                    errorMsg += ")";
                    errors.emplace_back(errorMsg);
                }
            }
            // 检查括号内部的symbol是否符合 C 命名规范，以及symbol的值是否为二进制，是否连续，是否重复
            std::string variable = trimmedStr.substr(index + 1);
            if (size_t closeParenPos = variable.find(')'); closeParenPos != std::string::npos) {
                variable = variable.substr(0, closeParenPos);
            }
            variable = Core::stringReplace(variable, "，", ",");
            std::list<std::string> symbols = Core::splitString(variable, ',');

            // 检查括号内变量symbol是否符合 C 变量命名规范
            for (std::list<std::string> variables = extractBitFieldVariable(symbols); const auto& var : variables) {
                if (!Core::isValidCVariableName(Core::stringFormat(var), 16, false)) {
                    std::string errorMsg = "[Error] " + cellAddress;
                    errorMsg += ": 第";
                    errorMsg += std::to_string(lineNumber);
                    errorMsg += "行：'";
                    errorMsg += var;
                    errorMsg += "' 括号内定义的名称不符合要求（大写字母开头，只能包含字母、数字、下划线，长度不超过 16 个字符";

                    if (var.size() > 16) {
                        errorMsg += ",实际:" + std::to_string(var.size());
                    }
                    errorMsg += ")";
                    errors.emplace_back(errorMsg);
                }
            }
            // symbol的值是否为二进制，是否连续，是否重复
            checkBitFieldInside(symbols, errors, cellAddress, label);
        }

        lineNumber++;
    }
    return numbers.empty() ? -1 : numbers.back();
}

void DataTypeConsistencyCheck::checkSignalLength(const double maxValue, const int len, const int row) {
    if (pow(2, len) - 1 < maxValue)
        errors.emplace_back("[Error] " + Core::numToCellAddress(row, sigLenCol) + ": " + " 信号长度太短或无效，最小应该为：" +
                            std::to_string(static_cast<int>(ceil(log2(maxValue + 1)))));
}

void DataTypeConsistencyCheck::checkSignalLength(int64_t maxValue, const int length, const std::string& baseType, const int row) {
    // 1. 长度太短（直接用 log2 比较）
    if (const double requiredLen = std::log2(maxValue + 1); length < requiredLen) {
        int recommendLen = static_cast<int>(std::ceil(requiredLen));
        errors.emplace_back(
            std::format("[Error] {}: 信号长度太短或无效，最小应该为：{}", Core::numToCellAddress(row, sigLenCol), recommendLen));
    }

    // 4. 长度太长（同样使用 log2 比较）
    int optimalLen = length;
    while (optimalLen > 0 && static_cast<int64_t>(std::pow(2, optimalLen - 1) - 1) >= maxValue) {
        optimalLen--;
    }
    if (optimalLen < length) {
        errors.emplace_back(std::format("[Warning] {}: 当前信号长度太长[{}], 有点浪费总线带宽，建议修改为：{}",
                                        Core::numToCellAddress(row, sigLenCol),length, optimalLen));
    }

    // 类型定义...
    std::vector<TypeInfo> types = {{"uint8", 0, UINT8_MAX, false},         {"uint16", 0, UINT16_MAX, false},
                                   {"uint32", 0, UINT32_MAX, false},       {"uint64", 0, INT64_MAX, false},
                                   {"sint8", INT8_MIN, INT8_MAX, true},    {"sint16", INT16_MIN, INT16_MAX, true},
                                   {"sint32", INT32_MIN, INT32_MAX, true}, {"sint64", INT64_MIN, INT64_MAX, true}};

    const auto it = std::ranges::find_if(types, [&baseType](const TypeInfo& t) { return t.name == baseType; });
    if (it == types.end()) return;
    const TypeInfo& currentType = *it;

    // 2. maxValue超出baseType取值范围
    if (maxValue < currentType.minVal || maxValue > currentType.maxVal) {
        errors.emplace_back(std::format("[Error] {}: maxvalue {} 超过 baseType {} 的取值范围 [{}, {}]",
                                        Core::numToCellAddress(row, baseTypeCol), maxValue, baseType, currentType.minVal,
                                        currentType.maxVal));
    }

    // 3. 选用的base type类型过大
    for (const auto& [name, minVal, maxVal, isSigned] : types) {
        if (name == baseType) continue;
        if (isSigned != currentType.isSigned) continue;
        const bool isNarrower =
            !currentType.isSigned ? currentType.maxVal > maxVal : currentType.minVal < minVal && currentType.maxVal > maxVal;
        if (isNarrower && maxValue >= minVal && maxValue <= maxVal) {
            errors.emplace_back(
                std::format("[Warning] {}: 请确认 base type 是否合理，使用 {} 理论上可以满足 maxvalue = {} 的要求",
                            Core::numToCellAddress(row, baseTypeCol), name, maxValue));
            break;
        }
    }
}

/*=========================================Struct check=====================================*/
void DataTypeConsistencyCheck::recordConsistencyCheck(const OpenXLSX::XLWorksheet& sheet, const int startRow, const int endRow) {
    std::list<int> numbers;
    for (int row = startRow; row <= endRow; row++) {
        // 检查成员名称检查
        if (const auto memName = Xlsx::getCellValue(sheet.cell(row, memNameCol)); !Core::isValidCVariableName(memName)) {
            errors.emplace_back("[Error] " + Core::numToCellAddress(row, memNameCol) + ": " + memName +
                                " 结构体成员名称不符合要求（大写字母开头，只能包含字"
                                "母、数字、下划线，长度不超过 32 个字符）");
        }
        // 检查成员序号
        const auto memPosi = Xlsx::getCellValue(sheet.cell(row, memPosiCol));
        bool ok;
        int posiValue = 0;
        try {
            posiValue = std::stoi(memPosi);
            ok = true;
        } catch (...) {
            ok = false;
        }

        if (!ok || posiValue < 0) {
            errors.emplace_back("[Error] " + Core::numToCellAddress(row, memPosiCol) + ": " + memPosi +
                                ": 结构体成员位置必须是正整数");
        } else {
            numbers.push_back(posiValue);
        }
        // 无需填写的单元格检查（不适用 Struct）
        if (!isUnusedCell(Xlsx::getCellValue(sheet.cell(row, lenTypeCol))))
            errors.emplace_back("[Error] " + Core::numToCellAddress(row, lenTypeCol) +
                                ": 不适用于结构体，此处应保持为空，或者填写/或\\");

        if (!isUnusedCell(Xlsx::getCellValue(sheet.cell(row, minLenCol))))
            errors.emplace_back("[Error] " + Core::numToCellAddress(row, minLenCol) +
                                ": 不适用于结构体，此处应保持为空，或者填写/或\\");

        if (!isUnusedCell(Xlsx::getCellValue(sheet.cell(row, maxLenCol))))
            errors.emplace_back("[Error] " + Core::numToCellAddress(row, maxLenCol) +
                                ": 不适用于结构体，此处应保持为空，或者填写/或\\");

        // 直接在成员所在行定义结构体成员数据类型
        if (const std::string baseType = Xlsx::getCellValue(sheet.cell(row, baseTypeCol)); !isUnusedCell(baseType)) {
            if (!isUnusedCell(Xlsx::getCellValue(sheet.cell(row, memTypeRefCol))))
                errors.emplace_back("[Error] " + Core::numToCellAddress(row, memTypeRefCol) +
                                    ": 结构体成员数据类型直接在本行定义时，此处应保持为空，或者填写/或\\");
            checkMustDefinedCell(sheet, row);

        } else { // 引用数据类型
            // 检查引用是否有定义
            const std::string memTypeRef = Core::stringTrim(Xlsx::getCellValue(sheet.cell(row, memTypeRefCol)));
            if (memTypeRef.find("CRC") == std::string::npos ||
                memTypeRef.find("RollgCntr") == std::string::npos && !dataTypeNames.contains(memTypeRef))
                errors.emplace_back("[Error] " + Core::numToCellAddress(row, memTypeRefCol) + ": " + memTypeRef +
                                    " 结构体成员引用数据类型未在表格中定义");
            //检查primitive type不使用的单元格
            unusedCellForRefType(sheet, row);
        }
    }

    // 检查结构体成员位置是否有重复，不连续
    const std::string msg = Xlsx::getCellValue(sheet.cell(startRow, dataTypeNameCol)) + ": 结构体成员";
    checkDuplicatedNums(numbers, errors, msg);
    checkIncontinuityNums(numbers, errors, msg);
}

/*======================================Array check============================================*/
void DataTypeConsistencyCheck::arrayConsistencyCheck(const OpenXLSX::XLWorksheet& sheet, const int row) {
    // 检查数组类型
    if (const std::string arrayType = Xlsx::getCellValue(sheet.cell(row, lenTypeCol)); arrayType != "Fixed") {
        errors.emplace_back("[Error]" + Core::numToCellAddress(row, lenTypeCol) +
                            " 仅支持定长数组，数组类长度型必须为 Fixed，实际为：" + arrayType);
    }

    // 数组长度最小值，必须是数字
    const auto minnLenCellAddr = Core::numToCellAddress(row, minLenCol);
    const std::string arrayMinLen = Xlsx::getCellValue(sheet.cell(row, minLenCol));
    bool ok;
    int minLen = 0;
    try {
        minLen = std::stoi(arrayMinLen);
        ok = true;
    } catch (...) {
        ok = false;
    }

    if (!ok || minLen < 0) {
        errors.emplace_back("[Error] " + minnLenCellAddr + " 定长数组类长度最小值必须为正整数，实际为：" + arrayMinLen);
    }
    // 数组长度最大值，必须是数字
    const auto maxLenCellAddr = Core::numToCellAddress(row, maxLenCol);
    const std::string arrayMaxLen = Xlsx::getCellValue(sheet.cell(row, maxLenCol));
    ok = false;
    int maxLen = 0;
    try {
        maxLen = std::stoi(arrayMaxLen);
        ok = true;
    } catch (...) {
        ok = false;
    }

    if (!ok || maxLen < 0) {
        errors.emplace_back("[Error] " + maxLenCellAddr + " 定长数组类长度最大值必须为正整数，实际为：" + arrayMaxLen);
    }
    if (maxLen != minLen) errors.emplace_back("[Error] " + maxLenCellAddr + " 定长数组类长度最小值应与最大值保持一致");

    // 成员名称和位置不适用与数组
    if (!isUnusedCell(Xlsx::getCellValue(sheet.cell(row, memNameCol))))
        errors.emplace_back("[Error] " + Core::numToCellAddress(row, memNameCol) + ": 数组类型，此处应保持为空，或者填写/或\\");

    if (!isUnusedCell(Xlsx::getCellValue(sheet.cell(row, memPosiCol))))
        errors.emplace_back("[Error] " + Core::numToCellAddress(row, memPosiCol) + ": 数组类型此处应保持为空，或者填写/或\\");

    // 直接在成员所在行定义结构体成员数据类型
    if (const std::string baseType = Xlsx::getCellValue(sheet.cell(row, baseTypeCol)); !isUnusedCell(baseType)) {
        // 直接定义的数据类型
        if (!isUnusedCell(Xlsx::getCellValue(sheet.cell(row, memTypeRefCol))))
            errors.emplace_back("[Error] " + Core::numToCellAddress(row, memTypeRefCol) +
                                ": 数组成员数据类型直接在本行定义时，此处应保持为空，或者填写/或\\");
        checkMustDefinedCell(sheet, row);
    } else {
        // 引用数据类型
        if (const std::string memTypeRef = Xlsx::getCellValue(sheet.cell(row, memTypeRefCol));
            !dataTypeNames.contains(memTypeRef))
            errors.emplace_back("[Error] " + Core::numToCellAddress(row, memTypeRefCol) + ": " + memTypeRef +
                                " 数组成员引用的数据类型未在表格中定义");
        unusedCellForRefType(sheet, row);
    }
}

void DataTypeConsistencyCheck::unusedCellForRefType(const OpenXLSX::XLWorksheet& sheet, const int row) {
    if (!isUnusedCell(Xlsx::getCellValue(sheet.cell(row, baseTypeCol))))
        errors.emplace_back("[Error] " + Core::numToCellAddress(row, baseTypeCol) +
                            ": 结构体或数组成员为引用类型时，此处应保持为空，或者填写/或\\");

    if (!isUnusedCell(Xlsx::getCellValue(sheet.cell(row, sigLenCol))))
        errors.emplace_back("[Error] " + Core::numToCellAddress(row, sigLenCol) +
                            ": 结构体或数组成员为引用类型时，此处应保持为空，或者填写/或\\");

    if (!isUnusedCell(Xlsx::getCellValue(sheet.cell(row, tableCol))))
        errors.emplace_back("[Error] " + Core::numToCellAddress(row, tableCol) +
                            ": 结构体或数组成员为引用类型时，此处应保持为空，或者填写/或\\");

    if (!isUnusedCell(Xlsx::getCellValue(sheet.cell(row, factorCol))))
        errors.emplace_back("[Error] " + Core::numToCellAddress(row, factorCol) +
                            ": 结构体或数组成员为引用类型时，此处应保持为空，或者填写/或\\");

    if (!isUnusedCell(Xlsx::getCellValue(sheet.cell(row, offsetCol))))
        errors.emplace_back("[Error] " + Core::numToCellAddress(row, offsetCol) +
                            ": 结构体或数组成员为引用类型时，此处应保持为空，或者填写/或\\");

    if (!isUnusedCell(Xlsx::getCellValue(sheet.cell(row, minPhyCol))))
        errors.emplace_back("[Error] " + Core::numToCellAddress(row, minPhyCol) +
                            ": 结构体或数组成员为引用类型时，此处应保持为空，或者填写/或\\");

    if (!isUnusedCell(Xlsx::getCellValue(sheet.cell(row, maxPhyCol))))
        errors.emplace_back("[Error] " + Core::numToCellAddress(row, maxPhyCol) +
                            ": 结构体或数组成员为引用类型时，此处应保持为空，或者填写/或\\");

    if (!isUnusedCell(Xlsx::getCellValue(sheet.cell(row, unitCol))))
        errors.emplace_back("[Error] " + Core::numToCellAddress(row, unitCol) +
                            ": 结构体或数组成员为引用类型时，此处应保持为空，或者填写/或\\");
}
