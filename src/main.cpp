#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <cmath>
using namespace std;

#endif /* __PROGTEST__ */
struct HeaderOfBlock
{
    HeaderOfBlock(bool _available = true, size_t _size = 0, HeaderOfBlock* _previousBlock = nullptr)
            : available(_available),
              size(_size),
              previousBlock(_previousBlock){}
    bool available;
    int size;
    HeaderOfBlock * previousBlock;
    unsigned int idOfBlock = 13072001;
};

const int HEADER_OF_BLOCK_SIZE = sizeof(HeaderOfBlock);

void * heapStart = nullptr;
void * heapEnd   = nullptr;
int memorySize = 0;
int ourPendingBlk;

bool isItTheRightBlock ( int index, int size )
{
    double number = index / size;
    return number - floor(number) > 0;
}

void   HeapInit    ( void * memPool, int memSize )
{
    heapStart = memPool;
    heapEnd   = (char *)heapStart + memSize;
    memorySize = memSize;
    ourPendingBlk = 0;

    HeaderOfBlock * initialBlock = (HeaderOfBlock * ) heapStart;

    initialBlock -> size            = memSize;
    initialBlock -> available       = true;
    initialBlock -> previousBlock   = nullptr;
    initialBlock -> idOfBlock       = 13072001;
}
void * HeapAlloc   ( int    size )
{
    if ( size <= 0 || size > memorySize - HEADER_OF_BLOCK_SIZE )
        return nullptr;

//    int minimumBlock = nearestPowerOfTwoNumber( size + HEADER_OF_BLOCK_SIZE );

    HeaderOfBlock * current = (HeaderOfBlock * ) heapStart;
    bool indicator = false;
    while ( true )
    {
        // we have enough space
        if ( current -> available && current -> size >= size + HEADER_OF_BLOCK_SIZE )
        {
            indicator = true;
            int centerOfBlock = ceil((double)current -> size / 2);
            // we need to split
            if ( centerOfBlock >= size + HEADER_OF_BLOCK_SIZE )
            {
                HeaderOfBlock * nextHeaderOfBlock = (HeaderOfBlock * )((char *)current + centerOfBlock);
                nextHeaderOfBlock -> available = true;
                nextHeaderOfBlock -> size = current -> size - centerOfBlock;
                nextHeaderOfBlock -> previousBlock = current;
                nextHeaderOfBlock -> idOfBlock = 13072001;
                current -> size = centerOfBlock;
            } else
            {
                ourPendingBlk++;
                current -> available = false;
                return (char *)current + HEADER_OF_BLOCK_SIZE;
            }
        }
        if ( indicator )
            indicator = false;
        else
        {
           if ( (char *)current + current -> size >= heapEnd )
               return nullptr;
           current = (HeaderOfBlock *)((char *)current + current -> size);
        }
    }
    return nullptr;
}

bool   HeapFree    ( void * blk )
{
    if ( ourPendingBlk <= 0 || blk == nullptr || blk > heapEnd || blk < (HeaderOfBlock * )heapStart + 1 )
        return false;

    HeaderOfBlock * current = (HeaderOfBlock *)((char *)blk - HEADER_OF_BLOCK_SIZE);
    if ( current -> idOfBlock != 13072001 || current -> available )
        return false;

    current -> available = true;

    bool indicator1 = false;
    bool indicator2 = false;
    while ( true )
    {
        auto next = (HeaderOfBlock* )((char *)current + current -> size);
        int index = (char *) current - (char*)heapStart;
        if (!isItTheRightBlock(index, current -> size) && next < heapEnd && next -> available && next -> size == current -> size )
        {
            current -> size += next -> size;
            if ( next + next -> size < heapEnd )
                next -> previousBlock = current;
        } else
        {
            indicator1 = true;
        }

        auto previous = current -> previousBlock;
        if ( isItTheRightBlock(index, current -> size) && previous != nullptr && previous -> available && previous -> size == current -> size )
        {
            previous -> size += current -> size;
            current = previous;
        } else
        {
            indicator2 = true;
        }

        if ( indicator1 && indicator2 )
            break;
        else
        {
            indicator1 = false;
            indicator2 = false;
        }
    }

    ourPendingBlk--;
    return true;
}
void   HeapDone    ( int  * pendingBlk )
{
    heapStart = nullptr;
    heapEnd   = nullptr;
    *pendingBlk = ourPendingBlk;
}

