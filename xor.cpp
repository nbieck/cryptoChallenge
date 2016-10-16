#include "defines.h"
#include "utils.h"

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

        score += std::max(0, 3 - diff);
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

//returns a set of likely keys
vector<bytestring> break_repeating_key_xor(const bytestring& ciphertext)
{
    //find keysize using normalized hamming distance
    vector<float> hamming_dists;

    for (int i = 2; i <= 40; ++i)
    {
        bytestring a(ciphertext.begin(), ciphertext.begin() + i);
        bytestring b(ciphertext.begin() + i, ciphertext.begin() + i * 2);
        bytestring c(ciphertext.begin() + i * 2, ciphertext.begin() + i * 3);
        bytestring d(ciphertext.begin() + i * 3, ciphertext.begin() + i * 4);

        size_t distance = hamming_distance(a, b);
        distance += hamming_distance(a, c);
        distance += hamming_distance(a, d);
        hamming_dists.push_back(static_cast<float>(distance) / static_cast<float>(i * 3.f));
    }

    vector<bytestring> keys;

    for (int j = 0; j < 4; ++j)
    {
        auto it = std::min_element(hamming_dists.begin(), hamming_dists.end());
        size_t key_length = std::distance(hamming_dists.begin(), it) + 2;
        *it = std::numeric_limits<float>::max();

        //transpose blocks
        vector<bytestring> blocks(key_length);

        size_t length = ciphertext.size();
        for (size_t i = 0; i < length; ++i)
        {
            blocks[i % key_length].push_back(ciphertext[i]);
        }

        //solve individual single XOR
        bytestring key;
        for (const auto& block : blocks)
        {
            key.push_back(find_xor_key(block));
        }

        keys.push_back(key);
    }

    return keys;
}

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

int main(void)
{
    std::ifstream file("6.txt");

    if (file)
    {
        std::stringstream buff;
        buff << file.rdbuf();
        string contents = buff.str();
        string base64;

        std::copy_if(contents.begin(), contents.end(), std::back_inserter(base64), [](char c) {return !std::iscntrl(c); });

        bytestring ciphertext = base64ToBytes(base64);

        vector<bytestring> keys = break_repeating_key_xor(ciphertext);

        for (auto k : keys)
        {
            auto text = repeating_key_xor(ciphertext, k);
            string res(text.begin(), text.end());

            std::cout << res << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
        }
    }

    system("pause");
    return 0;
}