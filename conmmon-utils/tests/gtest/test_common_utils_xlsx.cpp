//
// Created by gaopeng on 2026/3/29.
//
// test_common_utils_xlsx.cpp
#include <gtest/gtest.h>
#include "common-utils/xlsx.h"


using namespace common_utils;

// ... existing code ...

// ==================== getColumnByHeader 测试 ====================
TEST(GetColumnByHeaderTest, HeaderNotFound) {
    // 创建一个临时的 Excel 文件用于测试
    OpenXLSX::XLDocument doc;
    const std::string testFile = "test_header.xlsx";

    // 创建工作簿并写入测试数据
    doc.create(testFile);
    auto wks = doc.workbook().worksheet("Sheet1");
    wks.cell(1, 1).value() = "Name";
    wks.cell(1, 2).value() = "Age";
    wks.cell(1, 3).value() = "City";
    doc.save();
    doc.close();

    // 重新打开读取
    doc.open(testFile);
    wks = doc.workbook().worksheet("Sheet1");

    // 测试找到列
    EXPECT_EQ(Xlsx::getColumnByHeader(wks, "Age"), 2);
    EXPECT_EQ(Xlsx::getColumnByHeader(wks, "City"), 3);

    // 测试未找到列
    EXPECT_EQ(Xlsx::getColumnByHeader(wks, "Country"), -1);

    doc.close();
    std::filesystem::remove(testFile);
}

TEST(GetColumnByHeaderTest, CustomRow) {
    OpenXLSX::XLDocument doc;
    const std::string testFile = "test_custom_row.xlsx";

    doc.create(testFile);
    auto wks = doc.workbook().worksheet("Sheet1");
    // 在第 2 行写入表头
    wks.cell(2, 1).value() = "ID";
    wks.cell(2, 2).value() = "Value";
    doc.save();
    doc.close();

    doc.open(testFile);
    wks = doc.workbook().worksheet("Sheet1");

    // 指定在第 2 行查找表头
    EXPECT_EQ(Xlsx::getColumnByHeader(wks, "ID", 2), 1);
    EXPECT_EQ(Xlsx::getColumnByHeader(wks, "Value", 2), 2);

    doc.close();
    std::filesystem::remove(testFile);
}

// ==================== getCellValue 测试 ====================
TEST(GetCellValueTest, StringValue) {
    OpenXLSX::XLDocument doc;
    const std::string testFile = "test_string.xlsx";

    doc.create(testFile);
    auto wks = doc.workbook().worksheet("Sheet1");
    wks.cell(1, 1).value() = "Hello World";
    doc.save();
    doc.close();

    doc.open(testFile);
    wks = doc.workbook().worksheet("Sheet1");
    auto cell = wks.cell(1, 1);

    EXPECT_EQ(Xlsx::getCellValue(cell), "Hello World");

    doc.close();
    std::filesystem::remove(testFile);
}

TEST(GetCellValueTest, IntegerValue) {
    OpenXLSX::XLDocument doc;
    const std::string testFile = "test_integer.xlsx";

    doc.create(testFile);
    auto wks = doc.workbook().worksheet("Sheet1");
    wks.cell(1, 1).value() = 42;
    doc.save();
    doc.close();

    doc.open(testFile);
    wks = doc.workbook().worksheet("Sheet1");
    auto cell = wks.cell(1, 1);

    EXPECT_EQ(Xlsx::getCellValue(cell), "42");

    doc.close();
    std::filesystem::remove(testFile);
}

TEST(GetCellValueTest, FloatValue) {
    OpenXLSX::XLDocument doc;
    const std::string testFile = "test_float.xlsx";

    doc.create(testFile);
    auto wks = doc.workbook().worksheet("Sheet1");
    wks.cell(1, 1).value() = 3.14159;
    doc.save();
    doc.close();

    doc.open(testFile);
    wks = doc.workbook().worksheet("Sheet1");
    auto cell = wks.cell(1, 1);

    // 默认精度
    std::string value = Xlsx::getCellValue(cell);
    EXPECT_FALSE(value.empty());
    EXPECT_TRUE(value.find("3.14") != std::string::npos);

    // 指定精度
    value = Xlsx::getCellValue(cell, 2);
    EXPECT_EQ(value, "3.14");

    doc.close();
    std::filesystem::remove(testFile);
}

