/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotFactory;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.etk.math.Vector3b;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.object.EwolObject;

/*
 * @ingroup ewolWidgetGroup
 * the Cotainer widget is a widget that have an only one subWidget
 */
public class Container extends Widget {
	protected Widget subWidget = null;
	
	/**
	 * Constructor
	 */
	public Container() {}
	
	@Override
	public void calculateMinMaxSize() {
		// call main class
		super.calculateMinMaxSize();
		// call sub classes
		if (this.subWidget != null) {
			this.subWidget.calculateMinMaxSize();
			final Vector3f min = this.subWidget.getCalculateMinSize();
			this.minSize = Vector3f.max(this.minSize, min);
		}
		Log.warning("[{}] Result min size : {}", getId(), this.minSize);
	}
	
	@Override
	public void drawWidgetTree(int level) {
		super.drawWidgetTree(level);
		level++;
		if (this.subWidget != null) {
			this.subWidget.drawWidgetTree(level);
		}
	}
	
	@Override
	public EwolObject getSubObjectNamed(final String objectName) {
		final EwolObject tmpObject = super.getSubObjectNamed(objectName);
		if (tmpObject != null) {
			return tmpObject;
		}
		if (this.subWidget != null) {
			return this.subWidget.getSubObjectNamed(objectName);
		}
		return null;
	}
	
	/**
	 * get the main node widget
	 * @return the requested pointer on the node
	 */
	@AknotManaged
	@AknotAttribute(false)
	@AknotFactory(WidgetXmlFactory.class)
	@AknotDescription(value = "Sub-node with multiple names...")
	public Widget getSubWidget() {
		return this.subWidget;
	}
	
	@Override
	public Widget getWidgetAtPos(final Vector3f pos) {
		if (!this.propertyHide) {
			if (this.subWidget != null) {
				return this.subWidget.getWidgetAtPos(pos);
			}
		}
		return null;
	}
	
	//	@Override
	//	public boolean loadXML(final XmlElement node) {
	//		if (node == null) {
	//			return false;
	//		}
	//		// parse generic properties:
	//		super.loadXML(node);
	//		// remove previous element:
	//		subWidgetRemove();
	//		// parse all the elements:
	//		for (XmlNode it : node.getNodes()) {
	//			if (!it.isElement()) {
	//				// trash here all that is not element
	//				continue;
	//			}
	//			XmlElement pNode = it.toElement();
	//			String widgetName = pNode.getValue();
	//			Log.verbose("[" + getId() + "] t=" + getClass().getCanonicalName() + " Load node name : '" + widgetName + "'");
	//			if (!getWidgetManager().exist(widgetName)) {
	//				Log.error("Unknown basic node='" + widgetName + "' not in : [" + getWidgetManager().list() + "]");
	//				continue;
	//			}
	//			if (getSubWidget() != null) {
	//				Log.error("Can only have one subWidget ??? node='" + widgetName + "'");
	//				continue;
	//			}
	//			Log.debug("try to create subwidget : '" + widgetName + "'");
	//			Widget tmpWidget = getWidgetManager().create(widgetName, pNode);
	//			if (tmpWidget == null) {
	//				Log.error("Can not create the widget : '" + widgetName + "'");
	//				continue;
	//			}
	//			// add widget :
	//			setSubWidget(tmpWidget);
	//			if (!tmpWidget.loadXML(pNode)) {
	//				Log.error("can not load widget properties : '" + widgetName + "'");
	//				return false;
	//			}
	//		}
	//		if (node.getNodes().size() != 0 && this.subWidget == null) {
	//			Log.warning("Load container with no data inside");
	//		}
	//		return true;
	//	}
	
	@Override
	public void onChangeSize() {
		super.onChangeSize();
		if (this.propertyHide) {
			return;
		}
		if (this.subWidget == null) {
			return;
		}
		Vector3f origin = this.origin.add(this.offset);
		final Vector3f minSize = this.subWidget.getCalculateMinSize();
		final Vector3b expand = this.subWidget.getPropertyExpand();
		origin = origin.add(this.propertyGravity.gravityGenerateDelta(minSize.less(this.size)));
		this.subWidget.setOrigin(origin);
		this.subWidget.setSize(this.size);
		this.subWidget.onChangeSize();
	}
	
	@Override
	public void onRegenerateDisplay() {
		if (this.subWidget != null) {
			this.subWidget.systemRegenerateDisplay();
		}
	}
	
	@Override
	public void requestDestroyFromChild(final EwolObject child) {
		if (this.subWidget != child) {
			return;
		}
		if (this.subWidget == null) {
			return;
		}
		this.subWidget.removeParent();
		this.subWidget = null;
		markToRedraw();
	}
	
	@Override
	public void setOffset(final Vector3f newVal) {
		if (this.offset.equals(newVal)) {
			return;
		}
		super.setOffset(newVal);
		// recalculate the new sise and position of sub widget ...
		onChangeSize();
		
	}
	
	/**
	 * set the subWidget node widget.
	 * @param newWidget The widget to add.
	 */
	public void setSubWidget(final Widget newWidget) {
		if (newWidget == null) {
			return;
		}
		subWidgetRemove();
		this.subWidget = newWidget;
		if (this.subWidget != null) {
			this.subWidget.setParent(this);
		}
		markToRedraw();
		requestUpdateSize();
	}
	
	/**
	 * remove the subWidget node (async).
	 */
	public void subWidgetRemove() {
		if (this.subWidget != null) {
			this.subWidget.removeParent();
			this.subWidget = null;
			markToRedraw();
			requestUpdateSize();
		}
	}
	
	/**
	 * Replace a old subwidget with a new one.
	 * @param oldWidget The widget to replace.
	 * @param newWidget The widget to set.
	 */
	public void subWidgetReplace(final Widget oldWidget, final Widget newWidget) {
		if (this.subWidget != oldWidget) {
			Log.warning("Request replace with a wrong old widget");
			return;
		}
		this.subWidget.removeParent();
		this.subWidget = newWidget;
		if (this.subWidget != null) {
			this.subWidget.setParent(this);
		}
		markToRedraw();
		requestUpdateSize();
	}
	
	/**
	 * Unlink the subwidget Node.
	 */
	public void subWidgetUnLink() {
		if (this.subWidget != null) {
			this.subWidget.removeParent();
		}
		this.subWidget = null;
	}
	
	@Override
	public void systemDraw(final DrawProperty displayProp) {
		if (this.propertyHide) {
			// widget is hidden ...
			return;
		}
		super.systemDraw(displayProp);
		if (this.subWidget != null) {
			final DrawProperty prop = displayProp.withLimit(this.origin, this.size);
			//Log.info("Draw : [" + propertyName + "] t=" + getObjectType() + " o=" + this.origin + "  s=" + this.size);
			this.subWidget.systemDraw(prop);
		} else {
			Log.info("[" + getId() + "]       ++++++ : [null]");
		}
	}
}