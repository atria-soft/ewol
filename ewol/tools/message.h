/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <etk/Color.h>
#include <ewol/resource/ColorFile.h>
#include <list>

namespace ewol {
	namespace tools {
		namespace message {
			enum class type {
				info, //!< information message pop-up
				warning, //!< warning message pop-up
				error, //!< Error message pop-up
				critical //!< Critical message pop-up
			};
			/**
			 * @brief Create a simple pop-up message on the screen for application error.
			 * @param[in] _type Type of the error.
			 * @param[in] _message message to display (decorated text)
			 */
			void create(enum ewol::tools::message::type _type, const std::string& _message);
			/**
			 * @brief Create a simple information message
			 * @param[in] _message message to display (decorated text)
			 */
			void displayInfo(const std::string& _message);
			/**
			 * @brief Create a simple warning message
			 * @param[in] _message message to display (decorated text)
			 */
			void displayWarning(const std::string& _message);
			/**
			 * @brief Create a simple error message
			 * @param[in] _message message to display (decorated text)
			 */
			void displayError(const std::string& _message);
			/**
			 * @brief Create a simple critical message
			 * @param[in] _message message to display (decorated text)
			 */
			void displayCritical(const std::string& _message);
		}
	}
}

