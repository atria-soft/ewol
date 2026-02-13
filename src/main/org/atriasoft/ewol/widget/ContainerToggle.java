/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.dataformat.xml.annotation.JacksonXmlElementWrapper;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.Padding;
import org.atriasoft.ewol.object.EwolObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/*
 * @ingroup ewolWidgetGroup
 * the Container widget is a widget that have an only one subWidget
 */
public class ContainerToggle extends Widget {
	private static final Logger LOGGER = LoggerFactory.getLogger(ContainerToggle.class);
	protected Widget[] subWidget = new Widget[2];
	int idWidgetDisplayed = 0;
	
	/**
	 * Constructor
	 */
	public ContainerToggle() {
		this.subWidget[0] = null;
		this.subWidget[1] = null;
	}
	
	void calculateMinMaxSizePadded(final Padding padding) {
		// call main class
		this.minSize = Vector2f.ZERO;
		// call sub classes
		for (final Widget element : this.subWidget) {
			if (element != null) {
				element.calculateMinMaxSize();
				final Vector2f min = element.getCalculateMinSize();
				this.minSize = this.minSize.max(min);
			}
		}
		// add padding :
		this.minSize = this.minSize.add(padding.x(), padding.y());
		// verify the min max of the min size ...
		checkMinSize();
		//markToRedraw();
		LOGGER.trace("[{}] Result min size : {}", getId(), this.minSize);
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
	
	@JsonProperty
	@JacksonXmlElementWrapper(useWrapping = false)
	public Widget[] getSubWidgets() {
		return this.subWidget;
	}
	
	public Padding onChangeSizePadded(final Padding padding) {
		super.onChangeSize();
		final Vector2f localAvaillable = this.size.less(padding.x(), padding.y());
		// Checking the filling properties  == > for the subElements:
		Vector2f subElementSize = this.minSize.less(padding.x(), padding.y());
		if (this.propertyFill.x()) {
			subElementSize = subElementSize.withX(this.size.x() - padding.x());
		}
		if (this.propertyFill.y()) {
			subElementSize = subElementSize.withY(this.size.y() - padding.y());
		}
		final Vector2f delta = this.propertyGravity
				.gravityGenerateDelta(this.size.less(subElementSize.add(padding.x(), padding.y())));
		final Vector2f deltaPadded = delta.add(padding.left(), padding.bottom());
		//subElementSize = subElementSize.less(padding.x(), padding.y(), padding.z());
		for (final Widget element : this.subWidget) {
			if (element != null) {
				//final Vector2f origin2 = this.origin.add(this.offset);
				//final Vector2f minSize = this.subWidget[iii].getCalculateMinSize();
				//Vector2b expand = this.subWidget[iii].propertyExpand.get();
				//origin2 = origin2.add(this.propertyGravity.gravityGenerateDelta(minSize.less(localAvaillable)));
				element.setOrigin(this.origin.add(deltaPadded));
				element.setSize(subElementSize);
				element.onChangeSize();
			}
		}
		final Vector2f selectableAreaPos = this.origin.add(delta);//.less(padding.left(), padding.bottom(), padding.back());
		final Vector2f selectableAreaEndPos = this.size
				.less(selectableAreaPos.add(subElementSize.add(padding.x(), padding.y())));
		markToRedraw();
		return new Padding(selectableAreaPos.x(), selectableAreaEndPos.y(), selectableAreaEndPos.x(),
				selectableAreaPos.y());
	}
	
	@Override
	public void onRegenerateDisplay() {
		if (this.subWidget[this.idWidgetDisplayed] != null) {
			this.subWidget[this.idWidgetDisplayed].onRegenerateDisplay();
		}
	}
	
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
	public void setOffset(final Vector2f newVal) {
		if (this.offset.equals(newVal)) {
			return;
		}
		super.setOffset(newVal);
		// recalculate the new size and position of sub widget ...
		onChangeSize();
	}
	
	@JsonIgnore
	public void setSubWidget(final Widget newWidget) {
		setSubWidget(newWidget, 0);
	}
	
	/**
	 * set the subWidget node widget.
	 * @param newWidget The widget to add.
	 */
	public void setSubWidget(final Widget newWidget, final int idWidget) {
		subWidgetRemove(idWidget);
		this.subWidget[idWidget] = newWidget;
		if (this.subWidget[idWidget] != null) {
			LOGGER.trace("Add widget: {}", idWidget);
			this.subWidget[idWidget].setParent(this);
		}
		markToRedraw();
		requestUpdateSize();
	}
	
	public void setSubWidgets(final Widget[] newWidget) {
		for (int iii = 0; iii < Math.min(newWidget.length, this.subWidget.length); iii++) {
			setSubWidget(newWidget[iii], iii);
		}
	}
	
	public void subWidgetRemove(final int idWidget) {
		if (this.subWidget[idWidget] != null) {
			LOGGER.trace("Remove widget: {}", idWidget);
			this.subWidget[idWidget].removeParent();
			this.subWidget[idWidget] = null;
			markToRedraw();
			requestUpdateSize();
		}
	}
	
	public void subWidgetReplace(final Widget oldWidget, final Widget newWidget) {
		boolean haveChange = false;
		for (int iii = 0; iii < this.subWidget.length; ++iii) {
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
			LOGGER.warn("Request replace with a wrong old widget");
			return;
		}
		markToRedraw();
		requestUpdateSize();
	}
	
	public void subWidgetUnLink(final int idWidget) {
		if (this.subWidget[idWidget] != null) {
			this.subWidget[idWidget].removeParent();
			LOGGER.trace("Unlink widget: {}", idWidget);
		}
		this.subWidget[idWidget] = null;
	}
	
	@Override
	public void systemDraw(final DrawProperty displayProp) {
		if (this.propertyHide) {
			return;
		}
		super.systemDraw(displayProp);
		if (this.subWidget[this.idWidgetDisplayed] != null) {
			final DrawProperty prop = displayProp.withLimit(this.origin, this.size);
			this.subWidget[this.idWidgetDisplayed].systemDraw(prop);
		} else {
			LOGGER.debug("[{}]       ++++++ : [null]", getId());
		}
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	/**
	 * Create a new ContainerToggle.
	 * @return a new ContainerToggle
	 */
	public static ContainerToggle create() {
		return new ContainerToggle();
	}

	/**
	 * Fluent method to set first widget.
	 * @param widget the first widget
	 * @return this container for chaining
	 */
	public ContainerToggle first(final Widget widget) {
		setSubWidget(widget, 0);
		return this;
	}

	/**
	 * Fluent method to set second widget.
	 * @param widget the second widget
	 * @return this container for chaining
	 */
	public ContainerToggle second(final Widget widget) {
		setSubWidget(widget, 1);
		return this;
	}

	/**
	 * Fluent method to show first widget.
	 * @return this container for chaining
	 */
	public ContainerToggle showFirst() {
		this.idWidgetDisplayed = 0;
		markToRedraw();
		return this;
	}

	/**
	 * Fluent method to show second widget.
	 * @return this container for chaining
	 */
	public ContainerToggle showSecond() {
		this.idWidgetDisplayed = 1;
		markToRedraw();
		return this;
	}

	/**
	 * Toggle between first and second widget.
	 */
	public void toggle() {
		this.idWidgetDisplayed = (this.idWidgetDisplayed + 1) % 2;
		markToRedraw();
	}
}