#include "mpimessage.proinfo.h"
#include "port.designpattern.h"

typedef struct _Proinfo
{
	ProcInfo m_Temp;
	MPI_Datatype m_NewMPIProInfo;
	MPI_Datatype m_OldMPIProInfo[3];
	int m_BlockLens[3];
	MPI_Aint m_Displs[3];

	_Proinfo();

} SaveTypeProInfo;

_Proinfo::_Proinfo()
{
	MPI_Get_address(&m_Temp.PID, m_Displs);
	MPI_Get_address(&m_Temp.dfStart, m_Displs + 1);

	m_Displs[2] = sizeof(SaveTypeProInfo);
	m_Displs[1] = m_Displs[1] - m_Displs[0];
	m_Displs[0] = 0;

	m_BlockLens[0] = 3;
	m_BlockLens[1] = 2;
	m_BlockLens[2] = 1;

	m_OldMPIProInfo[0] = MPI_INT;
	m_OldMPIProInfo[1] = MPI_DOUBLE;
	m_OldMPIProInfo[2] = MPI_UB; //数据的上界

	MPI_Type_struct(3, m_BlockLens, m_Displs, m_OldMPIProInfo, &m_NewMPIProInfo);
	MPI_Type_commit(&m_NewMPIProInfo);
}

MPI_Datatype Commit_MPI_Type_ProInfo()
{
	SaveTypeProInfo * poTemp = port::Singleton<SaveTypeProInfo>::CreateInstance();
	return poTemp->m_NewMPIProInfo;
}

void Free_MPI_Type_ProInfo()
{
	SaveTypeProInfo * poTemp = port::Singleton<SaveTypeProInfo>::CreateInstance();
	MPI_Type_free(&poTemp->m_NewMPIProInfo);
	port::Singleton<SaveTypeProInfo>::DestoryInstance();
}
