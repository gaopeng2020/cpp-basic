#include <iostream>
#include <string>
#include <regex>
/*
模式 `std::regex` 本质是 通过 `std::regex_match` 进行匹配，从而产生 `std::smatch` （本质是 `std::match_results` 对象）
另一种常用的形式就是依次传入 `std::string`/`std::smatch`/`std::regex` 三个参数，
*/

int main() {
    std::string fnames[] = {"foo.txt", "bar.txt", "test", "a0.txt", "AAA.txt"};
    std::regex txt_regex("[a-z]+\\.txt");
    for (const auto &fname: fnames)
        std::cout << fname << ": " << std::regex_match(fname, txt_regex) << std::endl;
    
    std::regex base_regex("([a-z]+)\\.txt");
    std::smatch base_match;
    for(const auto &fname: fnames) {
        if (std::regex_match(fname, base_match, base_regex)) {
            // the first element of std::smatch matches the entire string
            // the second element of std::smatch matches the first expression with brackets
            if (base_match.size() == 2) {
                std::string base = base_match[1].str();
                std::cout << "sub-match[0]: " << base_match[0].str() << std::endl;
                std::cout << fname << " sub-match[1]: " << base << std::endl;
            }
        }
    }
    
    return 0;
}
