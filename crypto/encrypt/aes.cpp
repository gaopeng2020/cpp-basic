#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <iomanip>

// 错误处理辅助函数
void handle_openssl_error() {
    ERR_print_errors_fp(stderr);
    exit(1);
}

// 十六进制打印辅助函数
void print_hex(const unsigned char* data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                  << (int)data[i];
    }
    std::cout << std::dec << std::endl;
}

// ========== 1. AES-128-GCM 加解密 ==========

// 生成随机密钥和IV
void generate_key_iv(unsigned char* key, unsigned char* iv) {
    // AES-128 密钥长度 = 16字节 (128位)
    if (RAND_bytes(key, 16) != 1) {
        handle_openssl_error();
    }
    // GCM推荐IV长度 = 12字节
    if (RAND_bytes(iv, 12) != 1) {
        handle_openssl_error();
    }
}

// AES-128-GCM 加密
std::vector<unsigned char> aes_gcm_encrypt(
    const unsigned char* plaintext, size_t plaintext_len,
    const unsigned char* key, const unsigned char* iv,
    unsigned char* tag) {
    
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handle_openssl_error();
    
    // 初始化加密操作
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL))
        handle_openssl_error();
    
    // 设置IV长度
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 12, NULL))
        handle_openssl_error();
    
    // 初始化密钥和IV
    if (1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv))
        handle_openssl_error();
    
    // 加密
    std::vector<unsigned char> ciphertext(plaintext_len + 16);
    int len;
    if (1 != EVP_EncryptUpdate(ctx, ciphertext.data(), &len, 
                                plaintext, plaintext_len))
        handle_openssl_error();
    
    int ciphertext_len = len;
    
    // 完成加密
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len))
        handle_openssl_error();
    ciphertext_len += len;
    
    // 获取GCM认证标签
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag))
        handle_openssl_error();
    
    EVP_CIPHER_CTX_free(ctx);
    
    ciphertext.resize(ciphertext_len);
    return ciphertext;
}

// AES-128-GCM 解密
std::vector<unsigned char> aes_gcm_decrypt(
    const unsigned char* ciphertext, size_t ciphertext_len,
    const unsigned char* key, const unsigned char* iv,
    const unsigned char* tag) {
    
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handle_openssl_error();
    
    // 初始化解密操作
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL))
        handle_openssl_error();
    
    // 设置IV长度
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 12, NULL))
        handle_openssl_error();
    
    // 初始化密钥和IV
    if (1 != EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv))
        handle_openssl_error();
    
    // 解密
    std::vector<unsigned char> plaintext(ciphertext_len + 1);
    int len;
    if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len,
                                ciphertext, ciphertext_len))
        handle_openssl_error();
    
    int plaintext_len = len;
    
    // 设置预期的认证标签
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, (void*)tag))
        handle_openssl_error();
    
    // 完成解密并验证认证标签
    int ret = EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len);
    EVP_CIPHER_CTX_free(ctx);
    
    if (ret <= 0) {
        std::cerr << "解密失败：认证标签验证错误" << std::endl;
        return {};
    }
    
    plaintext_len += len;
    plaintext.resize(plaintext_len);
    return plaintext;
}

// ========== 2. 密钥存储与读取 ==========

// 保存密钥和IV到文件（二进制格式）
void save_key_iv(const unsigned char* key, const unsigned char* iv, 
                 const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "无法创建文件: " << filename << std::endl;
        return;
    }
    
    // 写入：密钥(16字节) + IV(12字节)
    file.write(reinterpret_cast<const char*>(key), 16);
    file.write(reinterpret_cast<const char*>(iv), 12);
    
    std::cout << "密钥和IV已保存到: " << filename << std::endl;
    std::cout << "  - 密钥(16字节): ";
    print_hex(key, 16);
    std::cout << "  - IV(12字节): ";
    print_hex(iv, 12);
}

// 从文件读取密钥和IV
bool load_key_iv(unsigned char* key, unsigned char* iv, 
                 const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return false;
    }
    
    // 读取：密钥(16字节) + IV(12字节)
    file.read(reinterpret_cast<char*>(key), 16);
    file.read(reinterpret_cast<char*>(iv), 12);
    
    if (file.gcount() != 28) { // 16+12=28
        std::cerr << "文件大小不正确" << std::endl;
        return false;
    }
    
    std::cout << "密钥和IV已从文件加载: " << filename << std::endl;
    return true;
}

// ========== 3. 摘要（SHA256和MD5） ==========

// SHA256哈希
std::vector<unsigned char> sha256(const unsigned char* data, size_t len) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) handle_openssl_error();
    
    std::vector<unsigned char> hash(EVP_MD_size(EVP_sha256()));
    
    if (1 != EVP_DigestInit_ex(ctx, EVP_sha256(), NULL))
        handle_openssl_error();
    if (1 != EVP_DigestUpdate(ctx, data, len))
        handle_openssl_error();
    if (1 != EVP_DigestFinal_ex(ctx, hash.data(), NULL))
        handle_openssl_error();
    
    EVP_MD_CTX_free(ctx);
    return hash;
}

