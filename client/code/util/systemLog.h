/*
 * File: systemLog.h
 * Author: Yujia Li(liyj070707@gmail.com), Hang Yuan(hyuan211@gmail.com)
 *
 * Created on Aug 11, 2019
 */

#ifndef SYSTEM_LOG_H
#define SYSTEM_LOG_H

#include "emailError.h"
#include "time.h"
#include "util.h"
#include "../fileIO/fileio.h"

// System manager should check log directory when
RC CreateLogDir();

// This function **ASSUME** log directory exists.
RC LOG(LOGTYPE type,string logMsg);

#endif /* systemLog.h */
