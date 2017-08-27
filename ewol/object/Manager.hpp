/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/object/Object.hpp>
#include <esignal/Signal.hpp>
#include <ewol/event/Time.hpp>
#include <echrono/Steady.hpp>
#include <echrono/Duration.hpp>

namespace ewol {
	class Context;
	namespace object {
		class Manager : public esignal::Interface {
			protected:
				std::recursive_mutex m_mutex;
			private:
				etk::Vector<ewol::ObjectWeak> m_eObjectList; // all widget allocated  == > all time increment ... never removed ...
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
				//! @not_in_doc
				friend class ewol::Object;
				/**
				 * @brief Internal API that used only with Object toi reference itself in the manager.
				 * @note The manager remove the object when the refecence Low down 1 (last keeper)
				 * @param[in] _object Reference shared pointer on the object
				 */
				void add(const ewol::ObjectShared& _object);
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
				ewol::ObjectShared get(const etk::String& _name);
			public:
				/**
				 * @brief retrive an object with his name
				 * @param[in] _name Name of the object
				 * @return the requested object or nullptr
				 */
				ewol::ObjectShared getObjectNamed(const etk::String& _name);
			private:
				etk::Vector<ewol::ObjectShared> m_workerList;
			public:
				/**
				 * @brief Add a worker on the system list.
				 * @param[in] _worker Worker to add in the list.
				 */
				void workerAdd(const ewol::ObjectShared& _worker);
				/**
				 * @brief Remove a worker on the system list.
				 * @param[in] _worker Worker to add in the list.
				 */
				void workerRemove(const ewol::ObjectShared& _worker);
			public:
				esignal::Signal<ewol::event::Time> periodicCall;
			private:
				echrono::Clock m_applWakeUpTime; //!< Time of the application initialize
				echrono::Clock m_lastPeriodicCallTime; //!< last call time ...
			public: // ewol system internal :
				/**
				 * @brief Call every time we can with the current time
				 * @param[in] _localTime Current system Time.
				 */
				void timeCall(const echrono::Clock& _localTime);
				/**
				 * @brief If the application is suspended The Ewol Object manager does not know it, just call this to update delta call
				 * @param[in] _localTime Current system Time.
				 */
				void timeCallResume(const echrono::Clock& _localTime);
				/**
				 * @breif check if the Interface have some user that request a periodic call
				 * @return true, have some periodic event...
				 */
				bool timeCallHave();
				
		};
	};
};
