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
			/*****************************************************
			 *    -= operator
			 *****************************************************/
			const Vector3D<T>& operator-= (const Vector3D<T>& obj) {
				x -= (T)obj.x;
				y -= (T)obj.y;
				z -= (T)obj.z;
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
			/*****************************************************
			 *    /= operator
			 *****************************************************/
			const Vector3D<T>& operator/= (const Vector3D<T>& obj) {
				x /= (T)obj.x;
				y /= (T)obj.y;
				z /= (T)obj.z;
				return *this;
			}
			/*****************************************************
			 *    / operator
			 *****************************************************/
			Vector3D<T> operator/ (const Vector3D<T>& obj) {
				Vector3D<T> tmpp(x,y,y);
				tmpp.x /= (T)obj.x;
				tmpp.y /= (T)obj.y;
				tmpp.z /= (T)obj.z;
				return *this;
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
			/*****************************************************
			 *    * operator
			 *****************************************************/
			Vector3D<T> operator* (const Vector3D<T>& obj) {
				Vector3D<T> tmpp(x,y,y);
				tmpp.x *= (T)obj.x;
				tmpp.y *= (T)obj.y;
				tmpp.z *= (T)obj.z;
				return *this;
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
	};
};



#endif
