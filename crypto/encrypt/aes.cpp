#include <cstring>
#include <openssl/core_names.h>
#include <openssl/evp.h>
#include <string>
#include <vector>
#include "../common.hpp"

// AES-GCM 加密
std::vector<unsigned char> aes_gcm_encrypt(const std::vector<unsigned char>& plaintext, const std::vector<unsigned char>& key,
                                           const std::vector<unsigned char>& iv, std::vector<unsigned char>& tag) {
    size_t iv_len = iv.size();
    std::vector<unsigned char> ciphertext(plaintext.size() + iv_len);
    int ciphertext_len = 0, tmp_len;

    /* Create a context for the encrypt operation */
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handle_openssl_error();

    // 初始化加密,设置算法
    EVP_CIPHER* cipher = nullptr;
    const std::string cipher_name = "AES-" + std::to_string(key.size() * 8) + "-GCM";
    if ((cipher = EVP_CIPHER_fetch(nullptr, cipher_name.c_str(), nullptr)) == nullptr) handle_openssl_error();

    // 设置IV长度
    OSSL_PARAM params[2] = {OSSL_PARAM_construct_size_t(OSSL_CIPHER_PARAM_AEAD_IVLEN, &iv_len), OSSL_PARAM_END};

    // 初始化算法，密钥，IV
    if (!EVP_EncryptInit_ex2(ctx, cipher, key.data(), iv.data(), params)) handle_openssl_error();

    // 加密明文数据
    if (!EVP_EncryptUpdate(ctx, ciphertext.data(), &tmp_len, plaintext.data(), static_cast<int>(plaintext.size())))
        handle_openssl_error();
    ciphertext_len += tmp_len;

    // 处理加密的最后一块数据并完成加密操作
    if (!EVP_EncryptFinal_ex(ctx, ciphertext.data() + tmp_len, &tmp_len)) handle_openssl_error();
    ciphertext_len += tmp_len;

    // 获取GCM认证标签
    // 根据 NIST 标准，Tag 长度通常为 4 到 16 byte, 16 字节是最常用且安全性最高的选择
    tag.resize(16);
    params[0] = OSSL_PARAM_construct_octet_string(OSSL_CIPHER_PARAM_AEAD_TAG, tag.data(), 16);
    if (!EVP_CIPHER_CTX_get_params(ctx, params)) handle_openssl_error();

    EVP_CIPHER_free(cipher);
    EVP_CIPHER_CTX_free(ctx);

    // 加密完成后需要截断掉多余的预分配空间。
    ciphertext.resize(ciphertext_len);
    return ciphertext;
}

// AES-GCM 解密
std::vector<unsigned char> aes_gcm_decrypt(const std::vector<unsigned char>& ciphertext, const std::vector<unsigned char>& key,
                                           const std::vector<unsigned char>& iv, const std::vector<unsigned char>& tag) {
    size_t iv_len = iv.size();
    int plaintext_len = 0, tmp_len;
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handle_openssl_error();

    // 设置解密算法
    EVP_CIPHER* cipher = nullptr;
    const std::string cipher_name = "AES-" + std::to_string(key.size() * 8) + "-GCM";
    if ((cipher = EVP_CIPHER_fetch(nullptr, cipher_name.c_str(), nullptr)) == nullptr) handle_openssl_error();

    // 设置IV长度
    OSSL_PARAM params[2] = {OSSL_PARAM_construct_size_t(OSSL_CIPHER_PARAM_AEAD_IVLEN, &iv_len), OSSL_PARAM_END};

    // 初始化算法，密钥，IV
    if (!EVP_DecryptInit_ex2(ctx, cipher, key.data(), iv.data(), params)) handle_openssl_error();

    // 解密
    std::vector<unsigned char> plaintext(ciphertext.size() + 1);
    if (!EVP_DecryptUpdate(ctx, plaintext.data(), &tmp_len, ciphertext.data(), static_cast<int>(ciphertext.size())))
        handle_openssl_error();
    plaintext_len += tmp_len;

    // 设置预期的认证标签
    params[0] = OSSL_PARAM_construct_octet_string(OSSL_CIPHER_PARAM_AEAD_TAG, (void*)tag.data(), tag.size());
    if (!EVP_CIPHER_CTX_set_params(ctx, params)) handle_openssl_error();

    // 完成解密并验证认证标签
    const int ret = EVP_DecryptFinal_ex(ctx, plaintext.data() + tmp_len, &tmp_len);
    plaintext_len += tmp_len;

    if (ret <= 0) {
        std::cerr << "解密失败：认证标签验证错误" << std::endl;
        return {};
    }

    EVP_CIPHER_free(cipher);
    EVP_CIPHER_CTX_free(ctx);

    plaintext.resize(plaintext_len);
    return plaintext;
}

