/* @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.resource;

import java.util.ArrayList;
import java.util.List;

import java.util.Iterator;
import java.util.Map;

import com.fasterxml.jackson.databind.JsonNode;

import org.atriasoft.ewol.internal.JsonHelper;
import org.atriasoft.etk.Uri;
import org.atriasoft.gale.resource.Resource;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

class ListElementConfig {
	public final String name;
	public JsonNode node;
	
	public ListElementConfig(final String name, final JsonNode node) {
		this.name = name;
		this.node = node;
	}
}

public class ResourceConfigFile extends Resource {
	private static final Logger LOGGER = LoggerFactory.getLogger(ResourceConfigFile.class);

	/**
	 * Get the configuration file {@code name}, shared: the living one is kept
	 * (count of references + 1), otherwise it is loaded. Call {@link #release()}
	 * once when it is not used any more.
	 * @param name Name of the configuration file.
	 * @return the configuration file.
	 * @throws IllegalStateException if a resource of another type has this name.
	 */
	public static ResourceConfigFile create(final Uri name) {
		if (name != null && !name.isEmpty()) {
			final ResourceConfigFile existing = keepExisting(name.toString(), ResourceConfigFile.class);
			if (existing != null) {
				return existing;
			}
		}
		return new ResourceConfigFile(name);
	}
	
	public static ResourceConfigFile keep(final String name) {
		return ResourceConfigFile.create(Uri.valueOf(name));
	}
	
	// List of all color in the file
	private final List<ListElementConfig> list = new ArrayList<>();
	
	protected ResourceConfigFile(final Uri uri) {
		super(uri.toString());
		LOGGER.debug("SFP : load '" + uri + "'");
		reload();
		
	}
	
	@Override
	public void cleanUp() {
		// TODO Auto-generated method stub
		
	}
	
	public boolean getBoolean(final int id) {
		if (id < 0 || this.list.get(id).node == null || !this.list.get(id).node.isBoolean()) {
			return false;
		}
		return this.list.get(id).node.asBoolean();
	}

	public synchronized double getNumber(final int id) {
		if (id < 0 || this.list.get(id).node == null || !this.list.get(id).node.isNumber()) {
			return 0.0;
		}
		return this.list.get(id).node.asDouble();
	}

	public String getString(final int id) {
		return getString(id, "");
	}

	public String getString(final int id, final String defaultValue) {
		if (id < 0 || this.list.get(id).node == null || !this.list.get(id).node.isTextual()) {
			return defaultValue;
		}
		return this.list.get(id).node.asText();
	}
	
	public synchronized void put(final String name, final JsonNode node) {
		for (final ListElementConfig elem : this.list) {
			if (elem.name.contentEquals(name)) {
				elem.node = node;
				return;
			}
		}
		this.list.add(new ListElementConfig(name, node));
	}
	
	@Override
	public synchronized void reload() {
		// reset all parameters
		for (final ListElementConfig listElementConfig : this.list) {
			listElementConfig.node = null;
		}
		JsonNode out;
		try {
			out = JsonHelper.parse(Uri.valueOf(this.name));
		} catch (final Exception e) {
			e.printStackTrace();
			return;
		}
		final Iterator<Map.Entry<String, JsonNode>> fields = out.fields();
		while (fields.hasNext()) {
			final Map.Entry<String, JsonNode> entry = fields.next();
			put(entry.getKey(), entry.getValue());
		}
	}
	
	public synchronized int request(final String paramName) {
		for (int iii = 0; iii < this.list.size(); iii++) {
			final ListElementConfig elem = this.list.get(iii);
			if (elem.name.contentEquals(paramName)) {
				return iii;
			}
		}
		this.list.add(new ListElementConfig(paramName, null));
		return this.list.size() - 1;
	}
}