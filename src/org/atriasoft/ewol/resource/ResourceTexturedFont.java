/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.resource;

import java.util.ArrayList;
import java.util.List;

import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.ewol.Ewol;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.resource.font.FontBase;
import org.atriasoft.ewol.resource.font.FontMode;
import org.atriasoft.ewol.resource.font.GlyphProperty;

public class ResourceTexturedFont extends ResourceTexture2 {
	/**
	 * @brief Get all the Path contain in the specidy path:
	 * @param[in] _path Generic path to parse ...
	 * @return The list of path found
	 * @example[start]
	 *     auto out = explodeMultiplePath("DATA:///font?lib=ewol");
	 *     // out contain: {"DATA:///font", "DATA:///font?lib=ewol"}
	 * @example[stop]
	 */
	private static List<Uri> explodeMultiplePath(final Uri _uri) {
		final List<Uri> out = new ArrayList<>();
		out.add(_uri);
		return out;
	}
	
	private final Uri[] fileName = new Uri[4];
	private int size = 10;
	private final int[] height = new int[4];
	// specific element to have the the know if the specify element is known...
	//  == > otherwise I can just generate italic ...
	//  == > Bold is a little more complicated (maybe with the bordersize)
	private final FontBase[] font = new FontBase[4];
	private final FontMode[] modeWraping = new FontMode[4]; //!< This is a wrapping mode to prevent the fact that no font is define for a specific mode
	public GlyphProperty emptyGlyph;
	public List<GlyphProperty>[] listElement;// = new (List<GlyphProperty>)[4];
	
	// for the texture generation :
	public Vector2i[] lastGlyphPos = new Vector2i[4];
	public int[] lastRawHeigh = new int[4];
	
