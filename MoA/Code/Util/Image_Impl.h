
#ifndef _IMAGE_IMPL
#define _IMAGE_IMPL

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


template <typename DataType>
	Image<DataType>::Image()
{
	mWidth = mHeight = mComponents = 0;
	mData = NULL;
	mTexID = 0;
}


template <typename DataType>
	Image<DataType>::Image(GLuint width, GLuint height, GLuint components)
{
	mData = NULL;
	mTexID = 0;
	this->SetDimensions(width, height, components);
}


template <typename DataType>
Image<DataType>::Image(const std::string & filename)
{
	mWidth = mHeight = mComponents = 0;
	mData = NULL;
	mTexID = 0;

	// Try to read the file
	this->ReadFromFile(filename);
}


template <typename DataType>
	Image<DataType>::Image(const Image & img)
{
	mWidth = img.mWidth;
	mHeight = img.mHeight;
	mComponents = img.mComponents;

	mData = NULL;
	mTexID = 0;
	if (img.mData != NULL)
	{
		mData = (DataType *)malloc(mWidth*mHeight*mComponents*sizeof(DataType));
		memcpy(mData, img.mData, mWidth*mHeight*mComponents*sizeof(DataType));
	}
}


template <typename DataType>
	Image<DataType>::~Image()
{
	free(mData);
	mData = NULL;
}


template <typename DataType>
	void Image<DataType>::SetDimensions(GLuint width, GLuint height, GLuint components)
{
	mWidth = width;
	mHeight = height;
	mComponents = components;

	free(mData); mData = NULL;
	mData = (DataType *)calloc(mWidth * mHeight * mComponents, sizeof(DataType));
}


