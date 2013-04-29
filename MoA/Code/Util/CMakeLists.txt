# Copyright Linköping University 2010
# Author Stefan Lindholm <stefan.lindholm@liu.se>

if(BUILD_LAB1)
	set(UTIL ${UTIL}
		Util/BitMask3D.h
		Util/BlackWhiteColorMap.cpp
		Util/BlackWhiteColorMap.h
		Util/ColorMap.cpp
		Util/ColorMap.h
		Util/ColorMapFactory.cpp
		Util/ColorMapFactory.h
		#Util/glext.h
		Util/GLObjectPlayback.cpp
		Util/GLObjectPlayback.h
		Util/GreenRedColorMap.cpp
		Util/GreenRedColorMap.h
		Util/Heap.cpp
		Util/Heap.h
		Util/HotColorMap.cpp
		Util/HotColorMap.h
		Util/HSVColorMap.cpp
		Util/HSVColorMap.h
		Util/IsoContourColorMap.cpp
		Util/IsoContourColorMap.h
		Util/JetColorMap.cpp
		Util/JetColorMap.h
		Util/MarchingCubes.cpp
		Util/MarchingCubes.h
		Util/MarchingCubesTable.h
		Util/ObjIO.cpp
		Util/ObjIO.h
		Util/Stopwatch.h
		Util/trackball.cpp
		Util/trackball.h
		Util/Util.cpp
		Util/Util.h
	)
	if(WIN32)
		set(UTIL ${UTIL}
			Util/Console.cpp
			Util/Console.h
		)
	endif(WIN32)
endif(BUILD_LAB1)
