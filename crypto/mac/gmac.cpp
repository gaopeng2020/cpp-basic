#include <chrono>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <map>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>

// 辅助函数：打印十六进制
void print_hex(const unsigned char* data, size_t len, const std::string& label = "") {
    if (!label.empty()) std::cout << label << ": ";
    for (size_t i = 0; i < len; i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)data[i];
    }
    std::cout << std::dec << std::endl;
}

// ========== GMAC 核心操作 ==========

/**
 * 使用GMAC生成认证标签
 * @param key AES密钥（16/24/32字节）
 * @param key_len 密钥长度
 * @param iv 初始化向量（Nonce）
 * @param iv_len Nonce长度（推荐12字节）
 * @param data 需要认证的数据
 * @param data_len 数据长度
 * @param tag 输出的认证标签（通常16字节）
 * @return 成功返回true
 */
bool gmac_generate_tag(const unsigned char* key, int key_len, const unsigned char* iv, size_t iv_len, const unsigned char* data,
                       size_t data_len, unsigned char* tag) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return false;

    // 根据密钥长度选择AES
    const EVP_CIPHER* cipher = nullptr;
    switch (key_len) {
        case 16:
            cipher = EVP_aes_128_gcm();
            break;
        case 24:
            cipher = EVP_aes_192_gcm();
            break;
        case 32:
            cipher = EVP_aes_256_gcm();
            break;
        default:
            return false;
    }

    // 初始化加密操作（GMAC是GCM模式没有明文数据）
    if (1 != EVP_EncryptInit_ex(ctx, cipher, nullptr, nullptr, nullptr)) return false;

    // 设置IV长度
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv_len, nullptr)) return false;

    // 初始化密钥和IV
    if (1 != EVP_EncryptInit_ex(ctx, nullptr, nullptr, key, iv)) return false;

    // 提供需要认证的数据（AAD - Additional Authenticated Data）
    int len;
    if (data_len > 0) {
        if (1 != EVP_EncryptUpdate(ctx, nullptr, &len, data, data_len)) return false;
    }

    // 完成并获取标签
    if (1 != EVP_EncryptFinal_ex(ctx, nullptr, &len)) return false;

    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag)) return false;

    EVP_CIPHER_CTX_free(ctx);
    return true;
}

/**
 * 验证GMAC标签
 */
bool gmac_verify_tag(const unsigned char* key, int key_len, const unsigned char* iv, size_t iv_len, const unsigned char* data,
                     size_t data_len, const unsigned char* expected_tag) {
    unsigned char computed_tag[16];

    if (!gmac_generate_tag(key, key_len, iv, iv_len, data, data_len, computed_tag)) {
        return false;
    }

    // 恒定时间比较（防止计时攻击）
    return CRYPTO_memcmp(computed_tag, expected_tag, 16) == 0;
}

// ========== Nonce管理策略 ==========

/**
 * 策略1：使用随机数（适用于低频操作）
 */
void random_nonce_strategy() {
    std::cout << "\n=== 策略1: 随机Nonce ===" << std::endl;

    unsigned char key[16];
    unsigned char nonce[12];
    unsigned char tag[16];
    const char* message = "Hello GMAC!";

    // 生成随机密钥
    RAND_bytes(key, sizeof(key));

    // 每次操作都生成新的随机Nonce
    RAND_bytes(nonce, sizeof(nonce));

    std::cout << "第一次操作 - 随机Nonce: ";
    print_hex(nonce, sizeof(nonce));

    if (gmac_generate_tag(key, sizeof(key), nonce, sizeof(nonce), (const unsigned char*)message, strlen(message), tag)) {
        std::cout << "生成的Tag: ";
        print_hex(tag, 16);
    }

    // 第二次操作：生成新Nonce
    RAND_bytes(nonce, sizeof(nonce));
    std::cout << "\n第二次操作 - 新随机Nonce: ";
    print_hex(nonce, sizeof(nonce));

    if (gmac_generate_tag(key, sizeof(key), nonce, sizeof(nonce), (const unsigned char*)message, strlen(message), tag)) {
        std::cout << "新生成的Tag: ";
        print_hex(tag, 16);
    }

    std::cout << "✓ 优点：简单，无需状态管理" << std::endl;
    std::cout << "✗ 缺点：每次需要12字节随机数，碰撞概率与生日攻击相关" << std::endl;
}

/**
 * 策略2：计数器方式（推荐用于高频操作）
 */
class CounterNonceManager {
private:
    unsigned char base_nonce[12]; // 基础Nonce（固定前缀）
    uint32_t counter; // 计数器
    std::map<uint32_t, bool> used_counters; // 记录已使用的计数器（演示用）

public:
    CounterNonceManager(const unsigned char* prefix, size_t prefix_len) {
        // 基础Nonce：前(12-4)=8字节固定，后4字节留给计数器
        memset(base_nonce, 0, sizeof(base_nonce));
        memcpy(base_nonce, prefix, std::min(prefix_len, sizeof(base_nonce) - 4));
        counter = 0;
    }

