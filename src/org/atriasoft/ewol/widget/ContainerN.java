package org.atriasoft.ewol.widget; /** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.annotation.EwolDescription;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.exml.annotation.XmlManaged;
import org.atriasoft.exml.annotation.XmlName;
import org.atriasoft.exml.annotation.XmlProperty;

import java.util.ArrayList;
import java.util.List;

/**
 * @ingroup ewolWidgetGroup
 * @brief the Cotainer widget is a widget that have an only one subWidget
 */
public class ContainerN extends Widget {
	@XmlManaged()
	@XmlProperty()
	@XmlName(value = "lock")
	@EwolDescription(value = "Lock the subwidget expand")
	protected Vector2b propertyLockExpand = new Vector2b(false,false); //!< Lock the expend of the sub widget to this one  == > this permit to limit bigger subWidget
	protected List<Widget> subWidget = new ArrayList<>();
	protected Vector2b subExpend = new Vector2b(false,false); //!< reference of the sub element expention requested.
	/**
	 * @brief Constructor
	 */
	protected 	ContainerN() {}
	// herited function
	public Vector2b canExpand() {
		Vector2b res = propertyExpand;
		if (!propertyLockExpand.x()) {
			if (this.subExpend.x()) {
				res = res.withX(true);
			}
		}
		if (!propertyLockExpand.y()) {
			if (this.subExpend.y()) {
				res = res.withY(true);
			}
		}
		//Log.debug("Expend check : user=" + this.userExpand + " lock=" + propertyLockExpand + " sub=" + this.subExpend + " res=" + res);
		return res;
	}
	/**
	 * @brief remove all sub element from the widget.
	 */
	public void subWidgetRemoveAll(){
		for(Widget it : this.subWidget) {
			if (it != null) {
				it.removeParent();
			}
			it = null;
		}
		this.subWidget.clear();
	}
		/**
		 * @brief remove all sub element from the widget (delayed to prevent remove in the callbback).
		 */
		public  void subWidgetRemoveAllDelayed(){
			subWidgetRemoveAll();
		}
		/**
		 * @brief Replace a old subwidget with a new one.
		 * @param[in] _oldWidget The widget to replace.
		 * @param[in] _newWidget The widget to set.
		 */
		public 	 void subWidgetReplace(Widget _oldWidget,
									  Widget _newWidget) {
			boolean haveChange = false;
			for (Widget it : this.subWidget) {
				if (it != _oldWidget) {
					continue;
				}
				it.removeParent();
				it.reset();
				if (_newWidget != null) {
					_newWidget.setParent(this);
				}
				it = _newWidget;
				haveChange = true;
			}
			if (!haveChange) {
				Log.warning("Request replace with a wrong old widget");
				return;
			}
			markToRedraw();
			requestUpdateSize();
		}
		/**
		 * @brief add at end position a Widget (note : This system use an inverted phylisophie (button to top, and left to right)
		 * @param[in] _newWidget the element pointer
		 * @return the ID of the set element
		 */
		public int subWidgetAdd(Widget _newWidget) {
			if (_newWidget == null) {
				Log.error("[" + getId() + "] {" + getClass().getCanonicalName() + "} Try to add An empty Widget ... ");
				return -1;
			}
			_newWidget.setParent(this);
			this.subWidget.add(_newWidget);
			markToRedraw();
			requestUpdateSize();
			// added at the last eelement :
			return _newWidget.getId();
		}
	
