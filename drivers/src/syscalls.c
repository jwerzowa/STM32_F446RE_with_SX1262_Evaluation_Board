/*
 * syscalls.c — retargets newlib's _write() so printf()/puts() route
 * through USART2 instead of doing nothing (the default bare-metal stub).
 */

#include <stdint.h>
#include <sys/stat.h>
#include "../inc/usart.h"

int _write(int file, char *ptr, int len)
{
    (void)file; /* no real stdout/stderr distinction in this bare-metal
                   setup — everything goes to the same UART */

    USART_transmit(USART2, (uint8_t *)ptr, (uint16_t)len);

    return len; /* newlib expects the number of bytes actually written */
}

/* ---- Everything below exists only so the linker has something to
 * resolve. None of it is meaningful in a bare-metal, no-OS, no-filesystem
 * environment — these are harmless stubs, except _sbrk which genuinely
 * needs to work since heap-using code (some printf paths, malloc) calls
 * it to grow the heap. ----
 */

int _close(int file) { (void)file; return -1; }

int _lseek(int file, int ptr, int dir) { (void)file; (void)ptr; (void)dir; return 0; }

int _read(int file, char *ptr, int len) { (void)file; (void)ptr; (void)len; return 0; }

int _fstat(int file, struct stat *st) { (void)file; st->st_mode = S_IFCHR; return 0; }

int _isatty(int file) { (void)file; return 1; }

void _exit(int status) { (void)status; while (1); }

int _kill(int pid, int sig) { (void)pid; (void)sig; return -1; }

int _getpid(void) { return 1; }

/* Heap grows upward from _ebss (end of .bss, defined in linker.ld)
 * toward the stack. No overflow check against the stack pointer yet —
 * fine for now since nothing here does heavy allocation, but worth
 * adding a bounds check later if you start relying on malloc more. */
extern uint32_t _ebss;
static uint8_t *heap_end = (uint8_t *)&_ebss;

void *_sbrk(int incr)
{
    uint8_t *prev_heap_end = heap_end;
    heap_end += incr;
    return prev_heap_end;
}