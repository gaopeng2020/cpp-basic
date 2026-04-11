//
// Created by gaopeng on 2026/3/29.
//

#pragma once
#include <OpenXLSX.hpp>

namespace common_utils::xlsx{
    /**
     * @brief Xlsx 类用于操作 Excel 文件
     *
     * 该类提供了一系列函数，用于读取、写入和操作 Excel 文件。
     * 主要功能包括：
     * - 检查单元格是否为合并单元格
     * - 获取单元格所在合并区域的起始和结束地址
     * - 根据表头名称获取对应的列号
     * - 获取 Excel 单元格的值并转换为字符串
     * - 获取指定列从某行开始的最后一行行号
     * - 获取指定列从某行开始的所有行数据
     * */
    class Xlsx{
    public:
        /**
         * @brief 打开 Excel 文档文件
         *
         * 该函数用于验证并打开指定路径的 Excel 文件（.xlsx 格式）。
         * 执行以下检查：
         * 1. 文件路径不能为空
         * 2. 文件必须存在于文件系统中
         * 3. 文件扩展名必须是 .xlsx
         * 4. 文件必须能够成功打开（未损坏、未加密）
         *
         * @param doc [out] XLDocument 对象引用，用于存储打开的文档
         * @param path [in] Excel 文件路径
         * @return bool 操作是否成功
         *         - true: 文件成功打开
         *         - false: 文件无效、不存在、损坏或加密
         *
         * @note 如果验证失败，会通过 glog 记录错误日志
         *       支持的 Excel 版本：Excel 2007 及以上（.xlsx 格式）
         *
         * @code{.cpp}
         * OpenXLSX::XLDocument doc;
         * bool success = Xlsx::openXlDocument(doc, "data.xlsx");
         * if (success) {
         *     // 使用 doc 进行后续操作
         * }
         * @endcode
         */
        static bool openXlDocument(OpenXLSX::XLDocument& doc, const std::string& path);
        /**
         * @brief 检查指定单元格是否为合并单元格
         *
         * 该函数检查工作表中的指定单元格是否属于某个合并区域。
         * 通过遍历工作表的所有合并区域，判断给定单元格是否在合并范围内。
         *
         * @param sheet Excel 工作表对象（引用传递）
         * @param row 行号（从 1 开始）
         * @param col 列号（从 1 开始）
         * @return bool 如果单元格是合并单元格的一部分返回 true，否则返回 false
         *
         * @note 该函数内部调用 getCellMergedRange() 获取合并区域信息
         *
         * @code{.cpp}
         * isCellMerged(sheet, 1, 1) -> true (如果 A1 是合并区域的一部分)
         * isCellMerged(sheet, 2, 3) -> false (如果 C2 不是合并区域)
         * @endcode
         */
        static bool isCellMerged(OpenXLSX::XLWorksheet& sheet, int row, int col);

        /**
         * @brief 获取单元格所在合并区域的起始和结束地址
         *
         * 该函数返回指定单元格所属合并区域的范围。如果单元格不在合并区域中，
         * 则返回单元格自身的地址作为起始和结束地址。
         *
         * @param sheet Excel 工作表对象（引用传递）
         * @param row 行号（从 1 开始）
         * @param col 列号（从 1 开始）
         * @return std::pair<std::string, std::string> 包含起始地址和结束地址的 pair
         *         - first: 合并区域的起始单元格地址（如 "A1"）
         *         - second: 合并区域的结束单元格地址（如 "E5"）
         *         如果单元格未合并，则两者都为该单元格自身的地址
         *
         * @note Excel 的有效范围：
         *       - 行号：[1, 1048576]
         *       - 列号：[1, 16384]
         *       如果超出范围，将记录错误日志并返回空 pair
         *
         * @code{.cpp}
         * getCellMergedRange(sheet, 1, 1) -> {"A1", "E1"} (A1:E1 是合并区域)
         * getCellMergedRange(sheet, 3, 3) -> {"C3", "C3"} (C3 未合并)
         * @endcode
         */
        static std::pair<std::string, std::string> getCellMergedRange(OpenXLSX::XLWorksheet& sheet,
                                                                      int row, int col);

