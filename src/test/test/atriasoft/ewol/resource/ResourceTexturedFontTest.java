package test.atriasoft.ewol.resource;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotSame;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

import org.atriasoft.etk.Uri;
import org.atriasoft.ewol.resource.ResourceTexturedFont;
import org.atriasoft.ewol.resource.font.FontMode;
import org.atriasoft.ewol.resource.font.GlyphProperty;
import org.atriasoft.gale.context.GaleContext;
import org.atriasoft.gale.resource.ResourceManager;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import test.atriasoft.ewol.EwolTestContext;

/** A textured font that cannot be built leaves nothing in the cache of the resources (no OpenGL needed). */
class ResourceTexturedFontTest {
	/** Name prefix of the textured fonts in the cache (ResourceTexturedFont.CACHE_PREFIX). */
	private static final String TEXTURED_FONT_PREFIX = "__TEXTURED_FONT__>>";

	@BeforeAll
	static void setUpClass() {
		EwolTestContext.init();
	}

	@Test
	void accentedCharactersAndTheEuroSignHaveTheirOwnGlyph() {
		final Uri fontUri = new Uri("FONTS", "FreeSherif.svg", "esvg");
		fontUri.setProperty("size", "14");
		final ResourceTexturedFont font = ResourceTexturedFont.create(fontUri);
		try {
			final GlyphProperty error = font.getGlyph((char) 0, FontMode.REGULAR);
			final String wanted = "éèàçôüßœ€";
			for (int iii = 0; iii < wanted.length(); iii++) {
				final char code = wanted.charAt(iii);
				// twice: the first call adds the glyph to the atlas, the second finds it
				for (int pass = 0; pass < 2; pass++) {
					final GlyphProperty glyph = font.getGlyph(code, FontMode.REGULAR);
					assertNotSame(error, glyph, "error glyph for U+" + Integer.toHexString(code) + " pass " + pass);
					assertEquals(code, glyph.getUnicodeValue());
					assertTrue(glyph.exist());
				}
			}
			assertEquals('A', font.getGlyph('A', FontMode.BOLD).getUnicodeValue());
			assertEquals('~', font.getGlyph('~', FontMode.REGULAR).getUnicodeValue());
		} finally {
			font.release();
		}
	}

	@Test
	void aFontThatFailsToBuildLeavesTheCacheWithItsSvgFont(@TempDir final Path folder) throws IOException {
		// An SVG with no font: the SVG font loads as null and the textured font fails on its height.
		final Path file = folder.resolve("Broken.svg");
		Files.writeString(file, "<svg xmlns=\"http://www.w3.org/2000/svg\"></svg>", StandardCharsets.UTF_8);
		final Uri fontUri = new Uri("FILE", file.toString());
		fontUri.setProperty("size", "14");
		final Uri svgFontKey = fontUri.clone();
		svgFontKey.getproperties().remove("size");

		assertThrows(RuntimeException.class, () -> ResourceTexturedFont.create(fontUri));

		final ResourceManager manager = GaleContext.getContext().getResourcesManager();
		assertNull(manager.localKeep(TEXTURED_FONT_PREFIX + fontUri.toString()), "the half-built textured font left");
		assertNull(manager.localKeep(svgFontKey.toString()), "the SVG font it kept was given back");
	}
}
