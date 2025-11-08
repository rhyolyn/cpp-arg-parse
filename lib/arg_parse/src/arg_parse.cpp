#include <iostream>

#include <common/api.h>
#include <arg_parse/arg_parse.h>

extern "C"
{
    API void hello()
    {
        std::cout << __func__ << ": hello" << std::endl;
    }
}