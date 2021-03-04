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

Widget ewol::widget::Container::getSubWidget() {
	return this.subWidget;
}

void ewol::widget::Container::setSubWidget(Widget _newWidget) {
	if (_newWidget == null) {
		return;
	}
	subWidgetRemove();
	this.subWidget = _newWidget;
	if (this.subWidget != null) {
		this.subWidget.setParent(sharedFromThis());
	}
	markToRedraw();
	requestUpdateSize();
}

void ewol::widget::Container::subWidgetReplace( Widget _oldWidget,
                                                Widget _newWidget) {
	if (this.subWidget != _oldWidget) {
		Log.warning("Request replace with a wrong old widget");
		return;
	}
	this.subWidget.removeParent();
	this.subWidget.reset();
	this.subWidget = _newWidget;
	if (this.subWidget != null) {
		this.subWidget.setParent(sharedFromThis());
	}
	markToRedraw();
	requestUpdateSize();
}

void ewol::widget::Container::subWidgetRemove() {
	if (this.subWidget != null) {
		this.subWidget.removeParent();
		this.subWidget.reset();
		markToRedraw();
		requestUpdateSize();
	}
}

void ewol::widget::Container::subWidgetUnLink() {
	if (this.subWidget != null) {
		this.subWidget.removeParent();
	}
	this.subWidget.reset();
}

EwolObject ewol::widget::Container::getSubObjectNamed( String _objectName) {
	EwolObject tmpObject = Widget::getSubObjectNamed(_objectName);
	if (tmpObject != null) {
		return tmpObject;
	}
	if (this.subWidget != null) {
		return this.subWidget.getSubObjectNamed(_objectName);
	}
	return null;
}

void ewol::widget::Container::systemDraw( ewol::DrawProperty _displayProp) {
	if (propertyHide.get() == true){
		// widget is hidden ...
		return;
	}
	Widget::systemDraw(_displayProp);
	if (this.subWidget != null) {
		ewol::DrawProperty prop = _displayProp;
		prop.limit(this.origin, this.size);
		//Log.info("Draw : [" + propertyName + "] t=" + getObjectType() + " o=" + this.origin + "  s=" + this.size);
		this.subWidget.systemDraw(prop);
	} else {
		Log.info("[" + getId() + "]       ++++++ : [null]");
	}
}

void ewol::widget::Container::onChangeSize() {
	Widget::onChangeSize();
	if (*propertyHide == true) {
		return;
	}
	if (this.subWidget == null) {
		return;
	}
	Vector2f origin = this.origin+this.offset;
	Vector2f minSize = this.subWidget.getCalculateMinSize();
	Vector2b expand = this.subWidget.propertyExpand.get();
	origin += ewol::gravityGenerateDelta(propertyGravity.get(), minSize - this.size);
	this.subWidget.setOrigin(origin);
	this.subWidget.setSize(this.size);
	this.subWidget.onChangeSize();
}

void ewol::widget::Container::calculateMinMaxSize() {
	// call main class
	Widget::calculateMinMaxSize();
	// call sub classes
	if (this.subWidget != null) {
		this.subWidget.calculateMinMaxSize();
		Vector2f min = this.subWidget.getCalculateMinSize();
		this.minSize.setMax(min);
	}
	//Log.error("[" + getId() + "] Result min size : " +  this.minSize);
}

void ewol::widget::Container::onRegenerateDisplay() {
	if (this.subWidget != null) {
		this.subWidget.onRegenerateDisplay();
	}
}

Widget ewol::widget::Container::getWidgetAtPos( Vector2f _pos) {
	if (propertyHide.get() == false) {
		if (this.subWidget != null) {
			return this.subWidget.getWidgetAtPos(_pos);
		}
	}
	return null;
};

boolean ewol::widget::Container::loadXML( exml::Element _node) {
	if (_node.exist() == false) {
		return false;
	}
	// parse generic properties:
	Widget::loadXML(_node);
	// remove previous element:
	subWidgetRemove();
	// parse all the elements:
	for ( auto it : _node.nodes) {
		exml::Element pNode = it.toElement();
		if (pNode.exist() == false) {
			// trash here all that is not element
			continue;
		}
		String widgetName = pNode.getValue();
		Log.verbose("[" + getId() + "] t=" + getObjectType() + " Load node name : '" + widgetName + "'");
		if (getWidgetManager().exist(widgetName) == false) {
			Log.error("(l " + pNode.getPos() + ") Unknown basic node='" + widgetName + "' not in : [" + getWidgetManager().list() + "]" );
			continue;
		}
		if (getSubWidget() != null) {
			Log.error("(l " + pNode.getPos() + ") Can only have one subWidget ??? node='" + widgetName + "'" );
			continue;
		}
		Log.debug("try to create subwidget : '" + widgetName + "'");
		Widget tmpWidget = getWidgetManager().create(widgetName, pNode);
		if (tmpWidget == null) {
			EWOL_ERROR ("(l " + pNode.getPos() + ") Can not create the widget : '" + widgetName + "'");
			continue;
		}
		// add widget :
		setSubWidget(tmpWidget);
		if (tmpWidget.loadXML(pNode) == false) {
			EWOL_ERROR ("(l " + pNode.getPos() + ") can not load widget properties : '" + widgetName + "'");
			return false;
		}
	}
	if (    _node.nodes.size() != 0
	     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.subWidget == null) {
		Log.warning("Load container with no data inside");
	}
	return true;
}

void ewol::widget::Container::setOffset( Vector2f _newVal) {
	if (this.offset != _newVal) {
		Widget::setOffset(_newVal);
		// recalculate the new sise and position of sub widget ...
		onChangeSize();
	}
}

void ewol::widget::Container::requestDestroyFromChild( EwolObject _child) {
	if (this.subWidget != _child) {
		return;
	}
	if (this.subWidget == null) {
		return;
	}
	this.subWidget.removeParent();
	this.subWidget.reset();
	markToRedraw();
}

void ewol::widget::Container::drawWidgetTree(int _level) {
	Widget::drawWidgetTree(_level);
	_level++;
	if (this.subWidget != null) {
		this.subWidget.drawWidgetTree(_level);
	}
}
