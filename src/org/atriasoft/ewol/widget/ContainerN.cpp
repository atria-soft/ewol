/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */


#include <ewol/ewol.hpp>
#include <ewol/widget/ContainerN.hpp>
#include <ewol/widget/Manager.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::ContainerN);

ewol::widget::ContainerN::ContainerN() :
  propertyLockExpand(this, "lock",
                           Vector2f(false,false),
                           "Lock the subwidget expand",
                           ewol::widget::ContainerN::onChangePropertyLockExpand),
  this.subExpend(false,false) {
	addObjectType("ewol::widget::ContainerN");
	// nothing to do ...
}

ewol::widget::ContainerN::~ContainerN() {
	subWidgetRemoveAll();
}


Vector2b ewol::widget::ContainerN::canExpand() {
	Vector2b res = propertyExpand.get();
	if (propertyLockExpand.x() == false) {
		if (this.subExpend.x() == true) {
			res.setX(true);
		}
	}
	if (propertyLockExpand.y() == false) {
		if (this.subExpend.y() == true) {
			res.setY(true);
		}
	}
	//Log.debug("Expend check : user=" + this.userExpand + " lock=" + propertyLockExpand + " sub=" + this.subExpend + " res=" + res);
	return res;
}

void ewol::widget::ContainerN::onChangePropertyLockExpand() {
	markToRedraw();
	requestUpdateSize();
}

void ewol::widget::ContainerN::subWidgetReplace(Widget _oldWidget,
                                                Widget _newWidget) {
	boolean haveChange = false;
	for (auto it : this.subWidget) {
		if (it != _oldWidget) {
			continue;
		}
		it.removeParent();
		it.reset();
		if (_newWidget != null) {
			_newWidget.setParent(sharedFromThis());
		}
		it = _newWidget;
		haveChange = true;
	}
	if (haveChange == false) {
		Log.warning("Request replace with a wrong old widget");
		return;
	}
	markToRedraw();
	requestUpdateSize();
}

int ewol::widget::ContainerN::subWidgetAdd(Widget _newWidget) {
	if (_newWidget == null) {
		Log.error("[" + getId() + "] {" + getObjectType() + "} Try to add An empty Widget ... ");
		return -1;
	}
	_newWidget.setParent(sharedFromThis());
	this.subWidget.pushBack(_newWidget);
	markToRedraw();
	requestUpdateSize();
	// added at the last eelement :
	return _newWidget.getId();
}

int ewol::widget::ContainerN::subWidgetAddStart(Widget _newWidget) {
	if (_newWidget == null) {
		Log.error("[" + getId() + "] {" + getObjectType() + "} Try to add start An empty Widget ... ");
		return -1;
	}
	if (_newWidget != null) {
		_newWidget.setParent(sharedFromThis());
	}
	this.subWidget.insert(this.subWidget.begin(), _newWidget);
	markToRedraw();
	requestUpdateSize();
	return _newWidget.getId();
}

void ewol::widget::ContainerN::subWidgetRemove(Widget _newWidget) {
	if (_newWidget == null) {
		return;
	}
	int errorControl = this.subWidget.size();
	
	auto it(this.subWidget.begin());
	while (it != this.subWidget.end()) {
		if (_newWidget == *it) {
			(*it).removeParent();
			this.subWidget.erase(it);
			it = this.subWidget.begin();
			markToRedraw();
			requestUpdateSize();
		} else {
			++it;
		}
	}
}

void ewol::widget::ContainerN::subWidgetUnLink(Widget _newWidget) {
	if (_newWidget == null) {
		return;
	}
	auto it(this.subWidget.begin());
	while (it != this.subWidget.end()) {
		if (_newWidget == *it) {
			(*it).removeParent();
			(*it).reset();
			this.subWidget.erase(it);
			it = this.subWidget.begin();
			markToRedraw();
			requestUpdateSize();
		} else {
			++it;
		}
	}
}

void ewol::widget::ContainerN::subWidgetRemoveAll() {
	for(auto it : this.subWidget) {
		if (it != null) {
			it.removeParent();
		}
		it.reset();
	}
	this.subWidget.clear();
}

void ewol::widget::ContainerN::subWidgetRemoveAllDelayed() {
	subWidgetRemoveAll();
}

EwolObject ewol::widget::ContainerN::getSubObjectNamed( String _objectName) {
	EwolObject tmpObject = Widget::getSubObjectNamed(_objectName);
	if (tmpObject != null) {
		return tmpObject;
	}
	for (auto it : this.subWidget) {
		if (it != null) {
			tmpObject = it.getSubObjectNamed(_objectName);
			if (tmpObject != null) {
				return tmpObject;
			}
		}
	}
	return null;
}

void ewol::widget::ContainerN::systemDraw( ewol::DrawProperty _displayProp) {
	if (*propertyHide == true){
		// widget is hidden ...
		return;
	}
	// local widget draw
	Widget::systemDraw(_displayProp);
	// subwidget draw
	ewol::DrawProperty prop = _displayProp;
	prop.limit(this.origin, this.size);
	for (long iii = this.subWidget.size()-1; iii>=0; --iii) {
		if (this.subWidget[iii] != null) {
			//Log.info("       ***** : [" + (*it).propertyName + "] t=" + (*it).getObjectType() + " o=" + (*it).this.origin + "  s=" + (*it).this.size);
			this.subWidget[iii].systemDraw(prop);
		}
	}
}

