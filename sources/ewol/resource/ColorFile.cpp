/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/os/FSNode.h>
#include <ewol/debug.h>
#include <ewol/resource/ColorFile.h>
#include <ewol/resource/Manager.h>
#include <ejson/ejson.h>
#include <stdexcept>

#undef __class__
#define __class__	"resource::ColorFile"


ewol::resource::ColorFile::ColorFile(const std::string& _filename) :
  ewol::Resource(_filename),
  m_errorColor(etk::color::orange) {
	addObjectType("ewol::ColorFile");
	EWOL_DEBUG("CF : load \"" << _filename << "\"");
	reload();
	EWOL_DEBUG("List of all color : " << m_list.getKeys());
}


ewol::resource::ColorFile::~ColorFile(void) {
	// remove all element
	m_list.clear();
}

void ewol::resource::ColorFile::reload(void) {
	// remove all previous set of value :
	for (int32_t iii = 0; iii < m_list.size() ; ++iii) {
		m_list[iii] = m_errorColor;
	}
	// open and read all json elements:
	ejson::Document doc;
	if (false == doc.load(m_name)) {
		EWOL_ERROR("Can not load file : '" << m_name << "' = " << etk::FSNode(m_name).getFileSystemName());
		return;
	}
	ejson::Array* baseArray = doc.getArray("color");
	if (baseArray == NULL) {
		EWOL_ERROR("Can not get basic array : 'color'");
		return;
	}
	for (int32_t iii = 0; iii < baseArray->size(); ++iii) {
		ejson::Object* tmpObj = baseArray->getObject(iii);
		if (tmpObj == NULL) {
			EWOL_DEBUG(" can not get object in 'color' id=" << iii);
			continue;
		}
		std::string name = tmpObj->getStringValue("name", "");
		std::string color = tmpObj->getStringValue("color", m_errorColor.getHexString());
		EWOL_DEBUG("find new color : '" << name << "' color='" << color << "'");
		if (name.size() == 0) {
			EWOL_ERROR("Drop an empty name");
			continue;
		}
		m_list.add(name, etk::Color<float>(color));
	}
}


int32_t ewol::resource::ColorFile::request(const std::string& _paramName) {
	// check if the parameters existed :
	if (m_list.exist(_paramName) == false) {
		m_list.add(_paramName, m_errorColor);
	}
	return m_list.getId(_paramName);
}


ewol::resource::ColorFile* ewol::resource::ColorFile::keep(const std::string& _filename) {
	EWOL_INFO("KEEP : ColorFile : file : \"" << _filename << "\"");
	ewol::resource::ColorFile* object = NULL;
	ewol::Resource* object2 = getManager().localKeep(_filename);
	if (NULL != object2) {
		object = dynamic_cast<ewol::resource::ColorFile*>(object2);
		if (NULL == object) {
			EWOL_CRITICAL("Request resource file : '" << _filename << "' With the wrong type (dynamic cast error)");
			return NULL;
		}
	}
	if (NULL != object) {
		return object;
	}
	// this element create a new one every time ....
	object = new ewol::resource::ColorFile(_filename);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : " << _filename);
		return NULL;
	}
	getManager().localAdd(object);
	return object;
}

void ewol::resource::ColorFile::release(ewol::resource::ColorFile*& _object) {
	if (NULL == _object) {
		return;
	}
	ewol::Resource* object2 = static_cast<ewol::Resource*>(_object);
	getManager().release(object2);
	_object = NULL;
}



