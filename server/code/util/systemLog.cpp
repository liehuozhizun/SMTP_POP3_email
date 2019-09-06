/*
 * File: systemLog.cpp
 * Author: Yujia Li(liyj070707@gmail.com), Hang Yuan(hyuan211@gmail.com)
 *
 * Created on SEP 4, 2019
 */

#include "systemLog.h"
#include <iostream>

RC CreateLogDir()
{
  std::string dirPath = "../../data/log";
  FileIO *fileio = FileIO::instance();
  return fileio->CreateDir(dirPath);
}

RC LOG(LOGTYPE type, const std::string logMsg)
{
  std::string filePath = "../../data/log/"+GetCurrentData()+".log";
  std::string now = GetCurrentTime();
  std::string logType = logTypeMap[type];
  ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app );
  ofs << now << ' [' << logType << ']\t'<< logMsg << '\n';
  ofs.close();
}
