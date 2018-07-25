#ifdef WIN32
#   include <Windows.h>
#endif
#include <tchar.h>
#include <cstdio>

#include "aurum/DebugConsole.h"

#ifdef WIN32
#   define snprintf _snprintf_s
#endif

namespace au {

struct DebugConsole::Impl
{
#ifdef WIN32
    HANDLE hConsole;
    FILE* fp;
#endif

    Impl()
    {
#ifdef WIN32
        ::AllocConsole();
        SetConsoleTitle(_T("AURUM Debug Console"));

        // Redirect stdout to the console
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        freopen_s(&fp, "CONOUT$", "w", stdout);
#endif
    }

    ~Impl()
    {
#ifdef WIN32
        fclose(fp);
        ::FreeConsole();
#endif
    }
};

DebugConsole::DebugConsole()
    : d(std::make_unique<Impl>())
{
}

DebugConsole::~DebugConsole() = default;

void DebugConsole::writeLine(const char *str)
{
    char buf[256] = {0};
    snprintf(buf, 255, "%s\n", str);
#ifdef WIN32
    DWORD written = 0;
    WriteConsole(d->hConsole, buf, static_cast<DWORD>(strlen(buf)), &written, 0);
#endif
}

} // namespace au
