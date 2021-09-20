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
    ifstream i(argv[2]);
    if(!i)
    {
        cerr << "createBST: file " << argv[2] << " does not exist" << endl;
        return 1;
    }
    i.close();
    /*******************/

    fstream inFile(argv[1],ios::in);
    BTree myTree;
    myTree.reset(argv[2]);
    myTree.writeHeader(argv[2]);
    Album t;
    while(inFile >> t)
    {
        if(inFile)
        {
            cout << "Now inserting " << t << endl;    
            myTree.insert(t);
        }
    }
    myTree.printTree();
    myTree.close();
    return 0;
}