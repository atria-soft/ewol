/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */


#include <ewol/ewol.h>
#include <ewol/widget/Container2.h>
#include <ewol/widget/Manager.h>


#undef __class__
#define __class__	"Container2"


ewol::widget::Container2::Container2() :
  m_idWidgetDisplayed(0) {
	addObjectType("ewol::widget::Container2");
	// nothing to do ...
}

void ewol::widget::Container2::init(std::shared_ptr<ewol::Widget> _subElement, std::shared_ptr<ewol::Widget> _subElementToggle) {
	ewol::Widget::init();
	m_subWidget[0] = _subElement;
	m_subWidget[1] = _subElementToggle;
}

ewol::widget::Container2::~Container2() {
	subWidgetRemove();
	subWidgetRemoveToggle();
}

void ewol::widget::Container2::setSubWidget(std::shared_ptr<ewol::Widget> _newWidget, int32_t _idWidget) {
	subWidgetRemove(_idWidget);
	m_subWidget[_idWidget] = _newWidget;
	if (m_subWidget[_idWidget] != nullptr) {
		EWOL_VERBOSE("Add widget : " << _idWidget);
		m_subWidget[_idWidget]->setParent(shared_from_this());
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

std::shared_ptr<ewol::Widget> ewol::widget::Container2::getWidgetNamed(const std::string& _widgetName) {
	std::shared_ptr<ewol::Widget> tmpUpperWidget = ewol::Widget::getWidgetNamed(_widgetName);
	if (tmpUpperWidget != nullptr) {
		return tmpUpperWidget;
	}
	if (m_subWidget[0] != nullptr) {
		return m_subWidget[0]->getWidgetNamed(_widgetName);
	}
	if (m_subWidget[1] != nullptr) {
		return m_subWidget[1]->getWidgetNamed(_widgetName);
	}
	return nullptr;
}

void ewol::widget::Container2::systemDraw(const ewol::DrawProperty& _displayProp) {
	if (true == m_hide){
		// widget is hidden ...
		return;
	}
	ewol::Widget::systemDraw(_displayProp);
	if (m_subWidget[m_idWidgetDisplayed] != nullptr) {
		m_subWidget[m_idWidgetDisplayed]->systemDraw(_displayProp);
	}
}

ewol::Padding ewol::widget::Container2::calculateSizePadded(const vec2& _availlable, const ewol::Padding& _padding) {
	vec2 localAvaillable = _availlable - vec2(_padding.x(), _padding.y());
	ewol::Widget::calculateSize(_availlable);
	// set minimal size
	if (m_userExpand->x() == true) {
		m_size.setX(_availlable.x());
	}
	if (m_userExpand->y() == true) {
		m_size.setY(_availlable.y());
	}
	// Checkin the filling properties  == > for the subElements:
	vec2 subElementSize = m_minSize;
	if (m_userFill->x() == true) {
		subElementSize.setX(m_size.x());
	}
	if (m_userFill->y() == true) {
		subElementSize.setY(m_size.y());
	}
	vec2 origin = (m_size - subElementSize)*0.5f + vec2(_padding.xLeft(), _padding.yButtom());
	subElementSize -= vec2(_padding.x(), _padding.y());
	for (size_t iii = 0; iii < 2; ++iii) {
		if (m_subWidget[iii] != nullptr) {
			vec2 origin2 = origin+m_offset;
			vec2 minSize = m_subWidget[iii]->getCalculateMinSize();
			bvec2 expand = m_subWidget[iii]->getExpand();
			if (    expand.x() == false
			    || minSize.x() > localAvaillable.x()) {
				if (m_gravity == ewol::gravityCenter) {
					origin2 -= vec2((minSize.x() - localAvaillable.x())/2.0f, 0);
				} else if (((int32_t)m_gravity & (int32_t)ewol::gravityRight) != 0) {
					origin2 -= vec2((minSize.x() - localAvaillable.x()), 0);
				}
			}
			if(    expand.y() == false
			    || minSize.y() > localAvaillable.y()) {
				if (m_gravity == ewol::gravityCenter) {
					origin2 -= vec2(0, (minSize.y() - localAvaillable.y())/2.0f);
				} else if (((int32_t)m_gravity & (int32_t)ewol::gravityTop) != 0) {
					origin2 -= vec2(0, (minSize.y() - localAvaillable.y()));
				}
			}
			m_subWidget[iii]->setOrigin(m_origin + origin);
			m_subWidget[iii]->calculateSize(subElementSize);
		}
	}
	vec2 selectableAreaPos = origin-vec2(_padding.xLeft(), _padding.yButtom());
	vec2 selectableAreaEndPos = m_size - (selectableAreaPos + subElementSize + vec2(_padding.x(), _padding.y()));
	markToRedraw();
	return ewol::Padding(selectableAreaPos.x(), selectableAreaEndPos.y(), selectableAreaEndPos.x(), selectableAreaPos.y());
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
std::shared_ptr<ewol::Widget> ewol::widget::Container2::getWidgetAtPos(const vec2& _pos) {
	if (isHide() == false) {
		if (m_subWidget[m_idWidgetDisplayed] != nullptr) {
			return m_subWidget[m_idWidgetDisplayed]->getWidgetAtPos(_pos);
		}
	}
	return nullptr;
}
*/

bool ewol::widget::Container2::loadXML(exml::Element* _node) {
	if (nullptr == _node) {
		return false;
	}
	// parse generic properties :
	ewol::Widget::loadXML(_node);
	// remove previous element :
	subWidgetRemove();
	
	// parse all the elements :
	for(size_t iii=0; iii< _node->size(); iii++) {
		exml::Element* pNode = _node->getElement(iii);
		if (pNode == nullptr) {
			// trash here all that is not element
			continue;
		}
		std::string widgetName = pNode->getValue();
		if (getWidgetManager().exist(widgetName) == false) {
			EWOL_ERROR("(l "<<pNode->getPos()<<") Unknown basic node=\"" << widgetName << "\" not in : [" << getWidgetManager().list() << "]" );
			continue;
		}
		bool toogleMode=false;
		if (nullptr != getSubWidget()) {
			toogleMode=true;
			if (nullptr != getSubWidgetToggle()) {
				EWOL_ERROR("(l "<<pNode->getPos()<<") " << __class__ << " Can only have one subWidget ??? node=\"" << widgetName << "\"" );
				continue;
			}
		}
		EWOL_DEBUG("try to create subwidget : '" << widgetName << "'");
		std::shared_ptr<ewol::Widget> tmpWidget = getWidgetManager().create(widgetName);
		if (tmpWidget == nullptr) {
			EWOL_ERROR ("(l "<<pNode->getPos()<<") Can not create the widget : \"" << widgetName << "\"");
			continue;
		}
		// add widget :
		if (toogleMode == false) {
			setSubWidget(tmpWidget);
		} else {
			setSubWidgetToggle(tmpWidget);
		}
		if (false == tmpWidget->loadXML(pNode)) {
			EWOL_ERROR ("(l "<<pNode->getPos()<<") can not load widget properties : \"" << widgetName << "\"");
			return false;
		}
	}
	return true;
}

void ewol::widget::Container2::setOffset(const vec2& _newVal) {
	if (m_offset != _newVal) {
		ewol::Widget::setOffset(_newVal);
		// recalculate the new sise and position of sub widget ...
		calculateSize(m_size);
	}
}

void ewol::widget::Container2::requestDestroyFromChild(const std::shared_ptr<Object>& _child) {
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


