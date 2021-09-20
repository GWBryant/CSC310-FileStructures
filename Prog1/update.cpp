#include <fstream>
#include <iostream>
#include <map>
#include <iomanip>

using namespace std;

enum TransactionType {Add, Delete, ChangeOnhand, ChangePrice};
typedef char String[25];

struct BookRec
{
    unsigned int isbn;
    String name;
    String author;
    int onhand;
    float price;
    String type;
};

struct TransactionRec
{
   TransactionType todo;
   BookRec B; 
};

void createMap(map<unsigned int, long> &offsetMap);
void updateFile(map<unsigned int, long> &offsetMap, string transact, fstream &errOut);
void addBook(map<unsigned int, long> &offsetMap, TransactionRec buff, fstream &errOut, int tNum);
void deleteBook(map<unsigned int, long> &offsetMap, TransactionRec buff, fstream &errOut, int tNum);
void changeOnHand(map<unsigned int, long> &offsetMap, TransactionRec buff, fstream &errOut, int tNum);
void changePrice(map<unsigned int, long> &offsetMap, TransactionRec buff, fstream &errOut, int tNum);
void printRecords(map<unsigned int, long> &offsetMap, string newMaster);
void printMaster(BookRec buffer);

int main(int argc, char **argv)
{
    //get file names
    string masterFile = argv[1];
    string transactFile = argv[2];
    string newMaster = argv[3];

    //buffers
    TransactionRec Tbuffer;
    BookRec BookBuffer;

    //open transaction and error file
    fstream transactInFile(transactFile,ios::in | ios::binary);
    fstream errOut("ERRORS",ios::out);

    //make copy of master file
    string command = "cp " + masterFile + " copy.out";
    system(command.c_str());

    //create map
    map<unsigned int, long> offsetMap;
    createMap(offsetMap);

    //update file
    updateFile(offsetMap,transactFile, errOut);

    //print records to new master file
    printRecords(offsetMap,newMaster);

    //get rid of temporary copy
    system("rm copy.out");
    return 0;
}

//create a map between isbn's and byte offsets
void createMap(map<unsigned int, long> &offsetMap)
{
    //open copy.out
    fstream inFile("copy.out",ios::in|ios::binary);

    BookRec BookBuffer;

    //go to the beginning of the file
    while(inFile.read((char *) &BookBuffer, sizeof(BookBuffer)))
    {
        //grab byte offset of bookRec and create a mapping between the isbn and offset
        long offset = inFile.tellg();
        offsetMap[BookBuffer.isbn] = offset;
    }
    inFile.close(); //close the file
}

void updateFile(map<unsigned int, long> &offsetMap, string transact, fstream &errOut)
{
    TransactionRec Tbuffer;
    fstream transactFile(transact,ios::in|ios::binary);
    int tNum = 1;
    while(transactFile.read((char *) &Tbuffer, sizeof(Tbuffer)))
    {
        if(Tbuffer.todo == Add)
        {
            addBook(offsetMap,Tbuffer,errOut, tNum);
        }
        else if(Tbuffer.todo == Delete)
        {
            deleteBook(offsetMap,Tbuffer,errOut, tNum);
        }
        else if(Tbuffer.todo == ChangeOnhand)
        {
            changeOnHand(offsetMap,Tbuffer,errOut, tNum);
        }
        else if(Tbuffer.todo == ChangePrice)
        {
            changePrice(offsetMap,Tbuffer,errOut, tNum);
        }
        tNum++;
    }
    transactFile.close();
}

//add a book to the database
void addBook(map<unsigned int, long> &offsetMap, TransactionRec buff, fstream &errOut, int tNum)
{
    fstream copyOut("copy.out",ios::out|ios::in|ios::binary);
    if(offsetMap.find(buff.B.isbn) == offsetMap.end())
    {
        copyOut.seekg(0,ios::end);
        copyOut.write((char *) &buff.B, sizeof(buff.B));
        offsetMap[buff.B.isbn] = copyOut.tellp();
    }
    else
    {
        errOut << "Error in transaction number " << tNum << ": cannot add duplicate key " << buff.B.isbn << endl;  
    }
    copyOut.close();   
}

