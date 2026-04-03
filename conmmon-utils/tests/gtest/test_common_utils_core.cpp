//
// Created by gaopeng on 2026/3/28.
//
// test_common_utils.cpp
#include <gtest/gtest.h>
#include "common-utils/core.h"
// #include <filesystem>

using namespace common_utils;


// ==================== stringFormat 测试 ====================
TEST(StringFormatTest, EmptyString) {
    EXPECT_EQ(Core::stringFormat(""), "");
}

TEST(StringFormatTest, NoWhitespace) {
    EXPECT_EQ(Core::stringFormat("hello"), "hello");
}

TEST(StringFormatTest, RemoveSpaces) {
    EXPECT_EQ(Core::stringFormat("h e l l o"), "hello");
}

TEST(StringFormatTest, RemoveTabsAndNewlines) {
    EXPECT_EQ(Core::stringFormat("hello\tworld\n"), "helloworld");
}

TEST(StringFormatTest, MixedWhitespace) {
    EXPECT_EQ(Core::stringFormat(" a b \t c \n d "), "abcd");
}

// ==================== stringTrim 测试 ====================
TEST(StringTrimTest, EmptyString) {
    EXPECT_EQ(Core::stringTrim(""), "");
}

TEST(StringTrimTest, NoWhitespace) {
    EXPECT_EQ(Core::stringTrim("hello"), "hello");
}

TEST(StringTrimTest, TrimLeading) {
    EXPECT_EQ(Core::stringTrim("   hello"), "hello");
}

TEST(StringTrimTest, TrimTrailing) {
    EXPECT_EQ(Core::stringTrim("hello   "), "hello");
}

TEST(StringTrimTest, TrimBoth) {
    EXPECT_EQ(Core::stringTrim("  hello  "), "hello");
}

TEST(StringTrimTest, TrimTabsAndNewlines) {
    EXPECT_EQ(Core::stringTrim("\t\nhello\r\n"), "hello");
}

TEST(StringTrimTest, OnlyWhitespace) {
    EXPECT_EQ(Core::stringTrim("   \t\n  "), "");
}

// ==================== isValidCVariableName 测试 ====================
TEST(IsValidCVariableNameTest, ValidNames) {
    EXPECT_TRUE(Core::isValidCVariableName("MyVar"));
    EXPECT_TRUE(Core::isValidCVariableName("Test123"));
    EXPECT_TRUE(Core::isValidCVariableName("A"));
    EXPECT_TRUE(Core::isValidCVariableName("My_Var_123"));
}

TEST(IsValidCVariableNameTest, InvalidStartChar) {
    EXPECT_FALSE(Core::isValidCVariableName("myVar")); // 小写开头
    EXPECT_FALSE(Core::isValidCVariableName("123Var")); // 数字开头
    EXPECT_FALSE(Core::isValidCVariableName("_Var")); // 下划线开头
}

TEST(IsValidCVariableNameTest, Keywords) {
    EXPECT_FALSE(Core::isValidCVariableName("int"));
    EXPECT_FALSE(Core::isValidCVariableName("return"));
    EXPECT_FALSE(Core::isValidCVariableName("sizeof"));
}

TEST(IsValidCVariableNameTest, EmptyAndLength) {
    EXPECT_FALSE(Core::isValidCVariableName(""));
    EXPECT_FALSE(Core::isValidCVariableName("ThisNameIsWayTooLongAndShouldFailTheValidationCheck", 32));
}

// ==================== isNumber 测试 ====================
TEST(IsNumberTest, ValidIntegers) {
    EXPECT_TRUE(Core::isNumber("123"));
    EXPECT_TRUE(Core::isNumber("+456"));
    EXPECT_TRUE(Core::isNumber("-789"));
}

TEST(IsNumberTest, ValidFloats) {
    EXPECT_TRUE(Core::isNumber("3.14"));
    EXPECT_TRUE(Core::isNumber(".5"));
    EXPECT_TRUE(Core::isNumber("1."));
}

TEST(IsNumberTest, ScientificNotation) {
    EXPECT_TRUE(Core::isNumber("1e10"));
    EXPECT_TRUE(Core::isNumber("1.5e-3"));
    EXPECT_TRUE(Core::isNumber("2E+5"));
}

TEST(IsNumberTest, InvalidNumbers) {
    EXPECT_FALSE(Core::isNumber(""));
    EXPECT_FALSE(Core::isNumber("abc"));
    EXPECT_FALSE(Core::isNumber("1.2.3"));
    EXPECT_FALSE(Core::isNumber("1e"));
}

TEST(IsNumberTest, WithWhitespace) {
    EXPECT_TRUE(Core::isNumber("  123  "));
}

// ==================== getExePath 测试 ====================
TEST(GetExePathTest, ReturnsValidPath) {
    std::string path = Core::getExePath();
    EXPECT_FALSE(path.empty());

#ifdef _WIN32
    EXPECT_TRUE(path.find(".exe") != std::string::npos);
#endif

    EXPECT_TRUE(std::filesystem::exists(path));
}

// ==================== getUserHome 测试 ====================
TEST(GetUserHomeTest, ReturnsValidPath) {
    std::string home = Core::getUserHome();
#ifdef _WIN32
    // Windows: 应该有 USERPROFILE
    EXPECT_FALSE(home.empty());
    EXPECT_TRUE(home.find("Users") != std::string::npos ||
        home.find("user") != std::string::npos);
#else
    // Linux: 应该有 HOME
    EXPECT_FALSE(home.empty());
    EXPECT_TRUE(home[0] == '/');
#endif
}

