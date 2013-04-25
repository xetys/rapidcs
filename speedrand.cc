#include "speedrand.h"

unsigned long randbuf[17][2];
int p1, p2;


inline unsigned long _lrotl(unsigned long value, int shift)
{
  return (value << shift) | (value >> (sizeof (unsigned long) * 8 - shift)); 
}

void SpeedRand::InitRand()
{   
    int seed = time(NULL);
    int i, j;

    for(i = 0; i < 17; i++) {
        for(j = 0; j < 2; j++) {
            seed = seed * 2891336453UL + 1;
            randbuf[i][j] = seed;
        }
    }
    // initialize pointers to circular buffer
    p1 = 0;
    p2 = 10;
}
unsigned long SpeedRand::RandomNumber(unsigned long Max)
{
    unsigned long y, z;
    // generate next number
    z = _lrotl(randbuf[p1][0], 19) + randbuf[p2][0];
    y = _lrotl(randbuf[p1][1], 27) + randbuf[p2][1];
    randbuf[p1][0] = y;
    randbuf[p1][1] = z;
    // rotate list pointers
    if (--p1 < 0)
        p1 = 16;
    if (--p2 < 0)
        p2 = 16;
    return y % Max;
}

/*
int main()
{
    SpeedRand sr;
    sr.InitRand();
    for(int i = 0; i < 10;i++)printf("%i\r\n",(int)sr.RandomNumber(100));
    return 0;
}*/