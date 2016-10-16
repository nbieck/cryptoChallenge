#include "utils.h"

#include <cassert>

byte hexCharToBits(char hex)
{
    if (hex <= '9')
        return hex - '0';
    else
        return hex - 'a' + 10;
}

char bitsToHex(byte bits)
{
    if (bits < 10)
        return '0' + bits;
    else
        return 'a' + bits - 10;
}

char bitsToBase64(byte bits)
{
    if (bits < 26)
        return 'A' + bits;
    else if (bits < 52)
        return 'a' + bits - 26;
    else if (bits < 62)
        return '0' + bits - 52;
    else if (bits == 62)
        return '+';
    else
        return '/';
}

bytestring hexToBytes(const string& input)
{
    bytestring result;

    // with the hex string, we always should have an even number of chars, 
    // as out base unit is 1 byte -> 2 hex
    assert(input.size() % 2 == 0);

    for (size_t i = 0; i < input.size(); i += 2)
    {
        byte converted;
        converted = hexCharToBits(input[i]);
        converted <<= 4;
        converted |= hexCharToBits(input[i + 1]);

        result.push_back(converted);
    }

    return result;
}

string bytesToBase64(const bytestring& bytes)
{
    //groups of 3 bytes -> 4 chars
    string result;
    
    size_t curr = 0;
    size_t size = bytes.size();
    while (curr < size)
    {
        //we have a full triplet and can convert without issues
        if (size - curr >= 3)
        {
            byte a = bytes[curr];
            byte b = bytes[curr+1];
            byte c = bytes[curr+2];

            result.push_back(bitsToBase64(a >> 2));
            result.push_back(bitsToBase64((a & 0x3) << 4 | b >> 4));
            result.push_back(bitsToBase64((b & 0xf) << 2 | c >> 6));
            result.push_back(bitsToBase64(c & 0x3f));
        }
        else if (size - curr == 2)
        {
            byte a = bytes[curr];
            byte b = bytes[curr+1];

            result.push_back(bitsToBase64(a >> 2));
            result.push_back(bitsToBase64((a & 0x3) << 4 | b >> 4));
            result.push_back(bitsToBase64(b << 4));
            result.push_back('=');
        }
        else
        {
            byte a = bytes[curr];

            result.push_back(bitsToBase64(a >> 2));
            result.push_back(bitsToBase64((a & 0x3) << 4));
            result.push_back('=');
            result.push_back('=');
        }

        curr += 3;
    }

    return result;
}

string bytesToHex(const bytestring& bytes)
{
    string result;

    for (auto b : bytes)
    {
        result.push_back(bitsToHex(b >> 4));
        result.push_back(bitsToHex(b & 0x0f));
    }

    return result;
}