#ifndef __PROGTEST__

int main ( void )
{

    uint8_t       * p0, *p1, *p2, *p3, *p4;
    int             pendingBlk;
    static uint8_t  memPool[3 * 1048576];

    HeapInit ( memPool, 2097152 );
    assert ( ( p0 = (uint8_t*) HeapAlloc ( 512000 ) ) != NULL );
    memset ( p0, 0, 512000 );
    assert ( ( p1 = (uint8_t*) HeapAlloc ( 511000 ) ) != NULL );
    memset ( p1, 0, 511000 );
    assert ( ( p2 = (uint8_t*) HeapAlloc ( 26000 ) ) != NULL );
    memset ( p2, 0, 26000 );
    HeapDone ( &pendingBlk );
    assert ( pendingBlk == 3 );


    HeapInit ( memPool, 2097152 );
    assert ( ( p0 = (uint8_t*) HeapAlloc ( 1000000 ) ) != NULL );
    memset ( p0, 0, 1000000 );
    assert ( ( p1 = (uint8_t*) HeapAlloc ( 250000 ) ) != NULL );
    memset ( p1, 0, 250000 );
    assert ( ( p2 = (uint8_t*) HeapAlloc ( 250000 ) ) != NULL );
    memset ( p2, 0, 250000 );
    assert ( ( p3 = (uint8_t*) HeapAlloc ( 250000 ) ) != NULL );
    memset ( p3, 0, 250000 );
    assert ( ( p4 = (uint8_t*) HeapAlloc ( 50000 ) ) != NULL );
    memset ( p4, 0, 50000 );
    assert ( HeapFree ( p2 ) );
    assert ( HeapFree ( p4 ) );
    assert ( HeapFree ( p3 ) );
    assert ( HeapFree ( p1 ) );
    assert ( ( p1 = (uint8_t*) HeapAlloc ( 500000 ) ) != NULL );
    memset ( p1, 0, 500000 );
    assert ( HeapFree ( p0 ) );
    assert ( HeapFree ( p1 ) );
    HeapDone ( &pendingBlk );
    assert ( pendingBlk == 0 );


    HeapInit ( memPool, 2359296 );
    assert ( ( p0 = (uint8_t*) HeapAlloc ( 1000000 ) ) != NULL );
    memset ( p0, 0, 1000000 );
    assert ( ( p1 = (uint8_t*) HeapAlloc ( 500000 ) ) != NULL );
    memset ( p1, 0, 500000 );
    assert ( ( p2 = (uint8_t*) HeapAlloc ( 500000 ) ) != NULL );
    memset ( p2, 0, 500000 );
    assert ( ( p3 = (uint8_t*) HeapAlloc ( 500000 ) ) == NULL );
    assert ( HeapFree ( p2 ) );
    assert ( ( p2 = (uint8_t*) HeapAlloc ( 300000 ) ) != NULL );
    memset ( p2, 0, 300000 );
    assert ( HeapFree ( p0 ) );
    assert ( HeapFree ( p1 ) );
    HeapDone ( &pendingBlk );
    assert ( pendingBlk == 1 );



    HeapInit ( memPool, 2359296 );
    assert ( ( p0 = (uint8_t*) HeapAlloc ( 1000000 ) ) != NULL );
    memset ( p0, 0, 1000000 );
    assert ( ! HeapFree ( p0 + 1000 ) );
    HeapDone ( &pendingBlk );
    assert ( pendingBlk == 1 );

    return 0;
}

#endif /* __PROGTEST__ */

