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
ewol::object::SignalBase::SignalBase(ewol::object::SignalList& _objectLink,
                                   const std::string& _name,
                                   const std::string& _description) :
  m_objectLink(_objectLink),
  m_name(_name),
  m_description(_description) {
	// add a reference on the current signal ...
	m_objectLink.signalAdd(this);
}

std::ostream& ewol::object::operator <<(std::ostream& _os, const ewol::object::SignalBase& _obj) {
	_os << _obj.getName();
	return _os;
}

