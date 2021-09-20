#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

//typedef for String as length 25 char array
typedef char String[25];
//book record struct
struct BookRec
{
    unsigned int isbn;
    String name;
    String author;
    int onhand;
    float price;
    String type;
};

bool processRec(BookRec &buffer, ifstream &inFile,unsigned int &saveIsbn, int line, long isbn); //get data for bookRecord
void printRecord(BookRec &buffer);  //print a single record
void printFile(string outFileName); //print a binary file

int main(int argc, char **argv)
{
    //declare and instantiate variables
    bool validData;
    string outFileName = "library.out";
    string fileName = argv[1];
    ifstream inFile(fileName);
    fstream outFile(outFileName,ios::out|ios::binary);
    long isbn;

    BookRec buffer;
    int i = 1;  //tracks line number
    unsigned int saveIsbn = 0;
    //loop through input file
    while(inFile >> isbn) 
    {
        validData = processRec(buffer,inFile,saveIsbn,i,isbn);
        if(validData)   //only output data to binary file if the book record is valid
            outFile.write((char *) &buffer, sizeof(BookRec));
        i++;
    }
    outFile.close();

    printFile(outFileName); //print the file after making it to display processed data
    return 0;
}

//get data for a book record, if its a valid record return true, else return false
bool processRec(BookRec &buffer, ifstream &inFile, unsigned int &saveIsbn, int line, long isbn)
{
    //auto set validData and valid ISBN to true
    bool validData = true;
    bool validIsbn = true;

    //skip past delimiter after isbn
    char delim;
    inFile >> delim;

    if(isbn <= 0)   //if the isbn is invalid then set the data and isbn to invalid and print error message
    {
        cerr << ">error invalid isbn on line " << line << endl;
        validIsbn = false;
        validData = false;
    }
    else    //else get the isbn as normal
    {
        buffer.isbn = isbn;
        if(buffer.isbn <= saveIsbn) //make sure isbn is in sequecne
        {
            cerr << ">error: isbn out of sequence on line " << line << endl;
            validData = false;
        }
        else
            saveIsbn = buffer.isbn;
    }
    //get name
    inFile.getline(buffer.name,25,'|');

    //get author
    inFile.getline(buffer.author,25,'|');

    //get onhand
    //getline(inFile,onhand,'|');
    inFile >> buffer.onhand;
    inFile >> delim;
    //if(onhand[0] == '-')    //if the onhand value is negative print error and set data to invalid
    if(buffer.onhand < 0)
    {
        cerr << ">error: negative onhand value on line " << line << endl;
        validData = false;
    }
    //buffer.onhand = stoi(onhand);
    //get price

    inFile >> buffer.price;
    inFile >> delim;
    if(buffer.price < 0) //if price is negative print error and set data to invalid
    {
        cerr << ">error: price is less than 0 on line " << line << endl;
        validData = false;
    }
    //buffer.price = stof(price);
    //get type
    inFile.getline(buffer.type,25);
    if(!validData && validIsbn) //if the data is invalid but the isbn is valid print the book so the user sees which line is ommitted
    {
        printRecord(buffer);
    }
    return validData;
}

//method to print a record
void printRecord(BookRec &buffer)
{
    cout<<setprecision(2)<<fixed<<setw(10)<<setfill('0')<<buffer.isbn
	    <<setw(25)<<setfill(' ')<<buffer.name
	    <<setw(25)<<buffer.author
	    <<setw(3)<<buffer.onhand
	    <<setw(6)<<buffer.price
	    <<setw(10)<<buffer.type<<endl;
}

//method to print a whole file
void printFile(string outFileName)
{
    BookRec buffer;
    fstream in(outFileName.c_str(), ios::in | ios::binary);

    cout<<"Content of "<<outFileName<<":"<<endl;
	for (int i=0; i<80; i++) cout<<'^'; cout<<endl;

	while (in.read((char *) &buffer, sizeof(BookRec) ) )
	{
	  printRecord(buffer);
	}

	for (int i=0; i<80; i++) cout<<'^'; cout<<endl; 
    cout<<endl;
}