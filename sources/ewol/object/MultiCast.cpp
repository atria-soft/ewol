/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/object/MultiCast.h>
#include <ewol/context/Context.h>

#undef __class__
#define __class__ "object::MultiCast"

ewol::object::MultiCast::MultiCast() {
	EWOL_INFO("EObject message Multi-Cast");
}


ewol::object::MultiCast::~MultiCast() {
	EWOL_INFO("EObject message Multi-Cast");
	m_messageList.clear();
}

void ewol::object::MultiCast::clear() {
	EWOL_INFO("EObject message Multi-Cast");
	m_messageList.clear();
}

void ewol::object::MultiCast::add(const std::shared_ptr<ewol::Object>& _object, const char* const _message) {
	if (_object == nullptr) {
		EWOL_ERROR("Add with nullptr object");
		return;
	}
	if (_message == nullptr) {
		EWOL_ERROR("Add with nullptr Message");
		return;
	}
	m_messageList.push_back(MessageList(_message, _object));
	EWOL_DEBUG("SendMulticast ADD listener :" << _object->getId() << " on \"" << _message << "\"" );
}


void ewol::object::MultiCast::rm(const std::shared_ptr<ewol::Object>& _object) {
	if (_object == nullptr) {
		EWOL_ERROR("Rm with nullptr object");
		return;
	}
	// send the message at all registered widget ...
	auto it(m_messageList.begin());
	while (it != m_messageList.end()) {
		std::shared_ptr<ewol::Object> obj = it->m_object.lock();
		if(obj == _object) {
			EWOL_DEBUG("SendMulticast RM listener :" << _object->getId());
			m_messageList.erase(it);
			it = m_messageList.begin();
		} else {
			++it;
		}
	}
}

void ewol::object::MultiCast::send(const std::shared_ptr<ewol::Object>& _object, const char* const _message, const std::string& _data) {
	EWOL_VERBOSE("SendMulticast message \"" << _message << "\" data=\"" << _data << "\" to:");
	// send the message at all registered widget ...
	for (auto &it : m_messageList) {
		std::shared_ptr<ewol::Object> obj = it.m_object.lock();
		if(    it.m_message == _message
		    && obj != _object) {
			if (obj != nullptr) {
				EWOL_VERBOSE("        id = " << obj->getId() << " type=" << obj->getObjectType());
				// generate event ... (create message before ...
				ewol::object::Message tmpMsg(_object, it.m_message, _data);
				obj->onReceiveMessage(tmpMsg);
			}
		}
	}
}

