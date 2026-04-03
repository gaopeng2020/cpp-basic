//
// Created by gaopeng on 2026/4/2.
//

#pragma once
#include "tinyxml2.h"
#include <string>
#include <vector>

namespace common_utils{
    /**
     * @brief XML 工具类，提供 XML 文档解析和操作功能
     */
    class Xml{
    public:
        /**
         * @brief 打开并解析 XML 文档
         * @param doc [out] XML 文档对象引用，用于存储解析后的文档内容
         * @param path [in] XML 文件路径
         * @return 成功返回 true，失败返回 false
         */
        static bool openXmlDocument(tinyxml2::XMLDocument& doc, const std::string& path);
        /**
         * @brief 保存 XML 文档到指定路径
         * @param doc [in] XML 文档对象引用，包含要保存的文档内容
         * @param path [in] 文件保存路径
         */
       static void saveXmlDocument(tinyxml2::XMLDocument& doc, const std::string& path);
        /**
         * @brief 查找第一个 AR Packages 元素
         * @param doc [in] XML 文档对象引用
         * @return 找到则返回指向该元素的指针，否则返回 nullptr
         */
        static tinyxml2::XMLElement* findArPackages(tinyxml2::XMLDocument& doc);

        /**
         * @brief 查找所有 AR Packages 元素
         * @param ar_packages [in/out] AR Packages 根元素引用
         * @return 包含所有 AR Package 元素的向量
         */
        static std::vector<tinyxml2::XMLElement*> findAllArPackages(tinyxml2::XMLElement*& ar_packages);

        /**
         * @brief 根据短名称查找指定的 AR Package 元素
         * @param arPackages [in] AR Packages 元素指针
         * @param shortName [in] 要查找的 AR Package 的短名称
         * @return 找到则返回指向该元素的指针，否则返回 nullptr
         */
        static tinyxml2::XMLElement* findArPackage(tinyxml2::XMLElement* arPackages,
                                                         const std::string& shortName);

        /**
         * @brief 在指定的 AR Package 中查找特定名称和类型的元素
         * @param arPkg [in] AR Package 元素指针
         * @param name [in] 要查找的元素名称
         * @param type [in] 要查找的元素类型
         * @return 找到则返回指向该元素的指针，否则返回 nullptr
         */
        static tinyxml2::XMLElement* findElement(tinyxml2::XMLElement* arPkg, const std::string& name,
                                                       const std::string& type);

        /**
         * @brief 移除所有指定标签的描述元素
         * @param arPkgs [in] AR Packages 元素指针
         * @param tag [in] 要移除的描述标签名称，默认为 "DESC"
         */
       static void removeAllDescriptions(tinyxml2::XMLElement* arPkgs, const std::string& tag = "DESC") ;
    };
} // namespace common_utils

