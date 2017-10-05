#ifndef __FILE_H__
#define __FILE_H__

#include "location.hpp"
#include "reader.hpp"
#include "token.hpp"
#include "lexer.hpp"

#include <system_error>
#include <cstddef>
#include <string>
#include <vector>
#include <tuple>

class FileMap;

class File
{
    friend bool        reader::isValid (const Reader& reader, const File& file);
    friend std::size_t reader::offset  (const Reader& reader, const File& file);

public:

    File() = delete;

    File(const std::string& pathAsString,
         std::error_code& errorCode,
         std::size_t margin = 0x20,
         uint8_t fill = 0x20);

    Reader makeReader() const;

    FileMap makeFileMap() const;

private:

    void checkPath(const std::string& pathAsString,
                   std::error_code& errorCode);

    void fillBuffer(const std::string& pathAsString,
                    std::size_t margin,
                    uint8_t fill);

    const uint8_t*           _eof;
    std::vector<uint8_t>     _buffer;
};

namespace file
{
    std::tuple<File, std::error_code> make(const std::string pathAsString);

} // end file namespace

#endif // end __FILE_H__