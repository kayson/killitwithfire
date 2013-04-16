# Copyright Linköping University 2010
# Author Stefan Lindholm <stefan.lindholm@liu.se>

if(BUILD_LAB1)
	set(GEOMETRY ${GEOMETRY}
		Geometry/Geometry.h
		Geometry/HalfEdgeMesh.cpp
		Geometry/HalfEdgeMesh.h
		Geometry/Mesh.cpp
		Geometry/Mesh.h
		Geometry/SimpleMesh.cpp
		Geometry/SimpleMesh.h
	)
endif(BUILD_LAB1)

if(BUILD_LAB3)
	set(GEOMETRY ${GEOMETRY}
		Geometry/LineStrip.cpp
		Geometry/LineStrip.h
	)
endif(BUILD_LAB3)

if(BUILD_LAB4)
	set(GEOMETRY ${GEOMETRY}
		Geometry/Bbox.h
		Geometry/CSG.h
		Geometry/Cube.cpp
		Geometry/Cube.h
		Geometry/Implicit.cpp
		Geometry/Implicit.h
		Geometry/ImplicitGradientField.h
		Geometry/ImplicitMesh.cpp
		Geometry/ImplicitMesh.h
		Geometry/ImplicitValueField.h
		Geometry/Quadric.cpp
		Geometry/Quadric.h
		Geometry/SignedDistanceSphere.cpp
		Geometry/SignedDistanceSphere.h
		Geometry/Sphere.cpp
		Geometry/Sphere.h
		Geometry/SphereFractal.cpp
		Geometry/SphereFractal.h
	)
endif(BUILD_LAB4)
