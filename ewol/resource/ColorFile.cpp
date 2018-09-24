/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etk/uri/uri.hpp>
#include <ewol/debug.hpp>
#include <ewol/resource/ColorFile.hpp>
#include <ejson/ejson.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::resource::ColorFile);

ewol::resource::ColorFile::ColorFile() :
  gale::Resource(),
  // Set the list unodered
  m_list(0, false),
  m_errorColor(etk::color::orange) {
	addResourceType("ewol::ColorFile");
}

void ewol::resource::ColorFile::init(const etk::Uri& _uri) {
	ethread::RecursiveLock lock(m_mutex);
	gale::Resource::init(_uri.get());
	EWOL_DEBUG("CF : load \"" << _uri << "\"");
	reload();
	EWOL_DEBUG("List of all color : " << m_list.getKeys());
}

ewol::resource::ColorFile::~ColorFile() {
	// remove all element
	m_list.clear();
}


void ewol::resource::ColorFile::reload() {
	ethread::RecursiveLock lock(m_mutex);
	// remove all previous set of value :
	for (size_t iii = 0; iii < m_list.size() ; ++iii) {
		m_list.getValue(iii) = m_errorColor;
	}
	// open and read all json elements:
	ejson::Document doc;
	if (doc.load(etk::Uri(m_name)) == false) {
		EWOL_ERROR("Can not load file : '" << m_name << "'");
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
		etk::String name = tmpObj["name"].toString().get();
		etk::String color = tmpObj["color"].toString().get(m_errorColor.getHexString());
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


int32_t ewol::resource::ColorFile::request(const etk::String& _paramName) {
	ethread::RecursiveLock lock(m_mutex);
	// check if the parameters existed :
	if (m_list.exist(_paramName) == false) {
		m_list.add(_paramName, m_errorColor);
	}
	return m_list.getId(_paramName);
}
