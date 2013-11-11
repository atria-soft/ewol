/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/os/FSNode.h>
#include <ewol/debug.h>
#include <ewol/resources/ConfigFile.h>
#include <ewol/resources/ResourceManager.h>

#undef __class__
#define __class__	"ConfigFile"


void ewol::SimpleConfigElement::parse(const etk::UString& value) {
	etk::Char tmp = value.c_str();
	m_valueInt = 0;
	m_valuefloat = 0;
	sscanf(tmp, "%d", &m_valueInt);
	sscanf(tmp, "%f", &m_valuefloat);
	m_value = value;
}



ewol::ConfigFile::ConfigFile(const etk::UString& _filename) :
  ewol::Resource(_filename) {
	EWOL_DEBUG("SFP : load \"" << _filename << "\"");
	reload();
}


ewol::ConfigFile::~ConfigFile(void) {
	// remove all element
	for (int32_t iii=0; iii<m_list.size(); iii++){
		if (NULL != m_list[iii]) {
			delete(m_list[iii]);
			m_list[iii] = NULL;
		}
	}
	m_list.clear();
}

void ewol::ConfigFile::reload(void) {
	// reset all parameters
	for (int32_t iii=0; iii<m_list.size(); iii++){
		if (NULL != m_list[iii]) {
			m_list[iii]->parse("");
		}
	}
	// acess of the file :
	etk::FSNode file(m_name);
	
	if (false == file.exist()) {
		EWOL_ERROR("File does not Exist : \"" << file << "\"");
		return;
	}
	etk::UString fileExtention = file.fileGetExtention();
	if (fileExtention != "conf") {
		EWOL_ERROR("File does not have extention \".conf\" for program but : \"" << fileExtention << "\"");
		return;
	}
	if (false == file.fileOpenRead()) {
		EWOL_ERROR("Can not open the file : " << file);
		return;
	}
	#define MAX_LINE_SIZE   (2048)
	char tmpData[MAX_LINE_SIZE];
	while (file.fileGets(tmpData, MAX_LINE_SIZE) != NULL) {
		int32_t len = strlen(tmpData);
		if(    tmpData[len-1] == '\n'
			|| tmpData[len-1] == '\r') {
			tmpData[len-1] = '\0';
			len--;
		}
		//EWOL_DEBUG(" Read data : \"" << tmpData << "\"");
		if (len == 0) {
			continue;
		}
		etk::UString tmpData2(tmpData);
		etk::UString tmppp("#");
		if (true == tmpData2.startWith(tmppp)) {
			// comment ...
			continue;
		}
		tmppp="//";
		if (true == tmpData2.startWith(tmppp)) {
			// comment ...
			continue;
		}
		// get parameters :
		int32_t pos = tmpData2.findForward('=');
		if (pos == -1){
			//the element "=" is not find ...
			continue;
		}
		etk::UString paramName = tmpData2.extract(0, pos);
		etk::UString paramValue = tmpData2.extract(pos+1, 0x7FFFF);
		EWOL_DEBUG("        param name=\"" << paramName << "\" val=\"" << paramValue << "\"");
		// check if the parameters existed :
		bool findParam = false;
		for (int32_t iii=0; iii<m_list.size(); iii++){
			if (NULL != m_list[iii]) {
				if (m_list[iii]->m_paramName == paramName) {
					m_list[iii]->parse(paramValue);
					findParam = true;
					// stop parsing ...
					break;
				}
			}
		}
		if (false == findParam) {
			ewol::SimpleConfigElement* tmpElement = new ewol::SimpleConfigElement(paramName);
			if (NULL == tmpElement) {
				EWOL_DEBUG("error while allocation");
			} else {
				tmpElement->parse(paramValue);
				m_list.push_back(tmpElement);
			}
		}
	}
	// close the file:
	file.fileClose();
}


int32_t ewol::ConfigFile::request(const etk::UString& _paramName) {
	// check if the parameters existed :
	for (int32_t iii=0; iii<m_list.size(); iii++){
		if (NULL != m_list[iii]) {
			if (m_list[iii]->m_paramName == _paramName) {
				return iii;
			}
		}
	}
	ewol::SimpleConfigElement* tmpElement = new ewol::SimpleConfigElement(_paramName);
	if (NULL == tmpElement) {
		EWOL_DEBUG("error while allocation");
	} else {
		m_list.push_back(tmpElement);
	}
	return m_list.size()-1;
}


ewol::ConfigFile* ewol::ConfigFile::keep(const etk::UString& _filename) {
	EWOL_INFO("KEEP : SimpleConfig : file : \"" << _filename << "\"");
	ewol::ConfigFile* object = static_cast<ewol::ConfigFile*>(getManager().localKeep(_filename));
	if (NULL != object) {
		return object;
	}
	// this element create a new one every time ....
	object = new ewol::ConfigFile(_filename);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : ??Mesh.obj??");
		return NULL;
	}
	getManager().localAdd(object);
	return object;
}

void ewol::ConfigFile::release(ewol::ConfigFile*& _object) {
	if (NULL == _object) {
		return;
	}
	ewol::Resource* object2 = static_cast<ewol::Resource*>(_object);
	getManager().release(object2);
	_object = NULL;
}



