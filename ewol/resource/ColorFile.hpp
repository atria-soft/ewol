/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/Color.hpp>
#include <etk/Map.hpp>
#include <ewol/debug.hpp>
#include <gale/resource/Resource.hpp>

namespace ewol {
	namespace resource {
		/**
		 * @brief ColorFile is a Resource designed to be specific with the theme (for example black, or white or orange ...)
		 */
		class ColorFile : public gale::Resource {
			private:
				etk::Map<etk::String, etk::Color<float> > m_list; //!< List of all color in the file
				etk::Color<float> m_errorColor; //!< Error returned color
			protected:
				/**
				 * @brief Constructor of the color property file
				 * @param[in] _uri Name of the file needed
				 */
				ColorFile();
				void init(const etk::Uri& _uri);
			public:
				DECLARE_RESOURCE_NAMED_FACTORY(ColorFile);
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
				int32_t request(const etk::String& _paramName);
				/**
				 * @brief Get the associated color of the ID.
				 * @param[in] _Id Id of the color.
				 * @return The requested color.
				 */
				const etk::Color<float>& get(int32_t _id) const {
					if (_id < 0) {
						return m_errorColor;
					}
					return m_list.getValue(_id);
				};
				/**
				 * @brief Get All color name
				 * @return list of all color existing
				 */
				etk::Vector<etk::String> getColors() const {
					return m_list.getKeys();
				}
			public: // herited function:
				void reload();
		};
	};
};