TEST(GetCellValueTest, EmptyValue) {
    OpenXLSX::XLDocument doc;
    const std::string testFile = "test_empty.xlsx";

    doc.create(testFile);
    doc.save();
    doc.close();

    doc.open(testFile);
    auto wks = doc.workbook().worksheet("Sheet1");
    auto cell = wks.cell(1, 1);

    EXPECT_EQ(Xlsx::getCellValue(cell), "");

    doc.close();
    std::filesystem::remove(testFile);
}

// ==================== getLastRowNum 测试 ====================
TEST(GetLastRowNumTest, SingleValue) {
    OpenXLSX::XLDocument doc;
    const std::string testFile = "test_single_row.xlsx";

    doc.create(testFile);
    auto wks = doc.workbook().worksheet("Sheet1");
    wks.cell(1, 1).value() = "Test";
    doc.save();
    doc.close();

    doc.open(testFile);
    wks = doc.workbook().worksheet("Sheet1");

    // 只有一个值，应该返回当前行
    EXPECT_EQ(Xlsx::getLastRowNum(wks, 1, 1), 1);

    doc.close();
    std::filesystem::remove(testFile);
}

TEST(GetLastRowNumTest, InvalidInput) {
    OpenXLSX::XLDocument doc;
    const std::string testFile = "test_invalid.xlsx";

    doc.create(testFile);
    doc.save();
    doc.close();

    doc.open(testFile);
    auto wks = doc.workbook().worksheet("Sheet1");

    // 行号小于 1，会触发 LOG(ERROR)，但函数仍会执行
    // 这里主要测试函数不会崩溃
    EXPECT_NO_THROW(Xlsx::getLastRowNum(wks, 0, 1));
    EXPECT_NO_THROW(Xlsx::getLastRowNum(wks, 1, 0));

    doc.close();
    std::filesystem::remove(testFile);
}

// ==================== getLastColumnNum 测试 ====================
TEST(GetLastColumnNumTest, SingleValue) {
    OpenXLSX::XLDocument doc;
    const std::string testFile = "test_single_col.xlsx";

    doc.create(testFile);
    auto wks = doc.workbook().worksheet("Sheet1");
    wks.cell(1, 1).value() = "Test";
    doc.save();
    doc.close();

    doc.open(testFile);
    wks = doc.workbook().worksheet("Sheet1");

    EXPECT_EQ(Xlsx::getLastColumnNum(wks, 1, 1), 1);

    doc.close();
    std::filesystem::remove(testFile);
}

TEST(GetLastColumnNumTest, InvalidInput) {
    OpenXLSX::XLDocument doc;
    const std::string testFile = "test_invalid_col.xlsx";

    doc.create(testFile);
    doc.save();
    doc.close();

    doc.open(testFile);
    auto wks = doc.workbook().worksheet("Sheet1");

    EXPECT_NO_THROW(Xlsx::getLastColumnNum(wks, 0, 1));
    EXPECT_NO_THROW(Xlsx::getLastColumnNum(wks, 1, 0));

    doc.close();
    std::filesystem::remove(testFile);
}

// ==================== isCellMerged 测试 ====================
TEST(IsCellMergedTest, NonMergedCell) {
    OpenXLSX::XLDocument doc;
    const std::string testFile = "test_non_merged.xlsx";

    doc.create(testFile);
    auto wks = doc.workbook().worksheet("Sheet1");
    wks.cell(1, 1).value() = "A1";
    wks.cell(1, 2).value() = "B1";
    doc.save();
    doc.close();

    doc.open(testFile);
    wks = doc.workbook().worksheet("Sheet1");

    // 未合并的单元格应该返回 false
    EXPECT_FALSE(Xlsx::isCellMerged(wks, 1, 1));
    EXPECT_FALSE(Xlsx::isCellMerged(wks, 1, 2));

    doc.close();
    std::filesystem::remove(testFile);
}

