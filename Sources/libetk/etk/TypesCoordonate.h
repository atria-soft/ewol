/**
 *******************************************************************************
 * @file etk/TypesCoordonate.h
 * @brief Ewol Tool Kit : generique coordonate type abstraction for openGl
 * @author Edouard DUPIN
 * @date 07/06/2012
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

#ifndef __ETK_TYPES_COORDONATES_H__
#define __ETK_TYPES_COORDONATES_H__


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
			return *this;
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
			return *this;
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
			return *this;
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
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:

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



#endif
