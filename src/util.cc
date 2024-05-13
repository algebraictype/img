#include "include/util.hpp"

void Error::img_log(const std::string& message, ...){

    std::string debug_line(message);
    debug_line = debug_line.c_str();

    //list of variable arguments
    va_list args;

    //initialize variable arguments
    //va_start(args, debug_line);

    std::cout << "img: " << debug_line <<  std::endl;

    va_end(args);
}
