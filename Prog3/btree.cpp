#include "btree.h"

/***constructor***/
BTree::BTree()
{
    read = 0;
    write = 0;
    height = 0;
    root.currSize = 0;
    for(int i = 0; i < ORDER; i++)
    {
        root.child[i] = -1;
    }
}
/******************/

//resets the header for when you are creating a new BTree
void BTree::reset(char * fileName)
{
    treeFile.open(fileName,ios::trunc|ios::in|ios::out|ios::binary);
    BTNode node;
    node.child[0] = sizeof(node);
    treeFile.write((char *) &node, sizeof(BTNode));
    write++;
    treeFile.write((char *) &root, sizeof(BTNode));
    write++;
    treeFile.close();
}

//grabs values from the header
void BTree::writeHeader(char * fileName)
{
    treeFile.open(fileName,ios::in|ios::out|ios::binary);
    strcpy(treeFileName,fileName);
    BTNode node;
    treeFile.seekg(0,ios::beg);
    treeFile.read((char *) &node, sizeof(BTNode));
    read++;
    rootAddr = node.child[0];
    treeFile.seekg(rootAddr);
    treeFile.read((char *) &root, sizeof(BTNode));
    read++;
}

/***inserting keys into the tree***/
void BTree::insert(keyType key)
{
    insert(key,rootAddr,-1,-1);
}

//insert into the tree
void BTree::insert(keyType key, int recAddr, int oneAddr, int twoAddr)
{
    BTNode t = getNode(recAddr);
    if(!isLeaf(t))
    {
        for(int i = 0; i < t.currSize; i++)
        {
            if(key < t.contents[i])
            {
                insert(key,t.child[i], oneAddr, twoAddr);
                return;
            }
        }
        insert(key,t.child[t.currSize], oneAddr, twoAddr);
        return;
    }
    else
    {
        if(t.currSize != ORDER-1)
        {
            t.contents[t.currSize] = key;
            t.currSize++;
            /***sort***/
            keyType temp;
            for(int i = t.currSize-1; i > 0; i--)
            {
                if(t.contents[i] < t.contents[i-1])
                {
                    temp = t.contents[i-1];
                    t.contents[i-1] = t.contents[i];
                    t.contents[i] = temp;
                }
            }
            /**********/
            treeFile.seekg(recAddr,ios::beg);
            treeFile.write((char *) &t, sizeof(BTNode));
            write++;
        }
        else
        {
            cout << "Now Splitting!" << endl;
            splitNode(key,recAddr,oneAddr,twoAddr);
        }
    }
}

//split a node up
void BTree::splitNode(keyType& key,int recAddr,int& oneAddr,int& twoAddr)
{
    //edit current BTNode
    BTNode t = getNode(recAddr);
    keyType temp;
    int swapAddr;

    if(key < t.contents[t.currSize-1])
    {
        temp = t.contents[t.currSize-1];
        t.contents[t.currSize-1] = key;
        key = temp;

        swapAddr = t.child[t.currSize];
        t.child[t.currSize] = twoAddr;
        twoAddr = swapAddr;
        for(int i = t.currSize-1; i > 0; i--)
        {
            if(t.contents[i] < t.contents[i-1])
            {
                temp = t.contents[i-1];
                t.contents[i-1] = t.contents[i];
                t.contents[i] = temp;

                swapAddr = t.child[i];
                t.child[i] = t.child[i+1];
                t.child[i+1] = swapAddr;
            }
            else
                break;
        }
    }

    keyType  rootVal = t.contents[ORDER/2];
    t.currSize = 2;
    treeFile.seekg(recAddr,ios::beg);
    treeFile.write((char *) &t, sizeof(BTNode));
    write++;

    //create new node for split
    BTNode splitNode;
    splitNode.contents[0] = t.contents[3];
    splitNode.contents[1] = key;
    splitNode.currSize = 2;
    splitNode.child[0] = t.child[3];
    splitNode.child[1] = t.child[4];
    splitNode.child[2] = twoAddr;
    treeFile.seekg(0,ios::end);
    int rightAddr = treeFile.tellg();
    treeFile.write((char *) &splitNode, sizeof(BTNode));
    write++;

    if(recAddr == rootAddr)
    {
        //adjust the root
        cout << "Adjusting Root!" << endl;
        adjRoot(rootVal,recAddr,rightAddr);

        //indicate height change
        height++;
    }
    else
    {
        //place temp key in root
        placeNode(rootVal,findpAddr(key,root,rootAddr,recAddr),recAddr,rightAddr);
    }
}

