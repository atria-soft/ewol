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
	class Padding {
		private:
			float value[4];
		public:
			Padding(void) { }
			Padding(float _xl, float _yt=0, float _xr=0, float _yb=0) {
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
			float xRight(void) const {
				return value[2];
			}
			float yTop(void) const {
				return value[1];
			}
			float yButtom(void) const {
				return value[3];
			}
	};
};

#endif
