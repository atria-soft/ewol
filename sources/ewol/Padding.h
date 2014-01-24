/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_PADDING_H__
#define __EWOL_PADDING_H__

#include <ewol/debug.h>

namespace ewol {
	/**
	 * @breif Simple class to abstarct the padding porperty.
	 */
	class Padding {
		private:
			float value[4]; //!< this represent the 4 padding value Left top right buttom (like css)
		public:
			Padding(void) { }
			Padding(float _xl, float _yt=0, float _xr=0, float _yb=0) {
				setValue(_xl, _yt, _xr, _yb);
			}
			void setValue(float _xl, float _yt=0, float _xr=0, float _yb=0) {
				value[0] = _xl;
				value[1] = _yt;
				value[2] = _xr;
				value[3] = _yb;
			}
			
			float x(void) const {
				return value[0] + value[2];
			}
			float y(void) const {
				return value[1] + value[3];
			}
			float xLeft(void) const {
				return value[0];
			}
			void setXLeft(float _val) {
				value[0] = _val;
			}
			float xRight(void) const {
				return value[2];
			}
			void setXRight(float _val) {
				value[2] = _val;
			}
			float yTop(void) const {
				return value[1];
			}
			void setYTop(float _val) {
				value[1] = _val;
			}
			float yButtom(void) const {
				return value[3];
			}
			void setYButtom(float _val) {
				value[3] = _val;
			}
	};
};

#endif
