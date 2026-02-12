/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.aknot.annotation.AknotSignal;
import org.atriasoft.esignal.Connection;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.ewol.compositing.CompositingSVG;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.exml.Exml;
import org.atriasoft.exml.model.XmlElement;
import org.atriasoft.exml.model.XmlNode;
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
	
	@AknotSignal
	@AknotName("pressed")
	@AknotDescription("Icon is pressed")
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
	@AknotName("background-color")
	@AknotDescription("Background color for the icon (replaces white in SVG)")
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
	
	/** Black color values to replace with fillColor */
	private static final Set<String> BLACK_COLORS = Set.of("#000", "#000000", "black");
	
	/** White color values to replace with backgroundColor */
	private static final Set<String> WHITE_COLORS = Set.of("#fff", "#ffffff", "#FFF", "#FFFFFF", "white");
	
	/** Attributes that contain color values */
	private static final Set<String> COLOR_ATTRIBUTES = Set.of("fill", "stroke", "stop-color", "flood-color",
			"lighting-color");

	/** SVG elements that can have fill/stroke applied */
	private static final Set<String> SHAPE_ELEMENTS = Set.of("path", "circle", "ellipse", "rect", "polygon",
			"polyline", "line", "text", "tspan", "use");
	
	/**
	 * Apply fill and background colors to SVG data using XML parsing.
	 * - Black colors (#000, #000000, black) are replaced with fillColor
	 * - White colors (#FFF, #FFFFFF, white) are replaced with backgroundColor
	 * @param svgData Original SVG data
	 * @return SVG data with colors applied, or null if parsing fails
	 */
	private String applyColors(final String svgData) {
		if (svgData == null) {
			return null;
		}
		try {
			final XmlElement doc = Exml.parse(svgData);
			if (doc == null) {
				LOGGER.warn("Failed to parse SVG as XML");
				return svgData;
			}
			final String fillHex = colorToHex(this.propertyFillColor);
			final String backgroundHex = colorToHex(this.propertyBackgroundColor);
			
			// Process all elements recursively
			applyColorsToElement(doc, fillHex, backgroundHex);
			
			// Generate the modified XML
			final StringBuilder result = new StringBuilder();
			Exml.generate(doc, result);
			return result.toString();
		} catch (final Exception e) {
			LOGGER.warn("Failed to parse SVG for color replacement: {}", e.getMessage());
			return svgData;
		}
	}
	
	/**
	 * Recursively apply color replacements to an XML element and its children.
	 * @param element The XML element to process
	 * @param fillHex The hex color to replace black colors with
	 * @param backgroundHex The hex color to replace white colors with
	 */
	private void applyColorsToElement(final XmlElement element, final String fillHex, final String backgroundHex) {
		final String elementName = element.getValue() != null ? element.getValue().toLowerCase() : "";
		final boolean isShapeElement = SHAPE_ELEMENTS.contains(elementName);

		// Process color attributes on this element
		for (final String attrName : COLOR_ATTRIBUTES) {
			if (element.existAttribute(attrName)) {
				final String value = element.getAttribute(attrName, "").toLowerCase();
				if (BLACK_COLORS.contains(value)) {
					element.setAttribute(attrName, fillHex);
				} else if (WHITE_COLORS.contains(value)) {
					element.setAttribute(attrName, backgroundHex);
				}
			}
		}

		// Process style attribute (only replace colors, don't add defaults here)
		if (element.existAttribute("style")) {
			final String style = element.getAttribute("style", "");
			final String newStyle = applyColorsToStyle(style, fillHex, backgroundHex);
			if (!style.equals(newStyle)) {
				element.setAttribute("style", newStyle);
			}
		}

		// For shape elements: add default fill/stroke if not specified anywhere
		if (isShapeElement) {
			final boolean hasFillAttr = element.existAttribute("fill");
			final boolean hasFillInStyle = hasPropertyInStyle(element, "fill");

			// SVG default fill is black - if no fill specified, add fillColor
			if (!hasFillAttr && !hasFillInStyle) {
				element.setAttribute("fill", fillHex);
			}

			// Check if stroke-width is defined but stroke color is not
			final boolean hasStrokeWidth = element.existAttribute("stroke-width")
					|| hasPropertyInStyle(element, "stroke-width");
			final boolean hasStrokeAttr = element.existAttribute("stroke");
			final boolean hasStrokeInStyle = hasPropertyInStyle(element, "stroke");

			// If stroke-width is defined but no stroke color, add fillColor as stroke
			if (hasStrokeWidth && !hasStrokeAttr && !hasStrokeInStyle) {
				element.setAttribute("stroke", fillHex);
			}
		}

		// Process child elements recursively
		for (final XmlNode child : element.getNodes()) {
			if (child.isElement()) {
				applyColorsToElement(child.toElement(), fillHex, backgroundHex);
			}
		}
	}

	/**
	 * Check if a CSS property exists in the style attribute.
	 * @param element The element to check
	 * @param propertyName The CSS property name to look for
	 * @return true if the property is defined in the style attribute
	 */
	private boolean hasPropertyInStyle(final XmlElement element, final String propertyName) {
		if (!element.existAttribute("style")) {
			return false;
		}
		final String style = element.getAttribute("style", "").toLowerCase();
		// Check for "propertyName:" pattern
		return style.contains(propertyName + ":") || style.contains(propertyName + " :");
	}

	/**
	 * Apply color replacements to a CSS style string.
	 * @param style The style string (e.g., "fill:#000000;stroke:#FFFFFF")
	 * @param fillHex The hex color to replace black colors with
	 * @param backgroundHex The hex color to replace white colors with
	 * @return The modified style string
	 */
	private String applyColorsToStyle(final String style, final String fillHex, final String backgroundHex) {
		if (style == null || style.isEmpty()) {
			return style;
		}

		final StringBuilder result = new StringBuilder();
		final String[] properties = style.split(";");

		for (int i = 0; i < properties.length; i++) {
			final String property = properties[i].trim();
			if (property.isEmpty()) {
				continue;
			}

			final int colonIndex = property.indexOf(':');
			if (colonIndex <= 0) {
				// No colon or at start, keep as is
				if (result.length() > 0) {
					result.append(";");
				}
				result.append(property);
				continue;
			}

			final String propName = property.substring(0, colonIndex).trim().toLowerCase();
			final String propValue = property.substring(colonIndex + 1).trim().toLowerCase();

			String newValue = propValue;
			if (COLOR_ATTRIBUTES.contains(propName)) {
				if (BLACK_COLORS.contains(propValue)) {
					newValue = fillHex;
				} else if (WHITE_COLORS.contains(propValue)) {
					newValue = backgroundHex;
				}
			}

			if (result.length() > 0) {
				result.append(";");
			}
			result.append(propName).append(":").append(newValue);
		}

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
