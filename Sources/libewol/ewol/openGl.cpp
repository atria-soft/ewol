/**
 *******************************************************************************
 * @file ewol/openGl.cpp
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

#include <ewol/openGl.h>


void glOrthoEwol(GLfloat left,
             GLfloat right,
             GLfloat bottom,
             GLfloat top,
             GLfloat nearVal,
             GLfloat farVal)
{
	GLfloat myMatrix[4*4];
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

	glLoadMatrixf(myMatrix);


}