//place a key in its parent when promoting
void BTree::placeNode(keyType k,int recAddr,int oneAddr,int twoAddr)
{
    BTNode parent = getNode(recAddr);
    if(parent.currSize != ORDER-1)
    {
        int i;
        for(i = parent.currSize-1; i >= 0; i--)
        {
            if(k < parent.contents[i])
            {
                parent.contents[i+1] = parent.contents[i];
                parent.child[i+2] = parent.child[i+1];
            }
            else
                break;
        }
        parent.contents[i+1] = k;
        parent.child[i+2] = twoAddr;
        parent.currSize++;
        treeFile.seekg(recAddr,ios::beg);
        treeFile.write((char *) &parent, sizeof(BTNode));
        write++;

        if(recAddr == rootAddr)
            root = parent;
    }
    else 
    {
        cout << "Now Splitting!" << endl;
        splitNode(k,recAddr,oneAddr,twoAddr);
    }
}

//adjust the root node when the root splits
void BTree::adjRoot(keyType rootElem, int oneAddr, int twoAddr)
{
    BTNode newRoot;
    newRoot.currSize = 1;
    newRoot.contents[0] = rootElem;
    newRoot.child[0] = oneAddr;
    newRoot.child[1] = twoAddr;
    treeFile.seekg(0,ios::end);
    rootAddr = treeFile.tellg();
    treeFile.write((char *) &newRoot, sizeof(BTNode));
    write++;
    root = newRoot;

    BTNode dummy = getNode(0);
    dummy.child[0] = rootAddr;
    treeFile.seekg(0,ios::beg);
    treeFile.write((char *) &dummy, sizeof(BTNode));
    write++;
}
/**************************************/

/***get a node given a rec Adress***/
BTNode BTree::getNode(int recAddr)
{
    BTNode temp;
    treeFile.seekg(recAddr,ios::beg);
    treeFile.read((char *) &temp,sizeof(BTNode));
    read++;
    return temp;
}
/***********************************/

/***leaf code***/
bool BTree::isLeaf(int recAddr)
{
    BTNode t = getNode(recAddr);
    return isLeaf(t);
}

//tell if a node is a leaf
bool BTree::isLeaf(BTNode t)
{
    for(int i = 0; i < t.currSize; i++)
    {
        if(t.child[i] != -1)
            return false;
    }
    return true;
}

int BTree::countLeaves()
{
    return countLeaves(rootAddr);
}

//count leaves in tree
int BTree::countLeaves(int recAddr)
{
    if(recAddr != -1)
    {
        BTNode node = getNode(recAddr);
        if(isLeaf(node))
            return 1;
        else
        {
            for(int j = 0; j <= node.currSize; j++)
            {
                if(node.child[j] != -1)
                    countLeaves(node.child[j]);
            }
        }
    }
    return 0;
}
/***************/

/***search functions***/
bool BTree::search(string key) //public
{
    return search(key,root,rootAddr);
}

//search for a key and see if its in the tree
bool BTree::search(string key, BTNode t, int tAddr) //private
{
    for(int i = 0; i < t.currSize; i++)
    {
        // cout << key << " = " << t.contents[i].getUPC() << " is " << boolalpha << (strcmp(key.c_str(), t.contents[i].getUPC().c_str()) == 0) << endl;
        if(strcmp(key.c_str(), t.contents[i].getUPC().c_str()) == 0)
        {
            // cout << "returning" << endl;
            return true;
        }
    }
    if(!isLeaf(tAddr))
    {
        for(int i = 0; i < t.currSize; i++)
        {
            if(key < t.contents[i].getUPC())
                return search(key,getNode(t.child[i]),t.child[i]);
        }
        return search(key,getNode(t.child[t.currSize]),t.child[t.currSize]);
    }
    return false;
}

