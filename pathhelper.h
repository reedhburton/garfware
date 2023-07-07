#ifndef _PATHHELPER_FILE
#define _PATHHELPER_FILE

#include <shlobj.h>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

fs::path get_desktop_path();
fs::path get_tmp_dir();
fs::path get_cwd();

#endif
