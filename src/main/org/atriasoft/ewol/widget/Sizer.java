/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import java.util.List;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.dataformat.xml.annotation.JacksonXmlProperty;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class Sizer extends ContainerN {
	private static final Logger LOGGER = LoggerFactory.getLogger(Sizer.class);

	public enum DisplayMode {
		HORIZONTAL, //!< Horizontal mode
		VERTICAL; //!< Vertical mode
	}
	
	protected Dimension2f propertyBorderSize = Dimension2f.ZERO; //!< Border size needed for all the display
	protected DisplayMode propertyMode = DisplayMode.HORIZONTAL; //!< Method to display the widget list (vert/hory ...)
	
	/**
	 * Constructor
	 */
	public Sizer() {
		
	}
	
	/**
	 * Constructor
	 * @param mode The mode to display the elements
	 */
	public Sizer(final DisplayMode mode) {
		this.propertyMode = mode;
	}
	
	@Override
	public void calculateMinMaxSize() {
		LOGGER.trace("[{}] update minimum size", getId());
		List<Widget> orderedSubWidget = this.subWidget;
		if (this.propertyMode == DisplayMode.VERTICAL) {
			orderedSubWidget = this.subWidget.reversed();
		}
		this.subExpend = Vector2b.FALSE;
		this.minSize = this.propertyMinSize.getPixel();
		final Vector2f tmpBorderSize = this.propertyBorderSize.getPixel();
		LOGGER.trace("[{}] {{}} set min size: {}", getId(), getClass().getCanonicalName(), this.minSize);
		for (final Widget it : orderedSubWidget) {
			if (it == null) {
				continue;
			}
			it.calculateMinMaxSize();
			if (it.canExpand().x()) {
				this.subExpend = this.subExpend.withX(true);
			}
			if (it.canExpand().y()) {
				this.subExpend = this.subExpend.withY(true);
			}
			final Vector2f tmpSize = it.getCalculateMinSize();
			LOGGER.trace("[{}] NewMinSize={}", getId(), tmpSize);
			LOGGER.trace("[{}] {{}}     Get minSize={}", getId(), getClass().getCanonicalName(), tmpSize);
			if (this.propertyMode == DisplayMode.VERTICAL) {
				this.minSize = this.minSize.withY(this.minSize.y() + tmpSize.y());
				if (tmpSize.x() > this.minSize.x()) {
					this.minSize = this.minSize.withX(tmpSize.x());
				}
			} else {
				this.minSize = this.minSize.withX(this.minSize.x() + tmpSize.x());
				if (tmpSize.y() > this.minSize.y()) {
					this.minSize = this.minSize.withY(tmpSize.y());
				}
			}
		}
		this.minSize = this.minSize.add(tmpBorderSize.multiply(2));
		LOGGER.trace("[{}] Result min size : {}", getId(), this.minSize);
	}
	
	@JsonProperty("border")
	@JacksonXmlProperty(isAttribute = true, localName = "border")
	public Dimension2f getPropertyBorderSize() {
		return this.propertyBorderSize;
	}
	
	@JsonProperty("mode")
	@JacksonXmlProperty(isAttribute = true, localName = "mode")
	public DisplayMode getPropertyMode() {
		return this.propertyMode;
	}
	
	@Override
	public void onChangeSize() {
		super.onChangeSize();
		List<Widget> orderedSubWidget = this.subWidget;
		if (this.propertyMode == DisplayMode.VERTICAL) {
			orderedSubWidget = this.subWidget.reversed();
		}
		final Vector2f tmpBorderSize = this.propertyBorderSize.getPixel();
		LOGGER.trace("[{}] update size: {} nbElement: {} borderSize={} from border={}", getId(), this.size,
				orderedSubWidget.size(), tmpBorderSize, this.propertyBorderSize);
		final Vector2f localWidgetSize = this.size.less(tmpBorderSize.multiply(2.0f));
		// -1- calculate min-size and expand requested:
		Vector2f minSize = Vector2f.ZERO;
		Vector2i nbWidgetExpand = Vector2i.ZERO;
		for (final Widget it : orderedSubWidget) {
			if (it == null) {
				continue;
			}
			final Vector2f tmpSize = it.getCalculateMinSize();
			if (this.propertyMode == DisplayMode.VERTICAL) {
				minSize = new Vector2f(Math.max(minSize.x(), tmpSize.x()), minSize.y() + tmpSize.y());
			} else {
				minSize = new Vector2f(minSize.x() + tmpSize.x(), Math.max(minSize.y(), tmpSize.y()));
			}
			final Vector2b expand = it.canExpand();
			nbWidgetExpand = nbWidgetExpand.add(expand.x() ? 1 : 0, expand.y() ? 1 : 0);
		}
		// -2- Calculate the size to add at every elements...
		float deltaExpandSize = 0.0f;
		if (!nbWidgetExpand.isEqual(Vector2i.ZERO)) {
			if (this.propertyMode == DisplayMode.VERTICAL) {
				deltaExpandSize = (localWidgetSize.y() - minSize.y()) / (nbWidgetExpand.y());
			} else {
				deltaExpandSize = (localWidgetSize.x() - minSize.x()) / (nbWidgetExpand.x());
			}
			if (deltaExpandSize < 0.0) {
				deltaExpandSize = 0;
			}
		}
		// -3- Configure all at the min size ...
		for (final Widget it : orderedSubWidget) {
			if (it == null) {
				continue;
			}
			it.setSize(it.getCalculateMinSize());
		}
		// -4- For each element we apply the minmax range and update if needed
		while (deltaExpandSize > 0.0001f) {
			float residualNext = 0.0f;
			// get the number of element that need to devide...
			int countCalculation = nbWidgetExpand.x();
			if (this.propertyMode == DisplayMode.VERTICAL) {
				countCalculation = nbWidgetExpand.y();
			}
			// -4.1- Update every subWidget size
			Widget lastWidget = null;
			if (!orderedSubWidget.isEmpty()) {
				lastWidget = orderedSubWidget.get(orderedSubWidget.size() - 1);
			}
			for (final Widget it : orderedSubWidget) {
				if (it == null) {
					continue;
				}
				Vector2f tmpSizeMin = it.getSize();
				final Vector2f tmpSizeMax = it.getCalculateMaxSize();
				// Now update his size  his size in X and the current sizer size in Y:
				if (this.propertyMode == DisplayMode.VERTICAL) {
					if (it.canExpand().y() || (it == lastWidget && it.canExpandIfFree().y())) {
						float sizeExpand = tmpSizeMin.y() + deltaExpandSize;
						if (sizeExpand > tmpSizeMax.y()) {
							residualNext += (sizeExpand - tmpSizeMax.y());
							sizeExpand = tmpSizeMax.y();
							countCalculation--;
						}
						tmpSizeMin = tmpSizeMin.withY(sizeExpand);
					}
					it.setSize(tmpSizeMin);
				} else {
					if (it.canExpand().x() || (it == lastWidget && it.canExpandIfFree().x())) {
						float sizeExpand = tmpSizeMin.x() + deltaExpandSize;
						if (sizeExpand > tmpSizeMax.x()) {
							residualNext += (sizeExpand - tmpSizeMax.x());
							sizeExpand = tmpSizeMax.x();
							countCalculation--;
						}
						tmpSizeMin = tmpSizeMin.withX(sizeExpand);
					}
					it.setSize(tmpSizeMin);
				}
			}
			// Reset size add ...
			deltaExpandSize = 0.0f;
			if (residualNext < 0.0001f) {
				break;
			}
			if (countCalculation <= 0) {
				break;
			}
			deltaExpandSize = residualNext / countCalculation;
			if (deltaExpandSize < 0.0f) {
				deltaExpandSize = 0.0f;
				break;
			}
		}
		// -5- Update the expand in the second size if vert ==> X and if hori ==> Y
		for (final Widget it : orderedSubWidget) {
			if (it == null) {
				continue;
			}
			// Now update his size, his size in X and the current sizer size in Y:
			if (this.propertyMode == DisplayMode.VERTICAL) {
				if (!it.canExpand().x() && !it.canExpandIfFree().x()) {
					continue;
				}
				Vector2f tmpSizeMin = it.getSize();
				tmpSizeMin = tmpSizeMin
						.withX(FMath.avg(tmpSizeMin.x(), localWidgetSize.x(), it.getCalculateMaxSize().x()));
				it.setSize(tmpSizeMin);
			} else {
				if (!it.canExpand().y() && !it.canExpandIfFree().y()) {
					continue;
				}
				Vector2f tmpSizeMin = it.getSize();
				tmpSizeMin = tmpSizeMin
						.withY(FMath.avg(tmpSizeMin.y(), localWidgetSize.y(), it.getCalculateMaxSize().y()));
				it.setSize(tmpSizeMin);
			}
		}
		// -6- Force size at the entire number:
		for (final Widget it : orderedSubWidget) {
			if (it == null) {
				continue;
			}
			it.setSize(Vector2f.clipInt(it.getSize()));
		}
		// -7- get under Size
		Vector2f underSize = Vector2f.ZERO;
		for (final Widget it : orderedSubWidget) {
			if (it == null) {
				continue;
			}
			final Vector2f size = it.getSize();
			if (this.propertyMode == DisplayMode.VERTICAL) {
				underSize = new Vector2f(Math.max(underSize.x(), size.x()), underSize.y() + size.y());
			} else {
				underSize = new Vector2f(underSize.x() + size.x(), Math.max(underSize.y(), size.y()));
			}
		}
		final Vector2f deltas = localWidgetSize.less(underSize);
		
		// -8- Calculate the local origin, depending of the gravity:
		Vector2f tmpOrigin = this.origin.add(tmpBorderSize).add(this.propertyGravity.gravityGenerateDelta(deltas));
		// -9- Set sub widget origin:
		for (final Widget it : orderedSubWidget) {
			if (it == null) {
				continue;
			}
			Vector2f origin;
			final Vector2f size = it.getSize();
			if (this.propertyMode == DisplayMode.VERTICAL) {
				origin = Vector2f.clipInt(tmpOrigin.add(this.offset)
						.add(this.propertyGravity.gravityGenerateDelta(new Vector2f(underSize.x() - size.x(), 0.0f))));
			} else {
				origin = Vector2f.clipInt(tmpOrigin.add(this.offset)
						.add(this.propertyGravity.gravityGenerateDelta(new Vector2f(0.0f, underSize.y() - size.y()))));
			}
			it.setOrigin(origin);
			if (this.propertyMode == DisplayMode.VERTICAL) {
				tmpOrigin = tmpOrigin.withY(tmpOrigin.y() + size.y());
			} else {
				tmpOrigin = tmpOrigin.withX(tmpOrigin.x() + size.x());
			}
		}
		// -10- Update all subSize at every element:
		for (final Widget it : orderedSubWidget) {
			if (it == null) {
				continue;
			}
			it.onChangeSize();
		}
		markToRedraw();
	}
	
	public void setPropertyBorderSize(final Dimension2f propertyBorderSize) {
		if (this.propertyBorderSize.equals(propertyBorderSize)) {
			return;
		}
		this.propertyBorderSize = propertyBorderSize;
	}
	
	public void setPropertyMode(final DisplayMode propertyMode) {
		if (this.propertyMode.equals(propertyMode)) {
			return;
		}
		this.propertyMode = propertyMode;
	}
	
	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================
	
	/**
	 * Create a horizontal sizer.
	 * @return a new horizontal Sizer
	 */
	public static Sizer horizontal() {
		return new Sizer(DisplayMode.HORIZONTAL);
	}
	
	/**
	 * Create a vertical sizer.
	 * @return a new vertical Sizer
	 */
	public static Sizer vertical() {
		return new Sizer(DisplayMode.VERTICAL);
	}
	
	/**
	 * Fluent method to set border size.
	 * @param border the border size
	 * @return this sizer for chaining
	 */
	public Sizer border(final Dimension2f border) {
		setPropertyBorderSize(border);
		return this;
	}
	
	/**
	 * Fluent method to add a child widget.
	 * @param widget the widget to add
	 * @return this sizer for chaining
	 */
	public Sizer add(final Widget widget) {
		subWidgetAdd(widget);
		return this;
	}
	
	/**
	 * Fluent method to add multiple child widgets.
	 * @param widgets the widgets to add
	 * @return this sizer for chaining
	 */
	public Sizer add(final Widget... widgets) {
		for (final Widget widget : widgets) {
			subWidgetAdd(widget);
		}
		return this;
	}
}
