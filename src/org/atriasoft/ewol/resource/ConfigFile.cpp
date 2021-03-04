/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etk/uri/uri.hpp>
#include <ewol/debug.hpp>
#include <ewol/resource/ConfigFile.hpp>
#include <gale/resource/Manager.hpp>
#include <ejson/ejson.hpp>
#include <ejson/Number.hpp>
#include <ejson/String.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::resource::ConfigFile);

ewol::resource::ConfigFile::ConfigFile() :
  gale::Resource(),
  // set map unorderred
  this.list(0, false) {
	addResourceType("ewol::ConfigFile");
}

void ewol::resource::ConfigFile::init( etk::Uri _uri) {
	ethread::RecursiveLock lock(this.mutex);
	gale::Resource::init(_uri.get());
	Log.debug("SFP : load \"" + _uri + "\"");
	reload();
}


ewol::resource::ConfigFile::~ConfigFile() {
	this.list.clear();
}

void ewol::resource::ConfigFile::reload() {
	ethread::RecursiveLock lock(this.mutex);
	// reset all parameters
	for (int iii=0; iii<this.list.size(); ++iii){
		if (this.list.getValue(iii).exist() == true) {
			this.list.getValue(iii) = ejson::empty();
		}
	}
	this.doc.load(etk::Uri(this.name));
	
	for (auto elementName : this.list.getKeys()) {
		if (this.doc[elementName].exist() == true) {
			this.list[elementName] = this.doc[elementName];
		}
	}
}


int ewol::resource::ConfigFile::request( String _paramName) {
	ethread::RecursiveLock lock(this.mutex);
	// check if the parameters existed :
	if (this.list.exist(_paramName) == false) {
		this.list.add(_paramName, ejson::empty());
	}
	if (this.doc[_paramName].exist() == true) {
		this.list[_paramName] = this.doc[_paramName];
	}
	return this.list.getId(_paramName);
}


double ewol::resource::ConfigFile::getNumber(int _id) {
	ethread::RecursiveLock lock(this.mutex);
	if (    _id < 0
	     || this.list.getValue(_id).exist() == false) {
		return 0.0;
	}
	return this.list.getValue(_id).toNumber().get();
}

String ewol::resource::ConfigFile::getString(int _id) {
	ethread::RecursiveLock lock(this.mutex);
	if (    _id < 0
	     || this.list.getValue(_id).exist() == false) {
		return "";
	}
	return this.list.getValue(_id).toString().get();
}

boolean ewol::resource::ConfigFile::getBoolean(int _id) {
	ethread::RecursiveLock lock(this.mutex);
	if (    _id < 0
	     || this.list.getValue(_id).exist() == false) {
		return false;
	}
	return this.list.getValue(_id).toBoolean().get();
}
