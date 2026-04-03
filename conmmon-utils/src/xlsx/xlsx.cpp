//
// Created by gaopeng on 2026/3/29.
//

#include "common-utils/xlsx.h"
#include "common-utils/core.h"
#include <glog/logging.h>
#include <regex>
#include <filesystem>

namespace common_utils{

    bool Xlsx::openXlDocument(OpenXLSX::XLDocument& doc, const std::string& path) {
        //判断文件是否存在
        if (path.empty() || !std::filesystem::exists(path) || path.substr(path.length() - 5) != ".xlsx") {
            LOG(ERROR) << "输入不是一个有效的Excel文件,文件必须以.xlsx结尾";
            return false;
        }

        doc.open(path);
        if (!doc.isOpen()) {
            LOG(ERROR) << "输入的Excel文件无法打开，请查看文件是否损坏或加密了";
            return false;
        }

        return true;
    }

    int Xlsx::getColumnByHeader(const OpenXLSX::XLWorksheet& sheet, const std::string& headerName, const int row) {
        if (row < 1 || row > 1048576) {
            LOG(ERROR) << "rowNumber valid range [1;1048576]，but the input row= " << row;
            return -1;
        }
        // 获取工作表的列范围（最大列数）
        const int maxColumn = sheet.columnCount();
        if (maxColumn == 0) return -1;

        // 遍历第一行的所有单元格
        for (int col = 1; col <= maxColumn; ++col) {
            auto cell = sheet.cell(row, col);
            // 使用 Core::stringFormat 移除空白字符后比较（包括换行符、空格等）
            // if (auto value = getCellValue(cell); Core::stringFormat(value) == Core::stringFormat(headerName)) {
            //     return col; // 找到匹配，返回列号
            // }
            // 详细调试日志：打印原始值和规范化后的值
            auto value = getCellValue(cell);
            const std::string normalizedValue = Core::stringFormat(value);
            const std::string normalizedHeader = Core::stringFormat(headerName);

            // LOG(INFO) << "[Debug] Col=" << col
            //           << ", raw_cell_value=[" << value << "]"
            //           << ", normalized_cell=[" << normalizedValue << "]"
            //           << ", raw_header=[" << headerName << "]"
            //           << ", normalized_header=[" << normalizedHeader << "]"
            //           << ", match=" << (normalizedValue == normalizedHeader ? "true" : "false");

            if (normalizedValue == normalizedHeader) {
                return col; // 找到匹配，返回列号
            }
        }
        LOG(ERROR) << headerName << " 在[" << sheet.name() << "]中第(" << row << ")行没找到对应的列";
        return -1; // 未找到匹配
    }

    std::string Xlsx::getCellValue(const OpenXLSX::XLCell& cell, const int precision) {
        std::string value;
        if (cell.value().type() == OpenXLSX::XLValueType::String) {
            value = cell.value().get<std::string>();
        } else if (cell.value().type() == OpenXLSX::XLValueType::Float) {
            // 获取单元格格式，判断是否为日期格式
            value = Core::doublePrecision(cell.value().get<double>(), precision);
        } else if (cell.value().type() == OpenXLSX::XLValueType::Integer) {
            value = std::to_string(cell.value().get<int64_t>());
        } else {
            value = "";
        }
        return value;
    }

