/*
 * fileio.cpp
 *
 * This file provides file/ directory access to other modules or layers.
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

RC FileIO::CreateFile  (const std::string &fileName)
{
  RC rc;
  // If the file already exists, error
  if ((rc = FileExists(fileName)))
    return FILE_EXISTS;

  // Attempt to open the file for writing
  FILE *pFile = fopen(fileName.c_str(), "wb");
  if (pFile == NULL)
    return CREATE_FILE_ERROR;

  fclose(pFile);
  return SUCCESS;
}

RC FileIO::CreateDir   (const std::string &dirName)
{
  // Create the directory
	RC rc = mkdir(dirName.c_str(), 777);
  if (rc == SUCCESS) {
    return SUCCESS;
  }
  return CREATE_DIR_ERROR;
}

RC FileIO::DestroyFile (const std::string &fileName)
{
  // If file cannot be successfully removed, error
  if (remove(fileName.c_str()) != SUCCESS)
    return DESTROY_FILE_ERROR;
  return SUCCESS;
}

RC FileIO::DestroyDir  (const std::string &dirName)
{
  // Remove the directory recursively
  RC rc = rmdir(dirName.c_str());
  if (rc == SUCCESS) {
    return SUCCESS;
  }
  return DESTROY_DIR_ERROR;
}

RC FileIO::OpenFile    (const std::string &fileName)
{
  // If this class already has an open file, error
  if (Getfd() != NULL)
    return FILE_DESCIPTOR_IN_USE;

  // If the file doesn't exist, error
  if (!FileExists(fileName.c_str()))
    return FILE_NOT_EXISTS;

  // Open the file for reading/writing in binary mode
  FILE *pFile;
  pFile = fopen(fileName.c_str(), "rb+");
  // If we fail, error
  if (pFile == NULL)
    return OPEN_ERROR;

  Setfd(pFile);

  return SUCCESS;
}

RC FileIO::CloseFile   ()
{
  FILE *pFile = Getfd();

  // If not an open file, ignore
  if (pFile == NULL)
    return SUCCESS;

  // Flush and close the file
  fclose(pFile);

  Setfd(NULL);

  return SUCCESS;
}

RC FileIO::ResetFile   (const std::string &fileName)
{
  RC rc;
  if ((rc = DestroyFile(fileName)))
    return rc;

  rc = CreateFile(fileName);
    return rc;
}

RC FileIO::ResetDir    (const std::string &dirName)
{
  RC rc;
  if ((rc = DestroyDir(dirName)))
    return rc;

  rc = CreateDir(dirName);
  return rc;
}

RC FileIO::WriteFile   (size_t offset, size_t length, const void *data)
{
  if (_fd == NULL)
    return FILE_DESCIPTOR_NOT_EXISTS;

  // Seek to the start position in file
  if (fseek(_fd, offset, SEEK_SET))
    return FH_SEEK_FAILED;

  // Write the data
  if (fwrite(data, ONE_BYTE, length, _fd) == length)
  {
    // Immediately commit changes to disk
    fflush(_fd);
    return SUCCESS;
  }

  return WRITE_ERROR;
}

RC FileIO::ReadFile    (size_t offset, size_t length, void *data)
{
  if (_fd == NULL)
    return FILE_DESCIPTOR_NOT_EXISTS;

  // Try to seek to the starting position in the file
  if (fseek(_fd, offset, SEEK_SET))
    return FH_SEEK_FAILED;

  // Try to read the specified page
  if (fread(data, ONE_BYTE, length, _fd) != length)
    return READ_ERROR;

  return SUCCESS;
}

RC FileIO::AppendFile  (size_t length, const void *data)
{
  if (_fd == NULL)
    return FILE_DESCIPTOR_NOT_EXISTS;

  // Seek to the start position in file
  if (fseek(_fd, ZERO, SEEK_END))
    return FH_SEEK_FAILED;

  // Write the data
  if (fwrite(data, ONE_BYTE, length, _fd) == length)
  {
    // Immediately commit changes to disk
    fflush(_fd);
    return SUCCESS;
  }

  return APPEND_ERROR;
}

unsigned FileIO::GetFileSize ()
{
  if (_fd == NULL)
    return ZERO;

  // Obatin the size of the file
  fpos_t pos;
  fgetpos(_fd, &pos);
  fseek(_fd, ZERO, SEEK_END);
  unsigned size = ftell(_fd);
  fsetpos(_fd, &pos);
  return size;
}

bool FileIO::FileExists  (const std::string &fileName)
{
  // If stat fails, we can safely assume the file doesn't exist
  struct stat file_status;
  return stat(fileName.c_str(), &file_status) == SUCCESS;
}
