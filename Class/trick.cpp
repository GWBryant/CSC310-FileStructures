
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char * argv[])
{

        char ch;
        ifstream infile(argv[1]);
        infile.get(ch);
        if (!infile)
                cout << "File pointer dead -- it's a directory" << endl;
        else
                cout << "File pointer alive -- it's a file" << endl;

        return 0;
}

