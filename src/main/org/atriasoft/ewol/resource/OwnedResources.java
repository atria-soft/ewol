/** @file
 * @author Edouard DUPIN
 * @copyright 2026, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.resource;

import java.lang.ref.Cleaner;
import java.util.ArrayList;
import java.util.List;

import org.atriasoft.gale.resource.Resource;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * The gale resources an object owns: each one created or kept for it
 * ({@code create()}, {@code keep()}), released exactly once.
 * <p>
 * A resource is released when the owner replaces it ({@link #releaseOwned}),
 * when the owner is known to be dead ({@link #releaseAll()}), otherwise when the
 * garbage collector collects the owner. ewol widgets have no destruction step:
 * they die when nothing references them any more (a widget taken out of the
 * tree can be put back), so the collection of the owner is the only sure end of
 * its resources. An owner still reachable, thus still drawn, is never collected.
 * <p>
 * Thread safety: every method may be called from any thread; the releases of
 * the collector run on the cleaner thread, and gale deletes the OpenGL objects
 * on the OpenGL thread.
 */
public final class OwnedResources {
	private static final Logger LOGGER = LoggerFactory.getLogger(OwnedResources.class);
	private static final Cleaner CLEANER = Cleaner.create();

	/**
	 * The resources themselves, apart from the owner: the cleaner references it,
	 * so it must never reference the owner (which would never be collected).
	 */
	private static final class State implements Runnable {
		private final List<Resource> resources = new ArrayList<>();
		private boolean released = false;

		synchronized boolean add(final Resource resource) {
			if (this.released) {
				return false;
			}
			this.resources.add(resource);
			return true;
		}

		synchronized boolean isReleased() {
			return this.released;
		}

		synchronized boolean remove(final Resource resource) {
			for (int iii = this.resources.size() - 1; iii >= 0; iii--) {
				if (this.resources.get(iii) == resource) {
					this.resources.remove(iii);
					return true;
				}
			}
			return false;
		}

		@Override
		public void run() {
			final List<Resource> toRelease;
			synchronized (this) {
				this.released = true;
				toRelease = new ArrayList<>(this.resources);
				this.resources.clear();
			}
			for (int iii = toRelease.size() - 1; iii >= 0; iii--) {
				releaseOnce(toRelease.get(iii));
			}
		}
	}

	/**
	 * Release one reference of {@code resource}. A resource released already (by
	 * code that freed it on its own) is left alone.
	 */
	static void releaseOnce(final Resource resource) {
		if (resource.isReleased()) {
			return;
		}
		try {
			resource.release();
		} catch (final RuntimeException ex) {
			// No context any more (end of the application): the OpenGL objects are gone with it.
			LOGGER.warn("Cannot release the resource [{}] '{}': {}", resource.getId(), resource.getName(), ex.toString());
		}
	}

	private final State state = new State();
	private final Cleaner.Cleanable cleanable;

	/**
	 * @param owner Object whose collection releases the resources still owned.
	 */
	public OwnedResources(final Object owner) {
		this.cleanable = OwnedResources.CLEANER.register(owner, this.state);
	}

	/**
	 * Whether the resources were released ({@link #releaseAll()} or collection
	 * of the owner): the owner must not draw any more.
	 * @return true once released.
	 */
	public boolean isReleased() {
		return this.state.isReleased();
	}

	/**
	 * Take one reference of a resource created or kept for the owner. Once the
	 * owner is released, the resource is released at once.
	 * @param resource The resource (null accepted, nothing is done).
	 * @return {@code resource}, for chained affectations.
	 */
	public <T extends Resource> T own(final T resource) {
		if (resource == null) {
			return null;
		}
		if (!this.state.add(resource)) {
			LOGGER.warn("The resource [{}] '{}' is given to an owner released already: released at once",
					resource.getId(), resource.getName());
			releaseOnce(resource);
		}
		return resource;
	}

	/**
	 * Release now all the resources owned; the owner must not use them any more.
	 * Called more than once, or after the collection, it does nothing.
	 */
	public void releaseAll() {
		this.cleanable.clean();
	}

	/**
	 * Release now one reference owned of {@code resource}, replaced by the owner.
	 * A resource not owned (released already by {@link #releaseAll()}) is left
	 * alone.
	 * @param resource The resource replaced (null accepted, nothing is done).
	 */
	public void releaseOwned(final Resource resource) {
		if (resource == null) {
			return;
		}
		if (this.state.remove(resource)) {
			releaseOnce(resource);
		}
	}
}
