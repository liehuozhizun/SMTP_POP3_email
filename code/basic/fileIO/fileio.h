#ifndef FILE_IO
#define FILE_IO

#define SUCCESS 0

#include <cstio>
#include <string>
#include <climits>
using namespace std;

/**
 * FileIO
 * This class contains all interfaces that will be used to manage the file.
 *
 * Contained Public Functions:
 *   RC Init        ()
 *   RC ReInit      ()
 *   RC CreateFile  (const string &fileName)
 *   RC DestroyFile (const string &fileName)
 *   RC OpenFile    (const string &fileName)
 *   RC CloseFile   (const string &fileName)
 */

class FileIO
{
public:
  /**
   * These function transitions between rooms. Each call should return SUCCESS if the current room has
   * an exit in the correct direction and the new room was able to be loaded, and STANDARD_ERROR
   * otherwise.
   * @param title A character array to copy the room title into. Should be GAME_MAX_ROOM_TITLE_LENGTH+1
   *             in length in order to allow for all possible titles to be copied into it.
   * @return SUCCESS if the room CAN be navigated to and changing the current room to that new room
   *         succeeded.
   */
  static FileIO* instance();

  /**
   * This function check the current files stored in data directory to see if all
   * the required fundemental functions exists and are available to read/write.
   * If any file are missing, this function will create the missing one. If any
   * file is unaccessible, this function will throw and error to the caller.
   * @param  none
   * @return SUCCESS if every fundemental file exists and accessible
   *         STANDARD_ERROR if any fundemental file is lost or unaccessible
   */ 
  RC Init ();

  /**
   * These function transitions between rooms. Each call should return SUCCESS if the current room has
   * an exit in the correct direction and the new room was able to be loaded, and STANDARD_ERROR
   * otherwise.
   * @return SUCCESS if the room CAN be navigated to and changing the current room to that new room
   *         succeeded.
   */
  RC CreateFile  (const string &fileName);

  /**
   * These function transitions between rooms. Each call should return SUCCESS if the current room has
   * an exit in the correct direction and the new room was able to be loaded, and STANDARD_ERROR
   * otherwise.
   * @return SUCCESS if the room CAN be navigated to and changing the current room to that new room
   *         succeeded.
   */
  RC DestroyFile (const string &fileName);
  RC OpenFile    (const string &fileName);
  RC CloseFile   (const string &fileName);

protected:
  FileIO() {};   // Constructor
  ~FileIO() {};  // Destructor

private:
  FILE *_fd;                              // File descriptor, from <cstio>

  void _setfd(FILE *fd) { _fd = fd;   };  // Set the current file descriptor
  FILE *_getfd()        { return _fd; };  // Get current file descriptor
}

#endif
