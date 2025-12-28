/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import java.util.List;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotCaseSensitive;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotFactory;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.aknot.annotation.AknotSignal;
import org.atriasoft.esignal.Signal;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.compositing.CompositingDrawing;
import org.atriasoft.ewol.compositing.CompositingGC;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.object.EwolObject;
import org.atriasoft.ewol.resource.ResourceColorFile;
import org.atriasoft.gale.key.KeyStatus;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * A split pane widget that divides its space between two child widgets
 * with a draggable separator to resize them dynamically.
 */
public class SplitPane extends Widget {
	private static final Logger LOGGER = LoggerFactory.getLogger(SplitPane.class);

	@AknotCaseSensitive(value = false)
	public enum Orientation {
		HORIZONTAL,
		VERTICAL
	}

	@AknotSignal
	@AknotName("split-changed")
	@AknotDescription("The split position has changed")
	public Signal<Float> signalSplitChanged = new Signal<>();

	protected Widget firstWidget = null;
	protected Widget secondWidget = null;

	protected Orientation propertyOrientation = Orientation.HORIZONTAL;
	protected float propertySplitPosition = 0.5f;
	protected float propertySeparatorSize = 8.0f;
	protected float propertyMinFirstSize = 50.0f;
	protected float propertyMinSecondSize = 50.0f;

	protected ResourceColorFile colorProperty;
	protected int colorIdSeparator = -1;
	protected int colorIdSeparatorHover = -1;
	protected int colorIdSeparatorDrag = -1;

	protected CompositingDrawing drawing = new CompositingGC();

	private boolean isDragging = false;
	private boolean isHovering = false;
	private float dragStartPos = 0.0f;
	private float dragStartSplit = 0.0f;

	public SplitPane() {
		this.colorProperty = ResourceColorFile.create(new Uri("THEME", "/color/SplitPane.json", "ewol"));
		if (this.colorProperty != null) {
			this.colorIdSeparator = this.colorProperty.request("separator");
			this.colorIdSeparatorHover = this.colorProperty.request("separator-hover");
			this.colorIdSeparatorDrag = this.colorProperty.request("separator-drag");
		}
		setMouseLimit(1);
	}

	public SplitPane(final Orientation orientation) {
		this();
		this.propertyOrientation = orientation;
	}

	@Override
	public void calculateMinMaxSize() {
		super.calculateMinMaxSize();

		Vector2f minFirst = Vector2f.ZERO;
		Vector2f minSecond = Vector2f.ZERO;

		if (this.firstWidget != null) {
			this.firstWidget.calculateMinMaxSize();
			minFirst = this.firstWidget.getCalculateMinSize();
		}
		if (this.secondWidget != null) {
			this.secondWidget.calculateMinMaxSize();
			minSecond = this.secondWidget.getCalculateMinSize();
		}

		if (this.propertyOrientation == Orientation.HORIZONTAL) {
			this.minSize = new Vector2f(
				minFirst.x() + minSecond.x() + this.propertySeparatorSize,
				Math.max(minFirst.y(), minSecond.y())
			);
		} else {
			this.minSize = new Vector2f(
				Math.max(minFirst.x(), minSecond.x()),
				minFirst.y() + minSecond.y() + this.propertySeparatorSize
			);
		}

		checkMinSize();
		LOGGER.trace("[{}] Result min size : {}", getId(), this.minSize);
	}

	@Override
	public void drawWidgetTree(int level) {
		super.drawWidgetTree(level);
		level++;
		if (this.firstWidget != null) {
			this.firstWidget.drawWidgetTree(level);
		}
		if (this.secondWidget != null) {
			this.secondWidget.drawWidgetTree(level);
		}
	}

	@Override
	public EwolObject getSubObjectNamed(final String objectName) {
		EwolObject tmpObject = super.getSubObjectNamed(objectName);
		if (tmpObject != null) {
			return tmpObject;
		}
		if (this.firstWidget != null) {
			tmpObject = this.firstWidget.getSubObjectNamed(objectName);
			if (tmpObject != null) {
				return tmpObject;
			}
		}
		if (this.secondWidget != null) {
			return this.secondWidget.getSubObjectNamed(objectName);
		}
		return null;
	}

