/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_KEY_TYPE_H__
#define __EWOL_KEY_TYPE_H__

#include <etk/types.h>
#include <etk/Stream.h>

namespace ewol {
	namespace key {
		/**
		 * @brief type of input : Note that the keyboard is not prevent due to the fact that data is too different
		 */
		enum type {
			typeUnknow = 0, //!< Unknow input Type
			typeMouse,      //!< Mouse type
			typeFinger,     //!< Finger type
			typeStylet,     //!< Stylet type
			typeCount       //!< number of types
		};
	};
	/**
	 * @brief Debug operator To display the curent element in a Human redeable information
	 */
	etk::CCout& operator <<(etk::CCout& _os, const enum ewol::key::type _obj);
};



#endif


