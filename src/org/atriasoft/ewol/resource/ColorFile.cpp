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
  this.list(0, false),
  this.errorColor(etk::color::orange) {
	addResourceType("ewol::ColorFile");
}

void ewol::resource::ColorFile::init( etk::Uri _uri) {
	ethread::RecursiveLock lock(this.mutex);
	gale::Resource::init(_uri.get());
	Log.debug("CF : load \"" + _uri + "\"");
	reload();
	Log.debug("List of all color : " + this.list.getKeys());
}

ewol::resource::ColorFile::~ColorFile() {
	// remove all element
	this.list.clear();
}


void ewol::resource::ColorFile::reload() {
	ethread::RecursiveLock lock(this.mutex);
	// remove all previous set of value :
	for (int iii = 0; iii < this.list.size() ; ++iii) {
		this.list.getValue(iii) = this.errorColor;
	}
	// open and read all json elements:
	ejson::Document doc;
	if (doc.load(etk::Uri(this.name)) == false) {
		Log.error("Can not load file : '" + this.name + "'");
		return;
	}
	ejson::Array baseArray = doc["color"].toArray();
	if (baseArray.exist() == false) {
		Log.error("Can not get basic array : 'color' in file:" + this.name);
		doc.display();
		return;
	}
	boolean findError = false;
	for ( auto it : baseArray) {
		ejson::Object tmpObj = it.toObject();
		if (tmpObj.exist() == false) {
			Log.error(" can not get object in 'color' : " + it);
			findError = true;
			continue;
		}
		String name = tmpObj["name"].toString().get();
		String color = tmpObj["color"].toString().get(this.errorColor.getHexString());
		Log.debug("find new color : '" + name + "' color='" + color + "'");
		if (name.size() == 0) {
			Log.error("Drop an empty name");
			findError = true;
			continue;
		}
		this.list.add(name, etk::Color<float>(color));
	}
	if (findError == true) {
		Log.error("pb in parsing file:" + this.name);
		doc.display();
	}
}


int ewol::resource::ColorFile::request( String _paramName) {
	ethread::RecursiveLock lock(this.mutex);
	// check if the parameters existed :
	if (this.list.exist(_paramName) == false) {
		this.list.add(_paramName, this.errorColor);
	}
	return this.list.getId(_paramName);
}
