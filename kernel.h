#pragma once

struct sbiret {
  long error;
  long value;
};

struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4, long arg5, long fid, long eid);

#define PANIC(fmt, ...)                                                   \
  do {                                                                    \
    printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    while (1) {                                                           \
    }                                                                     \
  } while (0)