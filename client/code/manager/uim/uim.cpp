/*
 * uim.cpp (client side)
 *
 * This file provides System Manager to manage the user information.
 *
 * Author(s): Hang Yuan (hyuan211@gmail.com)
 * Tester(s): -
 *
 */

#include "uim.h"

UIM* UIM::_uim = NULL;
FileIO* FileIO::_fio = NULL;

UIM* UIM::instance()
{
  if(!_uim)
    _uim = new UIM();
  return _uim;
}

UIM::UIM()
{
  // Initialize FilIO interface for user.sys file
  _fio = FileIO::instance();
  if (_fio->OpenFile(USER_FILE_PATH)) {
    _fio->ResetFile(USER_FILE_PATH);
    _fio->OpenFile(USER_FILE_PATH);
  }
  GetUserNumber();
}

RC UIM::CreateUser (const string &userAccount)
{
  RC rc;
  char logMessage[LOG_MESSAGE_MAX_LENGTH];

  // Check out the user existence
  rc = CheckUserExistence(userAccount);
  if (rc != STANDARD_ERROR) {  // STANDARD_ERROR means not found
    // Add user info into server's database
    ClientUserInfo cui;
    memcpy(&cui.userAccount, userAccount.c_str(), length(userAccount));
    int offset;
    if ((offset = CheckEmptySpace()) == ZERO) { // Not enough space
      rc = _fio->AppendFile(sizeof(ClientUserInfo), &cui);
      sprintf(logMessage, CREATE_FAIL_LOG_PROMPT, userAccount.c_str(), "append failed");
    } else { // There is space
      rc = _fio->WriteFile(offset, sizeof(ClientUserInfo), &cui);
      sprintf(logMessage, CREATE_FAIL_LOG_PROMPT, userAccount.c_str(), "write failed");
    }
    if (rc) {
      LOG(WARN, string(logMessage));
      return STANDARD_ERROR;
    }

    // Set new total number of users
    ++_totalUserNumber;
    SetUserNumber();
  }

  sprintf(logMessage, CREATE_SUCC_LOG_PROMPT, userAccount.c_str())
  LOG(INFO, string(logMessage));
  return SUCCESS;
}

RC UIM::RemoveUser (const string &userAccount)
{
  RC rc;
  char logMessage[LOG_MESSAGE_MAX_LENGTH];

  // Check user existence in the user system file
  unsigned offset = CheckUserExistence(userAccount);
  if (offset == ZERO) { // ZERO means not found
    sprintf(logMessage, REMOVE_FAIL_LOG_PROMPT, userAccount.c_str(), "user not found");
    LOG(WARN, string(logMessage));
    return USER_NOT_EXISTS;
  }

  // Move the following user info frontward to cover the removed user info
  int moveBlockSize = _totalUserNumber * sizeof(ClientUserInfo) - offset - sizeof(ClientUserInfo);
  char * data = malloc(moveBlockSize);
  rc = _fio->ReadFile(offset + sizeof(ClientUserInfo), moveBlockSize, data);
  if (rc) {
    sprintf(logMessage, REMOVE_FAIL_LOG_PROMPT, userAccount.c_str(), "read user failed");
    LOG(WARN, string(logMessage));
    free(data);
    return STANDARD_ERROR;
  }
  rc = _fio->WriteFile(offset, moveBlockSize, data);
  free(data);
  if (rc) {
    LOG(WARN, string(logMessage), "move user failed");
    return STANDARD_ERROR;
  }

  // Reset the total number of users
  --_totalUserNumber;
  SetUserNumber();

  sprintf(logMessage, REMOVE_SUCC_LOG_PROMPT, userAccount.c_str());
  log(INFO, string(logMessage));
  return SUCCESS;
}

RC UIM::ReadUser   (const string &userAccount, ClientUserInfo &userInfo)
{
  RC rc;
  char logMessage[LOG_MESSAGE_MAX_LENGTH];

  unsigned offset = CheckUserExistence(userAccount);
  if (offset == ZERO) { // ZERO means not found
    sprintf(logMessage, READ_FAIL_LOG_PROMPT, userAccount.c_str(), "user not found");
    LOG(WARN, string(logMessage));
    return USER_NOT_EXISTS;
  }

  rc = _fio->ReadFile(offset, sizeof(ClientUserInfo), &userInfo);
  if (rc) {
    sprintf(logMessage, READ_FAIL_LOG_PROMPT, userAccount.c_str(), "read user failed");
    LOG(WARN, string(logMessage));
    return STANDARD_ERROR;
  }
  sprintf(logMessage, READ_SUCC_LOG_PROMPT, userAccount.c_str(), );
  LOG(INFO, string(logMessage));
  return SUCCESS;
}

RC UIM::Login      (const ServerUserInfo &userInfo)
{
  char logMessage[LOG_MESSAGE_MAX_LENGTH];
  sprintf(logMessage, LOGIN_LOG_PROMOT, userAccount.c_str());
  LOG(INFO, string(logMessage));
  return SUCCESS;
}

RC UIM::Logout     (const string &userAccount)
{
  char logMessage[LOG_MESSAGE_MAX_LENGTH];
  sprintf(logMessage, LOGOUT_LOG_PROMOT, userAccount.c_str());
  LOG(INFO, string(logMessage));
  return SUCCESS;
}

/************ Helper Functions *************/
void UIM::GetUserNumber ()
{
  CientUserInfoHeader cuih;
  _fio->ReadFile(ZERO, sizeof(CientUserInfoHeader), &cuih);
  _totalUserNumber = cuih.totalUserNumber;
}

void UIM::SetUserNumber ()
{
  CientUserInfoHeader cuih;
  _fio->ReadFile(ZERO, sizeof(CientUserInfoHeader), &cuih);
  cuih.totalUserNumber = _totalUserNumber;
  _fio->WriteFile(ZERO, sizeof(CientUserInfoHeader), &cuih);
}

unsigned UIM::CheckUserExistence (const string &userAccount)
{
  unsigned offset = ZERO;
  ClientUserInfo cui;
  for (size_t i = ZERO; i < _totalUserNumber; ++i) {
    offset = sizeof(CientUserInfoHeader) + i * sizeof(ClientUserInfo);
    _fio->ReadFile(offset, sizeof(ClientUserInfo), &cui);
    if (userAccount == string(cui.userAccount)) {
      return offset;
    }
  }
  return ZERO;
}

unsigned UIM::CheckEmptySpace ()
{
  unsigned size = _fio->GetFileSize();
  unsigned usedSpace = sizeof(CientUserInfoHeader) + sizeof(ClientUserInfo) * _totalUserNumber;
  if ((size - usedSpace) >= sizeof(ClientUserInfo))
    return usedSpace;
  else
    return ZERO;
}
