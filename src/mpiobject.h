/**
\file mpiobject.h
\brief MPI对象封装

该文件的主要用C++将MPI的相关操作集中起来操作。
\author htoo
\version 0.0.0.1
\date 2013-02-27
*/

#ifndef MPI_OBJECT_H_INCLUDE

#define MPI_OBJECT_H_INCLUDE

#include <iostream>

#include "mpi.h"

class MPIObject
{
public:
	/************************************************************************/
	/* 全局函数                                                              */
	/************************************************************************/

	///初始化MPI
	static MPIObject * CreateMPI(int MPI_argc, char ** MPI_argv);

	///退出MPI
	static void DestoryMPI();

	/************************************************************************/
	/*成员函数                                                               */
	/************************************************************************/

	///改变通信域，形成新的子通信域
	MPIObject(MPI_Comm comm);

	///全局通信域
	MPIObject();

	///将成员变量清空
	~MPIObject();

	/************************************************************************/
	/* 基本信息                                                              */
	/************************************************************************/

	///获得当前时间
	double GetTime() const;

	///获得初始化时间
	double GetInitTime() const;

	///获得当前进程号
	int GetRank() const;

	///获得当前通信域的进程个数
	int GetSize() const;

	///获得当前通信器
	MPI_Comm GetComm() const;

	///获得当前节点名
	const char * GetNodeName() const;

	///获得当前MPI的版本
	int GetVersion() const;

	///获得当前MPI的子版本
	int GetSubVersion() const;

	///在当前通信域同步
	void GetBarrier() const;

	/************************************************************************/
	/* 高级函数                                                              */
	/************************************************************************/

	///获得当前时间
	/** \sa MPIObject::GetTime()*/
	double GetNow() const;

	///判断是不是主进程
	/** \sa MPIObject::GetMaster*/
	bool IsMaster() const;

	///测断是不是根进程
	///
	///用来判断在全局域中的根进程
	/** \sa MPIObject::GetMaster*/
	bool IsRoot() const;

	///判断是不是从进程
	/** \sa MPIObject::GetMaster*/
	bool IsSlave() const;

	///返回主进程号
	int GetMaster() const;

	///获得当前进程号
	/** \sa MPIObject::GetRank*/
	int GetMe() const;

	///获得当前通信域的进程个数
	/** \sa MPIObject::GetSize*/
	int GetOurSize() const;

	///获得节点名
	/** \sa MPIObject::GetNodeName*/
	const char * GetMyName() const;

	///判断是不是当前进程
	/** \sa MPIObject::GetRank*/
	bool IsMe(const int rank) const ;

	///打印当前信息
	void PrintMe() const ;

	///打印信息
	void Print(const char * str) const;

	///打印附加信息
	template <typename T>
	void PrintMe(T val);

	/************************************************************************/
	/* 常量                                                                  */
	/************************************************************************/

	///主节点编号常量
	enum {MPI_MASTER = 0};

private:

	/************************************************************************/
	/* 全局变量                                                              */
	/************************************************************************/

	///全局通信域中节点的信息
	static MPIObject * m_MPIObject;

	///全局主节点编号
	static int m_MasterRank;

	///全局初始化时的时间
	static double m_StartTime;

	///MPI初始化函数
	MPIObject(int MPI_argc, char ** MPI_argv);

	/************************************************************************/
	/* 成员变量                                                              */
	/************************************************************************/

	///通信器
	MPI_Comm mpi_Comm;

	///节点名
	char * mpi_Name;

	///节点长度
	int mpi_Len;

	///节点编号
	int mpi_Rank;

	///节点个数
	int mpi_Size;

	///MPI大版本
	int mpi_Version;

	///MPI小版本
	int mpi_SubVersion;
};

template <typename T>
void MPIObject::PrintMe( T val )
{
	std::cout << "NodeName:" << GetNodeName()
	          << ", Rank:" << GetMe()
	          << ", Data:" << val
	          << "." << std::endl;
}

#endif