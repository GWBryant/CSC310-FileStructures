#include "album.h"
#include <iostream>
#include <fstream>

//const int UpTo = 50;
//typedef char String[UpTo];

using namespace std;

int main()
{
    String x = "a";
    String y = "b";
    String z = "c";
    Album a(x,y,z);
    Album b(a);
    Album c;
    cout << a << endl;
    cout << b << endl;
    cout << c << endl;
    c = a;
    cout << c << endl;
    cout << a.getUPC() << " " << b.getUPC() << " " << c.getUPC() << endl;
    cout << a.recordSize() << " " << b.recordSize() << " " << c.recordSize() << endl;
    fstream in("album.baby",ios::in);
    in >> c;
    cout << c << endl;
    cout << c.recordSize() << endl;
}