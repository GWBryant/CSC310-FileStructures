#include "btree.h"
#include "album.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv)
{
    /***error handling***/
    if(argc < 3 || argc > 3)
    {
        cerr << "invalid command line arguments" << endl << "try createBT --help for help" << endl;
        return 1;
    }
    ifstream i(argv[1]);
    if(!i)
    {
        cerr << "updateBST: file " << argv[1] << " does not exist" << endl;
        return 1;
    }
    i.close();
    /********************/
    fstream inFile(argv[2],ios::in);

    BTree myTree;
    myTree.writeHeader(argv[1]);
    Album t;
    char option;
    string dummy;
    string key;
    while(inFile >> option)
    {
        if(option == 'I')
        {
            getline(inFile,dummy);
            if(inFile)
            {
                inFile >> t;
                cout << "Now inserting " << t << endl;    
                myTree.insert(t);
            }
        }
        else if(option == 'S')
        {
            getline(inFile,dummy);
            if(inFile)
            {
                inFile >> key;
                if(myTree.search(key))
                {
                    keyType a = myTree.retrieve(key);
                    cout << "\t" << a << endl;
                }
                else
                {
                    cout << "\t\"" << key << "\" not found" << endl;
                }
            }
        }
    }
    cout << endl;
    myTree.totalio();
    myTree.close();
    return 0;
}