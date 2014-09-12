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
		#undef __class__
		#define __class__ "object::Signal<T>"
		template<typename T> class Signal : public SignalBase {
			private:
				std::vector<std::pair<std::weak_ptr<ewol::Object>,
				                      std::function<void(const T&)>>> m_callerList;
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
				 * @example signalXXXX.bind(shared_from_this(), &ClassName::onCallbackXXX);
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
				 * @brief remove link on the signal.
				 * @param[in] _obj shared pointer on the removing object
				 */
				void release(std::shared_ptr<ewol::Object> _obj) {
					auto it(m_callerList.begin());
					while(it != m_callerList.end()) {
						if (it->first.lock() == _obj) {
							it = m_callerList.erase(it);
						} else {
							++it;
						}
					}
				}
				/**
				 * @brief Generate a signal on all interface listening.
				 * @param[in] _data data to emit
				 */
				void emit(const T& _data) {
					#ifdef DEBUG
						int32_t tmpID = m_uidSignal++;
						ewol::Object* srcObject = dynamic_cast<ewol::Object*>(&m_objectLink);
						if (srcObject != nullptr) {
							EWOL_DEBUG("emit signal{" << tmpID << "} : " << srcObject->getObjectType() << " signal='" << m_name << "' data='" << etk::to_string(_data) << "' to:");
						} else {
							EWOL_DEBUG("emit signal{" << tmpID << "} : signal='" << m_name << "' data='" << etk::to_string(_data) << "' to:");
						}
					#endif
					for (auto &it : m_callerList) {
						std::shared_ptr<ewol::Object> destObject = it.first.lock();
						if (destObject == nullptr) {
							// TODO : Remove instance ...
							EWOL_VERBOSE("    nullptr dest");
							continue;
						}
						#ifdef DEBUG
							if (srcObject != nullptr) {
								EWOL_DEBUG("     signal{" << tmpID << "} : [" << destObject->getId() << "]" << destObject->getObjectType());
							} else {
								EWOL_DEBUG("     signal{" << tmpID << "} : [" << destObject->getId() << "]" << destObject->getObjectType());
							}
						#endif
						it.second(_data);
					}
				}
		};
		#undef __class__
		#define __class__ "object::Signal<void>"
		template<> class Signal<void> : public SignalBase {
			private:
				std::vector<std::pair<std::weak_ptr<ewol::Object>, std::function<void()>>> m_callerList;
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
					m_callerList.push_back(std::make_pair(std::weak_ptr<ewol::Object>(_obj), std::bind(_func, obj2.get())));
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
				 * @brief remove link on the signal.
				 * @param[in] _obj shared pointer on the removing object
				 */
				void release(std::shared_ptr<ewol::Object> _obj) {
					auto it(m_callerList.begin());
					while(it != m_callerList.end()) {
						if (it->first.lock() == _obj) {
							it = m_callerList.erase(it);
						} else {
							++it;
						}
					}
				}
				void emit() {
					#ifdef DEBUG
						int32_t tmpID = m_uidSignal++;
						ewol::Object* srcObject = dynamic_cast<ewol::Object*>(&m_objectLink);
						if (srcObject != nullptr) {
							EWOL_DEBUG("emit signal{" << tmpID << "} : " << srcObject->getObjectType() << " signal='" << m_name << "' BANG!!! to:");
						} else {
							EWOL_DEBUG("emit signal{" << tmpID << "} : signal='" << m_name << "' to:");
						}
					#endif
					for (auto &it : m_callerList) {
						std::shared_ptr<ewol::Object> destObject = it.first.lock();
						if (destObject == nullptr) {
							// TODO : Remove instance ...
							EWOL_VERBOSE("    nullptr dest");
							continue;
						}
						#ifdef DEBUG
							if (srcObject != nullptr) {
								EWOL_DEBUG("     signal{" << tmpID << "} : [" << destObject->getId() << "]" << destObject->getObjectType());
							} else {
								EWOL_DEBUG("     signal{" << tmpID << "} : [" << destObject->getId() << "]" << destObject->getObjectType());
							}
						#endif
						it.second();
					}
				}
		};
		#undef __class__
		#define __class__ nullptr
	};
};
#endif
