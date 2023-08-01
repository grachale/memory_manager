# Memory manager

The given task is focused on a better understanding of memory allocation in the operating system. It involves implementing a simplified version of a dynamic memory manager for a user process.

In simple model environment, there is no heap manager (it is disconnected in the testing environment on Progtest). Therefore, functions like malloc, free, ..., as well as new and delete operators do not exist. Task is to implement their simplified equivalents.

Interface will work with a block of allocated memory of a specified size (you will be provided with a pointer to an existing memory block and information about its size during initialization). The given memory block will be contiguous and relatively large (ranging from a few to hundreds of MiB). Your implemented memory subsystem will be responsible for managing this given memory block. It should enable the utilization of smaller parts of the block according to incoming requests, keep track of allocated and unused parts of the given memory block, allow for block allocation, block deallocation, and merging of freed blocks into contiguous chunks.

Required functions and their meanings:

1. int HeapInit(void* startAddr, int size);
   
The function HeapInit will be called at the beginning during the initialization of the subsystem, before calling other implemented functions. The function has two parameters: startAddr, a pointer that indicates the memory address of the block that your memory subsystem will manage, and size, the size of the given memory block in bytes.

In this memory block (starting at the address startAddr and having a size of size bytes), the heap will be implemented. Therefore, all allocated blocks and your service information about which parts of your heap are used or free will be stored here.

2. void HeapDone(int* pendingBlk);
   
The function HeapDone will be called at the end of using the heap. The function will determine how many memory blocks are still allocated on the heap and pass this information to the caller through the output parameter pendingBlk.

4. void* HeapAlloc(int size);
   
The function HeapAlloc will serve as the entry point for allocating a memory block in the managed heap. The function takes the size of the requested block as a parameter and returns a pointer to the allocated memory area. In case of failure, the function returns NULL. Failure can occur if the space in the given memory is already exhausted or if the given memory block does not contain a sufficiently large contiguous area for the requested allocation after previous allocations.

6. bool HeapFree(void* blk);
   
The function HeapFree is used to deallocate a previously allocated block using HeapAlloc. The function returns true if the block was successfully freed or false if the block could not be freed (due to an invalid address being provided, or the block has already been freed, etc.). You will have limited resources for implementation (see the list of header files). The STL is not available, and neither are standard functions for dynamic allocation (malloc, ...) nor their C++ equivalents (new). The submitted code will be executed in a restricted environment. 

You will be limited in available memory (apart from the given memory block, there will be only a few kilobytes available to store a few global variables). This extra available memory is not sufficient to store all the service information about allocations (you will have to save this information in the managed memory and manage their placement).
