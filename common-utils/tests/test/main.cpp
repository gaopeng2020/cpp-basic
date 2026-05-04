#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include "common-utils/core.h"
#include "common-utils/xlsx.h"
#include "common-utils/xml.h"
// #include "logger.h"

#include <common-utils/Log.hpp>

using namespace common_utils::core;
using namespace common_utils::xlsx;
using namespace common_utils::xml;
using namespace common_utils::log;

void test_basic_logging() {
    std::cout << "\n=== 测试1: 基本日志输出 ===" << std::endl;

    log_error(App, "这是一条错误日志");
    log_warning(App, "这是一条警告日志");
    log_info(App, "这是一条信息日志");
    log_debug(App, "这是一条调试日志");
}

void test_context_reporting() {
    std::cout << "\n=== 测试2: 上下文信息（文件名/函数名）===" << std::endl;

    Log::report_filenames(true);
    Log::report_functions(true);

    log_warning(Context, "启用文件名和函数名报告");
    log_error(Context, "错误日志包含完整上下文");
}

void test_multithread_logging() {
    std::cout << "\n=== 测试3: 多线程日志 ===" << std::endl;

    auto thread_func = [](const int id) {
        for (int i = 0; i < 10; ++i) {
            log_debug(Thread2, "线程 " << id << " - 消息 " << i);
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    };

    std::thread t1(thread_func, 1);
    std::thread t2(thread_func, 2);
    std::thread t3(thread_func, 3);

    t1.join();
    t2.join();
    t3.join();

    Log::flush();
    std::cout << "所有线程日志完成" << std::endl;
}

void test_verbosity_levels() {
    std::cout << "\n=== 测试4: 不同 verbosity 级别 ===" << std::endl;

    Log::set_verbosity(Log::Error);
    std::cout << "Verbosity = Error:" << std::endl;
    log_info(Level, "这条不会显示");
    log_warning(Level, "这条不会显示");
    log_error(Level, "错误会显示");

    Log::set_verbosity(Log::Warning);
    std::cout << "\nVerbosity = Warning:" << std::endl;
    log_info(Level, "这条不会显示");
    log_warning(Level, "警告会显示");
    log_error(Level, "错误会显示");

    Log::set_verbosity(Log::Info);
    std::cout << "\nVerbosity = Info:" << std::endl;
    log_info(Level, "信息会显示");
    log_warning(Level, "警告会显示");
    log_error(Level, "错误会显示");
}

void readXlsxTest() {
    // 1. 打开 Excel 文件
    OpenXLSX::XLDocument doc;
    doc.open("C:/Users/gaopeng/Desktop/QxlsxTest/Ethernet_someip_cmx_template_2.1.0.xlsx"); // 替换为你的文件路径

    // 2. 获取工作簿和工作表
    auto wb = doc.workbook();
    auto ws = wb.worksheet(4); // 或使用 wb.worksheet(0)
    // LOG(INFO) << "工作表名称: " << ws.name() << " column count = " << ws.columnCount();
    //
    // // 3. 读取指定单元格
    // const std::string cellAddr = "A1";
    // auto [row,col] = Core::cellAddressToNum(cellAddr);
    // LOG(INFO) << "row=" << row << " col=" << col;
    //
    // const auto cell = static_cast<OpenXLSX::XLCell>(ws.cell(row, col));
    // const auto value = Xlsx::getCellValue(cell);
    // LOG(INFO) << cellAddr << "= " << value;

    // 全部读取
    for (int t = 0; t < 2; t++) {
        for (int c = 1; c < wb.worksheetCount(); c++) {
            ws = wb.worksheet(c);
            // LOG(INFO) << "工作表名称: " << ws.name() << " column count = " << ws.columnCount();
            log_error(App, "工作表名称: " << ws.name() << " column count = " << ws.columnCount());
            for (int i = 1; i <= ws.rowCount(); i++) {
                for (int j = 1; j <= ws.columnCount(); j++) {
                    auto cell_assignable = ws.cell(i, j);
                    std::string address = Core::numToCellAddress(i, j);
                    std::string cell_value = Xlsx::getCellValue(cell_assignable);
                    if (cell_value.empty()) continue;
                    const int last_row_num = Xlsx::getLastRowNum(ws, i, j);
                    const int last_clo_num = Xlsx::getLastColumnNum(ws, i, j);
                    if (Xlsx::isCellMerged(ws, i, j)) {
                        // LOG(INFO) << address << "= " << cell_value << ", was merged,last row num= " << last_row_num
                        //           << ",last col num= " << last_clo_num;
                        log_error(App,
                                  address << "= " << cell_value << ", was merged,last row num= " << last_row_num
                                          << ",last col num= " << last_clo_num);
                    } else {
                        // LOG(INFO) << address << "= " << cell_value << ", was not merged,last row num= " << last_row_num
                        //           << ",last col num= " << last_clo_num;
                        log_error(App,
                                  address << "= " << cell_value << ", was not merged,last row num= " << last_row_num
                                          << ",last col num= " << last_clo_num);
                    }
                }
            }
        }
    }
}

void readArxmlTest(tinyxml2::XMLDocument& doc) {
    const std::string path = "C:/Users/gaopeng/Desktop/MPD/CGW.arxml";
    if (!Xml::openXmlDocument(doc, path)) return;
    const auto arPkgs = Xml::findArPackages(doc);

    // const auto comPkg = Xml::findArPackage(arPkgs, "Communication");
    // const auto frame = Xml::findElement(comPkg, "ECU1_PhyReq", "CAN-FRAME");
    // const auto mapping = Xml::findElement(frame, "NPdu_ECU1_PhyReq", "PDU-TO-FRAME-MAPPING");
    // const auto pdu_ref = mapping->FirstChildElement("PDU-REF")->Attribute("DEST");
    // std::cout << frame->Name() << " 关联的Pdu名称为 " << pdu_ref << std::endl;

    Xml::validElementName(arPkgs, "ApplicationDataTypes",64);
    Xml::validElementName(arPkgs, "CompuMethods",64);
}

void deleteDescription(tinyxml2::XMLDocument& doc) {
    const std::string& path = "C:/Users/gaopeng/Desktop/MPD/MPD.arxml";
    if (!Xml::openXmlDocument(doc, path)) return;
    tinyxml2::XMLElement* arPkgs = Xml::findArPackages(doc);
    if (!arPkgs) {
        std::cout << "arPkgs 没找到" << std::endl;
        // Logger::Shutdown();
        return;
    }

    auto elements = Xml::findAllArPackages(arPkgs);
    for (auto element : elements) {
        if (auto ele = element->FirstChildElement("SHORT-NAME")) {
            std::cout << ele->GetText() << std::endl;
        } else {
            std::cout << "不是有效的AP-PACKAGE: " << element->Name() << "位于" << element->GetLineNum() << std::endl;
        }
    }
    auto current_timestamp = Core::getCurrentTimestamp("%Y-%m-%d:%H-%M-%S", true);
    // LOG(INFO) << "开始时间:" << current_timestamp;
    std::cout << "开始时间:" << current_timestamp << std::endl;
    // 删除所有 DESC
    Xml::removeAllDescriptions(arPkgs, "DESC");

    // 删除所有 LONG-NAME
    Xml::removeAllDescriptions(arPkgs, "LONG-NAME");

    Xml::saveXmlDocument(doc, path);

    current_timestamp = Core::getCurrentTimestamp("%Y-%m-%d:%H-%M-%S", true);
    // LOG(INFO) << "结束时间:" << current_timestamp;
    std::cout << "结束时间:" << current_timestamp << std::endl;
}

void readAllArPackageTest(tinyxml2::XMLDocument& doc) {
    const std::string& path = "C:/Users/gaopeng/Desktop/MPD/MPD.arxml";
    if (!Xml::openXmlDocument(doc, path)) return;
    tinyxml2::XMLElement* arPkgs = Xml::findArPackages(doc);
    if (!arPkgs) {
        std::cout << "arPkgs 没找到" << std::endl;
        // Logger::Shutdown();
        return;
    }

    for (const auto elements = Xml::findAllArPackages(arPkgs); const auto element : elements) {
        if (const auto ele = element->FirstChildElement("SHORT-NAME")) {
            std::cout << ele->GetText() << std::endl;
        } else {
            std::cout << "不是有效的AP-PACKAGE: " << element->Name() << "位于" << element->GetLineNum() << std::endl;
        }
    }
}

int main(int argc, char** argv) {
    //------------------------------------------------------自定义log测试------------------------------------------------
    Log::init(argv[0], Log::Info, true, true, 5, 1);
    std::cout << "========================================" << std::endl;
    std::cout << "   异步日志系统完整测试" << std::endl;
    std::cout << "========================================" << std::endl;
    Log::set_verbosity(Log::Debug);

    // test_basic_logging();
    // test_context_reporting();
    // test_multithread_logging();
    // test_verbosity_levels();

    //------------------------------------------------------xml读取测试------------------------------------------------
    // readXlsxTest();
    //
    tinyxml2::XMLDocument doc;
    readArxmlTest(doc);
    // readAllArPackageTest(doc);

    Log::flush();
    std::cout << "\n========================================" << std::endl;
    std::cout << "   所有测试完成！" << std::endl;
    std::cout << "========================================" << std::endl;
    Log::shutdown();
    return 0;
}
