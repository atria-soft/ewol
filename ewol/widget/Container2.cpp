/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */


#include <ewol/ewol.hpp>
#include <ewol/widget/Container2.hpp>
#include <ewol/widget/Manager.hpp>

ewol::widget::Container2::Container2() :
  m_idWidgetDisplayed(0) {
	addObjectType("ewol::widget::Container2");
}

ewol::widget::Container2::~Container2() {
	subWidgetRemove();
	subWidgetRemoveToggle();
}

void ewol::widget::Container2::setSubWidget(ewol::WidgetShared _newWidget, int32_t _idWidget) {
	subWidgetRemove(_idWidget);
	m_subWidget[_idWidget] = _newWidget;
	if (m_subWidget[_idWidget] != nullptr) {
		EWOL_VERBOSE("Add widget : " << _idWidget);
		m_subWidget[_idWidget]->setParent(sharedFromThis());
	}
	markToRedraw();
	requestUpdateSize();
}

void ewol::widget::Container2::subWidgetReplace(const ewol::WidgetShared& _oldWidget,
                                                const ewol::WidgetShared& _newWidget) {
	bool haveChange = false;
	for (size_t iii=0; iii<2; ++iii) {
		if (m_subWidget[iii] != _oldWidget) {
			continue;
		}
		m_subWidget[iii]->removeParent();
		m_subWidget[iii].reset();
		m_subWidget[iii] = _newWidget;
		if (m_subWidget[iii] != nullptr) {
			m_subWidget[iii]->setParent(sharedFromThis());
		}
		haveChange = true;
	}
	if (haveChange == false) {
		EWOL_WARNING("Request replace with a wrong old widget");
		return;
	}
	markToRedraw();
	requestUpdateSize();
}


void ewol::widget::Container2::subWidgetRemove(int32_t _idWidget) {
	if (m_subWidget[_idWidget] != nullptr) {
		EWOL_VERBOSE("Remove widget : " << _idWidget);
		m_subWidget[_idWidget]->removeParent();
		m_subWidget[_idWidget].reset();
		markToRedraw();
		requestUpdateSize();
	}
}

void ewol::widget::Container2::subWidgetUnLink(int32_t _idWidget) {
	if (m_subWidget[_idWidget] != nullptr) {
		m_subWidget[_idWidget]->removeParent();
		EWOL_VERBOSE("Unlink widget : " << _idWidget);
	}
	m_subWidget[_idWidget].reset();
}

ewol::ObjectShared ewol::widget::Container2::getSubObjectNamed(const std::string& _widgetName) {
	ewol::ObjectShared tmpObject = ewol::Widget::getSubObjectNamed(_widgetName);
	if (tmpObject != nullptr) {
		return tmpObject;
	}
	if (m_subWidget[0] != nullptr) {
		tmpObject = m_subWidget[0]->getSubObjectNamed(_widgetName);
		if (tmpObject != nullptr) {
			return tmpObject;
		}
	}
	if (m_subWidget[1] != nullptr) {
		return m_subWidget[1]->getSubObjectNamed(_widgetName);
	}
	return nullptr;
}

void ewol::widget::Container2::systemDraw(const ewol::DrawProperty& _displayProp) {
	if (propertyHide.get() == true){
		// widget is hidden ...
		return;
	}
	ewol::Widget::systemDraw(_displayProp);
	if (m_subWidget[m_idWidgetDisplayed] != nullptr) {
		m_subWidget[m_idWidgetDisplayed]->systemDraw(_displayProp);
	}
}

ewol::Padding ewol::widget::Container2::onChangeSizePadded(const ewol::Padding& _padding) {
	ewol::Widget::onChangeSize();
	vec2 localAvaillable = m_size - vec2(_padding.x(), _padding.y());
	// Checkin the filling properties  == > for the subElements:
	vec2 subElementSize = m_minSize;
	if (propertyFill->x() == true) {
		subElementSize.setX(m_size.x());
	}
	if (propertyFill->y() == true) {
		subElementSize.setY(m_size.y());
	}
	vec2 delta = ewol::gravityGenerateDelta(propertyGravity, m_size - subElementSize);
	vec2 origin = delta + vec2(_padding.xLeft(), _padding.yButtom());
	subElementSize -= vec2(_padding.x(), _padding.y());
	for (size_t iii = 0; iii < 2; ++iii) {
		if (m_subWidget[iii] != nullptr) {
			vec2 origin2 = origin+m_offset;
			vec2 minSize = m_subWidget[iii]->getCalculateMinSize();
			//bvec2 expand = m_subWidget[iii]->propertyExpand.get();
			origin2 += ewol::gravityGenerateDelta(propertyGravity, minSize - localAvaillable);
			m_subWidget[iii]->setOrigin(m_origin + origin);
			m_subWidget[iii]->setSize(subElementSize);
			m_subWidget[iii]->onChangeSize();
		}
	}
	vec2 selectableAreaPos = origin-vec2(_padding.xLeft(), _padding.yButtom());
	vec2 selectableAreaEndPos = m_size - (selectableAreaPos + subElementSize + vec2(_padding.x(), _padding.y()));
	markToRedraw();
	return ewol::Padding(selectableAreaPos.x(),
	                     selectableAreaEndPos.y(),
	                     selectableAreaEndPos.x(),
	                     selectableAreaPos.y());
}

