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

	/**
	 * Periodic call to update grapgic display
	 * @param event Time generic event
	 */
	protected static void periodicCall(final Box self, final EventTime event) {
		LOGGER.trace("Periodic call on Entry(" + event + ")");
		self.markToRedraw();
	}
	
	//private Uri propertyConfig = new Uri("THEME", "shape/Button.json", "ewol");

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
		final Vector2f parentMinSize = new Vector2f(this.minSize.x(), this.minSize.y());
		final Vector2f parentMaxSize = new Vector2f(this.maxSize.x(), this.maxSize.y());
		
		LOGGER.debug("calculate min size: border=" + this.propertyBorderWidth + " min-size=" + this.propertyMinSize);
		final Vector2f borderSize = new Vector2f(this.propertyBorderWidth.size() * 2.0f,
				this.propertyBorderWidth.size() * 2.0f);
		final Vector2f padding = this.propertyPadding.size();
		final Vector2f margin = this.propertyMargin.size();
		final Vector3f minSize = this.propertyMinSize.size();
		final Vector2f borderMinSize = parentMinSize.add(margin).add(padding).add(borderSize);
		
		final Vector2f calculatedBoxMinSize = Vector2f.max(borderMinSize, new Vector2f(minSize.x(), minSize.y()));
		
		this.minSize = new Vector3f(calculatedBoxMinSize.x(), calculatedBoxMinSize.y(), 0);
		this.maxSize = Vector3f.max(this.minSize, this.propertyMaxSize.size());
		LOGGER.debug("set widget min=" + this.minSize + " max=" + this.maxSize);
		markToRedraw();
	}

	@Override
	protected void onDraw() {
		if (this.compositing != null) {
			this.compositing.draw(true);
		}
		super.onDraw();
	}
	
	@Override
	public void onRegenerateDisplay() {
		super.onRegenerateDisplay();
		if (!needRedraw()) {
			//return;
		}
		// remove data of the previous composition :
		this.compositing.clear();
		final int borderSize = (int) this.propertyBorderWidth.size();
		final int paddingCompensateBorder = Math.round(borderSize * 0.5f);
		
		final Vector2i renderSize = new Vector2i((int) (this.size.x() - this.propertyMargin.size().x() * 2),
				(int) (this.size.y() - this.propertyMargin.size().y() * 2));
		final long startTime = System.nanoTime();
		/*
		final EsvgDocument doc = new EsvgDocument();
		doc.addElement();
		final Rectangle rect = new Rectangle(//
				new Vector2f(paddingCompensateBorder, paddingCompensateBorder), //
				new Vector2f(paddingCompensateBorder, paddingCompensateBorder), //

				)
		==> render is OK
		this.compositing.setSource("""
				<svg>
				  <rect
				    x="%d"
				    y="%d"
				    width="%dpx"
				    height="%dpx"
				    rx="%dpx"
				    ry="%dpx"
				    fill="white"
				    stroke="black"
				    stroke-width="%dpx"
				  />
				</svg>""".formatted( //
				paddingCompensateBorder, paddingCompensateBorder, //
				renderSize.x() - 2 * paddingCompensateBorder, renderSize.y() - 2 * paddingCompensateBorder, //
				(int) this.propertyBorderRadius.size(), //
				200, //
				//this.propertyColor.toStringSharp(), //
				//this.propertyBorderColor.toStringSharp(), //
				borderSize //
		), //
				renderSize);
		*/

		// Bug intéressant: la parsing de la couleur est foireux, black et #000000FF ne rend pas la même chose ==> pour ètre plus précs le rendu avec alpha est foireux...
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
				renderSize.x() - 2 * paddingCompensateBorder, renderSize.y() - 2 * paddingCompensateBorder, //
				(int) this.propertyBorderRadius.size(), //
				(int) this.propertyBorderRadius.size(), //
				this.propertyColor.toStringSharp(), //
				this.propertyBorderColor.toStringSharp(), //
				borderSize //
		), //
				renderSize);
		final Vector2f imageRenderSize = new Vector2f(100, 100);
		final long endTime = System.nanoTime();
		
		// ca ca ne devrait pas ètre la ...
		Vector3f delta = this.propertyGravity
				.gravityGenerateDelta(this.size.less(imageRenderSize.x(), imageRenderSize.y(), 0));
		//LOGGER.debug("delta : " + delta);
		if (this.propertyFill.x()) {
			//imageRealSize = imageRealSize.withX(imageRealSizeMax.x());
			delta = delta.withX(0.0f);
		}
		if (this.propertyFill.y()) {
			//imageRealSize = imageRealSize.withY(imageRealSizeMax.y());
			delta = delta.withY(0.0f);
		}
		//this.origin = this.origin.add(delta);
		//this.origin = Vector3f.ZERO;
		this.compositing.setPos(this.propertyMargin.size());
		this.compositing.print(renderSize);
		//LOGGER.debug("generate image in : " + (endTime - startTime));
		//		LOGGER.debug("propertyBorderColor=" + this.propertyBorderColor.toStringSharp());
		//		LOGGER.debug("Paint Image at : " + this.origin + " size=" + this.size);
		//		LOGGER.debug("minSize: " + this.minSize + " size=" + this.size);
		this.compositing.flush();
	}
}
