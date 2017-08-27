/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/Widget.hpp>

etk::Stream& ewol::event::operator <<(etk::Stream& _os, const ewol::event::Time& _obj) {
	_os << "{time=" << _obj.getTime();
	_os << " uptime=" << _obj.getApplUpTime();
	_os << " delta=" << _obj.getDelta();
	_os << " deltaCall=" << _obj.getDeltaCall();
	_os << "}";
	return _os;
}

namespace etk {
	template<> etk::String toString<ewol::event::Time>(ewol::event::Time const& _obj) {
		etk::String out;
		out =  "{[ewol::event::Time]time=" + etk::toString(_obj.getTime());
		out += ";uptime=" + etk::toString(_obj.getApplUpTime());
		out += ";delta=" + etk::toString(_obj.getDelta());
		out += ";deltaCall=" + etk::toString(_obj.getDeltaCall());
		out += "}";
		return out;
	}
}

// declare for signal event
#include <esignal/details/Signal.hxx>
ESIGNAL_DECLARE_SIGNAL(ewol::event::Time);

