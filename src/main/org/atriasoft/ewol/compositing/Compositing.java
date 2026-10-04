package org.atriasoft.ewol.compositing;

import org.atriasoft.etk.math.Matrix4f;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.resource.OwnedResources;
import org.atriasoft.gale.resource.Resource;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

/**
 * Base of the drawing layers of the widgets. The gale resources of a layer
 * (vertex array, program, texture, font) are released once: when the widget
 * that holds it is collected by the garbage collector, or at once by
 * {@link #release()} when its owner knows it is drawn no more.
 */
public abstract class Compositing {
	protected Matrix4f matrixApply = Matrix4f.IDENTITY;
	/** The resources this layer created or kept; released with it. */
	private final OwnedResources resources = new OwnedResources(this);
	
	/**
	 * clear all the registered element in the current element
	 */
	public void clear() {
		this.matrixApply = Matrix4f.IDENTITY;
	}
	
	/**
	 * Virtual pure function that request the draw of all openGl elements
	 */
	public void draw() {
		draw(true);
	}
	
	public abstract void draw(final boolean disableDepthTest);
	
	/**
	 * Require the transfer of all the data in the Graphic card (does between the adding element and the draw)
	 */
	public abstract void flush();
	
	/**
	 * Whether the resources of this layer were released: it draws nothing any more.
	 * @return true once released.
	 */
	public boolean isReleased() {
		return this.resources.isReleased();
	}

	/**
	 * Take ownership of a resource created or kept for this layer: it is released
	 * with the layer.
	 * @param resource The resource (null accepted).
	 * @return {@code resource}.
	 */
	protected final <T extends Resource> T own(final T resource) {
		return this.resources.own(resource);
	}

	/**
	 * Release now the resources of this layer, when its owner knows it is drawn
	 * no more (otherwise the garbage collector does it when the layer is
	 * collected). The layer draws nothing afterwards; a second call does nothing.
	 */
	public void release() {
		this.resources.releaseAll();
	}

	/**
	 * Release now a resource owned by this layer, replaced by another one.
	 * @param resource The resource replaced (null accepted).
	 */
	protected final void releaseOwned(final Resource resource) {
		this.resources.releaseOwned(resource);
	}

	/**
	 * reset to the eye matrix the openGL moving system
	 */
	public void resetMatrix() {
		this.matrixApply = Matrix4f.IDENTITY;
	}
	
	/**
	 * rotate the curent display of this element
	 * @param vect The rotation vector to apply at the transformation matrix
	 */
	public void rotate(final Vector2f vect, final float angle) {
		this.matrixApply = this.matrixApply.multiply(Matrix4f.createMatrixRotate(vect, angle));
	}
	
	/**
	 * scale the current diaplsy of this element
	 * @param vect The scaling vector to apply at the transformation matrix
	 */
	public void scale(final Vector2f vect) {
		this.matrixApply = this.matrixApply.multiply(Matrix4f.createMatrixScale(vect));
	}
	
	/**
	 * set the transformation matrix
	 * @param mat The new matrix.
	 */
	public void setMatrix(final Matrix4f mat) {
		this.matrixApply = mat;
	}
	
	/**
	 * translate the current display of this element
	 * @param vect The translation vector to apply at the transformation matrix
	 */
	public void translate(final Vector2f vect) {
		this.matrixApply = this.matrixApply.multiply(Matrix4f.createMatrixTranslate(vect));
	}
}
