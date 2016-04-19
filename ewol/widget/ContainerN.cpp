/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */


#include <ewol/ewol.h>
#include <ewol/widget/ContainerN.h>
#include <ewol/widget/Manager.h>

ewol::widget::ContainerN::ContainerN() :
  propertyLockExpand(this, "lock",
                           vec2(false,false),
                           "Lock the subwidget expand",
                           &ewol::widget::ContainerN::onChangePropertyLockExpand),
  m_subExpend(false,false) {
	addObjectType("ewol::widget::ContainerN");
	// nothing to do ...
}

ewol::widget::ContainerN::~ContainerN() {
	subWidgetRemoveAll();
}


bvec2 ewol::widget::ContainerN::canExpand() {
	bvec2 res = propertyExpand.get();
	if (propertyLockExpand->x() == false) {
		if (m_subExpend.x() == true) {
			res.setX(true);
		}
	}
	if (propertyLockExpand->y() == false) {
		if (m_subExpend.y() == true) {
			res.setY(true);
		}
	}
	//EWOL_DEBUG("Expend check : user=" << m_userExpand << " lock=" << propertyLockExpand << " sub=" << m_subExpend << " res=" << res);
	return res;
}

void ewol::widget::ContainerN::onChangePropertyLockExpand() {
	markToRedraw();
	requestUpdateSize();
}

void ewol::widget::ContainerN::subWidgetReplace(const ewol::WidgetShared& _oldWidget,
                                                const ewol::WidgetShared& _newWidget) {
	bool haveChange = false;
	for (auto &it : m_subWidget) {
		if (it != _oldWidget) {
			continue;
		}
		it->removeParent();
		it.reset();
		if (_newWidget != nullptr) {
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

int32_t ewol::widget::ContainerN::subWidgetAdd(ewol::WidgetShared _newWidget) {
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

int32_t ewol::widget::ContainerN::subWidgetAddStart(ewol::WidgetShared _newWidget) {
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

void ewol::widget::ContainerN::subWidgetRemove(ewol::WidgetShared _newWidget) {
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

void ewol::widget::ContainerN::subWidgetUnLink(ewol::WidgetShared _newWidget) {
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

ewol::ObjectShared ewol::widget::ContainerN::getSubObjectNamed(const std::string& _objectName) {
	ewol::ObjectShared tmpObject = ewol::Widget::getSubObjectNamed(_objectName);
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
	if (*propertyHide == true){
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

void ewol::widget::ContainerN::onChangeSize() {
	for (auto &it : m_subWidget) {
		if (it == nullptr) {
			continue;
		}
		it->setOrigin(m_origin+m_offset);
		it->setSize(m_size);
		it->onChangeSize();
	}
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

ewol::WidgetShared ewol::widget::ContainerN::getWidgetAtPos(const vec2& _pos) {
	if (*propertyHide == true) {
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
				ewol::WidgetShared tmpWidget = it->getWidgetAtPos(_pos);
				if (tmpWidget != nullptr) {
					return tmpWidget;
				}
				// stop searching
				break;
			}
		}
	}
	return nullptr;
};

bool ewol::widget::ContainerN::loadXML(const exml::Element& _node) {
	if (_node.exist() == false) {
		return false;
	}
	// parse generic properties :
	ewol::Widget::loadXML(_node);
	// remove previous element :
	subWidgetRemoveAll();
	
	std::string tmpAttributeValue = _node.attributes["lock"];
	if (tmpAttributeValue.size()!=0) {
		propertyLockExpand.set(tmpAttributeValue);
	}
	bool invertAdding=false;
	tmpAttributeValue = _node.attributes["addmode"];
	if(etk::compare_no_case(tmpAttributeValue, "invert")) {
		invertAdding=true;
	}
	// parse all the elements :
	for (const auto nodeIt : _node.nodes) {
		const exml::Element pNode = nodeIt.toElement();
		if (pNode.exist() == false) {
			// trash here all that is not element
			continue;
		}
		std::string widgetName = pNode.getValue();
		if (getWidgetManager().exist(widgetName) == false) {
			EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} (l " << pNode.getPos() << ") Unknown basic node='" << widgetName << "' not in : [" << getWidgetManager().list() << "]" );
			continue;
		}
		EWOL_DEBUG("[" << getId() << "] {" << getObjectType() << "} load new element : '" << widgetName << "'");
		ewol::WidgetShared subWidget = getWidgetManager().create(widgetName);
		if (subWidget == nullptr) {
			EWOL_ERROR ("[" << getId() << "] {" << getObjectType() << "} (l " << pNode.getPos() << ") Can not create the widget : '" << widgetName << "'");
			continue;
		}
		// add sub element : 
		if (invertAdding == false) {
			subWidgetAdd(subWidget);
		} else {
			subWidgetAddStart(subWidget);
		}
		if (subWidget->loadXML(pNode) == false) {
			EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} (l " << pNode.getPos() << ") can not load widget properties : '" << widgetName << "'");
			return false;
		}
	}
	return true;
}


void ewol::widget::ContainerN::setOffset(const vec2& _newVal) {
	if (m_offset != _newVal) {
		ewol::Widget::setOffset(_newVal);
		// recalculate the new sise and position of sub widget ...
		onChangeSize();
	}
}

void ewol::widget::ContainerN::requestDestroyFromChild(const ewol::ObjectShared& _child) {
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

