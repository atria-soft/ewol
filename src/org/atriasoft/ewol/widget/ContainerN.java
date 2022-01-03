package org.atriasoft.ewol.widget;

/** @file
* @author Edouard DUPIN
* @copyright 2011, Edouard DUPIN, all right reserved
* @license MPL v2.0 (see license file)
*/

import java.util.ArrayList;
import java.util.List;
import java.util.ListIterator;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.annotation.EwolDescription;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.object.EwolObject;
import org.atriasoft.exml.annotation.XmlAttribute;
import org.atriasoft.exml.annotation.XmlManaged;
import org.atriasoft.exml.annotation.XmlName;
import org.atriasoft.exml.model.XmlElement;
import org.atriasoft.exml.model.XmlNode;

/**
 * @ingroup ewolWidgetGroup
 * the Cotainer widget is a widget that have an only one subWidget
 */
public class ContainerN extends Widget {
	
	protected Vector2b propertyLockExpand = new Vector2b(false, false); //!< Lock the expend of the sub widget to this one  == > this permit to limit bigger subWidget
	protected Vector2b subExpend = new Vector2b(false, false); //!< reference of the sub element expention requested.
	protected List<Widget> subWidget = new ArrayList<>();
	
	/**
	 * Constructor
	 */
	protected ContainerN() {}
	
	@Override
	public void calculateMinMaxSize() {
		this.subExpend = new Vector2b(false, false);
		this.minSize = Vector2f.ZERO;
		this.maxSize = Vector2f.MAX_VALUE;
		//Log.error("[" + getId() + "] {" + getObjectType() + "} set min size : " +  this.minSize);
		for (Widget it : this.subWidget) {
			if (it != null) {
				it.calculateMinMaxSize();
				Vector2b subExpendProp = it.canExpand();
				if (subExpendProp.x()) {
					this.subExpend = this.subExpend.withX(true);
				}
				if (subExpendProp.y()) {
					this.subExpend = this.subExpend.withX(true);
				}
				Vector2f tmpSize = it.getCalculateMinSize();
				this.minSize = Vector2f.max(tmpSize, this.minSize);
			}
		}
		//Log.error("[" + getId() + "] {" + getObjectType() + "} Result min size : " +  this.minSize);
	}
	
	// herited function
	@Override
	public Vector2b canExpand() {
		Vector2b res = this.propertyExpand;
		if (!this.propertyLockExpand.x()) {
			if (this.subExpend.x()) {
				res = res.withX(true);
			}
		}
		if (!this.propertyLockExpand.y()) {
			if (this.subExpend.y()) {
				res = res.withY(true);
			}
		}
		//Log.debug("Expend check : user=" + this.userExpand + " lock=" + propertyLockExpand + " sub=" + this.subExpend + " res=" + res);
		return res;
	}
	
	@Override
	public void drawWidgetTree(int level) {
		super.drawWidgetTree(level);
		level++;
		for (Widget it : this.subWidget) {
			if (it != null) {
				it.drawWidgetTree(level);
			}
		}
	}
	
	@XmlManaged
	@XmlAttribute
	@XmlName(value = "lock")
	@EwolDescription(value = "Lock the subwidget expand")
	public Vector2b getPropertyLockExpand() {
		return this.propertyLockExpand;
	}
	
	@Override
	public EwolObject getSubObjectNamed(final String objectName) {
		EwolObject tmpObject = super.getSubObjectNamed(objectName);
		if (tmpObject != null) {
			return tmpObject;
		}
		for (Widget it : this.subWidget) {
			if (it != null) {
				tmpObject = it.getSubObjectNamed(objectName);
				if (tmpObject != null) {
					return tmpObject;
				}
			}
		}
		return null;
	}
	
	@Override
	public Widget getWidgetAtPos(final Vector2f pos) {
		if (this.propertyHide) {
			return null;
		}
		// for all element in the sizer ...
		for (Widget it : this.subWidget) {
			if (it != null) {
				Vector2f tmpSize = it.getSize();
				Vector2f tmpOrigin = it.getOrigin();
				if ((tmpOrigin.x() <= pos.x() && tmpOrigin.x() + tmpSize.x() >= pos.x()) && (tmpOrigin.y() <= pos.y() && tmpOrigin.y() + tmpSize.y() >= pos.y())) {
					Widget tmpWidget = it.getWidgetAtPos(pos);
					if (tmpWidget != null) {
						return tmpWidget;
					}
					// stop searching
					break;
				}
			}
		}
		return null;
	}
	
