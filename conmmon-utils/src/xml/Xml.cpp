//
// Created by gaopeng on 2026/4/2.
//

#include "common-utils/Xml.h"
#include <glog/logging.h>
#include <filesystem>
#include <iostream>


namespace common_utils{
    bool Xml::openXmlDocument(tinyxml2::XMLDocument& doc, const std::string& path) {
        // 判断文件是否存在
        if (path.empty() || !std::filesystem::exists(path)) {
            LOG(ERROR) << "文件不存在：" << path;
            return false;
        }

        // 检查文件扩展名
        if (const std::string ext = path.substr(path.find_last_of("."));
            ext != ".xml" && ext != ".arxml" && ext != ".xlsx") {
            LOG(ERROR) << "输入不是一个有效的 XML 文件，文件必须以.xml/.arxml 结尾";
            return false;
        }

        if (doc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS) {
            LOG(ERROR) << "输入的 XML 文件无法打开，请查看文件是否损坏或加密了";
            return false;
        }

        return true;
    }

    const tinyxml2::XMLElement* Xml::getArPackages(const tinyxml2::XMLDocument& doc) {
        return doc.RootElement()->FirstChildElement("AR-PACKAGES");
    }

    // 根据名称查找 AR-PACKAGE
    const tinyxml2::XMLElement*
    Xml::findArPackage(const tinyxml2::XMLElement* arPackages, const std::string& shortName) {
        if (!arPackages) {
            LOG(ERROR) << "输入的AR-PACKAGES为空指针";
            return nullptr;
        }
        for (auto ar_pkg = arPackages->FirstChildElement();
             ar_pkg; ar_pkg = ar_pkg->NextSiblingElement()) {
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

    const tinyxml2::XMLElement* Xml::findElement(const tinyxml2::XMLElement* arPkg, const std::string& name,
                                                 const std::string& type) {
        if (!arPkg) {
            LOG(ERROR) << "输入的AR-PACKAGE为空指针";
            return nullptr;
        }
        // std::cout << "--------element type: " << arPkg->Name() << ", 位于：" << arPkg->GetLineNum() << std::endl;

        for (const tinyxml2::XMLElement* elem = arPkg->FirstChildElement(); elem; elem = elem->NextSiblingElement()) {
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
} // namespace common_utils
