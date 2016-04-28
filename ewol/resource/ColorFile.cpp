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
	if (doc.load(m_name) == false) {
		EWOL_ERROR("Can not load file : '" << m_name << "' = " << etk::FSNode(m_name).getFileSystemName());
		return;
	}
	ejson::Array baseArray = doc["color"].toArray();
	if (baseArray.exist() == false) {
		EWOL_ERROR("Can not get basic array : 'color' in file:" << m_name);
		doc.display();
		return;
	}
	bool findError = false;
	for (const auto it : baseArray) {
		ejson::Object tmpObj = it.toObject();
		if (tmpObj.exist() == false) {
			EWOL_ERROR(" can not get object in 'color' : " << it);
			findError = true;
			continue;
		}
		std::string name = tmpObj["name"].toString().get();
		std::string color = tmpObj["color"].toString().get(m_errorColor.getHexString());
		EWOL_DEBUG("find new color : '" << name << "' color='" << color << "'");
		if (name.size() == 0) {
			EWOL_ERROR("Drop an empty name");
			findError = true;
			continue;
		}
		m_list.add(name, etk::Color<float>(color));
	}
	if (findError == true) {
		EWOL_ERROR("pb in parsing file:" << m_name);
		doc.display();
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
