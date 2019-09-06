#ifndef FILE_IO
#define FILE_IO

/* ----- Include libries or files ----- */
// #include <cstio>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
// #include "../../util/systemLog.h"
#include "../../util/emailError.h"
#include "../../util/util.h"

/* ----- Define macros ----- */
enum {
  CREATE_FILE_ERROR,
  CREATE_DIR_ERROR,
  DESTROY_FILE_ERROR,
  DESTROY_DIR_ERROR,
  OPEN_ERROR,
  WRITE_ERROR,
  READ_ERROR,
  APPEND_ERROR,
  FILE_EXISTS,
  FILE_NOT_EXISTS,
  DIR_EXISTS,
  DIR_NOT_EXISTS,
  DIR_WRONG_PATH,
  FILE_DESCIPTOR_IN_USE,
  FILE_DESCIPTOR_NOT_EXISTS,
  FH_SEEK_FAILED,
};

#define ZERO     0
#define ONE_BYTE 1

/**
 * FileIO
 * This class contains all interfaces that will be used to manage the file.
 *
 * Contained Public Functions:
 *   FileIO* instance ()
 *   RC CreateFile  (const string &fileName)
 *   RC CreateDir   (const string &dirName)
 *   RC DestroyFile (const string &fileName)
 *   RC DestroyDir  (const string &dirName)
 *   RC OpenFile    (const string &fileName)
 *   RC CloseFile   ()
 *   RC ResetFile   (const string &fileName)
 *   RC ResetDir    (const string &dirName)
 *   RC WriteFile   (size_t offset, size_t length, const void *data)
 *   RC ReadFile    (size_t offset, size_t length, void *data)
 *   RC AppendFile  (size_t length, const void *data)
 *   unsigned GetFileSize ()
 */

class FileIO
{
public:
  /**
   * This function will initialize an instance for FileIO.
   * @return pointer of FileIO.
   */
  static FileIO* instance();

  /**
   * This function will create a new empty file with the given filename.
   * @param  const string given as filename (can be a path).
   * @return SUCCESS if the file is successfully created.
   *         CREATE_ERROR if create file not successfully.
   */
  RC CreateFile  (const std::string &fileName);

  /**
   * This function will create a new directory in the data folder.
   * @param  const string given as directory name.
   * @return SUCCESS if the directory is successfully created.
   *         CREATE_ERROR if create directory not successfully.
   */
  RC CreateDir   (const std::string &dirname);

  /**
   * This function will remove a file with the given filename.
   * @param  const string given as filename (can be a path).
   * @return SUCCESS if the file has been removed successfully.
   *         DESTROY_ERROR if failed to remove.
   */
  RC DestroyFile (const std::string &fileName);

  /**
   * This function will remove a directory with the given directory name.
   * @param  const string given as directory name.
   * @return SUCCESS if the directory has been removed successfully.
   *         DESTROY_ERROR if failed to remove.
   */
  RC DestroyDir  (const std::string &dirName);

  /**
   * This function will open a file with the given filename or path to open the
   * file wit read and write accesses and set the file descriptor appropriatly.
   * @param  const string given as the filename.
   * @return SUCCESS if the file is successfully opened.
   *         OPEN_ERROR if file cannot be opened.
   */
  RC OpenFile    (const std::string &fileName);

  /**
   * This function will close the file descriptor _fd.
   * @return SUCCESS if the _fd is successfully closed.
   *         OPEN_ERROR if _fd cannot be closed.
   */
  RC CloseFile   ();

  /**
   * This function will reset the file that already exists with the given filename.
   * This function will first destory that file and then re-create it.
   * @param  const string given as fileName (can be a path).
   * @return SUCCESS if destory and create successfully.
   *         pre-defined error number returned by other functions.
   */
  RC ResetFile   (const std::string &fileName);

  /**
   * This function will reset the directory that already exists with the given name.
   * This function will first destory that directory and then re-create it.
   * @param  const string given as dirctory name.
   * @return SUCCESS if destory and create successfully.
   *         pre-defined error number returned by other functions.
   */
  RC ResetDir    (const std::string &dirName);

  /**
   * This function will append(write) the fixed-length data from the given pointer
   * at the specific position in the specified file.
   * @param  size_t offset indicates the starting position in the file.
   *         size_t length indicates how long need to be read.
   *         void * indicates the pointer that need to store the data.
   * @return SUCCESS if write successfully.
   *         WRITE_ERROR or other pre-defined error if failed to write.
   */
  RC WriteFile  (size_t offset, size_t length, const void *data);

  /**
   * This function will read the fixed-length data from speicified file and copy
   * that data to given char pointer.
   * @param  size_t offset indicates the starting position in the file.
   *         size_t length indicates how long need to be read.
   *         void * indicates the pointer that need to store the data.
   * @return SUCCESS if read successfully.
   *         READ_ERROR or other pre-defined error if failed to read.
   */
  RC ReadFile    (size_t offset, size_t length, void *data);

  /**
   * This function will append(write) the fixed-length data from the given pointer
   * at the end of the speicified file.
   * @param  size_t length indicates how long need to be append.
   *         void * indicates the pointer that stores the data need to be stored.
   * @return SUCCESS if append successfully.
   *         APPEND_ERROR or other pre-defined error if failed to append.
   */
  RC AppendFile  (size_t length, const void *data);

  /**
   * This function will return the size of the file in bytes.
   * @return unsigned as the size of the file.
   */
  unsigned GetFileSize ();

protected:
  FileIO()  {};   // Constructor
  ~FileIO() {};   // Destructor

private:
  FILE *_fd;                               // File descriptor, from <cstio>
  static FileIO *_file_io;                 // Pointer of this class

  // Private helper function
  void Setfd(FILE *fd) { _fd = fd;   };    // Set the current file descriptor
  FILE *Getfd()        { return _fd; };    // Get current file descriptor
  bool FileExists(const std::string &fileName); // Check if the file exists
};

#endif
