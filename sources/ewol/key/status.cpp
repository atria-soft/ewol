/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/key/status.h>

static const char* statusDescriptionString[ewol::key::statusCount+1] = {
	"statusUnknow",
	"statusDown",
	"statusMove",
	"statusSingle",
	"statusDouble",
	"statusTriple",
	"statusQuad",
	"statusQuinte",
	"statusUp",
	"statusUpAfter",
	"statusEnter",
	"statusLeave",
	"statusAbort",
	"statusTransfert",
	"statusCount"
};

std::ostream& ewol::key::operator <<(std::ostream& _os, const enum ewol::key::status _obj) {
	if (_obj >= 0 && _obj <ewol::key::statusCount) {
		_os << statusDescriptionString[_obj];
	} else {
		_os << "[ERROR]";
	}
	return _os;
}