	//! @previous
		public int subWidgetAddBack(Widget _newWidget) {
			return subWidgetAdd(_newWidget);
		};
		//! @previous
		public int subWidgetAddEnd(Widget _newWidget) {
			return subWidgetAdd(_newWidget);
		};
		/**
		 * @brief add at start position a Widget (note : This system use an inverted phylisophie (button to top, and left to right)
		 * @param[in] _newWidget the element pointer
		 * @return the ID of the set element
		 */
		public int subWidgetAddStart(Widget _newWidget) {
			if (_newWidget == null) {
				Log.error("[" + getId() + "] {" + getClass().getCanonicalName() + "} Try to add start An empty Widget ... ");
				return -1;
			}
			if (_newWidget != null) {
				_newWidget.setParent(this);
			}
			this.subWidget.insert(this.subWidget.begin(), _newWidget);
			markToRedraw();
			requestUpdateSize();
			return _newWidget.getId();
		}
		//! @previous
		public int subWidgetAddFront(Widget _newWidget) {
			return subWidgetAddStart(_newWidget);
		};
		/**
		 * @brief remove definitly a widget from the system and this layer.
		 * @param[in] _newWidget the element pointer.
		 */
		public void subWidgetRemove(Widget _newWidget){
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
		/**
		 * @brief Just unlick the specify widget, this function does not remove it from the system (if you can, do nt use it ...)
		 * @param[in] _newWidget the element pointer.
		 */
		public void subWidgetUnLink(Widget _newWidget) {
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
	public void systemDraw( DrawProperty _displayProp) {
		if (propertyHide){
			// widget is hidden ...
			return;
		}
		// local widget draw
		super.systemDraw(_displayProp);
		// subwidget draw
		DrawProperty prop = _displayProp;
		prop = prop.withLimit(this.origin, this.size);
		for (long iii = this.subWidget.size()-1; iii>=0; --iii) {
			if (this.subWidget[iii] != null) {
				//Log.info("       ***** : [" + (*it).propertyName + "] t=" + (*it).getObjectType() + " o=" + (*it).this.origin + "  s=" + (*it).this.size);
				this.subWidget[iii].systemDraw(prop);
			}
		}
	}
	public 	void onRegenerateDisplay(){
		for (Widget it : this.subWidget) {
			if (it != null) {
				it.onRegenerateDisplay();
			}
		}
	}
	public void onChangeSize(){
		for (Widget it : this.subWidget) {
			if (it == null) {
				continue;
			}
			it.setOrigin(this.origin+this.offset);
			it.setSize(this.size);
			it.onChangeSize();
		}
	}
	
	public void calculateMinMaxSize() {
		this.subExpend.setValue(false, false);
		this.minSize.setValue(0,0);
		this.maxSize.setValue(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE);
		//Log.error("[" + getId() + "] {" + getObjectType() + "} set min size : " +  this.minSize);
		for (Widget it : this.subWidget) {
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
	public Widget getWidgetAtPos( Vector2f _pos) {
		if (*propertyHide == true) {
			return null;
		}
		// for all element in the sizer ...
		for (Widget it : this.subWidget) {
			if (it != null) {
				Vector2f tmpSize = it.getSize();
				Vector2f tmpOrigin = it.getOrigin();
				if(    (tmpOrigin.x() <= _pos.x() && tmpOrigin.x() + tmpSize.x() >= _pos.x())
						&& (tmpOrigin.y() <= _pos.y() && tmpOrigin.y() + tmpSize.y() >= _pos.y()) )
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
	
	public EwolObject getSubObjectNamed( String _objectName) {
		EwolObject tmpObject = Widget::getSubObjectNamed(_objectName);
		if (tmpObject != null) {
			return tmpObject;
		}
		for (Widget it : this.subWidget) {
			if (it != null) {
				tmpObject = it.getSubObjectNamed(_objectName);
				if (tmpObject != null) {
					return tmpObject;
				}
			}
		}
		return null;
	}
	public boolean loadXML( exml::Element _node) {
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
	public void setOffset( Vector2f _newVal){
		if (this.offset != _newVal) {
			Widget::setOffset(_newVal);
			// recalculate the new sise and position of sub widget ...
			onChangeSize();
		}
	}
	public void requestDestroyFromChild( EwolObject _child) {
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
	public void drawWidgetTree(int _level) {
		super.drawWidgetTree(_level);
		_level++;
		for (Widget it: this.subWidget) {
			if (it != null) {
				it.drawWidgetTree(_level);
			}
		}
	}
	
	protected void onChangePropertyLockExpand() {
		markToRedraw();
		requestUpdateSize();
	}
}
