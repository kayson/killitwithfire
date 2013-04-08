cmake_minimum_required(VERSION 2.4.0)
project(killitwithfire)

## Fixa openMP-flagga ################################################

IF (WIN32)
  IF(MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
  ENDIF(MSVC)
ENDIF (WIN32)

IF(UNIX)
  MESSAGE(STATUS "La till c++11!")
  SET(CMAKE_CXX_FLAGS "-std=c++11")
ENDIF(UNIX)



## Fixa alla headers ##################################################

FILE(GLOB killitwithfire_HEADERS  ./*.h*)

FILE(GLOB killitwithfire_ADVECT_HEADERS   ./advect/*.h*)

FILE(GLOB killitwithfire_DISCRETIZAITON_HEADERS ./discretization/*.h*)

FILE(GLOB killitwithfire_EXTRAPOLATION_HEADERS ./extrapolation/*.h*)

FILE(GLOB killitwithfire_LEVELSET_HEADERS   ./levelset/*.h*)

FILE(GLOB killitwithfire_PRESETS_HEADERS   ./presets/*.h*)

FILE(GLOB killitwithfire_PRESSURE_HEADERS   ./Pressure/*.h*)

FILE(GLOB killitwithfire_PRESSURE_PCGSOLVER_HEADERS   ./Pressure/pcgsolver/*.h*)

FILE(GLOB killitwithfire_WATER_HEADERS   ./Water/*.h*)



list(APPEND killitwithfire_ALL_HEADERS 

  ${killitwithfire_HEADERS}

  ${killitwithfire_ADVECT_HEADERS}

  ${killitwithfire_DISCRETIZAITON_HEADERS}

  ${killitwithfire_EXTRAPOLATION_HEADERS}

  ${killitwithfire_LEVELSET_HEADERS}

  ${killitwithfire_PRESETS_HEADERS}

  ${killitwithfire_PRESSURE_HEADERS}

  ${killitwithfire_PRESSURE_PCGSOLVER_HEADERS}

  ${killitwithfire_WATER_HEADERS}

  )



source_group("include" FILES ${killitwithfire_HEADERS})

source_group("include\\Advect" FILES ${killitwithfire_ADVECT_HEADERS})

source_group("include\\Discretization" FILES ${killitwithfire_DISCRETIZAITON_HEADERS})

source_group("include\\Extrapolation" FILES ${killitwithfire_EXTRAPOLATION_HEADERS})

source_group("include\\Levelset" FILES ${killitwithfire_LEVELSET_HEADERS})

source_group("include\\Presets" FILES ${killitwithfire_PRESETS_HEADERS})

source_group("include\\Pressure" FILES ${killitwithfire_PRESSURE_HEADERS})

source_group("include\\Pressure\\pcgsolver" FILES ${killitwithfire_PRESSURE_PCGSOLVER_HEADERS})

source_group("include\\Water" FILES ${killitwithfire_WATER_HEADERS})



## Fixa alla source-filer ################################################



FILE(GLOB killitwithfire_SRCS  ./*.cpp)

FILE(GLOB killitwithfire_ADVECT_SRCS   ./advect/*.cpp)

FILE(GLOB killitwithfire_DISCRETIZAITON_SRCS ./discretization/*.cpp)

FILE(GLOB killitwithfire_EXTRAPOLATION_SRCS ./extrapolation/*.cpp)

FILE(GLOB killitwithfire_LEVELSET_SRCS   ./levelset/*.cpp)

FILE(GLOB killitwithfire_PRESETS_SRCS   ./presets/*.cpp)

FILE(GLOB killitwithfire_PRESSURE_SRCS   ./Pressure/*.cpp)

FILE(GLOB killitwithfire_PRESSURE_PCGSOLVER_SRCS   ./Pressure/pcgsolver/*.cpp)

FILE(GLOB killitwithfire_WATER_SRCS   ./Water/*.cpp)



list(APPEND killitwithfire_ALL_SRCS 

  ${killitwithfire_SRCS}

  ${killitwithfire_ADVECT_SRCS}

  ${killitwithfire_DISCRETIZAITON_SRCS}

  ${killitwithfire_EXTRAPOLATION_SRCS}

  ${killitwithfire_LEVELSET_SRCS}

  ${killitwithfire_PRESETS_SRCS}

  ${killitwithfire_PRESSURE_SRCS}

  ${killitwithfire_PRESSURE_PCGSOLVER_SRCS}

  ${killitwithfire_WATER_SRCS}

  )



source_group("source" FILES ${killitwithfire_SRCS})
source_group("source\\Advect" FILES ${killitwithfire_ADVECT_SRCS})
source_group("source\\Discretization" FILES ${killitwithfire_DISCRETIZAITON_SRCS})
source_group("source\\Extrapolation" FILES ${killitwithfire_EXTRAPOLATION_SRCS})
source_group("source\\Levelset" FILES ${killitwithfire_LEVELSET_SRCS})
source_group("source\\Presets" FILES ${killitwithfire_PRESETS_SRCS})
source_group("source\\Pressure" FILES ${killitwithfire_PRESSURE_SRCS})
source_group("source\\Pressure\\pcgsolver" FILES ${killitwithfire_PRESSURE_PCGSOLVER_SRCS})
source_group("source\\Water" FILES ${killitwithfire_WATER_SRCS})

## Include dirs for att slippa lankar mellan mappar ###################

list(APPEND killitwithfire_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/advect
  ${CMAKE_CURRENT_SOURCE_DIR}/discretization
  ${CMAKE_CURRENT_SOURCE_DIR}/extrapolation
  ${CMAKE_CURRENT_SOURCE_DIR}/levelset
  ${CMAKE_CURRENT_SOURCE_DIR}/presets  
  ${CMAKE_CURRENT_SOURCE_DIR}/Water
  ${CMAKE_CURRENT_SOURCE_DIR}/Pressure
  ${CMAKE_CURRENT_SOURCE_DIR}/Pressure/pcgsolver
  )


## Fixar lankar till bibliotek ########################################

list(APPEND killitwithfire_INCLUDE_DIRS
  ${CMAKE_SOURCE_DIR}/../externals
  ${CMAKE_SOURCE_DIR}/../externals/glm
  ${CMAKE_SOURCE_DIR}/../externals/GL
  ${CMAKE_SOURCE_DIR}/../externals/GL/GL
  ${CMAKE_SOURCE_DIR}/../externals/FreeImage
  )

## Lagger till dem ################################################

include_directories(	
  ${killitwithfire_INCLUDE_DIRS}
  )

#################################################################

link_directories(
  ${CMAKE_SOURCE_DIR}/../externals/GL/lib
  ) 
add_executable(killitwithfire ${killitwithfire_ALL_HEADERS} ${killitwithfire_ALL_SRCS})

if(WIN32)
  target_link_libraries(killitwithfire freeglut.lib GLFW.lib)
endif(WIN32)

if(UNIX)
  target_link_libraries(killitwithfire GL glut glfw GLU)
endif(UNIX)
