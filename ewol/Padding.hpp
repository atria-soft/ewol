/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>

namespace ewol {
	/**
	 * @breif Simple class to abstarct the padding porperty.
	 */
	class Padding {
		private:
			float m_value[4]; //!< this represent the 4 padding value Left top right buttom (like css)
		public:
			Padding();
			Padding(float _xl, float _yt=0.0f, float _xr=0.0f, float _yb=0.0f);
			void setValue(float _xl, float _yt=0.0f, float _xr=0.0f, float _yb=0.0f);
			float x() const;
			float y() const;
			float xLeft() const;
			void setXLeft(float _val);
			float xRight() const;
			void setXRight(float _val);
			float yTop() const;
			void setYTop(float _val);
			float yButtom() const;
			void setYButtom(float _val);
			/**
			 * @brief Add a vector to this one 
			 * @param _v The vector to add to this one
			 */
			Padding& operator+=(const Padding& _v);
			//! @previous
			Padding operator+(const Padding& _v);
			
	};
	std::ostream& operator <<(std::ostream& _os, const ewol::Padding& _obj);
};

