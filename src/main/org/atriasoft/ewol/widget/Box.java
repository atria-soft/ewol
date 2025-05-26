package org.atriasoft.ewol.widget;

import java.io.IOException;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.aknot.exception.AknotException;
import org.atriasoft.ejson.JsonMapper;
import org.atriasoft.ejson.exception.EjsonException;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension1f;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.etk.math.Vector3b;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.compositing.CompositingSVG;
import org.atriasoft.ewol.event.EventTime;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * @ingroup ewolWidgetGroup
 * Entry box display :
 *
 * ~~~~~~~~~~~~~~~~~~~~~~
 * 	-------------
 * 	|  Content  |
 * 	-------------
 * ~~~~~~~~~~~~~~~~~~~~~~
 */
public class Box extends Container {
	private static final Logger LOGGER = LoggerFactory.getLogger(Box.class);
	protected CompositingSVG compositing = new CompositingSVG();
	
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

	private final Uri propertyConfig = new Uri("THEME", "shape/Button.json", "ewol");
	
	Vector2i startPosition = Vector2i.ZERO;
	Vector2i endPosition = Vector2i.ZERO;

	public boolean isInside(final Vector3f value) {
		return value.x() > this.startPosition.x() //
				&& value.y() > this.startPosition.y() //
				&& value.x() < this.endPosition.x() //
				&& value.y() < this.endPosition.y();
	}
	
	/**
	 * Constructor
	 */
	public Box() {
		updateBasicConfig();
	}

	/**
	 * Constructor with his subWidget
	 */
	public Box(final Widget subWidget) {
		super(subWidget);
		updateBasicConfig();
	}
	
	private void updateBasicConfig() {
		
		final JsonMapper mapper = new JsonMapper();
		try {
			final BoxParameter parameters = mapper.read(BoxParameter.class, this.propertyConfig);
			// TODO ...
		} catch (EjsonException | AknotException | IOException e) {
			e.printStackTrace();
		}
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

		Vector3f subWidgetSize = this.subWidget.getCalculateMinSize();
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
		if (this.subWidget.canExpand().z() && this.propertyFill.z()) {
			subWidgetSize = subWidgetSize.withZ(this.size.z());
		} else {
			subWidgetSize = subWidgetSize.withZ(this.minSize.z());
		}
		subWidgetSize = subWidgetSize.less(offsetSubWidget.x(), offsetSubWidget.y(), 0);
		subWidgetSize = subWidgetSize.clipInteger();
		
		// set config to the Sub-widget
		//Vector3f subWidgetOrigin = this.origin.add(this.size.less(subWidgetSize).multiply(0.5f));
		Vector3f subWidgetOrigin = this.origin
				.add(this.propertyGravity.gravityGenerateDelta(this.size.less(subWidgetSize))).add(50);
		// NOTE le add 150 est pour un pb de test ==> a nlever en prod ...
		subWidgetOrigin = subWidgetOrigin.clipInteger();
		
		this.subWidget.setOrigin(subWidgetOrigin);
		this.subWidget.setSize(subWidgetSize);
		this.subWidget.onChangeSize();
	}
	
	protected Vector2i renderOrigin;
	protected Vector2i renderSize;

