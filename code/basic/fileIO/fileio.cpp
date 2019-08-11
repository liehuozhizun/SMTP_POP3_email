/*
 * fileio.cpp
 *
 * This file provides file access to other modules as a module in Basic Layer.
 *
 * Author(s): Hang Yuan (hyuan211@gmail.com)
 * Tester(s): -
 *
 */

#include "fileio.h"

FileIO* FileIO::_file_io = NULL;

FileIO* FileIO::instance()
{
    if(!_file_io)
        _file_io = new FileIO();

    return _file_io;
}


RC FileIO::CreateFile  (const string &fileName)
RC FileIO::CreateDir   (const string &dirName)
RC FileIO::DestroyFile (const string &fileName)
RC FileIO::DestroyDir  (const string &dirName)
RC FileIO::OpenFile    (const string &fileName)
RC FileIO::CloseFile   (const string &fileName)
RC FileIO::ResetFile   (const string &fileName)
RC FileIO::ResetDir    (const string &dirName)
RC FileIO::WriteFile   (const string &fileName, size_t offset, const void *data)
RC FileIO::ReadFile    (const string &fileName, size_t offset, size_t length, void *data)
RC FileIO::AppendFile  (const string &fileName, size_t length, const void *data)
RC FileIO::FileExists  (const string &fileName)
