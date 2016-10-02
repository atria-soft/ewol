/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */


#include <ewol/ewol.hpp>
#include <ewol/widget/Composer.hpp>
#include <etk/os/FSNode.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/context/Context.hpp>

ewol::widget::Composer::Composer() :
  propertyRemoveIfUnderRemove(this, "remove-if-under-remove", true, "Demand the remove iof the widget if the subObject demand a remove") {
	addObjectType("ewol::widget::Composer");
	// nothing to do ...
}

static ewol::WidgetShared composerGenerate(bool _modeFile, const std::string& _data) {
	ewol::widget::Manager& widgetManager = ewol::getContext().getWidgetManager();
	if (_data == "") {
		return nullptr;
	}
	exml::Document doc;
	if (_modeFile == true) {
		if (doc.load(_data) == false) {
			EWOL_ERROR(" can not load file XML : " << _data);
			return nullptr;
		}
	} else {
		if (doc.parse(_data) == false) {
			EWOL_ERROR(" can not load file XML string...");
			return nullptr;
		}
	}
	exml::Element root = doc.toElement();
	if (root.nodes.size() == 0) {
		EWOL_ERROR(" (l ?) No node in the XML file/string.");
		return nullptr;
	}
	if (root.nodes.size() > 1) {
		EWOL_WARNING(" (l ?) More than 1 node in the XML file/string. (JUST parse the first)");
	}
	exml::Element pNode = root.nodes[0].toElement();
	if (pNode.exist() == false) {
		EWOL_ERROR(" (l ?) No node in the XML file/string. {2}");
		return nullptr;
	}
	std::string widgetName = pNode.getValue();
	if (widgetManager.exist(widgetName) == false) {
		EWOL_ERROR("(l " << pNode.getPos() << ") Unknown basic node='" << widgetName << "' not in : [" << widgetManager.list() << "]" );
		return nullptr;
	}
	EWOL_DEBUG("try to create subwidget : '" << widgetName << "'");
	ewol::WidgetShared tmpWidget = widgetManager.create(widgetName);
	if (tmpWidget == nullptr) {
		EWOL_ERROR ("(l " << pNode.getPos() << ") Can not create the widget : '" << widgetName << "'");
		return nullptr;
	}
	if (tmpWidget->loadXML(pNode) == false) {
		EWOL_ERROR ("(l " << pNode.getPos() << ") can not load widget properties : '" << widgetName << "'");
	}
	return tmpWidget;
}

ewol::WidgetShared ewol::widget::composerGenerateFile(const std::string& _data) {
	return composerGenerate(true, _data);
}

ewol::WidgetShared ewol::widget::composerGenerateString(const std::string& _data) {
	return composerGenerate(false, _data);
}

ewol::widget::Composer::~Composer() {
	
}

bool ewol::widget::Composer::loadFromFile(const std::string& _fileName) {
	exml::Document doc;
	if (doc.load(_fileName) == false) {
		EWOL_ERROR(" can not load file XML : " << _fileName);
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
	
	return true;
}

bool ewol::widget::Composer::loadFromString(const std::string& _composerXmlString) {
	exml::Document doc;
	if (doc.parse(_composerXmlString) == false) {
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