	@Override
	public void onRegenerateDisplay() {
		super.onRegenerateDisplay();
		if (!needRedraw()) {
			//return;
		}
		final Vector2f localMargin = this.propertyMargin.size();

		final Vector3f minSizeWithoutMargin = this.minSize.less(localMargin.x() * 2, localMargin.y() * 2, 0);
		Vector3f tmpRenderSize = minSizeWithoutMargin;
		Vector3f tmpRenderOrigin = this.propertyGravity.gravityGenerateDelta(this.size.less(minSizeWithoutMargin));
		if (this.propertyFill.x()) {
			tmpRenderSize = tmpRenderSize.withX(this.size.x());
			tmpRenderOrigin = tmpRenderOrigin.withX(0.0f);
		}
		if (this.propertyFill.y()) {
			tmpRenderSize = tmpRenderSize.withY(this.size.y());
			tmpRenderOrigin = tmpRenderOrigin.withY(0.0f);
		}
		if (this.propertyFill.z()) {
			tmpRenderSize = tmpRenderSize.withZ(this.size.y());
			tmpRenderOrigin = tmpRenderOrigin.withZ(0.0f);
		}
		// not sure this is needed...
		tmpRenderSize = tmpRenderSize.clipInteger();
		tmpRenderOrigin = tmpRenderOrigin.clipInteger();
		
		this.renderOrigin = new Vector2i((int) tmpRenderOrigin.x(), (int) tmpRenderOrigin.y());
		this.renderSize = new Vector2i((int) tmpRenderSize.x(), (int) tmpRenderSize.y());
		// remove data of the previous composition :
		this.compositing.clear();
		final int borderSize = (int) this.propertyBorderWidth.size();
		final int paddingCompensateBorder = Math.round(borderSize * 0.5f);

		//		this.renderSize = new Vector2i((int) (this.size.x() - this.propertyMargin.size().x() * 2),
		//				(int) (this.size.y() - this.propertyMargin.size().y() * 2));
		// Bug intéressant: la parsing de la couleur est foireux, black et #000000FF ne rend pas la même chose ==> pour ètre plus précs le rendu avec alpha est foireux...
		if (borderSize > 0.0f) {
			this.compositing.setSource("""
					<svg>
					  <rect
					    x="%d"
					    y="%d"
					    width="%d"
					    height="%d"
					    rx="%d"
					    ry="%d"
					    fill="%s"
					    stroke="%s"
					    stroke-width="%d"
					  />
					</svg>""".formatted( //
					paddingCompensateBorder, paddingCompensateBorder, //
					this.renderSize.x() - 2 * paddingCompensateBorder,
					this.renderSize.y() - 2 * paddingCompensateBorder, //
					(int) this.propertyBorderRadius.size(), //
					(int) this.propertyBorderRadius.size(), //
					this.propertyColor.toStringSharp(), //
					this.propertyBorderColor.toStringSharp(), //
					borderSize //
			), //
					this.renderSize);
		} else {
			this.compositing.setSource("""
					<svg>
					  <rect
					    x="%d"
					    y="%d"
					    width="%d"
					    height="%d"
					    fill="%s"
					  />
					</svg>""".formatted( //
					paddingCompensateBorder, paddingCompensateBorder, //
					this.renderSize.x() - 2 * paddingCompensateBorder,
					this.renderSize.y() - 2 * paddingCompensateBorder, //
					this.propertyColor.toStringSharp() //
			), //
					this.renderSize);
		}
		this.compositing.setPos(this.renderOrigin);
		// For events:
		this.startPosition = this.renderOrigin;
		this.endPosition = this.renderOrigin.add(this.renderSize);
		this.compositing.print(this.renderSize);
		//		LOGGER.debug("propertyBorderColor=" + this.propertyBorderColor.toStringSharp());
		//		LOGGER.debug("Paint Image at : " + this.origin + " size=" + this.size);
		//		LOGGER.debug("minSize: " + this.minSize + " size=" + this.size);
		this.compositing.flush();
	}

	public void onChangeSize_____sdfgsdfqsdfqsdfqsdfsqdfqsdfqsdfsqdfsqdfqsdfqdfsqdfqsdfqsdfqsdfqsdfsqfgsdfg() {
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

		final Vector2f localPadding = this.propertyPadding.size();
		final Vector2f localMargin = this.propertyMargin.size();
		final float localBorderSize = this.propertyBorderWidth.size();
		final Vector2f offsetSubWidget = localPadding.add(localMargin).add(localBorderSize);
		this.subWidget.setOrigin(origin.add(offsetSubWidget.x(), offsetSubWidget.y(), 0.0f));
		this.subWidget.setSize(this.size.less(offsetSubWidget.x() * 2, offsetSubWidget.y() * 2, 0.0f));
		this.subWidget.onChangeSize();
	}

	@Override
	public void calculateMinMaxSize() {
		super.calculateMinMaxSize();
		final Vector2f childMinSize = new Vector2f(this.minSize.x(), this.minSize.y());
		//final Vector2f parentMaxSize = new Vector2f(this.maxSize.x(), this.maxSize.y());

		LOGGER.debug("calculate min size: border=" + this.propertyBorderWidth + " min-size=" + this.propertyMinSize);
		final Vector2f borderSize = new Vector2f(this.propertyBorderWidth.size() * 2.0f,
				this.propertyBorderWidth.size() * 2.0f);
		final Vector2f padding = this.propertyPadding.size().multiply(2)
				.max(new Vector2f(this.propertyBorderRadius.size() * 2 + this.propertyBorderWidth.size(),
						this.propertyBorderRadius.size() * 2 + this.propertyBorderWidth.size()));
		final Vector2f margin = this.propertyMargin.size().multiply(2);
		final Vector3f minSize = this.propertyMinSize.size();
		final Vector2f borderMinSize = childMinSize.add(margin).add(padding).add(borderSize);

		final Vector2f calculatedBoxMinSize = Vector2f.max(borderMinSize, new Vector2f(minSize.x(), minSize.y()));
		// LOGGER.debug("set widget min=" + this.minSize + " max=" + this.maxSize);
		this.minSize = new Vector3f(calculatedBoxMinSize.x(), calculatedBoxMinSize.y(), 0);
		this.maxSize = Vector3f.max(this.minSize, this.propertyMaxSize.size());
		markToRedraw();
	}
	
	@Override
	protected void onDraw() {
		if (this.compositing != null) {
			this.compositing.draw(true);
		}
		super.onDraw();
	}

}
