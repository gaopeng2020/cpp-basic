//
// Created by gaopeng on 2026/3/29.
//当创建了 test_main.cpp 并包含了自己的 main 函数后，链接器会使用自定义的 main 函数，而不是 Google Test 提供的默认 main 函数
//
#include <gtest/gtest.h>
#include "logger.h"

class LoggerEnvironment : public ::testing::Environment{
public:
    void SetUp() override {
        Logger::Init("Google_Test", false, google::GLOG_INFO, 1, 5);
        std::cout << "Logger::Init completed" << std::endl;
    }

    void TearDown() override {
        Logger::Shutdown();
        std::cout << "=== TearDown END ===" << std::endl;
    }
};

int main(int argc, char** argv) {
    // 创建报告目录（如果不存在）
    std::filesystem::create_directories("test_reports");

    // 设置输出路径（必须在 InitGoogleTest 之前）
    // testing::GTEST_FLAG(output) = "xml:test_reports/results.xml";
    testing::GTEST_FLAG(output) = "json:test_reports/results.json";

    //初始化GoogleTest
    ::testing::InitGoogleTest(&argc, argv);
    // 注册全局环境
    ::testing::AddGlobalTestEnvironment(new LoggerEnvironment);

    return RUN_ALL_TESTS();
}
