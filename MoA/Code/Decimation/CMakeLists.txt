# Copyright Linköping University 2010
# Author Stefan Lindholm <stefan.lindholm@liu.se>

if(BUILD_LAB2)
	set(DECIMATION ${DECIMATION}
		Decimation/DecimationInterface.h
		Decimation/DecimationMesh.cpp
		Decimation/DecimationMesh.h
		Decimation/QuadricDecimationMesh.cpp
		Decimation/QuadricDecimationMesh.h
		Decimation/SimpleDecimationMesh.cpp
		Decimation/SimpleDecimationMesh.h
	)
endif(BUILD_LAB2)
