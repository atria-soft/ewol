/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_SIGNAL_H__
#define __EWOL_SIGNAL_H__

#include <functional>
#include <ewol/signal/Base.h>

namespace ewol {
	#undef __class__
	#define __class__ "Signal<T>"
	template<typename T> class Signal : public signal::Base {
		private:
			std::vector<std::pair<std::weak_ptr<void>,
			                      std::function<void(const T&)>>> m_callerList; // current list of binded element
			std::vector<std::pair<std::weak_ptr<void>,
			                      std::function<void(const T&)>>> m_callerListInCallback; // temporaty list (when add one in call process)
			std::vector<std::function<void(const T&)>> m_callerListDirect; // current list of binded element
			std::vector<std::function<void(const T&)>> m_callerListDirectInCallback; // temporaty list (when add one in call process)
		public:
			/**
			 * @brief Create a signal with a specific type.
			 * @param[in] _signalInterfaceLink reference on the signal lister.
			 * @param[in] _name Static name of the signal.
			 * @param[in] _description Description of the signal.
			 * @param[in] _periodic Customisation of the log display tag at true to down debug lebel at verbose.
			 */
			Signal(ewol::signal::Interface& _signalInterfaceLink,
			      const std::string& _name,
			      const std::string& _description = "",
			      bool _periodic = false) :
			  signal::Base(_signalInterfaceLink, _name, _description, _periodic) {
				
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
			template<class TYPE_CLASS, class TYPE, typename... TArgs>
			void bind(std::shared_ptr<TYPE_CLASS> _obj, void (TYPE::*_func)(const T&, TArgs...), TArgs... _args2) {
				std::shared_ptr<TYPE> obj2 = std::dynamic_pointer_cast<TYPE>(_obj);
				if (obj2 == nullptr) {
					EWOL_ERROR("Can not bind signal ...");
					return;
				}
				if (m_callInProgress == 0) {
					m_callerList.push_back(std::make_pair(std::weak_ptr<void>(_obj), std::bind(_func, obj2.get(), std::placeholders::_1, std::forward<TArgs>(_args2)...)));
				} else {
					m_callerListInCallback.push_back(std::make_pair(std::weak_ptr<void>(_obj), std::bind(_func, obj2.get(), std::placeholders::_1, std::forward<TArgs>(_args2)...)));
				}
			}
			/**
			 * @brief Advanced binding a callback function to the current signal.
			 * @param[in] _obj Shared pointer on the caller object
			 * @param[in] _func functor to call (do it yourself)
			 * @example signalXXXX.connect(shared_from_this(), std::bind(&ClassName::onCallbackXXX, this, std::placeholders::_1));
			 */
			void connect(std::shared_ptr<void> _obj, std::function<void(const T&)> _function ) {
				if (m_callInProgress == 0) {
					m_callerList.push_back(std::make_pair(std::weak_ptr<void>(_obj), _function));
				} else {
					m_callerListInCallback.push_back(std::make_pair(std::weak_ptr<void>(_obj), _function));
				}
			}
			//! @previous
			void connect(std::function<void(const T&)> _function ) {
				if (m_callInProgress == 0) {
					m_callerListDirect.push_back(_function);
				} else {
					m_callerListDirectInCallback.push_back(_function);
				}
			}
			/**
			 * @brief Check if an object is registered in the Signal
			 * @param[in] _obj shared pointer on the object
			 * @return true The object is connected at this signal.
			 * @return false The object is NOT connected on this signal.
			 */
			bool isRegistered(std::shared_ptr<void> _obj) {
				if (_obj == nullptr) {
					return false;
				}
				for (auto &it : m_callerList) {
					std::shared_ptr<void> obj = it.first.lock();
					if (obj == _obj) {
						return true;
					}
				}
				for (auto &it : m_callerListInCallback) {
					std::shared_ptr<void> obj = it.first.lock();
					if (obj == _obj) {
						return true;
					}
				}
				return false;
			}
			/**
			 * @brief remove link on the signal.
			 * @param[in] _obj shared pointer on the removing object
			 */
			void release(std::shared_ptr<void> _obj) {
				if (m_callInProgress == 0) {
					// Remove from the list :
					auto it(m_callerList.begin());
					while(it != m_callerList.end()) {
						if (it->first.lock() == _obj) {
							it = m_callerList.erase(it);
						} else {
							++it;
						}
					}
				} else {
					// just remove weak poointer
					auto it(m_callerList.begin());
					while(it != m_callerList.end()) {
						if (it->first.lock() == _obj) {
							it->first.reset();
						} else {
							++it;
						}
					}
					m_someOneRemoveInCall = true;
				}
				// remove from add list in callback progress
				auto it = m_callerListInCallback.begin();
				while(it != m_callerListInCallback.end()) {
					if (it->first.lock() == _obj) {
						it = m_callerListInCallback.erase(it);
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
					m_signalCallLevel++;
					int32_t tmpID = m_uidSignal++;
				#endif
				m_callInProgress++;
				if (m_periodic == true) {
					EWOL_VERBOSE(ewol::signal::logIndent(m_signalCallLevel-1) << "emit signal{" << tmpID << "} : signal='" << m_name << "' data='" << etk::to_string(_data) << "' to: " << m_callerList.size() << " element(s)");
				} else {
					EWOL_DEBUG(ewol::signal::logIndent(m_signalCallLevel-1) << "emit signal{" << tmpID << "} : signal='" << m_name << "' data='" << etk::to_string(_data) << "' to: " << m_callerList.size() << " element(s)");
				}
				{
					auto it(m_callerList.begin());
					while (it != m_callerList.end()) {
						std::shared_ptr<void> destObject = it->first.lock();
						if (destObject == nullptr) {
							it = m_callerList.erase(it);
							EWOL_DEBUG(ewol::signal::logIndent(m_signalCallLevel-1) << "    nullptr dest");
							continue;
						}
						if (m_periodic == true) {
							EWOL_VERBOSE(ewol::signal::logIndent(m_signalCallLevel-1) << "     signal{" << tmpID << "} :");// [" << destObject->getId() << "]" << destObject->getObjectType());
						} else {
							EWOL_DEBUG(ewol::signal::logIndent(m_signalCallLevel-1) << "     signal{" << tmpID << "} :");// [" << destObject->getId() << "]" << destObject->getObjectType());
						}
						it->second(_data);
						++it;
					}
				}
				{
					auto it(m_callerListDirect.begin());
					while (it != m_callerListDirect.end()) {
						if (m_periodic == true) {
							EWOL_VERBOSE(ewol::signal::logIndent(m_signalCallLevel-1) << "X     signal{" << tmpID << "} :");// [" << destObject->getId() << "]" << destObject->getObjectType());
						} else {
							EWOL_DEBUG(ewol::signal::logIndent(m_signalCallLevel-1) << "X     signal{" << tmpID << "} :");// [" << destObject->getId() << "]" << destObject->getObjectType());
						}
						(*it)(_data);
						++it;
					}
				}
				m_callInProgress--;
				#ifdef DEBUG
					m_signalCallLevel--;
				#endif
				// Remove element in call phase:
				if (m_someOneRemoveInCall == true) {
					m_someOneRemoveInCall = false;
					// Remove from the list :
					auto it(m_callerList.begin());
					while(it != m_callerList.end()) {
						if (it->first.expired() == true) {
							it = m_callerList.erase(it);
						} else {
							++it;
						}
					}
				}
				// add element in call phase:
				if (m_callerListInCallback.size() > 0) {
					for (auto &it : m_callerListInCallback) {
						m_callerList.push_back(it);
					}
					m_callerListInCallback.clear();
				}
				if (m_callerListDirectInCallback.size() > 0) {
					for (auto &it : m_callerListDirectInCallback) {
						m_callerListDirect.push_back(it);
					}
					m_callerListDirectInCallback.clear();
				}
			}
			size_t getNumberConnected() {
				return m_callerList.size() + m_callerListDirect.size();
			}
	};
	#undef __class__
	#define __class__ "Signal<void>"
	template<> class Signal<void> : public signal::Base {
		private:
			std::vector<std::pair<std::weak_ptr<void>, std::function<void()>>> m_callerList;
			std::vector<std::pair<std::weak_ptr<void>, std::function<void()>>> m_callerListInCallback;
			std::vector<std::function<void()>> m_callerListDirect;
			std::vector<std::function<void()>> m_callerListDirectInCallback;
		public:
			/**
			 * @brief Create a signal with a specific 'void' type.
			 * @param[in] _signalInterfaceLink reference on the signal lister.
			 * @param[in] _name Static name of the signal.
			 * @param[in] _description Description of the signal.
			 * @param[in] _periodic Customisation of the log display tag at true to down debug lebel at verbose.
			 */
			Signal(ewol::signal::Interface& _signalInterfaceLink,
			      const std::string& _name,
			      const std::string& _description = "",
			      bool _periodic = false) :
			  signal::Base(_signalInterfaceLink, _name, _description, _periodic) {
				
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
			template<class TYPE_CLASS, class TYPE, typename... TArgs>
			void bind(std::shared_ptr<TYPE_CLASS> _obj, void (TYPE::*_func)(TArgs...), TArgs... args2) {
				std::shared_ptr<TYPE> obj2 = std::dynamic_pointer_cast<TYPE>(_obj);
				if (obj2 == nullptr) {
					EWOL_ERROR("Can not bind signal ...");
					return;
				}
				if (m_callInProgress == 0) {
					m_callerList.push_back(std::make_pair(std::weak_ptr<void>(_obj), std::bind(_func, obj2.get(), std::forward<TArgs>(args2)...)));
				} else {
					m_callerListInCallback.push_back(std::make_pair(std::weak_ptr<void>(_obj), std::bind(_func, obj2.get(), std::forward<TArgs>(args2)...)));
				}
			}
			/**
			 * @brief Advanced binding a callback function to the current signal.
			 * @param[in] _obj Shared pointer on the caller object
			 * @param[in] _func functor to call (do it yourself)
			 * @example signalXXXX.connect(shared_from_this(), std::bind(&ClassName::onCallbackXXX, this, std::placeholders::_1));
			 */
			void connect(std::shared_ptr<void> _obj, std::function<void()> _function ) {
				if (m_callInProgress == 0) {
					m_callerList.push_back(std::make_pair(std::weak_ptr<void>(_obj), _function));
				} else {
					m_callerListInCallback.push_back(std::make_pair(std::weak_ptr<void>(_obj), _function));
				}
			}
			//! @previous
			void connect(std::function<void()> _function ) {
				if (m_callInProgress == 0) {
					m_callerListDirect.push_back(_function);
				} else {
					m_callerListDirectInCallback.push_back(_function);
				}
			}
			/**
			 * @brief remove link on the signal.
			 * @param[in] _obj shared pointer on the removing object
			 */
			void release(std::shared_ptr<void> _obj) {
				auto it(m_callerList.begin());
				if (m_callInProgress == 0) {
					// Remove from the list :
					while(it != m_callerList.end()) {
						if (it->first.lock() == _obj) {
							//EWOL_DEBUG(" unbind : " << _obj->getObjectType() << " signal='" << m_name << "'");
							it = m_callerList.erase(it);
						} else {
							++it;
						}
					}
				} else {
					// just remove weak poointer
					while(it != m_callerList.end()) {
						if (it->first.lock() == _obj) {
							//EWOL_DEBUG(" unbind : " << _obj->getObjectType() << " signal='" << m_name << "' (delayed)");
							it->first.reset();
						} else {
							++it;
						}
					}
					m_someOneRemoveInCall = true;
				}
				// remove from add list in callback progress
				it = m_callerListInCallback.begin();
				while(it != m_callerListInCallback.end()) {
					if (it->first.lock() == _obj) {
						//EWOL_DEBUG(" unbind : " << _obj->getObjectType() << " signal='" << m_name << "' (notActive)");
						it = m_callerListInCallback.erase(it);
					} else {
						++it;
					}
				}
			}
			void emit() {
				#ifdef DEBUG
					m_signalCallLevel++;
					int32_t tmpID = m_uidSignal++;
				#endif
				m_callInProgress++;
				if (m_periodic == true) {
					EWOL_VERBOSE(ewol::signal::logIndent(m_signalCallLevel-1) << "emit signal{" << tmpID << "} : signal='" << m_name << "' to: " << m_callerList.size() << " element(s)");
				} else {
					EWOL_DEBUG(ewol::signal::logIndent(m_signalCallLevel-1) << "emit signal{" << tmpID << "} : signal='" << m_name << "' to: " << m_callerList.size() << " element(s)");
				}
				{
					auto it(m_callerList.begin());
					while (it != m_callerList.end()) {
						std::shared_ptr<void> destObject = it->first.lock();
						if (destObject == nullptr) {
							it = m_callerList.erase(it);
							EWOL_DEBUG(ewol::signal::logIndent(m_signalCallLevel-1) << "    nullptr dest");
							continue;
						}
						if (m_periodic == true) {
							EWOL_VERBOSE(ewol::signal::logIndent(m_signalCallLevel-1) << "     signal{" << tmpID << "} :");// [" << destObject->getId() << "]" << destObject->getObjectType());
						} else {
							EWOL_DEBUG(ewol::signal::logIndent(m_signalCallLevel-1) << "     signal{" << tmpID << "} :");// [" << destObject->getId() << "]" << destObject->getObjectType());
						}
						it->second();
						++it;
					}
				}
				{
					auto it(m_callerListDirect.begin());
					while (it != m_callerListDirect.end()) {
						if (m_periodic == true) {
							EWOL_VERBOSE(ewol::signal::logIndent(m_signalCallLevel-1) << "     signal{" << tmpID << "} :");// [" << destObject->getId() << "]" << destObject->getObjectType());
						} else {
							EWOL_DEBUG(ewol::signal::logIndent(m_signalCallLevel-1) << "     signal{" << tmpID << "} :");// [" << destObject->getId() << "]" << destObject->getObjectType());
						}
						(*it)();
						++it;
					}
				}
				m_callInProgress--;
				#ifdef DEBUG
					m_signalCallLevel--;
				#endif
				// Remove element in call phase:
				if (m_someOneRemoveInCall == true) {
					m_someOneRemoveInCall = false;
					// Remove from the list :
					auto it(m_callerList.begin());
					while(it != m_callerList.end()) {
						if (it->first.expired() == true) {
							it = m_callerList.erase(it);
						} else {
							++it;
						}
					}
				}
				// add element in call phase:
				if (m_callerListInCallback.size() > 0) {
					for (auto &it : m_callerListInCallback) {
						if (it.first.expired() == false) {
							m_callerList.push_back(it);
						}
					}
					m_callerListInCallback.clear();
				}
				if (m_callerListDirectInCallback.size() > 0) {
					for (auto &it : m_callerListDirectInCallback) {
						m_callerListDirect.push_back(it);
					}
					m_callerListDirectInCallback.clear();
				}
			}
			size_t getNumberConnected() {
				return m_callerList.size() + m_callerListDirect.size();;
			}
	};
	#undef __class__
	#define __class__ nullptr
};
#endif