// AES-CTR 加密
std::vector<unsigned char> aes_ctr_encrypt(const std::vector<unsigned char>& plaintext, const std::vector<unsigned char>& key,
                                           const std::vector<unsigned char>& iv) {
    std::vector<unsigned char> ciphertext(plaintext.size());
    int ciphertext_len = 0, tmp_len;

    /* Create a context for the encrypt operation */
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handle_openssl_error();

    // 初始化加密,设置算法
    EVP_CIPHER* cipher = nullptr;
    const std::string cipher_name = "AES-" + std::to_string(key.size() * 8) + "-CTR";
    if ((cipher = EVP_CIPHER_fetch(nullptr, cipher_name.c_str(), nullptr)) == nullptr) handle_openssl_error();

    // CTR模式不需要特殊参数，直接初始化
    if (!EVP_EncryptInit_ex2(ctx, cipher, key.data(), iv.data(), nullptr)) handle_openssl_error();

    // 加密明文数据
    if (!EVP_EncryptUpdate(ctx, ciphertext.data(), &tmp_len, plaintext.data(), static_cast<int>(plaintext.size())))
        handle_openssl_error();
    ciphertext_len += tmp_len;

    // 处理加密的最后一块数据并完成加密操作
    if (!EVP_EncryptFinal_ex(ctx, ciphertext.data() + ciphertext_len, &tmp_len)) handle_openssl_error();
    ciphertext_len += tmp_len;

    EVP_CIPHER_free(cipher);
    EVP_CIPHER_CTX_free(ctx);

    // 截断多余空间
    ciphertext.resize(ciphertext_len);
    return ciphertext;
}

// AES-CTR 解密（CTR模式加解密相同）
std::vector<unsigned char> aes_ctr_decrypt(const std::vector<unsigned char>& ciphertext, const std::vector<unsigned char>& key,
                                           const std::vector<unsigned char>& iv) {
    int plaintext_len = 0, tmp_len;
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handle_openssl_error();

    // 设置解密算法
    EVP_CIPHER* cipher = nullptr;
    const std::string cipher_name = "AES-" + std::to_string(key.size() * 8) + "-CTR";
    if ((cipher = EVP_CIPHER_fetch(nullptr, cipher_name.c_str(), nullptr)) == nullptr) handle_openssl_error();

    // 初始化算法，密钥，IV
    if (!EVP_DecryptInit_ex2(ctx, cipher, key.data(), iv.data(), nullptr)) handle_openssl_error();

    // 解密
    std::vector<unsigned char> plaintext(ciphertext.size());
    if (!EVP_DecryptUpdate(ctx, plaintext.data(), &tmp_len, ciphertext.data(), static_cast<int>(ciphertext.size())))
        handle_openssl_error();
    plaintext_len += tmp_len;

    // 完成解密
    if (!EVP_DecryptFinal_ex(ctx, plaintext.data() + plaintext_len, &tmp_len)) handle_openssl_error();
    plaintext_len += tmp_len;

    EVP_CIPHER_free(cipher);
    EVP_CIPHER_CTX_free(ctx);

    plaintext.resize(plaintext_len);
    return plaintext;
}

