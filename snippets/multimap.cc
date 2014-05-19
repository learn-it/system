#include <string.h>
#include <iostream>
#include <map>

using namespace std;

struct ltstr
{
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) < 0;
  }
};

int main()
{
  multimap<size_t, int> m;
  
  m.insert(pair<size_t, int>(1, 1));
  m.insert(pair<size_t, int>(3, 2));
  m.insert(pair<size_t, int>(3, 2));
  m.insert(pair<size_t, int>(3, 2));
  m.insert(pair<size_t, int>(3, 2));
  m.insert(pair<size_t, int>(2, 3));

  cout << "Elements in m: " << endl;
  for (multimap<size_t, int>::iterator it = m.begin();
       it != m.end();
       ++it)
   cout << "  [" << (*it).first << ", " << (*it).second << "]" << endl;
   
   cout << (2 || 0) << endl;
}
