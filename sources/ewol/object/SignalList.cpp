/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <memory>
#include <ewol/debug.h>
#include <ewol/object/Object.h>
#include <ewol/object/SignalList.h>
#include <ewol/object/SignalBase.h>
#include <ewol/context/Context.h>

ewol::object::SignalList::SignalList() {
	
}

ewol::object::SignalList::~SignalList() {
	m_list.clear();
}

// note this pointer is not allocated and not free at the end of the class
void ewol::object::SignalList::signalAdd(SignalBase* _pointerOnSignal) {
	if (_pointerOnSignal == nullptr) {
		EWOL_ERROR("Try to link a nullptr parameters");
		return;
	}
	m_list.push_back(_pointerOnSignal);
}

std::vector<std::string> ewol::object::SignalList::signalGetAll() const {
	std::vector<std::string> out;
	for (auto &it : m_list) {
		if(it != nullptr) {
			out.push_back(it->getName());
		}
	}
	return out;
}

void ewol::object::SignalList::unBindAll(const std::shared_ptr<ewol::Object>& _object) {
	if (_object == nullptr) {
		EWOL_ERROR("Input ERROR nullptr pointer Object ...");
		return;
	}
	for(auto &it : m_list) {
		if (it == nullptr) {
			continue;
		}
		it->release(_object);
	}
}
