/**
 *******************************************************************************
 * @file etk/Matix.cpp
 * @brief Ewol Tool Kit : generique Matrix type (Sources)
 * @author Edouard DUPIN
 * @date 29/08/2012
 * @par Project
 * Ewol TK
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



#include <etk/Types.h>
#include <etk/Matrix.h>
#include <etk/DebugInternal.h>
#include <math.h>

etk::Matrix etk::matrix::Perspective(float left, float right, float bottom, float top, float nearVal, float farVal)
{
	etk::Matrix tmp;
	for(int32_t iii=0; iii<4*4 ; iii++) {
		tmp.m_mat[iii] = 0;
	}
	tmp.m_mat[0] = 2.0 / (right - left);
	tmp.m_mat[5] = 2.0 / (top - bottom);
	tmp.m_mat[10] = -2.0 / (farVal - nearVal);
	tmp.m_mat[3]  = -1*(right + left) / (right - left);
	tmp.m_mat[7]  = -1*(top + bottom) / (top - bottom);
	tmp.m_mat[11] = -1*(farVal + nearVal) / (farVal - nearVal);
	tmp.m_mat[15] = 1;
	//TK_INFO("Perspective :");
	//etk::matrix::Display(tmp);
	return tmp;
}

etk::Matrix etk::matrix::Translate(float x, float y, float z)
{
	etk::Matrix tmp;
	// set translation :
	tmp.m_mat[3] = x;
	tmp.m_mat[7] = y;
	tmp.m_mat[11] = z;
	//TK_INFO("Translate :");
	//etk::matrix::Display(tmp);
	return tmp;
}

etk::Matrix etk::matrix::Scale(float x, float y, float z)
{
	etk::Matrix tmp;
	// set scale :
	tmp.m_mat[0] = x;
	tmp.m_mat[5] = y;
	tmp.m_mat[10] = z;
	//TK_INFO("Scale :");
	//etk::matrix::Display(tmp);
	return tmp;
}

etk::Matrix etk::matrix::rotate(float x, float y, float z, float angleRad)
{
	etk::Matrix tmp;
	float cosVal = cos(angleRad);
	float sinVal = sin(angleRad);
	float invVal = 1.0-cosVal;
	// set rotation : 
	tmp.m_mat[0]  = x*x*invVal + cosVal;
	tmp.m_mat[1]  = x*y*invVal - z*cosVal;
	tmp.m_mat[2]  = x*z*invVal + y*sinVal;
	
	tmp.m_mat[4]  = y*x*invVal + z*sinVal;
	tmp.m_mat[5]  = y*y*invVal + cosVal;
	tmp.m_mat[6]  = y*z*invVal - x*sinVal;
	
	tmp.m_mat[8]  = z*x*invVal - y*sinVal;
	tmp.m_mat[9]  = z*y*invVal + x*sinVal;
	tmp.m_mat[10] = z*z*invVal + cosVal;
	return tmp;
}


void etk::matrix::Display(etk::Matrix& tmp)
{
	TK_INFO("matrix : (" << tmp.m_mat[0] << " , " << tmp.m_mat[1] << " , " << tmp.m_mat[2] << " , " << tmp.m_mat[3] << " , ");
	TK_INFO("          " << tmp.m_mat[4] << " , " << tmp.m_mat[5] << " , " << tmp.m_mat[6] << " , " << tmp.m_mat[7] << " , ");
	TK_INFO("          " << tmp.m_mat[8] << " , " << tmp.m_mat[9] << " , " << tmp.m_mat[10] << " , " << tmp.m_mat[11] << " , ");
	TK_INFO("          " << tmp.m_mat[12] << " , " << tmp.m_mat[13] << " , " << tmp.m_mat[14] << " , " << tmp.m_mat[15] << " )");
}
