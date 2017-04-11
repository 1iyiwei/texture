#include <iostream>
#include "Exception.hpp"

int main(int argc, char **argv)
{
    try
    {
        throw Exception("Hello world!");
        
        std::cout << "Nothing is happening here!" << std::endl;
    }   
    catch(Exception e)
    {   
        std::cout << e.Message() << endl;
    }

    return 1;
}

