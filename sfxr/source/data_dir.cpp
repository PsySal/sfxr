#include "data_dir.h"

#include <iostream>
#include <limits.h>
#include <unistd.h>

#ifdef __MINGW32__

#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>

#elif __MACOSX__

#include "CoreServices/CoreServices.h"
#include "CoreFoundation/CoreFoundation.h"

#endif

std::string get_exe_parent_dir()
{
    std::string exe_path = get_exe_path();
    int char_pos = exe_path.rfind ('/') + 1;
    std::string exe_parent_dir;
    exe_parent_dir.append(exe_path, 0, char_pos - 1);
    return exe_parent_dir;
}

#ifdef __linux__

std::string get_exe_path()
{
    char path_buffer[PATH_MAX];
    ssize_t bytes = readlink("/proc/self/exe", path_buffer, PATH_MAX);
    return std::string(path_buffer, bytes > 0 ? bytes : 0);
}

#elif __MINGW32__

std::string get_exe_path()
{
    char path_buffer[MAX_PATH];
    SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path_buffer);
    return std::string(path_buffer);
}

#elif __MACOSX__

std::string get_exe_path()
{
    std::string path;

    CFBundleRef mainBundle = CFBundleGetMainBundle();
    char path_buffer[PATH_MAX];
    if(CFURLGetFileSystemRepresentation(CFBundleCopyResourcesDirectoryURL(mainBundle), TRUE, (UInt8 *) path_buffer, PATH_MAX)) {

        path = std::string(path_buffer);
    }
    CFRelease(resourcesURL);

    return path;
}

#else

std::string get_exe_path()
{
    return "";
}

#endif
