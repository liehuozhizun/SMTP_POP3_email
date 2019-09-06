/*
 * uim.cpp
 *
 * This file provides System Manager to manage the user information.
 *
 * Author(s): Hang Yuan (hyuan211@gmail.com)
 * Tester(s): -
 *
 */

#include "uim.h"

UserInfoManager* UserInfoManager::_uim = NULL;
FileIO* UserInfoManager::_fio = NULL;

UserInfoManager::UserInfoManager()
{
  // Initialize the internal FileIO instance
  _fio = FileIO::instance();
}

UserInfoManager* UserInfoManager::instance()
{
  if(!_uim)
    _uim = new UserInfoManager();
  return _uim;
}

RC UserInfoManager::CreateUser (const UserInfo &userInfo)
{
  RC rc;

  // Get in the user file
  rc = SetUserFilePath(userInfo);
  if (rc)
    return STANDARD_ERROR;

  // Check out the user existence
  unsigned offset = ObatinUserOffset(userInfo);
  if (offset) // true(not ZERO) means the user already exists
    return USER_EXISTS;

  // Add user into the user database
  offset = CheckEmptySpace();
  if (offset) { // true(not ZERO) means there is availiable space to store
    _fio->WriteFile(offset, sizeof(UserInfo), &userInfo);
  } else { // false(ZERO) means need to append the user info
    _fio->AppendFile(sizeof(UserInfo), &userInfo);
  }

  // Increase the header by ONE
  ++_totalUserNumber;
  SetUserNumber();

  return SUCCESS;
}

RC UserInfoManager::CloseUser  (const UserInfo &userInfo)
{
  RC rc;

  rc = SetUserFilePath(userInfo);
  if (rc)
    return STANDARD_ERROR;

  unsigned offset = ObatinUserOffset(userInfo);
  if (offset) { // true(not ZERO) means found the user
    // Move the following user info towards ahead to cover the one need to be closed
    unsigned moveBlockSize = sizeof(UserInfoHeader) + (_totalUserNumber - ONE) * sizeof(UserInfo);
    void* moveBlock = malloc(moveBlockSize);
    rc = _fio->ReadFile(offset + sizeof(UserInfo), moveBlockSize, moveBlock);
    if (rc) {
      free(moveBlock);
      return STANDARD_ERROR;
    }
    rc = _fio->WriteFile(offset, moveBlockSize, moveBlock);
    if (rc) {
      free(moveBlock);
      return STANDARD_ERROR;
    }
    free(moveBlock);

    // Increase the totalUserNumber in the header
    ++_totalUserNumber;
    SetUserNumber();
  } else { // false(ZERO) means the user not found
    return USER_NOT_EXISTS;
  }

  return SUCCESS;
}

RC UserInfoManager::UpdateUser (const UserInfo &userInfo)
{
  RC rc;

  rc = SetUserFilePath(userInfo);
  if (rc)
    return STANDARD_ERROR;

  unsigned offset = ObatinUserOffset(userInfo);
  if (offset) { // true(not ZERO) means found the user
    _fio->WriteFile(offset, sizeof(UserInfo), &userInfo);
  } else { // false(ZERO) means the user not found
    return USER_NOT_EXISTS;
  }

  return SUCCESS;
}

RC UserInfoManager::ReadUser   (UserInfo &userInfo)
{
  RC rc;

  rc = SetUserFilePath(userInfo);
  if (rc)
    return STANDARD_ERROR;

  unsigned offset = ObatinUserOffset(userInfo);
  if (offset) { // true(not ZERO) means found the user
    rc = _fio->ReadFile(offset, sizeof(UserInfo), &userInfo);
  } else { // false(ZERO) means the user not found
    return USER_NOT_EXISTS;
  }

  return SUCCESS;
}

