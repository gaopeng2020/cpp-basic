//
// Created by gaopeng on 2026/4/2.
//

#pragma once
#include <string>
#include <vector>
#include "tinyxml2.h"

namespace common_utils::xml {
/**
 * @brief XML 工具类，提供 XML 文档解析和操作功能
 *
 * 该类基于 tinyxml2 库，封装了常用的 XML 文档操作函数，包括：
 * - XML 文档的加载和保存
 * - 元素的查找和遍历
 * - 属性的读取和设置
 * - 文本内容的获取
 * - 节点的插入和删除
 *
 * @par tinyxml2 常用函数参考表
 *
 * | 操作类型 | 方法名 | 说明 |
 * |----------|--------|------|
 * | 加载文件 | `doc.LoadFile(path)` | 加载 XML 文件，返回 XML_SUCCESS 表示成功 |
 * | 保存文件 | `doc.SaveFile(path)` | 将 XML 文档保存到指定路径 |
 * | 创建元素 | `doc.NewElement(name)` | 创建新元素，需手动插入到文档树 |
 * | 插入子节点 | `parent->InsertEndChild(child)` | 将子节点插入到父节点末尾 |
 * | 设置属性 | `elem->SetAttribute(name, value)` | 为元素设置属性值 |
 * | 读取属性 | `elem->Attribute(name)` | 读取元素属性，返回 const char* |
 * | 读取数值属性 | `elem->QueryIntAttribute(name, &val)` | 读取整数属性，返回错误码（更安全） |
 * | 设置文本 | `elem->SetText(text)` | 设置元素的文本内容 |
 * | 获取文本 | `elem->GetText()` | 获取元素文本，返回 const char* |
 * | 获取数值文本 | `elem->QueryDoubleText(&val)` | 读取双精度数值文本，返回错误码 |
 * | 遍历子元素 | `parent->FirstChildElement(name)` | 获取第一个子元素，常配合 NextSiblingElement 使用 |
 * | 删除子节点 | `parent->DeleteChild(child)` | 从父节点中删除指定子节点 |
 *
 * @note 所有方法都是静态方法，可以直接通过类名调用
 */
class Xml {
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
    static tinyxml2::XMLElement* findArPackage(tinyxml2::XMLElement* arPackages, const std::string& shortName);

    /**
     * @brief 在指定的 AR Package 中查找特定名称和类型的元素
     * @param arPkg [in] AR Package 元素指针
     * @param name [in] 要查找的元素名称
     * @param type [in] 要查找的元素类型
     * @return 找到则返回指向该元素的指针，否则返回 nullptr
     */
    static tinyxml2::XMLElement* findElement(tinyxml2::XMLElement* arPkg, const std::string& name, const std::string& type);

    /**
     * @brief 移除所有指定标签的描述元素
     * @param arPkgs [in] AR Packages 元素指针
     * @param tag [in] 要移除的描述标签名称，默认为 "DESC"
     */
    static void removeAllDescriptions(tinyxml2::XMLElement* arPkgs, const std::string& tag = "DESC");
};
} // namespace common_utils::xml
