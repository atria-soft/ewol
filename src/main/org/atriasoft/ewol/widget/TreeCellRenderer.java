/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.compositing.CompositingDrawing;
import org.atriasoft.ewol.compositing.CompositingText;

/**
 * Functional interface for rendering a cell in a TreeView column.
 */
@FunctionalInterface
public interface TreeCellRenderer {
	/**
	 * Render a cell in the tree view.
	 * @param gc The drawing compositing for shapes/backgrounds.
	 * @param text The text compositing for text rendering.
	 * @param node The tree node being rendered.
	 * @param pos The top-left position of the cell (in widget coordinates).
	 * @param size The size allocated for the cell.
	 * @param selected Whether this node is currently selected.
	 */
	void render(
			final CompositingDrawing gc,
			final CompositingText text,
			final TreeNode<?> node,
			final Vector2f pos,
			final Vector2f size,
			final boolean selected);
}
