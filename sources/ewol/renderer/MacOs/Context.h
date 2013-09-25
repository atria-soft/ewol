/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __MAC_OS_CONTEXT_H__
#define __MAC_OS_CONTEXT_H__

#include <ewol/key.h>

namespace MacOs
{
	
			// return true if a flush is needed
			bool Draw(bool _displayEveryTime);
			/**
			 * @brief The OS inform that the current windows has change his size.
			 * @param[in] _size new size of the windows.
			 */
			void Resize(float _x, float _y);
			
			void SetMouseState(int32_t _id, bool _isDown, float _x, float _y);
			void SetMouseMotion(int32_t _id, float _x, float _y);
			void SetKeyboard(ewol::SpecialKey _keyboardMode, int32_t _unichar, bool _isDown, bool _isAReapeateKey);
	
};

#endif