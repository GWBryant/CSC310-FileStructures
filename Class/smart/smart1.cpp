
#include <iostream>
#include <memory>
using namespace std;

void my_func();
int main()
{
       my_func();
       return 1;
}

void my_func()
{
       unique_ptr<int> valuePtr(new int(15));
       int x = 45;

       if (x == 45)
             return;   // no memory leak!
}
