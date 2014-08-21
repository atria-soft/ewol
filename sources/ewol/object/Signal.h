/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/object/Object.h>

#ifndef __EWOL_SIGNAL_H__
#define __EWOL_SIGNAL_H__

#include <functional>
#include <ewol/object/ParameterList.h>
#include <ewol/object/SignalBase.h>
#include <ewol/object/Object.h>

namespace ewol {
	namespace object {
		class SignalCallerIdentifier {
			public:
				std::weak_ptr<ewol::Object> m_object;
				const char* m_enevntId;
				std::string m_data;
				SignalCallerIdentifier(std::shared_ptr<ewol::Object> _object, const char* _enevntId, const std::string& _data) :
				  m_object(_object),
				  m_enevntId(_enevntId),
				  m_data(_data) {
					// nothing to do ...
				}
		};
		template<typename T> class Signal : public SignalBase {
			private:
				std::vector<std::pair<std::weak_ptr<ewol::Object>,
				                      std::function<void(const T&)>>> m_callerList;
				std::vector<SignalCallerIdentifier> m_serializedCallerList;
			public:
				/**
				 * @brief Create a parameter with a specific type.
				 * @param[in] _objectLink reference on the parameter lister.
				 * @param[in] _name Static name of the parameter.
				 * @param[in] _defaultValue Default value of the parameter.
				 * @param[in] _min Minumum value.
				 * @param[in] _max Maximum value.
				 * @param[in] _description description of the parameter.
				 */
				Signal(ewol::object::SignalList& _objectLink,
				      const std::string& _name,
				      const std::string& _description = "") :
				  SignalBase(_objectLink, _name, _description) {
					
				};
				/**
				 * @brief Destructor.
				 */
				virtual ~Signal() { };
				/**
				 * @brief Bind a callback function to the current signal (generic methis (simplest))
				 * @param[in] _obj Shared pointer on the caller object
				 * @param[in] _func Link on the fuction that might be called (inside a class)
				 * @example signalXXXX.connect(shared_from_this(), &ClassName::onCallbackXXX);
				 */
				template<class TYPE> void bind(std::shared_ptr<ewol::Object> _obj, void (TYPE::*_func)(const T&)) {
					std::shared_ptr<TYPE> obj2 = std::dynamic_pointer_cast<TYPE>(_obj);
					if (obj2 == nullptr) {
						EWOL_ERROR("Can not bind signal ...");
						return;
					}
					m_callerList.push_back(std::make_pair(std::weak_ptr<ewol::Object>(_obj), std::bind(_func, obj2.get(), std::placeholders::_1)));
				}
				/**
				 * @brief Advanced binding a callback function to the current signal.
				 * @param[in] _obj Shared pointer on the caller object
				 * @param[in] _func functor to call (do it yourself)
				 * @example signalXXXX.connect(shared_from_this(), std::bind(&ClassName::onCallbackXXX, this, std::placeholders::_1));
				 */
				void connect(std::shared_ptr<ewol::Object> _obj, std::function<void(const T&)> _function ) {
					m_callerList.push_back(std::make_pair(std::weak_ptr<ewol::Object>(_obj), _function));
				}
				/**
				 * @brief DEPRECATED: old connect signal between Objects
				 */
				void connect(std::shared_ptr<ewol::Object> _obj, const char* _destId=nullptr, const std::string& _data="" ) __attribute__ ((deprecated)) {
					m_serializedCallerList.push_back(SignalCallerIdentifier(_obj, _destId, _data));
				}
				/**
				 * @brief remove link on the signal.
				 * @param[in] _obj shared pointer on the removing object
				 */
				void release(std::shared_ptr<ewol::Object> _obj) {
					for (auto it(m_serializedCallerList.begin()) ; it != m_serializedCallerList.end(); ++it) {
						if (it->m_object.lock() == _obj) {
							m_serializedCallerList.erase(it);
							it = m_serializedCallerList.begin();
						}
					}
					for (auto it(m_callerList.begin()) ; it != m_callerList.end(); ++it) {
						if (it->first.lock() == _obj) {
							m_callerList.erase(it);
							it = m_callerList.begin();
						}
					}
				}
				/**
				 * @brief Generate a signal on all interface listening.
				 * @param[in] _data data to emit
				 */
				void emit(const std::shared_ptr<ewol::Object>& _source, const T& _data) __attribute__ ((deprecated)) {
					emit(_data);
				}
				void emit(const T& _data) {
					// note : this can not emit on function ....
					if (m_serializedCallerList.size()>0 ) {
						std::string stringData = etk::to_string(_data);
						for (auto &it : m_serializedCallerList) {
							std::shared_ptr<ewol::Object> destObject = it.m_object.lock();
							if (destObject == nullptr) {
								// TODO : Remove instance ...
								EWOL_VERBOSE("    nullptr dest");
								continue;
							}
							const char* eventId = m_name.c_str();
							if (it.m_enevntId != nullptr) {
								eventId = it.m_enevntId;
							}
							if (it.m_data.size() <= 0){
								ewol::object::Message tmpMsg(eventId, stringData);
								EWOL_VERBOSE("send message " << tmpMsg);
								destObject->onReceiveMessage(tmpMsg);
							} else {
								// set the user requested data ...
								ewol::object::Message tmpMsg(eventId, it.m_data);
								EWOL_VERBOSE("send message " << tmpMsg);
								destObject->onReceiveMessage(tmpMsg);
							}
						}
					}
					for (auto &it : m_callerList) {
						std::shared_ptr<ewol::Object> destObject = it.first.lock();
						if (destObject == nullptr) {
							// TODO : Remove instance ...
							EWOL_VERBOSE("    nullptr dest");
							continue;
						}
						it.second(_data);
					}
				}
		};
		
