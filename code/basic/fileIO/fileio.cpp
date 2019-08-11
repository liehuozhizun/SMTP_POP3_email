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

RC FileIO::CreateFile  (const string &fileName)
{
  // If the file already exists, error
  if ((rc = FileExists(fileName)))
    return FILE_EXIST;

  // Attempt to open the file for writing
  FILE *pFile = fopen(fileName.c_str(), "wb");
  if (pFile == NULL)
    return CREATE_FILE_ERROR;

  fclose(pFile);
  return SUCCESS;
}

RC FileIO::CreateDir   (const string &dirName)
{
  // Create a path and check if the
  boost::filesystem::path dir(dir_path);

  // Check if the path is a directory
  if (!boost::filesystem::is_directory(dir))
    return DIR_WRONG_PATH;

  // Check if the directory exists
  if (boost::filesystem::exists(dir))
    return DIR_EXISTS;

  // Create the directory
	if(boost::filesystem::create_directories(dir)) {
		return SUCCESS;
	}
  return CREATE_DIR_ERROR;
}

RC FileIO::DestroyFile (const string &fileName)
{
  // If file cannot be successfully removed, error
  if (remove(fileName.c_str()) != 0)
    return DESTROY_FILE_ERROR;
  return SUCCESS;
}

RC FileIO::DestroyDir  (const string &dirName)
{
  boost::filesystem::path dir(dirName);

  // Check if the path is a directory
  if (!boost::filesystem::is_directory(dir))
    return DIR_WRONG_PATH;

  // Check if the directory exists
  if (!boost::filesystem::exists(dir))
    return DIR_DN_EXISTS;

  // Remove the directory recursively
  uintmax_t n = boost::filesystem::remove_all(dir/ dirName);
  if (n == 0)
    return DESTROY_DIR_ERROR;
}

RC FileIO::OpenFile    (const string &fileName)
{
  // If this class already has an open file, error
  if (Getfd() != NULL)
      return FILE_DESCIPTOR_IN_USE;

  // If the file doesn't exist, error
  if (!fileExists(fileName.c_str()))
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

RC FileIO::CloseFile   (const string &fileName)
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

RC FileIO::ResetFile   (const string &fileName)
{
  RC rc;
  if ((rc = DestroyFile(fileName)))
    return rc;

  rc = CreateFile(fileName);
    return rc;
}

RC FileIO::ResetDir    (const string &dirName)
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
  if (fseek(_fd, PAGE_SIZE * pageNum, SEEK_SET))
      return FH_SEEK_FAILED;

  // Try to read the specified page
  if (fread(data, 1, PAGE_SIZE, _fd) != PAGE_SIZE)
      return READ_ERROR;

  readPageCounter++;
  return SUCCESS;
}

RC FileIO::AppendFile  (size_t length, const void *data)
{
  if (_fd == NULL)
      return FILE_DESCIPTOR_NOT_EXISTS;

  // Seek to the start position in file
  if (fseek(_fd, offset, SEEK_END))
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

bool FileIO::FileExists  (const string &fileName)
{
  // If stat fails, we can safely assume the file doesn't exist
  struct stat sb;
  return stat(fileName.c_str(), &sb) == 0;
}
