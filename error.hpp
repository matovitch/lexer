#ifndef __MY_ERROR_H__
#define __MY_ERROR_H__

#include <system_error>
#include <type_traits> 

enum Error
{
    FILE_NOT_FOUND = 1,
    FILE_NOT_REGULAR
};

namespace std
{

template <>
struct is_error_condition_enum<Error> : true_type {};

} // end std namespace

std::error_condition make_error_condition(const Error error);

std::error_code make_error_code(const Error error);

namespace error
{
    std::error_code make(const Error error);

} // end error namespace

#endif // __ERROR_H__