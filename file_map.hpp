#ifndef __FILE_MAP_H__
#define __FILE_MAP_H__

#include "location.hpp"

#include <cstdint>
#include <vector>
#include <string>

class File;

namespace file
{

class Map
{

public:

    Map(const File& file);

    Location searchLocation(const uint8_t offset) const;

private:

    void fillLines(const File& file);

    std::vector<std::size_t> _lines;
};

} // end file namespace

#endif // __FILE_MAP_H__