#include "mpimessage.datblock.h"
#include "port.designpattern.h"
#include "port.datblock.h"

using port::DatBlock;

typedef struct _datblock
{
	DatBlock m_Temp;
	MPI_Datatype m_NewMPIProInfo;
	MPI_Datatype m_OldMPIProInfo[4];
	int m_BlockLens[4];
	MPI_Aint m_Displs[4];

	_datblock();

} SaveTypeDatBlock;

_datblock::_datblock()
{
	MPI_Get_address(&m_Temp.DID, m_Displs);
	MPI_Get_address(&m_Temp.PID, m_Displs + 1);
	MPI_Get_address(&m_Temp.tStart, m_Displs + 2);

	m_Displs[3] = sizeof(SaveTypeDatBlock);
	m_Displs[2] = m_Displs[2] - m_Displs[1];
	m_Displs[1] = m_Displs[1] - m_Displs[0];
	m_Displs[0] = 0;

	m_BlockLens[0] = 6;
	m_BlockLens[1] = 2;
	m_BlockLens[2] = 2;
	m_BlockLens[3] = 1;

	m_OldMPIProInfo[0] = MPI_LONG;
	m_OldMPIProInfo[1] = MPI_INT;
	m_OldMPIProInfo[2] = MPI_DOUBLE;
	m_OldMPIProInfo[3] = MPI_UB;

	MPI_Type_struct(4, m_BlockLens, m_Displs, m_OldMPIProInfo, &m_NewMPIProInfo);
	MPI_Type_commit(&m_NewMPIProInfo);
}


MPI_Datatype Commit_MPI_Type_DatBlock()
{
	SaveTypeDatBlock * pTemp = port::Singleton<SaveTypeDatBlock>::CreateInstance();
	return pTemp->m_NewMPIProInfo;
}

void Free_MPI_Type_DatBlock()
{
	SaveTypeDatBlock * pTemp = port::Singleton<SaveTypeDatBlock>::CreateInstance();
	MPI_Type_free(&pTemp->m_NewMPIProInfo);
	port::Singleton<SaveTypeDatBlock>::DestoryInstance();
}
