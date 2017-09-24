#ifndef __READER_H__
#define __READER_H__

#include <cstddef>
#include <string>

class Reader;
class File;

namespace reader
{

    bool        isValid (const Reader& reader, const File& file);
    std::size_t offset  (const Reader& reader, const File& file);

} // end reader namespace

class Reader
{
    friend bool        reader::isValid (const Reader& reader, const File& file);
    friend std::size_t reader::offset  (const Reader& reader, const File& file);

public:

    Reader() = delete;

    Reader(const uint8_t* const head);

    const uint8_t* operator()() const;

    void operator+=(std::size_t offset);

    void capture();

    void restore();

    char asChar() const;

    bool match(const std::string& toMatch);

    bool match(char toMatch);

private:

    const uint8_t* _head;
    const uint8_t* _save;
};

#endif // end __READER_H__