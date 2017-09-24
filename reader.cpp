#include "reader.hpp"

#include "file.hpp"

#include <cstddef>

Reader::Reader(const uint8_t* const head) : 
    _head{head}
{}

const uint8_t* Reader::operator()() const 
{ 
    return _head; 
}

void Reader::operator+=(std::size_t offset) 
{ 
    _head += offset;
}

void Reader::capture() 
{ 
    _save = _head; 
}

void Reader::restore() 
{ 
    _head = _save; 
}

char Reader::asChar() const
{
    return *(reinterpret_cast<const char*>(_head));
}

bool Reader::match(char toMatch)
{
    if (*_head != static_cast<uint8_t>(toMatch))
    {
        return false;
    }

    _head += 1;

    return true;
}

bool Reader::match(const std::string& toMatch)
{
    const uint8_t* const toMatchAsByte = reinterpret_cast<const uint8_t*>(toMatch.data());

    const std::size_t size = toMatch.size();

    for (std::size_t i = 0; i < size; i++)
    {
        if (*(_head + i) != toMatchAsByte[i])
        {
            return false;
        }
    }

    _head += size;

    return true;
}

namespace reader
{
    bool isValid(const Reader& reader, const File& file)
    {
        return reader._head < file._eof;
    }

    std::size_t offset(const Reader& reader, const File& file)
    {
        return reader._head - file._buffer.data();
    }
}

