#include <iostream>
#include "logger.h"
#include "common-utils/core.h"
#include "common-utils/xlsx.h"
#include "common-utils/xml.h"
using namespace common_utils;

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


    //全部读取
    for (int t = 0; t < 2; t++) {
        for (int c = 1; c < wb.worksheetCount(); c++) {
            ws = wb.worksheet(c);
            LOG(INFO) << "工作表名称: " << ws.name() << " column count = " << ws.columnCount();
            for (int i = 1; i <= ws.rowCount(); i++) {
                for (int j = 1; j <= ws.columnCount(); j++) {
                    auto cell_assignable = ws.cell(i, j);
                    std::string address = Core::numToCellAddress(i, j);
                    std::string cell_value = Xlsx::getCellValue(cell_assignable);
                    if (cell_value.empty()) continue;
                    const int last_row_num = Xlsx::getLastRowNum(ws, i, j);
                    const int last_clo_num = Xlsx::getLastColumnNum(ws, i, j);
                    if (Xlsx::isCellMerged(ws, i, j)) {
                        LOG(INFO) << address << "= " << cell_value << ", was merged,last row num= " << last_row_num
                            << ",last col num= " << last_clo_num;
                    } else {
                        LOG(INFO) << address << "= " << cell_value << ", was not merged,last row num= " << last_row_num
                            << ",last col num= " << last_clo_num;
                    }
                }
            }
        }
    }
}

void readArxmlTest(tinyxml2::XMLDocument& doc) {
    const std::string path = "C:/Users/gaopeng/Desktop/MPD/ECU1.arxml";
    if (!Xml::openXmlDocument(doc, path)) return;
    const auto arPkgs = Xml::findArPackages(doc);

    const auto comPkg = Xml::findArPackage(arPkgs, "Communication");
    const auto frame = Xml::findElement(comPkg, "ECU1_PhyReq", "CAN-FRAME");
    const auto mapping = Xml::findElement(frame, "NPdu_ECU1_PhyReq", "PDU-TO-FRAME-MAPPING");
    const auto pdu_ref = mapping->FirstChildElement("PDU-REF")->Attribute("DEST");
    std::cout << frame->Name() << " 关联的Pdu名称为 " << pdu_ref << std::endl;
}

void deleteDescription(tinyxml2::XMLDocument& doc) {
    const std::string& path = "C:/Users/gaopeng/Desktop/MPD/MPD.arxml";
    if (!Xml::openXmlDocument(doc, path)) return;
    tinyxml2::XMLElement* arPkgs = Xml::findArPackages(doc);
    if (!arPkgs) {
        std::cout << "arPkgs 没找到" << std::endl;
        Logger::Shutdown();
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
    LOG(INFO) << "开始时间:" << current_timestamp;
    std::cout << "开始时间:" << current_timestamp << std::endl;
    // 删除所有 DESC
    Xml::removeAllDescriptions(arPkgs, "DESC");

    // 删除所有 LONG-NAME
    Xml::removeAllDescriptions(arPkgs, "LONG-NAME");

    Xml::saveXmlDocument(doc, path);

    current_timestamp = Core::getCurrentTimestamp("%Y-%m-%d:%H-%M-%S", true);
    LOG(INFO) << "结束时间:" << current_timestamp;
    std::cout << "结束时间:" << current_timestamp << std::endl;
}

void readAllArPackageTest(tinyxml2::XMLDocument& doc) {
    const std::string& path = "C:/Users/gaopeng/Desktop/MPD/MPD.arxml";
    if (!Xml::openXmlDocument(doc, path)) return;
    tinyxml2::XMLElement* arPkgs = Xml::findArPackages(doc);
    if (!arPkgs) {
        std::cout << "arPkgs 没找到" << std::endl;
        Logger::Shutdown();
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
    Logger::Init(argv[0], false, google::GLOG_INFO, 1, 5);

    LOG(INFO) << "开始读取Excel测试";
    // readXlsxTest();

    tinyxml2::XMLDocument doc;
    LOG(INFO) << "开始读取ARXML测试";
    readArxmlTest(doc);

    readAllArPackageTest(doc);

    Logger::Shutdown();
    return 0;
}
