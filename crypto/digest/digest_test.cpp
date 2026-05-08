#include <vector>
#include "../common.hpp"
// ========== 3. 摘要（SHA256和MD5） ==========

// SHA256哈希
std::vector<unsigned char> sha256(const unsigned char* data, const size_t len) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) handle_openssl_error();

    std::vector<unsigned char> hash(EVP_MD_size(EVP_sha256()));

    if (1 != EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr)) handle_openssl_error();
    if (1 != EVP_DigestUpdate(ctx, data, len)) handle_openssl_error();
    if (1 != EVP_DigestFinal_ex(ctx, hash.data(), nullptr)) handle_openssl_error();

    EVP_MD_CTX_free(ctx);
    return hash;
}

// MD5哈希
std::vector<unsigned char> md5(const unsigned char* data, const size_t len) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) handle_openssl_error();

    std::vector<unsigned char> hash(EVP_MD_size(EVP_md5()));

    if (1 != EVP_DigestInit_ex(ctx, EVP_md5(), nullptr)) handle_openssl_error();
    if (1 != EVP_DigestUpdate(ctx, data, len)) handle_openssl_error();
    if (1 != EVP_DigestFinal_ex(ctx, hash.data(), nullptr)) handle_openssl_error();

    EVP_MD_CTX_free(ctx);
    return hash;
}

int main() {
    const auto plaintext = "Hello, OpenSSL! ===== SHA256摘要 =====";
    const size_t plaintext_len = strlen(plaintext);

    std::cout << "\n5. SHA256 哈希演示" << std::endl;
    const auto sha256_hash = sha256(reinterpret_cast<const unsigned char*>(plaintext), plaintext_len);

    std::cout << "原文: " << plaintext << std::endl;
    std::cout << "SHA256: ";
    print_hex(sha256_hash.data(), sha256_hash.size());

    // ===== 6. MD5摘要 =====
    std::cout << "\n6. MD5 哈希演示" << std::endl;
    const auto md5_hash = md5(reinterpret_cast<const unsigned char*>(plaintext), plaintext_len);

    std::cout << "原文: " << plaintext << std::endl;
    std::cout << "MD5: ";
    print_hex(md5_hash.data(), md5_hash.size());

    return 0;
}
