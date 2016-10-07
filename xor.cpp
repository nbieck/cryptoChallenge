#include "defines.h"

#include <cassert>

bytestring xor(const bytestring& a, const bytestring& b)
{
    assert(a.size() == b.size());

    size_t size = a.size();
    bytestring result(size, 0);

    for (size_t i = 0; i < size; ++i)
    {
        result[i] = a[i] ^ b[i];
    }

    return result;
}


#include "encoding.h"
#include <iostream>

int main(void)
{
    auto a = hexToBytes("1c0111001f010100061a024b53535009181c");
    auto b = hexToBytes("686974207468652062756c6c277320657965");

    auto c = xor(a, b);

    std::cout << bytesToHex(c) << std::endl;

    system("pause");

    return 0;
}