/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __ETK_TYPES_MATRIX4_H__
#define __ETK_TYPES_MATRIX4_H__

namespace etk {

	class Matrix4
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
			Matrix4(void) {
				Identity();
			}
			Matrix4(const Matrix4& obj) {
				for(int32_t iii=0; iii<4*4 ; iii++) {
					m_mat[iii] = obj.m_mat[iii];
				}
			}
			Matrix4(float a1, float b1, float c1, float d1,
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
			Matrix4(float * obj) {
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
			virtual ~Matrix4(void) {
				
			}
			/*****************************************************
			 *    = assigment
			 *****************************************************/
			const Matrix4& operator= (const Matrix4& obj ) {
				for(int32_t iii=0; iii<4*4 ; iii++) {
					m_mat[iii] = obj.m_mat[iii];
				}
				return *this;
			}
			/*****************************************************
			 *    == operator
			 *****************************************************/
			bool  operator== (const Matrix4& obj) const {
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
			bool  operator!= (const Matrix4& obj) const {
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
			const Matrix4& operator+= (const Matrix4& obj) {
				for(int32_t iii=0; iii<4*4 ; iii++) {
					m_mat[iii] += obj.m_mat[iii];
				}
				return *this;
			}
			/*****************************************************
			 *    + operator
			 *****************************************************/
			Matrix4 operator+ (const Matrix4& obj) {
				Matrix4 tmpp(*this);
				tmpp += obj;
				return tmpp;
			}
			/*****************************************************
			 *    -= operator
			 *****************************************************/
			const Matrix4& operator-= (const Matrix4& obj) {
				for(int32_t iii=0; iii<4*4 ; iii++) {
					m_mat[iii] -= obj.m_mat[iii];
				}
				return *this;
			}
			/*****************************************************
			 *    - operator
			 *****************************************************/
			Matrix4 operator- (const Matrix4& obj) {
				Matrix4 tmpp(*this);
				tmpp += obj;
				return tmpp;
			}
			/*****************************************************
			 *    *= operator
			 *****************************************************/
			const Matrix4& operator*= (const Matrix4& obj) {
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
			Matrix4 operator* (const Matrix4& obj) {
				Matrix4 tmpp(*this);
				tmpp *= obj;
				return tmpp;
			}
			/*****************************************************
			 *  other basic function :
			 *****************************************************/
			void Transpose(void)
			{
				float tmpVal = m_mat[1];
				m_mat[1] = m_mat[4];
				m_mat[4] = tmpVal;
				
				tmpVal = m_mat[2];
				m_mat[2] = m_mat[8];
				m_mat[8] = tmpVal;
				
				tmpVal = m_mat[6];
				m_mat[6] = m_mat[9];
				m_mat[9] = tmpVal;
				
				tmpVal = m_mat[3];
				m_mat[3] = m_mat[12];
				m_mat[12] = tmpVal;
				
				tmpVal = m_mat[7];
				m_mat[7] = m_mat[13];
				m_mat[13] = tmpVal;
				
				tmpVal = m_mat[11];
				m_mat[11] = m_mat[14];
				m_mat[14] = tmpVal;
				
			}
	};
	namespace matrix {
		Matrix4 Perspective(float left, float right, float bottom, float top, float nearVal, float farVal);
		Matrix4 Translate(float x=0.0, float y=0.0, float z=0.0);
		Matrix4 Scale(float x=1.0, float y=1.0, float z=1.0);
		Matrix4 rotate(float x, float y, float z, float angleRad=0.0);
		void Display(Matrix4& tmp);
	};
};

#endif
