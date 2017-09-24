#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <cstddef>
#include <string>

class Index;

struct Token
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

    Token() = delete;

    operator uint8_t() const;

    const std::string& asString() const;

    const uint8_t      value;
    const std::size_t  offset;
    const std::size_t  lexeme;
};

namespace token
{

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

} // end token namespace

#endif // end __TOKEN_H__