	@Override
	public boolean loadXML(final XmlElement node) {
		if (node == null) {
			return false;
		}
		// parse generic properties :
		super.loadXML(node);
		// remove previous element :
		subWidgetRemoveAll();
		
		String tmpAttributeValue = node.getAttribute("lock", "");
		if (tmpAttributeValue.length() != 0) {
			setPropertyLockExpand(Vector2b.valueOf(tmpAttributeValue));
		}
		boolean invertAdding = false;
		tmpAttributeValue = node.getAttribute("addmode", "").toLowerCase();
		if (tmpAttributeValue.contentEquals("invert")) {
			invertAdding = true;
		}
		// parse all the elements :
		for (XmlNode nodeIt : node.getNodes()) {
			if (!nodeIt.isElement()) {
				// trash here all that is not element
				continue;
			}
			XmlElement pNode = nodeIt.toElement();
			String widgetName = pNode.getValue();
			Log.verbose(" t=" + this.getClass().getCanonicalName() + " Load node name : '" + widgetName + "'");
			if (!getWidgetManager().exist(widgetName)) {
				Log.error("[" + getId() + "] {" + this.getClass().getCanonicalName() + "} Unknown basic node='" + widgetName + "' not in : [" + getWidgetManager().list() + "]");
				continue;
			}
			Log.debug("[" + getId() + "] {" + this.getClass().getCanonicalName() + "} load new element : '" + widgetName + "'");
			Widget subWidget = getWidgetManager().create(widgetName, pNode);
			if (subWidget == null) {
				Log.error("[" + getId() + "] {" + this.getClass().getCanonicalName() + "} Can not create the widget : '" + widgetName + "'");
				continue;
			}
			// add sub element :
			if (!invertAdding) {
				subWidgetAdd(subWidget);
			} else {
				subWidgetAddStart(subWidget);
			}
			if (!subWidget.loadXML(pNode)) {
				Log.error("[" + getId() + "] {" + this.getClass().getCanonicalName() + "} can not load widget properties : '" + widgetName + "'");
				return false;
			}
		}
		return true;
	}
	
	@Override
	public void onChangeSize() {
		for (Widget it : this.subWidget) {
			if (it == null) {
				continue;
			}
			it.setOrigin(this.origin.add(this.offset));
			it.setSize(this.size);
			it.onChangeSize();
		}
	}
	
	@Override
	public void onRegenerateDisplay() {
		for (Widget it : this.subWidget) {
			if (it != null) {
				it.systemRegenerateDisplay();
			}
		}
	}
	
	@Override
	public void requestDestroyFromChild(final EwolObject child) {
		ListIterator<Widget> it = this.subWidget.listIterator();
		while (it.hasNext()) {
			Widget elem = it.next();
			if (elem != child) {
				continue;
			}
			if (elem == null) {
				it.remove();
				continue;
			}
			elem.removeParent();
			it.remove();
			it = this.subWidget.listIterator();
			markToRedraw();
		}
	}
	
	@Override
	public void setOffset(final Vector2f newVal) {
		if (this.offset != newVal) {
			super.setOffset(newVal);
			// recalculate the new sise and position of sub widget ...
			onChangeSize();
		}
	}
	
	public void setPropertyLockExpand(final Vector2b propertyLockExpand) {
		if (propertyLockExpand.equals(this.propertyLockExpand)) {
			return;
		}
		this.propertyLockExpand = propertyLockExpand;
		markToRedraw();
		requestUpdateSize();
	}
	
	/**
	 * add at end position a Widget (note : This system use an inverted phylisophie (button to top, and left to right)
	 * @param newWidget the element pointer
	 * @return the ID of the set element
	 */
	public int subWidgetAdd(final Widget newWidget) {
		if (newWidget == null) {
			Log.error("[" + getId() + "] {" + getClass().getCanonicalName() + "} Try to add An empty Widget ... ");
			return -1;
		}
		newWidget.setParent(this);
		this.subWidget.add(newWidget);
		markToRedraw();
		requestUpdateSize();
		// added at the last eelement :
		return newWidget.getId();
	}
	
