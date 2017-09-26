#include "file.hpp"

#include "file_map.hpp"
#include "reader.hpp"

#include <experimental/filesystem>
#include <exception>
#include <cstddef>
#include <fstream>
#include <string>
#include <vector>
#include <ios>

namespace stdx = std::experimental;

File::File(const std::string& path,
           std::size_t margin,
           uint8_t fill) :
    _eof{NULL}
{
    checkPath  (path);
    fillBuffer (path, fill, margin);
}

Reader File::craftReader() const
{
    return Reader{_buffer.data()};
}

FileMap File::craftFileMap() const
{
    return FileMap{*this};
}

void File::checkPath(const std::string& pathAsString)
{
    stdx::filesystem::path path{pathAsString};

    if (!stdx::filesystem::exists(path))
    {
        throw std::runtime_error(path.string() + ": file not found.");
    }

    if (!stdx::filesystem::is_regular_file(path))
    {
        throw std::runtime_error(path.string() + ": this is not a regular file.");
    }
}

void File::fillBuffer(const std::string& pathAsString,
                      std::size_t margin,
                      uint8_t fill)
{

    stdx::filesystem::path path{pathAsString};

    const std::size_t fileSize = stdx::filesystem::file_size(path);

    _buffer.resize(fileSize + margin);

    std::ifstream ifs{ path.string().c_str(), std::ios_base::in |
                                              std::ios_base::binary };

    if (ifs.fail())
    {
        throw std::runtime_error(path.string() + ": failed to open file.");
    }

    ifs.read(reinterpret_cast<char*>(_buffer.data()), fileSize);

    if (ifs.fail())
    {
        throw std::runtime_error(path.string() + ": failed to read file.");
    }

    for (std::size_t i = fileSize; i < fileSize + margin; i++)
    {
        _buffer[i] = fill;
    }

    _eof  = _buffer.data() + fileSize;
}