//delete a book from the database
void deleteBook(map<unsigned int, long> &offsetMap, TransactionRec buff, fstream &errOut, int tNum)
{
    //if the isbn is in the map remove it from the map
    if(offsetMap.find(buff.B.isbn) != offsetMap.end())
    {
        offsetMap.erase(buff.B.isbn);
    }
    else
    {
        errOut << "Error in transaction number " << tNum << ": cannot delete - no such key " << buff.B.isbn << endl;  
    }
}

//change on hand
void changeOnHand(map<unsigned int, long> &offsetMap, TransactionRec buff, fstream &errOut, int tNum)
{
    fstream copyOut("copy.out",ios::out|ios::in|ios::binary);
    BookRec buffer;
    if(offsetMap.find(buff.B.isbn) != offsetMap.end())
    {
        //pull out the bookRec from the copy of the master to access its onhadn val
        copyOut.seekg(offsetMap[buff.B.isbn] - sizeof(BookRec));
        copyOut.read((char *) &buffer, sizeof(buffer));
        //add the old onhand val to the new onhand val
        buff.B.onhand += buffer.onhand;
        //if it makes it negative set it to zero
        if(buff.B.onhand < 0)
        {
            errOut << "Error in transaction number " << tNum << ": count = " << buff.B.onhand << " for key " << buff.B.isbn << endl;
            buff.B.onhand = 0;
        }
        //get rid of the old bookRec and add the new one
        offsetMap.erase(buffer.isbn);
        copyOut.seekg(0,ios::end);
        copyOut.write((char *) &buff.B, sizeof(buff.B));
        offsetMap[buff.B.isbn] = copyOut.tellp();
    }
    else
    {
        errOut << "Error in transaction number " << tNum << ": cannot change count - no such key " << buff.B.isbn << endl;  
    }   
    copyOut.close();
}

//change price
void changePrice(map<unsigned int, long> &offsetMap, TransactionRec buff, fstream &errOut, int tNum)
{
    fstream copyOut("copy.out",ios::out|ios::in|ios::binary);
    if(offsetMap.find(buff.B.isbn) != offsetMap.end())
    {
        copyOut.seekg(0,ios::end);
        offsetMap.erase(buff.B.isbn);
        copyOut.write((char *) &buff.B, sizeof(buff.B));
        offsetMap[buff.B.isbn] = copyOut.tellp();
    }
    else
    {
        errOut << "Error in transaction number " << tNum << ": cannot change price - no such key " << buff.B.isbn << endl;  
    }   
    copyOut.close(); 
}

//print records to new master file
void printRecords(map<unsigned int, long> &offsetMap, string newMaster)
{
    fstream copyFile("copy.out",ios::in|ios::binary);
    fstream masterFile(newMaster,ios::out|ios::binary);
    BookRec buffer;
    map<unsigned int, long>::iterator itr;

    for(itr = offsetMap.begin(); itr != offsetMap.end(); ++itr)
    {
        copyFile.seekg(itr->second - sizeof(BookRec));
        copyFile.read((char *) &buffer, sizeof(buffer));
        printMaster(buffer);
        masterFile.write((char *) &buffer, sizeof(buffer));
    }
    copyFile.close(); 
    masterFile.close();
}

//print master file to the screen
void printMaster(BookRec buffer)
{
    cout<< setprecision(2) << fixed 
    << setw(10) <<setfill('0') << buffer.isbn
    <<setw(25)<<setfill(' ')<<buffer.name
    <<setw(25)<<buffer.author
    <<setw(3)<<buffer.onhand
    <<setw(6)<<buffer.price
    <<setw(10)<<buffer.type<<endl;
}