# Copyright Linköping University 2010
# Author Stefan Lindholm <stefan.lindholm@liu.se>

project(MoA)
cmake_minimum_required(VERSION 2.6.2)

###
## WX
#
if(WIN32)
	find_path(wxWidgets_ROOT_DIR /include/wx/wx.h ${CMAKE_SOURCE_DIR}/VC++)
endif(WIN32)
FIND_PACKAGE(wxWidgets REQUIRED base core gl)
mark_as_advanced(wxWidgets_LIB_DIR wxWidgets_wxrc_EXECUTABLE)
INCLUDE(${wxWidgets_USE_FILE})

###
## GLUT
#
if(WIN32)
	include_directories(${wxWidgets_ROOT_DIR}/include/GL)
	link_directories(${wxWidgets_ROOT_DIR}/lib)
	# set(MY_GLUT_DIR ${wxWidgets_ROOT_DIR}/lib)
	 # if(BUILD_SHARED_LIBRARIES)
		 # set(GLUT_LIBRARIES optimized ${MY_GLUT_DIR}/freeglut.lib debug ${MY_GLUT_DIR}/freeglutd.lib)
	 # else(BUILD_SHARED_LIBRARIES)
		 # set(GLUT_LIBRARIES optimized ${MY_GLUT_DIR}/freeglut_static.lib debug ${MY_GLUT_DIR}/freeglut_staticd.lib)
	 # endif(BUILD_SHARED_LIBRARIES)
else(WIN32)
	FIND_PACKAGE(GLUT REQUIRED)
endif(WIN32)


#--------------------------------------------------------------------
# Output paths for the executables and libraries
#
if(NOT EXECUTABLE_OUTPUT_PATH)
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE PATH
    "Single output directory for building all executables.")
endif(NOT EXECUTABLE_OUTPUT_PATH)

if(NOT LIBRARY_OUTPUT_PATH)
  set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib CACHE PATH
    "Single output directory for building all libraries.")
endif(NOT LIBRARY_OUTPUT_PATH)

mark_as_advanced(EXECUTABLE_OUTPUT_PATH LIBRARY_OUTPUT_PATH CMAKE_INSTALL_PREFIX)

