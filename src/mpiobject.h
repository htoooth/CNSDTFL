/**
\file mpiobject.h
\brief MPI�����װ

���ļ�����Ҫ��C++��MPI����ز�����������������
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
	/* ȫ�ֺ���                                                              */
	/************************************************************************/

	///��ʼ��MPI
	static MPIObject * CreateMPI(int MPI_argc, char ** MPI_argv);

	///�˳�MPI
	static void DestoryMPI();

	/************************************************************************/
	/*��Ա����                                                               */
	/************************************************************************/

	///�ı�ͨ�����γ��µ���ͨ����
	MPIObject(MPI_Comm comm);

	///ȫ��ͨ����
	MPIObject();

	///����Ա�������
	~MPIObject();

	/************************************************************************/
	/* ������Ϣ                                                              */
	/************************************************************************/

	///��õ�ǰʱ��
	double GetTime() const;

	///��ó�ʼ��ʱ��
	double GetInitTime() const;

	///��õ�ǰ���̺�
	int GetRank() const;

	///��õ�ǰͨ����Ľ��̸���
	int GetSize() const;

	///��õ�ǰͨ����
	MPI_Comm GetComm() const;

	///��õ�ǰ�ڵ���
	const char * GetNodeName() const;

	///��õ�ǰMPI�İ汾
	int GetVersion() const;

	///��õ�ǰMPI���Ӱ汾
	int GetSubVersion() const;

	///�ڵ�ǰͨ����ͬ��
	void GetBarrier() const;

	/************************************************************************/
	/* �߼�����                                                              */
	/************************************************************************/

	///��õ�ǰʱ��
	/** \sa MPIObject::GetTime()*/
	double GetNow() const;

	///�ж��ǲ���������
	/** \sa MPIObject::GetMaster*/
	bool IsMaster() const;

	///����ǲ��Ǹ�����
	///
	///�����ж���ȫ�����еĸ�����
	/** \sa MPIObject::GetMaster*/
	bool IsRoot() const;

	///�ж��ǲ��Ǵӽ���
	/** \sa MPIObject::GetMaster*/
	bool IsSlave() const;

	///���������̺�
	int GetMaster() const;

	///��õ�ǰ���̺�
	/** \sa MPIObject::GetRank*/
	int GetMe() const;

	///��õ�ǰͨ����Ľ��̸���
	/** \sa MPIObject::GetSize*/
	int GetOurSize() const;

	///��ýڵ���
	/** \sa MPIObject::GetNodeName*/
	const char * GetMyName() const;

	///�ж��ǲ��ǵ�ǰ����
	/** \sa MPIObject::GetRank*/
	bool IsMe(const int rank) const ;

	///��ӡ��ǰ��Ϣ
	void PrintMe() const ;

	///��ӡ��Ϣ
	void Print(const char * str) const;

	///��ӡ������Ϣ
	template <typename T>
	void PrintMe(T val);

	/************************************************************************/
	/* ����                                                                  */
	/************************************************************************/

	///���ڵ��ų���
	enum {MPI_MASTER = 0};

private:

	/************************************************************************/
	/* ȫ�ֱ���                                                              */
	/************************************************************************/

	///ȫ��ͨ�����нڵ����Ϣ
	static MPIObject * m_MPIObject;

	///ȫ�����ڵ���
	static int m_MasterRank;

	///ȫ�ֳ�ʼ��ʱ��ʱ��
	static double m_StartTime;

	///MPI��ʼ������
	MPIObject(int MPI_argc, char ** MPI_argv);

	/************************************************************************/
	/* ��Ա����                                                              */
	/************************************************************************/

	///ͨ����
	MPI_Comm mpi_Comm;

	///�ڵ���
	char * mpi_Name;

	///�ڵ㳤��
	int mpi_Len;

	///�ڵ���
	int mpi_Rank;

	///�ڵ����
	int mpi_Size;

	///MPI��汾
	int mpi_Version;

	///MPIС�汾
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