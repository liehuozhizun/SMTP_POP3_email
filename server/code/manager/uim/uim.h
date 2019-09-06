#ifndef USER_INFO_MANAGER
#define USER_INFO_MANAGER

/* ----- Include libries or files ----- */
#include <cstring>
#include <stdlib.h>
#include <string>
#include <time.h>
#include "../../util/emailError.h"
#include "../../basic/fileIO/fileio.h"
#include "../../util/util.h"

/* ----- Define macros ----- */
enum {
  USER_EXISTS,
  USER_NOT_EXISTS,
};

#define ZERO     0
#define ONE      1
#define ONE_BYTE 1
#define USERNAME_MAX_LANGTH       15
#define DOMAIN_NAME_MAX_LENGTH    15
#define PASSWORD_MAX_LENGTH       16
#define USER_FILE_PATH_MAX_LENGTH 40 // DATAPATH(14)+domainName(15)+(10)+extra(1)
const char USER_FILE_PATH_FORMAT[] = "%s%s/user.data"; // DATAPATH/domainName/user.data

/* ----- Define structs ----- */
struct UserInfoHeader {
  unsigned totalUserNumber;
};

struct UserInfo {
  char username  [USERNAME_MAX_LANGTH];      // Example: user
  char domainName[DOMAIN_NAME_MAX_LENGTH];   // Example: @example.com
  char password  [PASSWORD_MAX_LENGTH];      // Example: 1a2b3c4d
  time_t lastLoginTime;
  time_t lastLogoutTime;
};

/**
 * UserInfoManager
 * This class contains all interfaces that will be used to manage the user info.
 *
 * Contained Public Functions:
 *   UserInfoManager* instance ()
 *   RC CreateUser (const UserInfo &userInfo)
 *   RC CloseUser  (const UserInfo &userInfo)
 *   RC ReadUser   (UserInfo &userInfo)
 *   RC UpdateUser (const UserInfo &userInfo)
 *   RC Login      (const UserInfo &userInfo)
 *   RC Logout     (const UserInfo &userInfo)
 */

class UserInfoManager
{
public:
  /**
   * This function will initialize an instance for UserInfoManager
   * @return pointer of UserInfoManager
   */
  static UserInfoManager* instance();

  /**
   * This function will create a new user account in the database. If the user
   * already exists, return error.
   * @param UserInfo contains all the information needed to create a user.
   * @return SUCCESS if create a new user successfully.
   *         USER_EXISTS if user already exists.
   *         STANDARD_ERROR otherwise.
   */
  RC CreateUser (const UserInfo &userInfo);

  /**
   * This function remove a user's account info from the user database.
   * @param UserInfo contains the username and domain name to close a user.
   * @return SUCCESS if remove the user successfully.
   *         USER_NOT_EXISTS if the user doesn't exist in the database.
   *         STANDARD_ERROR otherwise.
   */
  RC CloseUser  (const UserInfo &userInfo);

  /**
   * This function will update the UserInfo stored in the database.
   * @param UserInfo contains all the info need to be updated.
   * @return SUCCESS if update user info successfully.
   *         USER_NOT_EXISTS if the user doesn't exist in the database.
   *         STANDARD_ERROR otherwise.
   */
  RC UpdateUser (const UserInfo &userInfo);

  /**
   * This function read the specific user info given the userAccount.
   * @param UserInfo indicates which user and stores the user info.
   * @return SUCCESS if the user has been successfully read.
   *         USER_NOT_EXISTS if the user doesn't exist in the database.
   *         STANDARD_ERROR otherwise.
   */
  RC ReadUser   (UserInfo &userInfo);

  /**
   * This function will verify the user pasword with the database and update the
   * lastLoginTime.
   * @param UserInfo indicates which user to verify info.
   * @return SUCCESS if info matches.
   *         USER_NOT_EXISTS if the user doesn't exist in the database.
   *         STANDARD_ERROR if info matches or other cases.
   */
  RC Login      (const UserInfo &userInfo);

  /**
   * This function will update the lastLogoutTime.
   * @param UserInfo indicates which user to logout.
   * @return SUCCESS if update successfully.
   *         USER_NOT_EXISTS if the user doesn't exist in the database.
   *         STANDARD_ERROR otherwise.
   */
  RC Logout     (const UserInfo &userInfo);

protected:
  UserInfoManager();      // Constructor
  ~UserInfoManager() {};  // Destructor

private:
  static UserInfoManager *_uim;    // Pointer of this class
  static FileIO *_fio; // Pointer of FileIO class

  unsigned _totalUserNumber;

  // Private helper functions
  /**
   * This function will read the file header and save the totalUserNumber.
   */
  void GetUserNumber ();

  /**
   * This function will set totalUserNumber back to the user sys file header.
   */
  void SetUserNumber ();

  /**
   * This function will traverse the user system file to look for a userAccount.
   * @param UserInfo indicates the user
   * @return unsigned as the offset of the user in the user system file.
   *         0(ZERO) if not found.
   */
  unsigned ObatinUserOffset (const UserInfo &userInfo);

  /**
   * This function will check if there is empty space to add new user.
   * @return unsigned as the availiable position if there is enough space.
   *         0(ZERO) if there is not enough space.
   */
  unsigned CheckEmptySpace ();

  /**
   * This function will generate the file path and set up the private variable
   * _fio for later use.
   * @param UserInfo to indicate which user.
   * @return SUCCESS if set up the _fio successfully.
   *         STANDARD_ERROR otherwise.
   */
  RC SetUserFilePath (const UserInfo &userInfo);

};

#endif
