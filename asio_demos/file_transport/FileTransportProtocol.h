#pragma once
#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

#ifdef _WIN32 // Windows 平台
#    include <winsock2.h>
#    pragma comment(lib, "ws2_32.lib") // 链接 Winsock 库
#else // POSIX 平台 (Linux, macOS, Unix)
#    include <arpa/inet.h>
#endif

class FileTransportProtocol {
public:
    uint16_t type_len;
    std::string type;
    uint16_t file_name_len;
    std::string file_name;
    std::uint64_t file_size;

    FileTransportProtocol() : type_len(0), file_name_len(0), file_size(0) {}
    ~FileTransportProtocol() { rest(); };

    [[nodiscard]] size_t get_header_len() const {
        return sizeof(type_len) + type_len + sizeof(file_name_len) + file_name_len + sizeof(file_size);
    }

    [[nodiscard]] bool encode_header(char* buffer) const {
        if (!buffer) return false;
        size_t offset = 0;
        const uint16_t type_len_net = htons(type_len);
        if (type_len > 65535) return false;
        memcpy(buffer + offset, &type_len_net, sizeof(type_len));
        offset += sizeof(type_len);
        memcpy(buffer + offset, type.c_str(), type_len);
        offset += type_len;
        const uint16_t file_name_len_net = htons(file_name_len);
        memcpy(buffer + offset, &file_name_len_net, sizeof(file_name_len));

        if (file_name_len > 0) {
            if (file_name_len > 65535) return false;
            offset += sizeof(file_name_len);
            memcpy(buffer + offset, file_name.c_str(), file_name_len);
            offset += file_name_len;
            const uint64_t file_size_net = htonll(file_size);
            memcpy(buffer + offset, &file_size_net, sizeof(file_size));
        }
        return true;
    }

    void decode_header(const char* buffer) {
        if (!buffer) return;
        if (type_len > 0) {
            type.assign(buffer, type_len);
        }
        if (file_name_len > 0) {
            size_t offset = 0;
            file_name.assign(buffer + offset, file_name_len);
            offset += file_name_len;

            // 解析 file_size (使用 memcpy 避免未对齐访问)
            uint64_t decoded_file_size = 0;
            memcpy(&decoded_file_size, buffer + offset, sizeof(decoded_file_size));
            file_size = ntohll(decoded_file_size);
        }
    }
    void decode_header_len(const char* buffer, const bool is_type) {
        if (!buffer) return;
        size_t offset = 0;
        if (is_type) {
            uint16_t decoded_type_len = 0;
            memcpy(&decoded_type_len, buffer + offset, sizeof(decoded_type_len));
            type_len = ntohs(decoded_type_len);
        } else {
            offset += sizeof(type_len);
            offset += type_len;
            uint16_t decoded_file_name_len = 0;
            memcpy(&decoded_file_name_len, buffer + offset, sizeof(decoded_file_name_len));
            file_name_len = ntohs(decoded_file_name_len);
        }
    }

    void rest() {
        type_len = 0;
        type.clear();
        file_name_len = 0;
        file_name.clear();
        file_size = 0;
    }

    friend std::ostream& operator<<(std::ostream& os, const FileTransportProtocol& obj) {
        return os << "type_len: " << obj.type_len << ", type: " << obj.type << ", file_name_len: " << obj.file_name_len
                  << ", file_name: " << obj.file_name << ", file_size: " << obj.file_size;
    }
};
