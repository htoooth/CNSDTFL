#include <time.h>

#include "mpiobject.h"
#include "framework.h"
#include "geoalgorithm.cimg2gdal.h"

int test(int argc, char ** argv)
{
	CIMGAlgorihtm cimgalg;
	cimgalg.Test(argc, argv);

	return 1;
}

int main(int MPI_argc, char ** MPI_argv)
{
	/************************************************************************/
	/* init mpi                                                             */
	/************************************************************************/

	MPIObject::CreateMPI(MPI_argc, MPI_argv);

	MPIObject mo;

	if (mo.IsMaster())
	{
		struct tm * tmptr = NULL;
		time_t secnow;
		time(&secnow);
		tmptr = localtime(&secnow);
		printf("[START TIME]%4d/%02d/%02d %02d:%02d:%02d",
		       tmptr->tm_year + 1900, tmptr->tm_mon + 1,
		       tmptr->tm_mday, tmptr->tm_hour,
		       tmptr->tm_min, tmptr->tm_sec);
	}

	/************************************************************************/
	/* run alg                                                              */
	/************************************************************************/

	HPGCFrameWork<HPGAlgorithm<CIMGAlgorihtm>, HPGPartition> algorithm;
	algorithm.Init(MPI_argc, MPI_argv);

	///run master and slave
	algorithm.RunAlgorithmInMasterAndSlave();

	///run peer to peer
	//r2ralg.RunAlgorithmInPeerToPeer();

	///run serial
	//r2ralg.RunAlgorithm();

	/************************************************************************/
	/* record time                                                          */
	/************************************************************************/

	double dfTotalTime = mo.GetTime() - mo.GetInitTime();

	if (mo.IsMaster())
	{
		struct tm * tmptr = NULL;
		time_t secnow;
		time(&secnow);
		tmptr = localtime(&secnow);
		printf("[END TIME]%4d/%02d/%02d %02d:%02d:%02d",
		       tmptr->tm_year + 1900, tmptr->tm_mon + 1,
		       tmptr->tm_mday, tmptr->tm_hour,
		       tmptr->tm_min, tmptr->tm_sec);

		printf("[TOTAL]The Total time is < %f > seconds!", dfTotalTime);
	}

	/************************************************************************/
	/* exit mpi system                                                      */
	/************************************************************************/

	MPIObject::DestoryMPI();

	return 0;
}
