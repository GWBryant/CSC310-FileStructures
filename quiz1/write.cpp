#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    fstream inFile("file.dat",ios::in|ios::out);
    char a;
    char b;
    inFile.seekg(703,ios::beg);
    cout << inFile.tellg() << " ";
    inFile.seekg(-2,ios::cur);
    cout << inFile.tellg();
    inFile >> a;
    inFile >> b >> b >> b;
    cout << inFile.tellg() << a << b;
    inFile.seekg(0,ios::beg);
    inFile << a << b;
}