# Copyright Linköping University 2010
# Author Stefan Lindholm <stefan.lindholm@liu.se>

if(BUILD_LAB3)
	set(SUBDIVISION ${SUBDIVISION}
		Subdivision/AdaptiveLoopSubdivisionMesh.cpp
		Subdivision/AdaptiveLoopSubdivisionMesh.h
		Subdivision/LoopSubdivisionMesh.cpp
		Subdivision/LoopSubdivisionMesh.h
		Subdivision/StrangeSubdivisionMesh.h
		Subdivision/Subdivision.h
		Subdivision/UniformCubicSpline.cpp
		Subdivision/UniformCubicSpline.h
		Subdivision/UniformCubicSplineSubdivisionCurve.cpp
		Subdivision/UniformCubicSplineSubdivisionCurve.h
	)
endif(BUILD_LAB3)
