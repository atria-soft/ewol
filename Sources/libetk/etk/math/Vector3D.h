/**
 *******************************************************************************
 * @file etk/math/Vector3D.h
 * @brief Ewol Tool Kit : Vector 3 dimention (x, y, z)
 * @author Edouard DUPIN
 * @date 26/10/2012
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

#ifndef __ETK_MATH_VECTOR3D_H__
#define __ETK_MATH_VECTOR3D_H__

namespace etk
{
	template <typename T> class Vector3D
	{
		public:
			T x;
			T y;
			T z;
		public:
			/*****************************************************
			 *    Constructor
			 *****************************************************/
			 Vector3D(void) : x(0), y(0), z(0) { };
			 Vector3D(double _x, double _y, double _z)    : x(_x), y(_y), z(_z) { };
			 Vector3D(float _x, float _y, float _z)       : x(_x), y(_y), z(_z) { };
			 Vector3D(int32_t _x, int32_t _y, int32_t _z) : x(_x), y(_y), z(_z) { };
			 Vector3D(const Vector3D<double>& obj)  : x((T)obj.x), y((T)obj.y), z((T)obj.z) { };
			 Vector3D(const Vector3D<float>& obj)   : x((T)obj.x), y((T)obj.y), z((T)obj.z) { };
			 Vector3D(const Vector3D<int32_t>& obj) : x((T)obj.x), y((T)obj.y), z((T)obj.z) { };
			~Vector3D(void) { };
			/*****************************************************
			 *    = assigment
			 *****************************************************/
			const Vector3D<T>& operator= (const Vector3D<T>& obj ) {
				x = (T)obj.x;
				y = (T)obj.y;
				z = (T)obj.z;
				return *this;
			}
			/*****************************************************
			 *    == operator
			 *****************************************************/
			bool  operator== (const Vector3D<T>& obj) const {
				if ((T)obj.x == x && (T)obj.y == y && (T)obj.z == z) {
					return true;
				}
				return false;
			}
			/*****************************************************
			 *    != operator
			 *****************************************************/
			bool  operator!= (const Vector3D<T>& obj) const {
				if ((T)obj.x == x && (T)obj.y == y && (T)obj.z == z) {
					return false;
				}
				return true;
			}
			/*****************************************************
			 *    += operator
			 *****************************************************/
			const Vector3D<T>& operator+= (const Vector3D<T>& obj) {
				x += (T)obj.x;
				y += (T)obj.y;
				z += (T)obj.z;
				return *this;
			}
			const Vector3D<T>& operator+= (const float val) {
				x += val;
				y += val;
				z += val;
				return *this;
			}
			/*****************************************************
			 *    + operator
			 *****************************************************/
			Vector3D<T> operator+ (const Vector3D<T>& obj) {
				Vector3D<T> tmpp(x,y,y);
				tmpp.x += (T)obj.x;
				tmpp.y += (T)obj.y;
				tmpp.z += (T)obj.z;
				return *this;
			}
			Vector3D<T> operator+ (const float val) {
				Vector3D<T> tmpp(x,y,y);
				tmpp.x += val;
				tmpp.y += val;
				tmpp.z += val;
				return *this;
			}
			/*****************************************************
			 *    -= operator
			 *****************************************************/
			const Vector3D<T>& operator-= (const Vector3D<T>& obj) {
				x -= (T)obj.x;
				y -= (T)obj.y;
				z -= (T)obj.z;
				return *this;
			}
			const Vector3D<T>& operator-= (const float val) {
				x -= val;
				y -= val;
				z -= val;
				return *this;
			}
			/*****************************************************
			 *    - operator
			 *****************************************************/
			Vector3D<T> operator- (const Vector3D<T>& obj) {
				Vector3D<T> tmpp(x,y,y);
				tmpp.x -= (T)obj.x;
				tmpp.y -= (T)obj.y;
				tmpp.z -= (T)obj.z;
				return *this;
			}
			Vector3D<T> operator- (const float val) {
				Vector3D<T> tmpp(x,y,y);
				tmpp.x -= val;
				tmpp.y -= val;
				tmpp.z -= val;
				return *this;
			}
			/*****************************************************
			 *    /= operator
			 *****************************************************/
			const Vector3D<T>& operator/= (const Vector3D<T>& obj) {
				if (obj.x != 0) {
					x /= (T)obj.x;
				}
				if (obj.y != 0) {
					y /= (T)obj.y;
				}
				if (obj.z != 0) {
					z /= (T)obj.z;
				}
				return *this;
			}
			const Vector3D<T>& operator/= (const float val) {
				if (val==0) {
					return *this;
				}
				x /= val;
				y /= val;
				z /= val;
				return *this;
			}
			/*****************************************************
			 *    / operator
			 *****************************************************/
			Vector3D<T> operator/ (const Vector3D<T>& obj) {
				Vector3D<T> tmpp(x,y,y);
				if (obj.x != 0) {
					tmpp.x /= (T)obj.x;
				}
				if (obj.y != 0) {
					tmpp.y /= (T)obj.y;
				}
				if (obj.z != 0) {
					tmpp.z /= (T)obj.z;
				}
				return tmpp;
			}
			Vector3D<T> operator/ (const float val) {
				Vector3D<T> tmpp(x,y,y);
				if (val==0) {
					return tmpp;
				}
				tmpp.x /= val;
				tmpp.y /= val;
				tmpp.z /= val;
				return tmpp;
			}
			/*****************************************************
			 *    *= operator
			 *****************************************************/
			const Vector3D<T>& operator*= (const Vector3D<T>& obj) {
				x *= (T)obj.x;
				y *= (T)obj.y;
				z *= (T)obj.z;
				return *this;
			}
			const Vector3D<T>& operator*= (const float val) {
				x *= val;
				y *= val;
				z *= val;
				return *this;
			}
			/*****************************************************
			 *    * operator
			 *****************************************************/
			Vector3D<T> operator* (const Vector3D<T>& obj) {
				Vector3D<T> tmpp(x,y,y);
				tmpp.x *= (T)obj.x;
				tmpp.y *= (T)obj.y;
				tmpp.z *= (T)obj.z;
				return tmpp;
			}
			Vector3D<T> operator* (const float val) {
				Vector3D<T> tmpp(x,y,y);
				tmpp.x *= val;
				tmpp.y *= val;
				tmpp.z *= val;
				return tmpp;
			}
			/*****************************************************
			 *    ++ operator
			 *****************************************************/
			Vector3D<T>& operator++() // prefix
			{
				++x;
				++y;
				++z;
				return *this;
			}
			Vector3D<T> operator++(int unused) // postfix
			{
				Vector3D<T> result = *this;
				++(*this);
				return result;
			}
			/*****************************************************
			 *    -- operator
			 *****************************************************/
			Vector3D<T>& operator--() // prefix
			{
				--x;
				--y;
				--z;
				return *this;
			}
			Vector3D<T> operator--(int unused) // postfix
			{
				Vector3D<T> result = *this;
				--(*this);
				return result;
			}
			
			void Zero(void)
			{
				x=0;
				y=0;
				z=0;
			};
			void One(void)
			{
				x=1;
				y=1;
				z=1;
			};
			
			//vector algebra
			Vector3D<T> CrossProduct(const Vector3D<T>& obj) const
			{
				return Vector3D<T>( y*obj.z - z*obj.y,
				                    z*obj.x - x*obj.z,
				                    x*obj.y - y*obj.x);
			};
			
			float DotProduct(const Vector3D<T>& obj) const
			{
				return   x*obj.x
				       + y*obj.y
				       + z*obj.z;
			};
			
			void Normalize()
			{
				float length=GetLength();
				if(length==1 || length==0) {
					return;
				}
				float scalefactor = 1.0f/length;
				x *= scalefactor;
				y *= scalefactor;
				z *= scalefactor;
			};
			
			Vector3D<T> GetNormalized() const
			{
				Vector3D<T> tmpp(*this);
				tmpp.Normalize();
				return tmpp;
			};
			
			float GetLength() const
			{
				return (float)sqrt((x*x)+(y*y)+(z*z));
			};
			
			float GetSquaredLength() const
			{
				return (x*x)+(y*y)+(z*z);
			};
			
			//rotations
			void RotateX(double angle)
			{
				(*this)=GetRotatedX(angle);
			};
			
			Vector3D<T> GetRotatedX(double angle) const
			{
				if(angle==0.0) {
					return (*this);
				}
				float sinAngle=(float)sin(M_PI*angle/180);
				float cosAngle=(float)cos(M_PI*angle/180);
				
				return Vector3D<T>( x,
				                    y*cosAngle - z*sinAngle,
				                    y*sinAngle + z*cosAngle);
			};
			
			void RotateY(double angle)
			{
				(*this)=GetRotatedY(angle);
			};
			
			Vector3D<T> GetRotatedY(double angle) const
			{
				if(angle==0.0) {
					return (*this);
				}
				float sinAngle=(float)sin(M_PI*angle/180);
				float cosAngle=(float)cos(M_PI*angle/180);
				return Vector3D<T>( x*cosAngle + z*sinAngle,
				                    y,
				                    -x*sinAngle + z*cosAngle);
			};
			
			void RotateZ(double angle)
			{
				(*this)=GetRotatedZ(angle);
			};
			
			Vector3D<T> GetRotatedZ(double angle) const
			{
				if(angle==0.0) {
					return (*this);
				}
				float sinAngle=(float)sin(M_PI*angle/180);
				float cosAngle=(float)cos(M_PI*angle/180);
				return Vector3D<T>( x*cosAngle - y*sinAngle,
				                    x*sinAngle + y*cosAngle,
				                    z);
			};
			
			void RotateAxis(double angle, const Vector3D<T> & axis)
			{
				(*this)=GetRotatedAxis(angle, axis);
			};
			
			Vector3D<T> GetRotatedAxis(double angle, const Vector3D<T> & axis) const
			{
				if(angle==0.0) {
					return (*this);
				}
				Vector3D<T> u=axis.GetNormalized();
				Vector3D<T> rotMatrixRow0, rotMatrixRow1, rotMatrixRow2;
				float sinAngle=(float)sin(M_PI*angle/180);
				float cosAngle=(float)cos(M_PI*angle/180);
				float MinusCosAngle=1.0f-cosAngle;
				rotMatrixRow0.x=(u.x)*(u.x) + cosAngle*(1-(u.x)*(u.x));
				rotMatrixRow0.y=(u.x)*(u.y)*(MinusCosAngle) - sinAngle*u.z;
				rotMatrixRow0.z=(u.x)*(u.z)*(MinusCosAngle) + sinAngle*u.y;
				rotMatrixRow1.x=(u.x)*(u.y)*(MinusCosAngle) + sinAngle*u.z;
				rotMatrixRow1.y=(u.y)*(u.y) + cosAngle*(1-(u.y)*(u.y));
				rotMatrixRow1.z=(u.y)*(u.z)*(MinusCosAngle) - sinAngle*u.x;
				rotMatrixRow2.x=(u.x)*(u.z)*(MinusCosAngle) - sinAngle*u.y;
				rotMatrixRow2.y=(u.y)*(u.z)*(MinusCosAngle) + sinAngle*u.x;
				rotMatrixRow2.z=(u.z)*(u.z) + cosAngle*(1-(u.z)*(u.z));
				return Vector3D<T>( this->DotProduct(rotMatrixRow0),
				                    this->DotProduct(rotMatrixRow1),
				                    this->DotProduct(rotMatrixRow2));
			};
			
			/**
			 * @brief Linar intermolation of the curent Vector
			 * @param[in] input
			 * @param[in] factor
			 * @return the interpolate vector
			 */
			Vector3D<T> LinearInterpolate(const Vector3D<T>& input, float factor) const
			{
				return (*this)*(1.0f-factor) + input*factor;
			};
			
			/**
			 * @brief Quadratic intermolation of the curent Vector
			 * @param[in] v1
			 * @param[in] v2
			 * @param[in] factor
			 * @return the interpolate vector
			 */
			Vector3D<T> QuadraticInterpolate(const Vector3D<T>& v2, const Vector3D<T>& v3, float factor) const
			{
				return (*this)*(1.0f-factor)*(1.0f-factor) + 2*v2*factor*(1.0f-factor) + v3*factor*factor;
			};
	};
};



#endif
