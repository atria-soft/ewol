package org.atriasoft.ewol.resource;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
import java.util.ArrayList;
import java.util.List;

import com.fasterxml.jackson.databind.JsonNode;

import org.atriasoft.ewol.internal.JsonHelper;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Uri;
import org.atriasoft.gale.resource.Resource;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

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
	private static final Logger LOGGER = LoggerFactory.getLogger(ResourceColorFile.class);

	/**
	 * Get the color file of {@code uri}, shared: the living one is kept (count of
	 * references + 1), otherwise it is loaded. Call {@link #release()} once when
	 * it is not used any more.
	 * @param uri File of the colors.
	 * @return the color file.
	 * @throws IllegalStateException if a resource of another type has this name.
	 */
	public static ResourceColorFile create(final Uri uri) {
		LOGGER.trace("KEEP: ColorFile: {}", uri);
		final ResourceColorFile existing = keepExisting(uri.toString(), ResourceColorFile.class);
		if (existing != null) {
			return existing;
		}
		LOGGER.debug("CREATE: ColorFile: {}", uri);
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
		LOGGER.debug("CF : load \"" + uri + "\"");
		reload();
		// LOGGER.debug("List of all color : " + this.list.keySet());
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
		for (final ListElement element : this.list) {
			final ListElement elem = element;
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
		for (final ListElement element : this.list) {
			element.color = this.errorColor;
		}
		LOGGER.info("[TODO] Mut be implemented ...");
		// open and read all json elements:
		try {
			final JsonNode out = JsonHelper.parse(Uri.valueOf(this.name));

			final JsonNode baseArray = out.get("color");
			if (baseArray == null || !baseArray.isArray()) {
				LOGGER.error("Can not get basic array : 'color' in file: {}", this.name);
				return;
			}
			boolean findError = false;
			for (final JsonNode it : baseArray) {
				if (!it.isObject()) {
					LOGGER.error(" can not get object in 'color' : {}", it);
					findError = true;
					continue;
				}
				final String name = it.get("name").asText();
				final String color = it.get("color").asText();
				LOGGER.debug("find new color : '{}' color='{}'", name, color);
				if (name.length() == 0) {
					LOGGER.error("Drop an empty name");
					findError = true;
					continue;
				}
				if (color.length() == 0) {
					put(name, this.errorColor);
				}
				put(name, Color.valueOf(color));
			}
			if (findError) {
				LOGGER.error("pb in parsing file: {}", this.name);
			}
		} catch (final Exception e) {
			LOGGER.error("catch exception in parsing config file: {}", e.getMessage());
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