        /**
         * @brief 根据表头名称获取对应的列号
         *
         * 该函数在指定的行（默认为第 1 行）中搜索匹配的表头名称，
         * 返回找到的列号。适用于快速定位 Excel 表格的列位置。
         *
         * @param sheet Excel 工作表对象（常量引用）
         * @param headerName 要查找的表头名称字符串
         * @param row 表头所在的行号，默认为 1
         * @return int 匹配的列号（从 1 开始），如果未找到或参数无效则返回 -1
         *
         * @note 行号必须在有效范围 [1, 1048576] 内，否则会记录错误日志并返回 -1
         *       比较时使用精确匹配（区分大小写）
         *
         * @code{.cpp}
         * getColumnByHeader(sheet, "姓名") -> 1 (如果 A1 单元格的值为"姓名")
         * getColumnByHeader(sheet, "年龄", 1) -> 2 (如果 B1 单元格的值为"年龄")
         * getColumnByHeader(sheet, "不存在") -> -1
         * @endcode
         */
        static int getColumnByHeader(const OpenXLSX::XLWorksheet& sheet, const std::string& headerName, int row = 1);

        /**
         * @brief 获取 Excel 单元格的值并转换为字符串
         *
         * 该函数根据单元格的数据类型进行不同的处理：
         * - **字符串类型**：直接返回字符串值
         * - **浮点类型**：使用 Core::doublePrecision() 进行格式化，支持精度控制
         * - **整数类型**：转换为字符串表示
         * - **空值或错误**：返回空字符串
         *
         * @param cell Excel 单元格对象（常量引用）
         * @param precision 浮点数精度，默认为 -1（自动判断小数位数）
         *        - >= 0: 保留指定的小数位数
         *        - < 0: 自动去除末尾多余的零
         * @return std::string 单元格值的字符串表示
         *
         * @code{.cpp}
         * getCellValue(cell) -> "Hello" (字符串单元格)
         * getCellValue(cell) -> "3.14" (浮点数单元格，自动精度)
         * getCellValue(cell, 2) -> "3.14" (浮点数单元格，保留 2 位小数)
         * getCellValue(cell) -> "100" (整数单元格)
         * @endcode
         */
        static std::string getCellValue(const OpenXLSX::XLCell& cell, int precision = -1);

        /**
         * @brief 获取指定列从某行开始的最后一行行号
         *
         * 该函数用于查找连续数据区域的最后一行，支持以下场景：
         * 1. **合并单元格**：如果单元格属于合并区域，返回合并区域的最后一行
         * 2. **连续相同值**：如果下方单元格有相同的值，继续向下查找直到值不同
         * 3. **空值或错误**：如果单元格为空或错误，直接返回当前行
         *
         * @param sheet Excel 工作表对象（引用传递）
         * @param row 起始行号（从 1 开始）
         * @param col 列号（从 1 开始）
         * @return int 最后一行的行号
         *
         * @note Excel 的有效范围：
         *       - 行号：[1, 1048576]
         *       - 列号：[1, 16384]
         *       如果超出范围，将记录错误日志并返回输入的列号
         *
         * @code{.cpp}
         * getLastRowNum(sheet, 1, 1) -> 5 (A1:A5 是连续的数据区域)
         * getLastRowNum(sheet, 2, 3) -> 2 (C2 是独立单元格)
         * @endcode
         */
        static int getLastRowNum(OpenXLSX::XLWorksheet& sheet, int row, int col);

        /**
         * @brief 获取指定行从某列开始的最后一列列号
         *
         * 该函数用于查找连续数据区域的最后一列，支持以下场景：
         * 1. **合并单元格**：如果单元格属于合并区域，返回合并区域的最后一列
         * 2. **连续相同值**：如果右侧单元格有相同的值，继续向右查找直到值不同
         * 3. **空值或错误**：如果单元格为空或错误，直接返回当前列
         *
         * @param sheet Excel 工作表对象（引用传递）
         * @param row 行号（从 1 开始）
         * @param col 起始列号（从 1 开始）
         * @return int 最后一列的列号
         *
         * @note Excel 的有效范围：
         *       - 行号：[1, 1048576]
         *       - 列号：[1, 16384]
         *       如果超出范围，将记录错误日志并返回输入的列号
         *
         * @code{.cpp}
         * getLastColumnNum(sheet, 1, 1) -> 5 (A1:E1 是连续的数据区域)
         * getLastColumnNum(sheet, 2, 3) -> 2 (C2 是独立单元格)
         * @endcode
         */
        static int getLastColumnNum(OpenXLSX::XLWorksheet& sheet, int row, int col);
    };
} // namespace common_utils
