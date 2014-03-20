/**
\file mpimessage.h
\brief MPI消息封装文件

该文件的主要用C++将MPI发送消息的六个数据封装在一起，形成操作方便的类。
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
该类主要封装了通信器，进程编号，标号。确定使用正确的通信器，进程编号，标号。
*/
class Envelope
{
public:
	Envelope();

	Envelope(MPI_Comm comm, int processid, int tag);

	Envelope(const Envelope & envelope);

	~Envelope() {};

	///可以进行赋值。
	Envelope & operator=(const Envelope & roj);

	///设置通信器。
	void SetComm(MPI_Comm comm);

	///返回当前通信器。
	MPI_Comm GetComm() const;

	///设置进程编号。
	void SetProcessId(int processid);

	///返回进程编号。
	int GetProcessId() const;

	///设置当前标签。
	void SetTag(int tag);

	///返回当前标签。
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
该类主要封装了消息地址，消息个数，消息数据类型。
*/
class Data
{
public:
	Data();;

	Data(void * buf, int count, MPI_Datatype datatpye);

	Data(const Data & data);

	~Data();

	///可以进行赋值，进行的是浅拷贝，没有拷贝指针指向的对象。
	Data & operator=(const Data & roj);;

	///设置缓冲区。
	void SetBuffer(void * buffer);

	///返回缓冲区。
	void * GetBuffer() const;

	///设置数据的个数。
	void SetCount(int count);

	///返回数据的个数。
	int GetCount() const;

	///设置数据的类型。
	void SetDataType(MPI_Datatype datatype);

	///返回数据的类型。
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
该类将Envelope和Data组合在一起，形成消息对象。
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

	///设置发送内容
	/** \sa Data */
	void SetMessageDataBody(const Data & databody);

	///设置发送形式
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