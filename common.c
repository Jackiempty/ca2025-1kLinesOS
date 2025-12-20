#include "common.h"

struct sbiret {
  long error;
  long value;
};
extern struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4, long arg5, long fid,
                              long eid);

void putchar(char ch) { sbi_call(ch, 0, 0, 0, 0, 0, 0, 1 /* Console Putchar */); }

void printf(const char *fmt, ...) {
  va_list vargs;
  va_start(vargs, fmt);

  while (*fmt) {
    if (*fmt == '%') {
      fmt++;           // Skip '%'
      switch (*fmt) {  // Read the next character
        case '\0':     // '%' at the end of the format string
          putchar('%');
          goto end;
        case '%':  // Print '%'
          putchar('%');
          break;
        case 's': {  // Print a NULL-terminated string.
          const char *s = va_arg(vargs, const char *);
          while (*s) {
            putchar(*s);
            s++;
          }
          break;
        }
        case 'd': {  // Print an integer in decimal.
          int value = va_arg(vargs, int);
          unsigned magnitude = value;  // https://github.com/nuta/operating-system-in-1000-lines/issues/64
          if (value < 0) {
            putchar('-');
            magnitude = -magnitude;
          }

          unsigned divisor = 1;
          while (magnitude / divisor > 9) divisor *= 10;

          while (divisor > 0) {
            putchar('0' + magnitude / divisor);
            magnitude %= divisor;
            divisor /= 10;
          }

          break;
        }
        case 'x': {  // Print an integer in hexadecimal.
          unsigned value = va_arg(vargs, unsigned);
          for (int i = 7; i >= 0; i--) {
            unsigned nibble = (value >> (i * 4)) & 0xf;
            putchar("0123456789abcdef"[nibble]);
          }
        }
      }
    } else {
      putchar(*fmt);
    }

    fmt++;
  }

end:
  va_end(vargs);
}

void *memcpy(void *dst, const void *src, size_t n) {
  uint8_t *d = (uint8_t *)dst;
  const uint8_t *s = (const uint8_t *)src;
  while (n--) *d++ = *s++;
  return dst;
}

void *memset(void *buf, char c, size_t n) {
  uint8_t *p = (uint8_t *)buf;
  while (n--) *p++ = c;
  return buf;
}

char *strcpy(char *dst, const char *src) {
  char *d = dst;
  while (*src) *d++ = *src++;
  *d = '\0';
  return dst;
}

int strcmp(const char *s1, const char *s2) {
  while (*s1 && *s2) {
    if (*s1 != *s2) break;
    s1++;
    s2++;
  }

  return *(unsigned char *)s1 - *(unsigned char *)s2;
}

struct trap_frame {
  uint32_t ra;
  uint32_t gp;
  uint32_t tp;
  uint32_t t0;
  uint32_t t1;
  uint32_t t2;
  uint32_t t3;
  uint32_t t4;
  uint32_t t5;
  uint32_t t6;
  uint32_t a0;
  uint32_t a1;
  uint32_t a2;
  uint32_t a3;
  uint32_t a4;
  uint32_t a5;
  uint32_t a6;
  uint32_t a7;
  uint32_t s0;
  uint32_t s1;
  uint32_t s2;
  uint32_t s3;
  uint32_t s4;
  uint32_t s5;
  uint32_t s6;
  uint32_t s7;
  uint32_t s8;
  uint32_t s9;
  uint32_t s10;
  uint32_t s11;
  uint32_t sp;
} __attribute__((packed));

#define READ_CSR(reg)                                     \
  ({                                                      \
    unsigned long __tmp;                                  \
    __asm__ __volatile__("csrr %0, " #reg : "=r"(__tmp)); \
    __tmp;                                                \
  })

#define WRITE_CSR(reg, value)                               \
  do {                                                      \
    uint32_t __tmp = (value);                               \
    __asm__ __volatile__("csrw " #reg ", %0" ::"r"(__tmp)); \
  } while (0)