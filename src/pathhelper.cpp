#include "pathhelper.h"
#include "conversions.h"

fs::path get_desktop_path() {
    static TCHAR path[MAX_PATH + 1];
    std::string path_string;
    std::wstring wStr;
    if (SHGetSpecialFolderPath(HWND_DESKTOP, path, CSIDL_DESKTOP, FALSE)) {
#ifndef UNICODE
        path_string = path;
#else
        wStr = path;
        path_string = std::string(wStr.begin(), wStr.end());
#endif
        return string2path(path_string);
    } else
        return "ERROR";
}

fs::path get_tmp_dir() {
    return fs::temp_directory_path();
}

fs::path get_cwd() {
    return fs::current_path();
}
