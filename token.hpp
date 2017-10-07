#ifndef __TOKEN_VECTOR_H__
#define __TOKEN_VECTOR_H__

#include "file_map.hpp"
#include "reader.hpp"
#include "index.hpp"
#include "file.hpp"

#include <cstdint>
#include <string>
#include <vector>

using Token = uint8_t;

namespace token
{

enum : uint8_t
{
    COMMENT,
    WHITE_SPACE,
    IDENTIFIER,
    FLOATTING,
    INTEGER,
    IMPORT,
    MODULE,
    RETURN,
    ALIAS,
    IF,
    THEN,
    ELSE,
    WHILE,
    L_PARENT,
    R_PARENT,
    L_BRACE,
    R_BRACE,
    SEMI_COLON,
    DOT,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    LST,
    LEQ,
    GST,
    GEQ,
    EQL,
    AND,
    OR,
    XOR,
    NOT,
    C_EQL,
    C_NEQ,
    C_AND,
    C_OR,
    UKNOWN,
    END_OF_FILE
};

static const std::string AS_STRING[] =
    {
        "COMMENT",
        "WHITE_SPACE",
        "IDENTIFIER",
        "FLOATTING",
        "INTEGER",
        "IMPORT",
        "MODULE",
        "RETURN",
        "ALIAS",
        "IF",
        "THEN",
        "ELSE",
        "WHILE",
        "L_PARENT",
        "R_PARENT",
        "L_BRACE",
        "R_BRACE",
        "SEMI_COLON",
        "DOT",
        "ADD",
        "SUB",
        "MUL",
        "DIV",
        "MOD",
        "LST",
        "LEQ",
        "GST",
        "GEQ",
        "EQL",
        "AND",
        "OR",
        "XOR",
        "NOT",
        "C_EQL",
        "C_NEQ",
        "C_AND",
        "C_OR",
        "UKNOWN",
        "END_OF_FILE"
    };

static const std::string AS_LEXEME[] =
    {
        "",
        "",
        "",
        "",
        "",
        "import",
        "module",
        "return",
        "alias",
        "if",
        "then",
        "else",
        "while",
        "(",
        ")",
        "{",
        "}",
        ";",
        ".",
        "+",
        "-",
        "*",
        "/",
        "%",
        "<",
        "<=",
        ">",
        ">=",
        "=",
        "&",
        "|",
        "^",
        "!",
        "==",
        "!=",
        "&&",
        "||",
        "",
        ""
    };


const std::string& asString(uint8_t word);

class Vector
{

    friend std::ostream& operator<< (std::ostream& outputStream, const token::Vector& tokens);

public:

    Vector(const File   & file,
           const Reader & reader,
           Index& index);

    std::size_t size() const;

    const std::string& filePath() const;

    void pushBack(const uint8_t word, 
                  const index::Status indexStatus = index::Status::DROP_LEXEME);

    template <class UnaryPred>
    void trim(const UnaryPred& isTrimable)
    {
        std::vector<Token>       trimedWords;
        std::vector<std::size_t> trimedOffsets;
        std::vector<std::size_t> trimedLexemeIDs;

        for (std::size_t i = 0; i < size(); i++)
        {
            const auto& word = _words[i];

            if (!isTrimable(word))
            {
                trimedWords     .push_back( word        );
                trimedOffsets   .push_back(_offsets[i]  );
                trimedLexemeIDs .push_back(_lexemeIDs[i]);
            }
        }

        std::swap(_words    , trimedWords    );
        std::swap(_offsets  , trimedOffsets  );
        std::swap(_lexemeIDs, trimedLexemeIDs);
    }

private:

    const File&    _file;
    const Reader&  _reader;
    Index&         _index;
 
    const file::Map _fileMap;

    const uint8_t* _prevReader;
    std::size_t    _prevOffset;

    std::vector<Token>       _words;
    std::vector<std::size_t> _offsets;
    std::vector<std::size_t> _lexemeIDs;
};

} // end token namespace

#endif