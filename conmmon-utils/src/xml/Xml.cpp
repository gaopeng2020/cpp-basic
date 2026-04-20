//
// Created by gaopeng on 2026/4/2.
//

#include "common-utils/Xml.h"
#include <filesystem>
#include <format>
#include "common-utils/Log.hpp"
#include "common-utils/core.h"

namespace common_utils::xml {
using namespace common_utils::core;
using namespace common_utils::log;
bool Xml::openXmlDocument(tinyxml2::XMLDocument& doc, const std::string& path) {
    // 判断文件是否存在
    if (path.empty() || !std::filesystem::exists(path)) {
        // LOG(ERROR) << "文件不存在：" << path;
        log_error("Xml", "文件不存在：" << path);
        return false;
    }

    // 检查文件扩展名
    if (const std::string ext = path.substr(path.find_last_of(".")); ext != ".xml" && ext != ".arxml" && ext != ".xlsx") {
        // LOG(ERROR) << "输入不是一个有效的 XML 文件，文件必须以.xml/.arxml 结尾";
        log_error("Xml", "输入不是一个有效的 XML 文件，文件必须以.xml/.arxml 结尾");
        return false;
    }

    if (doc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS) {
        // LOG(ERROR) << "输入的 XML 文件无法打开，请查看文件是否损坏或加密了";
        log_error("Xml", "输入的 XML 文件无法打开，请查看文件是否损坏或加密了");
        return false;
    }

    return true;
}

void Xml::saveXmlDocument(tinyxml2::XMLDocument& doc, const std::string& path) {
    // 获取path所在目录
    const std::string dir = std::filesystem::path(path).parent_path().string();
    // 获取文件名
    const std::size_t pos = path.find_last_of("/\\");
    const std::string fileName = pos != std::string::npos ? path.substr(pos + 1) : path;
    // 在文件名加上_modified
    const std::string modifiedPath = dir + "/" + fileName.substr(0, fileName.find_last_of(".")) +
        "_modified"
        // 加上当前时间
        + "_" + Core::getCurrentTimestamp() + fileName.substr(fileName.find_last_of("."));

    // 这种保存方法会格式化文档
    if (doc.SaveFile(modifiedPath.c_str()) == tinyxml2::XML_SUCCESS) {
        // LOG(INFO) << "XML 已保存到：" << modifiedPath;
        log_info("Xml", "XML 已保存到：" << modifiedPath);
    }
}

tinyxml2::XMLElement* Xml::findArPackages(tinyxml2::XMLDocument& doc) {
    return doc.RootElement()->FirstChildElement("AR-PACKAGES");
}

// 根据名称查找 AR-PACKAGE
tinyxml2::XMLElement* Xml::findArPackage(tinyxml2::XMLElement* arPackages, const std::string& shortName) {
    if (!arPackages) {
        // LOG(ERROR) << "输入的AR-PACKAGES为空指针";
        log_error("Xml", "输入的AR-PACKAGES为空指针");
        return nullptr;
    }
    for (auto ar_pkg = arPackages->FirstChildElement(); ar_pkg; ar_pkg = ar_pkg->NextSiblingElement()) {
        if (std::string(ar_pkg->Name()) == "AR-PACKAGE") {
            if (const auto nameEle = ar_pkg->FirstChildElement("SHORT-NAME");
                nameEle && nameEle->GetText() && shortName == nameEle->GetText()) {
                // std::cout << "-------已找到AR-PACKAGE：" << shortName << "，位于第" << nameEle->GetLineNum() << std::endl;
                return ar_pkg;
            }
        }

        if (ar_pkg->FirstChildElement()) {
            // std::cout << "-------正在递归查找---------" << ar_pkg->Name() << " 位于 " << ar_pkg->GetLineNum() << std::endl;
            if (const auto found = findArPackage(ar_pkg, shortName)) return found;
        }
    }
    return nullptr;
}

std::vector<tinyxml2::XMLElement*> Xml::findAllArPackages(tinyxml2::XMLElement*& ar_packages) {
    std::vector<tinyxml2::XMLElement*> vec;
    if (!ar_packages) {
        // LOG(ERROR) << "输入的AR-PACKAGES为空指针";
        log_error("Xml", "输入的AR-PACKAGES为空指针");
        return vec;
    }
    for (auto ar_pkg = ar_packages->FirstChildElement(); ar_pkg; ar_pkg = ar_pkg->NextSiblingElement()) {
        if (std::string(ar_pkg->Name()) == "AR-PACKAGE" && !ar_pkg->FirstChildElement("AR-PACKAGES")) {
            vec.push_back(ar_pkg);
        }

        if (ar_pkg->FirstChildElement()) {
            if (const auto found = findAllArPackages(ar_pkg); !found.empty()) {
                vec.insert(vec.end(), found.begin(), found.end());
            }
        }
    }

    return vec;
}

tinyxml2::XMLElement* Xml::findElement(tinyxml2::XMLElement* arPkg, const std::string& name, const std::string& type) {
    if (!arPkg) {
        // LOG(ERROR) << "输入的AR-PACKAGE为空指针";
        log_error("Xml", "输入的AR-PACKAGE为空指针");
        return nullptr;
    }
    // std::cout << "--------element type: " << arPkg->Name() << ", 位于：" << arPkg->GetLineNum() << std::endl;
    for (tinyxml2::XMLElement* elem = arPkg->FirstChildElement(); elem; elem = elem->NextSiblingElement()) {
        // XMLElement *attr = elem->FirstChildElement();
        if (elem->GetText() && name == elem->GetText() && elem->Parent()->ToElement()->Name() == type) {
            // std::cout << "-------已找到Element: Name=" << name << ", Type：" << type
            //     << "，位于 " << elem->GetLineNum() << std::endl;
            return elem->Parent()->ToElement();
        }
        if (elem->FirstChildElement()) {
            // std::cout << "-------正在递归查找---------" << elem->GetLineNum() << std::endl;
            if (const auto target = findElement(elem, type, name)) return target;
        }
    }
    return nullptr;
}

void Xml::removeAllDescriptions(tinyxml2::XMLElement* arPkgs, const std::string& tag) {
    if (!arPkgs) {
        // LOG(ERROR) << "输入的AR-PACKAGE为空指针";
        log_error("Xml", "输入的AR-PACKAGE为空指针");
        return;
    }

    // 1. 删除当前元素的所有指定标签的子元素（可能有多个）
    while (const auto child = arPkgs->FirstChildElement(tag.c_str())) {
        arPkgs->DeleteChild(child);
        // LOG(INFO) << "已删除元素 " << arPkgs->Name() << " 的 " << tag << " (第" << arPkgs->GetLineNum() << "行)";
        log_info("Xml", "已删除元素 " << arPkgs->Name() << " 的 " << tag << " (第" << arPkgs->GetLineNum() << "行)");
    }

    // 2. 深度优先遍历所有子元素
    for (auto child = arPkgs->FirstChildElement(); child; child = child->NextSiblingElement()) {
        // 递归调用，深入下一层
        removeAllDescriptions(child, tag);
    }
}

void Xml::validElementName(tinyxml2::XMLElement* ar_pkgs, const std::string& type, const uint8_t len) {
    if (!ar_pkgs) {
        log_error("Xml", "输入的AR-PACKAGE为空指针");
        return;
    }
    auto ar_pkg = Xml::findArPackage(ar_pkgs, type);
    if (!ar_pkg) {
        log_error("Xml", std::format(" 没有找到{}类型的AP-PACKAGE，请确认输入是否正确", type));
        return;
    }
    auto elements = ar_pkg->FirstChildElement("ELEMENTS");
    if (!elements) {
        log_error("Xml", type << " 下没有找到ELEMENTS节点，请确认输入是否正确");
        return;
    }
    for (tinyxml2::XMLElement* element = elements->FirstChildElement(); element; element = element->NextSiblingElement()) {
        // 判断元素名称是否符合C变量命名要求
        if (const auto name_element = element->FirstChildElement("SHORT-NAME");
            name_element && !Core::isValidCVariableName(name_element->GetText(), len, false)) {
            const char* name = name_element->GetText();
            std::string msg = std::format(
                "{} 名称不符合C变量命名要求(建议大写字母开头，只能包含字母、数字、下划线，长度不超过{}个字符)", name, len);
            if (std::string(name).length() > len) {
                msg = std::format("{} (实际长度：{}，位于第{}行)", msg, std::string(name).length(), name_element->GetLineNum());
            } else {
                msg = std::format("{} (位于第{}行)", msg, name_element->GetLineNum());
            }
            log_info("Xml", msg);
        }
        if (type != "CompuMethods") {
            continue;
        }
        // 定义一个lambda，内部采用深度优先遍历，找到<VT>标签后检查Text是否符号C变量命名要求
        std::function<void(tinyxml2::XMLElement*)> dfs = [&](tinyxml2::XMLElement* node) {
            if (!node) return;
            if (const auto vt_node = node->FirstChildElement("VT");
                vt_node && !Core::isValidCVariableName(vt_node->GetText(), len, false)) {
                auto text = std::string(vt_node->GetText());
                std::string error_msg = std::format(
                    "{} 名称不符合C变量命名要求(建议大写字母开头，只能包含字母、数字、下划线，长度不超过{}个字符)", text, len);
                if (text.length() > len) {
                    error_msg = std::format("{} (实际长度：{}，位于第{}行)", error_msg, text.length(), vt_node->GetLineNum());
                } else {
                    error_msg = std::format("{} (位于第{}行)", error_msg, vt_node->GetLineNum());
                }
                log_info("Xml", error_msg);
            }
            // 递归遍历子元素
            for (tinyxml2::XMLElement* sub_node = node->FirstChildElement(); sub_node;
                 sub_node = sub_node->NextSiblingElement()) {
                dfs(sub_node);
            }
        };
        dfs(element);
    }
}

} // namespace common_utils::xml
