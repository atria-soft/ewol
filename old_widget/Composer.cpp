/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */


#include <ewol/ewol.hpp>
#include <ewol/widget/Composer.hpp>
#include <etk/uri/uri.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/context/Context.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::Composer);

ewol::widget::Composer::Composer() :
  propertyRemoveIfUnderRemove(this, "remove-if-under-remove", true, "Demand the remove iof the widget if the subObject demand a remove"),
  propertySubFile(this, "sub-file", "", "compose with a subXML file", ewol::widget::Composer::onChangePropertySubFile) {
	addObjectType("ewol::widget::Composer");
	// nothing to do ...
}

Widget ewol::widget::composerGenerateFile( etk::Uri _uri, ulong _id) {
	String tmpData;
	if (etk::uri::readAll(_uri, tmpData) == false) {
		Log.error("Can not read the file: " + _uri);
		return null;
	}
	return ewol::widget::composerGenerateString(tmpData, _id);
}

Widget ewol::widget::composerGenerateString( String _data, ulong _id) {
	ewol::widget::Manager widgetManager = ewol::getContext().getWidgetManager();
	if (_data == "") {
		return null;
	}
	exml::Document doc;
	String tmpData = _data;
	// replace all elements:
	if (_id != 0) {
		tmpData.replace("{ID}", etk::toString(_id));
	}
	if (doc.parse(tmpData) == false) {
		Log.error(" can not load file XML string...");
		return null;
	}
	exml::Element root = doc.toElement();
	if (root.nodes.size() == 0) {
		Log.error(" (l ?) No node in the XML file/string.");
		return null;
	}
	if (root.nodes.size() > 1) {
		Log.warning(" (l ?) More than 1 node in the XML file/string. (JUST parse the first)");
	}
	exml::Element pNode = root.nodes[0].toElement();
	if (pNode.exist() == false) {
		Log.error(" (l ?) No node in the XML file/string. {2}");
		return null;
	}
	String widgetName = pNode.getValue();
	if (widgetManager.exist(widgetName) == false) {
		Log.error("(l " + pNode.getPos() + ") Unknown basic node='" + widgetName + "' not in : [" + widgetManager.list() + "]" );
		return null;
	}
	Log.debug("try to create subwidget : '" + widgetName + "'");
	Widget tmpWidget = widgetManager.create(widgetName);
	if (tmpWidget == null) {
		EWOL_ERROR ("(l " + pNode.getPos() + ") Can not create the widget : '" + widgetName + "'");
		return null;
	}
	if (tmpWidget.loadXML(pNode) == false) {
		EWOL_ERROR ("(l " + pNode.getPos() + ") can not load widget properties : '" + widgetName + "'");
	}
	return tmpWidget;
}

ewol::widget::Composer::~Composer() {
	
}

boolean ewol::widget::Composer::loadFromFile( etk::Uri _uri, ulong _id) {
	String tmpData;
	if (etk::uri::readAll(_uri, tmpData) == false) {
		Log.error("Can not read the file: " + _uri);
		return false;
	}
	return loadFromString(tmpData, _id);
}

boolean ewol::widget::Composer::loadFromString( String _composerXmlString, ulong _id) {
	exml::Document doc;
	String tmpData = _composerXmlString;
	// replace all elements:
	if (_id != 0) {
		tmpData.replace("{ID}", etk::toString(_id));
	}
	if (doc.parse(tmpData) == false) {
		Log.error(" can not load file XML string...");
		return false;
	}
	exml::Element root = doc.nodes["composer"];
	if (root.exist() == false) {
		// Maybe a multiple node XML for internal config:
		root = doc.toElement();
		if (root.exist() == false) {
			Log.error("[" + getId() + "] {" + getObjectType() + "} (l ?) main node not find: 'composer' ...");
			return false;
		}
		if (root.nodes.size() == 0) {
			Log.error("[" + getId() + "] {" + getObjectType() + "} (l ?) no node in the Container XML element.");
			return false;
		}
	}
	// call upper class to parse his elements ...
	ewol::widget::Container::loadXML(root);
	if (this.subWidget == null) {
		Log.warning("Load data from composer and have no under Widget after loading");
		if (_composerXmlString.size() != 0) {
			Log.error("Error Loading XML data : " + _composerXmlString);
			return false;
		}
	}
	requestUpdateSize();
	return true;
}

void ewol::widget::Composer::requestDestroyFromChild( EwolObject _child) {
	ewol::widget::Container::requestDestroyFromChild(_child);
	if (*propertyRemoveIfUnderRemove == true) {
		Log.debug("Child widget remove ==> auto-remove");
		autoDestroy();
	}
}

void ewol::widget::Composer::onChangePropertySubFile() {
	Log.info("Load compositing form external file : " + propertySubFile);
	if (*propertySubFile == "") {
		// remove all elements:
		subWidgetRemove();
		return;
	}
	if (loadFromFile(*propertySubFile, getId()) == false) {
		Log.error("Can not load Player GUI from file ... " + propertySubFile);
		return;
	}
}

boolean ewol::widget::Composer::loadXML( exml::Element _node) {
	//Log.verbose("[" + getId() + "] t=" + getObjectType() + " Load XML (start)");
	if (_node.exist() == false) {
		return false;
	}
	// parse generic properties:
	Widget::loadXML(_node);
	// parse all the elements:
	if (_node.nodes.size() != 0) {
		Log.error("a composer Node Can not have Sub-element in XML ==> must be done in an external file and load it with attribute: 'sub-file'");
	}
	//drawWidgetTree();
	//Log.verbose("[" + getId() + "] t=" + getObjectType() + " Load XML (stop)");
	return true;
}