/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.resource;

import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.List;

import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.ewol.Ewol;
import org.atriasoft.ewol.resource.font.FontMode;
import org.atriasoft.ewol.resource.font.GlyphProperty;
import org.atriasoft.gale.resource.Resource;
import org.atriasoft.gale.resource.ResourceTexture2;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ResourceTexturedFont extends ResourceTexture2 {
	private static final Logger LOGGER = LoggerFactory.getLogger(ResourceTexturedFont.class);

	private static final String CACHE_PREFIX = "__TEXTURED_FONT__>>";
	
	public static ResourceTexturedFont create(final Uri fontBaseUri) {
		ResourceTexturedFont resource;
		Resource resource2;
		if (fontBaseUri.isEmpty()) {
			LOGGER.error("Can not create a Texture Font without a filename: {}", fontBaseUri);
			return null;
		}
		resource2 = Resource.getManager().localKeep(CACHE_PREFIX + fontBaseUri.toString());
		if (resource2 != null) {
			if (resource2 instanceof ResourceTexturedFont) {
				return (ResourceTexturedFont) resource2;
			}
			LOGGER.error("Request resource fontName: '{}' with the wrong type (dynamic cast error)", fontBaseUri);
			System.exit(-1);
			return null;
		}
		resource = new ResourceTexturedFont(fontBaseUri);
		Resource.getManager().localAdd(resource);
		return resource;
	}
	
	// font is define for a specific mode
	public GlyphProperty emptyGlyph;
	private final Uri[] fileName = new Uri[4];
	// specific element to have the the know if the specify element is known...
	// == > otherwise I can just generate italic ...
	// == > Bold is a little more complicated (maybe with the border-size)
	private final ResourceFontSvg[] font = new ResourceFontSvg[4];
	private final int[] height = new int[4];
	// for the texture generation :
	public Vector2i[] lastGlyphPos = new Vector2i[4];
	public int[] lastRawHeigh = new int[4];
	public List<GlyphProperty>[] listElement = new ArrayList[4];
	private final FontMode[] modeWraping = new FontMode[4]; // !< This is a wrapping mode to prevent the fact that no
	private final boolean[] syntheticBold = new boolean[4];
	private final boolean[] syntheticItalic = new boolean[4];
	private int size = 10;

	protected ResourceTexturedFont(final Uri fontBaseUri) {
		super(CACHE_PREFIX + fontBaseUri.toString());
		LOGGER.debug("Load font: '{}'", fontBaseUri);
		
		this.font[0] = null;
		this.font[1] = null;
		this.font[2] = null;
		this.font[3] = null;
		
		this.modeWraping[0] = FontMode.REGULAR;
		this.modeWraping[1] = FontMode.REGULAR;
		this.modeWraping[2] = FontMode.REGULAR;
		this.modeWraping[3] = FontMode.REGULAR;
		
		this.lastGlyphPos[0] = Vector2i.ONE;
		this.lastGlyphPos[1] = Vector2i.ONE;
		this.lastGlyphPos[2] = Vector2i.ONE;
		this.lastGlyphPos[3] = Vector2i.ONE;
		
		this.lastRawHeigh[0] = 0;
		this.lastRawHeigh[1] = 0;
		this.lastRawHeigh[2] = 0;
		this.lastRawHeigh[3] = 0;
		
		this.listElement[0] = new ArrayList<>();
		this.listElement[1] = new ArrayList<>();
		this.listElement[2] = new ArrayList<>();
		this.listElement[3] = new ArrayList<>();
		
		final String sizeString = fontBaseUri.getProperty("size");
		if (sizeString == null) {
			this.size = 25;
		} else {
			this.size = Integer.parseInt(sizeString);
		}
		// find all the fonts...
		final Uri fontBaseUriBold = new Uri(fontBaseUri.getGroup(), fontBaseUri.getPath().replace(".svg", "Bold.svg"),
				fontBaseUri.getproperties());
		final Uri fontBaseUriOblique = new Uri(fontBaseUri.getGroup(),
				fontBaseUri.getPath().replace(".svg", "Oblique.svg"), fontBaseUri.getproperties());
		final Uri fontBaseUriBoldOblique = new Uri(fontBaseUri.getGroup(),
				fontBaseUri.getPath().replace(".svg", "BoldOblique.svg"), fontBaseUri.getproperties());
		if (fontBaseUri.exist()) {
			this.fileName[FontMode.REGULAR.getValue()] = fontBaseUri;
		}
		if (fontBaseUriBold.exist()) {
			this.fileName[FontMode.BOLD.getValue()] = fontBaseUriBold;
		}
		if (fontBaseUriOblique.exist()) {
			this.fileName[FontMode.ITALIC.getValue()] = fontBaseUriOblique;
		}
		if (fontBaseUriBoldOblique.exist()) {
			this.fileName[FontMode.BOLD_ITALIC.getValue()] = fontBaseUriBoldOblique;
		}
		
		// try to find the reference mode :
		FontMode refMode = FontMode.REGULAR;
		for (int iii = 3; iii >= 0; iii--) {
			if (this.fileName[iii] != null) {
				refMode = FontMode.get(iii);
			}
		}
		LOGGER.debug("         set reference mode: {}", refMode);
		// generate the wrapping on the preventing error
		for (int iii = 3; iii >= 0; iii--) {
			if (this.fileName[iii] != null) {
				this.modeWraping[iii] = FontMode.get(iii);
			} else {
				this.modeWraping[iii] = refMode;
			}
		}
		
		// Load native font variants
		for (int iiiFontId = 0; iiiFontId < 4; iiiFontId++) {
			if (this.fileName[iiiFontId] == null) {
				LOGGER.trace("can not load FONT [{}] name: \"{}\" ==> size={}", iiiFontId, this.fileName[iiiFontId],
						this.size);
				this.font[iiiFontId] = null;
				continue;
			}
			LOGGER.debug("Load FONT [{}] name: \"{}\" ==> size={}", iiiFontId, this.fileName[iiiFontId], this.size);
			this.font[iiiFontId] = ResourceFontSvg.create(this.fileName[iiiFontId]);
			if (this.font[iiiFontId] == null) {
				LOGGER.warn("error in loading FONT [{}] name: \"{}\" ==> size={}", iiiFontId, this.fileName[iiiFontId],
						this.size);
			}
		}
		// For missing variants, use the reference font with synthetic transformations
		final ResourceFontSvg refFont = this.font[refMode.getValue()];
		for (int iiiFontId = 0; iiiFontId < 4; iiiFontId++) {
			if (this.font[iiiFontId] == null && refFont != null) {
				this.font[iiiFontId] = refFont;
				final FontMode mode = FontMode.get(iiiFontId);
				this.syntheticBold[iiiFontId] = (mode == FontMode.BOLD || mode == FontMode.BOLD_ITALIC);
				this.syntheticItalic[iiiFontId] = (mode == FontMode.ITALIC || mode == FontMode.BOLD_ITALIC);
				if (this.syntheticBold[iiiFontId] || this.syntheticItalic[iiiFontId]) {
					LOGGER.debug("Using synthetic {} for mode {}",
							(this.syntheticBold[iiiFontId] && this.syntheticItalic[iiiFontId]) ? "bold+italic" :
							this.syntheticBold[iiiFontId] ? "bold" : "italic", mode);
				}
			}
		}
		for (int iiiFontId = 0; iiiFontId < 4; iiiFontId++) {
			// set the basic char-set:
			this.listElement[iiiFontId].clear();
			if (this.font[iiiFontId] == null) {
				continue;
			}
			this.height[iiiFontId] = this.font[iiiFontId].getHeight(this.size);
			// TODO : basic font use 512 is better ... == > maybe estimate it with the dpi
			// ???
			setImageSize(new Vector2i(FMath.nextP2(256 * this.size / 10), 32));
			// now we can access directly on the image
			this.data = new BufferedImage(this.data.getWidth(), this.data.getHeight(), BufferedImage.TYPE_INT_ARGB);
		}
		// add error glyph
		addGlyph((char) 0);
		// by default we set only the first AINSI char available
		for (int iii = 0x20; iii < 0x7F; iii++) {
			LOGGER.trace("Add glyph: {}", iii);
			addGlyph((char) iii);
		}
		flush();
		LOGGER.debug("Wrapping properties:");
		LOGGER.debug("    {} ==> {}", FontMode.REGULAR, getWrappingMode(FontMode.REGULAR));
		LOGGER.debug("    {} ==> {}", FontMode.ITALIC, getWrappingMode(FontMode.ITALIC));
		LOGGER.debug("    {} ==> {}", FontMode.BOLD, getWrappingMode(FontMode.BOLD));
		LOGGER.debug("    {} ==> {}", FontMode.BOLD_ITALIC, getWrappingMode(FontMode.BOLD_ITALIC));
	}
	
	/**
	 * add a glyph in a texture font.
	 * All 4 font modes share the same texture atlas position for each glyph
	 * to avoid spatial overlap between channels.
	 * @param val Char value to add.
	 * @return true if the image size have change, false otherwise
	 */
	private synchronized boolean addGlyph(final Character val) {
		boolean hasChange = false;
		// Step 1: Create GlyphProperty for all 4 modes and find max size
		final GlyphProperty[] glyphProps = new GlyphProperty[4];
		int maxWidth = 0;
		int maxHeight = 0;
		for (int iii = 0; iii < 4; iii++) {
			if (this.font[iii] == null) {
				continue;
			}
			final GlyphProperty tmpchar = this.font[iii].getGlyphProperty(this.size, val);
			glyphProps[iii] = tmpchar;
			if (tmpchar != null && tmpchar.exist()) {
				// Use the renderer's own size calculation for synthetic bold/italic
				if (this.syntheticBold[iii] || this.syntheticItalic[iii]) {
					final Vector2i rasterSize = this.font[iii].calculateRasterSize(
							this.size, val, this.syntheticBold[iii], this.syntheticItalic[iii]);
					if (rasterSize != null) {
						tmpchar.sizeTexture = rasterSize;
					}
				}
				if (tmpchar.sizeTexture.x() > maxWidth) {
					maxWidth = tmpchar.sizeTexture.x();
				}
				if (tmpchar.sizeTexture.y() > maxHeight) {
					maxHeight = tmpchar.sizeTexture.y();
				}
			}
		}
		// Step 2: Use a single shared position for all modes
		if (maxWidth > 0) {
			hasChange = true;
			// Check if we need to wrap to next line (use shared position from mode 0)
			if (this.lastGlyphPos[0].x() + maxWidth + 3 > this.data.getWidth()) {
				this.lastGlyphPos[0] = new Vector2i(1, this.lastGlyphPos[0].y() + this.lastRawHeigh[0]);
				this.lastRawHeigh[0] = 0;
			}
			// Check if we need to grow the image vertically
			while (this.lastGlyphPos[0].y() + maxHeight + 3 > this.data.getHeight()) {
				this.data = resizeImage(this.data, this.data.getWidth(), this.data.getHeight() * 2);
				for (int kkk = 0; kkk < 4; kkk++) {
					for (final GlyphProperty element : this.listElement[kkk]) {
						element.texturePosStart = element.texturePosStart.multiply(new Vector2f(1.0f, 0.5f));
						element.texturePosSize = element.texturePosSize.multiply(new Vector2f(1.0f, 0.5f));
					}
				}
			}
			final Vector2i sharedPos = this.lastGlyphPos[0];
			// Step 3: Draw each mode at the shared position
			for (int iii = 0; iii < 4; iii++) {
				final GlyphProperty tmpchar = glyphProps[iii];
				if (tmpchar == null || !tmpchar.exist() || this.font[iii] == null) {
					this.listElement[iii].add(tmpchar);
					continue;
				}
				this.font[iii].drawGlyph(this.data, this.size, sharedPos, tmpchar, iii,
						this.syntheticBold[iii], this.syntheticItalic[iii]);
				tmpchar.texturePosStart = new Vector2f(
						(float) sharedPos.x() / (float) this.data.getWidth(),
						(float) sharedPos.y() / (float) this.data.getHeight());
				tmpchar.texturePosSize = new Vector2f(
						(float) tmpchar.sizeTexture.x() / this.data.getWidth(),
						(float) tmpchar.sizeTexture.y() / this.data.getHeight());
				this.listElement[iii].add(tmpchar);
			}
			// Update shared position tracking
			if (this.lastRawHeigh[0] < maxHeight) {
				this.lastRawHeigh[0] = maxHeight + 1;
			}
			this.lastGlyphPos[0] = this.lastGlyphPos[0].add(new Vector2i(maxWidth + 1, 0));
		} else {
			// No existing glyph in any mode — just add the properties
			for (int iii = 0; iii < 4; iii++) {
				this.listElement[iii].add(glyphProps[iii]);
			}
		}
		if (hasChange) {
			LOGGER.trace("All gliph added ====> request a redraw of all the GUI");
			flush();
			Ewol.getContext().forceRedrawAllAsync();
		}
		return hasChange;
	}
	
	/**
	 * get the font height (user friendly)
	 * @return Dimention of the font the user requested
	 */
	public int getFontSize() {
		return this.size;
	}
	
	/**
	 * get the pointer on the corresponding glyph
	 * @param charcode The unicodeValue
	 * @param displayMode Mode to display the current font
	 * @return The pointer on the glyph == > never null
	 */
	public synchronized GlyphProperty getGlyph(final Character charcode, final FontMode displayMode) {
		// index : " + this.modeWraping[displayMode]);
		final int index = getIndex(charcode, displayMode);
		if (index < 0 || index >= this.listElement[displayMode.getValue()].size()) {
			LOGGER.warn("Try to get glyph index inexistant ==> return the index 0, id={}", index);
			if (this.listElement[displayMode.getValue()].size() > 0) {
				return this.listElement[displayMode.getValue()].get(0);
			}
			return this.emptyGlyph;
		}
		return this.listElement[displayMode.getValue()].get(index);
	}
	
	/**
	 * get the display height of this font
	 * @param DisplayMode Mode to display the current font
	 * @return Dimension of the font need between 2 lines
	 */
	public int getHeight() {
		return this.height[FontMode.REGULAR.getValue()];
	}
	
	public int getHeight(final FontMode displayMode) {
		return this.height[displayMode.getValue()];
	}
	
	/**
	 * get the ID of a unicode charcode
	 * @param charcode The unicodeValue
	 * @param displayMode Mode to display the current font
	 * @return The ID in the table (if it does not exist : return 0)
	 */
	private synchronized int getIndex(final Character charcode, final FontMode displayMode) {
		if (charcode < 0x20) {
			return 0;
		}
		if (charcode < 0x80) {
			return charcode - 0x1F;
		}
		for (int iii = 0x80 - 0x20; iii < this.listElement[displayMode.getValue()].size(); iii++) {
			// LOGGER.debug("search : '" + charcode + "' =?= '" +
			// (this.listElement[displayMode])[iii].UVal + "'");
			if (charcode == this.listElement[displayMode.getValue()].get(iii).getUnicodeValue()) {
				// LOGGER.debug("search : '" + charcode + "'");
				if (this.listElement[displayMode.getValue()].get(iii).exist()) {
					// LOGGER.debug("return " + iii);
					return charcode;
				}
				return 0;
			}
		}
		if (addGlyph(charcode)) {
			// TODO : This does not work due to the fact that the update of open GL is not
			// done in the context main cycle !!!
			Ewol.getContext().forceRedrawAll();
		}
		return 0;
	}
	
	/**
	 * The wrapping mode is used to prevent the non existance of a specific
	 *        mode. For exemple when a blod mode does not exist, this resend a
	 *        regular mode.
	 * @param source The requested mode.
	 * @return the best mode we have in stock.
	 */
	public FontMode getWrappingMode(final FontMode source) {
		return this.modeWraping[source.getValue()];
	}

}
