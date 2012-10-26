/**
 *******************************************************************************
 * @file etk/math/Vector2D.h
 * @brief Ewol Tool Kit : Vector 2 dimention (x,y)
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

#ifndef __ETK_MATH_VECTOR2D_H__
#define __ETK_MATH_VECTOR2D_H__

namespace etk
{
	template <typename T> class Vector2D
	{
		public:
			T x;
			T y;
		public:
			/*****************************************************
			 *    Constructor
			 *****************************************************/
			 Vector2D(void) : x(0), y(0) { };
			 Vector2D(double _x, double _y) : x(_x), y(_y) { };
			 Vector2D(float _x, float _y) : x(_x), y(_y) { };
			 Vector2D(int32_t _x, int32_t _y) : x(_x), y(_y) { };
			 Vector2D(const Vector2D<double>& obj)   : x((T)obj.x), y((T)obj.y) { };
			 Vector2D(const Vector2D<float>& obj)    : x((T)obj.x), y((T)obj.y) { };
			 Vector2D(const Vector2D<int32_t>& obj)  : x((T)obj.x), y((T)obj.y) { };
			~Vector2D(void) { };
			/*****************************************************
			 *    = assigment
			 *****************************************************/
			const Vector2D<T>& operator= (const Vector2D<T>& obj ) {
				x = (T)obj.x;
				y = (T)obj.y;
				return *this;
			}
			/*****************************************************
			 *    == operator
			 *****************************************************/
			bool  operator== (const Vector2D<T>& obj) const {
				if ((T)obj.x == x && (T)obj.y == y) {
					return true;
				}
				return false;
			}
			/*****************************************************
			 *    != operator
			 *****************************************************/
			bool  operator!= (const Vector2D<T>& obj) const {
				if ((T)obj.x == x && (T)obj.y == y) {
					return false;
				}
				return true;
			}
			/*****************************************************
			 *    += operator
			 *****************************************************/
			const Vector2D<T>& operator+= (const Vector2D<T>& obj) {
				x += (T)obj.x;
				y += (T)obj.y;
				return *this;
			}
			/*****************************************************
			 *    + operator
			 *****************************************************/
			Vector2D<T> operator+ (const Vector2D<T>& obj) {
				Vector2D<T> tmpp(x,y);
				tmpp.x += (T)obj.x;
				tmpp.y += (T)obj.y;
				return tmpp;
			}
			/*****************************************************
			 *    -= operator
			 *****************************************************/
			const Vector2D<T>& operator-= (const Vector2D<T>& obj) {
				x -= (T)obj.x;
				y -= (T)obj.y;
				return *this;
			}
			/*****************************************************
			 *    - operator
			 *****************************************************/
			Vector2D<T> operator- (const Vector2D<T>& obj) {
				Vector2D<T> tmpp(x,y);
				tmpp.x -= (T)obj.x;
				tmpp.y -= (T)obj.y;
				return tmpp;
			}
			/*****************************************************
			 *    /= operator
			 *****************************************************/
			const Vector2D<T>& operator/= (const Vector2D<T>& obj) {
				x /= (T)obj.x;
				y /= (T)obj.y;
				return *this;
			}
			/*****************************************************
			 *    / operator
			 *****************************************************/
			Vector2D<T> operator/ (const Vector2D<T>& obj) {
				Vector2D<T> tmpp(x,y);
				tmpp.x /= (T)obj.x;
				tmpp.y /= (T)obj.y;
				return tmpp;
			}
			/*****************************************************
			 *    *= operator
			 *****************************************************/
			const Vector2D<T>& operator*= (const Vector2D<T>& obj) {
				x *= (T)obj.x;
				y *= (T)obj.y;
				return *this;
			}
			/*****************************************************
			 *    * operator
			 *****************************************************/
			Vector2D<T> operator* (const Vector2D<T>& obj) {
				Vector2D<T> tmpp(x,y);
				tmpp.x *= (T)obj.x;
				tmpp.y *= (T)obj.y;
				return tmpp;
			}
			/*****************************************************
			 *    ++ operator
			 *****************************************************/
			Vector2D<T>& operator++() // prefix
			{
				++x;
				++y;
				return *this;
			}
			Vector2D<T> operator++(int unused) // postfix
			{
				Vector2D<T> result = *this;
				++(*this);
				return result;
			}
			/*****************************************************
			 *    -- operator
			 *****************************************************/
			Vector2D<T>& operator--() // prefix
			{
				--x;
				--y;
				return *this;
			}
			
			Vector2D<T> operator--(int unused) // postfix
			{
				Vector2D<T> result = *this;
				--(*this);
				return result;
			}
			
			T QuadDist(void)
			{
				return x*x + y*y;
			}
			
			T Dist(void)
			{
				return sqrt(x*x + y*y);
			}
	};
};

#endif