	protected ResourceTexturedFont(final String _fontName) {
		super(_fontName);
		
		Log.debug("Load font : '" + _fontName + "'");
		
		this.font[0] = null;
		this.font[1] = null;
		this.font[2] = null;
		this.font[3] = null;
		
		this.modeWraping[0] = FontMode.Regular;
		this.modeWraping[1] = FontMode.Regular;
		this.modeWraping[2] = FontMode.Regular;
		this.modeWraping[3] = FontMode.Regular;
		
		this.lastGlyphPos[0].setValue(1, 1);
		this.lastGlyphPos[1].setValue(1, 1);
		this.lastGlyphPos[2].setValue(1, 1);
		this.lastGlyphPos[3].setValue(1, 1);
		
		this.lastRawHeigh[0] = 0;
		this.lastRawHeigh[1] = 0;
		this.lastRawHeigh[2] = 0;
		this.lastRawHeigh[3] = 0;
		
		int tmpSize = 0;
		// extarct name and size :
		final String[] tmpList = _fontName.split(":");
		
		if (tmpList.length == 1) {
			this.size = 1;
			Log.critical("Can not parse the font name: '" + _fontName + "' ??? ':' ");
			return;
		} else {
			// zsdefsdf
			tmpSize = Integer.valueOf(tmpList[1]);
		}
		
		final String localName = tmpList[0];
		if (tmpSize > 400) {
			Log.error("Font size too big ==> limit at 400 when exceed ==> error: " + tmpSize + "==>30");
			tmpSize = 30;
		}
		this.size = tmpSize;
		
		final List<Uri> folderList = new ArrayList<>();
		;
		if (Ewol.getContext().getFontDefault().getUseExternal() == true) {
			/*
			#if defined(__TARGET_OS__Android)
				folderList.pushBack(etk::Path("/system/fonts"));#elif defined(__TARGET_OS__Linux)
				folderList.pushBack(etk::Path("/usr/share/fonts"));
			#endif
			*/
		}
		final Uri applicationBaseFont = Ewol.getContext().getFontDefault().getFolder();
		for (final Uri it : explodeMultiplePath(applicationBaseFont)) {
			folderList.add(it);
		}
		for (int folderID = 0; folderID < folderList.size(); folderID++) {
			final List<Uri> output = Uri.listRecursive(folderList.get(folderID));
			
			final String[] split = localName.split(";");
			Log.debug("try to find font named : " + split + " in: " + output);
			//Log.critical("parse string : " + split);
			boolean hasFindAFont = false;
			for (int jjj = 0; jjj < split.length; jjj++) {
				Log.debug("    try with : '" + split[jjj] + "'");
				for (int iii = 0; iii < output.size(); iii++) {
					final String nameFolder = output.get(iii).getPath();
					//Log.debug(" file : " + output.get(iii));
					if (nameFolder.endsWith(split[jjj] + "-" + "bold" + ".ttf") == true || nameFolder.endsWith(split[jjj] + "-" + "b" + ".ttf") == true
							|| nameFolder.endsWith(split[jjj] + "-" + "bd" + ".ttf") == true || nameFolder.endsWith(split[jjj] + "bold" + ".ttf") == true
							|| nameFolder.endsWith(split[jjj] + "bd" + ".ttf") == true || nameFolder.endsWith(split[jjj] + "b" + ".ttf") == true) {
						Log.debug(" find Font [Bold]        : " + output.get(iii));
						this.fileName[FontMode.Bold.getValue()] = output.get(iii);
						hasFindAFont = true;
					} else if (nameFolder.endsWith(split[jjj] + "-" + "oblique" + ".ttf") == true || nameFolder.endsWith(split[jjj] + "-" + "italic" + ".ttf") == true
							|| nameFolder.endsWith(split[jjj] + "-" + "Light" + ".ttf") == true || nameFolder.endsWith(split[jjj] + "-" + "i" + ".ttf") == true
							|| nameFolder.endsWith(split[jjj] + "oblique" + ".ttf") == true || nameFolder.endsWith(split[jjj] + "italic" + ".ttf") == true
							|| nameFolder.endsWith(split[jjj] + "light" + ".ttf") == true || nameFolder.endsWith(split[jjj] + "i" + ".ttf") == true) {
						Log.debug(" find Font [Italic]      : " + output.get(iii));
						this.fileName[FontMode.Italic.getValue()] = output.get(iii);
						hasFindAFont = true;
					} else if (nameFolder.endsWith(split[jjj] + "-" + "bolditalic" + ".ttf") == true || nameFolder.endsWith(split[jjj] + "-" + "boldoblique" + ".ttf") == true
							|| nameFolder.endsWith(split[jjj] + "-" + "bi" + ".ttf") == true || nameFolder.endsWith(split[jjj] + "-" + "z" + ".ttf") == true
							|| nameFolder.endsWith(split[jjj] + "bolditalic" + ".ttf") == true || nameFolder.endsWith(split[jjj] + "boldoblique" + ".ttf") == true
							|| nameFolder.endsWith(split[jjj] + "bi" + ".ttf") == true || nameFolder.endsWith(split[jjj] + "z" + ".ttf") == true) {
						Log.debug(" find Font [Bold-Italic] : " + output.get(iii));
						this.fileName[FontMode.BoldItalic.getValue()] = output.get(iii);
						hasFindAFont = true;
					} else if (nameFolder.endsWith(split[jjj] + "-" + "regular" + ".ttf") == true || nameFolder.endsWith(split[jjj] + "-" + "r" + ".ttf") == true
							|| nameFolder.endsWith(split[jjj] + "regular" + ".ttf") == true || nameFolder.endsWith(split[jjj] + "r" + ".ttf") == true
							|| nameFolder.endsWith(split[jjj] + ".ttf") == true) {
						Log.debug(" find Font [Regular]     : " + output.get(iii));
						this.fileName[FontMode.Regular.getValue()] = output.get(iii);
						hasFindAFont = true;
					}
				}
				if (hasFindAFont == true) {
					Log.debug("    find this font : '" + split[jjj] + "'");
					break;
				} else if (jjj == split.length - 1) {
					Log.debug("Find NO font in the LIST ... " + split);
				}
			}
			if (hasFindAFont == true) {
				Log.debug("    find this font : '" + folderList.get(folderID) + "'");
				break;
			} else if (folderID == folderList.size() - 1) {
				Log.error("Find NO font in the LIST ... " + folderList);
			}
		}
		// try to find the reference mode :
		FontMode refMode = FontMode.Regular;
		for (int iii = 3; iii >= 0; iii--) {
			if (this.fileName[iii] != null) {
				refMode = FontMode.get(iii);
			}
		}
		Log.debug("         set reference mode : " + refMode);
		// generate the wrapping on the preventing error
		for (int iii = 3; iii >= 0; iii--) {
			if (this.fileName[iii] != null) {
				this.modeWraping[iii] = FontMode.get(iii);
			} else {
				this.modeWraping[iii] = refMode;
			}
		}
		
		for (int iiiFontId = 0; iiiFontId < 4; iiiFontId++) {
			if (this.fileName[iiiFontId] == null) {
				Log.debug("can not load FONT [" + iiiFontId + "] name : \"" + this.fileName[iiiFontId] + "\"  == > size=" + this.size);
				this.font[iiiFontId] = null;
				continue;
			}
			Log.debug("Load FONT [" + iiiFontId + "] name : \"" + this.fileName[iiiFontId] + "\"  == > size=" + this.size);
			this.font[iiiFontId] = ResourceFontFreeType.create(this.fileName[iiiFontId]);
			if (this.font[iiiFontId] == null) {
				Log.debug("error in loading FONT [" + iiiFontId + "] name : \"" + this.fileName[iiiFontId] + "\"  == > size=" + this.size);
			}
		}
		for (int iiiFontId = 0; iiiFontId < 4; iiiFontId++) {
			// set the bassic charset:
			this.listElement[iiiFontId].clear();
			if (this.font[iiiFontId] == null) {
				continue;
			}
			this.height[iiiFontId] = this.font[iiiFontId].getHeight(this.size);
			// TODO : basic font use 512 is better ...  == > maybe estimate it with the dpi ???
			setImageSize(new Vector2i(256, 32));
			// now we can acces directly on the image
			this.data.clear();
		}
		// add error glyph
		addGlyph((char) 0);
		// by default we set only the first AINSI char availlable
		for (int iii = 0x20; iii < 0x7F; iii++) {
			Log.verbose("Add clyph :" + iii);
			addGlyph((char) iii);
		}
		flush();
		Log.debug("Wrapping properties : ");
		Log.debug("    " + FontMode.Regular + " == >" + getWrappingMode(FontMode.Regular));
		Log.debug("    " + FontMode.Italic + " == >" + getWrappingMode(FontMode.Italic));
		Log.debug("    " + FontMode.Bold + " == >" + getWrappingMode(FontMode.Bold));
		Log.debug("    " + FontMode.BoldItalic + " == >" + getWrappingMode(FontMode.BoldItalic));
	}
	