		template<> class Signal<void> : public SignalBase {
			private:
				std::vector<std::pair<std::weak_ptr<ewol::Object>,
				                      std::function<void()>>> m_callerList;
				std::vector<SignalCallerIdentifier> m_serializedCallerList;
			public:
				/**
				 * @brief Create a parameter with a specific type.
				 * @param[in] _objectLink reference on the parameter lister.
				 * @param[in] _name Static name of the parameter.
				 * @param[in] _defaultValue Default value of the parameter.
				 * @param[in] _min Minumum value.
				 * @param[in] _max Maximum value.
				 * @param[in] _description description of the parameter.
				 */
				Signal(ewol::object::SignalList& _objectLink,
				      const std::string& _name,
				      const std::string& _description = "") :
				  SignalBase(_objectLink, _name, _description) {
					
				};
				/**
				 * @brief Destructor.
				 */
				virtual ~Signal() { };
				
				/**
				 * @brief Bind a callback function to the current signal (generic methis (simplest))
				 * @param[in] _obj Shared pointer on the caller object
				 * @param[in] _func Link on the fuction that might be called (inside a class)
				 * @example signalXXXX.connect(shared_from_this(), &ClassName::onCallbackXXX);
				 */
				template<class TYPE> void bind(std::shared_ptr<ewol::Object> _obj, void (TYPE::*_func)()) {
					std::shared_ptr<TYPE> obj2 = std::dynamic_pointer_cast<TYPE>(_obj);
					if (obj2 == nullptr) {
						EWOL_ERROR("Can not bind signal ...");
						return;
					}
					m_callerList.push_back(std::make_pair(std::weak_ptr<ewol::Object>(_obj), std::bind(_func, obj2.get(), std::placeholders::_1)));
				}
				/**
				 * @brief Advanced binding a callback function to the current signal.
				 * @param[in] _obj Shared pointer on the caller object
				 * @param[in] _func functor to call (do it yourself)
				 * @example signalXXXX.connect(shared_from_this(), std::bind(&ClassName::onCallbackXXX, this, std::placeholders::_1));
				 */
				void connect(std::shared_ptr<ewol::Object> _obj, std::function<void()> _function ) {
					m_callerList.push_back(std::make_pair(std::weak_ptr<ewol::Object>(_obj), _function));
				}
				/**
				 * @brief DEPRECATED: old connect signal between Objects
				 */
				void connect(std::shared_ptr<ewol::Object> _obj, const char* _destId=nullptr, const std::string& _data="" ) __attribute__ ((deprecated)) {
					m_serializedCallerList.push_back(SignalCallerIdentifier(_obj, _destId, _data));
				}
				/**
				 * @brief remove link on the signal.
				 * @param[in] _obj shared pointer on the removing object
				 */
				void release(std::shared_ptr<ewol::Object> _obj) {
					for (auto it(m_serializedCallerList.begin()) ; it != m_serializedCallerList.end(); ++it) {
						if (it->m_object.lock() == _obj) {
							m_serializedCallerList.erase(it);
							it = m_serializedCallerList.begin();
						}
					}
					for (auto it(m_callerList.begin()) ; it != m_callerList.end(); ++it) {
						if (it->first.lock() == _obj) {
							m_callerList.erase(it);
							it = m_callerList.begin();
						}
					}
				}
				void emit(const std::shared_ptr<ewol::Object>& _source) __attribute__ ((deprecated)) {
					emit();
				}
				void emit() {
					// note : this can not emit on function ....
					std::string stringData;
					for (auto &it : m_serializedCallerList) {
						std::shared_ptr<ewol::Object> destObject = it.m_object.lock();
						if (destObject == nullptr) {
							// TODO : Remove instance ...
							EWOL_VERBOSE("    nullptr dest");
							continue;
						}
						const char* eventId = m_name.c_str();
						if (it.m_enevntId != nullptr) {
							eventId = it.m_enevntId;
						}
						if (it.m_data.size() <= 0){
							ewol::object::Message tmpMsg(eventId, stringData);
							EWOL_VERBOSE("send message " << tmpMsg);
							destObject->onReceiveMessage(tmpMsg);
						} else {
							// set the user requested data ...
							ewol::object::Message tmpMsg(eventId, it.m_data);
							EWOL_VERBOSE("send message " << tmpMsg);
							destObject->onReceiveMessage(tmpMsg);
						}
					}
					for (auto &it : m_callerList) {
						std::shared_ptr<ewol::Object> destObject = it.first.lock();
						if (destObject == nullptr) {
							// TODO : Remove instance ...
							EWOL_VERBOSE("    nullptr dest");
							continue;
						}
						it.second();
					}
				}
		};
		
	};
};
#endif
