/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/parameter/Interface.h>
#include <ewol/parameter/Parameter.h>


ewol::parameter::Parameter::Parameter(ewol::parameter::Interface& _paramInterfaceLink, const std::string& _name) :
  m_interfaceLink(_paramInterfaceLink),
  m_name(_name) {
	// add a reference on the current parameter ...
	m_interfaceLink.parameterAdd(this);
}

void ewol::parameter::Parameter::notifyChange() const {
	m_interfaceLink.onParameterChangeValue(ewol::parameter::Ref(this));
}

bool ewol::parameter::operator==(const Ref& _obj, const Parameter& _obj2) noexcept {
	return &_obj2 == _obj.m_ref;
}
bool ewol::parameter::operator==(const Parameter& _obj2, const Ref& _obj) noexcept {
	return &_obj2 == _obj.m_ref;
}