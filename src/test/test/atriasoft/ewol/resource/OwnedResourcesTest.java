package test.atriasoft.ewol.resource;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertTrue;

import org.atriasoft.etk.Uri;
import org.atriasoft.ewol.compositing.CompositingDrawing;
import org.atriasoft.ewol.compositing.CompositingGC;
import org.atriasoft.ewol.compositing.CompositingText;
import org.atriasoft.ewol.resource.OwnedResources;
import org.atriasoft.ewol.resource.ResourceColorFile;
import org.atriasoft.ewol.resource.ResourceTexturedFont;
import org.atriasoft.gale.resource.Resource;
import org.atriasoft.gale.resource.ResourceProgram;
import org.atriasoft.gale.resource.ResourceVirtualArrayObject;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import test.atriasoft.ewol.EwolTestContext;

/**
 * Release of the resources owned by ewol objects (layers, widgets), without
 * OpenGL: a released resource leaves the manager at once.
 */
class OwnedResourcesTest {

	/** A resource with no OpenGL object. */
	private static final class PlainResource extends Resource {
		PlainResource(final String name) {
			super(name);
		}

		@Override
		public void cleanUp() {}
	}

	/** A shapes layer showing its resources. */
	private static final class ShapesProbe extends CompositingGC {
		ResourceVirtualArrayObject vertexArray() {
			return this.vbo;
		}

		ResourceProgram program() {
			return this.oGLprogram;
		}
	}

	/** A text layer showing its resources. */
	private static final class TextProbe extends CompositingText {
		ResourceVirtualArrayObject vertexArray() {
			return this.vbo;
		}

		CompositingDrawing background() {
			return this.vectorialDraw;
		}

		ResourceTexturedFont font() {
			return this.font;
		}
	}

	@BeforeAll
	static void setUpClass() {
		EwolTestContext.init();
	}

	/** Give {@code resource} to an owner that nothing references once this method returns. */
	private static void ownByForgottenOwner(final Resource resource) {
		new OwnedResources(new Object()).own(resource);
	}

	@Test
	void releaseAllReleasesEachResourceOnce() {
		final OwnedResources owned = new OwnedResources(new Object());
		final PlainResource alone = owned.own(new PlainResource("owned-alone"));
		final PlainResource shared = new PlainResource("owned-shared");
		shared.keep();
		owned.own(shared);
		owned.releaseAll();
		assertTrue(owned.isReleased());
		assertTrue(alone.isReleased());
		assertFalse(shared.isReleased(), "the other user keeps it");
		assertEquals(1, shared.getCount());
		owned.releaseAll();
		assertEquals(1, shared.getCount(), "a second releaseAll does nothing");
		shared.release();
	}

	@Test
	void releaseOwnedReleasesTheReplacedResourceOnly() {
		final OwnedResources owned = new OwnedResources(new Object());
		final PlainResource first = owned.own(new PlainResource("owned-first"));
		final PlainResource second = owned.own(new PlainResource("owned-second"));
		owned.releaseOwned(first);
		assertTrue(first.isReleased());
		assertFalse(second.isReleased());
		owned.releaseOwned(first);
		owned.releaseAll();
		assertTrue(second.isReleased());
	}

	@Test
	void aResourceGivenToAReleasedOwnerIsReleasedAtOnce() {
		final OwnedResources owned = new OwnedResources(new Object());
		owned.releaseAll();
		final PlainResource late = owned.own(new PlainResource("owned-late"));
		assertTrue(late.isReleased());
	}

	@Test
	void theCollectionOfTheOwnerReleasesItsResources() throws InterruptedException {
		final PlainResource resource = new PlainResource("owned-collected");
		ownByForgottenOwner(resource);
		for (int iii = 0; iii < 200 && !resource.isReleased(); iii++) {
			System.gc();
			Thread.sleep(10);
		}
		assertTrue(resource.isReleased(), "released by the cleaner once the owner is collected");
	}

	@Test
	void aShapesLayerReleasesItsVertexArrayAndItsProgramReference() {
		final ShapesProbe keeper = new ShapesProbe();
		final ShapesProbe layer = new ShapesProbe();
		final ResourceProgram program = layer.program();
		assertSame(keeper.program(), program, "the program is shared");
		final int users = program.getCount();
		layer.release();
		assertTrue(layer.isReleased());
		assertTrue(layer.vertexArray().isReleased());
		assertEquals(users - 1, program.getCount());
		assertFalse(program.isReleased());
		layer.release();
		assertEquals(users - 1, program.getCount(), "a second release does nothing");
		keeper.release();
	}

	@Test
	void aTextLayerReleasesItsArraysAndItsFontReference() {
		final TextProbe keeper = new TextProbe();
		final TextProbe text = new TextProbe();
		final ResourceTexturedFont font = text.font();
		assertNotNull(font);
		assertSame(keeper.font(), font, "the font is shared and kept for each user");
		final int users = font.getCount();
		text.release();
		assertTrue(text.vertexArray().isReleased());
		assertTrue(text.background().isReleased());
		assertEquals(users - 1, font.getCount());
		assertFalse(font.isReleased());
		keeper.release();
	}

	@Test
	void theColorFileFactoryKeepsTheSharedFile() {
		final Uri uri = new Uri("THEME", "/color/Label.json", "ewol");
		final ResourceColorFile first = ResourceColorFile.create(uri);
		final int users = first.getCount();
		final ResourceColorFile second = ResourceColorFile.create(uri);
		assertSame(first, second);
		assertEquals(users + 1, first.getCount());
		second.release();
		assertFalse(first.isReleased(), "the first user still has it");
		first.release();
	}
}
