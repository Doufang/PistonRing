#pragma once


#define M(x)	0 + (x)
#define X(x)	20480 + (x)
#define EX(x)	20736 + (x)
#define Y(x)	24576 + (x)
#define EY(x)	24832 + (x)
#define S(x)	28672 + (x)
#define T(x)	40960 + (x)
#define C(x)	45056 + (x)

#define D(x)	0 + (x)
#define SD(x)	28672 + (x)
#define TD(x)	32768 + (x)
#define CD(x)	36864 + (x)
#define HD(x)	41088 + (x)

#define ToDwordPtr(x)	((DWORD*)(x))
#define ToDword(x)		(*((DWORD*)&(x)))