// ==================== doublePrecision 测试 ====================
TEST(DoublePrecisionTest, FixedPrecision) {
    EXPECT_EQ(Core::doublePrecision(3.14159, 2), "3.14");
    EXPECT_EQ(Core::doublePrecision(2.5, 3), "2.500");
    EXPECT_EQ(Core::doublePrecision(10.0, 0), "10");
}

TEST(DoublePrecisionTest, AutoPrecision) {
    EXPECT_EQ(Core::doublePrecision(3.14000, -1), "3.14");
    EXPECT_EQ(Core::doublePrecision(2.50000, -1), "2.5");
    EXPECT_EQ(Core::doublePrecision(10.00000, -1), "10");
    EXPECT_EQ(Core::doublePrecision(1.23456789, -1), "1.23456789");
}

TEST(DoublePrecisionTest, EdgeCases) {
    EXPECT_EQ(Core::doublePrecision(0.0, 2), "0.00");
    EXPECT_EQ(Core::doublePrecision(-3.14, 2), "-3.14");
}

// ==================== columnToNumber 测试 ====================
TEST(ColumnToNumberTest, BasicColumns) {
    EXPECT_EQ(Core::columnToNumber("A"), 1);
    EXPECT_EQ(Core::columnToNumber("B"), 2);
    EXPECT_EQ(Core::columnToNumber("Z"), 26);
}

TEST(ColumnToNumberTest, MultiLetterColumns) {
    EXPECT_EQ(Core::columnToNumber("AA"), 27);
    EXPECT_EQ(Core::columnToNumber("AB"), 28);
    EXPECT_EQ(Core::columnToNumber("AZ"), 52);
    EXPECT_EQ(Core::columnToNumber("BA"), 53);
}

TEST(ColumnToNumberTest, CaseInsensitive) {
    EXPECT_EQ(Core::columnToNumber("a"), 1);
    EXPECT_EQ(Core::columnToNumber("aa"), 27);
}

// ==================== numToCellAddress 测试 ====================
TEST(NumToCellAddressTest, BasicAddresses) {
    EXPECT_EQ(Core::numToCellAddress(1, 1), "A1");
    EXPECT_EQ(Core::numToCellAddress(1, 2), "B1");
    EXPECT_EQ(Core::numToCellAddress(1, 26), "Z1");
    EXPECT_EQ(Core::numToCellAddress(2, 1), "A2");
}

TEST(NumToCellAddressTest, MultiLetterColumns) {
    EXPECT_EQ(Core::numToCellAddress(1, 27), "AA1");
    EXPECT_EQ(Core::numToCellAddress(1, 28), "AB1");
}

TEST(NumToCellAddressTest, InvalidInput) {
    EXPECT_EQ(Core::numToCellAddress(0, 1), "");
    EXPECT_EQ(Core::numToCellAddress(1, 0), "");
}

// ==================== cellAddressToNum 测试 ====================
TEST(CellAddressToNumTest, BasicAddresses) {
    auto [row, col] = Core::cellAddressToNum("A1");
    EXPECT_EQ(row, 1);
    EXPECT_EQ(col, 1);
}

TEST(CellAddressToNumTest, MultiLetterColumns) {
    auto [row, col] = Core::cellAddressToNum("AA10");
    EXPECT_EQ(row, 10);
    EXPECT_EQ(col, 27);
}

TEST(CellAddressToNumTest, ComplexAddresses) {
    auto [row, col] = Core::cellAddressToNum("Z100");
    EXPECT_EQ(row, 100);
    EXPECT_EQ(col, 26);
}

// ==================== getLastOpenDir 测试 ====================
TEST(GetLastOpenDirTest, SaveAndRead) {
    // 保存一个路径
    std::string testDir = std::filesystem::current_path().string();
    std::string result = Core::getLastOpenDir(testDir);

    // 应该返回保存的路径
    EXPECT_EQ(result, testDir);

    // 再次调用（读取模式）
    std::string readResult = Core::getLastOpenDir("");
    EXPECT_FALSE(readResult.empty());
}

TEST(GetLastOpenDirTest, EmptyPathReturnsHome) {
    std::string result = Core::getLastOpenDir("");
    std::string home = Core::getUserHome();

    // 如果没有保存过，应该返回用户主目录
    if (result == home) {
        SUCCEED();
    } else {
        // 或者返回之前保存的路径
        EXPECT_FALSE(result.empty());
    }
}

// ==================== 集成测试示例 ====================
TEST(IntegrationTest, RoundTripCellAddress) {
    // 测试转换的可逆性
    int originalRow = 100;
    int originalCol = 50;

    std::string addr = Core::numToCellAddress(originalRow, originalCol);
    auto [row, col] = Core::cellAddressToNum(addr);

    EXPECT_EQ(row, originalRow);
    EXPECT_EQ(col, originalCol);
}

TEST(IntegrationTest, StringFormatAndTrim) {
    std::string str = "  hello \t world  \n";
    std::string formatted = Core::stringFormat(str);
    std::string trimmed = Core::stringTrim(formatted);

    EXPECT_EQ(trimmed, "helloworld");
}
