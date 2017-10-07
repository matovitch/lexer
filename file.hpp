#ifndef __FILE_H__
#define __FILE_H__

#include "location.hpp"
#include "reader.hpp"
#include "error.hpp"
#include "token.hpp"
#include "lexer.hpp"

#include <cstddef>
#include <string>
#include <vector>
#include <tuple>

namespace file
{

class Map;

} // end file namespace

class File
{
    friend bool        reader::isValid (const Reader& reader, const File& file);
    friend std::size_t reader::offset  (const Reader& reader, const File& file);

public:

    File() = delete;

    File(const std::string& pathAsString,
         Error& error,
         std::size_t margin = 0x20,
         uint8_t fill = 0x20);

    Reader makeReader() const;

    file::Map makeMap() const;

    const std::string& pathAsString() const;

    void freeBuffer();

private:

    void checkPath(const std::string& pathAsString,
                   Error& error);

    void fillBuffer(const std::string& pathAsString,
                    std::size_t margin,
                    uint8_t fill);

    const uint8_t*           _eof;
    std::vector<uint8_t>     _buffer;
    const std::string        _pathAsString;
};

namespace file
{

std::tuple<File, Error> make(const std::string pathAsString);

} // end file namespace

#endif // end __FILE_H__