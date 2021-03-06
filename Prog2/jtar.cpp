#include <iostream>
#include "file.h"
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <utime.h>
#include <dirent.h>
#include <vector>
#include <filesystem>

using namespace std;
using recursive_directory_iterator = filesystem::recursive_directory_iterator;

int parseArgs(char **argv, int argc);
bool isDir(string fileName);
void create(char **files, int n);
void printFiles(string fileName);
void uncompress(string fileName);
string getSize(string fileName);
string getProt(string fileName);
string getStamp(string fileName);

int main(int argc, char **argv)
{
    //get what option the use has chosen by prasing the command line arguments
    int option = parseArgs(argv,argc);
    //if the command line input was invalid return an error and prompt the user to use --help
    if(option == 0)
    {
        cerr << "jtar: Invalid format" << endl;
        cerr << "try 'jtar --help' for more information." << endl;
    }
    //if the user used -cf call the requisite function
    else if(option == 1)
    {
        create(argv,argc);
    }
    //if the user used -tf call the requisite function
    else if(option == 2)
    {
        printFiles(argv[2]);
    }
    //if the user used -xf call the requisite function
    else if(option == 3)
    {
        uncompress(argv[2]);
    }
    //else the user called the help function so we exit
    else
        return 0;
    return 0; 
}

//parse command line arguments and return an int that indicated task to perform
int parseArgs(char *argv[], int argc)
{
    //check if there are enough command line args
    if(argc > 1)
    {
        //if they call -cf check if the number of args are correct
        if(strcmp(argv[1],"-cf") == 0)
        {
            if(argc < 4)
                return 0;
            else
                return 1;
        }
        //check for correct number of args
        else if(strcmp(argv[1],"-tf") == 0)
        {
            if(argc < 3)
                return 0;
            else
                return 2;
        }
        //check for correct number of args
        else if(strcmp(argv[1],"-xf") == 0)
        {      
            if(argc < 3)
                return 0;
            else
                return 3;
        }
        //check for correct number of args
        else if(strcmp(argv[1],"--help") == 0)
        {
            system("cat help");
            return 4;
        }
        else
            return 0;
    }
    else
        return 0;
}

//return whether a file is a directory or not
bool isDir(string name)
{
    struct stat buf;
    lstat (name.c_str(), &buf);
    return S_ISDIR(buf.st_mode);
}

//return the size of a file
string getSize(string fileName)
{
    struct stat buf;
    lstat(fileName.c_str(), &buf);
    return to_string(buf.st_size);
}

//return protections of a file
string getProt(string fileName)
{
    struct stat buf;
    lstat(fileName.c_str(),&buf);
    string protection = to_string((buf.st_mode & S_IRWXU) >> 6) 
                      + to_string(((buf.st_mode & S_IRWXG) >> 3))
                      + to_string(buf.st_mode & S_IRWXO);
    return protection;
}

//return the timestamp of a file
string getStamp(string fileName)
{
    struct stat buf;
    struct utimbuf timebuf;
    char stamp[16];
    lstat(fileName.c_str(),&buf);
    strftime(stamp,16,"%Y%m%d%H%M.%S", localtime(&buf.st_mtime));
    return string(stamp);
}

//create a tar file
void create(char **files, int n)
{
    //vector of files
    vector<File> fs;

    //start from 3rd command line arg, where files start and loop through them
    for(int i = 3; i < n; i++)
    {
        //create a file object for each file passed in
        string name = files[i];
        //check if the file exists
        fstream checkFile(name,ios::in);
        if(checkFile)
        {
            File f(name.c_str(),getProt(name).c_str(),getSize(name).c_str(),getStamp(name).c_str());
            //if it is a directory flag it, push it, and get its files and subdirectories
            if(isDir(name))
            {
                f.flagAsDir();
                fs.push_back(f);

                //recursive loop to get all files and subdirectories in a directory
                for(auto& p: recursive_directory_iterator(name))
                {
                    File subf(p.path().c_str(),getProt(p.path()).c_str(),getSize(p.path()).c_str(),getStamp(p.path()).c_str());
                    if(isDir(p.path()))
                        subf.flagAsDir();
                    fs.push_back(subf);
                }
            }
            else
                fs.push_back(f);
        }
        checkFile.close();
    }
    
    //open your tar file to utput to
    fstream tarFile(files[2],ios::out|ios::binary);

    //write out the number of files to the file first
    int s = fs.size();
    tarFile.write((char *) &s,sizeof(int));

    //loop through the vector and write the files out to the tar file
    for(int i = 0; i < fs.size(); i++)
    {   
        //write the file object
        tarFile.write((char *) &fs[i], sizeof(fs[i]));
        //if it isn't a directory write out the files contents
        if(!fs[i].isADir())
        {
            //open the file and get its size into an int
            int fileSize = stoi(fs[i].getSize());
            fstream in(fs[i].getName(),ios::in);

            //create a character array the size of the file and read the file into it
            char *c = new char[fileSize];
            in.read(c,fileSize);
            in.close();

            //write out the contents of the file to the tar file
            tarFile.write(c, fileSize);
        }
    }
    tarFile.close();
}

//print out file names
void printFiles(string fileName)
{
    //open the tar file
    fstream inFile(fileName,ios::in|ios::binary);

    //declare necessary variables
    int n;
    File f;

    //grab the number of records in the file from the beginning of the file
    inFile.read((char *) &n, sizeof(int));

    //loop through the file
    for(int i = 0; i < n; i++)
    {
        //read in each file object and print it
        inFile.read((char *) &f, sizeof(File));
        cout << f.getName() << endl;

        //if the file isn't a directory skip the file data inside the tar file to get to the next fileName
        if(!f.isADir())
            inFile.seekg(stoi(f.getSize()),ios::cur);
    }
}

//uncompress that tar file
void uncompress(string fileName)
{
    //open tar file
    fstream inFile(fileName,ios::in|ios::binary);

    //create necessary varialbes
    int n;
    File f;

    //read in number of files in tar file
    inFile.read((char *) &n, sizeof(int));

    //loop through tar file
    for(int i = 0; i < n; i++)
    {
        //read in file objects
        inFile.read((char *) &f, sizeof(File));

        //if the file is a directory make that directory, unless the directory already exists
        if(f.isADir())
        {
            if(!isDir(f.getName()))
                system(("mkdir " + f.getName()).c_str());
        }
        //else create the file and fill it with the file contents
        else
        {
            fstream outFile(f.getName(),ios::out);
            int fileSize = stoi(f.getSize());
            char *c = new char[fileSize];
            inFile.read(c, fileSize);
            outFile.write(c,fileSize);
            outFile.close();
        }
        //change permissions and timestamp of file
        system(("touch -t " + f.getStamp() + " " + f.getName()).c_str());
        system(("chmod " + f.getPmode() + " " + f.getName()).c_str());
    }
    inFile.close();
}