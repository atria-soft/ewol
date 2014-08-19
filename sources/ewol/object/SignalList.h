/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */


#ifndef __EWOL_SIGNAL_LIST_H__
#define __EWOL_SIGNAL_LIST_H__

#include <vector>
#include <map>

namespace ewol {
	namespace object {
		class SignalBase;
		class SignalList {
			friend class ewol::object::SignalBase; // to register parameter in the list.
			private:
				std::vector<ewol::object::SignalBase*> m_list;  //!< list of availlable Parameters
			public:
				/**
				 * @brief Constructor.
				 */
				SignalList();
				/**
				 * @brief Destructor.
				 */
				virtual ~SignalList();
				/**
				 * @brief Register a parameter class pointer in the List of parameters
				 * @note This class does not destroy the parameter pointer!!!
				 * @param[in] pointerOnParameter Pointer on the parameter that might be added.
				 */
				void signalAdd(SignalBase* _pointerOnParameter);
				#if 0
				/**
				 * @brief Set a specific value to the parameter reference name.
				 * @param[in] parameter The parameter string name.
				 * @param[in] value The new value of the parameter (string).
				 * @return true Parameter update.
				 * @return false Parameter not update.
				 */
				bool parameterSet(const std::string& _parameter, const std::string& _value);
				/**
				 * @brief Get a specific value of the parameter reference name.
				 * @param[in] parameter The parameter string name.
				 * @return The value of the parameter (string).
				 */
				std::string parameterGet(const std::string& _parameter) const;
				/**
				 * @brief Display all the parameter value with there name.
				 * @param[in] changeOnly check at true if the user want to display only parameter that are not at default value.
				 */
				void parameterDisplay(bool _changeOnly = false) const;
				/**
				 * @brief Called when a parameter change value.
				 * @param[in] _paramPointer Pointer on the parameter (to know which parameter have change);
				 */
				virtual void onParameterChangeValue(const ewol::object::ParameterRef& _paramPointer) { };
				/**
				 * @brief Get All the parameter configuration:
				 * @return map on the parameters
				 */
				std::map<std::string, std::string> parameterGetAll(bool _notIfDefault=true) const;
				#endif
				/**
				 * @brief Register an Event an named widget. @see registerOnEvent
				 * @param[in] _destinationObject pointer on the object that might be call when an event is generated
				 * @param[in] _objectName Name of the object.
				 * @param[in] _eventId Event generate inside the object.
				 * @param[in] _eventIdgenerated event generated when call the distant EObject.onReceiveMessage(...)
				 * @param[in] _overloadData When the user prever to receive a data specificly for this event ...
				 * @note : To used when NOT herited from this object.
				 */
				// TODO : Change name : registerOnSignal
				void registerOnObjectEvent(const std::shared_ptr<ewol::Object>& _destinationObject,
				                           const std::string& _objectName,
				                           const char * _eventId,
				                           const char * _eventIdgenerated = nullptr,
				                           const std::string& _overloadData="");
				
			public:
				/**
				 * @brief Register an Object over an other to get event on the second...
				 * @param[in] _destinationObject pointer on the object that might be call when an event is generated
				 * @param[in] _eventId Event generate inside the object (note : "*" event register on all event generated )
				 * @param[in] _eventIdgenerated event generated when call the distant Object.onReceiveMessage(...)
				 * @param[in] _overloadData When the user prever to receive a data specificly for this event ...
				 */
				void registerOnEvent(const std::shared_ptr<ewol::Object>& _destinationObject,
				                     const char * _eventId,
				                     const char * _eventIdgenerated = nullptr,
				                     const std::string& _overloadData = "");
				/**
				 * @brief Un-Register an Object over an other.
				 * @param[in] _destinationObject pointer on the object that might be call when an event is generated
				 * @param[in] _eventId Event generate inside the object (nullptr to remove all event on this object)
				 */
				void unRegisterOnEvent(const std::shared_ptr<ewol::Object>& _destinationObject,
				                       const char * _eventId = nullptr);
				/**
				 * @brief Receive a message from an other Object with a specific eventId and data
				 * @param[in] _msg Message handle
				 */
				virtual void onReceiveMessage(const ewol::object::Message& _msg) { };
		};
	};
};

#endif
