#include "utils.h"

#include <cassert>
#include <cctype>

byte hexCharToBits(char hex)
{
    assert(std::isxdigit(hex));

    if (hex <= '9')
        return hex - '0';
    else
        return hex - 'a' + 10;
}

byte base64CharToBits(char base64)
{
    assert(std::isalnum(base64) || base64 == '+' || base64 == '/');

    if (base64 == '+')
        return 62;
    else if (base64 == '/')
        return 63;
    else if (base64 <= '9')
        return 52 + (base64 - '0');
    else if (base64 <= 'Z')
        return base64 - 'A';
    else
        return 26 + (base64 - 'a');
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

bytestring base64ToBytes(const string& input)
{
    //input string should always be multiple of 4 chars
    assert(input.size() % 4 == 0);

    bytestring result;

    size_t length = input.size();

    for (size_t i = 0; i < length; i += 4)
    {
        //one byte
        if (input[i + 2] == '=')
        {
            byte a = base64CharToBits(input[i]);
            byte b = base64CharToBits(input[i + 1]);

            result.push_back(a << 2 | b >> 4);
        }
        //two bytes
        else if (input[i + 3] == '=')
        {
            byte a = base64CharToBits(input[i]);
            byte b = base64CharToBits(input[i + 1]);
            byte c = base64CharToBits(input[i + 2]);

            result.push_back(a << 2 | b >> 4);
            result.push_back(b << 4 | c >> 2);
        }
        //standard case
        else
        {
            byte a = base64CharToBits(input[i]);
            byte b = base64CharToBits(input[i + 1]);
            byte c = base64CharToBits(input[i + 2]);
            byte d = base64CharToBits(input[i + 3]);

            result.push_back(a << 2 | b >> 4);
            result.push_back(b << 4 | c >> 2);
            result.push_back(c << 6 | d);
        }
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
            result.push_back(bitsToBase64((b & 0xF) << 2));
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

size_t hamming_distance(const bytestring& a, const bytestring& b)
{
    assert(a.size() == b.size());
    //hamming distance -> number of 1 bits in a xor b

    size_t distance = 0;
    
    size_t length = a.size();
    for (size_t i = 0; i < length; ++i)
    {
        byte x = a[i] ^ b[i];

        for (; x; distance++)
            x &= x - 1;
    }

    return distance;
}
