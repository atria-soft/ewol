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
				//std::vector<std::funtion<void(const T&)>> m_callerList;
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
				void connect(std::shared_ptr<ewol::Object> _obj, const char* _destId=nullptr, const std::string& _data="" ) {
					m_serializedCallerList.push_back(SignalCallerIdentifier(_obj, _destId, _data));
				}
				void release(std::shared_ptr<ewol::Object> _obj) {
					for (auto it(m_serializedCallerList.begin()) ; it != m_serializedCallerList.end(); ++it) {
						if (it->m_object.lock() == _obj) {
							m_serializedCallerList.erase(it);
							it = m_serializedCallerList.begin();
						}
					}
				}
				/*
				bool emitString(const std::shared_ptr<ewol::Object>& _source, const std::string& _data) {
					// note : this can not emit on function ....
					for (auto &it : m_serializedCallerList) {
						std::shared_ptr<ewol::Object> destObject = std::get<0>(it).lock();
						if (destObject == nullptr) {
							// TODO : Remove instance ...
							EWOL_VERBOSE("    nullptr dest");
							continue;
						}
						const char* eventId = m_name.c_str();
						if (std::get<1>(it) != nullptr) {
							eventId = std::get<1>(it);
						}
						if (std::get<2>(it).size() <= 0){
							ewol::object::Message tmpMsg(_source, eventId, _data);
							EWOL_VERBOSE("send message " << tmpMsg);
							destObject->onReceiveMessage(tmpMsg);
						} else {
							// set the user requested data ...
							ewol::object::Message tmpMsg(_source, eventId, std::get<2>(it));
							EWOL_VERBOSE("send message " << tmpMsg);
							destObject->onReceiveMessage(tmpMsg);
						}
					}
				}
				*/
				void emit(const std::shared_ptr<ewol::Object>& _source, const T& _data) {
					// note : this can not emit on function ....
					std::string stringData;
					if (m_serializedCallerList.size()>0 ) {
						stringData = etk::to_string(_data);
					}
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
							ewol::object::Message tmpMsg(_source, eventId, stringData);
							EWOL_VERBOSE("send message " << tmpMsg);
							destObject->onReceiveMessage(tmpMsg);
						} else {
							// set the user requested data ...
							ewol::object::Message tmpMsg(_source, eventId, it.m_data);
							EWOL_VERBOSE("send message " << tmpMsg);
							destObject->onReceiveMessage(tmpMsg);
						}
					}
				}
		};
		
		template<> class Signal<void> : public SignalBase {
			private:
				//std::vector<std::funtion<void(const T&)>> m_callerList;
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
				
				void connect(std::shared_ptr<ewol::Object> _obj, const char* _destId=nullptr, const std::string& _data="" ) {
					m_serializedCallerList.push_back(SignalCallerIdentifier(_obj, _destId, _data));
				}
				void release(std::shared_ptr<ewol::Object> _obj) {
					for (auto it(m_serializedCallerList.begin()) ; it != m_serializedCallerList.end(); ++it) {
						if (it->m_object.lock() == _obj) {
							m_serializedCallerList.erase(it);
							it = m_serializedCallerList.begin();
						}
					}
				}
				void emit(const std::shared_ptr<ewol::Object>& _source) {
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
							ewol::object::Message tmpMsg(_source, eventId, stringData);
							EWOL_VERBOSE("send message " << tmpMsg);
							destObject->onReceiveMessage(tmpMsg);
						} else {
							// set the user requested data ...
							ewol::object::Message tmpMsg(_source, eventId, it.m_data);
							EWOL_VERBOSE("send message " << tmpMsg);
							destObject->onReceiveMessage(tmpMsg);
						}
					}
				}
		};
		
	};
};
#endif
