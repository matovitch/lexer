#ifndef __FILE_MAP_H__
#define __FILE_MAP_H__

#include "location.hpp"

#include <cstdint>
#include <vector>

class File;

class FileMap
{

public:

    FileMap(const File& file);

    Location searchLocation(const uint8_t offset) const;

private:

    void fillLines(const File& file);

    std::vector<std::size_t> _lines;
};


#endif // __FILE_MAP_H__