void ewol::widget::ContainerN::onChangeSize() {
	for (auto it : this.subWidget) {
		if (it == null) {
			continue;
		}
		it.setOrigin(this.origin+this.offset);
		it.setSize(this.size);
		it.onChangeSize();
	}
}

void ewol::widget::ContainerN::calculateMinMaxSize() {
	this.subExpend.setValue(false, false);
	this.minSize.setValue(0,0);
	this.maxSize.setValue(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE);
	//Log.error("[" + getId() + "] {" + getObjectType() + "} set min size : " +  this.minSize);
	for (auto it : this.subWidget) {
		if (it != null) {
			it.calculateMinMaxSize();
			Vector2b subExpendProp = it.canExpand();
			if (true == subExpendProp.x()) {
				this.subExpend.setX(true);
			}
			if (true == subExpendProp.y()) {
				this.subExpend.setY(true);
			}
			Vector2f tmpSize = it.getCalculateMinSize();
			this.minSize.setValue( etk::max(tmpSize.x(), this.minSize.x()),
			                    etk::max(tmpSize.y(), this.minSize.y()) );
		}
	}
	//Log.error("[" + getId() + "] {" + getObjectType() + "} Result min size : " +  this.minSize);
}

void ewol::widget::ContainerN::onRegenerateDisplay() {
	for (auto it : this.subWidget) {
		if (it != null) {
			it.onRegenerateDisplay();
		}
	}
}

Widget ewol::widget::ContainerN::getWidgetAtPos( Vector2f _pos) {
	if (*propertyHide == true) {
		return null;
	}
	// for all element in the sizer ...
	for (auto it : this.subWidget) {
		if (it != null) {
			Vector2f tmpSize = it.getSize();
			Vector2f tmpOrigin = it.getOrigin();
			if(    (tmpOrigin.x() <= _pos.x() LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM tmpOrigin.x() + tmpSize.x() >= _pos.x())
			    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (tmpOrigin.y() <= _pos.y() LOMLOMLOMLOMLOM tmpOrigin.y() + tmpSize.y() >= _pos.y()) )
			{
				Widget tmpWidget = it.getWidgetAtPos(_pos);
				if (tmpWidget != null) {
					return tmpWidget;
				}
				// stop searching
				break;
			}
		}
	}
	return null;
};

boolean ewol::widget::ContainerN::loadXML( exml::Element _node) {
	if (_node.exist() == false) {
		return false;
	}
	// parse generic properties :
	Widget::loadXML(_node);
	// remove previous element :
	subWidgetRemoveAll();
	
	String tmpAttributeValue = _node.attributes["lock"];
	if (tmpAttributeValue.size()!=0) {
		propertyLockExpand.set(tmpAttributeValue);
	}
	boolean invertAdding=false;
	tmpAttributeValue = _node.attributes["addmode"];
	if(etk::compare_no_case(tmpAttributeValue, "invert")) {
		invertAdding=true;
	}
	// parse all the elements :
	for ( auto nodeIt : _node.nodes) {
		 exml::Element pNode = nodeIt.toElement();
		if (pNode.exist() == false) {
			// trash here all that is not element
			continue;
		}
		String widgetName = pNode.getValue();
		Log.verbose(" t=" + getObjectType() + " Load node name : '" + widgetName + "'");
		if (getWidgetManager().exist(widgetName) == false) {
			Log.error("[" + getId() + "] {" + getObjectType() + "} (l " + pNode.getPos() + ") Unknown basic node='" + widgetName + "' not in : [" << getWidgetManager().list() << "]" );
			continue;
		}
		Log.debug("[" + getId() + "] {" + getObjectType() + "} load new element : '" + widgetName + "'");
		Widget subWidget = getWidgetManager().create(widgetName, pNode);
		if (subWidget == null) {
			EWOL_ERROR ("[" + getId() + "] {" + getObjectType() + "} (l " + pNode.getPos() + ") Can not create the widget : '" + widgetName + "'");
			continue;
		}
		// add sub element : 
		if (invertAdding == false) {
			subWidgetAdd(subWidget);
		} else {
			subWidgetAddStart(subWidget);
		}
		if (subWidget.loadXML(pNode) == false) {
			Log.error("[" + getId() + "] {" + getObjectType() + "} (l " + pNode.getPos() + ") can not load widget properties : '" + widgetName + "'");
			return false;
		}
	}
	return true;
}


void ewol::widget::ContainerN::setOffset( Vector2f _newVal) {
	if (this.offset != _newVal) {
		Widget::setOffset(_newVal);
		// recalculate the new sise and position of sub widget ...
		onChangeSize();
	}
}

void ewol::widget::ContainerN::requestDestroyFromChild( EwolObject _child) {
	auto it = this.subWidget.begin();
	while (it != this.subWidget.end()) {
		if (*it == _child) {
			if (*it == null) {
				this.subWidget.erase(it);
				it = this.subWidget.begin();
				continue;
			}
			(*it).removeParent();
			(*it).reset();
			this.subWidget.erase(it);
			it = this.subWidget.begin();
			markToRedraw();
			continue;
		}
		++it;
	}
}

void ewol::widget::ContainerN::drawWidgetTree(int _level) {
	Widget::drawWidgetTree(_level);
	_level++;
	for (auto it: this.subWidget) {
		if (it != null) {
			it.drawWidgetTree(_level);
		}
	}
}
