#include "conversions.h"

std::string string_tolower(std::string s) {
    std::string lower_string = s;
    std::transform(lower_string.begin(), lower_string.end(),
                   lower_string.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return lower_string;
}

std::string path2string(fs::path p) {
    return p.generic_string();
}

fs::path string2path(std::string s) {
    return fs::u8path(s);
}
