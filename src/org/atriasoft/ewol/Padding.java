/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/Stream.hpp>

namespace ewol {
	/**
	 * @breif Simple class to abstarct the padding porperty.
	 */
	class Padding {
		private:
			float this.value[4]; //!< this represent the 4 padding value Left top right buttom (like css)
		public:
			Padding();
			Padding(float _xl, float _yt=0.0f, float _xr=0.0f, float _yb=0.0f);
			void setValue(float _xl, float _yt=0.0f, float _xr=0.0f, float _yb=0.0f);
			float x() ;
			float y() ;
			float xLeft() ;
			void setXLeft(float _val);
			float xRight() ;
			void setXRight(float _val);
			float yTop() ;
			void setYTop(float _val);
			float yButtom() ;
			void setYButtom(float _val);
			/**
			 * @brief Add a vector to this one 
			 * @param _v The vector to add to this one
			 */
			Padding operator+=( Padding _v);
			//! @previous
			Padding operator+( Padding _v);
			
	};
	etk::Stream operator +(etk::Stream _os,  ewol::Padding _obj);
};

