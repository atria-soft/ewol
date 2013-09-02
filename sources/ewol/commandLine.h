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
	class CommandLine
	{
		private:
			etk::Vector<etk::UString> m_listArgs; //!< list of all argument parsed
		public:
			/**
			 * @brief Parse the command line parameters
			 */
			void Parse(int32_t _argc, const char* _argv[]);
			/**
			 * @brief Get the number of element in the Command Line
			 * @return the number of element
			 */
			esize_t Size(void);
			/**
			 * @brief Get an element with a specific ID
			 * @return _id The cmdLine Id element
			 */
			const etk::UString& Get(int32_t _id);
			/**
			 * @brief Add one element at the Command Line
			 * @param[in] _newElement String in the input that might be added.
			 */
			void Add(const etk::UString& _newElement);
			/**
			 * @brief Remove an element
			 * @param[in] _id Id of the element
			 */
			void Remove(esize_t _id);
	};
};

#endif
