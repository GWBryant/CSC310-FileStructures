#include <fstream>
#include <iostream>
using namespace std;

int main()
{
	fstream inOut ("file.dat", ios::in | ios::out);
        inOut.seekg(703);

        char ch1, ch2;
        inOut.get(ch1);

        inOut.seekg(700);
        inOut.get(ch2);

        if (ch1 < ch2)
        {
            inOut << ch1;
            inOut << ch2;
        }
        else
        {
            inOut << ch2;
            inOut << ch1;
        }

        return 0;
}
