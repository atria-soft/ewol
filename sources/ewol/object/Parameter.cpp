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


ewol::object::Parameter::Parameter(ewol::object::ParameterList& _objectLink, const std::string& _name) :
  m_objectLink(_objectLink),
  m_name(_name) {
	// add a reference on the current parameter ...
	_objectLink.parameterAdd(this);
}

void ewol::object::Parameter::notifyChange() const {
	m_objectLink.onParameterChangeValue(ewol::object::ParameterRef(this));
}