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
    _fileMap    {file.makeMap()},
    _prevReader {reader()},
    _prevOffset {1}
{}

std::size_t Vector::size() const
{
    return _words.size();
}

const std::string& Vector::filePath() const
{
    return _file.pathAsString;
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

    _words    .push_back(word    );
    _offsets  .push_back(offset  );
    _lexemeIDs.push_back(lexemeID);

    _prevReader = nextReader;
    _prevOffset = nextOffset;
}

std::ostream& operator<< (std::ostream& outputStream, const token::Vector& tokens)
{
    const auto& fileMap = tokens._fileMap;
    const auto& index   = tokens._index;

    outputStream << "Tokens from " << tokens.filePath() << ":" << std::endl;

    for (std::size_t i = 0; i < tokens.size(); i++)
    {
        const auto& word     = tokens  ._words[i];
        const auto& location = fileMap .searchLocation (tokens._offsets[i]);
        const auto& lexeme   = index   .searchLexeme   (tokens._lexemeIDs[i], word);

        outputStream                 << std::left  << std::setw(12)
            << token::asString(word) << std::right << std::setw (2)
            << location.line << ':'  << std::right << std::setw (2)
            << location.col          << std::right << std::setw (4);

        if (!lexeme.empty())
        {
            outputStream << "(\"" << lexeme << "\")";
        }
    
        outputStream << std::endl;
    }

    return outputStream;
}

} // end token namespace