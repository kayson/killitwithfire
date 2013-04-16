# Copyright Linköping University 2010
# Author Stefan Lindholm <stefan.lindholm@liu.se>

if(BUILD_LAB5)
	set(LEVELSET ${LEVELSET}
		Levelset/LevelSet.cpp
		Levelset/LevelSet.h
		Levelset/LevelSetGrid.cpp
		Levelset/LevelSetGrid.h
		Levelset/LevelSetOperator.cpp
		Levelset/LevelSetOperator.h
		Levelset/OperatorAdvect.h
		Levelset/OperatorDilateErode.h
		Levelset/OperatorMeanCurvatureFlow.h
		Levelset/OperatorMorph.h
		Levelset/OperatorReinitialize.h
	)
endif(BUILD_LAB5)
