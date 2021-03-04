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
  this.idWidgetDisplayed(0) {
	addObjectType("ewol::widget::Container2");
}

ewol::widget::Container2::~Container2() {
	subWidgetRemove();
	subWidgetRemoveToggle();
}

void ewol::widget::Container2::setSubWidget(Widget _newWidget, int _idWidget) {
	subWidgetRemove(_idWidget);
	this.subWidget[_idWidget] = _newWidget;
	if (this.subWidget[_idWidget] != null) {
		Log.verbose("Add widget : " + _idWidget);
		this.subWidget[_idWidget].setParent(sharedFromThis());
	}
	markToRedraw();
	requestUpdateSize();
}

void ewol::widget::Container2::subWidgetReplace( Widget _oldWidget,
                                                 Widget _newWidget) {
	boolean haveChange = false;
	for (int iii=0; iii<2; ++iii) {
		if (this.subWidget[iii] != _oldWidget) {
			continue;
		}
		this.subWidget[iii].removeParent();
		this.subWidget[iii].reset();
		this.subWidget[iii] = _newWidget;
		if (this.subWidget[iii] != null) {
			this.subWidget[iii].setParent(sharedFromThis());
		}
		haveChange = true;
	}
	if (haveChange == false) {
		Log.warning("Request replace with a wrong old widget");
		return;
	}
	markToRedraw();
	requestUpdateSize();
}


void ewol::widget::Container2::subWidgetRemove(int _idWidget) {
	if (this.subWidget[_idWidget] != null) {
		Log.verbose("Remove widget : " + _idWidget);
		this.subWidget[_idWidget].removeParent();
		this.subWidget[_idWidget].reset();
		markToRedraw();
		requestUpdateSize();
	}
}

void ewol::widget::Container2::subWidgetUnLink(int _idWidget) {
	if (this.subWidget[_idWidget] != null) {
		this.subWidget[_idWidget].removeParent();
		Log.verbose("Unlink widget : " + _idWidget);
	}
	this.subWidget[_idWidget].reset();
}

EwolObject ewol::widget::Container2::getSubObjectNamed( String _widgetName) {
	EwolObject tmpObject = Widget::getSubObjectNamed(_widgetName);
	if (tmpObject != null) {
		return tmpObject;
	}
	if (this.subWidget[0] != null) {
		tmpObject = this.subWidget[0].getSubObjectNamed(_widgetName);
		if (tmpObject != null) {
			return tmpObject;
		}
	}
	if (this.subWidget[1] != null) {
		return this.subWidget[1].getSubObjectNamed(_widgetName);
	}
	return null;
}

void ewol::widget::Container2::systemDraw( ewol::DrawProperty _displayProp) {
	if (propertyHide.get() == true){
		// widget is hidden ...
		return;
	}
	Widget::systemDraw(_displayProp);
	if (this.subWidget[this.idWidgetDisplayed] != null) {
		//Log.info("Draw : [" + propertyName + "] t=" + getObjectType() + " o=" + this.origin + "  s=" + this.size);
		this.subWidget[this.idWidgetDisplayed].systemDraw(_displayProp);
	}
}

ewol::Padding ewol::widget::Container2::onChangeSizePadded( ewol::Padding _padding) {
	Widget::onChangeSize();
	Vector2f localAvaillable = this.size - Vector2f(_padding.x(), _padding.y());
	// Checkin the filling properties  == > for the subElements:
	Vector2f subElementSize = this.minSize;
	if (propertyFill.x() == true) {
		subElementSize.setX(this.size.x());
	}
	if (propertyFill.y() == true) {
		subElementSize.setY(this.size.y());
	}
	Vector2f delta = ewol::gravityGenerateDelta(propertyGravity, this.size - subElementSize);
	Vector2f origin = delta + Vector2f(_padding.xLeft(), _padding.yButtom());
	subElementSize -= Vector2f(_padding.x(), _padding.y());
	for (int iii = 0; iii < 2; ++iii) {
		if (this.subWidget[iii] != null) {
			Vector2f origin2 = origin+this.offset;
			Vector2f minSize = this.subWidget[iii].getCalculateMinSize();
			//Vector2b expand = this.subWidget[iii].propertyExpand.get();
			origin2 += ewol::gravityGenerateDelta(propertyGravity, minSize - localAvaillable);
			this.subWidget[iii].setOrigin(this.origin + origin);
			this.subWidget[iii].setSize(subElementSize);
			this.subWidget[iii].onChangeSize();
		}
	}
	Vector2f selectableAreaPos = origin-Vector2f(_padding.xLeft(), _padding.yButtom());
	Vector2f selectableAreaEndPos = this.size - (selectableAreaPos + subElementSize + Vector2f(_padding.x(), _padding.y()));
	markToRedraw();
	return ewol::Padding(selectableAreaPos.x(),
	                     selectableAreaEndPos.y(),
	                     selectableAreaEndPos.x(),
	                     selectableAreaPos.y());
}

