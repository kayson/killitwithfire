cmake_minimum_required(VERSION 2.4.0)
project(killitwithfire)

set(CMAKE_BUILD_TYPE "Release")

FILE(GLOB killitwithfire_HEADERS ./*.h* ./advect/*.h* ./levelset/*.h* ./presets/*.h*)
source_group(include FILES ${killitwithfire_HEADERS})

FILE(GLOB killitwithfire_SRCS ./*.cpp ./advect/*.cpp ./levelset/*.cpp ./presets/*.cpp)
source_group(source FILES ${killitwithfire_SRCS})

include_directories(	${CMAKE_SOURCE_DIR}/../externals
			${CMAKE_SOURCE_DIR}/../externals/glm
			${CMAKE_SOURCE_DIR}/../externals/GL
			${CMAKE_SOURCE_DIR}/../externals/GL/GL
			${CMAKE_SOURCE_DIR}/../externals/FreeImage		
		)

link_directories(	${CMAKE_SOURCE_DIR}/../externals/GL/lib
		) 

add_executable(killitwithfire ${killitwithfire_HEADERS} ${killitwithfire_SRCS})

target_link_libraries(killitwithfire freeglut.lib GLFW.lib)





