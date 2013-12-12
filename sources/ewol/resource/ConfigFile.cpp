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
#include <stdexcept>

#undef __class__
#define __class__	"resource::ConfigFile"


void ewol::resource::SimpleConfigElement::parse(const std::string& _value) {
	m_value = _value;
	#ifdef __EXCEPTIONS
		try {
			m_valueInt = std::stoi(_value);
			m_valuefloat = std::stof(_value);
		}
		catch (const std::invalid_argument& ia) {
			EWOL_VERBOSE(" invalid argument= " << ia.what() << "val='" << _value << "'");
			m_valueInt = 0;
			m_valuefloat = 0;
		}
	#else
		m_valueInt = std::stoi(_value);
		m_valuefloat = std::stof(_value);
	#endif
}



ewol::resource::ConfigFile::ConfigFile(const std::string& _filename) :
  ewol::resource::Resource(_filename) {
	addObjectType("ewol::ConfigFile");
	EWOL_DEBUG("SFP : load \"" << _filename << "\"");
	reload();
}


ewol::resource::ConfigFile::~ConfigFile(void) {
	// remove all element
	for (size_t iii=0; iii<m_list.size(); iii++){
		if (NULL != m_list[iii]) {
			delete(m_list[iii]);
			m_list[iii] = NULL;
		}
	}
	m_list.clear();
}

void ewol::resource::ConfigFile::reload(void) {
	// reset all parameters
	for (size_t iii=0; iii<m_list.size(); iii++){
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
	std::string fileExtention = file.fileGetExtention();
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
		std::string tmpData2(tmpData);
		std::string tmppp("#");
		if (start_with(tmpData2, tmppp) == true) {
			// comment ...
			continue;
		}
		tmppp="//";
		if (start_with(tmpData2, tmppp) == true) {
			// comment ...
			continue;
		}
		// get parameters :
		size_t pos = tmpData2.find('=');
		if (pos == 0){
			//the element "=" is not find ...
			continue;
		}
		std::string paramName = std::string(tmpData2, 0, pos);
		std::string paramValue = std::string(tmpData2, pos+1, 0x7FFFF);
		EWOL_DEBUG("        param name=\"" << paramName << "\" val=\"" << paramValue << "\"");
		// check if the parameters existed :
		bool findParam = false;
		for (size_t iii=0; iii<m_list.size(); iii++){
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


int32_t ewol::resource::ConfigFile::request(const std::string& _paramName) {
	// check if the parameters existed :
	for (size_t iii=0; iii<m_list.size(); iii++){
		if (NULL != m_list[iii]) {
			if (m_list[iii]->m_paramName == _paramName) {
				return iii;
			}
		}
	}
	ewol::resource::SimpleConfigElement* tmpElement = new ewol::resource::SimpleConfigElement(_paramName);
	if (NULL == tmpElement) {
		EWOL_DEBUG("error while allocation");
	} else {
		m_list.push_back(tmpElement);
	}
	return m_list.size()-1;
}


ewol::resource::ConfigFile* ewol::resource::ConfigFile::keep(const std::string& _filename) {
	EWOL_INFO("KEEP : SimpleConfig : file : \"" << _filename << "\"");
	ewol::resource::ConfigFile* object = static_cast<ewol::resource::ConfigFile*>(getManager().localKeep(_filename));
	if (NULL != object) {
		return object;
	}
	// this element create a new one every time ....
	object = new ewol::resource::ConfigFile(_filename);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : ??Mesh.obj??");
		return NULL;
	}
	getManager().localAdd(object);
	return object;
}

void ewol::resource::ConfigFile::release(ewol::resource::ConfigFile*& _object) {
	if (NULL == _object) {
		return;
	}
	ewol::resource::Resource* object2 = static_cast<ewol::resource::Resource*>(_object);
	getManager().release(object2);
	_object = NULL;
}



