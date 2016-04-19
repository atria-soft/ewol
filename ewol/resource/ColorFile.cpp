/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <etk/os/FSNode.h>
#include <ewol/debug.h>
#include <ewol/resource/ColorFile.h>
#include <ejson/ejson.h>
#include <stdexcept>

ewol::resource::ColorFile::ColorFile() :
  gale::Resource(),
  m_errorColor(etk::color::orange) {
	addResourceType("ewol::ColorFile");
}

void ewol::resource::ColorFile::init(const std::string& _filename) {
	std::unique_lock<std::recursive_mutex> lock(m_mutex);
	gale::Resource::init(_filename);
	EWOL_DEBUG("CF : load \"" << _filename << "\"");
	reload();
	EWOL_DEBUG("List of all color : " << m_list.getKeys());
}

ewol::resource::ColorFile::~ColorFile() {
	// remove all element
	m_list.clear();
}


void ewol::resource::ColorFile::reload() {
	std::unique_lock<std::recursive_mutex> lock(m_mutex);
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
	std::shared_ptr<ejson::Array> baseArray = doc.getArray("color");
	if (baseArray == nullptr) {
		EWOL_ERROR("Can not get basic array : 'color'");
		return;
	}
	for (size_t iii = 0; iii < baseArray->size(); ++iii) {
		std::shared_ptr<ejson::Object> tmpObj = baseArray->getObject(iii);
		if (tmpObj == nullptr) {
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
	std::unique_lock<std::recursive_mutex> lock(m_mutex);
	// check if the parameters existed :
	if (m_list.exist(_paramName) == false) {
		m_list.add(_paramName, m_errorColor);
	}
	return m_list.getId(_paramName);
}
