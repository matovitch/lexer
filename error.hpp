#ifndef __ERROR_H__
#define __ERROR_H__

struct Error
{
    enum
    {
        FILE_NOT_FOUND = 1,
        FILE_NOT_REGULAR
    };

    Error();

    Error(int aValue);

    operator bool() const;

    operator int() const;

    int value;
};

#endif // __ERROR_H__