template <typename DataType>
void Image<DataType>::ReadFromFile(const std::string & filename)
{
	GLubyte TGAcomment[1];
	GLubyte TGAheader[] = { 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	GLubyte TGAcompare[11];
	GLubyte header[6];
	int imageSize;

	FILE * file = fopen(filename.c_str(), "rb");

	if (file == NULL ||
		fread(TGAcomment, 1, sizeof(TGAcomment), file) != sizeof(TGAcomment) ||
		fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
		fread(header, 1, sizeof(header), file) != sizeof(header))
	{
		fprintf(stderr, "\nTGA load error.\n");
		fclose(file);
		return;
	}

	char * comment = new char[TGAcomment[0]];
	if(TGAcomment[0] != 0)
	{
		fread(comment, 1, sizeof(comment), file);
		printf("TGA comment: %s\n", comment);
	}
	delete[] comment;

	this->SetDimensions(header[1] * 256 + header[0], header[3] * 256 + header[2], header[4]/8);

	if (mWidth <= 0 || mHeight <= 0 || mComponents < 1 || mComponents > 4)
	{
		fprintf(stderr, "\nTGA load error.\n");
		fclose(file);
		return;
	}

	imageSize = mWidth * mHeight * mComponents;

	GLubyte * data = (GLubyte *)malloc(imageSize);

	if (data == NULL) {
		fprintf(stderr, "\nTGA load error.\n");
		fclose(file);
		return;
	}

	if ((int)fread(data, 1, imageSize, file) != imageSize)
	{
		free(data);
		fprintf(stderr, "\nTGA load error.\n");
		fclose(file);
		return;
	}

	// Copy data to member storage buffer
	// Remember to normalize values
	for (int i = 0; i < imageSize; i++)
		mData[i] = (DataType)data[i] / (DataType)255.0;

	free(data);

	// Flip red and blue components
	if (mComponents > 2)
	{
		for (int i = 0; i < imageSize; i += mComponents)
		{
			DataType tmp = mData[i];
			mData[i] = mData[i+2];
			mData[i+2] = tmp;
		}
	}

	fclose(file);
	printf("TGA file '%s' opened [%d x %d, %i Bpp]\n\n", filename.c_str(), mWidth, mHeight, mComponents);
}


template <typename DataType>
void Image<DataType>::SaveToFile(const std::string & filename)
{
	GLubyte header[] =
	{
		0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		mWidth & 0x00FF, (mWidth & 0xFF00) / 256,
		mHeight & 0x00FF, (mHeight & 0xFF00) / 256,
		mComponents*8,
		0
	};

	GLuint imageSize = mWidth * mHeight * mComponents;

	FILE * file = fopen(filename.c_str(), "wb");

	if (file == NULL)
	{
    std::cerr << "Error when writing to file '" << filename << "'" << std::endl;
		fclose(file);
	}

	GLubyte * data = new GLubyte[imageSize];

	// Copy member storage buffer to byte array
	for (GLuint i = 0; i < imageSize; i++)
		data[i] = (GLubyte)(mData[i]*255.0);

	// Flip red and blue components
	if (mComponents > 2)
	{
		for (GLuint i = 0; i < imageSize; i += mComponents)
		{
			GLubyte tmp = data[i];
			data[i] = data[i+2];
			data[i+2] = tmp;
		}
	}

	fwrite(header, 1, sizeof(header), file);
	fwrite(data, 1, imageSize, file);
	fclose(file);

	delete[] data;

  std::cout << "TGA file written to '" << filename << "'" << std::endl << std::endl;
}


template <typename DataType>
	void Image<DataType>::CaptureScreen()
{
	// Get size of the window
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	this->SetDimensions(viewport[2]+1, viewport[3]+1, 3);

	GLuint imageSize = mWidth * mHeight * mComponents;
	GLubyte * data = new GLubyte[imageSize];

  glFinish();
	glReadPixels(0, 0, mWidth, mHeight, GL_RGB, GL_UNSIGNED_BYTE, data);

	// Copy byte array to member storage buffer
	for (GLuint i = 0; i < imageSize; i++)
		mData[i] = (DataType)data[i] / (DataType)255.0;

  delete[] data;
}


template <typename DataType>
	GLuint Image<DataType>::LoadTexture()
{
	if (mData == NULL) return 0;

	// OpenGL can't handle 2 component images
	GLuint components = (mComponents != 2 ? mComponents : 3);

	// We load the texture as a float image
	GLuint imageSize = mWidth * mHeight * components;
	GLfloat * data = (GLfloat *)calloc(imageSize, sizeof(GLfloat));

	// Copy member storage buffer to byte array
	for (unsigned int y = 0; y < mHeight; y++)
		for (unsigned int x = 0; x < mWidth; x++)
			for (unsigned int c = 0; c < mComponents; c++)
				(data + components*(x + mWidth*y))[c] = (*this)(x,y,c);

	GLuint texID;

	if (glIsTexture(mTexID) == GL_FALSE)
		glGenTextures(1, &mTexID);

	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, mTexID);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, components, mWidth, mHeight, 0,
				(components == 1 ? GL_LUMINANCE : (components == 3 ? GL_RGB : GL_RGBA)), GL_FLOAT, data);

	free(data);

	fprintf(stderr, "Image uploaded to texture memory [ID: %i]\n", mTexID);

	return mTexID;
}


template <typename DataType>
	DataType Image<DataType>::Max(int component)
{
	DataType m = (*this)(0,0,component);
	for (int y = 0; y < mHeight; y++)
		for (int x = 0; x < mWidth; x++)
			if ((*this)(x,y,component) > m)
				m = (*this)(x,y,component);

	return m;
}


template <typename DataType>
	DataType Image<DataType>::Min(int component)
{
	DataType m = (*this)(0,0,component);
	for (int y = 0; y < mHeight; y++)
		for (int x = 0; x < mWidth; x++)
			if ((*this)(x,y,component) < m)
				m = (*this)(x,y,component);

	return m;
}


template <typename DataType>
	Image<DataType> & Image<DataType>::operator = (const Image & img)
{
	if (this == &img) return *this;

	mWidth = img.mWidth;
	mHeight = img.mHeight;
	mComponents = img.mComponents;

	free(mData);
	mData = NULL;

	if (img.mData != NULL)
	{
		mData = (DataType *)malloc(mWidth*mHeight*mComponents*sizeof(DataType));
		memcpy(mData, img.mData, mWidth*mHeight*mComponents*sizeof(DataType));
	}

	return *this;
}

#endif



