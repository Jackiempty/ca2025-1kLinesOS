#pragma once
#include "common.h"

// ====================================================================
// System Constants
// ====================================================================
#define PROCS_MAX 8      // Maximum number of processes
#define PROC_UNUSED 0    // Unused process control structure
#define PROC_RUNNABLE 1  // Runnable process

// ====================================================================
// RISC-V Hardware Definitions
// ====================================================================
#define SATP_SV32 (1u << 31)
#define PAGE_V (1 << 0)  // "Valid" bit (entry is enabled)
#define PAGE_R (1 << 1)  // Readable
#define PAGE_W (1 << 2)  // Writable
#define PAGE_X (1 << 3)  // Executable
#define PAGE_U (1 << 4)  // User (accessible in user mode)

// ====================================================================
// Data Structures
// ====================================================================

struct sbiret {
  long error;
  long value;
};

struct trap_frame {
  uint32_t ra, gp, tp, t0, t1, t2, t3, t4, t5, t6;
  uint32_t a0, a1, a2, a3, a4, a5, a6, a7;
  uint32_t s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
  uint32_t sp;
} __attribute__((packed));

struct process {
  int pid;     // Process ID
  int state;   // Process state: PROC_UNUSED or PROC_RUNNABLE
  vaddr_t sp;  // Stack pointer
  uint32_t *page_table;
  uint8_t stack[8192];  // Kernel stack
};

// ====================================================================
// Public API
// ====================================================================

struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4, long arg5, long fid, long eid);

// ====================================================================
// Macros
// ====================================================================

#define PANIC(fmt, ...)                                                   \
  do {                                                                    \
    printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    while (1) {                                                           \
    }                                                                     \
  } while (0)

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