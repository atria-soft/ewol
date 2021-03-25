package org.atriasoft.ewol.context;

import org.atriasoft.etk.Uri;
import org.atriasoft.ewol.internal.Log;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

public class ConfigFont {
	private Uri folder = new Uri("DATA", "fonts", "ewol");
	private String name = "Arial;Helvetica";
	private int size = 10;
	private boolean useExternal = false;
	
	/**
	 * Constructor
	 */
	public ConfigFont() {}
	
	/**
	 * get the default font folder.
	 * @return The default font folder.
	 */
	public Uri getFolder() {
		return this.folder;
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
	 * get the use of internal/external Font
	 * @return true to enable search of internal data.
	 */
	public boolean getUseExternal() {
		return this.useExternal;
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
	 * Specify the default font folder for the Ewol search system (only needed when embended font)
	 * @param folder basic folder of the font (ex: DATA:fonts)
	 */
	public void setFolder(final Uri folder) {
		this.folder = folder;
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
	
	/**
	 * set use of internal/external Font
	 * @param val true to enable search of internal data.
	 */
	public void setUseExternal(final boolean val) {
		this.useExternal = val;
	}
}
