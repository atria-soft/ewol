/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_WIDGET_COMPOSER_H__
#define __EWOL_WIDGET_COMPOSER_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Container.h>

namespace widget
{
	/**
	 * @brief the composer widget is a widget that create a link on a string.file to parse the data and generate some widget tree
	 */
	class Composer : public widget::Container
	{
		public:
			typedef enum {
				None,
				String,
				File
			} composerMode_te;
		public:
			/**
			 * @brief Constructor
			 */
			Composer(void);
			/**
			 * @brief Constructor
			 * @param[in] mode mode of parsing the string
			 * @param[in] data File/directString data to generate compositing of the widget..
			 */
			Composer(composerMode_te mode, const etk::UString& data);
			/**
			 * @brief Destructor
			 */
			~Composer(void);
			/**
			 * @brief Load a composition with a file
			 * @param[in] fileName Name of the file
			 * @return true ==> all done OK
			 * @return false ==> some error occured
			 */
			bool LoadFromFile(const etk::UString& fileName);
			/**
			 * @brief Load a composition with a file
			 * @param[in] composerXmlString xml to parse directly
			 * @return true ==> all done OK
			 * @return false ==> some error occured
			 */
			bool LoadFromString(const etk::UString& composerXmlString);
		private:
			/**
			 * @brief Load a composition with a file.
			 * @param[in] data pointer on the file data.
			 * @return true ==> all done OK.
			 * @return false ==> some error occured.
			 */
			bool CommonLoadXML(const char* data);
	};
};

#endif