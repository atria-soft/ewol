/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */


#include <ewol/ewol.hpp>
#include <ewol/widget/Composer.hpp>
#include <etk/os/FSNode.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/context/Context.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::Composer);

ewol::widget::Composer::Composer() :
  propertyRemoveIfUnderRemove(this, "remove-if-under-remove", true, "Demand the remove iof the widget if the subObject demand a remove"),
  propertySubFile(this, "sub-file", "", "compose with a subXML file", &ewol::widget::Composer::onChangePropertySubFile) {
	addObjectType("ewol::widget::Composer");
	// nothing to do ...
}

ewol::WidgetShared ewol::widget::composerGenerateFile(const etk::String& _fileName, uint64_t _id) {
	etk::String tmpData = etk::FSNodeReadAllData(_fileName);
	return ewol::widget::composerGenerateString(tmpData, _id);
}

ewol::WidgetShared ewol::widget::composerGenerateString(const etk::String& _data, uint64_t _id) {
	ewol::widget::Manager& widgetManager = ewol::getContext().getWidgetManager();
	if (_data == "") {
		return null;
	}
	exml::Document doc;
	etk::String tmpData = _data;
	// replace all elements:
	if (_id != 0) {
		tmpData.replace("{ID}", etk::toString(_id));
	}
	if (doc.parse(tmpData) == false) {
		EWOL_ERROR(" can not load file XML string...");
		return null;
	}
	exml::Element root = doc.toElement();
	if (root.nodes.size() == 0) {
		EWOL_ERROR(" (l ?) No node in the XML file/string.");
		return null;
	}
	if (root.nodes.size() > 1) {
		EWOL_WARNING(" (l ?) More than 1 node in the XML file/string. (JUST parse the first)");
	}
	exml::Element pNode = root.nodes[0].toElement();
	if (pNode.exist() == false) {
		EWOL_ERROR(" (l ?) No node in the XML file/string. {2}");
		return null;
	}
	etk::String widgetName = pNode.getValue();
	if (widgetManager.exist(widgetName) == false) {
		EWOL_ERROR("(l " << pNode.getPos() << ") Unknown basic node='" << widgetName << "' not in : [" << widgetManager.list() << "]" );
		return null;
	}
	EWOL_DEBUG("try to create subwidget : '" << widgetName << "'");
	ewol::WidgetShared tmpWidget = widgetManager.create(widgetName);
	if (tmpWidget == null) {
		EWOL_ERROR ("(l " << pNode.getPos() << ") Can not create the widget : '" << widgetName << "'");
		return null;
	}
	if (tmpWidget->loadXML(pNode) == false) {
		EWOL_ERROR ("(l " << pNode.getPos() << ") can not load widget properties : '" << widgetName << "'");
	}
	return tmpWidget;
}

ewol::widget::Composer::~Composer() {
	
}

bool ewol::widget::Composer::loadFromFile(const etk::String& _fileName, uint64_t _id) {
	etk::String tmpData = etk::FSNodeReadAllData(_fileName);
	return loadFromString(tmpData, _id);
}

bool ewol::widget::Composer::loadFromString(const etk::String& _composerXmlString, uint64_t _id) {
	exml::Document doc;
	etk::String tmpData = _composerXmlString;
	// replace all elements:
	if (_id != 0) {
		tmpData.replace("{ID}", etk::toString(_id));
	}
	if (doc.parse(tmpData) == false) {
		EWOL_ERROR(" can not load file XML string...");
		return false;
	}
	exml::Element root = doc.nodes["composer"];
	if (root.exist() == false) {
		// Maybe a multiple node XML for internal config:
		root = doc.toElement();
		if (root.exist() == false) {
			EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} (l ?) main node not find: 'composer' ...");
			return false;
		}
		if (root.nodes.size() == 0) {
			EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} (l ?) no node in the Container XML element.");
			return false;
		}
	}
	// call upper class to parse his elements ...
	ewol::widget::Container::loadXML(root);
	if (m_subWidget == null) {
		EWOL_WARNING("Load data from composer and have no under Widget after loading");
		if (_composerXmlString.size() != 0) {
			EWOL_ERROR("Error Loading XML data : " << _composerXmlString);
			return false;
		}
	}
	requestUpdateSize();
	return true;
}

void ewol::widget::Composer::requestDestroyFromChild(const ewol::ObjectShared& _child) {
	ewol::widget::Container::requestDestroyFromChild(_child);
	if (*propertyRemoveIfUnderRemove == true) {
		EWOL_DEBUG("Child widget remove ==> auto-remove");
		autoDestroy();
	}
}

void ewol::widget::Composer::onChangePropertySubFile() {
	EWOL_INFO("Load compositing form external file : " << propertySubFile);
	if (*propertySubFile == "") {
		// remove all elements:
		subWidgetRemove();
		return;
	}
	if (loadFromFile(*propertySubFile, getId()) == false) {
		EWOL_ERROR("Can not load Player GUI from file ... " << propertySubFile);
		return;
	}
}

bool ewol::widget::Composer::loadXML(const exml::Element& _node) {
	//EWOL_VERBOSE("[" << getId() << "] t=" << getObjectType() << " Load XML (start)");
	if (_node.exist() == false) {
		return false;
	}
	// parse generic properties:
	ewol::Widget::loadXML(_node);
	// parse all the elements:
	if (_node.nodes.size() != 0) {
		EWOL_ERROR("a composer Node Can not have Sub-element in XML ==> must be done in an external file and load it with attribute: 'sub-file'");
	}
	//drawWidgetTree();
	//EWOL_VERBOSE("[" << getId() << "] t=" << getObjectType() << " Load XML (stop)");
	return true;
}