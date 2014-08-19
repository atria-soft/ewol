/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/object/ParameterList.h>
#include <ewol/object/Parameter.h>

ewol::object::ParameterList::ParameterList() {
	
}

ewol::object::ParameterList::~ParameterList() {
	parameterClean();
}

// note this pointer is not allocated and not free at the end of the class
void ewol::object::ParameterList::parameterAdd(Parameter* _pointerOnParameter) {
	if (_pointerOnParameter == nullptr) {
		EWOL_ERROR("Try to link a nullptr parameters");
		return;
	}
	m_list.push_back(_pointerOnParameter);
}

void ewol::object::ParameterList::parameterClean() {
	// remove all pointer on these parameters
	m_list.clear();
}

// Note no lock is needed at this level, because the lock is done is the upper elements ...
// the parameter set might be done with a pool of parameter, allone, the overhed is bigger ...
bool ewol::object::ParameterList::parameterSet(const std::string& _parameter, const std::string& _value) {
	for (auto &it : m_list) {
		if(    it != nullptr
		    && it->getName() == _parameter) {
			it->setString(_value);
			return true;
		}
	}
	// can not find the parameters :
	return false;
}

std::string ewol::object::ParameterList::parameterGet(const std::string& _parameter) const {
	for (auto &it : m_list) {
		if(    it != nullptr
		    && it->getName() == _parameter) {
			return it->getString();
		}
	}
	return "???";
}

void ewol::object::ParameterList::parameterDisplay(bool _changeOnly) const {
	EWOL_INFO("    Object parameters:");
	for (auto &it : m_list) {
		if(it != nullptr) {
			std::string paramName = it->getName();
			std::string paramVal = it->getString();
			std::string paramInfo = it->getInfo();
			if (    _changeOnly == false
			     || it->isDefault() == false) {
				EWOL_INFO("    |       param='" << paramName << "' value=" << paramVal << "     (" << paramInfo << ")");
			}
		} else {
			EWOL_INFO("    |       param=nullptr");
		}
	}
}

std::map<std::string, std::string> ewol::object::ParameterList::parameterGetAll(bool _notIfDefault) const {
	std::map<std::string, std::string> out;
	for (auto &it : m_list) {
		if(it != nullptr) {
			std::string paramName = it->getName();
			std::string paramVal = it->getString();
			if (    _notIfDefault == false
			     || it->isDefault() == false) {
				out.insert(std::make_pair(paramName, paramVal));
			}
		}
	}
	return out;
}
