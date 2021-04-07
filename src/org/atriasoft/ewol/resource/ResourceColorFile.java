package org.atriasoft.ewol.resource;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
import java.util.ArrayList;
import java.util.List;

import org.atriasoft.ejson.Ejson;
import org.atriasoft.ejson.model.JsonArray;
import org.atriasoft.ejson.model.JsonNode;
import org.atriasoft.ejson.model.JsonObject;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Uri;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.gale.resource.Resource;

class ListElement {
	public Color color;
	public String name;
	
	public ListElement(final String name, final Color color) {
		this.name = name;
		this.color = color;
	}
	
}

/**
 * ColorFile is a Resource designed to be specific with the theme (for
 *        example black, or white or orange ...)
 */
public class ResourceColorFile extends Resource {
	public static ResourceColorFile create(final Uri uri) {
		Log.verbose("KEEP: ColorFile: " + uri);
		ResourceColorFile object = null;
		final Resource object2 = Resource.getManager().localKeep(uri);
		if (object2 != null) {
			if (object2 instanceof ResourceColorFile) {
				return (ResourceColorFile) object2;
			}
			Log.critical("Request resource file : '" + uri + "' With the wrong type (dynamic cast error)");
			return null;
		}
		Log.debug("CREATE: FontFreeType: " + uri);
		// need to crate a new one ...
		return new ResourceColorFile(uri);
	}
	
	private Color errorColor = Color.ORANGE;
	
	private final List<ListElement> list = new ArrayList<>(); // !< List of all color in the file
	
	/**
	 * Constructor of the color property file
	 * @param uri Name of the file needed
	 */
	public ResourceColorFile(final Uri uri) {
		super(uri);
		Log.debug("CF : load \"" + uri + "\"");
		reload();
		// Log.debug("List of all color : " + this.list.keySet());
	}
	
	@Override
	public void cleanUp() {
		
	}
	
	/**
	 * Get the associated color of the ID.
	 * @param id Id of the color.
	 * @return The requested color.
	 */
	public Color get(final int id) {
		if (id < 0) {
			return this.errorColor;
		}
		return this.list.get(id).color;
	}
	
	/**
	 * Get All color name
	 * @return list of all color existing
	 */
	List<String> getColors() {
		final List<String> out = new ArrayList<>(this.list.size());
		for (int iii = 0; iii < this.list.size(); iii++) {
			out.add(this.list.get(iii).name);
		}
		return out;
	}
	
	public synchronized void put(final String name, final Color color) {
		for (int iii = 0; iii < this.list.size(); iii++) {
			final ListElement elem = this.list.get(iii);
			if (elem.name.contentEquals(name)) {
				elem.color = color;
				return;
			}
		}
		this.list.add(new ListElement(name, color));
	}
	
	@Override
	public synchronized void reload() {
		// remove all previous set of value :
		for (int iii = 0; iii < this.list.size(); ++iii) {
			this.list.get(iii).color = this.errorColor;
		}
		Log.todo("Mut be implemented ...");
		// open and read all json elements:
		try {
			final JsonObject out = Ejson.parse(Uri.valueOf(this.name)).toJsonObject();
			
			final JsonArray baseArray = out.get("color").toJsonArray();
			if (baseArray == null) {
				Log.error("Can not get basic array : 'color' in file:" + this.name);
				Ejson.display(out);
				return;
			}
			boolean findError = false;
			for (final JsonNode it : baseArray.getNodes()) {
				final JsonObject tmpObj = it.toJsonObject();
				if (tmpObj == null) {
					Log.error(" can not get object in 'color' : " + it);
					findError = true;
					continue;
				}
				final String name = tmpObj.get("name").toJsonString().getValue();
				final String color = tmpObj.get("color").toJsonString().getValue();
				Log.debug("find new color : '" + name + "' color='" + color + "'");
				if (name.length() == 0) {
					Log.error("Drop an empty name");
					findError = true;
					continue;
				}
				if (color.length() == 0) {
					put(name, this.errorColor);
				}
				put(name, Color.valueOf(color));
			}
			if (findError) {
				Log.error("pb in parsing file:" + this.name);
				Ejson.display(out);
			}
		} catch (final Exception e) {
			Log.error("chach exception in parsing config file... " + e.getMessage());
			e.printStackTrace();
		}
	}
	
	/**
	 * Request the presence of a specific color.
	 * @param paramName Name of the color.
	 * @return A unique ID of the color (or -1 if an error occured).
	 */
	public synchronized int request(final String paramName) {
		for (int iii = 0; iii < this.list.size(); iii++) {
			final ListElement elem = this.list.get(iii);
			if (elem.name.contentEquals(paramName)) {
				return iii;
			}
		}
		this.list.add(new ListElement(paramName, this.errorColor));
		return this.list.size() - 1;
	}
	
	/**
	 * Set the error color.
	 * @param errorColor Color that might be set when not finding a color
	 */
	public void setErrorColor(final Color errorColor) {
		this.errorColor = errorColor;
	}
	
}
