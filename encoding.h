#pragma once

#include "defines.h"


bytestring hexToBytes(const string& input);
string bytesToBase64(const bytestring& bytes);
string bytesToHex(const bytestring& bytes);