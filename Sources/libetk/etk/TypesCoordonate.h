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

typedef float  oglt;

#if 0
class oglt
{
	public:
		#if defined(EWOL_PECISION_FIXPOINT32)
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
		#if defined(EWOL_PECISION_FIXPOINT32)
			 oglt(const int32_t val) : val(val << 16) { };
			 oglt(const float val) : val(val*65536.0) { };
			 oglt(const double val) : val(val*65536.0) { };
			//operator int32_t() const {  return val>>16; }
			operator float() const {    return (float)((float)val/(float)65536.0); }
			//operator double() const {   return (double)((double)val/(double)65536.0); }
		#else
			 oglt(const int32_t val) : val(val) { };
			 oglt(const float val) : val(val) { };
			 oglt(const double val) : val(val) { };
			//operator int32_t() const {  return (int32_t)val; }
			operator float() const {    return (float)val; }
			//operator double() const {   return (double)val; }
		#endif
		/*****************************************************
		 *    = assigment
		 *****************************************************/
		const oglt& operator= (const oglt& obj ) {
			val = obj.val;
			return *this;
		}
		const oglt& operator= (const int32_t value) {
			#if defined(EWOL_PECISION_FIXPOINT32)
				val = ((int32_t)(value<<16);
			#else
				val = value;
			#endif
			return *this;
		}
		const oglt& operator= (const float value) {
			#if defined(EWOL_PECISION_FIXPOINT32)
				val = ((int32_t)(value*65636.0);
			#else
				val = value;
			#endif
			return *this;
		}
		/*****************************************************
		 *    == operator
		 *****************************************************/
		const bool operator== (const oglt& obj) const {
			return (obj.val == val) ? true : false;
		}
		const bool operator== (const int32_t value) const {
			#if defined(EWOL_PECISION_FIXPOINT32)
				return ((int32_t)(value<<16) == val) ? false : true;
			#else
				return (value == val) ? false : true;
			#endif
		}
		const bool operator== (const float value) const {
			#if defined(EWOL_PECISION_FIXPOINT32)
				return ((int32_t)(value*65636.0) == val) ? false : true;
			#else
				return (value == val) ? false : true;
			#endif
		}
		/*****************************************************
		 *    != operator
		 *****************************************************/
		const bool operator!= (const oglt& obj) const {
			return (obj.val != val) ? false : true;
		}
		const bool operator!= (const int32_t value) const {
			#if defined(EWOL_PECISION_FIXPOINT32)
				return ((int32_t)(value<<16) != val) ? false : true;
			#else
				return (value != val) ? false : true;
			#endif
		}
		const bool operator!= (const float value) const {
			#if defined(EWOL_PECISION_FIXPOINT32)
				return ((int32_t)(value*65636.0) != val) ? false : true;
			#else
				return (value != val) ? false : true;
			#endif
		}
		/*****************************************************
		 *    <= operator
		 *****************************************************/
		const bool operator<= (const oglt& obj) const {
			return (obj.val <= val) ? false : true;
		}
		const bool operator<= (const int32_t value) const {
			#if defined(EWOL_PECISION_FIXPOINT32)
				return ((int32_t)(value<<16) <= val) ? false : true;
			#else
				return (value <= val) ? false : true;
			#endif
		}
		const bool operator<= (const float value) const {
			#if defined(EWOL_PECISION_FIXPOINT32)
				return ((int32_t)(value*65636.0) <= val) ? false : true;
			#else
				return (value <= val) ? false : true;
			#endif
		}
		/*****************************************************
		 *    < operator
		 *****************************************************/
		const bool operator< (const oglt& obj) const {
			return (obj.val < val) ? false : true;
		}
		const bool operator< (const int32_t value) const {
			#if defined(EWOL_PECISION_FIXPOINT32)
				return ((int32_t)(value<<16) < val) ? false : true;
			#else
				return (value < val) ? false : true;
			#endif
		}
		const bool operator< (const float value) const {
			#if defined(EWOL_PECISION_FIXPOINT32)
				return ((int32_t)(value*65636.0) < val) ? false : true;
			#else
				return (value < val) ? false : true;
			#endif
		}
		/*****************************************************
		 *    >= operator
		 *****************************************************/
		const bool operator>= (const oglt& obj) const {
			return (obj.val >= val) ? false : true;
		}
		const bool operator>= (const int32_t value) const {
			#if defined(EWOL_PECISION_FIXPOINT32)
				return ((int32_t)(value<<16) >= val) ? false : true;
			#else
				return (value >= val) ? false : true;
			#endif
		}
		const bool operator>= (const float value) const {
			#if defined(EWOL_PECISION_FIXPOINT32)
				return ((int32_t)(value*65636.0) >= val) ? false : true;
			#else
				return (value >= val) ? false : true;
			#endif
		}
		/*****************************************************
		 *    > operator
		 *****************************************************/
		const bool operator> (const oglt& obj) const {
			return (obj.val > val) ? false : true;
		}
		const bool operator> (const int32_t value) const {
			#if defined(EWOL_PECISION_FIXPOINT32)
				return ((int32_t)(value<<16) > val) ? false : true;
			#else
				return (value > val) ? false : true;
			#endif
		}
		const bool operator> (const float value) const {
			#if defined(EWOL_PECISION_FIXPOINT32)
				return ((int32_t)(value*65636.0) > val) ? false : true;
			#else
				return (value > val) ? false : true;
			#endif
		}
		/*****************************************************
		 *    += operator
		 *****************************************************/
		const oglt& operator+= (const oglt& obj) {
			val += obj.val;
			return *this;
		}
		const oglt& operator+= (const int32_t value) {
			#if defined(EWOL_PECISION_FIXPOINT32)
				val += (value<<16);
			#else
				val += value;
			#endif
			return *this;
		}
		const oglt& operator+= (const float value) {
			#if defined(EWOL_PECISION_FIXPOINT32)
				val += (int32_t)(value*65636.0);
			#else
				val += value;
			#endif
			return *this;
		}
		/*****************************************************
		 *    + operator
		 *****************************************************/
		const oglt& operator+ (const oglt& obj) {
			val += obj.val;
			return *this;
		}
		const oglt& operator+ (const int32_t value) {
			#if defined(EWOL_PECISION_FIXPOINT32)
				val += (value<<16);
			#else
				val += value;
			#endif
			return *this;
		}
		const oglt operator+ (const float value) {
			#if defined(EWOL_PECISION_FIXPOINT32)
				val += (int32_t)(value*65636.0);
			#else
				val += value;
			#endif
			return *this;
		}
		/*
		oglt operator+ (const oglt& obj, const oglt& obj2) {
			oglt tmpVal = obj;
			tmpVal += obj2;
			return tmpVal;
		}
		*/
		/*****************************************************
		 *    -= operator
		 *****************************************************/
		const oglt& operator-= (const oglt& obj) {
			val -= obj.val;
			return *this;
		}
		const oglt& operator-= (const int32_t value) {
			#if defined(EWOL_PECISION_FIXPOINT32)
				val -= (value<<16);
			#else
				val -= value;
			#endif
			return *this;
		}
		const oglt& operator-= (const float value) {
			#if defined(EWOL_PECISION_FIXPOINT32)
				val -= (int32_t)(value*65636.0);
			#else
				val -= value;
			#endif
			return *this;
		}
		/*****************************************************
		 *    - operator
		 *****************************************************/
		const oglt& operator- (const oglt& obj) {
			val -= obj.val;
			return *this;
		}
		const oglt& operator- (const int32_t value) {
			#if defined(EWOL_PECISION_FIXPOINT32)
				val -= (value<<16);
			#else
				val -= value;
			#endif
			return *this;
		}
		const oglt& operator- (const float value) {
			#if defined(EWOL_PECISION_FIXPOINT32)
				val -= (int32_t)(value*65636.0);
			#else
				val -= value;
			#endif
			return *this;
		}
		/*****************************************************
		 *    /= operator
		 *****************************************************/
		const oglt& operator/= (const oglt& obj) {
			#if defined(EWOL_PECISION_FIXPOINT32)
				val = ((int64_t)val<<16) / obj.val;
			#else
				val /= obj.val;
			#endif
			return *this;
		}
		const oglt& operator/= (const int32_t value) {
			#if defined(EWOL_PECISION_FIXPOINT32)
				val = val / value;
			#else
				val /= (float)value;
			#endif
			return *this;
		}
		const oglt& operator/= (const float value) {
			#if defined(EWOL_PECISION_FIXPOINT32)
				val = ((int64_t)val<<16) / (int32_t)(value);
			#else
				val /= value;
			#endif
			return *this;
		}
		/*****************************************************
		 *    / operator
		 *****************************************************/
		oglt operator/ (const oglt& obj) {
			#if defined(EWOL_PECISION_FIXPOINT32)
				val = ((int64_t)val<<16) / obj.val;
			#else
				val /= obj.val;
			#endif
			return *this;
		}
		oglt operator/ (const int32_t value) {
			#if defined(EWOL_PECISION_FIXPOINT32)
				val = val / value;
			#else
				val /= (float)value;
			#endif
			return *this;
		}
		oglt operator/ (const float value) {
			#if defined(EWOL_PECISION_FIXPOINT32)
				val = ((int64_t)val<<16) / (int32_t)(value);
			#else
				val /= value;
			#endif
			return *this;
		}
		/*****************************************************
		 *    *= operator
		 *****************************************************/
		const oglt& operator*= (const oglt& obj) {
			#if defined(EWOL_PECISION_FIXPOINT32)
				int64_t tmpVal = (int64_t)val * (int64_t)obj.val;
				val = tmpVal >> 16;
			#else
				val *= obj.val;
			#endif
			return *this;
		}
		const oglt& operator*= (const int32_t value) {
			#if defined(EWOL_PECISION_FIXPOINT32)
				val = (int32_t)((int64_t)val * (int64_t)value);
			#else
				val *= (float)value;
			#endif
			return *this;
		}
		const oglt& operator*= (const float value) {
			#if defined(EWOL_PECISION_FIXPOINT32)
				int64_t tmpVal = (int64_t)val * (int64_t)(value*65636.0);
				val = tmpVal >> 16;
			#else
				val *= value;
			#endif
			return *this;
		}
		/*****************************************************
		 *    * operator
		 *****************************************************/
		oglt operator* (const oglt& obj) {
			#if defined(EWOL_PECISION_FIXPOINT32)
				int64_t tmpVal = (int64_t)val * (int64_t)obj.val;
				val = tmpVal >> 16;
			#else
				val *= obj.val;
			#endif
			return *this;
		}
		oglt operator* (const int32_t value) {
			#if defined(EWOL_PECISION_FIXPOINT32)
				val = (int32_t)((int64_t)val * (int64_t)value);
			#else
				val *= (float)value;
			#endif
			return *this;
		}
		oglt operator* (const float value) {
			#if defined(EWOL_PECISION_FIXPOINT32)
				int64_t tmpVal = (int64_t)val * (int64_t)(value*65636.0);
				val = tmpVal >> 16;
			#else
				val *= value;
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
/*
inline explicit operator*(const float val, const oglt& obj) {
	float val2 = val;
	val2 *= (float)obj.val;
	return val2;
}
*/
#endif

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
		// Vector2D(const Vector2D<oglt>& obj)     : x((T)obj.x), y((T)obj.y) { };
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
			x += (T)obj.x;
			y += (T)obj.y;
			return *this;
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
			x -= (T)obj.x;
			y -= (T)obj.y;
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
			x /= (T)obj.x;
			y /= (T)obj.y;
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
			x *= (T)obj.x;
			y *= (T)obj.y;
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
		// Vector3D(const Vector3D<oglt>& obj)    : x((T)obj.x), y((T)obj.y), z((T)obj.z) { };
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
			x += (T)obj.x;
			y += (T)obj.y;
			z += (T)obj.z;
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
			x -= (T)obj.x;
			y -= (T)obj.y;
			z -= (T)obj.z;
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
			x /= (T)obj.x;
			y /= (T)obj.y;
			z /= (T)obj.z;
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
			x *= (T)obj.x;
			y *= (T)obj.y;
			z *= (T)obj.z;
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
