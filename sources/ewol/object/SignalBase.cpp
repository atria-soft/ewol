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

int32_t ewol::object::SignalBase::m_uidSignal = 0;
int32_t ewol::object::SignalBase::m_signalCallLeval = 0;
ewol::object::SignalBase::SignalBase(ewol::object::SignalList& _objectLink,
                                   const std::string& _name,
                                   const std::string& _description) :
  m_objectLink(_objectLink),
  m_name(_name),
  m_description(_description),
  m_callInProgress(0),
  m_someOneRemoveInCall(false) {
	// add a reference on the current signal ...
	m_objectLink.signalAdd(this);
}

std::ostream& ewol::object::operator <<(std::ostream& _os, const ewol::object::SignalBase& _obj) {
	_os << _obj.getName();
	return _os;
}


const char* ewol::object::logIndent(int32_t _iii) {
	static const char g_val[] = "                    ";
	if (_iii > 5) {
		return g_val;
	}
	return g_val + (5-_iii)*4;
}