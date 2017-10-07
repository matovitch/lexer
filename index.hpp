#ifndef __INDEX_H__
#define __INDEX_H__

#include <unordered_map>
#include <string>

class Index
{

public:

    Index();

    std::size_t findOrSave(const std::string& lexeme);

    const std::string& searchLexeme(const std::size_t lexemeID, 
                                    const uint8_t     word) const;

private:

    std::size_t _size;

    std::unordered_map<std::size_t, std::string> _forward;
    std::unordered_map<std::string, std::size_t> _backward;
};

namespace index
{

struct Status
{
    enum : uint8_t
    {
        DROP_LEXEME,
        SAVE_LEXEME
    };

    Status(uint8_t status) : value{status} {}

    operator uint8_t() const { return value; }

    uint8_t value;
};

} // end index namespace


#endif