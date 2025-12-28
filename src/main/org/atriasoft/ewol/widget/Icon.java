/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.aknot.annotation.AknotSignal;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.ewol.compositing.CompositingSVG;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.gale.key.KeyStatus;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Icon widget that displays SVG icons with customizable fill and stroke colors.
 *
 * Icons should be stored in theme/icon/ with:
 * - fill:#FFFFFF (white) for areas to be colored with fillColor
 * - stroke:#000000 (black) for strokes to be colored with strokeColor
 *
 * Example usage:
 * <pre>
 * Icon.create("Home")
 *     .fill(Color.RED)
 *     .stroke(Color.BLACK)
 *     .size(new Dimension2f(32, 32))
 *     .onPressed(() -> navigateHome());
 * </pre>
 */
public class Icon extends Widget {
	private static final Logger LOGGER = LoggerFactory.getLogger(Icon.class);

	/** Compositing for SVG rendering */
	private final CompositingSVG compositing = new CompositingSVG();

	/** Source URI of the icon SVG */
	private Uri propertySource = null;

	/** Icon name (simple name like "Home", "Search", etc.) */
	private String propertyIcon = null;

	/** Fill color (replaces #FFFFFF in SVG) */
	private Color propertyFillColor = Color.WHITE;

	/** Stroke color (replaces #000000 in SVG) */
	private Color propertyStrokeColor = Color.BLACK;

	/** Icon display size */
	private Dimension2f propertyIconSize = new Dimension2f(new Vector2f(24f, 24f));

	/** Cached SVG data */
	private String cachedSvgData = null;

	/** Cached colored SVG data */
	private String cachedColoredSvgData = null;

	@AknotSignal
	@AknotName("pressed")
	@AknotDescription("Icon is pressed")
	public final SignalEmpty signalPressed = new SignalEmpty();

	/**
	 * Default constructor.
	 */
	public Icon() {
		setMouseLimit(1);
	}

	// ========================================================================
	// Property accessors
	// ========================================================================

	@AknotManaged
	@AknotAttribute
	@AknotName("src")
	@AknotDescription("Icon source URI")
	public Uri getPropertySource() {
		return this.propertySource;
	}

	public void setPropertySource(final Uri source) {
		if (this.propertySource != null && this.propertySource.equals(source)) {
			return;
		}
		this.propertySource = source;
		this.propertyIcon = null; // Clear icon name when setting source directly
		this.cachedSvgData = null;
		this.cachedColoredSvgData = null;
		markToRedraw();
		requestUpdateSize();
	}

	@AknotManaged
	@AknotAttribute
	@AknotName("icon")
	@AknotDescription("Icon name (from theme/icon/ directory, without .svg extension)")
	public String getPropertyIcon() {
		return this.propertyIcon;
	}

	public void setPropertyIcon(final String iconName) {
		if (this.propertyIcon != null && this.propertyIcon.equals(iconName)) {
			return;
		}
		this.propertyIcon = iconName;
		if (iconName != null && !iconName.isEmpty()) {
			this.propertySource = new Uri("THEME", "icon/" + iconName + ".svg", "ewol");
		} else {
			this.propertySource = null;
		}
		this.cachedSvgData = null;
		this.cachedColoredSvgData = null;
		markToRedraw();
		requestUpdateSize();
	}

	@AknotManaged
	@AknotAttribute
	@AknotName("fill-color")
	@AknotDescription("Fill color for the icon")
	public Color getPropertyFillColor() {
		return this.propertyFillColor;
	}

	public void setPropertyFillColor(final Color color) {
		if (this.propertyFillColor.equals(color)) {
			return;
		}
		this.propertyFillColor = color;
		this.cachedColoredSvgData = null;
		markToRedraw();
	}

	@AknotManaged
	@AknotAttribute
	@AknotName("stroke-color")
	@AknotDescription("Stroke color for the icon")
	public Color getPropertyStrokeColor() {
		return this.propertyStrokeColor;
	}

	public void setPropertyStrokeColor(final Color color) {
		if (this.propertyStrokeColor.equals(color)) {
			return;
		}
		this.propertyStrokeColor = color;
		this.cachedColoredSvgData = null;
		markToRedraw();
	}

	@AknotManaged
	@AknotAttribute
	@AknotName("icon-size")
	@AknotDescription("Display size of the icon")
	public Dimension2f getPropertyIconSize() {
		return this.propertyIconSize;
	}

	public void setPropertyIconSize(final Dimension2f size) {
		if (this.propertyIconSize.equals(size)) {
			return;
		}
		this.propertyIconSize = size;
		markToRedraw();
		requestUpdateSize();
	}

	// ========================================================================
	// Color replacement
	// ========================================================================

	/**
	 * Apply fill and stroke colors to SVG data by replacing reference colors.
	 * @param svgData Original SVG data
	 * @return SVG data with colors replaced
	 */
	private String applyColors(final String svgData) {
		if (svgData == null) {
			return null;
		}
		String result = svgData;
		final String fillHex = colorToHex(this.propertyFillColor);
		final String strokeHex = colorToHex(this.propertyStrokeColor);

		// Replace white (fill reference) with fill color
		result = result.replace("#FFFFFF", fillHex);
		result = result.replace("#ffffff", fillHex);
		result = result.replace("#FFF", fillHex);
		result = result.replace("#fff", fillHex);

		// Replace black (stroke reference) with stroke color
		result = result.replace("#000000", strokeHex);
		result = result.replace("#000", strokeHex);

		return result;
	}

