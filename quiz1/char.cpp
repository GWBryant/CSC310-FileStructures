#include <iostream>

using namespace std;

struct student
{
    char gender;
    int age;
    char name[20];
};

int main()
{
    student s;
    int i = 0;
    char c = 'a';
    char cs[20];
    cout << sizeof(s) << endl;
}