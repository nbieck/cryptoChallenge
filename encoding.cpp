#include "defines.h"

#include <cassert>

byte hexCharToBits(char hex)
{
    switch(hex)
    {
    case '0':
        return 0x0;
        break;
    case '1':
        return 0x1;
        break;
    case '2':
        return 0x2;
        break;
    case '3':
        return 0x3;
        break;
    case '4':
        return 0x4;
        break;
    case '5':
        return 0x5;
        break;
    case '6':
        return 0x6;
        break;
    case '7':
        return 0x7;
        break;
    case '8':
        return 0x8;
        break;
    case '9':
        return 0x9;
        break;
    case 'a':
        return 0xa;
        break;
    case 'b':
        return 0xb;
        break;
    case 'c':
        return 0xc;
        break;
    case 'd':
        return 0xd;
        break;
    case 'e':
        return 0xe;
        break;
    case 'f':
        return 0xf;
        break;
    default:
        assert(0);
        break;
    }

    return 0;
}

// decode a hex string to raw bytes
vector<byte> hexToRawBytes(const string& input)
{
}

int main(void)
{
    return 0;
}
