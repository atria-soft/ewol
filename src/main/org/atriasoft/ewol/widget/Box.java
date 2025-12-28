package org.atriasoft.ewol.widget;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.DimensionBorderRadius;
import org.atriasoft.etk.DimensionInsets;
import org.atriasoft.etk.Insets;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.compositing.CompositingGC;
import org.atriasoft.ewol.event.EventTime;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class Box extends Container {
	private static final Logger LOGGER = LoggerFactory.getLogger(Box.class);
	protected CompositingGC vectorialDraw = new CompositingGC();

	public static class BoxParameter {
		public Float margin;
		public Float padding;
		public Float borderWidth;
		public Float borderRadius;
		public String borderColor;
		public String color;
	}

	/**
	 * Periodic call to update grapgic display
	 * @param event Time generic event
	 */
	protected static void periodicCall(final Box self, final EventTime event) {
		LOGGER.trace("Periodic call on Entry({})", event);
		self.markToRedraw();
	}
	
	Vector2f overPositionStart = Vector2f.ZERO;
	Vector2f overPositionStop = Vector2f.ZERO;
	Vector2f insidePositionStart = Vector2f.ZERO;
	Vector2f insidePositionStop = Vector2f.ZERO;
	
	public boolean isInside(final Vector2f value) {
		return value.x() > this.overPositionStart.x() //
				&& value.y() > this.overPositionStart.y() //
				&& value.x() < this.overPositionStop.x() //
				&& value.y() < this.overPositionStop.y();
	}

	/**
	 * Constructor
	 */
	public Box() {}
	
	/**
	 * Constructor with his subWidget
	 */
	public Box(final Widget subWidget) {
		super(subWidget);
	}

	protected DimensionInsets propertyBorderWidth = DimensionInsets.ZERO;
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "border-width")
	@AknotDescription(value = "Border of the box")
	public DimensionInsets getPropertyBorderWidth() {
		return this.propertyBorderWidth;
	}
	
	public void setPropertyBorderWidth(final DimensionInsets propertyBorder) {
		if (this.propertyBorderWidth.equals(propertyBorder)) {
			return;
		}
		this.propertyBorderWidth = propertyBorder;
		markToRedraw();
		requestUpdateSize();
	}
	
	protected DimensionBorderRadius propertyBorderRadius = DimensionBorderRadius.ZERO;
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "border-radius")
	@AknotDescription(value = "Border radius of the box")
	public DimensionBorderRadius getPropertyBorderRadius() {
		return this.propertyBorderRadius;
	}
	
	public void setPropertyBorderRadius(final DimensionBorderRadius propertyBorderRadius) {
		if (this.propertyBorderRadius.equals(propertyBorderRadius)) {
			return;
		}
		this.propertyBorderRadius = propertyBorderRadius;
		markToRedraw();
		requestUpdateSize();
	}
	
	protected Color propertyBorderColor = Color.NONE;

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "border-color")
	@AknotDescription(value = "Border color of the box")
	public Color getPropertyBorderColor() {
		return this.propertyBorderColor;
	}
	
	public void setPropertyBorderColor(final Color propertyBorderColor) {
		if (this.propertyBorderColor.equals(propertyBorderColor)) {
			return;
		}
		this.propertyBorderColor = propertyBorderColor;
		markToRedraw();
		requestUpdateSize();
	}

	protected Color propertyColor = Color.NONE;

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "color")
	@AknotDescription(value = "Border color of the box")
	public Color getPropertyColor() {
		return this.propertyColor;
	}
	
	public void setPropertyColor(final Color propertyColor) {
		if (this.propertyColor.equals(propertyColor)) {
			return;
		}
		this.propertyColor = propertyColor;
		markToRedraw();
		requestUpdateSize();
	}

	protected DimensionInsets propertyMargin = DimensionInsets.ZERO;
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "margin")
	@AknotDescription(value = "margin of the box")
	public DimensionInsets getPropertyMargin() {
		return this.propertyMargin;
	}
	
	public void setPropertyMargin(final DimensionInsets propertyMargin) {
		if (this.propertyMargin.equals(propertyMargin)) {
			return;
		}
		this.propertyMargin = propertyMargin;
		markToRedraw();
		requestUpdateSize();
	}

	protected DimensionInsets propertyPadding = DimensionInsets.ZERO;
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "padding")
	@AknotDescription(value = "Padding of the box")
	public DimensionInsets getPropertyPadding() {
		return this.propertyPadding;
	}
	
	public void setPropertyPadding(final DimensionInsets propertyPadding) {
		if (this.propertyPadding.equals(propertyPadding)) {
			return;
		}
		this.propertyPadding = propertyPadding;
		markToRedraw();
		requestUpdateSize();
	}

	protected void calculateMinMaxSizeChild(Vector2f childMinSize) {
		super.calculateMinMaxSize();
		childMinSize = this.minSize.max(childMinSize);
		LOGGER.trace("calculate min size: border={}", this.propertyBorderWidth);
		final Insets borderSize = this.propertyBorderWidth.getPixel();
		
		final Insets padding = this.propertyPadding.getPixel();
		final Insets margin = this.propertyMargin.getPixel();
		
		final Vector2f calculatedBoxMinSize = childMinSize.add(margin.toVector2f()).add(padding.toVector2f())
				.add(borderSize.toVector2f());
		
		this.minSize = calculatedBoxMinSize;
		this.maxSize = Vector2f.max(this.minSize, this.propertyMaxSize.getPixel());
		markToRedraw();
	}
	
	@Override
	public void calculateMinMaxSize() {
		calculateMinMaxSizeChild(Vector2f.ZERO);
	}
	
	public Insets getBorderAggregation() {
		final Insets localPadding = this.propertyPadding.getPixel();
		final Insets localMargin = this.propertyMargin.getPixel();
		final Insets localBorderSize = this.propertyBorderWidth.getPixel();
		return localPadding.add(localMargin).add(localBorderSize);
	}
	
	public Insets getBorderInsideAggregation() {
		final Insets localPadding = this.propertyPadding.getPixel();
		final Insets localBorderSize = this.propertyBorderWidth.getPixel();
		return localPadding.add(localBorderSize);
	}

	@Override
	public void onChangeSize() {
		markToRedraw();
		if (this.propertyHide) {
			return;
		}
		final Insets offsetSubWidget = getBorderAggregation();
		Vector2f subWidgetSize = Vector2f.ZERO;
		if (this.subWidget != null) {
			subWidgetSize = this.subWidget.getCalculateMinSize();
			if (this.subWidget.canExpand().x() && this.propertyFill.x()) {
				subWidgetSize = subWidgetSize.withX(this.size.x());
			} else {
				subWidgetSize = subWidgetSize.withX(this.minSize.x());
			}
			if (this.subWidget.canExpand().y() && this.propertyFill.y()) {
				subWidgetSize = subWidgetSize.withY(this.size.y());
			} else {
				subWidgetSize = subWidgetSize.withY(this.minSize.y());
			}
		} else {
			if (canExpand().x() && this.propertyFill.x()) {
				subWidgetSize = subWidgetSize.withX(this.size.x());
			} else {
				subWidgetSize = subWidgetSize.withX(this.minSize.x());
			}
			if (canExpand().y() && this.propertyFill.y()) {
				subWidgetSize = subWidgetSize.withY(this.size.y());
			} else {
				subWidgetSize = subWidgetSize.withY(this.minSize.y());
			}
		}
		subWidgetSize = subWidgetSize.less(offsetSubWidget.toVector2f());
		subWidgetSize = subWidgetSize.clipInteger();

		final Vector2f freeSizeWithoutWidget = this.size.less(offsetSubWidget.toVector2f()).less(subWidgetSize);
		this.insidePositionStart = this.origin.add(this.propertyGravity.gravityGenerateDelta(freeSizeWithoutWidget));
		this.insidePositionStart = this.insidePositionStart.add(offsetSubWidget.getOrigin());
		this.insidePositionStart = this.insidePositionStart.clipInteger();
		this.insidePositionStop = this.insidePositionStart.add(subWidgetSize);
		
		if (this.subWidget != null) {
			this.subWidget.setOrigin(this.insidePositionStart);
			this.subWidget.setSize(subWidgetSize);
			this.subWidget.onChangeSize();
		}
	}

	private Vector2f calculateOriginRendering(final Vector2f renderSize) {
		return this.propertyGravity.gravityGenerateDelta(this.size.less(renderSize));
	}
	
	private Vector2f calculateSizeRendering() {
		Vector2f tmpRenderSize = this.minSize;
		if (this.propertyFill.x()) {
			tmpRenderSize = tmpRenderSize.withX(this.size.x());
		}
		if (this.propertyFill.y()) {
			tmpRenderSize = tmpRenderSize.withY(this.size.y());
		}
		return tmpRenderSize;
	}
	
	@Override
	public void onRegenerateDisplay() {
		// Always regenerate children first (they may need redraw even if we don't)
		super.regenerateDisplay();

		if (!needRedraw()) {
			return;
		}
		// Regenerate our own display (box background, border, etc.)
		regenerateDisplay();
	}

	@Override
	public void regenerateDisplay() {
		// Note: super.regenerateDisplay() is called from onRegenerateDisplay() to ensure
		// children are always regenerated, even when Box itself doesn't need redraw.
		final Insets localMargin = this.propertyMargin.size();
		Vector2f renderSize = calculateSizeRendering();
		this.overPositionStart = calculateOriginRendering(renderSize);
		this.overPositionStart = this.overPositionStart.add(localMargin.getOrigin());
		renderSize = renderSize.less(localMargin.toVector2f());
		this.overPositionStart = this.overPositionStart.clipInteger();
		this.overPositionStop = this.overPositionStart.add(renderSize.clipInteger());
		final Insets offsetSubWidget = getBorderInsideAggregation();
		this.insidePositionStart = this.overPositionStart.add(offsetSubWidget.getOrigin());
		this.insidePositionStop = this.overPositionStop.less(offsetSubWidget.getEnd());

		// remove data of the previous composition:
		this.vectorialDraw.clear();
		this.vectorialDraw.setPaintFillColor(this.propertyColor);
		this.vectorialDraw.setPaintStrokeColor(this.propertyBorderColor);
		this.vectorialDraw.addRectangle(this.overPositionStart, this.overPositionStop,
				this.propertyBorderWidth.getPixel(), this.propertyBorderRadius.getPixel());
		//		this.vectorialDraw.setPaintFillColor(Color.RED);
		//		this.vectorialDraw.setPaintStrokeWidth(0);
		//		this.vectorialDraw.addRectangle(this.overPositionStart, Vector2f.VALUE_4);
		//		this.vectorialDraw.setPaintFillColor(Color.BLUE);
		//		this.vectorialDraw.addRectangle(this.overPositionStop.less(Vector2f.VALUE_4), Vector2f.VALUE_4);
		//		this.vectorialDraw.setPaintFillColor(Color.ORANGE);
		//		this.vectorialDraw.setPaintStrokeWidth(0);
		//		this.vectorialDraw.addRectangle(this.insidePositionStart, Vector2f.VALUE_4);
		//		this.vectorialDraw.setPaintFillColor(Color.PURPLE);
		//		this.vectorialDraw.addRectangle(this.insidePositionStop.less(Vector2f.VALUE_4), Vector2f.VALUE_4);
		this.vectorialDraw.flush();
	}
	
	@Override
	protected void onDraw() {
		if (this.vectorialDraw != null) {
			this.vectorialDraw.draw(true);
		}
		super.onDraw();
	}

	// ========================================================================
	// Fluent API methods
	// ========================================================================

	/**
	 * Fluent method to set border width.
	 * @param borderWidth the border width
	 * @return this box for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Box> T borderWidth(final DimensionInsets borderWidth) {
		setPropertyBorderWidth(borderWidth);
		return (T) this;
	}

	/**
	 * Fluent method to set border radius.
	 * @param borderRadius the border radius
	 * @return this box for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Box> T borderRadius(final DimensionBorderRadius borderRadius) {
		setPropertyBorderRadius(borderRadius);
		return (T) this;
	}

	/**
	 * Fluent method to set border color.
	 * @param color the border color
	 * @return this box for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Box> T borderColor(final Color color) {
		setPropertyBorderColor(color);
		return (T) this;
	}

	/**
	 * Fluent method to set background color.
	 * @param color the background color
	 * @return this box for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Box> T color(final Color color) {
		setPropertyColor(color);
		return (T) this;
	}

	/**
	 * Fluent method to set margin.
	 * @param margin the margin
	 * @return this box for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Box> T margin(final DimensionInsets margin) {
		setPropertyMargin(margin);
		return (T) this;
	}

	/**
	 * Fluent method to set padding.
	 * @param padding the padding
	 * @return this box for chaining
	 */
	@SuppressWarnings("unchecked")
	public <T extends Box> T padding(final DimensionInsets padding) {
		setPropertyPadding(padding);
		return (T) this;
	}
}