    int Xlsx::getLastRowNum(OpenXLSX::XLWorksheet& sheet, const int row, const int col) {
        if (row < 1 || row >= 1048576 || col < 1 || col >= 16384) {
            LOG(ERROR) << "rowNumber valid range [1;1048576], columnNumber validrange [1;16384]"
                "and the input is: row= " << row << ", column= " << col;
            return col;
        }
        auto cell = sheet.cell(row, col);
        // 如果单元格为空或者错误，则返回当前行
        if (cell.value().type() == OpenXLSX::XLValueType::Empty
            || cell.value().type() == OpenXLSX::XLValueType::Error)
            return row;

        const std::string cellValue = getCellValue(cell);
        //获取合并区域最后一行
        if (isCellMerged(sheet, row, col)) {
            auto [startAddr, endAddr] = getCellMergedRange(sheet, row, col);
            //通过正则获取endAddr的数字，即为最后一行
            const std::regex rex("([A-Z]+)(\\d+)");
            if (std::smatch match; std::regex_match(endAddr, match, rex)) return std::stoi(match[2].str());
        }
        //获取连续相同值的组后一列
        if (int lastRow = col; getCellValue(sheet.cell(lastRow + 1, col)) == cellValue) {
            while (lastRow < sheet.columnCount()) {
                // 如果值相同，继续
                if (const int nextRow = lastRow + 1; getCellValue(sheet.cell(nextRow, col)) == cellValue)
                    lastRow =
                        nextRow;
                else break;
            }
            return lastRow;
        }
        return row;
    }

    int Xlsx::getLastColumnNum(OpenXLSX::XLWorksheet& sheet, const int row, const int col) {
        if (row < 1 || row >= 1048576 || col < 1 || col >= 16384) {
            LOG(ERROR) << "rowNumber valid range [1;1048576], columnNumber validrange [1;16384]"
                "and the input is: row= " << row << ", column= " << col;
            return col;
        }

        auto cell = sheet.cell(row, col);
        // 如果单元格为空或者错误，则返回当前行
        if (cell.value().type() == OpenXLSX::XLValueType::Empty
            || cell.value().type() == OpenXLSX::XLValueType::Error)
            return row;

        const std::string cellValue = getCellValue(cell);

        //获取合并区域最后一行
        if (isCellMerged(sheet, row, col)) {
            auto [startAddr, endAddr] = getCellMergedRange(sheet, row, col);
            //通过正则获取endAddr的字母，即为第一行
            const std::regex rex("([A-Z]+)(\\d+)");
            if (std::smatch match; std::regex_match(endAddr, match, rex)) return Core::columnToNumber(match[1].str());
        }
        //获取连续相同值的组后一列
        if (int lastCol = col; getCellValue(sheet.cell(row, lastCol + 1)) == cellValue) {
            while (lastCol < sheet.columnCount()) {
                // 如果值相同，继续
                if (const int nextCol = lastCol + 1; getCellValue(sheet.cell(row, nextCol)) == cellValue)
                    lastCol =
                        nextCol;
                else break;
            }
            return lastCol;
        }
        return col;
    }

    std::pair<std::string, std::string> Xlsx::getCellMergedRange(OpenXLSX::XLWorksheet& sheet,
                                                                 const int row, const int col) {
        if (row < 1 || row >= 1048576 || col < 1 || col >= 16384) {
            LOG(ERROR) << "rowNumber valid range [1;1048576], columnNumber validrange [1;16384]"
                "and the input is: row= " << row << ", column= " << col;
            return {};
        }

        auto cellAddr = Core::numToCellAddress(row, col);
        const auto merges = sheet.merges();
        for (int i = 0; i < merges.count(); i++) {
            std::string range = merges[i];
            // 解析合并区域，如 "A1:E1"
            const size_t colon = range.find(':');
            const auto startAddr = range.substr(0, colon);
            const auto endAddr = range.substr(colon + 1);

            auto [startRow, startCol] = Core::cellAddressToNum(startAddr);
            auto [endRow, endCol] = Core::cellAddressToNum(endAddr);
            if (row >= startRow && row <= endRow && col >= startCol && col <= endCol) return {startAddr, endAddr};
        }
        return {cellAddr, cellAddr};
    }

    bool Xlsx::isCellMerged(OpenXLSX::XLWorksheet& sheet, const int row, const int col) {
        auto [startAddr, endAddr] = getCellMergedRange(sheet, row, col);
        return startAddr != endAddr;
    }
}
