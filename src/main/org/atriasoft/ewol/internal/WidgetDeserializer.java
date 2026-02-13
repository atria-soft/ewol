package org.atriasoft.ewol.internal;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.databind.BeanDescription;
import com.fasterxml.jackson.databind.DeserializationConfig;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JavaType;
import com.fasterxml.jackson.databind.JsonDeserializer;
import com.fasterxml.jackson.databind.deser.BeanDeserializerFactory;
import com.fasterxml.jackson.databind.deser.ResolvableDeserializer;
import com.fasterxml.jackson.databind.deser.std.StdDeserializer;
import com.fasterxml.jackson.dataformat.xml.deser.FromXmlParser;

import org.atriasoft.ewol.widget.*;

/**
 * Custom Jackson deserializer for Widget polymorphism in XML.
 * <p>
 * Uses the XML element name as the type discriminator to create the appropriate
 * Widget subclass. This replaces Jackson's built-in WRAPPER_OBJECT strategy
 * which doesn't work correctly with Jackson XML for nested widget trees.
 */
public class WidgetDeserializer extends StdDeserializer<Widget> {

	private static final Map<String, Class<? extends Widget>> TYPE_MAP = new HashMap<>();

	static {
		TYPE_MAP.put("Box", Box.class);
		TYPE_MAP.put("Label", Label.class);
		TYPE_MAP.put("Button", Button.class);
		TYPE_MAP.put("Sizer", Sizer.class);
		TYPE_MAP.put("Entry", Entry.class);
		TYPE_MAP.put("Icon", Icon.class);
		TYPE_MAP.put("Spacer", Spacer.class);
		TYPE_MAP.put("CheckBox", CheckBox.class);
		TYPE_MAP.put("Tick", Tick.class);
		TYPE_MAP.put("PopUp", PopUp.class);
		TYPE_MAP.put("Image", ImageDisplay.class);
		TYPE_MAP.put("SplitPane", SplitPane.class);
		TYPE_MAP.put("ScrollView", ScrollView.class);
		TYPE_MAP.put("Select", Select.class);
		TYPE_MAP.put("Slider", Slider.class);
		TYPE_MAP.put("Spin", Spin.class);
		TYPE_MAP.put("ColorGradient", ColorGradient.class);
		TYPE_MAP.put("ColorPicker", ColorPicker.class);
		TYPE_MAP.put("ListFileSystem", ListFileSystem.class);
		TYPE_MAP.put("Composer", Composer.class);
		TYPE_MAP.put("PopoverTrigger", org.atriasoft.ewol.widget.meta.PopoverTrigger.class);
	}

	// Cache for bean deserializers to avoid repeated lookups
	private final Map<Class<?>, JsonDeserializer<Object>> deserializerCache = new HashMap<>();

	public WidgetDeserializer() {
		super(Widget.class);
	}

	@Override
	public Widget deserialize(final JsonParser p, final DeserializationContext ctxt) throws IOException {
		// Get the current XML element name which identifies the widget type
		String typeName = null;
		if (p instanceof final FromXmlParser xmlParser) {
			typeName = xmlParser.getStaxReader().getLocalName();
		}

		if (typeName == null) {
			throw new IOException("Cannot determine widget type: not in an XML element context");
		}

		final Class<? extends Widget> widgetClass = TYPE_MAP.get(typeName);
		if (widgetClass == null) {
			throw new IOException("Unknown widget type: '" + typeName + "'. Known types: " + TYPE_MAP.keySet());
		}

		// Get the bean deserializer for the concrete class, bypassing the custom
		// @JsonDeserialize annotation to avoid infinite recursion
		JsonDeserializer<Object> deserializer = deserializerCache.get(widgetClass);
		if (deserializer == null) {
			final JavaType javaType = ctxt.constructType(widgetClass);
			final DeserializationConfig config = ctxt.getConfig();
			final BeanDescription beanDesc = config.introspect(javaType);
			deserializer = BeanDeserializerFactory.instance
					.createBeanDeserializer(ctxt, javaType, beanDesc);
			if (deserializer instanceof final ResolvableDeserializer resolvable) {
				resolvable.resolve(ctxt);
			}
			deserializerCache.put(widgetClass, deserializer);
		}

		return (Widget) deserializer.deserialize(p, ctxt);
	}
}
