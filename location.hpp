#ifndef __LOCATION_H__
#define __LOCATION_H__

#include <cstdint>

struct Location
{
    std::size_t line;
    std::size_t col;
};

#endif