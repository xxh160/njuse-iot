#ifndef __COMMON_H_
#define __COMMON_H_

#define ASNI_FG_GREEN "\33[1;32m"
#define ASNI_FG_RED "\33[1;31m"
#define ASNI_FG_BLUE "\33[1;34m"
#define ASNI_NONE "\33[0m"

#include <stdio.h>

#define uerror(format, ...) \
  printf(ASNI_FG_BLUE "[Error]: " ASNI_FG_RED format ".\n" ASNI_NONE, __VA_ARGS__)

#define uinfo(format, ...) \
  printf(ASNI_FG_BLUE "[Info ]: " ASNI_FG_GREEN format ".\n" ASNI_NONE, __VA_ARGS__)

#define ARRLEN(arr) (int)(sizeof(arr) / sizeof(arr[0]))

#endif

