/**
 *******************************************************************************
 * @file ewol/openGL/openGL.cpp
 * @brief ewol openGl abstarction (sources)
 * @author Edouard DUPIN
 * @date 19/09/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#include <ewol/Debug.h>
#include <ewol/openGL/openGL.h>


void glOrthoMatrix(GLfloat left,
             GLfloat right,
             GLfloat bottom,
             GLfloat top,
             GLfloat nearVal,
             GLfloat farVal,
             GLfloat* myMatrix)
{
	int iii;
	for(iii=0; iii<4*4 ; iii++) {
		myMatrix[iii] = 0;
	}
	myMatrix[0] = 2.0 / (right - left);
	myMatrix[5] = 2.0 / (top - bottom);
	myMatrix[10] = -2.0 / (farVal - nearVal);
#if 1
	myMatrix[3]  = -1*(right + left) / (right - left);
	myMatrix[7]  = -1*(top + bottom) / (top - bottom);
	myMatrix[11] = -1*(farVal + nearVal) / (farVal - nearVal);
#else
	// test if matrix is not corectly instanciate ...
	myMatrix[12]  = -1*(right + left) / (right - left);
	myMatrix[13]  = -1*(top + bottom) / (top - bottom);
	myMatrix[14] = -1*(farVal + nearVal) / (farVal - nearVal);
#endif
	myMatrix[15] = 1;
}

void glOrthoEwol(GLfloat left,
             GLfloat right,
             GLfloat bottom,
             GLfloat top,
             GLfloat nearVal,
             GLfloat farVal)
{
	GLfloat myMatrix[4*4];
	glOrthoMatrix(left, right, bottom, top, nearVal, farVal, myMatrix);
	glLoadMatrixf(myMatrix);
}


ewol::OglMatrix::OglMatrix(float left, float right, float bottom, float top, float nearVal, float farVal)
{
	Generate(left, right, bottom, top, nearVal, farVal);
}

void ewol::OglMatrix::Generate(float left, float right, float bottom, float top, float nearVal, float farVal)
{
	int iii;
	for(iii=0; iii<4*4 ; iii++) {
		m_Matrix[iii] = 0;
	}
	m_Matrix[0] = 2.0 / (right - left);
	m_Matrix[5] = 2.0 / (top - bottom);
	m_Matrix[10] = -2.0 / (farVal - nearVal);
	m_Matrix[3]  = -1*(right + left) / (right - left);
	m_Matrix[7]  = -1*(top + bottom) / (top - bottom);
	m_Matrix[11] = -1*(farVal + nearVal) / (farVal - nearVal);
	m_Matrix[15] = 1;
}

ewol::OglMatrix::~OglMatrix()
{
	
}
//http://www.siteduzero.com/tutoriel-3-5003-les-matrices.html
static void MultiplyMatrix(float* inOut, float* mult)
{
	
	// output Matrix
	float matrixOut[4*4];
	for(int32_t jjj=0; jjj<4 ; jjj++) {
		float* tmpLeft = inOut + jjj*4;
		for(int32_t iii=0; iii<4 ; iii++) {
			float* tmpUpper = mult+iii;
			float* tmpLeft2 = tmpLeft;
			float tmpElement = 0;
			for(int32_t kkk=0; kkk<4 ; kkk++) {
				tmpElement += *tmpUpper * *tmpLeft2;
				tmpUpper += 4;
				tmpLeft2++;
			}
			matrixOut[jjj*4+iii] = tmpElement;
		}
	}
	// set it at the output
	for(int32_t iii=0; iii<4*4 ; iii++) {
		inOut[iii] = matrixOut[iii];
	}
}

void ewol::OglMatrix::Translate(float x, float y, float z)
{
	float matrix[4*4];
	for(int32_t iii=0; iii<4*4 ; iii++) {
		matrix[iii] = 0;
	}
	// set identity :
	matrix[0] = 1;
	matrix[5] = 1;
	matrix[10] = 1;
	matrix[15] = 1;
	// set translation :
	matrix[3] = x;
	matrix[7] = y;
	matrix[11] = y;
	// generate output :
	MultiplyMatrix(m_Matrix, matrix);
}

void ewol::OglMatrix::Scale(float x, float y, float z)
{
	float matrix[4*4];
	for(int32_t iii=0; iii<4*4 ; iii++) {
		matrix[iii] = 0;
	}
	// set identity :
	matrix[0] = 1;
	matrix[5] = 1;
	matrix[10] = 1;
	matrix[15] = 1;
	// set scale :
	matrix[0] = x;
	matrix[5] = y;
	matrix[10] = z;
	// generate output :
	MultiplyMatrix(m_Matrix, matrix);
}

void ewol::OglMatrix::rotate(float x, float y, float z, float angle)
{
	float matrix[4*4];
	for(int32_t iii=0; iii<4*4 ; iii++) {
		matrix[iii] = 0;
	}
	// set identity :
	matrix[0] = 1;
	matrix[5] = 1;
	matrix[10] = 1;
	matrix[15] = 1;
	// TODO ...
	// generate output :
	MultiplyMatrix(m_Matrix, matrix);
}

