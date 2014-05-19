#include <map>
#include <iostream>
#include <string>
#include <string.h>

using namespace std;

typedef map<int, void *> map2;
typedef map<int, string> map1;

int main()
{
    map1 m;
    pair<map1::iterator, bool> res = m.insert(map1::value_type(0, "a"));
    m.insert(map1::value_type(1, "a"));
    void *b;
    memcpy(&b, &res.first, sizeof(b));
    cout << sizeof(void*) << endl;
    map1::iterator i = m.end();
    i--;
    cout << distance(m.begin(), i) << endl;
    return 0;
}