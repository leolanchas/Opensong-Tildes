/* 
 * File:   main.cpp
 * Author: LANCHAS
 *
 * Created on 30 de octubre de 2011, 21:19
 */

#include <fstream>
#include <iostream>
#include <string>
#include <map>

#include <cstdlib>

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <windows.h>

using namespace std;

/*
 *
 */

#define MAXPATHLEN 200

int main()
{
    string dir, filepath;
    char * dirbuf = new char[ MAXPATHLEN ]; dirbuf[ 0 ] = 0;
    DIR *dp = NULL;
    struct dirent *dirp;
    struct stat filestat;

    map <int, string> dictionary;

    dictionary[-31] = "a"; // á
    dictionary[-63] = "A"; // Á
    dictionary[-23] = "e"; // é
    dictionary[-55] = "E"; // É
    dictionary[-24] = "e"; // è
    dictionary[-56] = "E"; // È
    dictionary[-19] = "i"; // í
    dictionary[-51] = "I"; // Í
    dictionary[-13] = "o"; // ó
    dictionary[-45] = "O"; // Ó
    dictionary[-6 ] = "u"; // ú
    dictionary[-38] = "U"; // Ú
    dictionary[-15] = "n"; // ñ
    dictionary[-47] = "N"; // Ñ

    // IMPORTANT: IF WRONG DOES NOT WORK
    // getcwd (dirbuf, MAXPATHLEN); // UNIX
    GetCurrentDirectory( MAXPATHLEN - 1, dirbuf );  // where are we working?

    dp = opendir( dirbuf ); // open current directory

    if (dp == NULL) exit(0);

    dir = string(dirbuf);
    delete dirbuf;

    string newdir = dir + "\\acentos"; // modified files

    mkdir(newdir.c_str(), 0777 );
    //CreateDirectory(newdir.c_str(), NULL ); // windows

    cout << "DIRECTORIO CREADO: " << newdir << endl;

    while ((dirp = readdir( dp )))
    {
        filepath = dir + "\\" + dirp->d_name;

        // If the file is a directory (or is in some way invalid) we'll skip it
        if (stat( filepath.c_str(), &filestat )) continue;
        if (S_ISDIR( filestat.st_mode ))         continue;

        // procesar tildes
        string letter;
        bool mod = false; // filename modified?
        unsigned int i = 0; // count

        while( i < 256 && dirp->d_name[ i ] != 0 )
        {
            letter = dirp->d_name[i];

            if ( dictionary.find( letter[ 0 ] ) != dictionary.end() )
            {
                dirp->d_name[ i ] = dictionary[ letter[ 0 ] ][ 0 ];
                mod = true;
            }
            i++;
        }
        
        if ( mod )
        {
            string oldname = filepath;
            string newname = dir + "\\" + string( dirp->d_name );
            rename ( oldname.c_str(), newname.c_str() );
            cout << "RENOMBRADO : " << oldname << " A " << newname << endl;

            oldname = newdir + "\\" + string(dirp->d_name);
            CopyFile( newname.c_str(), oldname.c_str(), false);
            cout << "COPIADO: " << newname << " A " << oldname << endl << endl;
        }
    }

    closedir( dp );

    return 0;
}

