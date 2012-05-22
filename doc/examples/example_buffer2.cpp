/*! \example example_buffer2.cpp

Accessing data from a Buffer<T> template
*/

#include <iostream>
#include <pni/utils/Buffer.hpp>
#include <pni/utils/Types.hpp>

using namespace pni::utils;

int main(int argc,char **argv)
{
    Buffer<Float32> buffer(1234);

    //access data without index check
    std::cout<<buffer[10]<<std::endl;
    buffer[100] = 45.23;

    //access data with index check
    std::cout<<buffer.at(10)<<std::endl;
    buffer.at(100) = 567.2334;

    //access to raw pointers
    Float32 *ptr = buffer.ptr();
    void *vptr = buffer.void_ptr();

    return 0;
}