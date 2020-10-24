#include <string>

class StringWrap : public std::string
{
private:
public:
    StringWrap();
    StringWrap(std::string s);
    ~StringWrap();

    public:
    char* copy();
};
