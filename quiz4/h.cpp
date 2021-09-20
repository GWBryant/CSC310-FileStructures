#include <iostream>
#include <string>

using namespace std;

int H1(char *s);
int H2(char *s);
int main()
{
    //char *s1 = "BEAR";
    //char *s2 = "BARE";
    cout << "func 1:" << H1("BEAR") << " " << H1("BARE") << endl;
    cout << "func 2:" << H2("BEAR") << " " << H2("BARE") << endl;
}

int H1(char *s)
{
    int r = 1;
    int i;
    for(i = 0; i < 4; ++i)
        r = r* (int)s[i];
    return r % 2001;
}

int H2(char *s)
{
    int r = 0;
    int i;
    for(i = 0; i < 4; ++i)
        r = 3*r + (int) s[i];
    return r % 2001;
}