/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/os/FSNode.h>
#include <ewol/debug.h>
#include <ewol/renderer/resources/ConfigFile.h>


void ewol::SimpleConfigElement::Parse(etk::UString value)
{
	char* tmp = value.c_str();
	m_valueInt = 0;
	m_valuefloat = 0;
	sscanf(tmp, "%d", &m_valueInt);
	sscanf(tmp, "%f", &m_valuefloat);
	m_value = tmp;
}



ewol::ConfigFile::ConfigFile(const etk::UString& filename): 
	ewol::Resource(filename)
{
	EWOL_DEBUG("SFP : load \"" << filename << "\"");
	Reload();
}


ewol::ConfigFile::~ConfigFile(void)
{
	// remove all element
	for (int32_t iii=0; iii<m_list.Size(); iii++){
		if (NULL != m_list[iii]) {
			delete(m_list[iii]);
			m_list[iii] = NULL;
		}
	}
	m_list.Clear();
}


void ewol::ConfigFile::Reload(void)
{
	// Reset all parameters
	for (int32_t iii=0; iii<m_list.Size(); iii++){
		if (NULL != m_list[iii]) {
			m_list[iii]->Parse("");
		}
	}
	// acess of the file :
	etk::FSNode file(m_name);
	
	if (false == file.Exist()) {
		EWOL_ERROR("File does not Exist : \"" << file << "\"");
		return;
	}
	etk::UString fileExtention = file.FileGetExtention();
	if (fileExtention != "conf") {
		EWOL_ERROR("File does not have extention \".prog\" for program but : \"" << fileExtention << "\"");
		return;
	}
	if (false == file.FileOpenRead()) {
		EWOL_ERROR("Can not open the file : " << file);
		return;
	}
	#define MAX_LINE_SIZE   (2048)
	char tmpData[MAX_LINE_SIZE];
	while (file.FileGets(tmpData, MAX_LINE_SIZE) != NULL) {
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
		if (true == tmpData2.StartWith(tmppp)) {
			// comment ...
			continue;
		}
		tmppp="//";
		if (true == tmpData2.StartWith(tmppp)) {
			// comment ...
			continue;
		}
		// get parameters :
		int32_t pos = tmpData2.FindForward('=');
		if (pos == -1){
			//the element "=" is not find ...
			continue;
		}
		etk::UString paramName = tmpData2.Extract(0, pos);
		etk::UString paramValue = tmpData2.Extract(pos+1, 0x7FFFF);
		EWOL_DEBUG("        param name=\"" << paramName << "\" val=\"" << paramValue << "\"");
		// check if the parameters existed :
		bool findParam = false;
		for (int32_t iii=0; iii<m_list.Size(); iii++){
			if (NULL != m_list[iii]) {
				if (m_list[iii]->m_paramName == paramName) {
					m_list[iii]->Parse(paramValue);
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
				tmpElement->Parse(paramValue);
				m_list.PushBack(tmpElement);
			}
		}
	}
	// close the file:
	file.FileClose();
	
}


int32_t ewol::ConfigFile::Request(etk::UString paramName)
{
	// check if the parameters existed :
	for (int32_t iii=0; iii<m_list.Size(); iii++){
		if (NULL != m_list[iii]) {
			if (m_list[iii]->m_paramName == paramName) {
				return iii;
			}
		}
	}
	ewol::SimpleConfigElement* tmpElement = new ewol::SimpleConfigElement(paramName);
	if (NULL == tmpElement) {
		EWOL_DEBUG("error while allocation");
	} else {
		m_list.PushBack(tmpElement);
	}
	return m_list.Size()-1;
}

