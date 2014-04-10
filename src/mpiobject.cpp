#include "mpiobject.h"
#include "port.time.h"

double MPIObject::GetTime() const
{
    //return MPI_Wtime();
	return getCPUTime();
}

int MPIObject::GetRank() const
{
    return mpi_Rank;
}

int MPIObject::GetSize() const
{
    return mpi_Size;
}

void MPIObject::GetBarrier() const
{
    MPI_Barrier(mpi_Comm);
}

MPI_Comm MPIObject::GetComm() const
{
    return mpi_Comm;
}

const char * MPIObject::GetNodeName() const
{
    return mpi_Name;
}

int MPIObject::GetVersion() const
{
    return mpi_Version;
}

int MPIObject::GetSubVersion() const
{
    return mpi_SubVersion;
}

MPIObject * MPIObject::CreateMPI( int MPI_argc, char ** MPI_argv )
{
    return m_MPIObject ? m_MPIObject : m_MPIObject = new MPIObject(MPI_argc, MPI_argv);
}

void MPIObject::DestoryMPI()
{
    delete m_MPIObject;
    m_MPIObject = NULL;
    MPI_Finalize();
}

MPIObject::MPIObject( MPI_Comm comm )
{
    mpi_Comm = comm;

    MPI_Comm_rank(mpi_Comm, &mpi_Rank);
    MPI_Comm_size(mpi_Comm, &mpi_Size);

    mpi_Name = m_MPIObject->mpi_Name;
    mpi_Len = m_MPIObject->mpi_Len;
    mpi_Version = m_MPIObject->mpi_Version;
    mpi_SubVersion = m_MPIObject->mpi_SubVersion;
}

MPIObject::MPIObject()
{
    mpi_Comm = m_MPIObject->mpi_Comm;
    mpi_Name = m_MPIObject->mpi_Name;
    mpi_Rank = m_MPIObject->mpi_Rank;
    mpi_Size = m_MPIObject->mpi_Size;
    mpi_Len = m_MPIObject->mpi_Len;
    mpi_Version = m_MPIObject->mpi_Version;
    mpi_SubVersion = m_MPIObject->mpi_SubVersion;
}

MPIObject::MPIObject( int MPI_argc, char ** MPI_argv )
{
    int flag;
    ///是否已经初始化系统
    MPI_Initialized(&flag);
    mpi_Len = MPI_MAX_PROCESSOR_NAME;
    mpi_Name = new char[mpi_Len];
    mpi_Comm = MPI_COMM_WORLD;

    if (flag)
    {
        ///获得当前进程编号
        MPI_Comm_rank(mpi_Comm, &mpi_Rank);
        ///获得当前进程组的大小
        MPI_Comm_size(mpi_Comm, &mpi_Size);
        ///获得节点名
        MPI_Get_processor_name(mpi_Name, &mpi_Len);
        ///获得版本号
        MPI_Get_version(&mpi_Version, &mpi_SubVersion);
    }

    else
    {
        MPI_Init(&MPI_argc, &MPI_argv);
        MPI_Comm_rank(mpi_Comm, &mpi_Rank);
        MPI_Comm_size(mpi_Comm, &mpi_Size);
        MPI_Get_processor_name(mpi_Name, &mpi_Len);
        MPI_Get_version(&mpi_Version, &mpi_SubVersion);
    }

    ///记录系统启动时间
    m_StartTime = GetTime();
}

MPIObject::~MPIObject()
{
    ///什么也不做
}

bool MPIObject::IsMaster() const
{
    if (IsMe(m_MasterRank))
    {
        return true;
    }

    else
    {
        return false;
    }
}

bool MPIObject::IsSlave() const
{
    if (!IsMaster())
    {
        return true;
    }

    else
    {
        return false;
    }
}

int MPIObject::GetMaster() const
{
    return m_MasterRank;
}

int MPIObject::GetMe() const
{
    return GetRank();
}

bool MPIObject::IsMe( const int rank ) const
{
    if (rank == mpi_Rank)
    {
        return true;
    }

    else
    {
        return false;
    }
}

double MPIObject::GetInitTime() const
{
    return m_StartTime;
}

void MPIObject::PrintMe() const
{
    std::cout << "NodeName:" << GetNodeName()
              << ", Rank:" << GetMe()
              << "." << std::endl;
}

double MPIObject::GetNow() const
{
    return GetTime();
}

const char * MPIObject::GetMyName() const
{
    return GetNodeName();
}

int MPIObject::GetOurSize() const
{
    return GetSize();
}

void MPIObject::Print(const char * str) const
{
	std::cout << str << std::endl;
}

/************************************************************************/
/* 初始化全局变量                                                         */
/************************************************************************/

int MPIObject::m_MasterRank = MPI_MASTER;

MPIObject * MPIObject::m_MPIObject = NULL;

double MPIObject::m_StartTime = 0.0;
