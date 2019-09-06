/*
 * File: systemLog.h
 * Author: Yujia Li(liyj070707@gmail.com), Hang Yuan(hyuan211@gmail.com)
 *
 * Created on Aug 11, 2019
 */

#ifndef SYSTEM_LOG_H
#define SYSTEM_LOG_H

#include <map>

#include "emailError.h"
#include "time.h"
#include "util.h"
#include "../fileIO/fileio.h"

typedef enum {
  INFO = 0,
  WARN,
  ERRO
} LOGTYPE;

const std::map<int, std::string> logTypeMap =
{
  { 0, "INFO" },
  { 1, "WARN" },
  { 2, "ERRO" }
};

/**
 * System Info Manager should check log directory when init client or server.
 * @return SUCCESS if the directory is successfully created.
 *         CREATE_ERROR if create directory not successfully.
 */
RC CreateLogDir();

/**
 * output log to log file in log directory.
 * This function **ASSUME** log directory exists.
 * @param  LOGTYPE is from {INFO, WARN, ERRO};
 *         const string given as log message.
 * @return SUCCESS if the log has been write successfully.
 *         STANDARD_ERROR if failed.
 */
RC LOG(LOGTYPE type, const std::string logMsg);

#endif /* systemLog.h */
