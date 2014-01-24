#include "mpimessage.h"

/************************************************************************/
/* Envelop                                                              */
/************************************************************************/

Envelope & Envelope::operator=( const Envelope & roj )
{
    m_Comm = roj.m_Comm;
    m_ProcessID = roj.m_ProcessID;
    m_Tag = roj.m_Tag;
    return *this;
}

void Envelope::SetComm( MPI_Comm comm )
{
    m_Comm = comm;
}

MPI_Comm Envelope::GetComm() const
{
    return m_Comm;
}

void Envelope::SetProcessId( int processid )
{
    m_ProcessID = processid;
}

int Envelope::GetProcessId() const
{
    return m_ProcessID;
}

void Envelope::SetTag( int tag )
{
    m_Tag = tag;
}

int Envelope::GetTag() const
{
    return m_Tag;
}

Envelope::Envelope() :
m_Comm(MPI_COMM_WORLD),
    m_ProcessID(MPI_ANY_SOURCE),
    m_Tag(MPI_ANY_TAG)
{

}

Envelope::Envelope( MPI_Comm comm, int processid, int tag ) 
    :m_Comm(comm)
    ,m_ProcessID(processid)
    ,m_Tag(tag)
{

}

Envelope::Envelope( const Envelope & envelope ) 
    :m_Comm(envelope.m_Comm)
    ,m_ProcessID(envelope.m_ProcessID)
    ,m_Tag(envelope.m_Tag)
{

}

/************************************************************************/
/* Data                                                                 */
/************************************************************************/

Data & Data::operator=( const Data & roj )
{
    m_Buffer = roj.m_Buffer;
    m_Count = roj.m_Count;
    m_DataType = roj.m_DataType;
    return *this;
}

void Data::SetBuffer( void * buffer )
{
    m_Buffer = buffer;
}

void * Data::GetBuffer() const
{
    return m_Buffer;
}

void Data::SetCount( int count )
{
    m_Count = count;
}

int Data::GetCount() const
{
    return m_Count;
}

void Data::SetDataType( MPI_Datatype datatype )
{
    m_DataType = datatype;
}

MPI_Datatype Data::GetDataType() const
{
    return m_DataType;
}

Data::Data() 
    :m_Buffer(NULL)
    ,m_Count(0)
    ,m_DataType(MPI_INT)
{

}

Data::Data( void * buf, int count, MPI_Datatype datatpye ) 
    :m_Buffer(buf)
    ,m_Count(count)
    ,m_DataType(datatpye)
{

}

Data::Data( const Data & data ) 
    :m_Buffer(data.m_Buffer)
    ,m_Count(data.m_Count)
    ,m_DataType(data.m_DataType)
{

}

Data::~Data()
{
    m_Buffer = NULL;
}

/************************************************************************/
/* message                                                               */
/************************************************************************/

MPIMessage & MPIMessage::operator=( const MPIMessage & roj )
{
    m_Envelope = roj.m_Envelope;
    m_DataBody = roj.m_DataBody;
    return *this;
}

void MPIMessage::SetMessageDataBody( const Data & databody )
{
    m_DataBody = databody;
}

void MPIMessage::SetMessageEnvelope( const Envelope & envelope )
{
    m_Envelope = envelope;
}

void MPIMessage::SetComm( MPI_Comm comm )
{
    m_Envelope.SetComm(comm);
}

MPI_Comm MPIMessage::GetComm() const
{
    return m_Envelope.GetComm();
}

void MPIMessage::SetProcessId( int processid )
{
    m_Envelope.SetProcessId(processid);
}

int MPIMessage::GetProcessId() const
{
    return m_Envelope.GetProcessId();
}

void MPIMessage::SetTag( int tag )
{
    m_Envelope.SetTag(tag);
}

int MPIMessage::GetTag() const
{
    return m_Envelope.GetTag();
}

void MPIMessage::SetBuffer( void * buffer )
{
    m_DataBody.SetBuffer(buffer);
}

void * MPIMessage::GetBuffer() const
{
    return m_DataBody.GetBuffer();
}

void MPIMessage::SetCount( int count )
{
    m_DataBody.SetCount(count);
}

int MPIMessage::GetCount() const
{
    return m_DataBody.GetCount();
}

void MPIMessage::SetDataType( MPI_Datatype datatype )
{
    m_DataBody.SetDataType(datatype);
}

MPI_Datatype MPIMessage::GetDataType() const
{
    return m_DataBody.GetDataType();
}

MPIMessage::MPIMessage( void * buf, int count, MPI_Datatype datatpye, int processid, int tag, MPI_Comm comm ) 
    :m_Envelope(comm, processid, tag)
    ,m_DataBody(buf, count, datatpye)
{

}

MPIMessage::MPIMessage() 
    :m_Envelope()
    ,m_DataBody()
{

}

MPIMessage::MPIMessage( const MPIMessage & msg ) 
    :m_Envelope(msg.m_Envelope)
    ,m_DataBody(msg.m_DataBody)
{

}