	/**
	 * @brief add a glyph in a texture font.
	 * @param[in] _val Char value to add.
	 * @return true if the image size have change, false otherwise
	 */
	private synchronized boolean addGlyph(final Character _val) {
		boolean hasChange = false;
		// for each font :
		for (int iii = 0; iii < 4; iii++) {
			if (this.font[iii] == null) {
				continue;
			}
			// add the curent "char"
			final GlyphProperty tmpchar = new GlyphProperty();
			tmpchar.UVal = _val;
			
			if (this.font[iii].getGlyphProperty(this.size, tmpchar) == true) {
				//Log.debug("load char : '" + _val + "'=" + _val.get());
				hasChange = true;
				// change line if needed ...
				if (this.lastGlyphPos[iii].x + tmpchar.sizeTexture.x + 3 > this.data.getSize().x) {
					this.lastGlyphPos[iii].setX(1);
					this.lastGlyphPos[iii].add(new Vector2i(0, this.lastRawHeigh[iii]));
					this.lastRawHeigh[iii] = 0;
				}
				while (this.lastGlyphPos[iii].y + tmpchar.sizeTexture.y + 3 > this.data.getSize().y) {
					final Vector2i size = this.data.getSize();
					size.setY(size.y * 2);
					this.data.resize(size.x, size.y);
					// note : need to rework all the lyer due to the fact that the texture is used by the faur type...
					for (int kkk = 0; kkk < 4; kkk++) {
						// change the coordonate on the element in the texture
						for (int jjj = 0; jjj < this.listElement[kkk].size(); ++jjj) {
							this.listElement[kkk].get(jjj).texturePosStart.multiply(new Vector2f(1.0f, 0.5f));
							this.listElement[kkk].get(jjj).texturePosSize.multiply(new Vector2f(1.0f, 0.5f));
						}
					}
				}
				// draw the glyph
				this.font[iii].drawGlyph(this.data, this.size, this.lastGlyphPos[iii], tmpchar, iii);
				// set video position
				tmpchar.texturePosStart.setValue((float) this.lastGlyphPos[iii].x / (float) this.data.getSize().x, (float) this.lastGlyphPos[iii].y / (float) this.data.getSize().y);
				tmpchar.texturePosSize.setValue((float) tmpchar.sizeTexture.x / this.data.getSize().x, (float) tmpchar.sizeTexture.y / this.data.getSize().y);
				
				// update the maximum of the line hight : 
				if (this.lastRawHeigh[iii] < tmpchar.sizeTexture.y) {
					// note : +1 is for the overlapping of the glyph (Part 2)
					this.lastRawHeigh[iii] = tmpchar.sizeTexture.y + 1;
				}
				// note : +1 is for the overlapping of the glyph (Part 3)
				// update the Bitmap position drawing : 
				this.lastGlyphPos[iii].add(new Vector2i(tmpchar.sizeTexture.x + 1, 0));
			} else {
				Log.warning("Did not find char : '" + _val + "'=" + _val);
				tmpchar.setNotExist();
			}
			this.listElement[iii].add(tmpchar);
			//this.font[iii].display;
			// generate the kerning for all the characters :
			if (tmpchar.exist() == true) {
				// TODO : set the kerning back ...
				//this.font[iii].generateKerning(this.size, this.listElement[iii]);
			}
		}
		if (hasChange == true) {
			flush();
			Ewol.getContext().forceRedrawAll();
			//egami::store(this.data, "fileFont.bmp"); // ==> for debug test only ...
		}
		return hasChange;
	}
	
