/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __ETK_MATH_VECTOR4D_H__
#define __ETK_MATH_VECTOR4D_H__

namespace etk
{
	template <typename T> class Vector4D
	{
		public:
			T x;
			T y;
			union {
				T z;
				T width;
			};
			union {
				T w;
				T height;
			};
		public:
			/*****************************************************
			 *    Constructor
			 *****************************************************/
			 Vector4D(void) : x(0), y(0), z(0), w(0) { };
			 Vector4D(double _x, double _y, double _z, double _w)    : x(_x), y(_y), z(_z), w(_w) { };
			 Vector4D(float _x, float _y, float _z, float _w)       : x(_x), y(_y), z(_z), w(_w) { };
			 Vector4D(int32_t _x, int32_t _y, int32_t _z, int32_t _w) : x(_x), y(_y), z(_z), w(_w) { };
			 Vector4D(const Vector4D<double>& obj)  : x((T)obj.x), y((T)obj.y), z((T)obj.z), w((T)obj.w) { };
			 Vector4D(const Vector4D<float>& obj)   : x((T)obj.x), y((T)obj.y), z((T)obj.z), w((T)obj.w) { };
			 Vector4D(const Vector4D<int32_t>& obj) : x((T)obj.x), y((T)obj.y), z((T)obj.z), w((T)obj.w) { };
			~Vector4D(void) { };
			/*****************************************************
			 *    = assigment
			 *****************************************************/
			const Vector4D<T>& operator= (const Vector4D<T>& obj ) {
				x = (T)obj.x;
				y = (T)obj.y;
				z = (T)obj.z;
				w = (T)obj.w;
				return *this;
			}
			/*****************************************************
			 *    == operator
			 *****************************************************/
			bool  operator== (const Vector4D<T>& obj) const {
				if ((T)obj.x == x && (T)obj.y == y && (T)obj.z == z) {
					return true;
				}
				return false;
			}
			/*****************************************************
			 *    != operator
			 *****************************************************/
			bool  operator!= (const Vector4D<T>& obj) const {
				if ((T)obj.x == x && (T)obj.y == y && (T)obj.z == z && (T)obj.w == w) {
					return false;
				}
				return true;
			}
			/*****************************************************
			 *    += operator
			 *****************************************************/
			const Vector4D<T>& operator+= (const Vector4D<T>& obj) {
				x += (T)obj.x;
				y += (T)obj.y;
				z += (T)obj.z;
				w += (T)obj.w;
				return *this;
			}
			/*****************************************************
			 *    + operator
			 *****************************************************/
			Vector4D<T> operator+ (const Vector4D<T>& obj) {
				Vector4D<T> tmpp(x,y,y);
				tmpp.x += (T)obj.x;
				tmpp.y += (T)obj.y;
				tmpp.z += (T)obj.z;
				tmpp.w += (T)obj.w;
				return *this;
			}
			/*****************************************************
			 *    -= operator
			 *****************************************************/
			const Vector4D<T>& operator-= (const Vector4D<T>& obj) {
				x -= (T)obj.x;
				y -= (T)obj.y;
				z -= (T)obj.z;
				w -= (T)obj.w;
				return *this;
			}
			/*****************************************************
			 *    - operator
			 *****************************************************/
			Vector4D<T> operator- (const Vector4D<T>& obj) {
				Vector4D<T> tmpp(x,y,y);
				tmpp.x -= (T)obj.x;
				tmpp.y -= (T)obj.y;
				tmpp.z -= (T)obj.z;
				tmpp.w -= (T)obj.w;
				return *this;
			}
			/*****************************************************
			 *    /= operator
			 *****************************************************/
			const Vector4D<T>& operator/= (const Vector4D<T>& obj) {
				x /= (T)obj.x;
				y /= (T)obj.y;
				z /= (T)obj.z;
				w /= (T)obj.w;
				return *this;
			}
			/*****************************************************
			 *    / operator
			 *****************************************************/
			Vector4D<T> operator/ (const Vector4D<T>& obj) {
				Vector4D<T> tmpp(x,y,y);
				tmpp.x /= (T)obj.x;
				tmpp.y /= (T)obj.y;
				tmpp.z /= (T)obj.z;
				tmpp.w /= (T)obj.w;
				return *this;
			}
			/*****************************************************
			 *    *= operator
			 *****************************************************/
			const Vector4D<T>& operator*= (const Vector4D<T>& obj) {
				x *= (T)obj.x;
				y *= (T)obj.y;
				z *= (T)obj.z;
				w *= (T)obj.w;
				return *this;
			}
			/*****************************************************
			 *    * operator
			 *****************************************************/
			Vector4D<T> operator* (const Vector4D<T>& obj) {
				Vector4D<T> tmpp(x,y,y);
				tmpp.x *= (T)obj.x;
				tmpp.y *= (T)obj.y;
				tmpp.z *= (T)obj.z;
				tmpp.w *= (T)obj.w;
				return *this;
			}
			/*****************************************************
			 *    ++ operator
			 *****************************************************/
			Vector4D<T>& operator++() // prefix
			{
				++x;
				++y;
				++z;
				++w;
				return *this;
			}
			Vector4D<T> operator++(int unused) // postfix
			{
				Vector4D<T> result = *this;
				++(*this);
				return result;
			}
			/*****************************************************
			 *    -- operator
			 *****************************************************/
			Vector4D<T>& operator--() // prefix
			{
				--x;
				--y;
				--z;
				--w;
				return *this;
			}
			Vector4D<T> operator--(int unused) // postfix
			{
				Vector4D<T> result = *this;
				--(*this);
				return result;
			}
	};
};

#endif

