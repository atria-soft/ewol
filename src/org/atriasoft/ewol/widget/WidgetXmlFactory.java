package org.atriasoft.ewol.widget;

import java.util.HashMap;
import java.util.Map;

import org.atriasoft.aknot.model.InterfaceFactoryAccess;
import org.atriasoft.ewol.widget.meta.FileChooser;

public class WidgetXmlFactory implements InterfaceFactoryAccess {
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
		listWidgetAvaillable.put("PopUp", PopUp.class);
		listWidgetAvaillable.put("FileChooser", FileChooser.class);
		listWidgetAvaillable.put("Spin", Spin.class);
	}
	
	@Override
	public Class<?> findClass(final String name, final boolean caseSensitive) {
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
