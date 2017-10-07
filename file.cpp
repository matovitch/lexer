#include "file.hpp"

#include "file_map.hpp"
#include "reader.hpp"
#include "error.hpp"

#include <experimental/filesystem>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <ios>

namespace stdx = std::experimental;

File::File(const std::string& pathAsString,
           Error& error,
           std::size_t margin,
           uint8_t fill) :
    _eof{NULL},
    _pathAsString{pathAsString}
{
    checkPath (pathAsString, error);

    if (!error)
    {
        fillBuffer(pathAsString, margin, fill);
    }
}

Reader File::makeReader() const
{
    return Reader{_buffer.data()};
}

file::Map File::makeMap() const
{
    return file::Map{*this};
}

const std::string& File::pathAsString() const
{
    return _pathAsString;
}

void File::checkPath(const std::string& pathAsString,
                     Error& error)
{
    stdx::filesystem::path path{pathAsString};

    if (!stdx::filesystem::exists(path))
    {
        error = Error::FILE_NOT_FOUND;
        return;
    }

    if (!stdx::filesystem::is_regular_file(path))
    {
        error = Error::FILE_NOT_REGULAR;
        return;
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

void File::freeBuffer()
{
    std::vector<uint8_t>{}.swap(_buffer);
}

namespace file
{

std::tuple<File, Error> make(const std::string pathAsString)
{
    Error error;

    return std::make_tuple(File{pathAsString, error}, error);
}
    
} // end file namespace
