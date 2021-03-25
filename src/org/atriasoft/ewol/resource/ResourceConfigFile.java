/* @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.resource;

import java.util.ArrayList;
import java.util.List;

import org.atriasoft.ejson.Ejson;
import org.atriasoft.ejson.model.JsonNode;
import org.atriasoft.ejson.model.JsonObject;
import org.atriasoft.etk.Uri;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.gale.resource.Resource;

class ListElementConfig {
	public final String name;
	public JsonNode node;
	
	public ListElementConfig(final String name, final JsonNode node) {
		this.name = name;
		this.node = node;
	}
}

public class ResourceConfigFile extends Resource {
	/**
	 * keep the resource pointer.
	 * @note Never free this pointer by your own...
	 * @param name Name of the configuration file.
	 * @return pointer on the resource or null if an error occurred.
	 */
	public static ResourceConfigFile keep(final String name) {
		Resource resource2 = null;
		if (!name.isEmpty() && !name.contentEquals("---")) {
			resource2 = Resource.getManager().localKeep(name);
		}
		if (resource2 != null) {
			if (resource2 instanceof ResourceConfigFile) {
				resource2.keep();
				return (ResourceConfigFile) resource2;
			}
			Log.critical("Request resource file : '" + name + "' With the wrong type (dynamic cast error)");
			return null;
		}
		final ResourceConfigFile resource = new ResourceConfigFile(Uri.valueOf(name));
		Resource.getManager().localAdd(resource);
		return resource;
		
	}
	
	// List of all color in the file
	private final List<ListElementConfig> list = new ArrayList<>();
	
	protected ResourceConfigFile(final Uri uri) {
		super(uri.get());
		Log.debug("SFP : load '" + uri + "'");
		reload();
		
	}
	
	@Override
	public void cleanUp() {
		// TODO Auto-generated method stub
		
	}
	
	boolean getBoolean(final int id) {
		if (id < 0 || this.list.get(id).node == null || !this.list.get(id).node.isJsonBoolean()) {
			return false;
		}
		return this.list.get(id).node.toJsonBoolean().getValue();
	}
	
	public synchronized double getNumber(final int id) {
		if (id < 0 || this.list.get(id).node == null || !this.list.get(id).node.isJsonNumber()) {
			return 0.0;
		}
		return this.list.get(id).node.toJsonNumber().getValue();
	}
	
	String getString(final int id) {
		if (id < 0 || this.list.get(id).node == null || !this.list.get(id).node.isJsonString()) {
			return "";
		}
		return this.list.get(id).node.toJsonString().getValue();
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
		for (ListElementConfig listElementConfig : this.list) {
			listElementConfig.node = null;
		}
		JsonObject out;
		try {
			out = Ejson.parse(Uri.valueOf(this.name)).toJsonObject();
		} catch (final Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return;
		}
		out.getNodes().forEach(this::put);
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