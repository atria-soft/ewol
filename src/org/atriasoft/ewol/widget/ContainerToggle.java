/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.Padding;
import org.atriasoft.ewol.annotation.EwolDescription;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.object.EwolObject;
import org.atriasoft.exml.annotation.XmlFactory;
import org.atriasoft.exml.annotation.XmlManaged;

/*
 * @ingroup ewolWidgetGroup
 * the Cotainer widget is a widget that have an only one subWidget
 */
public class ContainerToggle extends Widget {
	protected Widget[] subWidget = new Widget[2];
	int idWidgetDisplayed = 0; //!< current widget displayed
	
	/**
	 * Constructor
	 */
	public ContainerToggle() {
		this.subWidget[0] = null;
		this.subWidget[1] = null;
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
	
	void calculateMinMaxSizePadded(final Padding padding) {
		// call main class
		this.minSize = Vector3f.ZERO;
		// call sub classes
		for (int iii = 0; iii < 2; ++iii) {
			if (this.subWidget[iii] != null) {
				this.subWidget[iii].calculateMinMaxSize();
				final Vector3f min = this.subWidget[iii].getCalculateMinSize();
				this.minSize = this.minSize.max(min);
			}
		}
		// add padding :
		this.minSize = this.minSize.add(padding.x(), padding.y(), padding.z());
		// verify the min max of the min size ...
		checkMinSize();
		//markToRedraw();
		Log.verbose("[{}] Result min size : {}", getId(), this.minSize);
	}
	
	@Override
	public void drawWidgetTree(int level) {
		super.drawWidgetTree(level);
		level++;
		if (this.subWidget[0] != null) {
			this.subWidget[0].drawWidgetTree(level);
		}
		if (this.subWidget[1] != null) {
			this.subWidget[1].drawWidgetTree(level);
		}
	}
	
	@Override
	public EwolObject getSubObjectNamed(final String widgetName) {
		EwolObject tmpObject = super.getSubObjectNamed(widgetName);
		if (tmpObject != null) {
			return tmpObject;
		}
		if (this.subWidget[0] != null) {
			tmpObject = this.subWidget[0].getSubObjectNamed(widgetName);
			if (tmpObject != null) {
				return tmpObject;
			}
		}
		if (this.subWidget[1] != null) {
			return this.subWidget[1].getSubObjectNamed(widgetName);
		}
		return null;
	}
	
	@XmlManaged
	@XmlFactory(value = WidgetXmlFactory.class)
	@EwolDescription(value = "Request the widget Expand size while free space is detected (does not generate expand in upper widget)")
	public Widget[] getSubWidgets() {
		return this.subWidget;
	}
	
	public Padding onChangeSizePadded(final Padding padding) {
		super.onChangeSize();
		final Vector3f localAvaillable = this.size.less(padding.x(), padding.y(), padding.z());
		// Checking the filling properties  == > for the subElements:
		Vector3f subElementSize = this.minSize.less(padding.x(), padding.y(), padding.z());
		if (this.propertyFill.x()) {
			subElementSize = subElementSize.withX(this.size.x() - padding.x());
		}
		if (this.propertyFill.y()) {
			subElementSize = subElementSize.withY(this.size.y() - padding.y());
		}
		if (this.propertyFill.z()) {
			subElementSize = subElementSize.withZ(this.size.z() - padding.z());
		}
		final Vector3f delta = this.propertyGravity.gravityGenerateDelta(this.size.less(subElementSize.add(padding.x(), padding.y(), padding.z())));
		final Vector3f deltaPadded = delta.add(padding.left(), padding.bottom(), padding.back());
		//subElementSize = subElementSize.less(padding.x(), padding.y(), padding.z());
		for (int iii = 0; iii < 2; ++iii) {
			if (this.subWidget[iii] != null) {
				//final Vector3f origin2 = this.origin.add(this.offset);
				//final Vector3f minSize = this.subWidget[iii].getCalculateMinSize();
				//Vector2b expand = this.subWidget[iii].propertyExpand.get();
				//origin2 = origin2.add(this.propertyGravity.gravityGenerateDelta(minSize.less(localAvaillable)));
				this.subWidget[iii].setOrigin(this.origin.add(deltaPadded));
				this.subWidget[iii].setSize(subElementSize);
				this.subWidget[iii].onChangeSize();
			}
		}
		final Vector3f selectableAreaPos = this.origin.add(delta);//.less(padding.left(), padding.bottom(), padding.back());
		final Vector3f selectableAreaEndPos = this.size.less(selectableAreaPos.add(subElementSize.add(padding.x(), padding.y(), padding.z())));
		markToRedraw();
		return new Padding(selectableAreaPos.x(), selectableAreaEndPos.y(), selectableAreaEndPos.x(), selectableAreaPos.y());
	}
	
	@Override
	public void onRegenerateDisplay() {
		if (this.subWidget[this.idWidgetDisplayed] != null) {
			this.subWidget[this.idWidgetDisplayed].onRegenerateDisplay();
		}
	}
	/*
	boolean loadXML( exml::Element _node) {
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
	}*/
	
	@Override
	public void requestDestroyFromChild(final EwolObject child) {
		if (this.subWidget[0] == child) {
			if (this.subWidget[0] == null) {
				return;
			}
			this.subWidget[0].removeParent();
			this.subWidget[0] = null;
			markToRedraw();
		}
		if (this.subWidget[1] == child) {
			if (this.subWidget[1] == null) {
				return;
			}
			this.subWidget[1].removeParent();
			this.subWidget[1] = null;
			markToRedraw();
		}
	}
	
	@Override
	public void setOffset(final Vector3f newVal) {
		if (this.offset.equals(newVal)) {
			return;
		}
		super.setOffset(newVal);
		// recalculate the new size and position of sub widget ...
		onChangeSize();
	}
	
	/**
	 * set the subWidget node widget.
	 * @param newWidget The widget to add.
	 */
	public void setSubWidget(final Widget newWidget, final int idWidget) {
		subWidgetRemove(idWidget);
		this.subWidget[idWidget] = newWidget;
		if (this.subWidget[idWidget] != null) {
			Log.verbose("Add widget : " + idWidget);
			this.subWidget[idWidget].setParent(this);
		}
		markToRedraw();
		requestUpdateSize();
	}
	
	public void subWidgetRemove(final int idWidget) {
		if (this.subWidget[idWidget] != null) {
			Log.verbose("Remove widget : " + idWidget);
			this.subWidget[idWidget].removeParent();
			this.subWidget[idWidget] = null;
			markToRedraw();
			requestUpdateSize();
		}
	}
	
	public void subWidgetReplace(final Widget oldWidget, final Widget newWidget) {
		boolean haveChange = false;
		for (int iii = 0; iii < 2; ++iii) {
			if (this.subWidget[iii] != oldWidget) {
				continue;
			}
			this.subWidget[iii].removeParent();
			this.subWidget[iii] = newWidget;
			if (this.subWidget[iii] != null) {
				this.subWidget[iii].setParent(this);
			}
			haveChange = true;
		}
		if (!haveChange) {
			Log.warning("Request replace with a wrong old widget");
			return;
		}
		markToRedraw();
		requestUpdateSize();
	}
	
	public void subWidgetUnLink(final int idWidget) {
		if (this.subWidget[idWidget] != null) {
			this.subWidget[idWidget].removeParent();
			Log.verbose("Unlink widget : " + idWidget);
		}
		this.subWidget[idWidget] = null;
	}
	
	@Override
	public void systemDraw(final DrawProperty displayProp) {
		if (this.propertyHide) {
			// widget is hidden ...
			return;
		}
		super.systemDraw(displayProp);
		if (this.subWidget[this.idWidgetDisplayed] != null) {
			final DrawProperty prop = displayProp.withLimit(this.origin, this.size);
			//Log.info("Draw : [" + propertyName + "] t=" + getObjectType() + " o=" + this.origin + "  s=" + this.size);
			this.subWidget[this.idWidgetDisplayed].systemDraw(prop);
		} else {
			Log.info("[" + getId() + "]       ++++++ : [null]");
		}
	}
}