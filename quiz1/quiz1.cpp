#include <iostream>
#include <string.h>

using namespace std;

int main() 
{
    char * s = new char[25];
    char * sp = s;
    string line = ""; int i , j;
    cin >> j;
    strcpy(s,"0123456789");
    for(int i = 0; i < j; i++) {
        *s = ('0' + i%10);
        line += *s;
        s++;
    }
    cout << "s = " << s << "\nsp = " << sp << "\nline = " << line << endl;
    return 0;
}