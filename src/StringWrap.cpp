#include <StringWrap.h>

StringWrap::StringWrap() {}
StringWrap::StringWrap(std::string s) {}
StringWrap::~StringWrap() {}

char* StringWrap::copy() {
    char copyOfAString[length()];
    std::string::copy(copyOfAString, length(), 0);
    return copyOfAString;
}