	/**
	 * Convert a Color to hexadecimal string.
	 * @param color Color to convert
	 * @return Hex string like "#RRGGBB"
	 */
	private String colorToHex(final Color color) {
		return String.format("#%02X%02X%02X",
				(int) (color.r() * 255),
				(int) (color.g() * 255),
				(int) (color.b() * 255));
	}

	// ========================================================================
	// Widget lifecycle
	// ========================================================================

	@Override
	public void calculateMinMaxSize() {
		final Vector2f iconSize = this.propertyIconSize.getPixel();
		this.minSize = iconSize;
		this.maxSize = this.propertyMaxSize.getPixel();
		this.maxSize = Vector2f.max(this.maxSize, this.minSize);
		LOGGER.trace("Icon min size = {}", this.minSize);
	}

	@Override
	public void onRegenerateDisplay() {
		if (!needRedraw()) {
			return;
		}

		this.compositing.clear();

		if (this.propertySource == null) {
			return;
		}

		// Load SVG data if not cached
		if (this.cachedSvgData == null) {
			this.cachedSvgData = Uri.getAllDataString(this.propertySource);
			this.cachedColoredSvgData = null;
		}

		if (this.cachedSvgData == null) {
			LOGGER.warn("Failed to load icon: {}", this.propertySource);
			return;
		}

		// Apply colors if not cached
		if (this.cachedColoredSvgData == null) {
			this.cachedColoredSvgData = applyColors(this.cachedSvgData);
		}

		// Calculate position and size
		final Vector2f iconSize = this.propertyIconSize.getPixel();
		final Vector2i renderSize = iconSize.toVector2i();

		// Center the icon in the widget
		final Vector2f delta = this.propertyGravity.gravityGenerateDelta(this.size.less(iconSize));

		this.compositing.setSource(this.cachedColoredSvgData, renderSize);
		this.compositing.setPos(delta);
		this.compositing.print(iconSize);
		this.compositing.flush();
	}

	@Override
	protected void onDraw() {
		this.compositing.draw(true);
	}

	@Override
	public boolean onEventInput(final EventInput event) {
		if (event.inputId() == 1 && event.status() == KeyStatus.pressSingle) {
			final Vector2f relPos = relativePosition(event.pos());
			// Check if click is inside widget bounds
			if (relPos.x() >= 0 && relPos.y() >= 0
					&& relPos.x() < this.size.x() && relPos.y() < this.size.y()) {
				this.signalPressed.emit();
				return true;
			}
		}
		return false;
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	/**
	 * Create a new Icon from the theme/icon/ directory.
	 * @param iconName Name of the icon (without .svg extension)
	 * @return A new Icon instance
	 */
	public static Icon create(final String iconName) {
		final Icon icon = new Icon();
		icon.setPropertySource(new Uri("THEME", "icon/" + iconName + ".svg", "ewol"));
		return icon;
	}

	/**
	 * Create a new Icon from a custom URI.
	 * @param source URI of the SVG icon
	 * @return A new Icon instance
	 */
	public static Icon create(final Uri source) {
		final Icon icon = new Icon();
		icon.setPropertySource(source);
		return icon;
	}

	/**
	 * Set the icon by name (from theme/icon/).
	 * @param iconName Name of the icon
	 * @return This icon for chaining
	 */
	public Icon icon(final String iconName) {
		setPropertySource(new Uri("THEME", "icon/" + iconName + ".svg", "ewol"));
		return this;
	}

	/**
	 * Set the icon source URI.
	 * @param source URI of the SVG icon
	 * @return This icon for chaining
	 */
	public Icon source(final Uri source) {
		setPropertySource(source);
		return this;
	}

	/**
	 * Set the fill color.
	 * @param color Fill color
	 * @return This icon for chaining
	 */
	public Icon fill(final Color color) {
		setPropertyFillColor(color);
		return this;
	}

	/**
	 * Set the stroke color.
	 * @param color Stroke color
	 * @return This icon for chaining
	 */
	public Icon stroke(final Color color) {
		setPropertyStrokeColor(color);
		return this;
	}

	/**
	 * Set both fill and stroke colors.
	 * @param fillColor Fill color
	 * @param strokeColor Stroke color
	 * @return This icon for chaining
	 */
	public Icon color(final Color fillColor, final Color strokeColor) {
		setPropertyFillColor(fillColor);
		setPropertyStrokeColor(strokeColor);
		return this;
	}

	/**
	 * Set the icon display size.
	 * @param size Icon size
	 * @return This icon for chaining
	 */
	public Icon size(final Dimension2f size) {
		setPropertyIconSize(size);
		return this;
	}

	/**
	 * Connect a callback to the pressed signal.
	 * @param callback Callback to invoke when icon is pressed
	 * @return This icon for chaining
	 */
	public Icon onPressed(final Runnable callback) {
		this.signalPressed.connect(callback);
		return this;
	}
}
