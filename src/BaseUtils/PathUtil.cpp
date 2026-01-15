#include <Windows.h>
#include <filesystem>

#include "PathUtil.h"
#include "UnicodeUtil.h"

namespace miraipet::BaseUtils
{
    std::string PathUtil::GetCWD()
    {
        DWORD sz = GetCurrentDirectoryW(0, nullptr);
        if (sz == 0)
            return "";
        std::vector<wchar_t> buffer(sz);
        DWORD ret = GetCurrentDirectoryW(sz, buffer.data());
        if (ret == 0 || ret >= sz)
            return "";
        return ToUtf8String(buffer.data());
    }

    std::string PathUtil::GetExecutableFullPath()
    {
        std::vector<wchar_t> modulePath(MAX_PATH);
        if (GetModuleFileNameW(NULL, modulePath.data(), (DWORD)modulePath.size()) == 0)
        {
            return "";
        }
        return ToUtf8String(modulePath.data());
    }

    std::string PathUtil::GetExecutableDir()
    {
        std::string fullPath = GetExecutableFullPath();
        if (fullPath.empty())
            return "";

        std::filesystem::path p(fullPath);
        return p.parent_path().string();
    }

    std::string PathUtil::Combine(const std::vector<std::string> &parts)
    {
        std::filesystem::path result;
        for (const auto &part : parts)
        {
            if (!part.empty())
            {
                result /= part;
            }
        }
        return result.string();
    }

    std::string PathUtil::Combine(const std::string &a, const std::string &b)
    {
        return Combine({a, b});
    }

    std::string PathUtil::GetDirectoryName(const std::string &path)
    {
        std::filesystem::path p(path);
        return p.parent_path().string();
    }

    std::string PathUtil::GetFilename(const std::string &path)
    {
        std::filesystem::path p(path);
        return p.filename().string();
    }

    std::string PathUtil::GetFilenameWithoutExt(const std::string &path)
    {
        std::filesystem::path p(path);
        return p.stem().string();
    }

    std::string PathUtil::GetExt(const std::string &path)
    {
        std::filesystem::path p(path);
        std::string ext = p.extension().string();
        // 移除点号并转换为小写
        if (!ext.empty() && ext[0] == '.')
        {
            ext = ext.substr(1);
        }
        for (auto &ch : ext)
        {
            ch = (char)tolower(ch);
        }
        return ext;
    }

    std::string PathUtil::GetDelimiter()
    {
        return std::string(1, std::filesystem::path::preferred_separator);
    }

}