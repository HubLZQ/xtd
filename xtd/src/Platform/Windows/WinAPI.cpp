#include "WinAPI.h"

#define NOMINMAX      1
#define LEAN_AND_MEAN 1

#include <Windows.h>

namespace windows
{
int MultiByteToWideChar(unsigned int  CodePage,
                        unsigned long dwFlags,
                        const char*   lpMultiByteStr,
                        int           cbMultiByte,
                        wchar_t*      lpWideCharStr,
                        int           cchWideChar)
{
    return ::MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
}

int WideCharToMultiByte(unsigned int   CodePage,
                        unsigned long  dwFlags,
                        const wchar_t* lpWideCharStr,
                        int            cchWideChar,
                        char*          lpMultiByteStr,
                        int            cbMultiByte,
                        const char*    lpDefaultChar,
                        int*           lpUsedDefaultChar)
{
    return ::WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, lpMultiByteStr, cbMultiByte, lpDefaultChar, lpUsedDefaultChar);
}

uint32_t GetAcpCodePage()
{
    return CP_ACP;
}

uint32_t GetUtf8CodePage()
{
    return CP_UTF8;
}
}// namespace windows
