package test.atriasoft.ewol.compositing;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.atriasoft.ewol.compositing.TextBase;
import org.atriasoft.ewol.widget.Label;
import org.junit.jupiter.api.Test;

/** The text of a label is markup: a literal '<' needs an entity, and the existing markup is kept. */
class TextMarkupTest {
	@Test
	void escapeThenUnescapeGivesBackTheText() {
		final String text = "a < b && c > d \"q\" 'x'";
		assertEquals(text, TextBase.unescapeMarkup(TextBase.escapeMarkup(text)));
	}

	@Test
	void escapeProducesEntities() {
		assertEquals("1 &lt; 2 &amp; 3 &gt; 2", Label.escape("1 < 2 & 3 > 2"));
	}

	@Test
	void entitiesAreDecoded() {
		assertEquals("<>&\"'", TextBase.unescapeMarkup("&lt;&gt;&amp;&quot;&apos;"));
		assertEquals("é€", TextBase.unescapeMarkup("&#233;&#x20AC;"));
	}

	@Test
	void unknownSequencesAreKept() {
		assertEquals("Q&A; R&D &#zz; &;", TextBase.unescapeMarkup("Q&A; R&D &#zz; &;"));
		assertEquals("plain", TextBase.unescapeMarkup("plain"));
	}
}
