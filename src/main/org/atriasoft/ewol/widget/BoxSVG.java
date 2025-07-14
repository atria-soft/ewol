package org.atriasoft.ewol.widget;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension1f;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.ewol.compositing.CompositingSVG;
import org.atriasoft.ewol.event.EventTime;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class BoxSVG extends Container {
	private static final Logger LOGGER = LoggerFactory.getLogger(BoxSVG.class);
	protected CompositingSVG vectorialDraw = new CompositingSVG();
	
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
	protected static void periodicCall(final BoxSVG self, final EventTime event) {
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
	public BoxSVG() {}

	/**
	 * Constructor with his subWidget
	 */
	public BoxSVG(final Widget subWidget) {
		super(subWidget);
	}
	
	protected Dimension1f propertyBorderWidth = Dimension1f.ZERO;

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "border-width")
	@AknotDescription(value = "Border of the box")
	public Dimension1f getPropertyBorderWidth() {
		return this.propertyBorderWidth;
	}

	public void setPropertyBorderWidth(final Dimension1f propertyBorder) {
		if (this.propertyBorderWidth.equals(propertyBorder)) {
			return;
		}
		this.propertyBorderWidth = propertyBorder;
		markToRedraw();
		requestUpdateSize();
	}

	protected Dimension1f propertyBorderRadius = new Dimension1f(0);

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "border-radius")
	@AknotDescription(value = "Border radius of the box")
	public Dimension1f getPropertyBorderRadius() {
		return this.propertyBorderRadius;
	}

	public void setPropertyBorderRadius(final Dimension1f propertyBorderRadius) {
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
		final Vector2f borderSize = new Vector2f(this.propertyBorderWidth.size() * 2.0f,
				this.propertyBorderWidth.size() * 2.0f);
		final Vector2f padding = this.propertyPadding.size().multiply(2);
		final Vector2f margin = this.propertyMargin.size().multiply(2);
		final Vector2f calculatedBoxMinSize = childMinSize.add(margin).add(padding).add(borderSize);

		this.minSize = calculatedBoxMinSize;
		this.maxSize = Vector2f.max(this.minSize, this.propertyMaxSize.size());
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
		final Vector2f localPadding = this.propertyPadding.size();
		final Vector2f localMargin = this.propertyMargin.size();
		final float localBorderSize = this.propertyBorderWidth.size();
		final Vector2f offsetSubWidget = localPadding.add(localMargin).add(localBorderSize);
		
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
		
		final Vector2f freeSizeWithoutWidget = this.size.less(offsetSubWidget.multiply(2)).less(subWidgetSize);
		Vector2f subWidgetOrigin = this.origin.add(this.propertyGravity.gravityGenerateDelta(freeSizeWithoutWidget));
		subWidgetOrigin = subWidgetOrigin.add(offsetSubWidget);
		subWidgetOrigin = subWidgetOrigin.clipInteger();
		this.subWidget.setOrigin(subWidgetOrigin);
		this.subWidget.setSize(subWidgetSize);
		this.subWidget.onChangeSize();
	}
	
	protected Vector2i renderOrigin;
	protected Vector2i renderSize;

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
			return;
		}
		final Vector2f localMargin = this.propertyMargin.size();

		Vector2f tmpRenderSize = calculateSizeRendering();
		Vector2f tmpRenderOrigin = calculateOriginRendering(tmpRenderSize);

		tmpRenderOrigin = tmpRenderOrigin.add(localMargin);
		tmpRenderSize = tmpRenderSize.less(localMargin.multiply(2));
		// not sure this is needed...
		tmpRenderSize = tmpRenderSize.clipInteger();
		tmpRenderOrigin = tmpRenderOrigin.clipInteger();
		
		this.renderOrigin = new Vector2i((int) tmpRenderOrigin.x(), (int) tmpRenderOrigin.y());
		this.renderSize = new Vector2i((int) tmpRenderSize.x(), (int) tmpRenderSize.y());
		//System.out.println("renderSize: " + this.renderSize);
		// remove data of the previous composition :
		this.vectorialDraw.clear();
		final int borderSize = (int) this.propertyBorderWidth.size();
		final int paddingCompensateBorder = Math.round(borderSize * 0.5f);
		if (borderSize > 0.0f) {
			this.vectorialDraw.setRectangleBorderAsSource(this.renderSize.x(), this.renderSize.y(), this.propertyColor,
					borderSize, (int) this.propertyBorderRadius.size(), this.propertyBorderColor);
		} else {
			this.vectorialDraw.setRectangleAsSource(this.renderSize.x(), this.renderSize.y(), this.propertyColor);
		}
		this.vectorialDraw.setPos(this.renderOrigin);
		// For events:
		this.startPosition = this.renderOrigin;
		this.endPosition = this.renderOrigin.add(this.renderSize);
		this.vectorialDraw.print(this.renderSize);
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
