package org.atriasoft.ewol.widget.meta;

/**
 * Position of the Popover relative to its anchor point.
 */
public enum PopoverPosition {
	/** Popover appears above the anchor, arrow points down. */
	TOP,
	/** Popover appears below the anchor, arrow points up. */
	BOTTOM,
	/** Popover appears to the left of the anchor, arrow points right. */
	LEFT,
	/** Popover appears to the right of the anchor, arrow points left. */
	RIGHT,
	/** Auto-detect the best position based on available space. */
	AUTO
}
