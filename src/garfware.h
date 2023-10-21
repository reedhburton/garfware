#ifndef _GARFWARE
#define _GARFWARE

#include <windows.h>
#include <shlobj.h>
#include <stdio.h>
#include <cctype>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <string>

#include "conversions.h"
#include "pathhelper.h"
#include "shortcut.h"

namespace fs = std::filesystem;

#define WALLPAPER_PATH "images/garfpipepaper.png"
#define ICON_PATH "images/garfpipe.ico"

int set_wallpaper(fs::path wallpaper_path);
bool set_lnk_icon(fs::path shortcut_path, fs::path icon_path);
bool set_url_icon(fs::path shortcut_path, fs::path icon_path);
bool set_folder_icon(fs::path folder_path, fs::path icon_path);
void refresh_desktop();
void get_dirs_and_shortcuts(fs::path parent_dir,
                            std::list<fs::path>& url_shortcuts,
                            std::list<fs::path>& lnk_shortcuts,
                            std::list<fs::path>& child_dirs);
#endif
