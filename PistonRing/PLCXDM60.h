#pragma once


#define PLCM(x)	0 + (x)
#define PLCX(x)	20480 + (x)
#define PLCEX(x)	20736 + (x)
#define PLCY(x)	24576 + (x)
#define PLCEY(x)	24832 + (x)
#define PLCS(x)	28672 + (x)
#define PLCT(x)	40960 + (x)
#define PLCC(x)	45056 + (x)

#define PLCD(x)	0 + (x)
#define PLCSD(x)	28672 + (x)
#define PLCTD(x)	32768 + (x)
#define PLCCD(x)	36864 + (x)
#define PLCHD(x)	41088 + (x)

#define ToDwordPtr(x)	((DWORD*)(x))
#define ToDword(x)		(*((DWORD*)&(x)))
#define ToFloat(x)		(*((float*)&(x)))
#define ToInt16(x)		(*((short*)&(x)))
#define ToLong32(x)		(*((int*)&(x)))

