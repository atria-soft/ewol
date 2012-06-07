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

class oglt
{
	public:
		#if defined(EWOL_PECISION_FIXPOINT)
			int32_t val;
		#elseif defined(EWOL_PECISION_DOUBLE)
			double val;
		#else
			float val;
		#endif
	public:
		/*****************************************************
		 *    Constructor
		 *****************************************************/
		 oglt(void) : val(0) { };
		 oglt(const oglt& obj) : val(obj.val) { };
		~oglt(void) { };
		#if defined(EWOL_PECISION_FIXPOINT)
			 oglt(const int32_t val) : val(val << 16) { };
			 oglt(const float val) : val(val*65536.0) { };
			 oglt(const double val) : val(val*65536.0) { };
			operator int32_t() const {  return val>>16; }
			operator float() const {    return (float)((float)val/(float)65536.0); }
			operator double() const {   return (double)((double)val/(double)65536.0); }
		#else
			 oglt(const int32_t val) : val(val) { };
			 oglt(const float val) : val(val) { };
			 oglt(const double val) : val(val) { };
			operator int32_t() const {  return (int32_t)val; }
			operator float() const {    return (float)val; }
			operator double() const {   return (double)val; }
		#endif
		/*****************************************************
		 *    = assigment
		 *****************************************************/
		const oglt& operator= (const oglt& obj ) {
			val = obj.val;
			return *this;
		}
		/*****************************************************
		 *    == operator
		 *****************************************************/
		const bool operator== (const oglt& obj) const {
			return (obj.val == val) ? true : false;
		}
		/*****************************************************
		 *    != operator
		 *****************************************************/
		bool operator!= (const oglt& obj) const {
			return (obj.val == val) ? false : true;
		}
		/*****************************************************
		 *    += operator
		 *****************************************************/
		const oglt& operator+= (const oglt& obj) {
			val += obj.val;
			return *this;
		}
		/*****************************************************
		 *    + operator
		 *****************************************************/
		const oglt& operator+ (const oglt& obj) {
			val += obj.val;
			return *this;
		}
		/*****************************************************
		 *    -= operator
		 *****************************************************/
		const oglt& operator-= (const oglt& obj) {
			val -= obj.val;
			return *this;
		}
		/*****************************************************
		 *    - operator
		 *****************************************************/
		const oglt& operator- (const oglt& obj) {
			val -= obj.val;
			return *this;
		}
		/*****************************************************
		 *    /= operator
		 *****************************************************/
		const oglt& operator/= (const oglt& obj) {
			#if defined(EWOL_PECISION_FIXPOINT)
				val = ((int64_t)val<<16) / obj.val;
			#else
				val /= obj.val;
			#endif
			return *this;
		}
		/*****************************************************
		 *    / operator
		 *****************************************************/
		const oglt& operator/ (const oglt& obj) {
			#if defined(EWOL_PECISION_FIXPOINT)
				val = ((int64_t)val<<16) / obj.val;
			#else
				val /= obj.val;
			#endif
			return *this;
		}
		/*****************************************************
		 *    *= operator
		 *****************************************************/
		const oglt& operator*= (const oglt& obj) {
			#if defined(EWOL_PECISION_FIXPOINT)
				int64_t tmpVal = (int64_t)val * (int64_t)obj.val;
				val = tmpVal >> 16;
			#else
				val *= obj.val;
			#endif
			return *this;
		}
		/*****************************************************
		 *    * operator
		 *****************************************************/
		const oglt& operator* (const oglt& obj) {
			#if defined(EWOL_PECISION_FIXPOINT)
				int64_t tmpVal = (int64_t)val * (int64_t)obj.val;
				val = tmpVal >> 16;
			#else
				val *= obj.val;
			#endif
			return *this;
		}
		/*****************************************************
		 *    ++ operator
		 *****************************************************/
		oglt& operator++() // prefix
		{
			++val;
			return *this;
		}
		oglt operator++(int unused) // postfix
		{
			oglt result = *this;
			++(*this);
			return result;
		}
		/*****************************************************
		 *    -- operator
		 *****************************************************/
		oglt& operator--() // prefix
		{
			--val;
			return *this;
		}
		oglt operator--(int unused) // postfix
		{
			oglt result = *this;
			--(*this);
			return result;
		}
};


