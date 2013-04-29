# Copyright Linköping University 2010
# Author Stefan Lindholm <stefan.lindholm@liu.se>

if(BUILD_LAB1)
	set(GUI ${GUI}
		GUI/FrameMain.cpp
		GUI/FrameMain.h
		GUI/GLAxis.cpp
		GUI/GLAxis.h
		GUI/GLCamera.h
		GUI/GLGridPlane.cpp
		GUI/GLGridPlane.h
		GUI/GLObject.cpp
		GUI/GLObject.h
		GUI/GLViewer.cpp
		GUI/GLViewer.h
		GUI/GUI.cpp
		GUI/GUI.h
	)
endif(BUILD_LAB1)

if(BUILD_LAB4)
	set(GUI ${GUI}
		GUI/ScalarCutPlane.cpp
		GUI/ScalarCutPlane.h
		GUI/VectorCutPlane.cpp
		GUI/VectorCutPlane.h
	)
endif(BUILD_LAB4)

if(BUILD_LAB6)
	set(GUI ${GUI}
		GUI/FluidVoxelCutPlane.cpp
		GUI/FluidVoxelCutPlane.h
	)
endif(BUILD_LAB6)
