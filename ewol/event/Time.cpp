/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/Widget.hpp>

std::ostream& ewol::event::operator <<(std::ostream& _os, const ewol::event::Time& _obj) {
	_os << "{time=" << _obj.getTime();
	_os << " uptime=" << _obj.getApplUpTime();
	_os << " delta=" << _obj.getDelta();
	_os << " deltaCall=" << _obj.getDeltaCall();
	_os << "}";
	return _os;
}

namespace etk {
	template<> std::string to_string<ewol::event::Time>(ewol::event::Time const& _obj) {
		std::string out;
		out =  "{[ewol::event::Time]time=" + etk::to_string(_obj.getTime());
		out += ";uptime=" + etk::to_string(_obj.getApplUpTime());
		out += ";delta=" + etk::to_string(_obj.getDelta());
		out += ";deltaCall=" + etk::to_string(_obj.getDeltaCall());
		out += "}";
		return out;
	}
}

// declare for signal event
#include <esignal/details/Signal.hxx>
ESIGNAL_DECLARE_SIGNAL(ewol::event::Time);