template <typename T> class Coord2D
{
	public:
		T x;
		T y;
	public:
		/*****************************************************
		 *    Constructor
		 *****************************************************/
		 Coord2D(void) : x(0), y(0) { };
		 Coord2D(double _x, double _y) : x(_x), y(_y) { };
		 Coord2D(float _x, float _y) : x(_x), y(_y) { };
		 Coord2D(int32_t _x, int32_t _y) : x(_x), y(_y) { };
		 Coord2D(const Coord2D<double>& obj)   : x((T)obj.x), y((T)obj.y) { };
		 Coord2D(const Coord2D<float>& obj)    : x((T)obj.x), y((T)obj.y) { };
		 Coord2D(const Coord2D<int32_t>& obj)  : x((T)obj.x), y((T)obj.y) { };
		 Coord2D(const Coord2D<oglt>& obj)     : x((T)obj.x), y((T)obj.y) { };
		~Coord2D(void) { };
		/*****************************************************
		 *    = assigment
		 *****************************************************/
		const Coord2D<T>& operator= (const Coord2D<double>& obj ) {
			x = (T)obj.x;
			y = (T)obj.y;
			return *this;
		}
		const Coord2D<T>& operator= (const Coord2D<float>& obj ) {
			x = (T)obj.x;
			y = (T)obj.y;
			return *this;
		}
		const Coord2D<T>& operator= (const Coord2D<int32_t>& obj ) {
			x = (T)obj.x;
			y = (T)obj.y;
			return *this;
		}
		const Coord2D<T>& operator= (const Coord2D<oglt>& obj ) {
			x = (T)obj.x;
			y = (T)obj.y;
			return *this;
		}
		/*****************************************************
		 *    == operator
		 *****************************************************/
		bool  operator== (const Coord2D<double>& obj) const {
			if ((T)obj.x == x && (T)obj.y == y) {
				return true;
			}
			return false;
		}
		bool  operator== (const Coord2D<float>& obj) const {
			if ((T)obj.x == x && (T)obj.y == y) {
				return true;
			}
			return false;
		}
		bool  operator== (const Coord2D<int32_t>& obj) const {
			if ((T)obj.x == x && (T)obj.y == y) {
				return true;
			}
			return false;
		}
		bool  operator== (const Coord2D<oglt>& obj) const {
			if ((T)obj.x == x && (T)obj.y == y) {
				return true;
			}
			return false;
		}
		/*****************************************************
		 *    != operator
		 *****************************************************/
		bool  operator!= (const Coord2D<double>& obj) const {
			if ((T)obj.x == x && (T)obj.y == y) {
				return false;
			}
			return true;
		}
		bool  operator!= (const Coord2D<float>& obj) const {
			if ((T)obj.x == x && (T)obj.y == y) {
				return false;
			}
			return true;
		}
		bool  operator!= (const Coord2D<int32_t>& obj) const {
			if ((T)obj.x == x && (T)obj.y == y) {
				return false;
			}
			return true;
		}
		bool  operator!= (const Coord2D<oglt>& obj) const {
			if ((T)obj.x == x && (T)obj.y == y) {
				return false;
			}
			return true;
		}
		/*****************************************************
		 *    += operator
		 *****************************************************/
		const Coord2D<T>& operator+= (const Coord2D<double>& obj) {
			x += (T)obj.x;
			y += (T)obj.y;
			return *this;
		}
		const Coord2D<T>& operator+= (const Coord2D<float>& obj) {
			x += (T)obj.x;
			y += (T)obj.y;
			return *this;
		}
		const Coord2D<T>& operator+= (const Coord2D<int32_t>& obj) {
			x += (T)obj.x;
			y += (T)obj.y;
			return *this;
		}
		const Coord2D<T>& operator+= (const Coord2D<oglt>& obj) {
			x += (T)obj.x;
			y += (T)obj.y;
			return *this;
		}
		/*****************************************************
		 *    + operator
		 *****************************************************/
		Coord2D<T> operator+ (const Coord2D<double>& obj) {
			x += (T)obj.x;
			y += (T)obj.y;
			return *this;
		}
		Coord2D<T> operator+ (const Coord2D<float>& obj) {
			x += (T)obj.x;
			y += (T)obj.y;
			return *this;
		}
		Coord2D<T> operator+ (const Coord2D<int32_t>& obj) {
			x += (T)obj.x;
			y += (T)obj.y;
			return *this;
		}
		Coord2D<T> operator+ (const Coord2D<oglt>& obj) {
			x += (T)obj.x;
			y += (T)obj.y;
			return *this;
		}
		/*****************************************************
		 *    -= operator
		 *****************************************************/
		const Coord2D<T>& operator-= (const Coord2D<double>& obj) {
			x -= (T)obj.x;
			y -= (T)obj.y;
			return *this;
		}
		const Coord2D<T>& operator-= (const Coord2D<float>& obj) {
			x -= (T)obj.x;
			y -= (T)obj.y;
			return *this;
		}
		const Coord2D<T>& operator-= (const Coord2D<int32_t>& obj) {
			x -= (T)obj.x;
			y -= (T)obj.y;
			return *this;
		}
		const Coord2D<T>& operator-= (const Coord2D<oglt>& obj) {
			x -= (T)obj.x;
			y -= (T)obj.y;
			return *this;
		}
		/*****************************************************
		 *    - operator
		 *****************************************************/
		Coord2D<T> operator- (const Coord2D<double>& obj) {
			x -= (T)obj.x;
			y -= (T)obj.y;
			return *this;
		}
		Coord2D<T> operator- (const Coord2D<float>& obj) {
			x -= (T)obj.x;
			y -= (T)obj.y;
			return *this;
		}
		Coord2D<T> operator- (const Coord2D<int32_t>& obj) {
			x -= (T)obj.x;
			y -= (T)obj.y;
			return *this;
		}
		Coord2D<T> operator- (const Coord2D<oglt>& obj) {
			x -= (T)obj.x;
			y -= (T)obj.y;
			return *this;
		}
		/*****************************************************
		 *    /= operator
		 *****************************************************/
		const Coord2D<T>& operator/= (const Coord2D<double>& obj) {
			x /= (T)obj.x;
			y /= (T)obj.y;
			return *this;
		}
		const Coord2D<T>& operator/= (const Coord2D<float>& obj) {
			x /= (T)obj.x;
			y /= (T)obj.y;
			return *this;
		}
		const Coord2D<T>& operator/= (const Coord2D<int32_t>& obj) {
			x /= (T)obj.x;
			y /= (T)obj.y;
			return *this;
		}
		const Coord2D<T>& operator/= (const Coord2D<oglt>& obj) {
			x /= (T)obj.x;
			y /= (T)obj.y;
			return *this;
		}
		/*****************************************************
		 *    / operator
		 *****************************************************/
		Coord2D<T> operator/ (const Coord2D<double>& obj) {
			x /= (T)obj.x;
			y /= (T)obj.y;
			return *this;
		}
		Coord2D<T> operator/ (const Coord2D<float>& obj) {
			x /= (T)obj.x;
			y /= (T)obj.y;
			return *this;
		}
		Coord2D<T> operator/ (const Coord2D<int32_t>& obj) {
			x /= (T)obj.x;
			y /= (T)obj.y;
			return *this;
		}
		Coord2D<T> operator/ (const Coord2D<oglt>& obj) {
			x /= (T)obj.x;
			y /= (T)obj.y;
			return *this;
		}
		/*****************************************************
		 *    *= operator
		 *****************************************************/
		const Coord2D<T>& operator*= (const Coord2D<double>& obj) {
			x *= (T)obj.x;
			y *= (T)obj.y;
			return *this;
		}
		const Coord2D<T>& operator*= (const Coord2D<float>& obj) {
			x *= (T)obj.x;
			y *= (T)obj.y;
			return *this;
		}
		const Coord2D<T>& operator*= (const Coord2D<int32_t>& obj) {
			x *= (T)obj.x;
			y *= (T)obj.y;
			return *this;
		}
		const Coord2D<T>& operator*= (const Coord2D<oglt>& obj) {
			x *= (T)obj.x;
			y *= (T)obj.y;
			return *this;
		}
		/*****************************************************
		 *    * operator
		 *****************************************************/
		Coord2D<T> operator* (const Coord2D<double>& obj) {
			x *= (T)obj.x;
			y *= (T)obj.y;
			return *this;
		}
		Coord2D<T> operator* (const Coord2D<float>& obj) {
			x *= (T)obj.x;
			y *= (T)obj.y;
			return *this;
		}
		Coord2D<T> operator* (const Coord2D<int32_t>& obj) {
			x *= (T)obj.x;
			y *= (T)obj.y;
			return *this;
		}
		Coord2D<T> operator* (const Coord2D<oglt>& obj) {
			x *= (T)obj.x;
			y *= (T)obj.y;
			return *this;
		}
		/*****************************************************
		 *    ++ operator
		 *****************************************************/
		Coord2D<T>& operator++() // prefix
		{
			++x;
			++y;
			return *this;
		}
		Coord2D<T> operator++(int unused) // postfix
		{
			Coord2D<T> result = *this;
			++(*this);
			return result;
		}
		/*****************************************************
		 *    -- operator
		 *****************************************************/
		Coord2D<T>& operator--() // prefix
		{
			--x;
			--y;
			return *this;
		}
		
		Coord2D<T> operator--(int unused) // postfix
		{
			Coord2D<T> result = *this;
			--(*this);
			return result;
		}
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:

template <typename T> class Coord3D
{
	public:
		T x;
		T y;
		T z;
	public:
		/*****************************************************
		 *    Constructor
		 *****************************************************/
		 Coord3D(void) : x(0), y(0), z(0) { };
		 Coord3D(double _x, double _y, double _z)    : x(_x), y(_y), z(_z) { };
		 Coord3D(float _x, float _y, float _z)       : x(_x), y(_y), z(_z) { };
		 Coord3D(int32_t _x, int32_t _y, int32_t _z) : x(_x), y(_y), z(_z) { };
		 Coord3D(const Coord3D<double>& obj)  : x((T)obj.x), y((T)obj.y), z((T)obj.z) { };
		 Coord3D(const Coord3D<float>& obj)   : x((T)obj.x), y((T)obj.y), z((T)obj.z) { };
		 Coord3D(const Coord3D<int32_t>& obj) : x((T)obj.x), y((T)obj.y), z((T)obj.z) { };
		 Coord3D(const Coord3D<oglt>& obj)    : x((T)obj.x), y((T)obj.y), z((T)obj.z) { };
		~Coord3D(void) { };
		/*****************************************************
		 *    = assigment
		 *****************************************************/
		const Coord3D<T>& operator= (const Coord3D<double>& obj ) {
			x = (T)obj.x;
			y = (T)obj.y;
			z = (T)obj.z;
			return *this;
		}
		const Coord3D<T>& operator= (const Coord3D<float>& obj ) {
			x = (T)obj.x;
			y = (T)obj.y;
			z = (T)obj.z;
			return *this;
		}
		const Coord3D<T>& operator= (const Coord3D<int32_t>& obj ) {
			x = (T)obj.x;
			y = (T)obj.y;
			z = (T)obj.z;
			return *this;
		}
		const Coord3D<T>& operator= (const Coord3D<oglt>& obj ) {
			x = (T)obj.x;
			y = (T)obj.y;
			z = (T)obj.z;
			return *this;
		}
		/*****************************************************
		 *    == operator
		 *****************************************************/
		bool  operator== (const Coord3D<double>& obj) const {
			if ((T)obj.x == x && (T)obj.y == y && (T)obj.z == z) {
				return true;
			}
			return false;
		}
		bool  operator== (const Coord3D<float>& obj) const {
			if ((T)obj.x == x && (T)obj.y == y && (T)obj.z == z) {
				return true;
			}
			return false;
		}
		bool  operator== (const Coord3D<int32_t>& obj) const {
			if ((T)obj.x == x && (T)obj.y == y && (T)obj.z == z) {
				return true;
			}
			return false;
		}
		bool  operator== (const Coord3D<oglt>& obj) const {
			if ((T)obj.x == x && (T)obj.y == y && (T)obj.z == z) {
				return true;
			}
			return false;
		}
		/*****************************************************
		 *    != operator
		 *****************************************************/
		bool  operator!= (const Coord3D<double>& obj) const {
			if ((T)obj.x == x && (T)obj.y == y && (T)obj.z == z) {
				return false;
			}
			return true;
		}
		bool  operator!= (const Coord3D<float>& obj) const {
			if ((T)obj.x == x && (T)obj.y == y && (T)obj.z == z) {
				return false;
			}
			return true;
		}
		bool  operator!= (const Coord3D<int32_t>& obj) const {
			if ((T)obj.x == x && (T)obj.y == y && (T)obj.z == z) {
				return false;
			}
			return true;
		}
		bool  operator!= (const Coord3D<oglt>& obj) const {
			if ((T)obj.x == x && (T)obj.y == y && (T)obj.z == z) {
				return false;
			}
			return true;
		}
		/*****************************************************
		 *    += operator
		 *****************************************************/
		const Coord3D<T>& operator+= (const Coord3D<double>& obj) {
			x += (T)obj.x;
			y += (T)obj.y;
			z += (T)obj.z;
			return *this;
		}
		const Coord3D<T>& operator+= (const Coord3D<float>& obj) {
			x += (T)obj.x;
			y += (T)obj.y;
			z += (T)obj.z;
			return *this;
		}
		const Coord3D<T>& operator+= (const Coord3D<int32_t>& obj) {
			x += (T)obj.x;
			y += (T)obj.y;
			z += (T)obj.z;
			return *this;
		}
		const Coord3D<T>& operator+= (const Coord3D<oglt>& obj) {
			x += (T)obj.x;
			y += (T)obj.y;
			z += (T)obj.z;
			return *this;
		}
		/*****************************************************
		 *    + operator
		 *****************************************************/
		Coord3D<T> operator+ (const Coord3D<double>& obj) {
			x += (T)obj.x;
			y += (T)obj.y;
			z += (T)obj.z;
			return *this;
		}
		Coord3D<T> operator+ (const Coord3D<float>& obj) {
			x += (T)obj.x;
			y += (T)obj.y;
			z += (T)obj.z;
			return *this;
		}
		Coord3D<T> operator+ (const Coord3D<int32_t>& obj) {
			x += (T)obj.x;
			y += (T)obj.y;
			z += (T)obj.z;
			return *this;
		}
		Coord3D<T> operator+ (const Coord3D<oglt>& obj) {
			x += (T)obj.x;
			y += (T)obj.y;
			z += (T)obj.z;
			return *this;
		}
		/*****************************************************
		 *    -= operator
		 *****************************************************/
		const Coord3D<T>& operator-= (const Coord3D<double>& obj) {
			x -= (T)obj.x;
			y -= (T)obj.y;
			z -= (T)obj.z;
			return *this;
		}
		const Coord3D<T>& operator-= (const Coord3D<float>& obj) {
			x -= (T)obj.x;
			y -= (T)obj.y;
			z -= (T)obj.z;
			return *this;
		}
		const Coord3D<T>& operator-= (const Coord3D<int32_t>& obj) {
			x -= (T)obj.x;
			y -= (T)obj.y;
			z -= (T)obj.z;
			return *this;
		}
		const Coord3D<T>& operator-= (const Coord3D<oglt>& obj) {
			x -= (T)obj.x;
			y -= (T)obj.y;
			z -= (T)obj.z;
			return *this;
		}
		/*****************************************************
		 *    - operator
		 *****************************************************/
		Coord3D<T> operator- (const Coord3D<double>& obj) {
			x -= (T)obj.x;
			y -= (T)obj.y;
			z -= (T)obj.z;
			return *this;
		}
		Coord3D<T> operator- (const Coord3D<float>& obj) {
			x -= (T)obj.x;
			y -= (T)obj.y;
			z -= (T)obj.z;
			return *this;
		}
		Coord3D<T> operator- (const Coord3D<int32_t>& obj) {
			x -= (T)obj.x;
			y -= (T)obj.y;
			z -= (T)obj.z;
			return *this;
		}
		Coord3D<T> operator- (const Coord3D<oglt>& obj) {
			x -= (T)obj.x;
			y -= (T)obj.y;
			z -= (T)obj.z;
			return *this;
		}
		/*****************************************************
		 *    /= operator
		 *****************************************************/
		const Coord3D<T>& operator/= (const Coord3D<double>& obj) {
			x /= (T)obj.x;
			y /= (T)obj.y;
			z /= (T)obj.z;
			return *this;
		}
		const Coord3D<T>& operator/= (const Coord3D<float>& obj) {
			x /= (T)obj.x;
			y /= (T)obj.y;
			z /= (T)obj.z;
			return *this;
		}
		const Coord3D<T>& operator/= (const Coord3D<int32_t>& obj) {
			x /= (T)obj.x;
			y /= (T)obj.y;
			z /= (T)obj.z;
			return *this;
		}
		const Coord3D<T>& operator/= (const Coord3D<oglt>& obj) {
			x /= (T)obj.x;
			y /= (T)obj.y;
			z /= (T)obj.z;
			return *this;
		}
		/*****************************************************
		 *    / operator
		 *****************************************************/
		Coord3D<T> operator/ (const Coord3D<double>& obj) {
			x /= (T)obj.x;
			y /= (T)obj.y;
			z /= (T)obj.z;
			return *this;
		}
		Coord3D<T> operator/ (const Coord3D<float>& obj) {
			x /= (T)obj.x;
			y /= (T)obj.y;
			z /= (T)obj.z;
			return *this;
		}
		Coord3D<T> operator/ (const Coord3D<int32_t>& obj) {
			x /= (T)obj.x;
			y /= (T)obj.y;
			z /= (T)obj.z;
			return *this;
		}
		Coord3D<T> operator/ (const Coord3D<oglt>& obj) {
			x /= (T)obj.x;
			y /= (T)obj.y;
			z /= (T)obj.z;
			return *this;
		}
		/*****************************************************
		 *    *= operator
		 *****************************************************/
		const Coord3D<T>& operator*= (const Coord3D<double>& obj) {
			x *= (T)obj.x;
			y *= (T)obj.y;
			z *= (T)obj.z;
			return *this;
		}
		const Coord3D<T>& operator*= (const Coord3D<float>& obj) {
			x *= (T)obj.x;
			y *= (T)obj.y;
			z *= (T)obj.z;
			return *this;
		}
		const Coord3D<T>& operator*= (const Coord3D<int32_t>& obj) {
			x *= (T)obj.x;
			y *= (T)obj.y;
			z *= (T)obj.z;
			return *this;
		}
		const Coord3D<T>& operator*= (const Coord3D<oglt>& obj) {
			x *= (T)obj.x;
			y *= (T)obj.y;
			z *= (T)obj.z;
			return *this;
		}
		/*****************************************************
		 *    * operator
		 *****************************************************/
		Coord3D<T> operator* (const Coord3D<double>& obj) {
			x *= (T)obj.x;
			y *= (T)obj.y;
			z *= (T)obj.z;
			return *this;
		}
		Coord3D<T> operator* (const Coord3D<float>& obj) {
			x *= (T)obj.x;
			y *= (T)obj.y;
			z *= (T)obj.z;
			return *this;
		}
		Coord3D<T> operator* (const Coord3D<int32_t>& obj) {
			x *= (T)obj.x;
			y *= (T)obj.y;
			z *= (T)obj.z;
			return *this;
		}
		Coord3D<T> operator* (const Coord3D<oglt>& obj) {
			x *= (T)obj.x;
			y *= (T)obj.y;
			z *= (T)obj.z;
			return *this;
		}
		/*****************************************************
		 *    ++ operator
		 *****************************************************/
		Coord3D<T>& operator++() // prefix
		{
			++x;
			++y;
			++z;
			return *this;
		}
		Coord3D<T> operator++(int unused) // postfix
		{
			Coord3D<T> result = *this;
			++(*this);
			return result;
		}
		/*****************************************************
		 *    -- operator
		 *****************************************************/
		Coord3D<T>& operator--() // prefix
		{
			--x;
			--y;
			--z;
			return *this;
		}
		Coord3D<T> operator--(int unused) // postfix
		{
			Coord3D<T> result = *this;
			--(*this);
			return result;
		}
};



#endif
