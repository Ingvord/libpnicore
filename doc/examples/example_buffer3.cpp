/*! \example example_buffer3.cpp

Using the RefBuffer template.
*/

#include <iostream>
#include <pni/utils/RBuffer.hpp>
#include <pni/utils/Types.hpp>

using namespace pni::utils;

int main(int argc,char **argv)
{
    size_t N=1024;
    Float32 *data = new Float32[N];

    //create a new reference buffer
    RBuffer<Float32> buffer(N,data);

    //now buffer can be used like a Buffer<T> object.

    delete [] data;
    return 0;
}
