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

void ewol::CommandLine::Parse(int32_t _argc, const char* _argv[])
{
	for( int32_t i=1 ; i<_argc; i++) {
		EWOL_INFO("commandLine : \"" << _argv[i] << "\"" );
		m_listArgs.PushBack(_argv[i]);
	}
}



esize_t ewol::CommandLine::Size(void)
{
	return m_listArgs.Size();
}

const etk::UString& ewol::CommandLine::Get(int32_t _id)
{
	static const etk::UString errorArg("");
	if (_id<0 && _id>=m_listArgs.Size()) {
		return errorArg;
	}
	return m_listArgs[_id];
}

void ewol::CommandLine::Add(const etk::UString& _newElement)
{
	m_listArgs.PushBack(_newElement);
}

void ewol::CommandLine::Remove(esize_t _id)
{
	m_listArgs.Remove(_id);
}

