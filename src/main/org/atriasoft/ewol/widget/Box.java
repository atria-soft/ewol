package org.atriasoft.ewol.widget;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.DimensionBorderRadius;
import org.atriasoft.etk.DimensionInsets;
import org.atriasoft.etk.Insets;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
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
		LOGGER.trace("Periodic call on Entry(" + event + ")");
		self.markToRedraw();
	}
	
	Vector2i startPosition = Vector2i.ZERO;
	Vector2i endPosition = Vector2i.ZERO;
	
	public boolean isInside(final Vector2f value) {
		return value.x() > this.startPosition.x() //
				&& value.y() > this.startPosition.y() //
				&& value.x() < this.endPosition.x() //
				&& value.y() < this.endPosition.y();
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

	protected Dimension2f propertyMargin = Dimension2f.ZERO;
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "margin")
	@AknotDescription(value = "margin of the box")
	public Dimension2f getPropertyMargin() {
		return this.propertyMargin;
	}
	
	public void setPropertyMargin(final Dimension2f propertyMargin) {
		if (this.propertyMargin.equals(propertyMargin)) {
			return;
		}
		this.propertyMargin = propertyMargin;
		markToRedraw();
		requestUpdateSize();
	}

	protected Dimension2f propertyPadding = Dimension2f.ZERO;
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "padding")
	@AknotDescription(value = "Padding of the box")
	public Dimension2f getPropertyPadding() {
		return this.propertyPadding;
	}
	
	public void setPropertyPadding(final Dimension2f propertyPadding) {
		if (this.propertyPadding.equals(propertyPadding)) {
			return;
		}
		this.propertyPadding = propertyPadding;
		markToRedraw();
		requestUpdateSize();
	}

	@Override
	public void calculateMinMaxSize() {
		super.calculateMinMaxSize();
		final Vector2f childMinSize = new Vector2f(this.minSize.x(), this.minSize.y());
		
		LOGGER.debug("calculate min size: border=" + this.propertyBorderWidth);
		final Insets borderSize = this.propertyBorderWidth.getPixel();
		
		final Vector2f padding = this.propertyPadding.getPixel().multiply(2);
		final Vector2f margin = this.propertyMargin.getPixel().multiply(2);
		
		final Vector2f calculatedBoxMinSize = childMinSize.add(margin).add(padding).add(borderSize.toVector2f());
		
		this.minSize = calculatedBoxMinSize;
		this.maxSize = Vector2f.max(this.minSize, this.propertyMaxSize.getPixel());
		markToRedraw();
	}

	@Override
	public void onChangeSize() {
		markToRedraw();
		if (this.propertyHide) {
			return;
		}
		if (this.subWidget == null) {
			return;
		}
		final Vector2f localPadding = this.propertyPadding.getPixel();
		final Vector2f localMargin = this.propertyMargin.getPixel();
		final Insets localBorderSize = this.propertyBorderWidth.getPixel();
		final Vector2f offsetSubWidget = localPadding.add(localMargin).add(localBorderSize.toVector2f());

		Vector2f subWidgetSize = this.subWidget.getCalculateMinSize();
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
		subWidgetSize = subWidgetSize.less(offsetSubWidget.multiply(2));
		subWidgetSize = subWidgetSize.clipInteger();

		on a un pb ici car on double les margin, ce qui est normal, mai on redouble les border size
		final Vector2f freeSizeWithoutWidget = this.size.less(offsetSubWidget.multiply(2)).less(subWidgetSize);
		Vector2f subWidgetOrigin = this.origin.add(this.propertyGravity.gravityGenerateDelta(freeSizeWithoutWidget));
		subWidgetOrigin = subWidgetOrigin.add(offsetSubWidget);
		subWidgetOrigin = subWidgetOrigin.clipInteger();
		this.subWidget.setOrigin(subWidgetOrigin);
		this.subWidget.setSize(subWidgetSize);
		this.subWidget.onChangeSize();
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
		super.onRegenerateDisplay();
		if (!needRedraw()) {
			//return;
		}
		final Vector2f localMargin = this.propertyMargin.size();
		
		Vector2f renderSize = calculateSizeRendering();
		Vector2f renderOrigin = calculateOriginRendering(renderSize);
		
		renderOrigin = renderOrigin.add(localMargin);
		renderSize = renderSize.less(localMargin.multiply(2));
		// not sure this is needed...
		renderSize = renderSize.clipInteger();
		renderOrigin = renderOrigin.clipInteger();
		
		renderOrigin = renderOrigin.clipInteger();
		renderSize = renderSize.clipInteger();
		this.startPosition = renderOrigin.toVector2i();
		this.endPosition = renderSize.toVector2i();
		
		// remove data of the previous composition:
		this.vectorialDraw.clear();
		this.vectorialDraw.setPaintFillColor(this.propertyColor);
		this.vectorialDraw.setPaintStrokeColor(this.propertyBorderColor);
		//this.vectorialDraw.setPaintStrokeWidth(borderSize);
		this.vectorialDraw.addRectangle(renderOrigin, renderSize, this.propertyBorderWidth.getPixel(),
				this.propertyBorderRadius.getPixel());
		this.vectorialDraw.flush();
	}
	
	@Override
	protected void onDraw() {
		if (this.vectorialDraw != null) {
			this.vectorialDraw.draw(true);
		}
		super.onDraw();
	}
	
}
