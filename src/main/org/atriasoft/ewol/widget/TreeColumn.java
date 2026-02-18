/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

/**
 * Describes a column in a TreeView.
 * Column 0 is the "tree column" (indented, with chevrons).
 */
public class TreeColumn {
	private final String name;
	private float width;
	private final TreeCellRenderer renderer;

	/**
	 * Create a column definition.
	 * @param name The column header label.
	 * @param width The column width in pixels.
	 * @param renderer The cell renderer for this column.
	 */
	public TreeColumn(final String name, final float width, final TreeCellRenderer renderer) {
		this.name = name;
		this.width = width;
		this.renderer = renderer;
	}

	public String getName() {
		return this.name;
	}

	public float getWidth() {
		return this.width;
	}

	public void setWidth(final float width) {
		this.width = width;
	}

	public TreeCellRenderer getRenderer() {
		return this.renderer;
	}
}
