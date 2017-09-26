#include "token.hpp"

namespace token
{

const std::string& asString(uint8_t word)
{
    return AS_STRING[word];
}

Vector::Vector(const File   & file,
               const Reader & reader,
               Index        & index) :
    _file       {file},
    _reader     {reader},
    _index      {index},
    _prevReader {reader()},
    _prevOffset {1}
{}

std::size_t Vector::size() const 
{ 
    return words.size();
}

void Vector::pushBack(const Token word, 
                           const index::Status indexStatus)
{
    const uint8_t*    nextReader = _reader();
    const std::size_t nextOffset = reader::offset(_reader, _file);

    std::size_t lexemeID = 0;

    if (indexStatus == index::Status::SAVE_LEXEME)
    {
        const std::string& lexeme = std::string{reinterpret_cast<const char*>(_prevReader),
                                                static_cast<std::size_t>(nextReader - _prevReader)};

        lexemeID = _index.findOrSave(lexeme);
    }

    const std::size_t offset = _prevOffset;

    words    .push_back(word    );
    offsets  .push_back(offset  );
    lexemeIDs.push_back(lexemeID);

    _prevReader = nextReader;
    _prevOffset = nextOffset;
}

} // end token namespace