#pragma once

#include <cassert>
#include <comdef.h>
#include <codecvt>
#include <string>

namespace StringUtils {
    inline void ToWideString(const std::string& source, std::wstring& dest) 
    {
        dest.assign(source.begin(), source.end());
    }

    inline std::wstring ToWideString(const std::string& source)
    {
        std::wstring dest;
        dest.assign(source.begin(), source.end());
        return dest;
    }

    inline std::string ToString(const std::wstring& source)
    {
        using convert_type = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_type, wchar_t> converter;
        return converter.to_bytes(source);
    }

    inline std::wstring AnsiToWString(const std::string& str)
    {
        WCHAR buffer[512U];
        MultiByteToWideChar(CP_ACP, 0U, str.c_str(), -1, buffer, 512);
        return std::wstring(buffer);
    }
}

#if defined(DEBUG) || defined(_DEBUG)
#define ASSERT(condition) assert(condition);
#else
#define ASSERT(condition) (condition)
#endif

#ifndef CHECK_HR
#define CHECK_HR(x)                                                                                                     \
    {                                                                                                                   \
    const HRESULT hr__ = (x);                                                                                           \
if (FAILED(hr__))                                                                                                       \
{                                                                                                                       \
    const std::wstring wfn = StringUtils::AnsiToWString(__FILE__);                                                      \
    _com_error err(hr__);                                                                                               \
    const std::wstring msg = err.ErrorMessage();                                                                        \
    const std::wstring outputMsg = L" failed in " + wfn + L"; line " + std::to_wstring(__LINE__) + L"; error: " + msg;  \
    MessageBox(0, outputMsg.c_str(), 0, 0);                                                                             \
    abort();                                                                                                            \
}                                                                                                                       \
    }
#endif