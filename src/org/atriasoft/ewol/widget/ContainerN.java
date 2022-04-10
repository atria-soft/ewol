package org.atriasoft.ewol.widget;

/** @file
* @author Edouard DUPIN
* @copyright 2011, Edouard DUPIN, all right reserved
* @license MPL v2.0 (see license file)
*/

import java.util.ArrayList;
import java.util.List;
import java.util.ListIterator;

import org.atriasoft.etk.math.Vector3b;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.annotation.EwolDescription;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.object.EwolObject;
import org.atriasoft.exml.annotation.XmlAttribute;
import org.atriasoft.exml.annotation.XmlFactory;
import org.atriasoft.exml.annotation.XmlManaged;
import org.atriasoft.exml.annotation.XmlName;

/**
 * @ingroup ewolWidgetGroup
 * the Cotainer widget is a widget that have an only one subWidget
 */
public class ContainerN extends Widget {
	
	protected Vector3b propertyLockExpand = Vector3b.FALSE; //!< Lock the expend of the sub widget to this one  == > this permit to limit bigger subWidget
	protected Vector3b subExpend = Vector3b.FALSE; //!< reference of the sub element expention requested.
	protected List<Widget> subWidget = new ArrayList<>();
	
	/**
	 * Constructor
	 */
	protected ContainerN() {}
	
	@Override
	public void calculateMinMaxSize() {
		this.subExpend = Vector3b.FALSE;
		this.minSize = Vector3f.ZERO;
		this.maxSize = Vector3f.MAX_VALUE;
		//Log.error("[" + getId() + "] {" + getObjectType() + "} set min size : " +  this.minSize);
		for (final Widget it : this.subWidget) {
			if (it != null) {
				it.calculateMinMaxSize();
				final Vector3b subExpendProp = it.canExpand();
				if (subExpendProp.x()) {
					this.subExpend = this.subExpend.withX(true);
				}
				if (subExpendProp.y()) {
					this.subExpend = this.subExpend.withX(true);
				}
				final Vector3f tmpSize = it.getCalculateMinSize();
				this.minSize = Vector3f.max(tmpSize, this.minSize);
			}
		}
		Log.warning("[{}] Result min size : {}", getId(), this.minSize);
	}
	
	// herited function
	@Override
	public Vector3b canExpand() {
		Vector3b res = this.propertyExpand;
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
		for (final Widget it : this.subWidget) {
			if (it != null) {
				it.drawWidgetTree(level);
			}
		}
	}
	
	@XmlManaged
	@XmlAttribute
	@XmlName(value = "lock")
	@EwolDescription(value = "Lock the subwidget expand")
	public Vector3b getPropertyLockExpand() {
		return this.propertyLockExpand;
	}
	
	@Override
	public EwolObject getSubObjectNamed(final String objectName) {
		EwolObject tmpObject = super.getSubObjectNamed(objectName);
		if (tmpObject != null) {
			return tmpObject;
		}
		for (final Widget it : this.subWidget) {
			if (it != null) {
				tmpObject = it.getSubObjectNamed(objectName);
				if (tmpObject != null) {
					return tmpObject;
				}
			}
		}
		return null;
	}
	
	@XmlManaged
	@XmlFactory(value = WidgetXmlFactory.class)
	@EwolDescription(value = "Request the widget Expand size while free space is detected (does not generate expand in upper widget)")
	public List<Widget> getSubWidgets() {
		return this.subWidget;
	}
	
	@Override
	public Widget getWidgetAtPos(final Vector3f pos) {
		if (this.propertyHide) {
			return null;
		}
		// for all element in the sizer ...
		for (final Widget it : this.subWidget) {
			if (it != null) {
				final Vector3f tmpSize = it.getSize();
				final Vector3f tmpOrigin = it.getOrigin();
				if ((tmpOrigin.x() <= pos.x() && tmpOrigin.x() + tmpSize.x() >= pos.x()) && (tmpOrigin.y() <= pos.y() && tmpOrigin.y() + tmpSize.y() >= pos.y())) {
					final Widget tmpWidget = it.getWidgetAtPos(pos);
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
	public void onChangeSize() {
		for (final Widget it : this.subWidget) {
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
		for (final Widget it : this.subWidget) {
			if (it != null) {
				it.systemRegenerateDisplay();
			}
		}
	}
	
	@Override
	public void requestDestroyFromChild(final EwolObject child) {
		ListIterator<Widget> it = this.subWidget.listIterator();
		while (it.hasNext()) {
			final Widget elem = it.next();
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
	public void setOffset(final Vector3f newVal) {
		if (this.offset != newVal) {
			super.setOffset(newVal);
			// recalculate the new sise and position of sub widget ...
			onChangeSize();
		}
	}
	
	public void setPropertyLockExpand(final Vector3b propertyLockExpand) {
		if (propertyLockExpand.equals(this.propertyLockExpand)) {
			return;
		}
		this.propertyLockExpand = propertyLockExpand;
		markToRedraw();
		requestUpdateSize();
	}
	
	public void setSubWidgets(final List<Widget> listData) {
		// Clean all previous widget
		this.subWidgetRemoveAll();
		// add separately all widgets
		for (final Widget elem : listData) {
			if (elem == null) {
				continue;
			}
			elem.setParent(this);
			this.subWidget.add(0, elem);
		}
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
		final int errorControl = this.subWidget.size();
		
		final ListIterator<Widget> it = this.subWidget.listIterator();
		while (it.hasNext()) {
			final Widget elem = it.next();
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
		final ListIterator<Widget> it = this.subWidget.listIterator();
		while (it.hasNext()) {
			final Widget elem = it.next();
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
		final ListIterator<Widget> it = this.subWidget.listIterator();
		while (it.hasNext()) {
			final Widget elem = it.next();
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
		final ListIterator<Widget> it = this.subWidget.listIterator(this.subWidget.size());
		while (it.hasPrevious()) {
			final Widget elem = it.previous();
			if (elem != null) {
				//Log.info("       ***** : [" + (*it).propertyName + "] t=" + (*it).getObjectType() + " o=" + (*it).this.origin + "  s=" + (*it).this.size);
				elem.systemDraw(prop);
			}
		}
	}
}
