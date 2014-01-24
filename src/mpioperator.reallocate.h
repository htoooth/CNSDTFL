/**
\file mpioperator.reallocate.h
\brief 栅格数据块重分布函数

主要针对栅格数据块进行全互换，尽可能使同一行的数据在同一个进程进行写，加快写的速度。
\author htoo
\version 0.0.0.1
\date 2013-02-18
*/
#ifndef MPI_OPERATOR_REALLOCATE_H_INCLUDE

#define MPI_OPERATOR_REALLOCATE_H_INCLUDE

#include "mpioperator.h"

int ReAllocateRasterBlock(void * SendBuf, int SendCount, MPI_Datatype SendType,
                          void * RecvBuf, int RecvCount, MPI_Datatype RecvType,
                          MPI_Comm Comm);

#endif