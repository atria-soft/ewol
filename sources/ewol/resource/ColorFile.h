/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __SIMPLE_COLOR_FILE_H__
#define __SIMPLE_COLOR_FILE_H__

#include <etk/types.h>
#include <etk/Color.h>
#include <etk/Hash.h>
#include <ewol/debug.h>
#include <ewol/resource/Resource.h>

namespace ewol {
	namespace resource {
		/**
		 * @brief ColorFile is a Resource designed to be specific with the theme (for example black, or white or orange ...)
		 */
		class ColorFile : public ewol::Resource {
			private:
				etk::Hash<etk::Color<float> > m_list; //!< List of all color in the file
				etk::Color<float> m_errorColor; //!< Error returned color
			protected:
				/**
				 * @brief Constructor of the color property file
				 * @param[in] _filename Name of the file needed
				 */
				ColorFile(const std::string& _filename);
				/**
				 * @brief Simple Destructor of this class (nothing specific ...)
				 */
				virtual ~ColorFile();
			public:
				/**
				 * @brief Set the error color.
				 * @param[in] _errorColor Color that might be set when not finding a color
				 */
				void setErrorColor(const etk::Color<float>& _errorColor) {
					m_errorColor = _errorColor;
				}
				/**
				 * @brief Request the presence of a specific color.
				 * @param[in] _paramName Name of the color.
				 * @return A unique ID of the color (or -1 if an error occured).
				 */
				int32_t request(const std::string& _paramName);
				/**
				 * @brief Get the associated color of the ID.
				 * @param[in] _Id Id of the color.
				 * @return The requested color.
				 */
				const etk::Color<float>& get(int32_t _id) {
					if (_id<0) {
						return m_errorColor;
					}
					return m_list[_id];
				};
				/**
				 * @brief Get All color name
				 * @return list of all color existing
				 */
				std::vector<std::string> getColors() const {
					return m_list.getKeys();
				}
			public: // herited function:
				void reload();
			public:
				/**
				 * @brief keep the resource pointer.
				 * @note Never free this pointer by your own...
				 * @param[in] _filename Name of the configuration file.
				 * @return pointer on the resource or NULL if an error occured.
				 */
				static ewol::resource::ColorFile* keep(const std::string& _filename);
				/**
				 * @brief release the keeped resources
				 * @param[in,out] reference on the object pointer
				 */
				static void release(ewol::resource::ColorFile*& _object);
		};
	};
};

#endif

