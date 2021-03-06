/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ewol/debug.hpp>
#include <ewol/object/Object.hpp>

namespace ewol {
	namespace object {
		class Worker;
		using WorkerShared = ememory::SharedPtr<ewol::object::Worker>;
		using WorkerWeak = ememory::WeakPtr<ewol::object::Worker>;
		/**
		 * @brief A worker might not been possesed by someone, then the system might keep a pointer on it.
		 */
		class Worker : public ewol::Object {
			protected:
				/**
				 * @brief Constructor.
				 */
				Worker();
				void init() override;
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
				void destroy() override;
		};
	}
}