set(MOA_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include)
set(MOA_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(MOA_BINARY_DIR ${CMAKE_BINARY_DIR})
set(MOA_LIBRARY_DIR ${LIBRARY_OUTPUT_PATH})
set(MOA_EXECUTABLE_DIR ${EXECUTABLE_OUTPUT_PATH})

#--------------------------------------------------------------------
# Boolean values to build the different projects
#
option(BUILD_LAB1 "Build Lab 1 - Mesh" ON)
option(BUILD_LAB2 "Build Lab 2 - Decimation" ON)
option(BUILD_LAB3 "Build Lab 3 - Subdivision" ON)
option(BUILD_LAB4 "Build Lab 4 - Implicits" ON)
option(BUILD_LAB5 "Build Lab 5 - Levelsets" ON)
option(BUILD_LAB6 "Build Lab 6 - Fluids" ON)

#mark_as_advanced(BUILD_LAB2)
#mark_as_advanced(BUILD_LAB3)
#mark_as_advanced(BUILD_LAB4)
#mark_as_advanced(BUILD_LAB5)
#mark_as_advanced(BUILD_LAB6)


if(BUILD_LAB6)
	set(BUILD_LAB1 ON)
	set(BUILD_LAB2 ON)
	set(BUILD_LAB3 ON)
	set(BUILD_LAB4 ON)
	set(BUILD_LAB5 ON)
endif(BUILD_LAB6)

if(BUILD_LAB5)
	set(BUILD_LAB1 ON)
	set(BUILD_LAB2 ON)
	set(BUILD_LAB3 ON)
	set(BUILD_LAB4 ON)
endif(BUILD_LAB5)

if(BUILD_LAB4)
	set(BUILD_LAB1 ON)
	set(BUILD_LAB2 ON)
	set(BUILD_LAB3 ON)
endif(BUILD_LAB4)

if(BUILD_LAB3)
	set(BUILD_LAB1 ON)
	set(BUILD_LAB2 ON)
endif(BUILD_LAB3)

if(BUILD_LAB2)
	set(BUILD_LAB1 ON)
endif(BUILD_LAB2)

###
## Add pre-processor definitions for each lab
#
if(BUILD_LAB1)
	add_definitions(-DLAB1)
endif(BUILD_LAB1)

if(BUILD_LAB2)
	add_definitions(-DLAB2)
endif(BUILD_LAB2)

if(BUILD_LAB3)
	add_definitions(-DLAB3)
endif(BUILD_LAB3)

if(BUILD_LAB4)
	add_definitions(-DLAB4)
endif(BUILD_LAB4)

if(BUILD_LAB5)
	add_definitions(-DLAB5)
endif(BUILD_LAB5)

if(BUILD_LAB6)
	add_definitions(-DLAB6)
endif(BUILD_LAB6)


#--------------------------------------------------------------------
# Build shared libs or static libs
option(BUILD_SHARED_LIBS "Build shared libs or static libs" ON)
mark_as_advanced(BUILD_SHARED_LIBS)

#--------------------------------------------------------------------
# If the generator is VS8, set the compiler to VS8
#
# if(${CMAKE_GENERATOR} MATCHES "Visual Studio 8 2005")
  # if(NOT ${CMAKE_CXX_COMPILER} MATCHES "8")
    # string(REPLACE "9.0" "8" CMAKE_CXX_COMPILER ${CMAKE_CXX_COMPILER})
    # string(REPLACE "9.0" "8" CMAKE_C_COMPILER   ${CMAKE_C_COMPILER})
  # endif(NOT ${CMAKE_CXX_COMPILER} MATCHES "8")
# endif(${CMAKE_GENERATOR} MATCHES "Visual Studio 8 2005")


#--------------------------------------------------------------------
# Disable deprecation warnings for standard C functions
#
# if(CMAKE_COMPILER_2005)
  # add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE)
# endif(CMAKE_COMPILER_2005)

#--------------------------------------------------------------------
# Set preprocessor definition for SbSystem.h to indicate whether
# to use the debug postfix
#
# if(DEBUG_POSTFIX)
	# add_definitions(-D_DEBUG_POSTFIX)
# endif(DEBUG_POSTFIX)

#--------------------------------------------------------------------
# Build modules
#

set(DECIMATION )
set(FLUID )
set(GEOMETRY )
set(GUI )
set(LEVELSET )
set(MY_MATH )
set(SUBDIVISION )
set(UTIL )

include(Decimation/CMakeLists.txt)
include(Fluid/CMakeLists.txt)
include(Geometry/CMakeLists.txt)
include(GUI/CMakeLists.txt)
include(Levelset/CMakeLists.txt)
include(Math/CMakeLists.txt)
include(Subdivision/CMakeLists.txt)
include(Util/CMakeLists.txt)

set(SOURCE
	${DECIMATION}
	${FLUID}
	${GEOMETRY}
	${GUI}
	${LEVELSET}
	${MY_MATH}
	${SUBDIVISION}
	${UTIL}
	main.cpp
)

add_definitions(-DFREEGLUT_STATIC)
add_definitions(-D_USE_MATH_DEFINES)
#add_definitions(-DWINVER=0x0400)
#add_definitions(-DwxUSE_GUI=1)

if(WIN32)
	#Exclude LIBCMT from linkage in MSVC (causes errors)
	set(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:LIBCMT)
endif(WIN32)

source_group("Decimation" FILES ${DECIMATION})
source_group("Fluid" FILES ${FLUID})
source_group("Geometry" FILES ${GEOMETRY})
source_group("GUI" FILES ${GUI} main.cpp)
source_group("Levelset" FILES ${LEVELSET})
source_group("Math" FILES ${MY_MATH})
source_group("Subdivision" FILES ${SUBDIVISION})
source_group("Util" FILES ${UTIL})

include_directories(
	.
	./
)



###############
# APPLICATION #
###############


if(WIN32)

	add_executable(MoA WIN32 ${SOURCE})
	#add_dependencies(MoA )
		#wxvc windows libs
		#target_link_libraries(MoA
	#		comctl32
	#		rpcrt4
	#		winmm
	#		advapi32
	#		wsock32
	#	)
	#endif(HAS_WXVC)

elseif(APPLE)

	add_executable(MoA MACOSX_BUNDLE  ${SOURCE})
	#SET_TARGET_PROPERTIES(MoA PROPERTIES COMPILE_FLAGS 	"-arch i386")

else(WIN32)

	add_executable(MoA ${SOURCE})
	#SET_TARGET_PROPERTIES(MoA PROPERTIES COMPILE_FLAGS 	"-arch i386")

endif(WIN32)

TARGET_LINK_LIBRARIES(MoA ${wxWidgets_LIBRARIES})
TARGET_LINK_LIBRARIES(MoA ${GLUT_LIBRARIES})
