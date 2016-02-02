/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <ewol/debug.h>

namespace ewol {
	/**
	 * @breif Simple class to abstarct the padding porperty.
	 */
	class Padding {
		private:
			float m_value[4]; //!< this represent the 4 padding value Left top right buttom (like css)
		public:
			Padding() { }
			Padding(float _xl, float _yt=0, float _xr=0, float _yb=0) {
				setValue(_xl, _yt, _xr, _yb);
			}
			void setValue(float _xl, float _yt=0, float _xr=0, float _yb=0) {
				m_value[0] = _xl;
				m_value[1] = _yt;
				m_value[2] = _xr;
				m_value[3] = _yb;
			}
			
			float x() const {
				return m_value[0] + m_value[2];
			}
			float y() const {
				return m_value[1] + m_value[3];
			}
			float xLeft() const {
				return m_value[0];
			}
			void setXLeft(float _val) {
				m_value[0] = _val;
			}
			float xRight() const {
				return m_value[2];
			}
			void setXRight(float _val) {
				m_value[2] = _val;
			}
			float yTop() const {
				return m_value[1];
			}
			void setYTop(float _val) {
				m_value[1] = _val;
			}
			float yButtom() const {
				return m_value[3];
			}
			void setYButtom(float _val) {
				m_value[3] = _val;
			}
			/**
			 * @brief Add a vector to this one 
			 * @param _v The vector to add to this one
			 */
			Padding& operator+=(const Padding& _v) {
				m_value[0] += _v.m_value[0];
				m_value[1] += _v.m_value[1];
				m_value[2] += _v.m_value[2];
				m_value[3] += _v.m_value[3];
				return *this;
			}
			//! @previous
			Padding operator+(const Padding& _v) {
				return Padding(m_value[0] + _v.m_value[0],
				               m_value[1] + _v.m_value[1],
				               m_value[2] + _v.m_value[2],
				               m_value[3] + _v.m_value[3]);
			}
			
	};
	std::ostream& operator <<(std::ostream& _os, const ewol::Padding& _obj);
};

