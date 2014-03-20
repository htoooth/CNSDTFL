/**
\file mpimessage.h
\brief MPI��Ϣ��װ�ļ�

���ļ�����Ҫ��C++��MPI������Ϣ���������ݷ�װ��һ���γɲ���������ࡣ
\author htoo
\version 0.0.0.1
\date 2013-02-18
*/

#ifndef MPI_MSG_H_INCLUDE

#define MPI_MSG_H_INCLUDE

#include <mpi.h>

#ifndef NULL
#define NULL 0
#endif
/************************************************************************/
/*                       Envelope                                       */
/************************************************************************/
/**
������Ҫ��װ��ͨ���������̱�ţ���š�ȷ��ʹ����ȷ��ͨ���������̱�ţ���š�
*/
class Envelope
{
public:
	Envelope();

	Envelope(MPI_Comm comm, int processid, int tag);

	Envelope(const Envelope & envelope);

	~Envelope() {};

	///���Խ��и�ֵ��
	Envelope & operator=(const Envelope & roj);

	///����ͨ������
	void SetComm(MPI_Comm comm);

	///���ص�ǰͨ������
	MPI_Comm GetComm() const;

	///���ý��̱�š�
	void SetProcessId(int processid);

	///���ؽ��̱�š�
	int GetProcessId() const;

	///���õ�ǰ��ǩ��
	void SetTag(int tag);

	///���ص�ǰ��ǩ��
	int GetTag() const;;

private:
	MPI_Comm m_Comm;
	int m_ProcessID;
	int m_Tag;
};

/************************************************************************/
/*                           Data                                       */
/************************************************************************/
/**
������Ҫ��װ����Ϣ��ַ����Ϣ��������Ϣ�������͡�
*/
class Data
{
public:
	Data();;

	Data(void * buf, int count, MPI_Datatype datatpye);

	Data(const Data & data);

	~Data();

	///���Խ��и�ֵ�����е���ǳ������û�п���ָ��ָ��Ķ���
	Data & operator=(const Data & roj);;

	///���û�������
	void SetBuffer(void * buffer);

	///���ػ�������
	void * GetBuffer() const;

	///�������ݵĸ�����
	void SetCount(int count);

	///�������ݵĸ�����
	int GetCount() const;

	///�������ݵ����͡�
	void SetDataType(MPI_Datatype datatype);

	///�������ݵ����͡�
	MPI_Datatype GetDataType() const;

private:
	void * m_Buffer;
	int m_Count;
	MPI_Datatype m_DataType;
};

/************************************************************************/
/*                          MPIMesage                                   */
/************************************************************************/
/**
���ཫEnvelope��Data�����һ���γ���Ϣ����
\sa Envelope,Data
*/
class MPIMessage
{
public:
	MPIMessage();

	MPIMessage(void * buf, int count, MPI_Datatype datatpye,
	           int processid, int tag, MPI_Comm comm);

	MPIMessage(const MPIMessage & msg);

	~MPIMessage() {};

	MPIMessage & operator=(const MPIMessage & roj);

	///���÷�������
	/** \sa Data */
	void SetMessageDataBody(const Data & databody);

	///���÷�����ʽ
	/** \sa Envelope */
	void SetMessageEnvelope(const Envelope & envelope);

	/** \sa Envelope::SetComm*/
	void SetComm(MPI_Comm comm);

	/** \sa Envelope::GetComm*/
	MPI_Comm GetComm() const;

	/** \sa Envelope::SetProcessId*/
	void SetProcessId(int processid);

	/** \sa Envelope::GetProcessId*/
	int GetProcessId() const;

	/** \sa Envelope::SetTag*/
	void SetTag(int tag);

	/** \sa Envelope::GetTag*/
	int GetTag() const;

	/** \sa Data::SetBuffer*/
	void SetBuffer(void * buffer);

	/** \sa Data::GetBuffer*/
	void * GetBuffer() const;

	/** \sa Data::SetCount*/
	void SetCount(int count);

	/** \sa Data::GetCount*/
	int GetCount() const;

	/** \sa Data::SetDataType*/
	void SetDataType(MPI_Datatype datatype);

	/** \sa Data::GetDataType*/
	MPI_Datatype GetDataType() const;

private:
	Envelope m_Envelope;
	Data m_DataBody;
};

#endif