RC UserInfoManager::Login      (const UserInfo &userInfo)
{
  RC rc;

  rc = SetUserFilePath(userInfo);
  if (rc)
    return STANDARD_ERROR;

  unsigned offset = ObatinUserOffset(userInfo);
  if (offset) { // true(not ZERO) means found the user
    // Read the stored UserInfo
    UserInfo compare_userInfo;
    _fio->ReadFile(offset, sizeof(UserInfo), &compare_userInfo);

    // Compare the password field to verify the identity
    if (strcmp(compare_userInfo.password, userInfo.password) != ZERO)
      return STANDARD_ERROR;

    // Update the lastLoginTime
    time_t timer;
    time(&timer);
    compare_userInfo.lastLoginTime = timer;
    rc = _fio->WriteFile(offset, sizeof(UserInfo), &compare_userInfo);
    if (rc)
      return STANDARD_ERROR;
  } else { // false(ZERO) means the user not found
    return USER_NOT_EXISTS;
  }

  return SUCCESS;
}

RC UserInfoManager::Logout     (const UserInfo &userInfo)
{
  RC rc;

  rc = SetUserFilePath(userInfo);
  if (rc)
    return STANDARD_ERROR;

  unsigned offset = ObatinUserOffset(userInfo);
  if (offset) { // true(not ZERO) means found the user
    // Update the lastLogoutTime
    UserInfo temp_userInfo;
    _fio->ReadFile(offset, sizeof(UserInfo), &temp_userInfo);
    time_t timer;
    time(&timer);
    temp_userInfo.lastLogoutTime = timer;
    _fio->WriteFile(offset, sizeof(UserInfo), &temp_userInfo);
  } else { // false(ZERO) means the user not found
    return USER_NOT_EXISTS;
  }

  return SUCCESS;
}

/************ Helper Functions *************/
void UserInfoManager::GetUserNumber ()
{
  UserInfoHeader header;
  _fio->ReadFile(ZERO, sizeof(UserInfoHeader), &header);
  _totalUserNumber = header.totalUserNumber;
}

void UserInfoManager::SetUserNumber ()
{
  UserInfoHeader header;
  _fio->ReadFile(ZERO, sizeof(UserInfoHeader), &header);
  header.totalUserNumber = _totalUserNumber;
  _fio->WriteFile(ZERO, sizeof(UserInfoHeader), &header);
}

unsigned UserInfoManager::ObatinUserOffset (const UserInfo &userInfo)
{
  unsigned offset = sizeof(UserInfoHeader);
  UserInfo compare_userInfo;

  // Traverse and compare each username with the given userInfo's username
  for (size_t i = ZERO; i < _totalUserNumber; ++i) {
    _fio->ReadFile(offset, sizeof(UserInfo), &compare_userInfo);
    if (strcmp(userInfo.username, compare_userInfo.username) == 0) {
      return offset;
    }
    offset += sizeof(UserInfo);
  }
  return ZERO;
}

unsigned UserInfoManager::CheckEmptySpace ()
{
  unsigned fileSize = _fio->GetFileSize();
  unsigned usedSpace = sizeof(UserInfoHeader) + sizeof(UserInfo) * _totalUserNumber;
  if ((fileSize - usedSpace) >= sizeof(UserInfo))
    return usedSpace;
  else
    return ZERO;
}

RC UserInfoManager::SetUserFilePath (const UserInfo &userInfo)
{
  RC rc;

  // Generate the file path
  char* dataPath = static_cast<char *>(calloc(USER_FILE_PATH_MAX_LENGTH, sizeof(char)));
  sprintf(dataPath, USER_FILE_PATH_FORMAT, DATAPATH, userInfo.domainName);
  std::string path = std::string(dataPath);
  free(dataPath);

  // Open the file. If the file not exists, create it
  _fio = FileIO::instance();
  if (_fio == NULL)
    return STANDARD_ERROR;
  rc = _fio->OpenFile(path);
  if (rc) { // Means failed to open: no such file. Create it.
    // Create the domainName folder
    std::string dir = std::string(DATAPATH) + std::string(userInfo.domainName);
    rc = _fio->CreateDir(dir);
    if (rc)
      return STANDARD_ERROR;

    // Create the user info file
    rc = _fio->CreateFile(path);
    if (rc)
      return STANDARD_ERROR;

    // Add the UserInfoHeader
    UserInfoHeader header;
    header.totalUserNumber = ZERO;
    _fio->AppendFile(sizeof(UserInfoHeader), &header);
  }

  GetUserNumber();
  return SUCCESS;
}
