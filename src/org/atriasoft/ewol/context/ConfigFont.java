package org.atriasoft.ewol.context;

import java.util.HashMap;
import java.util.Map;

import org.atriasoft.etk.Uri;
import org.atriasoft.ewol.internal.Log;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

public class ConfigFont {
	private final Map<String, Uri> fonts = new HashMap<>();
	private String name = "FreeSherif";
	private int size = 12;
	
	/**
	 * Constructor
	 */
	public ConfigFont() {
		// add default Esvg fonts:
		this.fonts.put("FreeSherif", new Uri("FONTS", "FreeSherif.svg", "esvg"));
		this.fonts.put("FreeSans", new Uri("FONTS", "FreeSans.svg", "esvg"));
		this.fonts.put("FreeMono", new Uri("FONTS", "FreeMono.svg", "esvg"));
	}
	
	public Uri getFontUri(final String fontName) {
		Uri out = this.fonts.get(fontName);
		if (out == null) {
			Log.warning(" try to get unexistant font : " + fontName);
		}
		return out;
	}
	
	/**
	 * get the current default font name
	 * @return a reference on the font name string
	 */
	public String getName() {
		return this.name;
	}
	
	/**
	 * get the default font size.
	 * @return the font size.
	 */
	public int getSize() {
		return this.size;
	}
	
	/**
	 * set the defaut font for all the widgets and basics display.
	 * @param fontName The font name requested (not case sensitive) ex "Arial" or multiple separate by ';' ex : "Arial;Helvetica".
	 * @param size The default size of the font default=10.
	 */
	public void set(final String fontName, final int size) {
		this.name = fontName;
		this.size = size;
		Log.debug("Set default Font : '" + this.name + "' size=" + this.size);
	}
	
	/**
	 * Set the current default font name
	 * @param fontName The font name requested (not case sensitive) ex "Arial" or multiple separate by ';' ex : "Arial;Helvetica".
	 */
	public void setName(final String fontName) {
		this.name = fontName;
		Log.debug("Set default Font : '" + this.name + "' size=" + this.size + " (change name only)");
	}
	
	/**
	 * Set the default font size.
	 * @param size new font size.
	 */
	public void setSize(final int size) {
		this.size = size;
		Log.debug("Set default Font : '" + this.name + "' size=" + this.size + " (change size only)");
	}
}