	@Override
	public Widget getWidgetAtPos(final Vector2f pos) {
		if (this.propertyHide) {
			return null;
		}

		// Check if position is on separator
		if (isOnSeparator(pos)) {
			LOGGER.debug("getWidgetAtPos: on separator, returning this");
			return this;
		}

		// Check first widget
		if (this.firstWidget != null) {
			final Vector2f tmpSize = this.firstWidget.getSize();
			final Vector2f tmpOrigin = this.firstWidget.getOrigin();
			LOGGER.debug("getWidgetAtPos: checking first widget pos={} origin={} size={}", pos, tmpOrigin, tmpSize);
			if (pos.x() >= tmpOrigin.x() && pos.x() <= tmpOrigin.x() + tmpSize.x()
					&& pos.y() >= tmpOrigin.y() && pos.y() <= tmpOrigin.y() + tmpSize.y()) {
				final Widget result = this.firstWidget.getWidgetAtPos(pos);
				LOGGER.debug("getWidgetAtPos: first widget hit, result={}", result);
				if (result != null) {
					return result;
				}
			}
		}

		// Check second widget
		if (this.secondWidget != null) {
			final Vector2f tmpSize = this.secondWidget.getSize();
			final Vector2f tmpOrigin = this.secondWidget.getOrigin();
			LOGGER.debug("getWidgetAtPos: checking second widget pos={} origin={} size={}", pos, tmpOrigin, tmpSize);
			if (pos.x() >= tmpOrigin.x() && pos.x() <= tmpOrigin.x() + tmpSize.x()
					&& pos.y() >= tmpOrigin.y() && pos.y() <= tmpOrigin.y() + tmpSize.y()) {
				final Widget result = this.secondWidget.getWidgetAtPos(pos);
				LOGGER.debug("getWidgetAtPos: second widget hit, result={}", result);
				if (result != null) {
					return result;
				}
			}
		}

		LOGGER.debug("getWidgetAtPos: no widget found, returning this");
		return this;
	}

	private boolean isOnSeparator(final Vector2f pos) {
		final Vector2f relPos = pos.less(this.origin);
		final float separatorPos = getSeparatorPosition();

		if (this.propertyOrientation == Orientation.HORIZONTAL) {
			return relPos.x() >= separatorPos && relPos.x() <= separatorPos + this.propertySeparatorSize;
		} else {
			return relPos.y() >= separatorPos && relPos.y() <= separatorPos + this.propertySeparatorSize;
		}
	}

	private float getSeparatorPosition() {
		if (this.propertyOrientation == Orientation.HORIZONTAL) {
			return (this.size.x() - this.propertySeparatorSize) * this.propertySplitPosition;
		} else {
			return (this.size.y() - this.propertySeparatorSize) * this.propertySplitPosition;
		}
	}

	@Override
	public void onChangeSize() {
		super.onChangeSize();

		final float separatorPos = getSeparatorPosition();

		if (this.propertyOrientation == Orientation.HORIZONTAL) {
			// First widget on the left
			if (this.firstWidget != null) {
				this.firstWidget.setOrigin(this.origin.add(this.offset));
				this.firstWidget.setSize(new Vector2f(separatorPos, this.size.y()));
				this.firstWidget.onChangeSize();
				this.firstWidget.markToRedraw();
			}

			// Second widget on the right
			if (this.secondWidget != null) {
				final float secondX = separatorPos + this.propertySeparatorSize;
				this.secondWidget.setOrigin(this.origin.add(this.offset).add(secondX, 0));
				this.secondWidget.setSize(new Vector2f(this.size.x() - secondX, this.size.y()));
				this.secondWidget.onChangeSize();
				this.secondWidget.markToRedraw();
			}
		} else {
			// First widget at the bottom
			if (this.firstWidget != null) {
				this.firstWidget.setOrigin(this.origin.add(this.offset));
				this.firstWidget.setSize(new Vector2f(this.size.x(), separatorPos));
				this.firstWidget.onChangeSize();
				this.firstWidget.markToRedraw();
			}

			// Second widget at the top
			if (this.secondWidget != null) {
				final float secondY = separatorPos + this.propertySeparatorSize;
				this.secondWidget.setOrigin(this.origin.add(this.offset).add(0, secondY));
				this.secondWidget.setSize(new Vector2f(this.size.x(), this.size.y() - secondY));
				this.secondWidget.onChangeSize();
				this.secondWidget.markToRedraw();
			}
		}

		markToRedraw();
	}

