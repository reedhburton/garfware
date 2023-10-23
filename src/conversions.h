#ifndef _CONVERSIONS_H
#define _CONVERSIONS_H

#include <algorithm>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

std::string string_tolower(std::string s);
std::string path2string(fs::path p);
fs::path string2path(std::string s);

#endif
