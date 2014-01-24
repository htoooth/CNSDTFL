#ifndef DAT_BLOCK_H_INCLUDE

#define DAT_BLOCK_H_INCLUDE

namespace port
{

typedef struct _b
{
	///数据信息
	long   DID;     ///数据编号
	long   nOffX;	///开始位置
	long   nOffY;   ///结束位置
	long   nXSize;  ///数据x大小
	long   nYSize;  ///数据y大小

	///进程信息
	long   PDID;    ///进程处理的数据编号
	int    PID;     ///进程编号
	int    bDone;   ///是否完成该数据的处理
	double tStart;  ///开始时间
	double tEnd;    ///结束时间

	_b();

} DatBlock;


}
#endif