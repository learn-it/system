#include <iostream>

using namespace std;

class Ex
{
public:
    Ex()
    {
        cout << "Constructed!\n";
    }

    Ex(const Ex& e)
    {
        cout << "Copied!\n";
    }
    
    Ex& clork()
    {
        return *this;
    }
};

int main()
{
    try {
        throw Ex().clork();
    } catch (Ex& e) {}
    return 0;
}
