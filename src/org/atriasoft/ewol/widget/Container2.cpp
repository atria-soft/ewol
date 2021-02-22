/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */


#include <ewol/ewol.hpp>
#include <ewol/widget/Container2.hpp>
#include <ewol/widget/Manager.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::Container2);

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
	if (m_subWidget[_idWidget] != null) {
		Log.verbose("Add widget : " << _idWidget);
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
		if (m_subWidget[iii] != null) {
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
	if (m_subWidget[_idWidget] != null) {
		Log.verbose("Remove widget : " << _idWidget);
		m_subWidget[_idWidget]->removeParent();
		m_subWidget[_idWidget].reset();
		markToRedraw();
		requestUpdateSize();
	}
}

void ewol::widget::Container2::subWidgetUnLink(int32_t _idWidget) {
	if (m_subWidget[_idWidget] != null) {
		m_subWidget[_idWidget]->removeParent();
		Log.verbose("Unlink widget : " << _idWidget);
	}
	m_subWidget[_idWidget].reset();
}

ewol::ObjectShared ewol::widget::Container2::getSubObjectNamed(const etk::String& _widgetName) {
	ewol::ObjectShared tmpObject = ewol::Widget::getSubObjectNamed(_widgetName);
	if (tmpObject != null) {
		return tmpObject;
	}
	if (m_subWidget[0] != null) {
		tmpObject = m_subWidget[0]->getSubObjectNamed(_widgetName);
		if (tmpObject != null) {
			return tmpObject;
		}
	}
	if (m_subWidget[1] != null) {
		return m_subWidget[1]->getSubObjectNamed(_widgetName);
	}
	return null;
}

void ewol::widget::Container2::systemDraw(const ewol::DrawProperty& _displayProp) {
	if (propertyHide.get() == true){
		// widget is hidden ...
		return;
	}
	ewol::Widget::systemDraw(_displayProp);
	if (m_subWidget[m_idWidgetDisplayed] != null) {
		//Log.info("Draw : [" << propertyName << "] t=" << getObjectType() << " o=" << m_origin << "  s=" << m_size);
		m_subWidget[m_idWidgetDisplayed]->systemDraw(_displayProp);
	}
}

ewol::Padding ewol::widget::Container2::onChangeSizePadded(const ewol::Padding& _padding) {
	ewol::Widget::onChangeSize();
	Vector2f localAvaillable = m_size - Vector2f(_padding.x(), _padding.y());
	// Checkin the filling properties  == > for the subElements:
	Vector2f subElementSize = m_minSize;
	if (propertyFill->x() == true) {
		subElementSize.setX(m_size.x());
	}
	if (propertyFill->y() == true) {
		subElementSize.setY(m_size.y());
	}
	Vector2f delta = ewol::gravityGenerateDelta(propertyGravity, m_size - subElementSize);
	Vector2f origin = delta + Vector2f(_padding.xLeft(), _padding.yButtom());
	subElementSize -= Vector2f(_padding.x(), _padding.y());
	for (size_t iii = 0; iii < 2; ++iii) {
		if (m_subWidget[iii] != null) {
			Vector2f origin2 = origin+m_offset;
			Vector2f minSize = m_subWidget[iii]->getCalculateMinSize();
			//Vector2b expand = m_subWidget[iii]->propertyExpand.get();
			origin2 += ewol::gravityGenerateDelta(propertyGravity, minSize - localAvaillable);
			m_subWidget[iii]->setOrigin(m_origin + origin);
			m_subWidget[iii]->setSize(subElementSize);
			m_subWidget[iii]->onChangeSize();
		}
	}
	Vector2f selectableAreaPos = origin-Vector2f(_padding.xLeft(), _padding.yButtom());
	Vector2f selectableAreaEndPos = m_size - (selectableAreaPos + subElementSize + Vector2f(_padding.x(), _padding.y()));
	markToRedraw();
	return ewol::Padding(selectableAreaPos.x(),
	                     selectableAreaEndPos.y(),
	                     selectableAreaEndPos.x(),
	                     selectableAreaPos.y());
}

void ewol::widget::Container2::calculateMinMaxSizePadded(const ewol::Padding& _padding) {
	// call main class
	m_minSize = Vector2f(0,0);
	// call sub classes
	for (size_t iii = 0; iii < 2; ++iii) {
		if (m_subWidget[iii] != null) {
			m_subWidget[iii]->calculateMinMaxSize();
			Vector2f min = m_subWidget[iii]->getCalculateMinSize();
			m_minSize.setMax(min);
		}
	}
	// add padding :
	m_minSize += Vector2f(_padding.x(), _padding.y());
	// verify the min max of the min size ...
	checkMinSize();
	markToRedraw();
}

void ewol::widget::Container2::onRegenerateDisplay() {
	if (m_subWidget[m_idWidgetDisplayed] != null) {
		m_subWidget[m_idWidgetDisplayed]->onRegenerateDisplay();
	}
}
/*
ewol::WidgetShared ewol::widget::Container2::getWidgetAtPos(const Vector2f& _pos) {
	if (isHide() == false) {
		if (m_subWidget[m_idWidgetDisplayed] != null) {
			return m_subWidget[m_idWidgetDisplayed]->getWidgetAtPos(_pos);
		}
	}
	return null;
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
	Log.verbose("Create en element 2 ... with nodes.size()=" << _node.nodes.size());
	// parse all the elements:
	for(const auto it : _node.nodes) {
		Log.verbose("    node: " << it);
		exml::Element pNode = it.toElement();
		if (pNode.exist() == false) {
			// trash here all that is not element
			continue;
		}
		etk::String widgetName = pNode.getValue();
		if (getWidgetManager().exist(widgetName) == false) {
			Log.error("(l " << pNode.getPos() << ") Unknown basic node='" << widgetName << "' not in: [" << getWidgetManager().list() << "]" );
			continue;
		}
		bool toogleMode=false;
		if (getSubWidget() != null) {
			toogleMode=true;
			if (getSubWidgetToggle() != null) {
				Log.error("(l " << pNode.getPos() << ") Can only have one subWidget ??? node='" << widgetName << "'" );
				continue;
			}
		}
		Log.debug("try to create subwidget : '" << widgetName << "'");
		ewol::WidgetShared tmpWidget = getWidgetManager().create(widgetName, pNode);
		if (tmpWidget == null) {
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

void ewol::widget::Container2::setOffset(const Vector2f& _newVal) {
	if (m_offset != _newVal) {
		ewol::Widget::setOffset(_newVal);
		// recalculate the new sise and position of sub widget ...
		calculateSize();
	}
}

void ewol::widget::Container2::requestDestroyFromChild(const ewol::ObjectShared& _child) {
	if (m_subWidget[0] == _child) {
		if (m_subWidget[0] == null) {
			return;
		}
		m_subWidget[0]->removeParent();
		m_subWidget[0].reset();
		markToRedraw();
	}
	if (m_subWidget[1] == _child) {
		if (m_subWidget[1] == null) {
			return;
		}
		m_subWidget[1]->removeParent();
		m_subWidget[1].reset();
		markToRedraw();
	}
}

void ewol::widget::Container2::drawWidgetTree(int32_t _level) {
	ewol::Widget::drawWidgetTree(_level);
	_level++;
	if (m_subWidget[0] != null) {
		m_subWidget[0]->drawWidgetTree(_level);
	}
	if (m_subWidget[1] != null) {
		m_subWidget[1]->drawWidgetTree(_level);
	}
}
