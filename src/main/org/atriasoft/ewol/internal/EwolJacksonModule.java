package org.atriasoft.ewol.internal;

import java.io.IOException;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.DimensionBorderRadius;
import org.atriasoft.etk.DimensionInsets;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.Gravity;

import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.deser.std.StdDeserializer;
import com.fasterxml.jackson.databind.module.SimpleModule;

public class EwolJacksonModule extends SimpleModule {

	public EwolJacksonModule() {
		super("EwolJacksonModule");

		addDeserializer(Vector2b.class, new StdDeserializer<Vector2b>(Vector2b.class) {
			@Override
			public Vector2b deserialize(final JsonParser p, final DeserializationContext ctxt) throws IOException {
				return Vector2b.valueOf(p.getValueAsString());
			}
		});

		addDeserializer(Dimension2f.class, new StdDeserializer<Dimension2f>(Dimension2f.class) {
			@Override
			public Dimension2f deserialize(final JsonParser p, final DeserializationContext ctxt) throws IOException {
				return Dimension2f.valueOf(p.getValueAsString());
			}
		});

		addDeserializer(DimensionInsets.class, new StdDeserializer<DimensionInsets>(DimensionInsets.class) {
			@Override
			public DimensionInsets deserialize(final JsonParser p, final DeserializationContext ctxt) throws IOException {
				return DimensionInsets.valueOf(p.getValueAsString());
			}
		});

		addDeserializer(DimensionBorderRadius.class, new StdDeserializer<DimensionBorderRadius>(DimensionBorderRadius.class) {
			@Override
			public DimensionBorderRadius deserialize(final JsonParser p, final DeserializationContext ctxt) throws IOException {
				return DimensionBorderRadius.valueOf(p.getValueAsString());
			}
		});

		addDeserializer(Color.class, new StdDeserializer<Color>(Color.class) {
			@Override
			public Color deserialize(final JsonParser p, final DeserializationContext ctxt) throws IOException {
				try {
					return Color.valueOf(p.getValueAsString());
				} catch (final Exception e) {
					throw new IOException("Failed to parse Color: " + p.getValueAsString(), e);
				}
			}
		});

		addDeserializer(Gravity.class, new StdDeserializer<Gravity>(Gravity.class) {
			@Override
			public Gravity deserialize(final JsonParser p, final DeserializationContext ctxt) throws IOException {
				return Gravity.valueOf(p.getValueAsString());
			}
		});

		addDeserializer(Uri.class, new StdDeserializer<Uri>(Uri.class) {
			@Override
			public Uri deserialize(final JsonParser p, final DeserializationContext ctxt) throws IOException {
				return Uri.valueOf(p.getValueAsString());
			}
		});
	}
}
