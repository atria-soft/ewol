package org.atriasoft.ewol.internal;

import java.io.IOException;
import java.io.InputStream;

import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;

import org.atriasoft.etk.Uri;

public final class JsonHelper {

	private static final ObjectMapper MAPPER = new ObjectMapper()
			.configure(JsonParser.Feature.ALLOW_UNQUOTED_FIELD_NAMES, true)
			.configure(JsonParser.Feature.ALLOW_TRAILING_COMMA, true)
			.configure(JsonParser.Feature.ALLOW_SINGLE_QUOTES, true)
			.configure(JsonParser.Feature.ALLOW_COMMENTS, true);

	private JsonHelper() {
	}

	/** Parse JSON from a Uri and return the root JsonNode. */
	public static JsonNode parse(final Uri uri) throws IOException {
		try (final InputStream is = Uri.getStream(uri)) {
			if (is == null) {
				throw new IOException("Can not read the Stream : " + uri);
			}
			return MAPPER.readTree(is);
		}
	}

	/** Parse JSON from a String and return the root JsonNode. */
	public static JsonNode parse(final String data) throws IOException {
		return MAPPER.readTree(data);
	}
}