// MD5哈希
std::vector<unsigned char> md5(const unsigned char* data, size_t len) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) handle_openssl_error();
    
    std::vector<unsigned char> hash(EVP_MD_size(EVP_md5()));
    
    if (1 != EVP_DigestInit_ex(ctx, EVP_md5(), NULL))
        handle_openssl_error();
    if (1 != EVP_DigestUpdate(ctx, data, len))
        handle_openssl_error();
    if (1 != EVP_DigestFinal_ex(ctx, hash.data(), NULL))
        handle_openssl_error();
    
    EVP_MD_CTX_free(ctx);
    return hash;
}

// ========== 主函数演示 ==========

int main() {
    // 初始化OpenSSL
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
    
    std::cout << "========== OpenSSL 加密演示 ==========" << std::endl;
    
    // ===== 1. 生成AES-128-GCM密钥和IV =====
    std::cout << "\n1. 生成AES-128-GCM密钥和IV" << std::endl;
    unsigned char key[16], iv[12];
    generate_key_iv(key, iv);
    
    // 保存到文件
    save_key_iv(key, iv, "aes_key.bin");
    
    // ===== 2. 加密示例 =====
    std::cout << "\n2. AES-128-GCM 加密演示" << std::endl;
    const char* plaintext = "Hello, OpenSSL! 这是一个测试消息。";
    size_t plaintext_len = strlen(plaintext);
    
    std::cout << "原始数据: " << plaintext << std::endl;
    std::cout << "原始长度: " << plaintext_len << " 字节" << std::endl;
    
    unsigned char tag[16];  // GCM认证标签
    auto ciphertext = aes_gcm_encrypt(
        reinterpret_cast<const unsigned char*>(plaintext), 
        plaintext_len, key, iv, tag);
    
    std::cout << "密文长度: " << ciphertext.size() << " 字节" << std::endl;
    std::cout << "认证标签: ";
    print_hex(tag, 16);
    
    // ===== 3. 解密示例 =====
    std::cout << "\n3. AES-128-GCM 解密演示" << std::endl;
    auto decrypted = aes_gcm_decrypt(
        ciphertext.data(), ciphertext.size(), key, iv, tag);
    
    if (!decrypted.empty()) {
        std::cout << "解密成功！" << std::endl;
        std::cout << "解密数据: " << std::string(decrypted.begin(), decrypted.end()) << std::endl;
    }
    
    // ===== 4. 从文件加载密钥进行解密 =====
    std::cout << "\n4. 从文件加载密钥后再解密" << std::endl;
    unsigned char loaded_key[16], loaded_iv[12];
    if (load_key_iv(loaded_key, loaded_iv, "aes_key.bin")) {
        auto decrypted2 = aes_gcm_decrypt(
            ciphertext.data(), ciphertext.size(), 
            loaded_key, loaded_iv, tag);
        
        if (!decrypted2.empty()) {
            std::cout << "使用加载的密钥解密成功！" << std::endl;
            std::cout << "解密数据: " << std::string(decrypted2.begin(), decrypted2.end()) << std::endl;
        }
    }
    
    // ===== 5. SHA256摘要 =====
    std::cout << "\n5. SHA256 哈希演示" << std::endl;
    auto sha256_hash = sha256(
        reinterpret_cast<const unsigned char*>(plaintext), plaintext_len);
    
    std::cout << "原文: " << plaintext << std::endl;
    std::cout << "SHA256: ";
    print_hex(sha256_hash.data(), sha256_hash.size());
    
    // ===== 6. MD5摘要 =====
    std::cout << "\n6. MD5 哈希演示" << std::endl;
    auto md5_hash = md5(
        reinterpret_cast<const unsigned char*>(plaintext), plaintext_len);
    
    std::cout << "原文: " << plaintext << std::endl;
    std::cout << "MD5: ";
    print_hex(md5_hash.data(), md5_hash.size());
    
    // ===== 7. 演示认证标签的保护作用 =====
    std::cout << "\n7. 演示GCM的完整性验证" << std::endl;
    std::cout << "尝试修改密文后解密..." << std::endl;
    
    auto corrupted_ciphertext = ciphertext;
    if (!corrupted_ciphertext.empty()) {
        corrupted_ciphertext[0] ^= 0x01; // 修改第一个字节
        
        auto failed_decrypt = aes_gcm_decrypt(
            corrupted_ciphertext.data(), corrupted_ciphertext.size(),
            key, iv, tag);
        
        if (failed_decrypt.empty()) {
            std::cout << "✓ 正确检测到数据被篡改，解密失败！" << std::endl;
        }
    }
    
    // 清理
    EVP_cleanup();
    ERR_free_strings();
    
    return 0;
}