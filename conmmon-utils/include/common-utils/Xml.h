//
// Created by gaopeng on 2026/4/2.
//

#pragma once
#include "tinyxml2.h"
#include <string>

namespace common_utils{
    class Xml{
    public:
        static bool openXmlDocument(tinyxml2::XMLDocument& doc, const std::string& path);
        static const tinyxml2::XMLElement* getArPackages(const tinyxml2::XMLDocument& doc);
        static const tinyxml2::XMLElement* findArPackage(const tinyxml2::XMLElement* arPackages,
                                                         const std::string& shortName);
        static const tinyxml2::XMLElement* findElement(const tinyxml2::XMLElement* arPkg, const std::string& name,
                                                       const std::string& type);
    };
} // namespace common_utils
