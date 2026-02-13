package org.atriasoft.ewol.internal;

import java.io.ByteArrayInputStream;
import java.io.StringWriter;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public final class XmlHelper {

	private XmlHelper() {
	}

	/** Parse XML from a string and return the document element. */
	public static Element parse(final String xml) throws Exception {
		final DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		factory.setNamespaceAware(false);
		trySetFeature(factory, "http://apache.org/xml/features/nonvalidating/load-external-dtd", false);
		trySetFeature(factory, "http://xml.org/sax/features/external-general-entities", false);
		trySetFeature(factory, "http://xml.org/sax/features/external-parameter-entities", false);
		trySetAttribute(factory, "http://www.oracle.com/xml/jaxp/properties/entityExpansionLimit", 0);
		trySetAttribute(factory, "http://javax.xml.XMLConstants/property/accessExternalDTD", "");
		final DocumentBuilder builder = factory.newDocumentBuilder();
		final Document doc = builder.parse(new ByteArrayInputStream(xml.getBytes(StandardCharsets.UTF_8)));
		return doc.getDocumentElement();
	}

	private static void trySetFeature(final DocumentBuilderFactory factory, final String feature, final boolean value) {
		try {
			factory.setFeature(feature, value);
		} catch (final Exception e) {
			// Feature may not be supported by all parsers
		}
	}

	private static void trySetAttribute(final DocumentBuilderFactory factory, final String attr, final Object value) {
		try {
			factory.setAttribute(attr, value);
		} catch (final Exception e) {
			// Attribute may not be supported by all parsers
		}
	}

	/** Get the first direct child element with the given tag name, or null. */
	public static Element getNode(final Element element, final String tagName) {
		final NodeList list = element.getChildNodes();
		for (int i = 0; i < list.getLength(); i++) {
			if (list.item(i) instanceof final Element child && child.getTagName().equals(tagName)) {
				return child;
			}
		}
		return null;
	}

	/** Get all direct child Nodes (Element, Text, Comment, etc.). */
	public static List<Node> allChildNodes(final Element element) {
		final List<Node> result = new ArrayList<>();
		final NodeList list = element.getChildNodes();
		for (int i = 0; i < list.getLength(); i++) {
			result.add(list.item(i));
		}
		return result;
	}

	/** Get all direct child Elements (skips text, comment, etc. nodes). */
	public static List<Element> children(final Element element) {
		final List<Element> result = new ArrayList<>();
		final NodeList list = element.getChildNodes();
		for (int i = 0; i < list.getLength(); i++) {
			if (list.item(i) instanceof final Element child) {
				result.add(child);
			}
		}
		return result;
	}

	/** Serialize an Element back to an XML string. */
	public static String generate(final Element element) throws Exception {
		final TransformerFactory tf = TransformerFactory.newInstance();
		final Transformer transformer = tf.newTransformer();
		transformer.setOutputProperty(OutputKeys.OMIT_XML_DECLARATION, "yes");
		transformer.setOutputProperty(OutputKeys.INDENT, "yes");
		final StringWriter writer = new StringWriter();
		transformer.transform(new DOMSource(element), new StreamResult(writer));
		return writer.toString();
	}
}