    // 获取下一个Nonce（线程安全版本需要加锁）
    void get_next_nonce(unsigned char* output) {
        memcpy(output, base_nonce, sizeof(base_nonce));

        // 将计数器写入最后4字节（大端序）
        uint32_t counter_val = counter++;
        output[8] = (counter_val >> 24) & 0xFF;
        output[9] = (counter_val >> 16) & 0xFF;
        output[10] = (counter_val >> 8) & 0xFF;
        output[11] = counter_val & 0xFF;

        // 记录已使用（演示用）
        used_counters[counter_val] = true;
    }

    void print_stats() {
        std::cout << "已分配的Nonce数量: " << used_counters.size() << std::endl;
        std::cout << "下一个计数器值: " << counter << std::endl;
    }

    bool is_counter_used(uint32_t c) { return used_counters.find(c) != used_counters.end(); }
};

void counter_nonce_strategy() {
    std::cout << "\n=== 策略2: 计数器Nonce（推荐） ===" << std::endl;

    unsigned char key[16];
    unsigned char nonce[12];
    unsigned char tag[16];
    const char* message = "Important data";

    // 生成随机密钥和基础前缀
    RAND_bytes(key, sizeof(key));
    unsigned char prefix[8];
    RAND_bytes(prefix, sizeof(prefix));

    CounterNonceManager nonce_mgr(prefix, sizeof(prefix));

    // 模拟多次操作
    for (int i = 0; i < 5; i++) {
        nonce_mgr.get_next_nonce(nonce);

        std::cout << "\n操作 #" << (i + 1) << " Nonce: ";
        print_hex(nonce, sizeof(nonce));

        if (gmac_generate_tag(key, sizeof(key), nonce, sizeof(nonce), (const unsigned char*)message, strlen(message), tag)) {
            std::cout << "Tag: ";
            print_hex(tag, 16);
        }
    }

    nonce_mgr.print_stats();
    std::cout << "✓ 优点：保证唯一性，性能好，适合高频操作" << std::endl;
    std::cout << "✗ 缺点：需要维护状态，多线程需要同步" << std::endl;
}

/**
 * 策略3：时间戳+计数器（分布式系统）
 */
void timestamp_counter_strategy() {
    std::cout << "\n=== 策略3: 时间戳+计数器（分布式友好） ===" << std::endl;

    unsigned char key[16];
    unsigned char nonce[12];
    RAND_bytes(key, sizeof(key));

    // 获取时间戳（毫秒）
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    uint64_t timestamp = static_cast<uint64_t>(ms);
    static uint32_t seq = 0;

    // Nonce结构：[时间戳高48位(6字节)][序列号(2字节)][随机填充(4字节)]
    memset(nonce, 0, sizeof(nonce));
    memcpy(nonce, &timestamp, 6); // 前6字节：时间戳
    memcpy(nonce + 6, &seq, 2); // 接着2字节：序列号
    RAND_bytes(nonce + 8, 4); // 最后4字节：随机数

    seq++;

    std::cout << "时间戳: " << timestamp << std::endl;
    std::cout << "序列号: " << (seq - 1) << std::endl;
    std::cout << "组合Nonce: ";
    print_hex(nonce, sizeof(nonce));

    unsigned char tag[16];
    const char* msg = "Distributed system message";

    if (gmac_generate_tag(key, sizeof(key), nonce, sizeof(nonce), (const unsigned char*)msg, strlen(msg), tag)) {
        std::cout << "Tag: ";
        print_hex(tag, 16);
    }

    std::cout << "✓ 优点：无需中心协调器，适合分布式系统" << std::endl;
    std::cout << "✗ 缺点：依赖时钟同步，nonce空间利用率低" << std::endl;
}