	//! @previous
	public int subWidgetAddBack(final Widget newWidget) {
		return subWidgetAdd(newWidget);
	}
	
	//! @previous
	public int subWidgetAddEnd(final Widget newWidget) {
		return subWidgetAdd(newWidget);
	}
	
	//! @previous
	public int subWidgetAddFront(final Widget newWidget) {
		return subWidgetAddStart(newWidget);
	}
	
	/**
	 * add at start position a Widget (note : This system use an inverted phylisophie (button to top, and left to right)
	 * @param newWidget the element pointer
	 * @return the ID of the set element
	 */
	public int subWidgetAddStart(final Widget newWidget) {
		if (newWidget == null) {
			Log.error("[" + getId() + "] {" + getClass().getCanonicalName() + "} Try to add start An empty Widget ... ");
			return -1;
		}
		if (newWidget != null) {
			newWidget.setParent(this);
		}
		this.subWidget.add(0, newWidget);
		markToRedraw();
		requestUpdateSize();
		return newWidget.getId();
	}
	
	/**
	 * remove definitly a widget from the system and this layer.
	 * @param newWidget the element pointer.
	 */
	public void subWidgetRemove(final Widget newWidget) {
		if (newWidget == null) {
			return;
		}
		int errorControl = this.subWidget.size();
		
		ListIterator<Widget> it = this.subWidget.listIterator();
		while (it.hasNext()) {
			Widget elem = it.next();
			if (newWidget == elem) {
				elem.removeParent();
				it.remove();
				markToRedraw();
				requestUpdateSize();
			}
		}
	}
	
	/**
	 * remove all sub element from the widget.
	 */
	public void subWidgetRemoveAll() {
		for (Widget it : this.subWidget) {
			if (it != null) {
				it.removeParent();
			}
			it = null;
		}
		this.subWidget.clear();
	}
	
	/**
	 * remove all sub element from the widget (delayed to prevent remove in the callbback).
	 */
	public void subWidgetRemoveAllDelayed() {
		subWidgetRemoveAll();
	}
	
	/**
	 * Replace a old subwidget with a new one.
	 * @param oldWidget The widget to replace.
	 * @param newWidget The widget to set.
	 */
	public void subWidgetReplace(final Widget oldWidget, final Widget newWidget) {
		boolean haveChange = false;
		ListIterator<Widget> it = this.subWidget.listIterator();
		while (it.hasNext()) {
			Widget elem = it.next();
			if (elem != oldWidget) {
				continue;
			}
			elem.removeParent();
			if (newWidget != null) {
				newWidget.setParent(this);
			}
			it.set(newWidget);
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
	 * Just unlock the specify widget, this function does not remove it from the system (if you can, do nt use it ...)
	 * @param newWidget the element pointer.
	 */
	public void subWidgetUnLink(final Widget newWidget) {
		if (newWidget == null) {
			return;
		}
		ListIterator<Widget> it = this.subWidget.listIterator();
		while (it.hasNext()) {
			Widget elem = it.next();
			if (newWidget == elem) {
				elem.removeParent();
				it.remove();
				markToRedraw();
				requestUpdateSize();
			}
		}
	}
	
	@Override
	public void systemDraw(final DrawProperty displayProp) {
		if (this.propertyHide) {
			// widget is hidden ...
			return;
		}
		// local widget draw
		super.systemDraw(displayProp);
		// subwidget draw
		DrawProperty prop = displayProp;
		prop = prop.withLimit(this.origin, this.size);
		ListIterator<Widget> it = this.subWidget.listIterator(this.subWidget.size());
		while (it.hasPrevious()) {
			Widget elem = it.previous();
			if (elem != null) {
				//Log.info("       ***** : [" + (*it).propertyName + "] t=" + (*it).getObjectType() + " o=" + (*it).this.origin + "  s=" + (*it).this.size);
				elem.systemDraw(prop);
			}
		}
	}
}