// ==================== getCellMergedRange 测试 ====================
TEST(GetCellMergedRangeTest, NonMergedCell) {
    OpenXLSX::XLDocument doc;
    const std::string testFile = "test_range.xlsx";

    doc.create(testFile);
    auto wks = doc.workbook().worksheet("Sheet1");
    wks.cell(2, 3).value() = "C2";
    doc.save();
    doc.close();

    doc.open(testFile);
    wks = doc.workbook().worksheet("Sheet1");

    // 未合并的单元格，起始和结束地址应该相同
    auto [startAddr, endAddr] = Xlsx::getCellMergedRange(wks, 2, 3);
    EXPECT_EQ(startAddr, "C2");
    EXPECT_EQ(endAddr, "C2");

    doc.close();
    std::filesystem::remove(testFile);
}

// ==================== 集成测试 ====================
TEST(IntegrationTest, CreateAndReadExcel) {
    const std::string testFile = "test_integration.xlsx";

    // 创建 Excel 文件
    {
        OpenXLSX::XLDocument doc;
        doc.create(testFile);
        auto wks = doc.workbook().worksheet("Sheet1");

        // 写入表头
        wks.cell(1, 1).value() = "Name";
        wks.cell(1, 2).value() = "Age";
        wks.cell(1, 3).value() = "Score";

        // 写入数据
        wks.cell(2, 1).value() = "Alice";
        wks.cell(2, 2).value() = 25;
        wks.cell(2, 3).value() = 95.5;

        wks.cell(3, 1).value() = "Bob";
        wks.cell(3, 2).value() = 30;
        wks.cell(3, 3).value() = 87.3;

        doc.save();
    }

    // 读取并验证
    {
        OpenXLSX::XLDocument doc;
        doc.open(testFile);
        auto wks = doc.workbook().worksheet("Sheet1");

        // 测试获取列号
        EXPECT_EQ(Xlsx::getColumnByHeader(wks, "Name"), 1);
        EXPECT_EQ(Xlsx::getColumnByHeader(wks, "Age"), 2);
        EXPECT_EQ(Xlsx::getColumnByHeader(wks, "Score"), 3);

        // 测试获取单元格值
        auto nameCell = wks.cell(2, 1);
        EXPECT_EQ(Xlsx::getCellValue(nameCell), "Alice");

        auto ageCell = wks.cell(2, 2);
        EXPECT_EQ(Xlsx::getCellValue(ageCell), "25");

        auto scoreCell = wks.cell(2, 3);
        std::string scoreValue = Xlsx::getCellValue(scoreCell, 1);
        EXPECT_EQ(scoreValue, "95.5");

        doc.close();
    }

    std::filesystem::remove(testFile);
}

TEST(IntegrationTest, ComplexDataTable) {
    const std::string testFile = "test_complex_table.xlsx";

    // 创建一个复杂的数据表
    {
        OpenXLSX::XLDocument doc;
        doc.create(testFile);
        auto wks = doc.workbook().worksheet("Sheet1");

        // 创建表头
        wks.cell(1, 1).value() = "ID";
        wks.cell(1, 2).value() = "Product";
        wks.cell(1, 3).value() = "Price";
        wks.cell(1, 4).value() = "Quantity";
        wks.cell(1, 5).value() = "Total";

        // 创建多行数据
        for (int i = 2; i <= 10; ++i) {
            wks.cell(i, 1).value() = i - 1;
            wks.cell(i, 2).value() = "Product_" + std::to_string(i - 1);
            wks.cell(i, 3).value() = 10.0 + (i - 1) * 0.5;
            wks.cell(i, 4).value() = static_cast<int64_t>(i * 2);
            wks.cell(i, 5).value() = (10.0 + (i - 1) * 0.5) * (i * 2);
        }

        doc.save();
    }

    // 验证数据
    {
        OpenXLSX::XLDocument doc;
        doc.open(testFile);
        auto wks = doc.workbook().worksheet("Sheet1");

        // 验证表头
        EXPECT_EQ(Xlsx::getColumnByHeader(wks, "Price"), 3);
        EXPECT_EQ(Xlsx::getColumnByHeader(wks, "Total"), 5);

        // 随机验证几行数据
        auto priceCell = wks.cell(5, 3);
        std::string priceValue = Xlsx::getCellValue(priceCell, 2);
        EXPECT_EQ(priceValue, "12.00");

        auto quantityCell = wks.cell(7, 4);
        EXPECT_EQ(Xlsx::getCellValue(quantityCell), "14");

        doc.close();
    }

    std::filesystem::remove(testFile);
}

// ... existing code ...