	/**
	 * @brief get the font height (user friendly)
	 * @return Dimention of the font the user requested
	 */
	private int getFontSize() {
		return this.size;
	}
	
	/**
	 * @brief get the pointer on the coresponding glyph
	 * @param[in] _charcode The unicodeValue
	 * @param[in] _displayMode Mode to display the currrent font
	 * @return The pointer on the glyph  == > never null
	 */
	private synchronized GlyphProperty getGlyph(final Character _charcode, final FontMode _displayMode) {
		//Log.debug("Get glyph property for mode: " + _displayMode + "  == > wrapping index : " + this.modeWraping[_displayMode]);
		final int index = getIndex(_charcode, _displayMode);
		if (index < 0 || index >= this.listElement[_displayMode.getValue()].size()) {
			Log.error(" Try to get glyph index inexistant ...  == > return the index 0 ... id=" + index);
			if (this.listElement[_displayMode.getValue()].size() > 0) {
				return this.listElement[_displayMode.getValue()].get(0);
			}
			return this.emptyGlyph;
		}
		//Log.error("      index=" + index);
		//Log.error("      this.UVal=" + this.listElement[_displayMode][index].UVal);
		//Log.error("      this.glyphIndex=" + this.listElement[_displayMode][index].glyphIndex);
		//Log.error("      this.advance=" + this.listElement[_displayMode][index].advance);
		//Log.error("      this.bearing=" + this.listElement[_displayMode][index].bearing);
		return this.listElement[_displayMode.getValue()].get(index);
	};
	
	/**
	 * @brief get the display height of this font
	 * @param[in] _displayMode Mode to display the currrent font
	 * @return Dimention of the font need between 2 lines
	 */
	private int getHeight() {
		return this.height[FontMode.Regular.getValue()];
	}
	
	private int getHeight(final FontMode _displayMode) {
		return this.height[_displayMode.getValue()];
	}
	
	/**
	 * @brief get the ID of a unicode charcode
	 * @param[in] _charcode The unicodeValue
	 * @param[in] _displayMode Mode to display the currrent font
	 * @return The ID in the table (if it does not exist : return 0)
	 */
	private synchronized int getIndex(final Character _charcode, final FontMode _displayMode) {
		if (_charcode < 0x20) {
			return 0;
		} else if (_charcode < 0x80) {
			return _charcode - 0x1F;
		} else {
			for (int iii = 0x80 - 0x20; iii < this.listElement[_displayMode.getValue()].size(); iii++) {
				//Log.debug("search : '" + charcode + "' =?= '" + (this.listElement[displayMode])[iii].UVal + "'");
				if (_charcode == this.listElement[_displayMode.getValue()].get(iii).UVal) {
					//Log.debug("search : '" + charcode + "'");
					if (this.listElement[_displayMode.getValue()].get(iii).exist()) {
						//Log.debug("return " + iii);
						return iii;
					} else {
						return 0;
					}
				}
			}
		}
		if (addGlyph(_charcode) == true) {
			// TODO : This does not work due to the fact that the update of open GL is not done in the context main cycle !!!
			Ewol.getContext().forceRedrawAll();
		}
		return 0;
	};
	
	/**
	 * @brief The wrapping mode is used to prevent the non existance of a specific mode.
	 *        For exemple when a blod mode does not exist, this resend a regular mode.
	 * @param[in] _source The requested mode.
	 * @return the best mode we have in stock.
	 */
	private FontMode getWrappingMode(final FontMode _source) {
		return this.modeWraping[_source.getValue()];
	}
}
