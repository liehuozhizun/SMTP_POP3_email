/*
 * File: emailError.h
 * Author: Yujia Li(liyj070707@gmail.com), Hang Yuan(hyuan211@gmail.com)
 *
 * Created on Aug 10, 2019
 */

#ifndef EMAIL_ERROR
#define EMAIL_ERROR

enum {
  OPEN_FAILED = 101,
  REMOVE_FAILED,
  SEEK_FAILED,
  READ_FAILED,
  WRITE_FAILED
};

enum {
  MALLOC_FAILED = 201,
};

#endif /* emailError.h */