	@Override
	protected void onDraw() {
		this.drawing.draw();
	}

	@Override
	public void onRegenerateDisplay() {
		// Always regenerate sub-widgets first
		if (this.firstWidget != null) {
			this.firstWidget.systemRegenerateDisplay();
		}
		if (this.secondWidget != null) {
			this.secondWidget.systemRegenerateDisplay();
		}

		if (!needRedraw()) {
			return;
		}

		this.drawing.clear();

		// Draw separator
		Color separatorColor;
		if (this.isDragging) {
			separatorColor = this.colorProperty != null
				? this.colorProperty.get(this.colorIdSeparatorDrag)
				: new Color(0x60, 0x60, 0x60, 0xFF);
		} else if (this.isHovering) {
			separatorColor = this.colorProperty != null
				? this.colorProperty.get(this.colorIdSeparatorHover)
				: new Color(0x80, 0x80, 0x80, 0xFF);
		} else {
			separatorColor = this.colorProperty != null
				? this.colorProperty.get(this.colorIdSeparator)
				: new Color(0xA0, 0xA0, 0xA0, 0xFF);
		}

		this.drawing.setColor(separatorColor);

		final float separatorPos = getSeparatorPosition();

		if (this.propertyOrientation == Orientation.HORIZONTAL) {
			this.drawing.setPos(new Vector2f(separatorPos, 0));
			this.drawing.rectangleWidth(new Vector2f(this.propertySeparatorSize, this.size.y()));
		} else {
			this.drawing.setPos(new Vector2f(0, separatorPos));
			this.drawing.rectangleWidth(new Vector2f(this.size.x(), this.propertySeparatorSize));
		}

		this.drawing.flush();
	}

	@Override
	protected boolean onEventInput(final EventInput event) {
		final Vector2f relPos = relativePosition(new Vector2f(event.pos().x(), event.pos().y()));
		final boolean onSeparator = isOnSeparator(event.pos());

		if (event.inputId() == 1) {
			if (event.status() == KeyStatus.down && onSeparator) {
				this.isDragging = true;
				if (this.propertyOrientation == Orientation.HORIZONTAL) {
					this.dragStartPos = event.pos().x();
				} else {
					this.dragStartPos = event.pos().y();
				}
				this.dragStartSplit = this.propertySplitPosition;
				markToRedraw();
				return true;
			} else if (event.status() == KeyStatus.up && this.isDragging) {
				this.isDragging = false;
				markToRedraw();
				return true;
			} else if (event.status() == KeyStatus.move && this.isDragging) {
				float currentPos;
				float totalSize;

				if (this.propertyOrientation == Orientation.HORIZONTAL) {
					currentPos = event.pos().x();
					totalSize = this.size.x() - this.propertySeparatorSize;
				} else {
					currentPos = event.pos().y();
					totalSize = this.size.y() - this.propertySeparatorSize;
				}

				if (totalSize > 0) {
					final float delta = currentPos - this.dragStartPos;
					float newSplit = this.dragStartSplit + delta / totalSize;

					// Apply minimum size constraints
					final float minFirstRatio = this.propertyMinFirstSize / totalSize;
					final float minSecondRatio = this.propertyMinSecondSize / totalSize;

					newSplit = FMath.avg(minFirstRatio, newSplit, 1.0f - minSecondRatio);

					if (Math.abs(newSplit - this.propertySplitPosition) > 0.001f) {
						this.propertySplitPosition = newSplit;
						this.signalSplitChanged.emit(this.propertySplitPosition);
						onChangeSize();
					}
				}
				return true;
			} else if (event.status() == KeyStatus.move && !this.isDragging) {
				final boolean wasHovering = this.isHovering;
				this.isHovering = onSeparator;
				if (wasHovering != this.isHovering) {
					markToRedraw();
				}
			}
		}

		return false;
	}

	@Override
	public void requestDestroyFromChild(final EwolObject child) {
		if (this.firstWidget == child) {
			if (this.firstWidget != null) {
				this.firstWidget.removeParent();
				this.firstWidget = null;
				markToRedraw();
			}
		}
		if (this.secondWidget == child) {
			if (this.secondWidget != null) {
				this.secondWidget.removeParent();
				this.secondWidget = null;
				markToRedraw();
			}
		}
	}