void ewol::widget::Container2::calculateMinMaxSizePadded(const ewol::Padding& _padding) {
	// call main class
	m_minSize = vec2(0,0);
	// call sub classes
	for (size_t iii = 0; iii < 2; ++iii) {
		if (m_subWidget[iii] != nullptr) {
			m_subWidget[iii]->calculateMinMaxSize();
			vec2 min = m_subWidget[iii]->getCalculateMinSize();
			m_minSize.setMax(min);
		}
	}
	// add padding :
	m_minSize += vec2(_padding.x(), _padding.y());
	// verify the min max of the min size ...
	checkMinSize();
	markToRedraw();
}

void ewol::widget::Container2::onRegenerateDisplay() {
	if (m_subWidget[m_idWidgetDisplayed] != nullptr) {
		m_subWidget[m_idWidgetDisplayed]->onRegenerateDisplay();
	}
}
/*
ewol::WidgetShared ewol::widget::Container2::getWidgetAtPos(const vec2& _pos) {
	if (isHide() == false) {
		if (m_subWidget[m_idWidgetDisplayed] != nullptr) {
			return m_subWidget[m_idWidgetDisplayed]->getWidgetAtPos(_pos);
		}
	}
	return nullptr;
}
*/

bool ewol::widget::Container2::loadXML(const exml::Element& _node) {
	if (_node.exist() == false) {
		return false;
	}
	// parse generic properties :
	ewol::Widget::loadXML(_node);
	// remove previous element :
	subWidgetRemove();
	EWOL_VERBOSE("Create en element 2 ... with nodes.size()=" << _node.nodes.size());
	// parse all the elements:
	for(const auto it : _node.nodes) {
		EWOL_VERBOSE("    node: " << it);
		exml::Element pNode = it.toElement();
		if (pNode.exist() == false) {
			// trash here all that is not element
			continue;
		}
		std::string widgetName = pNode.getValue();
		if (getWidgetManager().exist(widgetName) == false) {
			EWOL_ERROR("(l " << pNode.getPos() << ") Unknown basic node='" << widgetName << "' not in: [" << getWidgetManager().list() << "]" );
			continue;
		}
		bool toogleMode=false;
		if (getSubWidget() != nullptr) {
			toogleMode=true;
			if (getSubWidgetToggle() != nullptr) {
				EWOL_ERROR("(l " << pNode.getPos() << ") Can only have one subWidget ??? node='" << widgetName << "'" );
				continue;
			}
		}
		EWOL_DEBUG("try to create subwidget : '" << widgetName << "'");
		ewol::WidgetShared tmpWidget = getWidgetManager().create(widgetName);
		if (tmpWidget == nullptr) {
			EWOL_ERROR ("(l " << pNode.getPos() << ") Can not create the widget: '" << widgetName << "'");
			continue;
		}
		// add widget :
		if (toogleMode == false) {
			setSubWidget(tmpWidget);
		} else {
			setSubWidgetToggle(tmpWidget);
		}
		if (tmpWidget->loadXML(pNode) == false) {
			EWOL_ERROR ("(l "<<pNode.getPos()<<") can not load widget properties: '" << widgetName << "'");
			return false;
		}
	}
	return true;
}

void ewol::widget::Container2::setOffset(const vec2& _newVal) {
	if (m_offset != _newVal) {
		ewol::Widget::setOffset(_newVal);
		// recalculate the new sise and position of sub widget ...
		calculateSize();
	}
}

void ewol::widget::Container2::requestDestroyFromChild(const ewol::ObjectShared& _child) {
	if (m_subWidget[0] == _child) {
		if (m_subWidget[0] == nullptr) {
			return;
		}
		m_subWidget[0]->removeParent();
		m_subWidget[0].reset();
		markToRedraw();
	}
	if (m_subWidget[1] == _child) {
		if (m_subWidget[1] == nullptr) {
			return;
		}
		m_subWidget[1]->removeParent();
		m_subWidget[1].reset();
		markToRedraw();
	}
}


