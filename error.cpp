#include "error.hpp"

Error::Error() : value{0} {}

Error::Error(int aValue) : value{aValue} {}

Error::operator bool() const
{
    return value != 0;
}

Error::operator int() const
{
    return value;
}