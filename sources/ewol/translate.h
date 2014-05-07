/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_TRANSLATE_H__
#define __EWOL_TRANSLATE_H__

#include <etk/types.h>

namespace ewol {
	/**
	 * @brief This is a simple interface to converte application display string in a generic current system language
	 * @note: The current name of language reprenent the file name, then if you want to get the machine language in an other than generic passed, juste add it.
	 * Generic langage char: (all translation might be done in UTF-8 this simplify interface)
	 * English : "EN"
	 * French : "FR"
	 * German : "DE"
	 * Spanish : "SP"
	 * Japanese : "JA"
	 * Italian : "IT"
	 * Korean : "KO"
	 * Russian : "RU"
	 * Portuguese, Brazilian : "PT"
	 * Chinese : "ZH"
	 */
	namespace translate {
		/**
		 * @brief Set the path folder of the translation files
		 * @param[in] _lib Library name that the path depend
		 * @param[in] _path ETK generic path (DATA:... or /xxx)
		 * @param[in] _major This path is the major path (The last loaded, the one wichi overload all)
		 */
		void addPath(const std::string& _lib, const std::string& _path, bool _major = false);
		/**
		 * @brief Get the current paths of the library
		 * @param[in] _lib Library name that the path depend
		 * @return Path name.
		 */
		const std::string& getPaths(const std::string& _lib);
		/**
		 * @brief Set the default language to load data (the default language might contain all internal data for the basic application)
		 * @param[in] _lang Language to load : ("EN" for english, "FR" for french, "DE" for German, "SP" for spanish ...)
		 */
		void setLanguageDefault(const std::string& _lang);
		/**
		 * @brief Get the current language selected
		 * @return The 2/3 char defining the language
		 */
		const std::string& getLanguageDefault(void);
		/**
		 * @brief Set the language to load data. when no data availlable, we get the default language.
		 * @param[in] _lang Language to load : ("EN" for english, "FR" for french, "DE" for German, "SP" for spanish ...)
		 */
		void setLanguage(const std::string& _lang);
		/**
		 * @brief Get the current language loaded
		 * @return The 2/3 char defining the language
		 */
		const std::string& getLanguage(void);
		/**
		 * @brief Automatic detection of the system language
		 */
		void autoDetectLanguage(void);
		/**
		 * @brief Translate a specific text (if not find, it will be retured the same text).
		 * @param[in] _instance Text to translate.
		 * @return The tranlated text.
		 */
		const std::string& get(const std::string& _instance);
	};
};
// Here we define a simple macro to Translate all string simply:
#define TRANSLATE(a) ewol::translate::get(a)

#endif

