/**
\file mpioperator.reallocate.h
\brief դ�����ݿ��طֲ�����

��Ҫ���դ�����ݿ����ȫ������������ʹͬһ�е�������ͬһ�����̽���д���ӿ�д���ٶȡ�
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