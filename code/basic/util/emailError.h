/*
 * File: emailError.h
 * Author: Yujia Li(liyj070707@gmail.com), Hang Yuan(hyuan211@gmail.com)
 *
 * Created on Aug 10, 2019
 */

#ifndef EMAIL_ERROR
#define EMAIL_ERROR

#define SUCCESS 0

enum {
  OPEN_FAILED = 100,
  REMOVE_FAILED,
  SEEK_FAILED,
  READ_FAILED,
  WRITE_FAILED
};

enum {
  MALLOC_FAILED = 200,
};

#endif /* emailError.h */
