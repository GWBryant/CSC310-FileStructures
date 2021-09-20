#include <fstream>
#include <iostream>
#include <sys/stat.h>

using namespace std;


bool ValidNAme(string name);

int main(int argc, char **argv)
{
    string dirName = argv[1];
    cout << boolalpha << ValidNAme(dirName) << endl;
    return 0;
}

bool ValidNAme(string name)
{
    struct stat buf;
    lstat (name.c_str(), &buf);
    return S_ISDIR(buf.st_mode);
}