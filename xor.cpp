#include "defines.h"

#include <cassert>
#include <algorithm>
#include <cctype>

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

bytestring single_xor(const bytestring& test, byte key)
{
    bytestring result(test);

    std::transform(result.begin(), result.end(), result.begin(), [key](byte a) {return a^key; });

    return result;
}

unsigned score_string(const bytestring& input)
{
    vector<std::pair<byte, int>> char_counts(26);
    byte c = 'a';
    std::generate(char_counts.begin(), char_counts.end(),
        [&c]() 
        {
            std::pair<byte, int> p;
            p.first = c;
            p.second = -1;
            c++;

            return p;
        });

    for (auto b : input)
    {
        if (std::isalpha(b))
        {
            char_counts[std::tolower(b) - 'a'].second++;
        }
        if (std::iscntrl(b) && !std::isspace(b))
        {
            return 0;
        }
    }

    std::sort(char_counts.begin(), char_counts.end(), 
        [](std::pair<byte, int>& a, std::pair<byte, int>& b) 
        {
            return a.second > b.second;
        });

    const int frequencies[] = 
    {
        3, 20, 12, 10,
        1, 16, 17, 8,
        5, 23, 22, 11,
        14, 6, 4, 19,
        25, 9, 7, 2,
        13, 21, 15, 24,
        18, 26
    };

    unsigned score = 0;

    for (int i = 0; i < 26; ++i)
    {
        auto p = char_counts[i];

        if (p.second < 0)
            continue;

        int freq = frequencies[p.first - 'a'];

        int diff = abs((i + 1) - freq);

        score += std::max(0, 2 - diff);
    }

    return score;
}

byte find_xor_key(const bytestring& ciphertext, unsigned* score = nullptr)
{
    vector<std::pair<byte, unsigned>> score_per_key;

    for (unsigned i = 0; i < 256; ++i)
    {
        bytestring decoded = single_xor(ciphertext, i);

        score_per_key.push_back(std::make_pair(i, score_string(decoded)));
    }

    std::sort(score_per_key.begin(), score_per_key.end(), 
        [](const std::pair<byte, unsigned>& a, const std::pair<byte, unsigned>& b)
        {
            return a.second > b.second;
        });

    if (score)
    {
        *score = score_per_key[0].second;
    }
    return score_per_key[0].first;
}

bytestring repeating_key_xor(const bytestring& text, const bytestring& key)
{
    bytestring result(text);

    size_t key_length = key.size();
    size_t text_length = text.size();

    for (size_t i = 0; i < text_length; ++i)
    {
        size_t j = i % key_length;

        result[i] = text[i] ^ key[j];
    }

    return result;
}

#include "encoding.h"
#include <iostream>
#include <fstream>

int main(void)
{
    string text("Burning 'em, if you ain't quick and nimble I go crazy when I hear a cymbal");
    bytestring test_bin(text.begin(), text.end());

    bytestring key = { 'I', 'C', 'E' };

    bytestring cypher = repeating_key_xor(test_bin, key);

    std::cout << bytesToHex(cypher) << std::endl;

    system("pause");
    return 0;
}