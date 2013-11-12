/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/commandLine.h>
#include <vector>

void ewol::CommandLine::parse(int32_t _argc, const char* _argv[]) {
	for( int32_t i=1 ; i<_argc; i++) {
		EWOL_INFO("commandLine : \"" << _argv[i] << "\"" );
		m_listArgs.push_back(_argv[i]);
	}
}

esize_t ewol::CommandLine::size(void) {
	return m_listArgs.size();
}

const std::string& ewol::CommandLine::get(int32_t _id) {
	static const std::string errorArg("");
	if (_id<0 && _id >= m_listArgs.size()) {
		return errorArg;
	}
	return m_listArgs[_id];
}

void ewol::CommandLine::add(const std::string& _newElement) {
	m_listArgs.push_back(_newElement);
}

void ewol::CommandLine::remove(esize_t _id) {
	m_listArgs.erase(m_listArgs.begin()+_id);
}

