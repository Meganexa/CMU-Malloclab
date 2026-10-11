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
#include <stddef.h>
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

//  Private constants
static char *heap_listp;
//  Macros

#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1 << 12)

#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define PACK(size, state) (size | state)

#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_STATE(p) (GET(p) & 0x1)

#define HDR(bp) ((char *)bp - WSIZE)
#define FTR(bp) (GET_SIZE(HDR(bp)) + (char *)bp - DSIZE)

#define NXTBL(bp) ((char *)bp + GET_SIZE(HDR(bp)))
#define PRVBL(bp) ((char *)bp - GET_SIZE((char *)bp - DSIZE))

static void *coalesce(void *bp) {
  size_t prev_state = GET_STATE(FTR(PRVBL(bp)));
  size_t next_state = GET_STATE(HDR(NXTBL(bp)));
  size_t size = GET_SIZE(HDR(bp));
}
static void *extend_heap(size_t word) {
  char *bp;
  size_t size;
  size = (word % 2) ? ((word + 1) * WSIZE) : (word * WSIZE);
  if ((long)(bp = mem_sbrk(size)) == -1)
    return NULL;

  PUT(HDR(bp), PACK(size, 0));
  PUT(FTR(bp), PACK(size, 0));
  PUT(HDR(NXTBL(bp)), PACK(0, 1));

  return coalesce(bp);
}

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {
  if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *)-1)
    return -1;

  PUT(heap_listp, 0);
  PUT(heap_listp + 1 * (WSIZE), PACK(DSIZE, 1));
  PUT(heap_listp + 2 * WSIZE, PACK(DSIZE, 1));
  PUT(heap_listp + 3 * WSIZE, PACK(0, 1));
  heap_listp += (2 * WSIZE);

  if (extend_heap(CHUNKSIZE / WSIZE) == NULL)
    return -1;
  return 0;
}

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
