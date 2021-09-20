#include <string>
#include <iostream>
#include <sstream>


using namespace std;

int totalCompressed(string compressed);

int main()
{
    string comp = "ff 93 04 92 91 91 ff 94 26";
    cout << totalCompressed(comp) << endl;
    return 0;
}

int totalCompressed(string compressed)
{
    string input, decomp, freq;
    int retVal = 0;
    stringstream ss(compressed);
    while(ss >> input)
    {
        if(input == "ff")
        {
            ss >> input >> freq;
            retVal += stoi(freq);
        }
        else
            retVal++;
    }
    return retVal;
}

