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

UIM* UIM::_uim = ZERO;

UIM* UIM::instance()
{
  if(!_file_io)
    _file_io = new FileIO();

  return _file_io;
}

/**
 * This function will check if the given user account exists in server's database.
 * @param string as the given username@domain. E.g., admin@sample.com.
 * @return SUCCESS if the username exists in the server's database.
 *         USER_EXISTS if the user exists in server's database.
 *         STANDARD_ERROR otherwise.
 */
RC UIM::CheckExist (const string &userAccount)
{
  // Send info to server
  return SUCCESS;
}

/**
 * This function will create a new user account both locally and remotely.
 * @param ServerUserInfo contains all the information needed to create a user.
 * @return SUCCESS if create a new user successfully.
 *         CREATE_USER_ERROR if fail to create.
 *         STANDARD_ERROR otherwise.
 */
RC UIM::CreateUser (const ServerUserInfo &userInfo)
{
  RC rc;

  // Generate file name
  string userAccount = "";
  rc = UserAccountGenerator_S(&userInfo, &userAccount);
  if (rc)
    return rc;
  string fileName = userAccount + SYS_EXTENSION;

  // Create user.sys file locally
  FileIO *fio = FileIO::instance();
  if (fio == NULL)
    return STANDARD_ERROR;
  string filePath = DATAPATH + fileName;
  rc = fio->CreateFile(filePath);
  rc = fio->WriteFile(ZERO, sizeof ClientUserInfo, &userInfo)
  if (rc)
    return STANDARD_ERROR;

  // Add user info into server's database
  //send

  return SUCCESS;
}

/**
 * This function close a user's account by removing all information of this user
 * locally and remotely.
 * @param string as the given username@domain. E.g., admin@sample.com.
 * @return SUCCESS if the user has been successfully removed.
 *         USER_NOT_EXISTS if the user doesn't exist in server's database.
 *         STANDARD_ERROR otherwise.
 */
RC UIM::CloseUser  (const string &userAccount) {
  RC rc;

  // Remove user info at server's database

  // Remove local user info file
  string filePath = DATAPATH + userAccount + SYS_EXTENSION;
  FileIO *fio = FileIO::instance();
  if (fio == NULL)
    return STANDARD_ERROR;
  rc = fio->DestroyFile(filePath);
  if (rc)
    return STANDARD_ERROR;

  return SUCCESS;
}

RC UIM::ReadUser   (const string &userAccount, ClientUserInfo &userInfo)
{
  RC rc;
  FileIO *fio = FileIO::instance();
  if (fio == NULL)
    return STANDARD_ERROR;

  string filePath = DATAPATH + userAccount + SYS_EXTENSION;
  rc = fio->OpenFile(filePath);
  if (rc)
    return USER_NOT_EXISTS;

  rc = fio->ReadFile(ZERO, sizeof ClientUserInfo, &userInfo);
  if (rc)
    return READ_USER_ERROR;

  return SUCCESS;
}

/**
 * This function will update the user's information locally and remotely.
 * @param ServerUserInfo contains all information needed to be updated.
 * @return SUCCESS if update successfully.
 *         STANDARD_ERROR otherwise.
 */
RC UIM::UpdateUser (const ServerUserInfo &userInfo)
{
  RC rc;

  // Save the user info locally
  ClientUserInfo clientUserInfo;
  rc = UserInfo_StoC(&userInfo, &clientUserInfo);
  if (rc)
    return STANDARD_ERROR;

  FileIO *fio = FileIO::instance();
  if (fio == NULL)
    return STANDARD_ERROR;

  string userAccount = "";
  rc = UserAccountGenerator_S(&userInfo, &userAccount);
  if (rc)
    return STANDARD_ERROR;
  string filePath = DATAPATH + userAccount + SYS_EXTENSION;
  rc = fio->OpenFile(filePath);
  if (rc)
    return STANDARD_ERROR;

  rc = fio->WriteFile(ZERO, sizeof ClientUserInfo, &userInfo);
  if (rc)
    return STANDARD_ERROR;

  // Send updated information to server

  return SUCCESS;
}

/**
 * This function will try to obtain the user's information from the server's
 * database. If the user's info system file doesn't exist, create it first.
 * @param string as the given username@domain. E.g., admin@sample.com.
 * @return SUCCESS if the user has been successfully setup.
 *         STANDARD_ERROR otherwise.
 */
RC UIM::SetupUser  (const string &userAccount);

/**
 * This function will login the user with given password and verify it with the
 * help of server's interface. Login will SetupUser.
 * @param ServerUserInfo contains all the information needed to login the account
 * @return SUCCESS if the user has successfullt logined and been set up
 *         PASSWORD_ERROR if the password is mismatched with the record in server
 *         USER_NOT_EXISTS if the user doesn't exist in server
 *         STANDARD_ERROR otherwise;
 */
RC UIM::Login      (const ServerUserInfo &userInfo);

/**
 * This function will logout your account from the current system and send your
 * logout action to server.
 * @param string as the given username@domain. E.g., admin@sample.com.
 * @return SUCCESS if logout successfully
 *         STANDARD_ERROR otherwise;
 */
RC UIM::Logout     (const string &userAccount);

/**
 * These function will translate the UserInfo to a userAccount.
 * @param ServerUserInfo/ClientUserInfo contain the given information.
 *        string to store the userAccount.
 * @return SUCCESS if translate successfully.
 *         STANDARD_ERROR otherwise.
 */
RC UIM::UserAccountGenerator_S (const ServerUserInfo &userInfo, string &userAccount);
RC UIM::UserAccountGenerator_C (const ClientUserInfo &userInfo, string &userAccount);

/**
 * This function will generate a half-empty UserInfo with given userAccount
 * @param string as the given username@domain. E.g., admin@sample.com.
 *        ServerUserInfo/ClientUserInfo contain the given information.
 * @return SUCCESS if translate successfully
 *         STANDARD_ERROR otherwise;
 */
RC UIM::ClientUserInfoGenerator (const string &userAccount, ClientUserInfo &userInfo);
RC UIM::ServerUserInfoGenerator (const string &userAccount, ServerUserInfo &userInfo);

/**
 * These function will translate UserInfo from the other type
 * @param ServerUserInfo/ClientUserInfo contains the given information.
 *        ClientUserInfo/ServerUserInfo to store translation.
 * @return SUCCESS if translate successfully
 *         STANDARD_ERROR otherwise;
 */
RC UserInfo_StoC (const ServerUserInfo &userInfo, ClientUserInfo & C_userInfo);
RC UserInfo_CtoS (const ClientUserInfo &userInfo, ServerUserInfo & S_userInfo);
