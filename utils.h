#pragma once

#include "defines.h"


bytestring hexToBytes(const string& input);
bytestring base64ToBytes(const string& input);
string bytesToBase64(const bytestring& bytes);
string bytesToHex(const bytestring& bytes);
size_t hamming_distance(const bytestring& a, const bytestring& b);