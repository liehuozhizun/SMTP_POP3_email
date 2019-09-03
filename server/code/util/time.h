/*
 * File: time.h
 * Author: Yujia Li(liyj070707@gmail.com), Hang Yuan(hyuan211@gmail.com)
 *
 * Created on Aug 11, 2019
 */

#ifndef TIME_H
#define TIME_H

#include <time.h>

/**
 * This function will return current time in Year Month Date Time format
 * @return return current time in [Year Month Date Time] format
 */
inline string GetCurrentTime()
{
  char buf[80];
  time_t now = time(0);
  struct tm *tstruct;
  tstruct = localtime(&now);
  strftime(buf, sizeof(buf), "%Y-%m-%d %X", tstruct);
  return string(buf);
}

/**
 * This function will return current Data in Year Month Dateformat
 * @return return current time in [Year Month Date] format
 */
inline string GetCurrentData()
{
  char  buf[80];
  time_t now = time(0);
  struct tm *tstruct;
  tstruct = localtime(&now);
  strftime(buf, sizeof(buf), "%Y-%m-%d", tstruct);
  return string(buf);
}

#endif /* time.h */