//find the parent address of a given node
int BTree::findpAddr(keyType key, BTNode t, int tAddr, int findAddr)
{
    if(tAddr != -1)
    {
        for(int i = 0; i <= t.currSize; i++)
        {
            if(t.child[i] == findAddr)
                return tAddr;
        }
        for(int i = 0; i < t.currSize; i++)
        {
            if(key < t.contents[i])
            {
                if(t.child[i] != -1)
                    return findpAddr(key,getNode(t.child[i]),t.child[i],findAddr);
                else
                    return -1;
            }
        }
        if(t.child[t.currSize] != -1)
            return findpAddr(key,getNode(t.child[t.currSize]),t.child[t.currSize],findAddr);
        else
            return -1;
    }
    return -1;
}

//public retrieve call
keyType BTree::retrieve (string key)
{
    return retrieve(key,root,rootAddr);
}

//retrieve a value you by entering a key
keyType BTree::retrieve(string key,BTNode t,int tAddr)
{
    for(int i = 0; i < t.currSize; i++)
    {
        if(strcmp(key.c_str(), t.contents[i].getUPC().c_str()) == 0)
        {
            return t.contents[i];
        }
    }
    if(!isLeaf(tAddr))
    {
        for(int i = 0; i < t.currSize; i++)
        {
            if(key < t.contents[i].getUPC())
                return retrieve(key,getNode(t.child[i]),t.child[i]);
        }
        return retrieve(key,getNode(t.child[t.currSize]),t.child[t.currSize]);
    }
    keyType k;
    return k;
}
/*********************/

/***print functions***/
//public
void BTree::printTree()
{
    cout << "-------- B-tree of height " << height << " --------" << endl;
    printTree(rootAddr);
}

void BTree::inorder()
{
    cout << "-------- B-tree of height " << height << " --------" << endl;
    inorder(rootAddr);
}

void BTree::reverse()
{
    cout << "-------- B-tree of height " << height << " --------" << endl;
    reverse(rootAddr);
}
//private
//print tree node by node in preorder traversal
void BTree::printTree(int recAddr)
{
    if(recAddr != -1 && recAddr != 0)
    {
        BTNode t = getNode(recAddr);  
        printNode(recAddr);
        for(int i = 0; i <= t.currSize; i++)
        {
            printTree(t.child[i]);
        }
    }
}

//print inorder traversal of tree contents
void BTree::inorder(int recAddr)
{
    if(recAddr != -1 && recAddr != 0)
    {
        BTNode t = getNode(recAddr);
        for(int i = 0; i < t.currSize; i++)
        {
            if(t.child[i] != -1)
                inorder(t.child[i]);
            cout << t.contents[i] << endl;
        }
        inorder(t.child[t.currSize]);
    }
}

//print tree in reverse order
void BTree::reverse(int recAddr)
{
    if(recAddr != -1 && recAddr != 0)
    {
        BTNode t = getNode(recAddr);
        reverse(t.child[t.currSize]);
        for(int i = t.currSize-1; i >= 0; i--)
        {
            if(t.child[i] != -1)
                reverse(t.child[i]);
            cout << t.contents[i] << endl;
        }
    }
}

//print a single node of the tree
void BTree::printNode(int recAddr)
{
    treeFile.seekg(recAddr,ios::beg);
    BTNode t;
    treeFile.read((char *) &t, sizeof(BTNode));
    read++;
    cout << "    *** node of size " << t.currSize << " ***" << endl;
    for(int i = 0; i < t.currSize; i++)
        cout << t.contents[i] << endl;
}
/*********************/

//close the file
void BTree::close()
{
    treeFile.close();
}

//print total reads and writes performed
void BTree::totalio() const
{
    cout << "\tTotal Reads: " << read << endl;
    cout << "\tTotal Writes: " << write << endl;
}