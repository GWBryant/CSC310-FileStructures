#include "album.h"

/***********constructors***********/
Album::Album(const Album& otherAlbum)
{
    strcpy(UPC,otherAlbum.UPC);
    strcpy(Artist,otherAlbum.Artist);
    strcpy(Title,otherAlbum.Title);
}

Album::Album(String upc, String artist, String title)
{
    strcpy(UPC,upc);
    strcpy(Artist,artist);
    strcpy(Title,title);
}

Album::Album()
{
    strcpy(UPC,"\0");
    strcpy(Artist,"\0");
    strcpy(Title,"\0");
}

Album & Album::operator = (const Album& otherAlbum)
{
    strcpy(UPC,otherAlbum.UPC);
    strcpy(Artist,otherAlbum.Artist);
    strcpy(Title,otherAlbum.Title);
    return *this;
}

/************************************/

bool operator < (const Album& a, const Album& b)
{
    //return stoi(a.UPC) < stoi(b.UPC);
    return (strcmp(a.UPC,b.UPC) < 0);
}

istream & operator >> (istream & stream, Album & C)
{
    string upc, artist, title;
    getline(stream,upc);
    getline(stream,artist);
    getline(stream,title);
    strcpy(C.UPC,upc.c_str());
    strcpy(C.Artist,artist.c_str());
    strcpy(C.Title,title.c_str());
    return stream;
}

ostream & operator << (ostream & stream, Album & C)
{
    return stream << C.UPC << "|" << C.Artist << "|" << C.Title;
}

string Album::getUPC()
{
    return UPC;
}

int Album::recordSize()
{
    return strlen(UPC) + strlen(Artist) + strlen(Title);
}