	@Override
	public void setOffset(final Vector2f newVal) {
		if (this.offset.equals(newVal)) {
			return;
		}
		super.setOffset(newVal);
		onChangeSize();
	}

	@Override
	public void systemDraw(final DrawProperty displayProp) {
		if (this.propertyHide) {
			return;
		}
		super.systemDraw(displayProp);

		final DrawProperty prop = displayProp.withLimit(this.origin, this.size);

		if (this.firstWidget != null) {
			this.firstWidget.systemDraw(prop);
		}
		if (this.secondWidget != null) {
			this.secondWidget.systemDraw(prop);
		}
	}

	// ========================================================================
	// Property getters and setters
	// ========================================================================

	@AknotManaged
	@AknotAttribute
	@AknotName("orientation")
	@AknotDescription("Split orientation (HORIZONTAL or VERTICAL)")
	public Orientation getPropertyOrientation() {
		return this.propertyOrientation;
	}

	public void setPropertyOrientation(final Orientation orientation) {
		if (this.propertyOrientation == orientation) {
			return;
		}
		this.propertyOrientation = orientation;
		markToRedraw();
		requestUpdateSize();
	}

	@AknotManaged
	@AknotAttribute
	@AknotName("split-position")
	@AknotDescription("Split position ratio (0.0 to 1.0)")
	public float getPropertySplitPosition() {
		return this.propertySplitPosition;
	}

	public void setPropertySplitPosition(final float position) {
		final float clamped = FMath.avg(0.0f, position, 1.0f);
		if (Math.abs(this.propertySplitPosition - clamped) < 0.001f) {
			return;
		}
		this.propertySplitPosition = clamped;
		this.signalSplitChanged.emit(this.propertySplitPosition);
		markToRedraw();
		requestUpdateSize();
	}

	@AknotManaged
	@AknotAttribute
	@AknotName("separator-size")
	@AknotDescription("Size of the separator in pixels")
	public float getPropertySeparatorSize() {
		return this.propertySeparatorSize;
	}

	public void setPropertySeparatorSize(final float size) {
		if (this.propertySeparatorSize == size) {
			return;
		}
		this.propertySeparatorSize = Math.max(1.0f, size);
		markToRedraw();
		requestUpdateSize();
	}

	@AknotManaged
	@AknotAttribute
	@AknotName("min-first-size")
	@AknotDescription("Minimum size of the first widget in pixels")
	public float getPropertyMinFirstSize() {
		return this.propertyMinFirstSize;
	}

	public void setPropertyMinFirstSize(final float size) {
		if (this.propertyMinFirstSize == size) {
			return;
		}
		this.propertyMinFirstSize = Math.max(0.0f, size);
	}

	@AknotManaged
	@AknotAttribute
	@AknotName("min-second-size")
	@AknotDescription("Minimum size of the second widget in pixels")
	public float getPropertyMinSecondSize() {
		return this.propertyMinSecondSize;
	}

	public void setPropertyMinSecondSize(final float size) {
		if (this.propertyMinSecondSize == size) {
			return;
		}
		this.propertyMinSecondSize = Math.max(0.0f, size);
	}

	/**
	 * Get the list of sub-widgets for XML parsing.
	 * Returns a list containing firstWidget and secondWidget (if set).
	 * @return list of child widgets
	 */
	@AknotManaged
	@AknotFactory(value = WidgetXmlFactory.class)
	@AknotDescription(value = "Sub-widgets of the split pane (first and second)")
	public List<Widget> getSubWidgets() {
		if (this.firstWidget != null && this.secondWidget != null) {
			return List.of(this.firstWidget, this.secondWidget);
		} else if (this.firstWidget != null) {
			return List.of(this.firstWidget);
		} else if (this.secondWidget != null) {
			return List.of(this.secondWidget);
		}
		return List.of();
	}

	/**
	 * Set the sub-widgets from XML parsing.
	 * The first widget in the list becomes the first widget,
	 * the second becomes the second widget.
	 * @param widgets list of widgets to add
	 */
	public void setSubWidgets(final List<Widget> widgets) {
		if (widgets == null || widgets.isEmpty()) {
			return;
		}
		if (widgets.size() >= 1) {
			setFirstWidget(widgets.get(0));
		}
		if (widgets.size() >= 2) {
			setSecondWidget(widgets.get(1));
		}
	}

