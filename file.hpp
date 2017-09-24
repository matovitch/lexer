#ifndef __FILE_H__
#define __FILE_H__

#include "location.hpp"
#include "reader.hpp"
#include "token.hpp"
#include "lexer.hpp"

#include <cstddef>
#include <string>
#include <vector>

class File
{
    friend bool        reader::isValid (const Reader& reader, const File& file);
    friend std::size_t reader::offset  (const Reader& reader, const File& file);

    friend Location lexer::searchLocation(const Token& token, 
                                          const File& file);

public:

    File() = delete;

    File(const std::string& path,
         std::size_t margin = 0xff,
         uint8_t fill = 0x20);

    Reader craftReader() const;

private:

    void checkPath(const std::string& pathAsString);

    void fillBuffer(const std::string& pathAsString,
                    std::size_t margin,
                    uint8_t fill);

    void fillLines();

    const uint8_t*           _eof;
    std::vector<uint8_t>     _buffer;
    std::vector<std::size_t> _lines;
};


#endif // end __FILE_H__