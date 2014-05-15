/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ewol/ewol.h>
#include <ewol/widget/Container.h>
#include <ewol/widget/Manager.h>


#undef __class__
#define __class__	"Container"


ewol::widget::Container::Container(ewol::Widget* _subElement) :
  m_subWidget(_subElement) {
	addObjectType("ewol::widget::Container");
	// nothing to do ...
}

ewol::widget::Container::~Container() {
	subWidgetRemove();
}

ewol::Widget* ewol::widget::Container::getSubWidget() {
	return m_subWidget;
}

void ewol::widget::Container::setSubWidget(ewol::Widget* _newWidget) {
	if (NULL == _newWidget) {
		return;
	}
	subWidgetRemove();
	m_subWidget = _newWidget;
	if (m_subWidget!=NULL) {
		m_subWidget->setUpperWidget(this);
	}
	markToRedraw();
	requestUpdateSize();
}

void ewol::widget::Container::subWidgetRemove() {
	if (NULL != m_subWidget) {
		m_subWidget->removeUpperWidget();
		m_subWidget->removeObject();
		m_subWidget=NULL;
		markToRedraw();
		requestUpdateSize();
	}
}

void ewol::widget::Container::subWidgetUnLink() {
	if (NULL != m_subWidget) {
		m_subWidget->removeUpperWidget();
	}
	m_subWidget=NULL;
}

ewol::Widget* ewol::widget::Container::getWidgetNamed(const std::string& _widgetName) {
	ewol::Widget* tmpUpperWidget = ewol::Widget::getWidgetNamed(_widgetName);
	if (NULL!=tmpUpperWidget) {
		return tmpUpperWidget;
	}
	if (NULL != m_subWidget) {
		return m_subWidget->getWidgetNamed(_widgetName);
	}
	return NULL;
}

void ewol::widget::Container::onObjectRemove(ewol::Object* _removeObject) {
	if (m_subWidget == _removeObject) {
		m_subWidget=NULL;
		markToRedraw();
		requestUpdateSize();
	}
}

void ewol::widget::Container::systemDraw(const ewol::DrawProperty& _displayProp) {
	if (true == m_hide){
		// widget is hidden ...
		return;
	}
	ewol::Widget::systemDraw(_displayProp);
	if (NULL!=m_subWidget) {
		ewol::DrawProperty prop = _displayProp;
		prop.limit(m_origin, m_size);
		m_subWidget->systemDraw(prop);
	}
}

void ewol::widget::Container::calculateSize(const vec2& _availlable) {
	if (NULL!=m_subWidget) {
		vec2 origin = m_origin+m_offset;
		vec2 minSize = m_subWidget->getCalculateMinSize();
		bvec2 expand = m_subWidget->getExpand();
		if (    expand.x() == false
		    || minSize.x()>_availlable.x()) {
			if (m_gravity == ewol::gravityCenter) {
				origin -= vec2((minSize.x() - _availlable.x())/2.0f, 0);
			} else if (((int32_t)m_gravity & (int32_t)ewol::gravityRight) != 0) {
				origin -= vec2((minSize.x() - _availlable.x()), 0);
			}
		}
		if(    expand.y() == false
		    || minSize.y()>_availlable.y()) {
			if (m_gravity == ewol::gravityCenter) {
				origin -= vec2(0, (minSize.y() - _availlable.y())/2.0f);
			} else if (((int32_t)m_gravity & (int32_t)ewol::gravityTop) != 0) {
				origin -= vec2(0, (minSize.y() - _availlable.y()));
			}
		}
		m_subWidget->setOrigin(origin);
		m_subWidget->calculateSize(_availlable);
	}
	ewol::Widget::calculateSize(_availlable);
}

void ewol::widget::Container::calculateMinMaxSize() {
	// call main class
	ewol::Widget::calculateMinMaxSize();
	// call sub classes
	if (NULL!=m_subWidget) {
		m_subWidget->calculateMinMaxSize();
		vec2 min = m_subWidget->getCalculateMinSize();
		m_minSize.setMax(min);
	}
	//EWOL_ERROR("[" << getId() << "] Result min size : " <<  m_minSize);
}

void ewol::widget::Container::onRegenerateDisplay() {
	if (NULL!=m_subWidget) {
		m_subWidget->onRegenerateDisplay();
	}
}

ewol::Widget* ewol::widget::Container::getWidgetAtPos(const vec2& _pos) {
	if (false == isHide()) {
		if (NULL!=m_subWidget) {
			return m_subWidget->getWidgetAtPos(_pos);
		}
	}
	return NULL;
};


bool ewol::widget::Container::loadXML(exml::Element* _node) {
	if (NULL == _node) {
		return false;
	}
	// parse generic properties :
	ewol::Widget::loadXML(_node);
	// remove previous element :
	subWidgetRemove();
	
	// parse all the elements :
	for(size_t iii=0; iii< _node->size(); iii++) {
		exml::Element* pNode = _node->getElement(iii);
		if (pNode == NULL) {
			// trash here all that is not element
			continue;
		}
		std::string widgetName = pNode->getValue();
		if (getWidgetManager().exist(widgetName) == false) {
			EWOL_ERROR("(l "<<pNode->getPos()<<") Unknown basic node=\"" << widgetName << "\" not in : [" << getWidgetManager().list() << "]" );
			continue;
		}
		if (NULL != getSubWidget()) {
			EWOL_ERROR("(l "<<pNode->getPos()<<") " << __class__ << " Can only have one subWidget ??? node=\"" << widgetName << "\"" );
			continue;
		}
		EWOL_DEBUG("try to create subwidget : '" << widgetName << "'");
		ewol::Widget* tmpWidget = getWidgetManager().create(widgetName);
		if (tmpWidget == NULL) {
			EWOL_ERROR ("(l "<<pNode->getPos()<<") Can not create the widget : \"" << widgetName << "\"");
			continue;
		}
		// add widget :
		setSubWidget(tmpWidget);
		if (false == tmpWidget->loadXML(pNode)) {
			EWOL_ERROR ("(l "<<pNode->getPos()<<") can not load widget properties : \"" << widgetName << "\"");
			return false;
		}
	}
	return true;
}

void ewol::widget::Container::setOffset(const vec2& _newVal) {
	if (m_offset != _newVal) {
		ewol::Widget::setOffset(_newVal);
		// recalculate the new sise and position of sub widget ...
		calculateSize(m_size);
	}
}


