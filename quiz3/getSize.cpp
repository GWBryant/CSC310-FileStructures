#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
    //fstream inFile(argv[1],ios::in);
    fstream inFile(argv[1],ios::out);
    inFile.seekg(0,ios::end);
    inFile << "2";
    // string s = "";
    // s = to_string(inFile.tellg());
    // cout << s << endl;
    return 0;
}