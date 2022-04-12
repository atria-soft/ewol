package org.atriasoft.ewol.widget;

import java.util.HashMap;
import java.util.Map;

import org.atriasoft.exml.annotation.XmlFactory.InterfaceXmlFactoryAccess;

public class WidgetXmlFactory implements InterfaceXmlFactoryAccess {
	private static Map<String, Class<?>> listWidgetAvaillable = new HashMap<>();
	static {
		listWidgetAvaillable.put("Button", Button.class);
		listWidgetAvaillable.put("Sizer", Sizer.class);
		listWidgetAvaillable.put("Label", LabelOnSVG.class);
		//listWidgetAvaillable.put("CheckBox", CheckBox.class);
		listWidgetAvaillable.put("Tick", Tick.class);
		listWidgetAvaillable.put("Image", ImageDisplay.class);
	}
	
	@Override
	public Class<?> findClass(final String name) {
		return listWidgetAvaillable.get(name);
	}
	
	@Override
	public String generateName(final Object widget) {
		return null;
	}
	
}
