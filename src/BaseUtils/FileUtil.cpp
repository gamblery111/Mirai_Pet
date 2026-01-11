#include "FileUtil.h"
#include "UnicodeUtil.h"
#include <iterator>

namespace miraipet::BaseUtils
{

    FileUtil::FileUtil()
    {
    }

    FileUtil::~FileUtil()
    {
    }

    bool FileUtil::OpenFile(const char *filepath, const char *mode)
    {
        if (m_fp != nullptr)
        {
            Close();
        }
        std::wstring wFilepath;
        if (!TryToWString(filepath, wFilepath))
        {
            return false;
        }
        std::wstring wMode;
        if (!TryToWString(mode, wMode))
        {
            return false;
        }
        auto err = _wfopen_s(&m_fp, wFilepath.c_str(), wMode.c_str());
        if (err != 0)
        {
            return false;
        }

        ClearBadFlag();

        Seek(0, SeekDir::End);
        m_fileSize = Tell();
        Seek(0, SeekDir::Begin);
        if (IsBad())
        {
            Close();
            return false;
        }
        return true;
    }

    bool FileUtil::Open(const char *filepath)
    {
        return OpenFile(filepath, "rb");
    }

    bool FileUtil::OpenText(const char *filepath)
    {
        return OpenFile(filepath, "r");
    }

    bool FileUtil::Create(const char *filepath)
    {
        return OpenFile(filepath, "wb");
    }

    bool FileUtil::CreateText(const char *filepath)
    {
        return OpenFile(filepath, "w");
    }

    void FileUtil::Close()
    {
        if (m_fp != nullptr)
        {
            fclose(m_fp);
            m_fp = nullptr;
            m_fileSize = 0;
            m_badFlag = false;
        }
    }

    bool FileUtil::IsOpen()
    {
        return m_fp != nullptr;
    }

    FileUtil::Offset FileUtil::GetSize() const
    {
        return m_fileSize;
    }

    bool FileUtil::IsBad() const
    {
        return m_badFlag;
    }

    void FileUtil::ClearBadFlag()
    {
        m_badFlag = false;
    }

    bool FileUtil::IsEOF()
    {
        return feof(m_fp) != 0;
    }

    FILE *FileUtil::GetFilePointer() const
    {
        return m_fp;
    }

    bool FileUtil::ReadAll(std::vector<char> *buffer)
    {
        if (buffer == nullptr)
        {
            return false;
        }

        buffer->resize(m_fileSize);
        Seek(0, SeekDir::Begin);
        if (!Read((*buffer).data(), m_fileSize))
        {
            return false;
        }

        return true;
    }

    bool FileUtil::ReadAll(std::vector<uint8_t> *buffer)
    {
        if (buffer == nullptr)
        {
            return false;
        }

        buffer->resize(m_fileSize);
        Seek(0, SeekDir::Begin);
        if (!Read((*buffer).data(), m_fileSize))
        {
            return false;
        }

        return true;
    }
    bool FileUtil::ReadAll(std::vector<int8_t> *buffer)
    {
        if (buffer == nullptr)
        {
            return false;
        }

        buffer->resize(m_fileSize);
        Seek(0, SeekDir::Begin);
        if (!Read((*buffer).data(), m_fileSize))
        {
            return false;
        }

        return true;
    }

    bool FileUtil::Seek(Offset offset, SeekDir origin)
    {
        if (m_fp == nullptr)
        {
            return false;
        }
        int cOrigin = 0;
        switch (origin)
        {
        case SeekDir::Begin:
            cOrigin = SEEK_SET;
            break;
        case SeekDir::Current:
            cOrigin = SEEK_CUR;
            break;
        case SeekDir::End:
            cOrigin = SEEK_END;
            break;
        default:
            return false;
        }
        if (_fseeki64(m_fp, offset, cOrigin) != 0)
        {
            m_badFlag = true;
            return false;
        }
        return true;
    }

    FileUtil::Offset FileUtil::Tell()
    {
        if (m_fp == nullptr)
        {
            return -1;
        }
        return (Offset)_ftelli64(m_fp);
    }

    TextFileReader::TextFileReader(const char *filepath)
    {
        Open(filepath);
    }

    TextFileReader::TextFileReader(const std::string &filepath)
    {
        Open(filepath);
    }

    bool TextFileReader::Open(const char *filepath)
    {
        return m_file.OpenText(filepath);
    }

    bool TextFileReader::Open(const std::string &filepath)
    {
        return Open(filepath.c_str());
    }

    void TextFileReader::Close()
    {
        m_file.Close();
    }

    bool TextFileReader::IsOpen()
    {
        return m_file.IsOpen();
    }

    std::string TextFileReader::ReadLine()
    {
        if (!IsOpen() || IsEof())
        {
            return "";
        }

        std::string line;
        auto outputIt = std::back_inserter(line);
        int ch;
        ch = fgetc(m_file.GetFilePointer());
        while (ch != EOF && ch != '\r' && ch != '\n')
        {
            line.push_back(ch);
            ch = fgetc(m_file.GetFilePointer());
        }
        if (ch != EOF)
        {
            if (ch == '\r')
            {
                ch = fgetc(m_file.GetFilePointer());
                if (ch != EOF && ch != '\n')
                {
                    ungetc(ch, m_file.GetFilePointer());
                }
            }
            else
            {
                ch = fgetc(m_file.GetFilePointer());
                if (ch != EOF)
                {
                    ungetc(ch, m_file.GetFilePointer());
                }
            }
        }

        return line;
    }

    void TextFileReader::ReadAllLines(std::vector<std::string> &lines)
    {
        lines.clear();
        if (!IsOpen() || IsEof())
        {
            return;
        }
        while (!IsEof())
        {
            lines.emplace_back(ReadLine());
        }
    }

    std::string TextFileReader::ReadAll()
    {
        std::string all;

        if (m_file.IsOpen())
        {
            int ch = fgetc(m_file.GetFilePointer());
            while (ch != EOF)
            {
                all.push_back(ch);
                ch = fgetc(m_file.GetFilePointer());
            }
        }
        return all;
    }

    bool TextFileReader::IsEof()
    {
        if (!m_file.IsOpen())
        {
            return false;
        }
        return m_file.IsEOF();
    }
}