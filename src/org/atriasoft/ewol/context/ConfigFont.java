package org.atriasoft.ewol.context;

import org.atriasoft.etk.Uri;
import org.atriasoft.ewol.internal.Log;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

public class ConfigFont {
	private Uri folder = new Uri("DATA:///fonts?lib=ewol");
	private String name = "Arial;Helvetica";
	private int size = 10;
	private boolean useExternal = false;
	
	/**
	 * Constructor
	 */
	public ConfigFont() {}
	
	/**
	 * @brief get the default font folder.
	 * @return The default font folder.
	 */
	public Uri getFolder() {
		return this.folder;
	}
	
	/**
	 * @brief get the current default font name
	 * @return a reference on the font name string
	 */
	public String getName() {
		return this.name;
	}
	
	/**
	 * @brief get the default font size.
	 * @return the font size.
	 */
	public int getSize() {
		return this.size;
	};
	
	/**
	 * @brief get the use of internal/external Font
	 * @return true to enable search of internal data.
	 */
	public boolean getUseExternal() {
		return this.useExternal;
	}
	
	/**
	 * @brief set the defaut font for all the widgets and basics display.
	 * @param[in] _fontName The font name requested (not case sensitive) ex "Arial" or multiple separate by ';' ex : "Arial;Helvetica".
	 * @param[in] _size The default size of the font default=10.
	 */
	public void set(final String _fontName, final int _size) {
		this.name = _fontName;
		this.size = _size;
		Log.debug("Set default Font : '" + this.name + "' size=" + this.size);
	};
	
	/**
	 * @brief Specify the default font folder for the Ewol search system (only needed when embended font)
	 * @param[in] _folder basic folder of the font (ex: DATA:fonts)
	 */
	public void setFolder(final Uri _folder) {
		this.folder = _folder;
	}
	
	/**
	 * @brief Set the current default font name
	 * @param[in] _fontName The font name requested (not case sensitive) ex "Arial" or multiple separate by ';' ex : "Arial;Helvetica".
	 */
	public void setName(final String _fontName) {
		this.name = _fontName;
		Log.debug("Set default Font : '" + this.name + "' size=" + this.size + " (change name only)");
	}
	
	/**
	 * @brief Set the default font size.
	 * @param[in] _size new font size.
	 */
	public void setSize(final int _size) {
		this.size = _size;
		Log.debug("Set default Font : '" + this.name + "' size=" + this.size + " (change size only)");
	};
	
	/**
	 * @brief set use of internal/external Font
	 * @param[in] _val true to enable search of internal data.
	 */
	public void setUseExternal(final boolean _val) {
		this.useExternal = _val;
	};
};
