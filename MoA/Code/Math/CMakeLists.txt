# Copyright Linköping University 2010
# Author Stefan Lindholm <stefan.lindholm@liu.se>

if(BUILD_LAB1)
	set(MY_MATH ${MY_MATH}
		Math/ConjugateGradient.h
		Math/ConstantVectorField.h
		Math/Function3D.h
		#Math/Image.h
		#Math/Image_Impl.h
		Math/Interpolator.h
		Math/Matrix4x4.h
		Math/Matrix4x4_Impl.h
		Math/TrilinearInterpolator.cpp
		Math/TrilinearInterpolator.h
		Math/Vector3.h
		Math/Vector4.h
		Math/Volume.h
		Math/VortexVectorField.h
	)
endif(BUILD_LAB1)