// AES-CCM 加密
std::vector<unsigned char> aes_ccm_encrypt(const std::vector<unsigned char>& plaintext, const std::vector<unsigned char>& key,
                                           const std::vector<unsigned char>& iv, std::vector<unsigned char>& tag,
                                           const std::vector<unsigned char>& aad = {}) {
    size_t iv_len = iv.size();
    size_t tag_len = tag.size();
    std::vector<unsigned char> ciphertext(plaintext.size());
    int out_len = 0, tmp_len = 0;

    /* Create a context for the encrypt operation */
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handle_openssl_error();

    // 初始化加密,设置算法
    EVP_CIPHER* cipher = nullptr;
    const std::string cipher_name = "AES-" + std::to_string(key.size() * 8) + "-CCM";
    if ((cipher = EVP_CIPHER_fetch(nullptr, cipher_name.c_str(), nullptr)) == nullptr) handle_openssl_error();

    // 设置IV长度和Tag长度参数
    OSSL_PARAM params[3] = {OSSL_PARAM_END, OSSL_PARAM_END, OSSL_PARAM_END};
    params[0] = OSSL_PARAM_construct_size_t(OSSL_CIPHER_PARAM_AEAD_IVLEN, &iv_len);
    params[1] = OSSL_PARAM_construct_octet_string(OSSL_CIPHER_PARAM_AEAD_TAG, nullptr, tag_len);

    // 初始化加密操作，设置参数（不设置密钥和IV）
    if (!EVP_EncryptInit_ex2(ctx, cipher, nullptr, nullptr, params)) handle_openssl_error();

    // 设置密钥和IV
    if (!EVP_EncryptInit_ex(ctx, nullptr, nullptr, key.data(), iv.data())) handle_openssl_error();

    // CCM模式需要先设置明文长度
    if (!EVP_EncryptUpdate(ctx, nullptr, &out_len, nullptr, static_cast<int>(plaintext.size()))) handle_openssl_error();

    // 如果有AAD，先处理AAD（可选）
    if (!aad.empty()) {
        if (!EVP_EncryptUpdate(ctx, nullptr, &out_len, aad.data(), static_cast<int>(aad.size()))) handle_openssl_error();
    }

    // 加密明文数据
    if (!EVP_EncryptUpdate(ctx, ciphertext.data(), &out_len, plaintext.data(), static_cast<int>(plaintext.size())))
        handle_openssl_error();

    // 完成加密（CCM的Final不输出数据）
    if (!EVP_EncryptFinal_ex(ctx, nullptr, &tmp_len)) handle_openssl_error();

    // 获取认证标签
    tag.resize(tag_len);
    OSSL_PARAM tag_params[2] = {OSSL_PARAM_construct_octet_string(OSSL_CIPHER_PARAM_AEAD_TAG, tag.data(), tag_len),
                                OSSL_PARAM_END};
    if (!EVP_CIPHER_CTX_get_params(ctx, tag_params)) handle_openssl_error();

    EVP_CIPHER_free(cipher);
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext;
}

// AES-CCM 解密
std::vector<unsigned char> aes_ccm_decrypt(const std::vector<unsigned char>& ciphertext, const std::vector<unsigned char>& key,
                                           const std::vector<unsigned char>& iv, const std::vector<unsigned char>& tag,
                                           const std::vector<unsigned char>& aad = {}) {
    size_t iv_len = iv.size();
    const size_t tag_len = tag.size();
    int out_len = 0;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handle_openssl_error();

    // 设置解密算法
    EVP_CIPHER* cipher = nullptr;
    const std::string cipher_name = "AES-" + std::to_string(key.size() * 8) + "-CCM";
    if ((cipher = EVP_CIPHER_fetch(nullptr, cipher_name.c_str(), nullptr)) == nullptr) handle_openssl_error();

    // 设置IV长度和预期的Tag
    OSSL_PARAM params[3] = {OSSL_PARAM_END, OSSL_PARAM_END, OSSL_PARAM_END};
    params[0] = OSSL_PARAM_construct_size_t(OSSL_CIPHER_PARAM_AEAD_IVLEN, &iv_len);
    params[1] = OSSL_PARAM_construct_octet_string(OSSL_CIPHER_PARAM_AEAD_TAG, (void*)tag.data(), tag_len);

    // 初始化解密操作，设置参数（不设置密钥和IV）
    if (!EVP_DecryptInit_ex2(ctx, cipher, nullptr, nullptr, params)) handle_openssl_error();

    // 设置密钥和IV
    if (!EVP_DecryptInit_ex(ctx, nullptr, nullptr, key.data(), iv.data())) handle_openssl_error();

    // CCM模式需要先设置密文长度
    if (!EVP_DecryptUpdate(ctx, nullptr, &out_len, nullptr, static_cast<int>(ciphertext.size()))) handle_openssl_error();

    // 如果有AAD，先处理AAD（必须与加密时相同）
    if (!aad.empty()) {
        if (!EVP_DecryptUpdate(ctx, nullptr, &out_len, aad.data(), static_cast<int>(aad.size()))) handle_openssl_error();
    }

    // 解密密文并验证标签
    std::vector<unsigned char> plaintext(ciphertext.size());

    if (const int ret =
            EVP_DecryptUpdate(ctx, plaintext.data(), &out_len, ciphertext.data(), static_cast<int>(ciphertext.size()));
        ret <= 0) {
        std::cerr << "解密失败：认证标签验证错误" << std::endl;
        EVP_CIPHER_free(cipher);
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }

    EVP_CIPHER_free(cipher);
    EVP_CIPHER_CTX_free(ctx);

    plaintext.resize(out_len);
    return plaintext;
}

