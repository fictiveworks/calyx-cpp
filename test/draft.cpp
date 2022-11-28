#include <string>
#include <iostream>

class Result
{
public:
    Result(std::string exp) : _exp(exp) {}

    std::string text()
    {
        return _exp;
    }

private:
    std::string _exp;
};

class Grammar
{
public:
    void start(std::string term)
    {
        _term = term;
    }

    Result generate()
    {
        return Result(_term);
    }
private:
    std::string _term;
};

int main(int argc, char** argv)
{
    Grammar g = Grammar();
    g.start("startrule");
    Result r = g.generate();

    std::cout << r.text();

    return 0;
}
