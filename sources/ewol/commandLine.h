/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_COMMAND_LINE_H__
#define __EWOL_COMMAND_LINE_H__

#include <etk/types.h>
#include <etk/UString.h>

namespace ewol
{
	namespace commandLine {
		/**
		 * @brief Remove all the element bufferised in the commandLine system
		 */
		void Clean(void);
		/**
		 * @brief Get the number of element in the Command Line
		 * @return the number of element
		 */
		int32_t Size(void);
		/**
		 * @brief Get an element with a specific ID
		 * @return _id The cmdLine Id element
		 */
		etk::UString Get(int32_t _id);
		/**
		 * @brief Add one element at the Command Line
		 * @param[in] _newElement String in the input that might be added.
		 */
		void Add(const etk::UString& _newElement);
	};
};

#endif