void ewol::widget::Container2::calculateMinMaxSizePadded( ewol::Padding _padding) {
	// call main class
	this.minSize = Vector2f(0,0);
	// call sub classes
	for (int iii = 0; iii < 2; ++iii) {
		if (this.subWidget[iii] != null) {
			this.subWidget[iii].calculateMinMaxSize();
			Vector2f min = this.subWidget[iii].getCalculateMinSize();
			this.minSize.setMax(min);
		}
	}
	// add padding :
	this.minSize += Vector2f(_padding.x(), _padding.y());
	// verify the min max of the min size ...
	checkMinSize();
	markToRedraw();
}

void ewol::widget::Container2::onRegenerateDisplay() {
	if (this.subWidget[this.idWidgetDisplayed] != null) {
		this.subWidget[this.idWidgetDisplayed].onRegenerateDisplay();
	}
}
/*
Widget ewol::widget::Container2::getWidgetAtPos( Vector2f _pos) {
	if (isHide() == false) {
		if (this.subWidget[this.idWidgetDisplayed] != null) {
			return this.subWidget[this.idWidgetDisplayed].getWidgetAtPos(_pos);
		}
	}
	return null;
}
*/

boolean ewol::widget::Container2::loadXML( exml::Element _node) {
	if (_node.exist() == false) {
		return false;
	}
	// parse generic properties :
	Widget::loadXML(_node);
	// remove previous element :
	subWidgetRemove();
	Log.verbose("Create en element 2 ... with nodes.size()=" + _node.nodes.size());
	// parse all the elements:
	for( auto it : _node.nodes) {
		Log.verbose("    node: " + it);
		exml::Element pNode = it.toElement();
		if (pNode.exist() == false) {
			// trash here all that is not element
			continue;
		}
		String widgetName = pNode.getValue();
		if (getWidgetManager().exist(widgetName) == false) {
			Log.error("(l " + pNode.getPos() + ") Unknown basic node='" + widgetName + "' not in: [" + getWidgetManager().list() + "]" );
			continue;
		}
		boolean toogleMode=false;
		if (getSubWidget() != null) {
			toogleMode=true;
			if (getSubWidgetToggle() != null) {
				Log.error("(l " + pNode.getPos() + ") Can only have one subWidget ??? node='" + widgetName + "'" );
				continue;
			}
		}
		Log.debug("try to create subwidget : '" + widgetName + "'");
		Widget tmpWidget = getWidgetManager().create(widgetName, pNode);
		if (tmpWidget == null) {
			EWOL_ERROR ("(l " + pNode.getPos() + ") Can not create the widget: '" + widgetName + "'");
			continue;
		}
		// add widget :
		if (toogleMode == false) {
			setSubWidget(tmpWidget);
		} else {
			setSubWidgetToggle(tmpWidget);
		}
		if (tmpWidget.loadXML(pNode) == false) {
			EWOL_ERROR ("(l "+pNode.getPos()+") can not load widget properties: '" + widgetName + "'");
			return false;
		}
	}
	return true;
}

void ewol::widget::Container2::setOffset( Vector2f _newVal) {
	if (this.offset != _newVal) {
		Widget::setOffset(_newVal);
		// recalculate the new sise and position of sub widget ...
		calculateSize();
	}
}

void ewol::widget::Container2::requestDestroyFromChild( EwolObject _child) {
	if (this.subWidget[0] == _child) {
		if (this.subWidget[0] == null) {
			return;
		}
		this.subWidget[0].removeParent();
		this.subWidget[0].reset();
		markToRedraw();
	}
	if (this.subWidget[1] == _child) {
		if (this.subWidget[1] == null) {
			return;
		}
		this.subWidget[1].removeParent();
		this.subWidget[1].reset();
		markToRedraw();
	}
}

void ewol::widget::Container2::drawWidgetTree(int _level) {
	Widget::drawWidgetTree(_level);
	_level++;
	if (this.subWidget[0] != null) {
		this.subWidget[0].drawWidgetTree(_level);
	}
	if (this.subWidget[1] != null) {
		this.subWidget[1].drawWidgetTree(_level);
	}
}