	public Widget getFirstWidget() {
		return this.firstWidget;
	}

	public void setFirstWidget(final Widget widget) {
		if (this.firstWidget != null) {
			this.firstWidget.removeParent();
		}
		this.firstWidget = widget;
		if (this.firstWidget != null) {
			this.firstWidget.setParent(this);
		}
		markToRedraw();
		requestUpdateSize();
	}

	public Widget getSecondWidget() {
		return this.secondWidget;
	}

	public void setSecondWidget(final Widget widget) {
		if (this.secondWidget != null) {
			this.secondWidget.removeParent();
		}
		this.secondWidget = widget;
		if (this.secondWidget != null) {
			this.secondWidget.setParent(this);
		}
		markToRedraw();
		requestUpdateSize();
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	/**
	 * Create a horizontal split pane.
	 * @return a new horizontal SplitPane
	 */
	public static SplitPane horizontal() {
		return new SplitPane(Orientation.HORIZONTAL);
	}

	/**
	 * Create a vertical split pane.
	 * @return a new vertical SplitPane
	 */
	public static SplitPane vertical() {
		return new SplitPane(Orientation.VERTICAL);
	}

	/**
	 * Create a new SplitPane with default settings.
	 * @return a new SplitPane
	 */
	public static SplitPane create() {
		return new SplitPane();
	}

	/**
	 * Fluent method to set orientation.
	 * @param orientation the orientation
	 * @return this split pane for chaining
	 */
	public SplitPane orientation(final Orientation orientation) {
		setPropertyOrientation(orientation);
		return this;
	}

	/**
	 * Fluent method to set split position.
	 * @param position the split position (0.0 to 1.0)
	 * @return this split pane for chaining
	 */
	public SplitPane splitPosition(final float position) {
		setPropertySplitPosition(position);
		return this;
	}

	/**
	 * Fluent method to set separator size.
	 * @param size the separator size in pixels
	 * @return this split pane for chaining
	 */
	public SplitPane separatorSize(final float size) {
		setPropertySeparatorSize(size);
		return this;
	}

	/**
	 * Fluent method to set minimum sizes.
	 * @param minFirst minimum size for first widget
	 * @param minSecond minimum size for second widget
	 * @return this split pane for chaining
	 */
	public SplitPane minSizes(final float minFirst, final float minSecond) {
		setPropertyMinFirstSize(minFirst);
		setPropertyMinSecondSize(minSecond);
		return this;
	}

	/**
	 * Fluent method to set the first (left/bottom) widget.
	 * @param widget the first widget
	 * @return this split pane for chaining
	 */
	public SplitPane first(final Widget widget) {
		setFirstWidget(widget);
		return this;
	}

	/**
	 * Fluent method to set the second (right/top) widget.
	 * @param widget the second widget
	 * @return this split pane for chaining
	 */
	public SplitPane second(final Widget widget) {
		setSecondWidget(widget);
		return this;
	}

	/**
	 * Fluent method to set left widget (alias for first in horizontal mode).
	 * @param widget the left widget
	 * @return this split pane for chaining
	 */
	public SplitPane left(final Widget widget) {
		return first(widget);
	}

	/**
	 * Fluent method to set right widget (alias for second in horizontal mode).
	 * @param widget the right widget
	 * @return this split pane for chaining
	 */
	public SplitPane right(final Widget widget) {
		return second(widget);
	}

	/**
	 * Fluent method to set bottom widget (alias for first in vertical mode).
	 * @param widget the bottom widget
	 * @return this split pane for chaining
	 */
	public SplitPane bottom(final Widget widget) {
		return first(widget);
	}

	/**
	 * Fluent method to set top widget (alias for second in vertical mode).
	 * @param widget the top widget
	 * @return this split pane for chaining
	 */
	public SplitPane top(final Widget widget) {
		return second(widget);
	}

	/**
	 * Fluent method to connect a split change callback.
	 * @param callback the callback to invoke when split position changes
	 * @return this split pane for chaining
	 */
	public SplitPane onSplitChange(final java.util.function.Consumer<Float> callback) {
		this.signalSplitChanged.connect(callback::accept);
		return this;
	}
}
