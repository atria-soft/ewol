/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <ewol/debug.h>
#include <ewol/object/Object.h>

namespace ewol {
	namespace object {
		/**
		 * @brief A worker might not been possesed by someone, then the system might keep a pointer on it.
		 */
		class Worker : public ewol::Object {
			protected:
				/**
				 * @brief Constructor.
				 */
				Worker();
				void init();
				//! @previous
				void init(const std::string& _name);
			public:
				/**
				 * @brief Factory
				 */
				DECLARE_FACTORY(Worker);
				/**
				 * @brief Destructor
				 */
				virtual ~Worker();
			public:
				virtual void destroy();
		};
	}
}
