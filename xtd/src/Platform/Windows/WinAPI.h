#pragma once
#include <cstdint>

namespace windows
{
int MultiByteToWideChar(unsigned int  CodePage,
                        unsigned long dwFlags,
                        const char*   lpMultiByteStr,
                        int           cbMultiByte,
                        wchar_t*      lpWideCharStr,
                        int           cchWideChar);
int WideCharToMultiByte(unsigned int   CodePage,
                        unsigned long  dwFlags,
                        const wchar_t* lpWideCharStr,
                        int            cchWideChar,
                        char*          lpMultiByteStr,
                        int            cbMultiByte,
                        const char*    lpDefaultChar,
                        int*           lpUsedDefaultChar);

uint32_t GetAcpCodePage();
uint32_t GetUtf8CodePage();
}//namespace windows
