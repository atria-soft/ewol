/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_COMMAND_LINE_H__
#define __EWOL_COMMAND_LINE_H__

#include <etk/Types.h>
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
		 * @return The cmdLine Id element
		 */
		etk::UString Get(int32_t id);
		/**
		 * @brief Add one element at the Command Line
		 * @param[in] newElement String in the input that might be added.
		 */
		void Add(etk::UString& newElement);
	};
};

#endif
