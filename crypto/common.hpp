#pragma once
#include <fstream>
#include <iomanip>
#include <iostream>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <string>
#include <vector>

// 错误处理辅助函数
inline void handle_openssl_error() {
    ERR_print_errors_fp(stderr);
    exit(1);
}

// 十六进制打印辅助函数
inline void print_hex(const std::vector<unsigned char>& data) {
    for (const unsigned char i : data) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);
    }
    std::cout << std::dec << std::endl;
}

// 生成随机密钥和IV
inline void generate_key_iv(std::vector<unsigned char>& key, std::vector<unsigned char>& iv) {
    // AES-128 密钥长度 = 16字节 (128,192，256)
    // key.resize(16);
    if (RAND_bytes(key.data(), static_cast<int>(key.size())) != 1) {
        handle_openssl_error();
    }
    // GCM推荐IV长度 = 12字节
    // iv.resize(12);
    if (RAND_bytes(iv.data(), static_cast<int>(iv.size())) != 1) {
        handle_openssl_error();
    }
}

// ========== 2. 密钥存储与读取 ==========
// 保存密钥和IV到文件（二进制格式）
inline void save_key_iv(const std::vector<unsigned char>& key, const std::vector<unsigned char>& iv,
                        const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "无法创建文件: " << filename << std::endl;
        return;
    }

    // 写入：密钥 + IV
    file.write(reinterpret_cast<const char*>(key.data()), key.size());
    file.write(reinterpret_cast<const char*>(iv.data()), iv.size());

    std::cout << "密钥和IV已保存到: " << filename << std::endl;
    std::cout << "  - 密钥(" << key.size() << "字节): ";
    print_hex(key);
    std::cout << "  - IV(" << iv.size() << "字节): ";
    print_hex(iv);
}

// 从文件读取密钥和IV
inline bool load_key_iv(std::vector<unsigned char>& key, std::vector<unsigned char>& iv, const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return false;
    }

    // 先读取文件大小来确定密钥和IV的长度
    file.seekg(0, std::ios::end);
    const size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (file_size < 28) { // 最小需要16+12=28字节
        std::cerr << "文件大小不正确" << std::endl;
        return false;
    }

    // 假设前16字节是密钥，剩余的是IV（通常12字节）
    key.resize(16);
    iv.resize(file_size - 16);

    // 读取：密钥(16字节) + IV(剩余字节)
    file.read(reinterpret_cast<char*>(key.data()), key.size());
    file.read(reinterpret_cast<char*>(iv.data()), iv.size());

    if (static_cast<size_t>(file.gcount()) != iv.size()) {
        std::cerr << "文件读取不完整" << std::endl;
        return false;
    }

    std::cout << "密钥和IV已从文件加载: " << filename << std::endl;
    return true;
}
