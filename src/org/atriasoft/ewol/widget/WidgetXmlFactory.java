package org.atriasoft.ewol.widget;

import java.util.HashMap;
import java.util.Map;

import org.atriasoft.exml.annotation.XmlFactory.InterfaceXmlFactoryAccess;

public class WidgetXmlFactory implements InterfaceXmlFactoryAccess {
	private static Map<String, Class<?>> listWidgetAvaillable = new HashMap<>();
	static {
		listWidgetAvaillable.put("Sizer", Sizer.class);
		listWidgetAvaillable.put("Spacer", Spacer.class);
		listWidgetAvaillable.put("Label", Label.class);
		listWidgetAvaillable.put("Entry", Entry.class);
		listWidgetAvaillable.put("Image", ImageDisplay.class);
		listWidgetAvaillable.put("Button", Button.class);
		listWidgetAvaillable.put("Tick", Tick.class);
		listWidgetAvaillable.put("CheckBox", CheckBox.class);
		listWidgetAvaillable.put("ListFileSystem", ListFileSystem.class);
	}
	
	@Override
	public Class<?> findClass(final String name) {
		return listWidgetAvaillable.get(name);
	}
	
	@Override
	public String generateName(final Object widget) {
		return null;
	}
	
	@Override
	public Map<String, Class<?>> getConversionMap() {
		return listWidgetAvaillable;
	}
	
}
