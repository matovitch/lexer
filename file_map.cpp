#include "file_map.hpp"

#include "location.hpp"
#include "file.hpp"

#include <cstdint>
#include <vector>

namespace file
{

Map::Map(const File& file)
{
    fillLines(file);
}

Location Map::searchLocation(const uint8_t offset) const
{
    const auto& fit = std::lower_bound(_lines.begin(),
                                        _lines.end(),
                                        offset);
    return {
                static_cast<std::size_t>(fit - _lines.begin()),
                static_cast<std::size_t>(offset - *(fit - 1))
            };
}

void Map::fillLines(const File& file)
{
    auto&& reader = file.makeReader();

    _lines.push_back(0);

    const auto& addLine = [&]() { _lines.push_back(reader::offset(reader, file)); };

    while (reader::isValid(reader, file))
    {
        if (reader.asChar() == '\n')
        {
            addLine();
        }

        reader += 1;
    }

    addLine();
}
    
} // end file namespace