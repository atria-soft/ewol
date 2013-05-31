/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/commandLine.h>
#include <etk/Vector.h>

// ------------------------------------------------------------------------
//                 Command line arguments
// ------------------------------------------------------------------------

static etk::Vector<etk::UString> listArgs;


void ewol::commandLine::Clean(void)
{
	EWOL_DEBUG("Clean commandLine (START)");
	listArgs.Clear();
	EWOL_DEBUG("Clean commandLine (END)");
}

int32_t ewol::commandLine::Size(void)
{
	return listArgs.Size();
}

etk::UString ewol::commandLine::Get(int32_t _id)
{
	if (_id<0 && _id>=listArgs.Size()) {
		return "";
	}
	return listArgs[_id];
}

void ewol::commandLine::Add(const etk::UString& _newElement)
{
	listArgs.PushBack(_newElement);
}


