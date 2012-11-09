/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __ETK_TYPES_MATRIX_H__
#define __ETK_TYPES_MATRIX_H__

//#include <etk/DebugInternal.h>
#include <etk/math/Vector2D.h>
#include <etk/Vector.h>

namespace etk
{
	template <typename T> class Matrix
	{
		private:
			etk::Vector2D<int32_t> m_size;
			etk::Vector<T>         m_data;
		public:
			/*****************************************************
			 *    Constructor
			 *****************************************************/
			Matrix(Vector2D<int32_t> size, T* defaultVal=NULL) :
				m_size(size),
				etk::Vector2D<T>(size.x* size.y)
			{
				if (NULL != defaultVal) {
					// copy all the elements
					for(int32_t iii=0; iii<=m_size.x*m_size.y; iii++) {
						// cast and set value : 
						m_data[iii] = (T)defaultVal++;
					}
				} else {
					Clear();
				}
			};
			Matrix(int32_t width=0, int32_t heigh=0, T* defaultVal=NULL) :
				m_size(width, heigh),
				etk::Vector2D<T>(width*heigh)
			{
				if (NULL != defaultVal) {
					// copy all the elements
					for(int32_t iii=0; iii<=m_size.x*m_size.y; iii++) {
						// cast and set value : 
						m_data[iii] = (T)defaultVal++;
					}
				} else {
					Clear();
				}
			};
			Matrix(const Matrix<double>& obj) :
				m_size(obj.m_size.x, obj.m_size.y),
				etk::Vector2D<T>(obj.m_size.x* obj.m_size.y)
			{
				// copy all the elements
				for(int32_t iii=0; iii<=m_size.x*m_size.y; iii++) {
					// cast and set value : 
					m_data[iii] = (T)obj.m_data[iii];
				}
			};
			Matrix(const Matrix<float>& obj) :
				m_size(obj.m_size.x, obj.m_size.y),
				etk::Vector2D<T>(obj.m_size.x* obj.m_size.y)
			{
				// copy all the elements
				for(int32_t iii=0; iii<=m_size.x*m_size.y; iii++) {
					// cast and set value : 
					m_data[iii] = (T)obj.m_data[iii];
				}
			};
			Matrix(const Matrix<int32_t>& obj) :
				m_size(obj.m_size.x, obj.m_size.y),
				etk::Vector2D<T>(obj.m_size.x* obj.m_size.y)
			{
				// copy all the elements
				for(int32_t iii=0; iii<=m_size.x*m_size.y; iii++) {
					// cast and set value : 
					m_data[iii] = (T)obj.m_data[iii];
				}
			};
			/*****************************************************
			 *    Destructor
			 *****************************************************/
			virtual ~Matrix(void) {};
			
			/*****************************************************
			 *    = assigment
			 *****************************************************/
			const Matrix<T>& operator= (const Matrix<T>& obj )
			{
				// check if it was the same pointer
				if( this == &obj ) {
					return *this;
				}
				// copy data :
				m_size = obj.m_size;
				m_data = obj.m_data;
				return *this;
			};
			/*****************************************************
			 *    == operator
			 *****************************************************/
			bool  operator== (const Matrix<T>& obj) const
			{
				return (m_data == obj.m_data);
			};
			/*****************************************************
			 *    != operator
			 *****************************************************/
			bool  operator!= (const Matrix<T>& obj) const
			{
				return (m_data != obj.m_data);
			};
			/*****************************************************
			 *    += operator
			 *****************************************************/
			const Matrix<T>& operator+= (const Matrix<T>& obj)
			{
				if (m_size != obj.m_size) {
					//TK_CRITICAL("add 2 Matrix with différent size ... ==> generate the max size of all the 2 matrix");
					etk::Matrix<T> tmpMatrix(etk_max(m_size.x,obj.m_size.x), etk_max(m_size.y,obj.m_size.y));
					for (int32_t jjj=0; jjj< m_size.y; jjj++) {
						T* tmpPointer = tmpMatrix[jjj];
						T* tmpPointerIn = (*this)[jjj];
						for (int32_t iii=0; iii< m_size.x; iii++) {
							tmpPointer[iii] = tmpPointerIn[iii];
						}
					}
					for (int32_t jjj=0; jjj< obj.m_size.y; jjj++) {
						T* tmpPointer = tmpMatrix[jjj];
						T* tmpPointerIn = obj[jjj];
						for (int32_t iii=0; iii< obj.m_size.x; iii++) {
							tmpPointer[iii] += tmpPointerIn[iii];
						}
					}
					// copy in local :
					m_size = tmpMatrix.m_size;
					m_data = tmpMatrix.m_data;
				} else {
					// copy data for the same size :
					for (int32_t iii=0; iii< m_data.Size(); iii++) {
						m_data[iii] += obj.m_data[iii];
					}
				}
				return *this;
			};
			/*****************************************************
			 *    + operator
			 *****************************************************/
			Matrix<T> operator+ (const Matrix<T>& obj) {
				Matrix<T> tmpp(*this);
				tmpp += obj;
				return tmpp;
			}
			/*****************************************************
			 *    -= operator
			 *****************************************************/
			const Matrix<T>& operator-= (const Matrix<T>& obj)
			{
				if (m_size != obj.m_size) {
					//TK_CRITICAL("less 2 Matrix with diffÃ©rent size ... ==> generate the max size of all the 2 matrix");
					etk::Matrix<T> tmpMatrix(etk_max(m_size.x,obj.m_size.x), etk_max(m_size.y,obj.m_size.y));
					for (int32_t jjj=0; jjj< m_size.y; jjj++) {
						T* tmpPointer = tmpMatrix[jjj];
						T* tmpPointerIn = (*this)[jjj];
						for (int32_t iii=0; iii< m_size.x; iii++) {
							tmpPointer[iii] = tmpPointerIn[iii];
						}
					}
					for (int32_t jjj=0; jjj< obj.m_size.y; jjj++) {
						T* tmpPointer = tmpMatrix[jjj];
						T* tmpPointerIn = obj[jjj];
						for (int32_t iii=0; iii< obj.m_size.x; iii++) {
							tmpPointer[iii] -= tmpPointerIn[iii];
						}
					}
					// copy in local :
					m_size = tmpMatrix.m_size;
					m_data = tmpMatrix.m_data;
				} else {
					// copy data for the same size :
					for (int32_t iii=0; iii< m_data.Size(); iii++) {
						m_data[iii] -= obj.m_data[iii];
					}
				}
				return *this;
			};
			/*****************************************************
			 *    - operator
			 *****************************************************/
			Matrix<T> operator- (const Matrix<T>& obj) {
				Matrix<T> tmpp(*this);
				tmpp += obj;
				return tmpp;
			}
			/*****************************************************
			 *    *= operator
			 *****************************************************/
			const Matrix<T>& operator*= (const Matrix<T>& obj)
			{
				if(    m_size.x != obj.m_size.y
				    || m_size.y != obj.m_size.x) {
					//TK_CRITICAL("Error while multipliying 2 matrix with different size ==> impossible case ...");
					return *this;
				}
				etk::Matrix<T> tmpMatrix(m_size);
				for (int32_t jjj=0; jjj< obj.m_size.y; jjj++) {
					for (int32_t iii=0; iii< obj.m_size.x; iii++) {
						T tmpVal = 0;
						for (int32_t kkk=0; kkk< obj.m_size.x; kkk++) {
							tmpVal += (*this)[jjj][iii+kkk] * obj[jjj+kkk][iii];
						}
						tmpMatrix[jjj][iii] = tmpVal;
					}
				}
				// copy in local :
				m_data = tmpMatrix.m_data;
				return *this;
			};
			/*****************************************************
			 *    * operator
			 *****************************************************/
			Matrix<T> operator* (const Matrix<T>& obj) {
				Matrix tmpp(*this);
				tmpp *= obj;
				return tmpp;
			}
			/*****************************************************
			 *    [] operator
			 *****************************************************/
			const T* operator[] (int32_t line) const {
				return &m_data[line*m_size.x];
			}
			T* operator[] (int32_t line) {
				return &m_data[line*m_size.x];
			}
			/*****************************************************
			 *    Other mathematical function
			 *****************************************************/
			/**
			 * @ brief Transpose Matrix
			 * @ return the transpose matrix
			 */
			Matrix<T> Transpose(void)
			{
				// create a matrix with the inverted size
				Matrix<T> tmpMatrix(m_size.x, m_size.y);
				for (int32_t jjj=0; jjj< m_size.y; jjj++) {
					for (int32_t iii=0; iii< m_size.x; iii++) {
						tmpMatrix[jjj][iii] = (*this)[iii][jjj];
					}
				}
				return tmpMatrix;
			};
			/**
			 * @ brief Create a convolution on the matrix : set convolution on the lines
			 * @ param[in] obj The convolution operator
			 * @ return the value of the convolution
			 */
			Matrix<T> Convolution(Matrix<T>& obj)
			{
				Matrix<T> tmppp(1,1);
				// TODO : ...
				return tmppp;
			};
			/**
			 * @ brief generate a devide of the curent Matrix with the specify power of 2
			 * @ param[in] decalage The power of 2 of the division
			 * @ return the result
			 */
			Matrix<T>& Fix(int32_t decalage)
			{
				Matrix<T> tmppp(1,1);
				// TODO : ...
				return tmppp;
			};
			/**
			 * @ brief generate a devide of the curent Matrix with the specify power of 2
			 * @ param[in] decalage The power of 2 of the division
			 * @ return the result
			 */
			Matrix<T>& Round2(int32_t decalage)
			{
				Matrix<T> tmppp(1,1);
				// TODO : ...
				return tmppp;
			};
			/*****************************************************
			 *    other stupid action :
			 *****************************************************/
			Vector2D<int32_t> Size(void) { return m_size; };
			void Clear(void)
			{
				// copy data for the same size :
				for (int32_t iii=0; iii< m_size.x*m_size.y; iii++) {
					m_data[iii] = (T)0;
				}
			};
			void Identity(void)
			{
				// copy data for the same size :
				for (int32_t iii=0; iii< etk_min(m_size.x, m_size.y); iii++) {
					(*this)[iii][iii] = (T)1;
				}
			};
			
			void Set(int32_t iii, int32_t jjj, T value)
			{
				m_data[iii*m_size.x+jjj] = value;
			}
			T Get(int32_t iii, int32_t jjj)
			{
				return m_data[iii*m_size.x+jjj];
			}
			void Display(void)
			{
				/*
				TK_INFO("Matrix display : ");
				for (int32_t jjj=0; jjj< m_size.y; jjj++) {
					if (m_size.x == 0) {
						TK_INFO("    --- , ");
					} else if (m_size.x == 1) {
						TK_INFO("    " << (*this)[jjj][0] << " , ");
					} else if (m_size.x == 2) {
						TK_INFO("    " << (*this)[jjj][0] << " , " << (*this)[jjj][1] << " , ");
					} else if (m_size.x == 3) {
						TK_INFO("    " << (*this)[jjj][0] << " , " << (*this)[jjj][1] << " , " << (*this)[jjj][2] << " , ");
					} else if (m_size.x == 4) {
						TK_INFO("    " << (*this)[jjj][0] << " , " << (*this)[jjj][1] << " , " << (*this)[jjj][2] << " , " << (*this)[jjj][3] << " , ");
					} else if (m_size.x == 5) {
						TK_INFO("    " << (*this)[jjj][0] << " , " << (*this)[jjj][1] << " , " << (*this)[jjj][2] << " , " << (*this)[jjj][3] << " , " << (*this)[jjj][4] << " , ");
					} else if (m_size.x == 6) {
						TK_INFO("    " << (*this)[jjj][0] << " , " << (*this)[jjj][1] << " , " << (*this)[jjj][2] << " , " << (*this)[jjj][3] << " , " << (*this)[jjj][4] << " , " << (*this)[jjj][5] << " , ");
					} else {
						TK_INFO("    " << (*this)[jjj][0] << " , " << (*this)[jjj][1] << " , " << (*this)[jjj][2] << " , " << (*this)[jjj][3] << " , " << (*this)[jjj][4] << " , " << (*this)[jjj][5] << " , " << (*this)[jjj][6] << " , ");
					}
				}
				*/
			};
	};
};

#endif
