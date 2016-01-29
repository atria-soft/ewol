/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */


#include <ewol/ewol.h>
#include <ewol/widget/ContainerN.h>
#include <ewol/widget/Manager.h>

#undef __class__
#define __class__	"ContainerN"


ewol::widget::ContainerN::ContainerN() :
  m_lockExpand(false,false),
  m_subExpend(false,false) {
	addObjectType("ewol::widget::ContainerN");
	// nothing to do ...
}

void ewol::widget::ContainerN::init() {
	ewol::Widget::init();
}

ewol::widget::ContainerN::~ContainerN() {
	subWidgetRemoveAll();
}


bvec2 ewol::widget::ContainerN::canExpand() {
	bvec2 res = m_userExpand;
	if (false == m_lockExpand.x()) {
		if (true == m_subExpend.x()) {
			res.setX(true);
		}
	}
	if (false == m_lockExpand.y()) {
		if (true == m_subExpend.y()) {
			res.setY(true);
		}
	}
	//EWOL_DEBUG("Expend check : user=" << m_userExpand << " lock=" << m_lockExpand << " sub=" << m_subExpend << " res=" << res);
	return res;
}

void ewol::widget::ContainerN::lockExpand(const bvec2& _lockExpand) {
	if (_lockExpand != m_lockExpand) {
		m_lockExpand = _lockExpand;
		markToRedraw();
		requestUpdateSize();
	}
}


void ewol::widget::ContainerN::subWidgetReplace(const std::shared_ptr<ewol::Widget>& _oldWidget,
                                                const std::shared_ptr<ewol::Widget>& _newWidget) {
	bool haveChange = false;
	for (auto &it : m_subWidget) {
		if (it != _oldWidget) {
			continue;
		}
		EWOL_WARNING("Remove old");
		it->removeParent();
		it.reset();
		EWOL_WARNING("Set New");
		if (_newWidget != nullptr) {
			EWOL_WARNING("set parrent");
			_newWidget->setParent(shared_from_this());
		}
		it = _newWidget;
		haveChange = true;
	}
	if (haveChange == false) {
		EWOL_WARNING("Request replace with a wrong old widget");
		return;
	}
	markToRedraw();
	requestUpdateSize();
}

int32_t ewol::widget::ContainerN::subWidgetAdd(std::shared_ptr<ewol::Widget> _newWidget) {
	if (_newWidget == nullptr) {
		EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} Try to add An empty Widget ... ");
		return -1;
	}
	_newWidget->setParent(shared_from_this());
	m_subWidget.push_back(_newWidget);
	markToRedraw();
	requestUpdateSize();
	// added at the last eelement :
	return _newWidget->getId();
}

int32_t ewol::widget::ContainerN::subWidgetAddStart(std::shared_ptr<ewol::Widget> _newWidget) {
	if (nullptr == _newWidget) {
		EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} Try to add start An empty Widget ... ");
		return -1;
	}
	if (_newWidget != nullptr) {
		_newWidget->setParent(shared_from_this());
	}
	m_subWidget.insert(m_subWidget.begin(), _newWidget);
	markToRedraw();
	requestUpdateSize();
	return _newWidget->getId();
}

void ewol::widget::ContainerN::subWidgetRemove(std::shared_ptr<ewol::Widget> _newWidget) {
	if (nullptr == _newWidget) {
		return;
	}
	size_t errorControl = m_subWidget.size();
	
	auto it(m_subWidget.begin());
	while (it != m_subWidget.end()) {
		if (_newWidget == *it) {
			(*it)->removeParent();
			m_subWidget.erase(it);
			it = m_subWidget.begin();
			markToRedraw();
			requestUpdateSize();
		} else {
			++it;
		}
	}
}

void ewol::widget::ContainerN::subWidgetUnLink(std::shared_ptr<ewol::Widget> _newWidget) {
	if (nullptr == _newWidget) {
		return;
	}
	auto it(m_subWidget.begin());
	while (it != m_subWidget.end()) {
		if (_newWidget == *it) {
			(*it)->removeParent();
			(*it).reset();
			m_subWidget.erase(it);
			it = m_subWidget.begin();
			markToRedraw();
			requestUpdateSize();
		} else {
			++it;
		}
	}
}

void ewol::widget::ContainerN::subWidgetRemoveAll() {
	for(auto &it : m_subWidget) {
		if (it != nullptr) {
			it->removeParent();
		}
		it.reset();
	}
	m_subWidget.clear();
}

void ewol::widget::ContainerN::subWidgetRemoveAllDelayed() {
	subWidgetRemoveAll();
}

std::shared_ptr<ewol::Object> ewol::widget::ContainerN::getSubObjectNamed(const std::string& _objectName) {
	std::shared_ptr<ewol::Object> tmpObject = ewol::Widget::getSubObjectNamed(_objectName);
	if (tmpObject != nullptr) {
		return tmpObject;
	}
	for (auto &it : m_subWidget) {
		if (it != nullptr) {
			tmpObject = it->getSubObjectNamed(_objectName);
			if (tmpObject != nullptr) {
				return tmpObject;
			}
		}
	}
	return nullptr;
}

void ewol::widget::ContainerN::systemDraw(const ewol::DrawProperty& _displayProp) {
	if (true == m_hide){
		// widget is hidden ...
		return;
	}
	// local widget draw
	ewol::Widget::systemDraw(_displayProp);
	// subwidget draw
	ewol::DrawProperty prop = _displayProp;
	prop.limit(m_origin, m_size);
	for (auto it(m_subWidget.rbegin()); it!=m_subWidget.rend(); ++it) {
		if (*it != nullptr) {
			(*it)->systemDraw(prop);
		}
	}
}

