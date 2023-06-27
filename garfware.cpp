#include "garfware.h"

std::list<fs::path> get_shortcuts_in_dir(fs::path dir) {
    std::list<fs::path> shortcuts;
    for (const auto& entry : fs::directory_iterator(dir))
        if (entry.path().extension() == ".url" ||
            entry.path().extension() == ".lnk") {
            shortcuts.push_back(entry.path());
        }
    return shortcuts;
}

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

int set_wallpaper(fs::path wallpaper_path) {
    bool result = SystemParametersInfoA(
        SPI_SETDESKWALLPAPER, 0, (PVOID*)path2string(wallpaper_path).c_str(),
        SPIF_SENDCHANGE);
    if (result) {
        printf("Wallpaper changed successfully!\n");
        return 0;
    }
    return -1;
}

int print_string_list(std::list<fs::path> strings) {
    for (const auto& string : strings) {
        std::cout << string << std::endl;
    }
    return 0;
}

bool set_lnk_icon(fs::path shortcut_path, fs::path icon_path) {
    return ResolveIt(NULL, path2string(shortcut_path).c_str(),
                     path2string(icon_path).c_str());
}

bool set_url_icon(fs::path shortcut_path, fs::path icon_path) {
    fs::path temp_file_path = get_tmp_dir() / "temp.url";
    std::ifstream filein(shortcut_path);    // File to read from
    std::ofstream fileout(temp_file_path);  // Temporary file
    if (!filein || !fileout) {
        std::cout << "Error opening files!" << std::endl;
        return false;
    }
    std::string strTemp;
    bool found = false;
    while (filein >> strTemp) {
        std::string lower_line = string_tolower(strTemp);
        if (lower_line.find("iconfile") != std::string::npos) {
            strTemp = "IconFile=" + path2string(icon_path);
            found = true;
            std::cout << "Found icon file!" << std::endl;
        } else if (lower_line.find("iconindex") != std::string::npos) {
            strTemp = "IconIndex=0";
            std::cout << "Found icon index!" << std::endl;
        }
        fileout << strTemp << "\n";
        if (found)
            break;
    }
    filein.close();
    fileout.close();
    if (found) {
        if (remove(path2string(shortcut_path).c_str()) == 0) {
            if (std::rename(path2string(temp_file_path).c_str(),
                            path2string(shortcut_path).c_str()) == 0) {
                std::cout << "Set URL icon!" << std::endl;
                return true;
            }
            std::cout << "Error renaming file files!" << std::endl;
        }
        std::cout << "Error removing old files!" << std::endl;
    }
    return false;
}

bool set_folder_icon(fs::path folder_path, fs::path icon_path) {
    SHFOLDERCUSTOMSETTINGS folder_settings{};
    std::wstring wfolder_path = folder_path.wstring();
    std::wstring wicon_path = icon_path.wstring();
    folder_settings.dwSize = sizeof(SHFOLDERCUSTOMSETTINGS);
    folder_settings.dwMask = FCSM_ICONFILE;
    folder_settings.pszIconFile = const_cast<LPWSTR>(wicon_path.c_str());
    folder_settings.cchIconFile = 0;
    folder_settings.iIconIndex = 0;
    PCWSTR pszPath = const_cast<LPWSTR>(wfolder_path.c_str());
    return SHGetSetFolderCustomSettings(&folder_settings, pszPath,
                                        FCS_FORCEWRITE);
}

void refresh_desktop() {
    LPITEMIDLIST pidl;
    SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidl);
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, pidl, 0);
}

void get_dirs_and_shortcuts(fs::path parent_dir,
                            std::list<fs::path>& url_shortcuts,
                            std::list<fs::path>& lnk_shortcuts,
                            std::list<fs::path>& child_dirs) {
    std::error_code ec;
    for (const auto& child : fs::directory_iterator(parent_dir)) {
        if (child.path().extension() == ".url") {
            url_shortcuts.push_back(child.path());
        } else if (child.path().extension() == ".lnk") {
            lnk_shortcuts.push_back(child.path());
        } else if (fs::is_directory(child, ec)) {
            child_dirs.push_back(child.path());
        }
    }
}

int main() {
    CoInitialize(NULL);
    fs::path desktop_path = get_desktop_path();
    fs::path cwd = get_cwd();
    fs::path icon_path = cwd / ICON_PATH;
    fs::path wallpaper_path = cwd / WALLPAPER_PATH;
    std::list<fs::path> url_shortcuts;
    std::list<fs::path> lnk_shortcuts;
    std::list<fs::path> child_dirs;

    get_dirs_and_shortcuts(desktop_path, url_shortcuts, lnk_shortcuts,
                           child_dirs);

    std::cout << "URL FILES" << std::endl;
    print_string_list(url_shortcuts);
    std::cout << "LNK FILES" << std::endl;
    print_string_list(lnk_shortcuts);
    std::cout << "DIRECTORIES" << std::endl;
    print_string_list(child_dirs);

    for (const auto& url : url_shortcuts) {
        set_url_icon(url, icon_path);
    }
    for (const auto& lnk : lnk_shortcuts) {
        set_lnk_icon(lnk, icon_path);
    }
    for (const auto& dir : child_dirs) {
        set_lnk_icon(dir, icon_path);
    }
    set_wallpaper(wallpaper_path);
    refresh_desktop();
    return 0;
}