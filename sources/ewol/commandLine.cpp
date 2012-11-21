/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/commandLine.h>
#include <etk/Vector.h>

// ------------------------------------------------------------------------
//                 Command line arguments
// ------------------------------------------------------------------------

static etk::Vector<etk::UString> listArgs;


void ewol::commandLine::Clean(void)
{
	listArgs.Clear();
}

int32_t ewol::commandLine::Size(void)
{
	return listArgs.Size();
}

etk::UString ewol::commandLine::Get(int32_t id)
{
	if (id<0 && id>=listArgs.Size()) {
		return "";
	}
	return listArgs[id];
}

void ewol::commandLine::Add(etk::UString& newElement)
{
	listArgs.PushBack(newElement);
}


