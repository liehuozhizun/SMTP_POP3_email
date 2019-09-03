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

/* ----- Define structs ----- */
struct ClientUserInfo {
  string username;
  string domainName;
  time_t lastLoginTime;
  time_t lastLogoutTime;
};

struct ServerUserInfo {
  string username;
  string domainName;
  string password;
  time_t lastLoginTime;
  time_t lastLogoutTime;
  time_t changeTimestamp;
};

/**
 * UIM (User Info Manager, client side)
 * This class contains all interfaces that will be used to manage the user info.
 *
 * Contained Public Functions:
 *   UIM* instance ()
 *   RC CheckExist (const string &userAccount)
 *   RC CreateUser (const ServerUserInfo &userInfo)
 *   RC CloseUser  (const string &userAccount)
 *   RC ReadUser   (const string &userAccount, ClientUserInfo &userInfo)
 *   RC UpdateUser (const ServerUserInfo &userInfo)
 *   RC SetupUser  (const string &userAccount)
 *   RC Login      (const ServerUserInfo &userInfo)
 *   RC Logout     (const string &userAccount)
 *
 * Private Functions:
 *   RC UserAccountGenerator (const ServerUserInfo &userInfo, string &userAccount)
 *   RC UserAccountGenerator (const ClientUserInfo &userInfo, string &userAccount)
 *   RC ClientUserInfoGenerator (const string &userAccount, ClientUserInfo &userInfo)
 *   RC ServerUserInfoGenerator (const string &userAccount, ServerUserInfo &userInfo)
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
   * This function will check if the given user account exists in server's database.
   * @param string as the given username@domain. E.g., admin@sample.com.
   * @return SUCCESS if the username exists in the server's database.
   *         USER_EXISTS if the user exists in server's database.
   *         STANDARD_ERROR otherwise.
   */
  RC CheckExist (const string &userAccount);

  /**
   * This function will create a new user account both locally and remotely.
   * @param ServerUserInfo contains all the information needed to create a user.
   * @return SUCCESS if create a new user successfully.
   *         CREATE_USER_ERROR if fail to create.
   *         STANDARD_ERROR otherwise.
   */
  RC CreateUser (const ServerUserInfo &userinfo);

  /**
   * This function close a user's account by removing all information of this user
   * locally and remotely.
   * @param string as the given username@domain. E.g., admin@sample.com.
   * @return SUCCESS if the user has been successfully removed.
   *         USER_NOT_EXISTS if the user doesn't exist in server's database.
   *         STANDARD_ERROR otherwise.
   */
  RC CloseUser  (const string &userAccount);

  /**
   * This function read user info from the user.sys file.
   * @param string as the given username@domain. E.g., admin@sample.com.
   *        ClientUserInfo to store the returned user information.
   * @return SUCCESS if the user info is successfully read.
   *         USER_NOT_EXISTS if the user doesn't exist.
   *         READ_USER_ERROR otherwise.
   */
  RC ReadUser   (const string &userAccount, ClientUserInfo &userInfo);

  /**
   * This function will update the user's information locally and remotely.
   * @param ServerUserInfo contains all information needed to be updated.
   * @return SUCCESS if update successfully.
   *         STANDARD_ERROR otherwise.
   */
  RC UpdateUser (const ServerUserInfo &userInfo);

  /**
   * This function will try to obtain the user's information from the server's
   * database. If the user's info system file doesn't exist, create it first.
   * @param string as the given username@domain. E.g., admin@sample.com.
   * @return SUCCESS if the user has been successfully setup.
   *         STANDARD_ERROR otherwise.
   */
  RC SetupUser  (const string &userAccount);

  /**
   * This function will login the user with given password and verify it with the
   * help of server's interface. Login will SetupUser.
   * @param ServerUserInfo contains all the information needed to login the account.
   * @return SUCCESS if the user has successfullt logined and been set up.
   *         PASSWORD_ERROR if the password is mismatched with the record in server.
   *         USER_NOT_EXISTS if the user doesn't exist in server.
   *         STANDARD_ERROR otherwise.
   */
  RC Login      (const ServerUserInfo &userInfo);

  /**
   * This function will logout your account from the current system and send your
   * logout action to server.
   * @param string as the given username@domain. E.g., admin@sample.com.
   * @return SUCCESS if logout successfully.
   *         STANDARD_ERROR otherwise.
   */
  RC Logout     (const string &userAccount);

protected:
  UIM() {};    // Constructor
  ~UIM() {};   // Destructor

private:
  static UIM *_uim;   // Pointer of this class

  // Private helper functions
  /**
   * These functions will translate the UserInfo to a userAccount.
   * @param ServerUserInfo/ClientUserInfo contain the given information.
   *        string to store the userAccount.
   * @return SUCCESS if translate successfully.
   *         STANDARD_ERROR otherwise.
   */
  RC UserAccountGenerator_S (const ServerUserInfo &userInfo, string &userAccount);
  RC UserAccountGenerator_C (const ClientUserInfo &userInfo, string &userAccount);

  /**
   * These functions will generate a half-empty UserInfo with given userAccount
   * @param string as the given username@domain. E.g., admin@sample.com.
   *        ServerUserInfo/ClientUserInfo contain the given information.
   * @return SUCCESS if translate successfully
   *         STANDARD_ERROR otherwise;
   */
  RC ClientUserInfoGenerator (const string &userAccount, ClientUserInfo &userInfo);
  RC ServerUserInfoGenerator (const string &userAccount, ServerUserInfo &userInfo);

  /**
   * These function will translate UserInfo from the other type
   * @param ServerUserInfo/ClientUserInfo contains the given information.
   *        ClientUserInfo/ServerUserInfo to store translation.
   * @return SUCCESS if translate successfully
   *         STANDARD_ERROR otherwise;
   */
  RC UserInfo_StoC (const ServerUserInfo &userInfo, ClientUserInfo & C_userInfo);
  RC UserInfo_CtoS (const ClientUserInfo &userInfo, ServerUserInfo & S_userInfo);
};

#endif
