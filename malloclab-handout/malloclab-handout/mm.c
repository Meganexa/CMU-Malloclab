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
/*
 * Team Information
 */
team_t team = {/* Team name */
               "bovik",
               /* First member's full name */
               "Harry Bovik",
               /* First member's email address */
               "bovik@cs.cmu.edu",
               /* Second member's full name (leave blank if none) */
               "",
               /* Second member's email address (leave blank if none) */
               ""};
static void *coalesce(void *bp) {
  size_t prev_state = GET_STATE(FTR(PRVBL(bp)));
  size_t next_state = GET_STATE(HDR(NXTBL(bp)));
  size_t size = GET_SIZE(HDR(bp));

  if (prev_state && next_state) {
    return bp;
  } else if (prev_state && !next_state) {
    size += GET_SIZE(HDR(NXTBL(bp)));
    PUT(HDR(bp), PACK(size, 0));
    PUT(FTR(bp), PACK(size, 0));
  } else if (!prev_state && next_state) {
    size += GET_SIZE(HDR(PRVBL(bp)));
    PUT(FTR(bp), PACK(size, 0));
    PUT(HDR(PRVBL(bp)), PACK(size, 0));
    bp = PRVBL(bp);
  } else {
    size += GET_SIZE(bp - DSIZE) + GET_SIZE(HDR(NXTBL(bp)));
    PUT(HDR(PRVBL(bp)), PACK(size, 0));
    PUT(FTR(NXTBL(bp)), PACK(size, 0));
    bp = PRVBL(bp);
  }
  return bp;
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

static void *find_fit(size_t size) {
  void *bp = heap_listp;
  size_t curSize = GET_SIZE(HDR(bp));
  int state = GET_STATE(HDR(bp));
  while (curSize != 0) {
    if (curSize >= size && !(state)) {
      return bp;
    } else {
      bp = NXTBL(bp);
      state = GET_STATE(HDR(bp));
      curSize = GET_SIZE(HDR(bp));
    }
  }
  return NULL;
}

static void place(char *bp, size_t size) {
  size_t extra = GET_SIZE(HDR(bp)) - size;
  if (extra >= (2 * DSIZE)) {
    PUT(HDR(bp), PACK(size, 1));
    PUT(FTR(bp), PACK(size, 1));
    PUT(HDR(NXTBL(bp)), PACK(extra, 0));
    PUT(FTR(NXTBL(bp)), PACK(extra, 0));
    coalesce(NXTBL(bp));
  } else {
    PUT(HDR(bp), PACK(GET_SIZE(HDR(bp)), 1));
    PUT(FTR(bp), PACK(GET_SIZE(HDR(bp)), 1));
  }
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size) {
  if (size == 0)
    return NULL;

  size = ALIGN(size + DSIZE);
  size_t excess;
  char *bp;

  if ((bp = find_fit(size)) != NULL) {
    place(bp, size);
    return bp;
  }

  excess = MAX(size, CHUNKSIZE);
  if ((bp = extend_heap(excess / WSIZE)) == NULL)
    return NULL;
  place(bp, size);
  return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *bp) {
  size_t size = GET_SIZE(HDR(bp));
  PUT(HDR(bp), PACK(size, 0));
  PUT(FTR(bp), PACK(size, 0));
  coalesce(bp);
}
/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size) {
  if (ptr == NULL)
    return mm_malloc(size);

  if (size == 0) {
    mm_free(ptr);
    return NULL;
  }

  void *np = mm_malloc(size);
  if (np == NULL)
    return NULL;

  size_t old_size = GET_SIZE(HDR(ptr)) - DSIZE;
  size_t copy = old_size;
  if (copy > size)
    copy = size;

  memcpy(np, ptr, copy);
  mm_free(ptr);
  return np;
}

