/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */


#include <ewol/ewol.hpp>
#include <ewol/widget/Container.hpp>
#include <ewol/widget/Manager.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::Container);

ewol::widget::Container::Container() {
	addObjectType("ewol::widget::Container");
	// nothing to do ...
}

ewol::widget::Container::~Container() {
	subWidgetRemove();
}

ewol::WidgetShared ewol::widget::Container::getSubWidget() {
	return m_subWidget;
}

void ewol::widget::Container::setSubWidget(ewol::WidgetShared _newWidget) {
	if (_newWidget == null) {
		return;
	}
	subWidgetRemove();
	m_subWidget = _newWidget;
	if (m_subWidget != null) {
		m_subWidget->setParent(sharedFromThis());
	}
	markToRedraw();
	requestUpdateSize();
}

void ewol::widget::Container::subWidgetReplace(const ewol::WidgetShared& _oldWidget,
                                               const ewol::WidgetShared& _newWidget) {
	if (m_subWidget != _oldWidget) {
		EWOL_WARNING("Request replace with a wrong old widget");
		return;
	}
	m_subWidget->removeParent();
	m_subWidget.reset();
	m_subWidget = _newWidget;
	if (m_subWidget != null) {
		m_subWidget->setParent(sharedFromThis());
	}
	markToRedraw();
	requestUpdateSize();
}

void ewol::widget::Container::subWidgetRemove() {
	if (m_subWidget != null) {
		m_subWidget->removeParent();
		m_subWidget.reset();
		markToRedraw();
		requestUpdateSize();
	}
}

void ewol::widget::Container::subWidgetUnLink() {
	if (m_subWidget != null) {
		m_subWidget->removeParent();
	}
	m_subWidget.reset();
}

ewol::ObjectShared ewol::widget::Container::getSubObjectNamed(const etk::String& _objectName) {
	ewol::ObjectShared tmpObject = ewol::Widget::getSubObjectNamed(_objectName);
	if (tmpObject != null) {
		return tmpObject;
	}
	if (m_subWidget != null) {
		return m_subWidget->getSubObjectNamed(_objectName);
	}
	return null;
}

void ewol::widget::Container::systemDraw(const ewol::DrawProperty& _displayProp) {
	if (propertyHide.get() == true){
		// widget is hidden ...
		return;
	}
	ewol::Widget::systemDraw(_displayProp);
	if (m_subWidget != null) {
		ewol::DrawProperty prop = _displayProp;
		prop.limit(m_origin, m_size);
		//EWOL_INFO("Draw : [" << propertyName << "] t=" << getObjectType() << " o=" << m_origin << "  s=" << m_size);
		m_subWidget->systemDraw(prop);
	} else {
		EWOL_INFO("[" << getId() << "]       ++++++ : [null]");
	}
}

void ewol::widget::Container::onChangeSize() {
	ewol::Widget::onChangeSize();
	if (*propertyHide == true) {
		return;
	}
	if (m_subWidget == null) {
		return;
	}
	vec2 origin = m_origin+m_offset;
	vec2 minSize = m_subWidget->getCalculateMinSize();
	bvec2 expand = m_subWidget->propertyExpand.get();
	origin += ewol::gravityGenerateDelta(propertyGravity.get(), minSize - m_size);
	m_subWidget->setOrigin(origin);
	m_subWidget->setSize(m_size);
	m_subWidget->onChangeSize();
}

void ewol::widget::Container::calculateMinMaxSize() {
	// call main class
	ewol::Widget::calculateMinMaxSize();
	// call sub classes
	if (m_subWidget != null) {
		m_subWidget->calculateMinMaxSize();
		vec2 min = m_subWidget->getCalculateMinSize();
		m_minSize.setMax(min);
	}
	//EWOL_ERROR("[" << getId() << "] Result min size : " <<  m_minSize);
}

void ewol::widget::Container::onRegenerateDisplay() {
	if (m_subWidget != null) {
		m_subWidget->onRegenerateDisplay();
	}
}

ewol::WidgetShared ewol::widget::Container::getWidgetAtPos(const vec2& _pos) {
	if (propertyHide.get() == false) {
		if (m_subWidget != null) {
			return m_subWidget->getWidgetAtPos(_pos);
		}
	}
	return null;
};

bool ewol::widget::Container::loadXML(const exml::Element& _node) {
	if (_node.exist() == false) {
		return false;
	}
	// parse generic properties:
	ewol::Widget::loadXML(_node);
	// remove previous element:
	subWidgetRemove();
	// parse all the elements:
	for (const auto it : _node.nodes) {
		exml::Element pNode = it.toElement();
		if (pNode.exist() == false) {
			// trash here all that is not element
			continue;
		}
		etk::String widgetName = pNode.getValue();
		EWOL_VERBOSE("[" << getId() << "] t=" << getObjectType() << " Load node name : '" << widgetName << "'");
		if (getWidgetManager().exist(widgetName) == false) {
			EWOL_ERROR("(l " << pNode.getPos() << ") Unknown basic node='" << widgetName << "' not in : [" << getWidgetManager().list() << "]" );
			continue;
		}
		if (getSubWidget() != null) {
			EWOL_ERROR("(l " << pNode.getPos() << ") Can only have one subWidget ??? node='" << widgetName << "'" );
			continue;
		}
		EWOL_DEBUG("try to create subwidget : '" << widgetName << "'");
		ewol::WidgetShared tmpWidget = getWidgetManager().create(widgetName, pNode);
		if (tmpWidget == null) {
			EWOL_ERROR ("(l " << pNode.getPos() << ") Can not create the widget : '" << widgetName << "'");
			continue;
		}
		// add widget :
		setSubWidget(tmpWidget);
		if (tmpWidget->loadXML(pNode) == false) {
			EWOL_ERROR ("(l " << pNode.getPos() << ") can not load widget properties : '" << widgetName << "'");
			return false;
		}
	}
	if (    _node.nodes.size() != 0
	     && m_subWidget == null) {
		EWOL_WARNING("Load container with no data inside");
	}
	return true;
}

void ewol::widget::Container::setOffset(const vec2& _newVal) {
	if (m_offset != _newVal) {
		ewol::Widget::setOffset(_newVal);
		// recalculate the new sise and position of sub widget ...
		onChangeSize();
	}
}

void ewol::widget::Container::requestDestroyFromChild(const ewol::ObjectShared& _child) {
	if (m_subWidget != _child) {
		return;
	}
	if (m_subWidget == null) {
		return;
	}
	m_subWidget->removeParent();
	m_subWidget.reset();
	markToRedraw();
}

void ewol::widget::Container::drawWidgetTree(int32_t _level) {
	ewol::Widget::drawWidgetTree(_level);
	_level++;
	if (m_subWidget != null) {
		m_subWidget->drawWidgetTree(_level);
	}
}