// ========== 演示Nonce重复的危害 ==========
void demonstrate_nonce_reuse_danger() {
    std::cout << "\n=== 警告：Nonce重复的危害演示 ===" << std::endl;

    unsigned char key[16];
    unsigned char nonce[12];
    unsigned char tag1[16], tag2[16];

    // 固定密钥和Nonce（人为制造重复）
    RAND_bytes(key, sizeof(key));
    memset(nonce, 0, sizeof(nonce)); // 固定为全0

    const char* message1 = "Secret message A";
    const char* message2 = "Secret message B";

    std::cout << "使用相同的密钥和Nonce..." << std::endl;
    std::cout << "Key: ";
    print_hex(key, sizeof(key));
    std::cout << "Nonce: ";
    print_hex(nonce, sizeof(nonce));

    // 生成两个不同消息的标签
    gmac_generate_tag(key, sizeof(key), nonce, sizeof(nonce), (const unsigned char*)message1, strlen(message1), tag1);
    gmac_generate_tag(key, sizeof(key), nonce, sizeof(nonce), (const unsigned char*)message2, strlen(message2), tag2);

    std::cout << "\n消息1: \"" << message1 << "\"" << std::endl;
    std::cout << "Tag1: ";
    print_hex(tag1, 16);

    std::cout << "\n消息2: \"" << message2 << "\"" << std::endl;
    std::cout << "Tag2: ";
    print_hex(tag2, 16);

    std::cout << "\n⚠️  安全问题：攻击者看到两个不同的Tag使用了相同Nonce，" << std::endl;
    std::cout << "    可以通过数学分析推导出密钥信息！" << std::endl;
    std::cout << "    在实际应用中，Nonce重复是严重的安全漏洞。" << std::endl;
}

// ========== 实际应用示例 ==========
void practical_usage_example() {
    std::cout << "\n=== 实际应用：API请求签名 ===" << std::endl;

    // 模拟客户端和服务器共享密钥
    unsigned char shared_key[16];
    RAND_bytes(shared_key, sizeof(shared_key));

    // 客户端：构造请求并生成GMAC标签
    class APIRequest {
    private:
        std::string endpoint;
        std::string params;
        uint64_t timestamp;
        uint32_t nonce_counter;

    public:
        APIRequest(const std::string& ep, const std::string& p) : endpoint(ep), params(p), nonce_counter(0) {
            timestamp =
                std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        }

        std::vector<unsigned char> sign(const unsigned char* key) {
            // 构造待认证数据
            std::string auth_data =
                endpoint + "|" + params + "|" + std::to_string(timestamp) + "|" + std::to_string(nonce_counter++);

            // 生成Nonce（使用计数器策略）
            unsigned char nonce[12];
            memset(nonce, 0, sizeof(nonce));
            memcpy(nonce, &timestamp, 8); // 前8字节：时间戳
            memcpy(nonce + 8, &nonce_counter, 4); // 后4字节：计数器

            // 生成GMAC标签
            std::vector<unsigned char> tag(16);
            gmac_generate_tag(key, 16, nonce, sizeof(nonce), (const unsigned char*)auth_data.c_str(), auth_data.length(),
                              tag.data());

            std::cout << "请求: " << endpoint << "?" << params << std::endl;
            std::cout << "时间戳: " << timestamp << std::endl;
            std::cout << "Nonce计数器: " << (nonce_counter - 1) << std::endl;
            std::cout << "生成的签名: ";
            print_hex(tag.data(), tag.size());

            return tag;
        }
    };

    // 客户端发送请求
    APIRequest req("/api/user", "id=123&action=get");
    auto tag = req.sign(shared_key);

    // 服务器验证（模拟）
    std::cout << "\n服务器验证..." << std::endl;
    // 注意：实际场景中服务器需要重新计算并比较
    std::cout << "✓ 签名验证通过" << std::endl;
}

// ========== 主函数 ==========
int main() {
    // 初始化OpenSSL
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    std::cout << "========== GMAC (AES-GMAC) 演示 ==========" << std::endl;
    std::cout << "GMAC = AES-GCM 模式用于消息认证（无加密数据）" << std::endl;

    // 基础示例
    std::cout << "\n--- 基础GMAC示例 ---" << std::endl;
    unsigned char key[16];
    unsigned char nonce[12];
    unsigned char tag[16];
    const char* message = "Hello GMAC! This is authentication only.";

    // 生成随机密钥和Nonce
    RAND_bytes(key, sizeof(key));
    RAND_bytes(nonce, sizeof(nonce));

    std::cout << "密钥: ";
    print_hex(key, sizeof(key));
    std::cout << "Nonce: ";
    print_hex(nonce, sizeof(nonce));
    std::cout << "消息: " << message << std::endl;

    // 生成GMAC标签
    if (gmac_generate_tag(key, sizeof(key), nonce, sizeof(nonce), (const unsigned char*)message, strlen(message), tag)) {
        std::cout << "GMAC标签: ";
        print_hex(tag, 16);

        // 验证
        if (gmac_verify_tag(key, sizeof(key), nonce, sizeof(nonce), (const unsigned char*)message, strlen(message), tag)) {
            std::cout << "✓ 验证成功" << std::endl;
        }
    }

    // 演示Nonce重复的危害
    demonstrate_nonce_reuse_danger();

    // Nonce管理策略
    random_nonce_strategy();
    counter_nonce_strategy();
    timestamp_counter_strategy();

    // 实际应用示例
    practical_usage_example();

    // 清理
    EVP_cleanup();
    ERR_free_strings();

    return 0;
}
