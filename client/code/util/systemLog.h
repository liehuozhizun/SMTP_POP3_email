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
#include "../fileIO/fileio.h"

RC CreateLogDir()
{
  string dirPath = "../../data/log";
  return CreateDir(dirPath);
}

RC Logger(string logMsg)
{
    string filePath = "../../data/log/"+GetCurrentData()+".log";
    string now = GetCurrentTime();
    ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app );
    ofs << now << '\t' << logMsg << '\n';
    ofs.close();
}
#endif /* systemLog.h */
