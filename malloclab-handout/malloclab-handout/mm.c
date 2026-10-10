/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "memlib.h"
#include "mm.h"

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

// Constants and Macros

#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1 << 12)

#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define PACK(size, state) (size | state)

#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)p = val)

#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_STATE(p) (GET(p) & 0x1)

#define HDR(bp) ((char *)bp - WSIZE)
#define FTR(bp) (GET_SIZE(HDR(bp)) + (char *)bp - DSIZE)

#define NXTBL(bp) ((char *)bp + GET_SIZE(HDR(bp))
#define PRVBL(bp) ((char *)bp - GET_SIZE((char *)bp - DSIZE))
/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void) { return 0; }

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size) {}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr) {}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size) {}
