/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <memory>
#include <ewol/debug.h>
#include <ewol/signal/Interface.h>
#include <ewol/signal/Base.h>

#ifdef DEBUG
	int32_t ewol::signal::Base::m_uidSignal = 0;
	int32_t ewol::signal::Base::m_signalCallLevel = 0;
#endif

ewol::signal::Base::Base(ewol::signal::Interface& _signalInterfaceLink,
                         const std::string& _name,
                         const std::string& _description,
                         bool _periodic) :
  m_signalInterfaceLink(_signalInterfaceLink),
  m_name(_name),
  m_description(_description),
  m_callInProgress(0),
  m_someOneRemoveInCall(false),
  m_periodic(_periodic) {
	// add a reference on the current signal ...
	m_signalInterfaceLink.signalAdd(this);
}

std::ostream& ewol::signal::operator <<(std::ostream& _os, const ewol::signal::Base& _obj) {
	_os << _obj.getName();
	return _os;
}


const char* ewol::signal::logIndent(int32_t _iii) {
	static const char g_val[] = "                    ";
	if (_iii > 5) {
		return g_val;
	}
	return g_val + (5-_iii)*4;
}