#ifndef WALLE_FILESYSTEM_H_
#define WALLE_FILESYSTEM_H_

#include <cstdio>
#include <string>
#include <vector>
#include <fcntl.h>
#include <sys/stat.h>

using namespace std;

namespace walle {
namespace sys {

class Filesystem {
    public:
        static bool fileExist( const string &file );

        static bool isLink( const string &file );

        static bool isDir( const string &file );

        static bool mkLink( const string &srcfile, const string &destfile );

        static bool link(const string &srcfile, const string &destfile);

        static size_t fileSize( const string &file );


        static time_t fileTime( const string &file );

        static string filePath( const string &file );


        static string fileName( const string &file );

        static bool renameFile( const string &oldname, const string &newname );


        static bool copyFile( const string &srcfile, const string &destfile );


        static bool deleteFile( const string &file );


        static bool moveFile( const string &srcfile, const string &destfile );

        static vector<string> dirFiles( const string &dir );

        static bool makeDir( const string &dir, 
	            const mode_t mode = S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH );


        static bool copyDir( const string &srcdir, const string &destdir );

        static bool deleteDir( const string &dir );


        static bool moveDir( const string &srcdir, const string &destdir );
		  
}; 
}
}
#endif
