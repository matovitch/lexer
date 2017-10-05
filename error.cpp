#include "error.hpp"

//namespace std
//{

std::error_condition make_error_condition(const Error error)
{
    return std::error_condition
           {
               static_cast<int>(error),
               std::generic_category()
           };
}

std::error_code make_error_code(const Error error)
{
    return std::error_code
           {
               static_cast<int>(error),
               std::generic_category()
           };
}

namespace error
{
    std::error_code make(const Error error)
    {
        return make_error_code(error);
    }

} // end error namespace

//} // end std namespace