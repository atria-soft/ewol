/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <ewol/object/Object.h>
#include <ewol/signal/Signal.h>
#include <ewol/event/Time.h>

namespace ewol {
	class Context;
	namespace object {
		class Manager : public ewol::signal::Interface {
			private:
				std::vector<std::weak_ptr<ewol::Object>> m_eObjectList; // all widget allocated  == > all time increment ... never removed ...
				Context& m_context;
			public:
				Manager(Context& _context);
				virtual ~Manager();
				/**
				 * @brief remove all resources (un-init) out of the destructor (due to the system implementation)
				 */
				void unInit();
				/**
				 * @brief Get the number of loaded object in the system
				 * @return number of Object
				 */
				int32_t getNumberObject();
				/**
				 * @brief Display all object Open.
				 */
				void displayListObject();
			private:
				//! @not-in-doc
				friend class ewol::Object;
				/**
				 * @brief Internal API that used only with Object toi reference itself in the manager.
				 * @note The manager remove the object when the refecence Low down 1 (last keeper)
				 * @param[in] _object Reference shared pointer on the object
				 */
				void add(const std::shared_ptr<ewol::Object>& _object);
			public:
				/**
				 * @brief clean the weak pointer list (remove weak_ptr that is remoed)
				 */
				void cleanInternalRemoved();
				/**
				 * @brief Retrive an Object with his name
				 * @param[in] _name Name of the Object
				 * @return Pointer on the finded Object.
				 */
				std::shared_ptr<ewol::Object> get(const std::string& _name);
			public:
				/**
				 * @brief retrive an object with his name
				 * @param[in] _name Name of the object
				 * @return the requested object or nullptr
				 */
				std::shared_ptr<ewol::Object> getObjectNamed(const std::string& _name);
			private:
				std::vector<std::shared_ptr<ewol::Object>> m_workerList;
			public:
				/**
				 * @brief Add a worker on the system list.
				 * @param[in] _worker Worker to add in the list.
				 */
				void workerAdd(const std::shared_ptr<ewol::Object>& _worker);
				/**
				 * @brief Remove a worker on the system list.
				 * @param[in] _worker Worker to add in the list.
				 */
				void workerRemove(const std::shared_ptr<ewol::Object>& _worker);
			public:
				ewol::Signal<ewol::event::Time> periodicCall;
			private:
				int64_t m_applWakeUpTime; //!< Time of the application initialize
				int64_t m_lastPeriodicCallTime; //!< last call time ...
			public: // ewol system internal :
				/**
				 * @brief Call every time we can with the current time
				 * @param[in] _localTime Current system Time.
				 */
				void timeCall(int64_t _localTime);
				/**
				 * @brief If the application is suspended The Ewol Object manager does not know it, just call this to update delta call
				 * @param[in] _localTime Current system Time.
				 */
				void timeCallResume(int64_t _localTime);
				/**
				 * @breif check if the Interface have some user that request a periodic call
				 * @return true, have some periodic event...
				 */
				bool timeCallHave();
				
		};
	};
};