void ewol::widget::ContainerN::calculateSize(const vec2& _availlable) {
	m_size = _availlable;
	for (auto &it : m_subWidget) {
		if (it != nullptr) {
			it->setOrigin(m_origin+m_offset);
			it->calculateSize(m_size);
		}
	}
	markToRedraw();
}

void ewol::widget::ContainerN::calculateMinMaxSize() {
	m_subExpend.setValue(false, false);
	m_minSize.setValue(0,0);
	m_maxSize.setValue(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE);
	//EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} set min size : " <<  m_minSize);
	for (auto &it : m_subWidget) {
		if (it != nullptr) {
			it->calculateMinMaxSize();
			bvec2 subExpendProp = it->canExpand();
			if (true == subExpendProp.x()) {
				m_subExpend.setX(true);
			}
			if (true == subExpendProp.y()) {
				m_subExpend.setY(true);
			}
			vec2 tmpSize = it->getCalculateMinSize();
			m_minSize.setValue( std::max(tmpSize.x(), m_minSize.x()),
			                    std::max(tmpSize.y(), m_minSize.y()) );
		}
	}
	//EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} Result min size : " <<  m_minSize);
}

void ewol::widget::ContainerN::onRegenerateDisplay() {
	for (auto &it : m_subWidget) {
		if (it != nullptr) {
			it->onRegenerateDisplay();
		}
	}
}

std::shared_ptr<ewol::Widget> ewol::widget::ContainerN::getWidgetAtPos(const vec2& _pos) {
	if (true == isHide()) {
		return nullptr;
	}
	// for all element in the sizer ...
	for (auto &it : m_subWidget) {
		if (it != nullptr) {
			vec2 tmpSize = it->getSize();
			vec2 tmpOrigin = it->getOrigin();
			if(    (tmpOrigin.x() <= _pos.x() && tmpOrigin.x() + tmpSize.x() >= _pos.x())
			    && (tmpOrigin.y() <= _pos.y() && tmpOrigin.y() + tmpSize.y() >= _pos.y()) )
			{
				std::shared_ptr<ewol::Widget> tmpWidget = it->getWidgetAtPos(_pos);
				if (nullptr != tmpWidget) {
					return tmpWidget;
				}
				// stop searching
				break;
			}
		}
	}
	return nullptr;
};


bool ewol::widget::ContainerN::loadXML(const std::shared_ptr<const exml::Element>& _node) {
	if (_node == nullptr) {
		return false;
	}
	// parse generic properties :
	ewol::Widget::loadXML(_node);
	// remove previous element :
	subWidgetRemoveAll();
	
	std::string tmpAttributeValue = _node->getAttribute("lock");
	if (tmpAttributeValue.size()!=0) {
		m_lockExpand = tmpAttributeValue;
	}
	bool invertAdding=false;
	tmpAttributeValue = _node->getAttribute("addmode");
	if(etk::compare_no_case(tmpAttributeValue, "invert")) {
		invertAdding=true;
	}
	// parse all the elements :
	for (size_t iii=0; iii < _node->size(); iii++) {
		std::shared_ptr<const exml::Element> pNode = _node->getElement(iii);
		if (pNode == nullptr) {
			// trash here all that is not element
			continue;
		}
		std::string widgetName = pNode->getValue();
		if (getWidgetManager().exist(widgetName) == false) {
			EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} (l "<<pNode->getPos()<<") Unknown basic node=\"" << widgetName << "\" not in : [" << getWidgetManager().list() << "]" );
			continue;
		}
		EWOL_DEBUG("[" << getId() << "] {" << getObjectType() << "} load new element : \"" << widgetName << "\"");
		std::shared_ptr<ewol::Widget> subWidget = getWidgetManager().create(widgetName);
		if (subWidget == nullptr) {
			EWOL_ERROR ("[" << getId() << "] {" << getObjectType() << "} (l "<<pNode->getPos()<<") Can not create the widget : \"" << widgetName << "\"");
			continue;
		}
		// add sub element : 
		if (invertAdding == false) {
			subWidgetAdd(subWidget);
		} else {
			subWidgetAddStart(subWidget);
		}
		if (subWidget->loadXML(pNode) == false) {
			EWOL_ERROR ("[" << getId() << "] {" << getObjectType() << "} (l "<<pNode->getPos()<<") can not load widget properties : \"" << widgetName << "\"");
			return false;
		}
	}
	return true;
}


void ewol::widget::ContainerN::setOffset(const vec2& _newVal) {
	if (m_offset != _newVal) {
		ewol::Widget::setOffset(_newVal);
		// recalculate the new sise and position of sub widget ...
		calculateSize(m_size);
	}
}

void ewol::widget::ContainerN::requestDestroyFromChild(const std::shared_ptr<Object>& _child) {
	auto it = m_subWidget.begin();
	while (it != m_subWidget.end()) {
		if (*it == _child) {
			if (*it == nullptr) {
				m_subWidget.erase(it);
				it = m_subWidget.begin();
				continue;
			}
			(*it)->removeParent();
			(*it).reset();
			m_subWidget.erase(it);
			it = m_subWidget.begin();
			markToRedraw();
			continue;
		}
		++it;
	}
}