// ========== 主函数演示 ==========

int main() {
    // ===== 0. 准备测试明文 =====
    const auto plaintext_str = "Hello, OpenSSL! 这是一个 OpenSSL 加密与认证演示演示。";
    const std::vector<unsigned char> plaintext(plaintext_str, plaintext_str + strlen(plaintext_str));
    std::cout << "原始数据: " << plaintext_str << std::endl;
    std::cout << "原始长度: " << plaintext.size() << " 字节" << std::endl;

    // ===== 1.0 生成密钥(长度128,192,256bits, 需转换成字节)和IV =====
    std::cout << "\n1. 生成AES-128-GCM密钥和IV" << std::endl;
    std::vector<unsigned char> key(16), iv(12);
    generate_key_iv(key, iv);

    // ===== 1.1 保存到文件=====
    save_key_iv(key, iv, "aes_key.bin");

    // 根据 NIST 标准GCM与CCM tag长度均为4-16 字节，16 字节（128 位）提供最强的认证保护
    std::vector<unsigned char> tag(16); // GCM认证标签

    // ===== 2.1 AES-GCM 加密示例 =====
    std::cout << "\n2.1 AES-128-GCM 加密演示" << std::endl;
    const auto gcm_cipher_text = aes_gcm_encrypt(plaintext, key, iv, tag);
    std::cout << "密文长度: " << gcm_cipher_text.size() << " 字节" << std::endl;
    print_hex(gcm_cipher_text);
    std::cout << "认证标签: ";
    print_hex(tag);

    // ===== 2.2 AES=GCM 解密示例 =====
    std::cout << "\n2.2  从文件加载密钥后再解密" << std::endl;
    std::vector<unsigned char> loaded_iv(12), loaded_key(16);
    if (!load_key_iv(loaded_key, loaded_iv, "aes_key.bin")) {
        std::cerr << "从文件读取密钥失败" << std::endl;
    }
    if (auto gcm_plain_text = aes_gcm_decrypt(gcm_cipher_text, loaded_key, loaded_iv, tag); !gcm_plain_text.empty()) {
        std::cout << "使用加载的密钥解密成功！" << std::endl;
        std::cout << "解密后的数据: " << std::string(gcm_plain_text.begin(), gcm_plain_text.end()) << std::endl;
    }

    // ===== 3.1 AES-CTR 加密示例 =====
    std::cout << "\n3.1 AES-128-CRT 加密演示" << std::endl;
    const auto ctr_cipher_text = aes_ctr_encrypt(plaintext, key, iv);
    std::cout << "密文长度: " << ctr_cipher_text.size() << " 字节" << std::endl;
    print_hex(ctr_cipher_text);

    // ===== 3.2 AES-CTR 解密示例 =====
    std::cout << "\n3.2 AES-128-CRT 解密演示" << std::endl;
    const auto ctr_plain_text = aes_ctr_decrypt(ctr_cipher_text, key, iv);
    std::cout << "解密后长度: " << ctr_plain_text.size() << " 字节" << std::endl;
    std::cout << "解密后的数据: " << std::string(ctr_plain_text.begin(), ctr_plain_text.end()) << std::endl;

    // ===== 4.1 AES-CCM 加密示例 =====
    std::cout << "\n4.1 AES-128-CCM 加密演示" << std::endl;
    tag.clear();
    tag.resize(16);
    const auto ccm_cipher_text = aes_ccm_encrypt(plaintext, key, iv, tag);
    std::cout << "密文长度: " << ccm_cipher_text.size() << " 字节" << std::endl;
    print_hex(ccm_cipher_text);
    std::cout << "认证标签: ";
    print_hex(tag);

    // ===== 4.2 AES-CCM 解密示例 =====
    std::cout << "\n4.2 AES-128-CCM 解密演示" << std::endl;
    const auto ccm_plain_text = aes_ccm_decrypt(ccm_cipher_text, key, iv, tag);
    std::cout << "解密后长度: " << ccm_plain_text.size() << " 字节" << std::endl;
    std::cout << "解密后的数据: " << std::string(ccm_plain_text.begin(), ccm_plain_text.end()) << std::endl;

    return 0;
}
