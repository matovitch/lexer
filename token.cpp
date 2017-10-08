#include "token.hpp"

#include "file_map.hpp"
#include "location.hpp"
#include "reader.hpp"
#include "file.hpp"

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <vector>
#include <string>

namespace token
{

Pretty::Pretty(const Token token,
               const Location& theLocation,
               const std::string& theLexeme,
               const std::string& theFilePath) :
    _token   {token},
    location {theLocation},
    lexeme   {theLexeme},
    filePath {theFilePath}
{}

const std::string& Pretty::asString() const
{
    return token::AS_STRING[_token];
}

Vector::Vector(const File   & file,
               const Reader & reader,
               Index        & index) :
    _file       {file},
    _reader     {reader},
    _index      {index},
    _fileMap    {file.makeMap()},
    _prevReader {reader()},
    _prevOffset {1}
{}

std::size_t Vector::size() const
{
    return _tokens.size();
}

const std::string& Vector::filePath() const
{
    return _file.pathAsString;
}

void Vector::pushBack(const Token token, 
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

    _tokens   .push_back(token   );
    _offsets  .push_back(offset  );
    _lexemeIDs.push_back(lexemeID);

    _prevReader = nextReader;
    _prevOffset = nextOffset;
}

Pretty Vector::operator[](const std::size_t i) const
{
    const auto& token    = _tokens[i];
    const auto& location = _fileMap .searchLocation (_offsets[i]);
    const auto& lexeme   = _index   .searchLexeme   (_lexemeIDs[i], token);
    const auto& filePath = this->filePath();

    return Pretty{token, location, lexeme, filePath};
}

std::ostream& operator<< (std::ostream& outputStream, const token::Pretty& token)
{
    outputStream                       << std::left  << std::setw(12)
        << token.asString()            << std::right << std::setw (2)
        << token.location.line << ':'  << std::right << std::setw (2)
        << token.location.col          << std::right << std::setw (4);

    const auto& lexeme = token.lexeme;

    if (!lexeme.empty())
    {
        outputStream << "(\"" << lexeme << "\")";
    }

    return outputStream;
}

std::ostream& operator<< (std::ostream& outputStream, const token::Vector& tokenVector)
{
    outputStream << "Tokens from " << tokenVector.filePath() << ":";

    outputStream << std::endl
                 << std::endl;

    for (std::size_t i = 0; i < tokenVector.size(); i++)
    {
        outputStream << tokenVector[i] << std::endl;
    }

    return outputStream;
}

} // end token namespace