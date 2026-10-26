# CMU's Malloclab

This is my solution for CMU's assignment on Custom Memory Allocator.

# Versions

This assignment explores different methods of allocating memory. They are listed as follows :

-  **Implicit free lists** -
  - Improved search for free blocks. (next_fit)
  - Coalesce_all() function to do a whole sweep instead of joining blocks every time.
  - Eliminated footers to reduce internal fragmentation and improve accessibility to state of previous blocks.
  
