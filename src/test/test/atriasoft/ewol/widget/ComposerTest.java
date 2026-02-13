package test.atriasoft.ewol.widget;

import static org.junit.jupiter.api.Assertions.*;

import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.dataformat.xml.XmlMapper;
import org.atriasoft.ewol.internal.EwolJacksonModule;
import org.atriasoft.ewol.widget.Box;
import org.atriasoft.ewol.widget.Composer;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Widget;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import test.atriasoft.ewol.EwolTestContext;

class ComposerTest {

	private static final XmlMapper XML_MAPPER = XmlMapper.builder()
			.addModule(new EwolJacksonModule())
			.configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false)
			.build();

	@BeforeAll
	static void setUpClass() {
		EwolTestContext.init();
	}

	@Test
	void testDeserializeWidgetAsSizer() throws Exception {
		// When deserializing Widget.class, the root element name identifies the type
		final String xml = """
			<Sizer mode="VERTICAL">
				<Label>Hello</Label>
			</Sizer>
			""";
		final Widget widget = XML_MAPPER.readValue(xml, Widget.class);
		assertNotNull(widget, "Widget should not be null");
		assertInstanceOf(Sizer.class, widget, "Widget should be a Sizer");
	}

	@Test
	void testComposerWithSizerChild() {
		final String xml = """
			<Composer>
				<Sizer mode="VERTICAL" lock="true" fill="true" expand="true">
					<Label>Hello</Label>
				</Sizer>
			</Composer>
			""";
		final Widget result = Composer.composerGenerateString(xml);
		assertNotNull(result, "Result should not be null");
		// The <Composer> wrapper is stripped, so the result is the Sizer directly
		assertInstanceOf(Sizer.class, result, "Result should be a Sizer");
	}

	@Test
	void testComposerWithoutWrapper() {
		final String xml = """
			<Sizer mode="HORIZONTAL">
				<Label>Test</Label>
				<Spacer/>
			</Sizer>
			""";
		final Widget result = Composer.composerGenerateString(xml);
		assertNotNull(result, "Result should not be null");
		assertInstanceOf(Sizer.class, result, "Result should be a Sizer");
	}

	@Test
	void testSizerWithChildren() throws Exception {
		// Test that Sizer children are deserialized correctly
		final String xml = """
			<Sizer mode="VERTICAL">
				<Label>First</Label>
				<Spacer/>
			</Sizer>
			""";
		final Widget result = XML_MAPPER.readValue(xml, Widget.class);
		assertNotNull(result, "Result should not be null");
		assertInstanceOf(Sizer.class, result, "Result should be a Sizer");
		final Sizer sizer = (Sizer) result;
		assertFalse(sizer.getSubWidgets().isEmpty(), "Sizer should have children");
		assertEquals(2, sizer.getSubWidgets().size(), "Sizer should have 2 children");
	}

	@Test
	void testBoxWithLabelChild() throws Exception {
		// Test that Box (extends Container) correctly deserializes its child widget
		final String xml = """
			<Box>
				<Label>Hello</Label>
			</Box>
			""";
		final Widget result = XML_MAPPER.readValue(xml, Widget.class);
		assertNotNull(result, "Result should not be null");
		assertInstanceOf(Box.class, result, "Result should be a Box");
		final Box box = (Box) result;
		assertNotNull(box.getSubWidget(), "Box should have a sub widget");
		assertInstanceOf(Label.class, box.getSubWidget(), "Sub widget should be a Label");
	}

	@Test
	void testToolMapHeightLikeXml() {
		// Mirror the structure of ToolMapHeight.xml
		final String xml = """
			<Composer>
				<Sizer mode="VERTICAL" lock="true" fill="true" expand="true">
					<Label name="[555]file-chooser:title-label">Change height</Label>
					<Sizer mode="HORIZONTAL" lock="true" expand="true,false,false">
						<Image src="width.svg" expand="false" image-size="48,48px"/>
						<Spacer min-size="2,2mm"/>
						<Slider name="[555]HeighMap:slider-width"
								expand="true,false"
								fill="true,false"
								minimum="0.1"
								maximum="40" />
					</Sizer>
				</Sizer>
			</Composer>
			""";
		final Widget result = Composer.composerGenerateString(xml);
		assertNotNull(result, "Result should not be null");
		assertInstanceOf(Sizer.class, result, "Result should be a Sizer");
		// Check we can find named sub-objects
		assertNotNull(result.getSubObjectNamed("[555]HeighMap:slider-width"),
				"Should find the slider by name");
	}

	@Test
	void testSizerWithMultipleChildrenViaComposer() {
		// Test Sizer with multiple children loaded through composerGenerateString
		final String xml = """
			<Composer>
				<Sizer mode="HORIZONTAL">
					<Label>First</Label>
					<Spacer/>
					<Label>Last</Label>
				</Sizer>
			</Composer>
			""";
		final Widget result = Composer.composerGenerateString(xml);
		assertNotNull(result, "Result should not be null");
		assertInstanceOf(Sizer.class, result, "Result should be a Sizer");
		final Sizer sizer = (Sizer) result;
		assertEquals(3, sizer.getSubWidgets().size(), "Sizer should have 3 children");
	}
}
