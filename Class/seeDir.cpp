#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void read_directory(string& name, vector<string>& v);

int main(int argc, char **argv)
{
    vector<string> v;
    string dirName = argv[1];
    read_directory(dirName,v);
    for(int i = 0; i < v.size(); i++)
        cout << argv[1] << '/' << v[i] << endl;
    return 0;
}
 
void read_directory(string& name, vector<string>& v)
{
    DIR* dirp = opendir(name.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
        v.push_back(dp->d_name);
    }
    closedir(dirp);
}