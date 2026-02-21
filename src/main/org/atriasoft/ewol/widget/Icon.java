/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.dataformat.xml.annotation.JacksonXmlProperty;
import org.atriasoft.esignal.Connection;
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
 * Icon widget that displays SVG icons with customizable fill and background colors.
 *
 * The colors are applied by replacing reference colors in the SVG:
 * - Black colors (#000, #000000, black) are replaced with fillColor
 * - White colors (#FFF, #FFFFFF, white) are replaced with backgroundColor
 *
 * Example usage:
 * <pre>
 * Icon.create("Home")
 *     .fill(Color.RED)
 *     .background(Color.TRANSPARENT)
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
	
	/** Fill color (replaces black colors in SVG) */
	private Color propertyFillColor = Color.WHITE;
	
	/** Background color (replaces white colors in SVG) */
	private Color propertyBackgroundColor = Color.BLACK;
	
	/** Icon display size */
	private Dimension2f propertyIconSize = new Dimension2f(new Vector2f(24f, 24f));
	
	/** Cached SVG data */
	private String cachedSvgData = null;
	
	/** Cached colored SVG data */
	private String cachedColoredSvgData = null;
	
	public final SignalEmpty signalPressed = new SignalEmpty();

	/** Stored connections from fluent API to prevent GC */
	private final List<Connection> fluentConnections = new ArrayList<>();
	
	/**
	 * Default constructor.
	 */
	public Icon() {
		setMouseLimit(1);
	}
	
	// ========================================================================
	// Property accessors
	// ========================================================================
	
	@JsonProperty("src")
	@JacksonXmlProperty(isAttribute = true, localName = "src")
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
	
	@JsonProperty("icon")
	@JacksonXmlProperty(isAttribute = true, localName = "icon")
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
	
	@JsonProperty("fill-color")
	@JacksonXmlProperty(isAttribute = true, localName = "fill-color")
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
	
	@JsonProperty("background-color")
	@JacksonXmlProperty(isAttribute = true, localName = "background-color")
	public Color getPropertyBackgroundColor() {
		return this.propertyBackgroundColor;
	}
	
	public void setPropertyBackgroundColor(final Color color) {
		if (this.propertyBackgroundColor.equals(color)) {
			return;
		}
		this.propertyBackgroundColor = color;
		this.cachedColoredSvgData = null;
		markToRedraw();
	}
	
	@JsonProperty("icon-size")
	@JacksonXmlProperty(isAttribute = true, localName = "icon-size")
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
	 * Regex matching color attribute values that are black or white.
	 * Captures: attribute name (group 1), color value (group 2).
	 * Handles: fill="black", stroke="#000000", fill="#FFF", etc.
	 */
	private static final Pattern COLOR_ATTR_PATTERN = Pattern.compile(
			"((?:fill|stroke|stop-color|flood-color|lighting-color)\\s*[=:]\\s*[\"']?)"
			+ "(#000(?:000)?|#fff(?:fff)?|black|white)",
			Pattern.CASE_INSENSITIVE);

	/**
	 * Apply fill and background colors to SVG data using string replacement.
	 * - Black colors (#000, #000000, black) are replaced with fillColor
	 * - White colors (#FFF, #FFFFFF, white) are replaced with backgroundColor
	 * @param svgData Original SVG data
	 * @return SVG data with colors applied, or null if input is null
	 */
	private String applyColors(final String svgData) {
		if (svgData == null) {
			return null;
		}
		final String fillHex = colorToHex(this.propertyFillColor);
		final String backgroundHex = colorToHex(this.propertyBackgroundColor);

		final Matcher matcher = COLOR_ATTR_PATTERN.matcher(svgData);
		final StringBuilder result = new StringBuilder();
		while (matcher.find()) {
			final String colorValue = matcher.group(2).toLowerCase();
			final String replacement;
			if ("#000".equals(colorValue) || "#000000".equals(colorValue) || "black".equals(colorValue)) {
				replacement = matcher.group(1) + fillHex;
			} else {
				replacement = matcher.group(1) + backgroundHex;
			}
			matcher.appendReplacement(result, Matcher.quoteReplacement(replacement));
		}
		matcher.appendTail(result);
		return result.toString();
	}

	/**
	 * Convert a Color to hexadecimal string.
	 * @param color Color to convert
	 * @return Hex string like "#RRGGBB"
	 */
	private String colorToHex(final Color color) {
		return String.format("#%02X%02X%02X", (int) (color.r() * 255), (int) (color.g() * 255),
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
			if (relPos.x() >= 0 && relPos.y() >= 0 && relPos.x() < this.size.x() && relPos.y() < this.size.y()) {
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
	 * Set the background color.
	 * @param color Background color
	 * @return This icon for chaining
	 */
	public Icon background(final Color color) {
		setPropertyBackgroundColor(color);
		return this;
	}
	
	/**
	 * Set both fill and background colors.
	 * @param fillColor Fill color
	 * @param backgroundColor Background color
	 * @return This icon for chaining
	 */
	public Icon color(final Color fillColor, final Color backgroundColor) {
		setPropertyFillColor(fillColor);
		setPropertyBackgroundColor(backgroundColor);
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
		this.fluentConnections.add(this.signalPressed.connect(callback));
		return this;
	}
}
