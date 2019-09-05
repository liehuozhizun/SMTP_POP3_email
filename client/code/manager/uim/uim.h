#ifndef UIM
#define UIM

/* ----- Include libries or files ----- */
// #include <cstio>
#include <string>
#include <time.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include "../../util/systemLog.h"
#include "../../util/emailError.h"
#include "../../basic/fileIO/fileio.h"
#include "../../util/util.h"
using namespace std;

/* ----- Define macros ----- */
enum {
  USER_EXISTS,
  USER_NOT_EXISTS,
  CREATE_USER_ERROR,
  READ_USER_ERROR,
};

#define ZERO     0
#define ONE_BYTE 1
#define USER_ACCOUNT_MAX_LANGTH 30
#define USERNAME_MAX_LANGTH     15
#define DOMAIN_NAME_MAX_LENGTH  15
#define PASSWORD_MAX_LENGTH     16
#define LOG_MESSAGE_MAX_LENGTH  80
#define USER_FILE_PATH "../../../data/user.sys"
#define CREATE_SUCC_LOG_PROMPT "[UIM] create user success, user: %s"
#define CREATE_FAIL_LOG_PROMPT "[UIM] create user fail, user: %s, reason: %s"
#define REMOVE_SUCC_LOG_PROMPT "[UIM] remove user success, user: %s"
#define REMOVE_FAIL_LOG_PROMPT "[UIM] remove user fail, user: %s, reason: %s"
#define READ_SUCC_LOG_PROMPT "[UIM] read user success, user: %s"
#define READ_FAIL_LOG_PROMOT "[UIM] read user fail, user: %s, reason: %s"
#define LOGIN_LOG_PROMOT "[UIM] user login success: %s"
#define LOGOUT_LOG_PROMOT "[UIM] user logout success: %s"

/* ----- Define structs ----- */
struct CientUserInfoHeader {
  unsigned totalUserNumber;
};

struct ClientUserInfo {
  char userAccount[USER_ACCOUNT_MAX_LANGTH];  // Example: user@example.com
};

struct ServerUserInfo {
  char username  [USERNAME_MAX_LANGTH];      // Example: user
  char domainName[DOMAIN_NAME_MAX_LENGTH];   // Example: @example.com
  char password  [PASSWORD_MAX_LENGTH];      // Example: 1a2b3c4d
  time_t lastLoginTime;
  time_t lastLogoutTime;
  bool changeStatus;     // True: changed, False: no change
};

/**
 * UIM (User Info Manager, client side)
 * This class contains all interfaces that will be used to manage the user info.
 *
 * Contained Public Functions:
 *   UIM* instance ()
 *   RC CreateUser (const string &userAccount)
 *   RC RemoveUser (const string &userAccount)
 *   RC ReadUser   (const string &userAccount, ClientUserInfo &userInfo)
 *   RC Login      (const string &userAccount)
 *   RC Logout     (const string &userAccount)
 */

class UIM
{
public:
  /**
   * This function will initialize an instance for UIM
   * @return pointer of UIM
   */
  static UIM* instance();

  /**
   * This function will create a new user account locally. If the user already
   * exists, do nothing.
   * @param ServerUserInfo contains all the information needed to create a user.
   * @return SUCCESS if create a new user successfully or the user already exists.
   *         CREATE_USER_ERROR if fail to create.
   *         STANDARD_ERROR otherwise.
   */
  RC CreateUser (const userAccount, const ClientUserInfo &userinfo);

  /**
   * This function remove a user's account info from the user system.
   * @param string as the given username@domain. E.g., admin@sample.com.
   * @return SUCCESS if the user has been successfully removed.
   *         USER_NOT_EXISTS if the user doesn't exist in server's database.
   *         STANDARD_ERROR otherwise.
   */
  RC RemoveUser (const string &userAccount);

  /**
   * This function read the specific user info given the userAccount.
   * @param string as the given username@domain. E.g., admin@sample.com.
   *        ClientUserInfo to store the user info.
   * @return SUCCESS if the user has been successfully read.
   *         USER_NOT_EXISTS if the user doesn't exist in server's database.
   *         STANDARD_ERROR otherwise.
   */
  RC ReadUser   (const string &userAccount, ClientUserInfo &userInfo);

  /**
   * This function will store the login info into the log file.
   * @param string as the given username@domain. E.g., admin@sample.com.
   * @return SUCCESS if store log successfully.
   *         STANDARD_ERROR otherwise.
   */
  RC Login      (const string &userAccount);

  /**
   * This function will store the logout info into the log file.
   * @param string as the given username@domain. E.g., admin@sample.com.
   * @return SUCCESS if store log successfully.
   *         STANDARD_ERROR otherwise.
   */
  RC Logout     (const string &userAccount);

protected:
  UIM();       // Constructor
  ~UIM() {};   // Destructor

private:
  static UIM *_uim;    // Pointer of this class
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
   * @param string as the given username@domain. E.g., admin@sample.com.
   * @return unsigned as the offset of the user in the user system file.
   *         0(ZERO) if not find.
   */
  unsigned CheckUserExistence (const string &userAccount);

  /**
   * This function will check if there is empty space to add new user.
   * @return unsigned as the availiable position if there is enough space.
   *         0(ZERO) if there is not enough space.
   */
  unsigned CheckEmptySpace ();

};

#endif
