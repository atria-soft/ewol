/**
 *******************************************************************************
 * @file etk/Matix.h
 * @brief Ewol Tool Kit : generique Matrix type (header)
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

#ifndef __ETK_TYPES_MATRIX_H__
#define __ETK_TYPES_MATRIX_H__

namespace etk {

	class Matrix
	{
		public:
			float m_mat[4*4];
			void Identity(void) {
				for(int32_t iii=0; iii<4*4 ; iii++) {
					m_mat[iii] = 0;
				}
				m_mat[0] = 1.0;
				m_mat[5] = 1.0;
				m_mat[10] = 1.0;
				m_mat[15] = 1.0;
			}
			/*****************************************************
			 *    Constructor
			 *****************************************************/
			Matrix(void) {
				Identity();
			}
			Matrix(const Matrix& obj) {
				for(int32_t iii=0; iii<4*4 ; iii++) {
					m_mat[iii] = obj.m_mat[iii];
				}
			}
			Matrix(float a1, float b1, float c1, float d1,
			       float a2, float b2, float c2, float d2,
			       float a3, float b3, float c3, float d3,
			       float a4, float b4, float c4, float d4) {
				m_mat[0] = a1;
				m_mat[1] = b1;
				m_mat[2] = c1;
				m_mat[3] = d1;
				m_mat[4] = a2;
				m_mat[5] = b2;
				m_mat[6] = c2;
				m_mat[7] = d2;
				m_mat[8] = a3;
				m_mat[9] = b3;
				m_mat[10] = c3;
				m_mat[11] = d3;
				m_mat[12] = a4;
				m_mat[13] = b4;
				m_mat[14] = c4;
				m_mat[15] = d4;
			}
			Matrix(float * obj) {
				if (NULL == obj) {
					Identity();
					return;
				}
				for(int32_t iii=0; iii<4*4 ; iii++) {
					m_mat[iii] = obj[iii];
				}
			}
			/*****************************************************
			 *    Destructor
			 *****************************************************/
			~Matrix() {
				
			}
			/*****************************************************
			 *    = assigment
			 *****************************************************/
			const Matrix& operator= (const Matrix& obj ) {
				for(int32_t iii=0; iii<4*4 ; iii++) {
					m_mat[iii] = obj.m_mat[iii];
				}
				return *this;
			}
			/*****************************************************
			 *    == operator
			 *****************************************************/
			bool  operator== (const Matrix& obj) const {
				for(int32_t iii=0; iii<4*4 ; iii++) {
					if(m_mat[iii] != obj.m_mat[iii]) {
						return false;
					}
				}
				return true;
			}
			/*****************************************************
			 *    != operator
			 *****************************************************/
			bool  operator!= (const Matrix& obj) const {
				for(int32_t iii=0; iii<4*4 ; iii++) {
					if(m_mat[iii] != obj.m_mat[iii]) {
						return true;
					}
				}
				return false;
			}
			/*****************************************************
			 *    += operator
			 *****************************************************/
			const Matrix& operator+= (const Matrix& obj) {
				for(int32_t iii=0; iii<4*4 ; iii++) {
					m_mat[iii] += obj.m_mat[iii];
				}
				return *this;
			}
			/*****************************************************
			 *    + operator
			 *****************************************************/
			Matrix operator+ (const Matrix& obj) {
				Matrix tmpp(*this);
				tmpp += obj;
				return tmpp;
			}
			/*****************************************************
			 *    -= operator
			 *****************************************************/
			const Matrix& operator-= (const Matrix& obj) {
				for(int32_t iii=0; iii<4*4 ; iii++) {
					m_mat[iii] -= obj.m_mat[iii];
				}
				return *this;
			}
			/*****************************************************
			 *    - operator
			 *****************************************************/
			Matrix operator- (const Matrix& obj) {
				Matrix tmpp(*this);
				tmpp += obj;
				return tmpp;
			}
			/*****************************************************
			 *    *= operator
			 *****************************************************/
			const Matrix& operator*= (const Matrix& obj) {
				// output Matrix
				float matrixOut[4*4];
				for(int32_t jjj=0; jjj<4 ; jjj++) {
					float* tmpLeft = m_mat + jjj*4;
					for(int32_t iii=0; iii<4 ; iii++) {
						const float* tmpUpper = obj.m_mat+iii;
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
					m_mat[iii] = matrixOut[iii];
				}
				return *this;
			}
			/*****************************************************
			 *    * operator
			 *****************************************************/
			Matrix operator* (const Matrix& obj) {
				Matrix tmpp(*this);
				tmpp *= obj;
				return tmpp;
			}
	};
	namespace matrix {
		Matrix Perspective(float left, float right, float bottom, float top, float nearVal, float farVal);
		Matrix Translate(float x=0.0, float y=0.0, float z=0.0);
		Matrix Scale(float x=1.0, float y=1.0, float z=1.0);
		Matrix rotate(float x, float y, float z, float angleRad=0.0);
		void Display(Matrix& tmp);
	};
};

#endif
