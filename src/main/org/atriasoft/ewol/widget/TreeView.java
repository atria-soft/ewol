/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.function.Consumer;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.dataformat.xml.annotation.JacksonXmlProperty;
import org.atriasoft.esignal.Connection;
import org.atriasoft.esignal.Signal;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.compositing.CompositingDrawing;
import org.atriasoft.ewol.compositing.CompositingGC;
import org.atriasoft.ewol.compositing.CompositingText;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.resource.ResourceColorFile;
import org.atriasoft.gale.key.KeyStatus;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Generic tree view widget with configurable columns.
 * Extends WidgetScrolled for scroll management.
 * Draws everything via CompositingGC + CompositingText (no child widgets per row).
 */
public class TreeView extends WidgetScrolled {
	private static final Logger LOGGER = LoggerFactory.getLogger(TreeView.class);

	// ========================
	// Constants
	// ========================
	private static final float INDENT_WIDTH = 20.0f;
	private static final float CHEVRON_SIZE = 10.0f;
	private static final float CHEVRON_MARGIN = 4.0f;
	private static final float ROW_PADDING_Y = 2.0f;
	private static final float ROW_PADDING_X = 2.0f;

	// ========================
	// Compositing objects
	// ========================
	private final CompositingGC compositingDrawing;
	private final CompositingText compositingText;

	// ========================
	// Signals
	// ========================
	public final Signal<TreeNode<?>> signalSelect = new Signal<>();
	public final Signal<TreeNode<?>> signalExpand = new Signal<>();
	public final Signal<TreeNode<?>> signalCollapse = new Signal<>();
	@SuppressWarnings("rawtypes")
	public final Signal<TreeColumnAction> signalColumnAction = new Signal<>();

	// ========================
	// Data model
	// ========================
	private TreeNode<?> rootNode;
	private boolean showRoot = true;

	// ========================
	// Column model
	// ========================
	private final List<TreeColumn> columns = new ArrayList<>();

	// ========================
	// Flattened visible rows
	// ========================
	private final List<TreeNode<?>> visibleRows = new ArrayList<>();

	// ========================
	// Selection
	// ========================
	private TreeNode<?> selectedNode;

	// ========================
	// Color theming
	// ========================
	private final ResourceColorFile colorProperty;
	private final int colorIdText;
	private final int colorIdBackground1;
	private final int colorIdBackground2;
	private final int colorIdBackgroundSelected;
	private final int colorIdChevron;
	private final int colorIdHeaderBg;
	private final int colorIdHeaderText;
	private final int colorIdIndentGuide;
	private final int colorIdIndentGuideHover;

	// ========================
	// Row metrics
	// ========================
	private float rowHeight = 0.0f;
	private boolean showHeaders = true;
	private boolean showAlternateRowBackground = false;
	private float headerHeight = 0.0f;

	// ========================
	// Column resize state
	// ========================
	private static final float COLUMN_RESIZE_ZONE = 5.0f;
	private int resizingColumnIndex = -1;
	private float resizeDragStartX = 0.0f;
	private float resizeDragStartWidth = 0.0f;
	private static final float COLUMN_MIN_WIDTH = 30.0f;

	// ========================
	// Indent guide hover state
	// ========================
	private int hoveredGuideDepth = -1;
	private int hoveredGuideParentRowIndex = -1;

	// ========================
	// Fluent API connection storage
	// ========================
	private final List<Connection> fluentConnections = new ArrayList<>();

	public TreeView() {
		this.compositingDrawing = new CompositingGC();
		this.compositingText = new CompositingText();
		this.propertyCanFocus = true;
		this.limitScrolling = new Vector2f(1.0f, 1.0f);
		setScrollingSize(80);
		setMouseLimit(2);

		// Load color theme
		this.colorProperty = own(ResourceColorFile.create(
				new Uri("THEME", "/color/TreeView.json", "ewol")));
		this.colorIdText = this.colorProperty.request("text");
		this.colorIdBackground1 = this.colorProperty.request("background1");
		this.colorIdBackground2 = this.colorProperty.request("background2");
		this.colorIdBackgroundSelected = this.colorProperty.request("selected");
		this.colorIdChevron = this.colorProperty.request("chevron");
		this.colorIdHeaderBg = this.colorProperty.request("headerBg");
		this.colorIdHeaderText = this.colorProperty.request("headerText");
		this.colorIdIndentGuide = this.colorProperty.request("indentGuide");
		this.colorIdIndentGuideHover = this.colorProperty.request("indentGuideHover");
	}

	// ========================================================================
	// Data model
	// ========================================================================

	public void setRootNode(final TreeNode<?> root) {
		this.rootNode = root;
		this.selectedNode = null;
		markToRedraw();
	}

	public TreeNode<?> getRootNode() {
		return this.rootNode;
	}

	// ========================================================================
	// Column management
	// ========================================================================

	public void addColumn(final TreeColumn column) {
		this.columns.add(column);
		markToRedraw();
	}

	public void clearColumns() {
		this.columns.clear();
		markToRedraw();
	}

	public List<TreeColumn> getColumns() {
		return Collections.unmodifiableList(this.columns);
	}

	// ========================================================================
	// Selection
	// ========================================================================

	public TreeNode<?> getSelectedNode() {
		return this.selectedNode;
	}

	public void setSelectedNode(final TreeNode<?> node) {
		selectNode(node);
	}

	private void selectNode(final TreeNode<?> node) {
		if (this.selectedNode != node) {
			this.selectedNode = node;
			this.signalSelect.emit(node);
			markToRedraw();
		}
	}

	// ========================================================================
	// Display options
	// ========================================================================

	@JsonProperty("show-root")
	@JacksonXmlProperty(isAttribute = true, localName = "show-root")
	public boolean isShowRoot() {
		return this.showRoot;
	}

	public void setShowRoot(final boolean show) {
		if (this.showRoot == show) {
			return;
		}
		this.showRoot = show;
		markToRedraw();
	}

	@JsonProperty("show-headers")
	@JacksonXmlProperty(isAttribute = true, localName = "show-headers")
	public boolean isShowHeaders() {
		return this.showHeaders;
	}

	public void setShowHeaders(final boolean show) {
		if (this.showHeaders == show) {
			return;
		}
		this.showHeaders = show;
		markToRedraw();
	}

	@JsonProperty("show-alternate-row-background")
	@JacksonXmlProperty(isAttribute = true, localName = "show-alternate-row-background")
	public boolean isShowAlternateRowBackground() {
		return this.showAlternateRowBackground;
	}

	public void setShowAlternateRowBackground(final boolean show) {
		if (this.showAlternateRowBackground == show) {
			return;
		}
		this.showAlternateRowBackground = show;
		markToRedraw();
	}

	// ========================================================================
	// Expand / Collapse
	// ========================================================================

	public void expandAll() {
		if (this.rootNode != null) {
			expandAllRecursive(this.rootNode);
			markToRedraw();
		}
	}

	public void collapseAll() {
		if (this.rootNode != null) {
			collapseAllRecursive(this.rootNode);
			markToRedraw();
		}
	}

	private void expandAllRecursive(final TreeNode<?> node) {
		node.setExpanded(true);
		for (final TreeNode<?> child : node.getChildren()) {
			expandAllRecursive(child);
		}
	}

	private void collapseAllRecursive(final TreeNode<?> node) {
		node.setExpanded(false);
		for (final TreeNode<?> child : node.getChildren()) {
			collapseAllRecursive(child);
		}
	}

	// ========================================================================
	// Tree flattening
	// ========================================================================

	private void flattenTree() {
		this.visibleRows.clear();
		if (this.rootNode == null) {
			return;
		}
		if (this.showRoot) {
			this.visibleRows.add(this.rootNode);
		}
		if (this.rootNode.isExpanded() || !this.showRoot) {
			flattenChildren(this.rootNode);
		}
	}

	private void flattenChildren(final TreeNode<?> node) {
		for (final TreeNode<?> child : node.getChildren()) {
			this.visibleRows.add(child);
			if (child.isExpanded() && child.hasChildren()) {
				flattenChildren(child);
			}
		}
	}

	// ========================================================================
	// Indent guide helpers
	// ========================================================================

	private static final float INDENT_GUIDE_WIDTH = 1.0f;

	/**
	 * Check if a vertical indent guide line should be drawn at a given depth
	 * for the specified row node. A guide line appears at depth d if the
	 * ancestor of this node at depth d+1 is NOT the last child of its parent
	 * (meaning there are more siblings below at that level).
	 */
	private boolean shouldDrawGuideAtDepth(final TreeNode<?> node,
			final int depth, final int baseDepth) {
		// Walk up from node to find the ancestor at (baseDepth + depth + 1)
		TreeNode<?> ancestor = node;
		final int nodeDepth = node.getDepth() - baseDepth;
		// Walk up (nodeDepth - depth - 1) levels
		for (int i = 0; i < nodeDepth - depth - 1; i++) {
			if (ancestor.getParent() == null) {
				return false;
			}
			ancestor = ancestor.getParent();
		}
		// ancestor is now at (baseDepth + depth + 1)
		// Check if it's NOT the last child of its parent
		final TreeNode<?> parent = ancestor.getParent();
		if (parent == null) {
			return false;
		}
		final List<?> siblings = parent.getChildren();
		return siblings.indexOf(ancestor) < siblings.size() - 1;
	}

	/**
	 * Find the expanded ancestor node at a given depth for a row node.
	 * Returns the ancestor at (baseDepth + depth + 1) — the one whose
	 * parent is at (baseDepth + depth) and shows the guide line.
	 */
	private TreeNode<?> findAncestorAtDepth(final TreeNode<?> node,
			final int depth, final int baseDepth) {
		TreeNode<?> ancestor = node;
		final int nodeDepth = node.getDepth() - baseDepth;
		for (int i = 0; i < nodeDepth - depth - 1; i++) {
			if (ancestor.getParent() == null) {
				return null;
			}
			ancestor = ancestor.getParent();
		}
		return ancestor;
	}

	/**
	 * Find the parent row index in visibleRows for a given guide depth.
	 * This is the row of the expanded folder that "owns" the guide line.
	 */
	private int findGuideParentRowIndex(final TreeNode<?> node,
			final int depth, final int baseDepth) {
		// The parent folder is at (baseDepth + depth), which is
		// the parent of the ancestor at (baseDepth + depth + 1).
		final TreeNode<?> ancestor = findAncestorAtDepth(
				node, depth, baseDepth);
		if (ancestor == null) {
			return -1;
		}
		final TreeNode<?> parentFolder = ancestor.getParent();
		if (parentFolder == null) {
			return -1;
		}
		return this.visibleRows.indexOf(parentFolder);
	}

	/**
	 * Find the next sibling node at a given guide depth after
	 * the current row's ancestor at that depth.
	 * Returns the next sibling, or null if none.
	 */
	private TreeNode<?> findNextSiblingAtGuideDepth(final TreeNode<?> node,
			final int depth, final int baseDepth) {
		final TreeNode<?> ancestor = findAncestorAtDepth(
				node, depth, baseDepth);
		if (ancestor == null) {
			return null;
		}
		final TreeNode<?> parent = ancestor.getParent();
		if (parent == null) {
			return null;
		}
		final List<?> siblings = parent.getChildren();
		final int idx = siblings.indexOf(ancestor);
		if (idx >= 0 && idx < siblings.size() - 1) {
			return (TreeNode<?>) siblings.get(idx + 1);
		}
		return null;
	}

	// ========================================================================
	// Sizing
	// ========================================================================

	@Override
	public void calculateMinMaxSize() {
		this.minSize = new Vector2f(200, 150);
	}

	private float calculateRowHeight() {
		final Vector2f charSize = this.compositingText.calculateSize("Ay");
		return charSize.y() + ROW_PADDING_Y * 3;
	}

	// ========================================================================
	// Rendering
	// ========================================================================

	@Override
	public void onRegenerateDisplay() {
		if (!needRedraw()) {
			return;
		}

		// 1. Clear compositing objects
		this.compositingDrawing.clear();
		this.compositingText.clear();

		// 2. Flatten the tree into visible rows
		flattenTree();

		// 3. Calculate row height from text metrics
		this.rowHeight = calculateRowHeight();
		this.headerHeight = this.showHeaders && !this.columns.isEmpty()
				? this.rowHeight + 4.0f : 0.0f;

		// 4. Calculate total content size for scroll management
		// maxSize is the scrollable content area (excluding fixed header)
		final float totalHeight = this.visibleRows.size() * this.rowHeight;
		float totalWidth = 0.0f;
		for (final TreeColumn col : this.columns) {
			totalWidth += col.getWidth();
		}
		if (totalWidth < this.size.x()) {
			totalWidth = this.size.x();
		}
		this.maxSize = new Vector2f(totalWidth, totalHeight);

		// 5. Draw background
		drawBackground();

		// 6. Draw visible rows (clipped to viewport, below header)
		drawRows();

		// 7. Draw column headers on top (fixed, not scrolled)
		if (this.showHeaders && !this.columns.isEmpty()) {
			drawHeaders();
		}

		// 8. Draw scrollbars via parent
		super.onRegenerateDisplay();

		// 9. Flush all compositing to GPU
		this.compositingDrawing.flush();
		this.compositingText.flush();
	}

	private void drawBackground() {
		final Color bgColor = this.colorProperty.get(this.colorIdBackground1);
		this.compositingDrawing.setColor(bgColor);
		this.compositingDrawing.setPos(Vector2f.ZERO);
		this.compositingDrawing.rectangleWidth(
				new Vector2f(this.size.x(), this.size.y()));
	}

	private void drawHeaders() {
		// Header is always at top of visible area (not scrolled)
		final float headerY = this.size.y() - this.headerHeight;

		// Header background
		final Color headerBg = this.colorProperty.get(this.colorIdHeaderBg);
		this.compositingDrawing.setColor(headerBg);
		this.compositingDrawing.setPos(new Vector2f(0, headerY));
		this.compositingDrawing.rectangleWidth(
				new Vector2f(this.size.x(), this.headerHeight));

		// Header text and column separators
		final Color headerTextColor = this.colorProperty.get(
				this.colorIdHeaderText);
		this.compositingText.setColor(headerTextColor);
		float xOffset = -this.originScrooled.x();
		for (final TreeColumn col : this.columns) {
			if (xOffset + col.getWidth() > 0 && xOffset < this.size.x()) {
				this.compositingText.setClippingWidth(
						new Vector2f(xOffset, headerY),
						new Vector2f(col.getWidth(), this.headerHeight));
				this.compositingText.setPos(
						new Vector2f(xOffset + ROW_PADDING_X,
								headerY + ROW_PADDING_Y));
				this.compositingText.print(col.getName());
				this.compositingText.setClippingMode(false);

				// Draw column separator line on the right edge
				final float separatorX = xOffset + col.getWidth() - 1.0f;
				if (separatorX > 0 && separatorX < this.size.x()) {
					this.compositingDrawing.setColor(headerTextColor);
					this.compositingDrawing.setPos(
							new Vector2f(separatorX, headerY));
					this.compositingDrawing.rectangleWidth(
							new Vector2f(1.0f, this.headerHeight));
				}
			}
			xOffset += col.getWidth();
		}
	}

	private void drawRows() {
		final Color textColor = this.colorProperty.get(this.colorIdText);
		final float viewportTop = this.size.y() - this.headerHeight;

		// Clip all row content below the header area so nothing
		// bleeds into the fixed header zone
		this.compositingDrawing.setClippingWidth(
				new Vector2f(0, 0), new Vector2f(this.size.x(), viewportTop));

		for (int rowIndex = 0; rowIndex < this.visibleRows.size(); rowIndex++) {
			final TreeNode<?> node = this.visibleRows.get(rowIndex);

			// Calculate Y position (top-down, OpenGL Y=0 at bottom)
			// Row 0 starts just below the header
			final float cumulativeY = rowIndex * this.rowHeight;
			final float startY = viewportTop + this.originScrooled.y()
					- cumulativeY - this.rowHeight;

			// Viewport clipping — skip rows below visible area
			if (startY + this.rowHeight < 0) {
				break;
			}
			// Skip rows above viewport (hidden behind header)
			if (startY >= viewportTop) {
				continue;
			}

			final boolean isSelected =
					(this.selectedNode != null && this.selectedNode == node);
			final int depth = node.getDepth() - (this.showRoot ? 0 : 1);

			// Row background
			drawRowBackground(rowIndex, startY, isSelected);

			// Draw indent guide lines at this row
			drawIndentGuides(node, depth, startY);

			// Set text color
			this.compositingText.setColor(textColor);

			// Draw columns
			float xOffset = -this.originScrooled.x();
			for (int colIndex = 0; colIndex < this.columns.size(); colIndex++) {
				final TreeColumn col = this.columns.get(colIndex);
				final float cellX = xOffset;
				final float cellWidth = col.getWidth();

				// Compute text clip height capped at viewport top
				final float clipBottom = Math.max(startY, 0);
				final float clipTop = Math.min(
						startY + this.rowHeight, viewportTop);
				final float clipHeight = clipTop - clipBottom;

				if (colIndex == 0) {
					// Tree column: indent + chevron + renderer
					final float indent = depth * INDENT_WIDTH;
					drawChevron(node, cellX + indent, startY);
					final float contentX = cellX + indent
							+ CHEVRON_SIZE + CHEVRON_MARGIN * 2;
					final float contentWidth = cellWidth - indent
							- CHEVRON_SIZE - CHEVRON_MARGIN * 2;
					if (contentWidth > 0 && clipHeight > 0) {
						this.compositingText.setClipping(
								new Vector2f(contentX, clipBottom),
								new Vector2f(contentX + contentWidth,
										clipTop));
						col.getRenderer().render(
								this.compositingDrawing,
								this.compositingText,
								node,
								new Vector2f(contentX,
										startY + ROW_PADDING_Y),
								new Vector2f(contentWidth,
										this.rowHeight - ROW_PADDING_Y * 2),
								isSelected);
						this.compositingText.setClippingMode(false);
					}
				} else {
					// Non-tree columns
					if (clipHeight > 0) {
						this.compositingText.setClipping(
								new Vector2f(cellX, clipBottom),
								new Vector2f(cellX + cellWidth, clipTop));
						col.getRenderer().render(
								this.compositingDrawing,
								this.compositingText,
								node,
								new Vector2f(cellX + ROW_PADDING_X,
										startY + ROW_PADDING_Y),
								new Vector2f(
										cellWidth - ROW_PADDING_X * 2,
										this.rowHeight - ROW_PADDING_Y * 2),
								isSelected);
						this.compositingText.setClippingMode(false);
					}
				}

				// Reset text color after renderer may have changed it
				this.compositingText.setColor(textColor);
				xOffset += cellWidth;
			}
		}

		// Disable viewport clipping after drawing all rows
		this.compositingDrawing.setClippingMode(false);
	}

	private void drawRowBackground(final int rowIndex, final float startY,
			final boolean isSelected) {
		final Color bgColor;
		if (isSelected) {
			bgColor = this.colorProperty.get(this.colorIdBackgroundSelected);
		} else if (this.showAlternateRowBackground && rowIndex % 2 != 0) {
			bgColor = this.colorProperty.get(this.colorIdBackground2);
		} else {
			bgColor = this.colorProperty.get(this.colorIdBackground1);
		}
		this.compositingDrawing.setColor(bgColor);
		this.compositingDrawing.setPos(new Vector2f(0, startY));
		this.compositingDrawing.rectangleWidth(
				new Vector2f(this.size.x(), this.rowHeight));
	}

	/**
	 * Draw the expand/collapse chevron for a node.
	 * In OpenGL coordinates: Y=0 is bottom, Y grows upward.
	 * Collapsed = right-pointing triangle (>)
	 * Expanded = down-pointing triangle (v)
	 *
	 * Note: uses setPos+addVertex instead of drawQuad because
	 * drawQuad does not call internalSetColor (bug in CompositingDrawing).
	 */
	private void drawChevron(final TreeNode<?> node, final float x,
			final float rowY) {
		if (node.isLeaf() && !node.hasChildren()) {
			return;
		}

		final Color chevronColor = this.colorProperty.get(this.colorIdChevron);
		this.compositingDrawing.setColor(chevronColor);

		final float centerX = x + CHEVRON_MARGIN + CHEVRON_SIZE / 2.0f;
		final float centerY = rowY + this.rowHeight / 2.0f;
		final float h = CHEVRON_SIZE * 0.4f;

		if (node.isExpanded()) {
			// Down-pointing triangle (v)
			this.compositingDrawing.setPos(
					new Vector3f(centerX - h, centerY + h * 0.5f, 0));
			this.compositingDrawing.addVertex();
			this.compositingDrawing.setPos(
					new Vector3f(centerX + h, centerY + h * 0.5f, 0));
			this.compositingDrawing.addVertex();
			this.compositingDrawing.setPos(
					new Vector3f(centerX, centerY - h * 0.5f, 0));
			this.compositingDrawing.addVertex();
		} else {
			// Right-pointing triangle (>)
			this.compositingDrawing.setPos(
					new Vector3f(centerX - h * 0.5f, centerY + h, 0));
			this.compositingDrawing.addVertex();
			this.compositingDrawing.setPos(
					new Vector3f(centerX - h * 0.5f, centerY - h, 0));
			this.compositingDrawing.addVertex();
			this.compositingDrawing.setPos(
					new Vector3f(centerX + h * 0.5f, centerY, 0));
			this.compositingDrawing.addVertex();
		}
	}

	/**
	 * Draw vertical indent guide lines for a row.
	 * For each depth level d from 0 to (depth-1), draw a vertical line
	 * if there are more siblings below at that depth level.
	 */
	private void drawIndentGuides(final TreeNode<?> node, final int depth,
			final float startY) {
		final int baseDepth = this.showRoot ? 0 : 1;
		final Color guideColor = this.colorProperty.get(
				this.colorIdIndentGuide);
		final Color guideHoverColor = this.colorProperty.get(
				this.colorIdIndentGuideHover);

		for (int d = 0; d < depth; d++) {
			if (!shouldDrawGuideAtDepth(node, d, baseDepth)) {
				continue;
			}
			// X position: center of the indent zone at depth d
			final float guideX = (d + 0.5f) * INDENT_WIDTH
					- this.originScrooled.x();
			if (guideX < -INDENT_GUIDE_WIDTH || guideX > this.size.x()) {
				continue;
			}

			// Check if this guide is hovered
			final boolean isHovered = (d == this.hoveredGuideDepth);

			this.compositingDrawing.setColor(
					isHovered ? guideHoverColor : guideColor);
			this.compositingDrawing.setPos(
					new Vector2f(guideX - INDENT_GUIDE_WIDTH * 0.5f,
							startY));
			this.compositingDrawing.rectangleWidth(
					new Vector2f(INDENT_GUIDE_WIDTH, this.rowHeight));
		}
	}

	// ========================================================================
	// Drawing
	// ========================================================================

	@Override
	protected void onDraw() {
		this.compositingDrawing.draw();
		this.compositingText.draw();
		super.onDraw();
	}

	// ========================================================================
	// Event handling
	// ========================================================================

	@Override
	public boolean onEventInput(final EventInput event) {
		final Vector2f relativePos = relativePosition(
				new Vector2f(event.pos().x(), event.pos().y()));

		// Handle column resize drag (must be checked before super
		// to intercept move/up events during drag)
		if (this.resizingColumnIndex >= 0) {
			if (event.inputId() == 1 && event.status() == KeyStatus.move) {
				final float deltaX = relativePos.x() - this.resizeDragStartX;
				final float newWidth = Math.max(COLUMN_MIN_WIDTH,
						this.resizeDragStartWidth + deltaX);
				this.columns.get(this.resizingColumnIndex).setWidth(newWidth);
				markToRedraw();
				return true;
			}
			if (event.inputId() == 1 && event.status() == KeyStatus.up) {
				this.resizingColumnIndex = -1;
				return true;
			}
		}

		// Let parent handle scrollbar events first
		if (super.onEventInput(event)) {
			keepFocus();
			return true;
		}

		// Check for column resize initiation (mouse down on header border)
		if (event.inputId() == 1 && event.status() == KeyStatus.down
				&& this.showHeaders && !this.columns.isEmpty()) {
			final float viewportTop = this.size.y() - this.headerHeight;
			if (relativePos.y() >= viewportTop) {
				// Click is in header area — check for resize zone
				final int resizeCol = hitTestColumnBorder(
						relativePos.x() + this.originScrooled.x());
				if (resizeCol >= 0) {
					this.resizingColumnIndex = resizeCol;
					this.resizeDragStartX = relativePos.x();
					this.resizeDragStartWidth =
							this.columns.get(resizeCol).getWidth();
					keepFocus();
					return true;
				}
			}
		}

		if (this.visibleRows.isEmpty()) {
			return false;
		}

		// Convert to content coordinates (Y inverted, scroll-adjusted)
		final float viewportTop = this.size.y() - this.headerHeight;
		final float contentY = viewportTop - relativePos.y()
				+ this.originScrooled.y();
		final Vector2f contentPos = new Vector2f(
				relativePos.x() + this.originScrooled.x(),
				contentY);

		// Clear hover on leave event
		if (event.status() == KeyStatus.leave) {
			if (this.hoveredGuideDepth >= 0) {
				this.hoveredGuideDepth = -1;
				this.hoveredGuideParentRowIndex = -1;
				markToRedraw();
			}
			return false;
		}

		// Hover tracking for indent guides (inputId == 0 = cursor movement)
		if (event.inputId() == 0) {
			updateIndentGuideHover(contentPos);
		}

		if (event.status() == KeyStatus.pressSingle && event.inputId() == 1) {
			// Click in header area — ignore
			if (contentPos.y() < 0) {
				return false;
			}

			// Find which row was clicked
			final int rowIndex = hitTestRow(contentPos);
			if (rowIndex < 0 || rowIndex >= this.visibleRows.size()) {
				return false;
			}

			final TreeNode<?> clickedNode = this.visibleRows.get(rowIndex);
			final int depth = clickedNode.getDepth()
					- (this.showRoot ? 0 : 1);

			// Check if click is on an indent guide line
			if (handleIndentGuideClick(clickedNode, depth, contentPos,
					relativePos.y())) {
				return true;
			}

			// Check if click is in the chevron zone
			final float chevronStartX = depth * INDENT_WIDTH;
			final float chevronEndX = chevronStartX + CHEVRON_SIZE
					+ CHEVRON_MARGIN * 2;

			if (contentPos.x() >= chevronStartX
					&& contentPos.x() < chevronEndX
					&& !clickedNode.isLeaf()
					&& clickedNode.hasChildren()) {
				// Toggle expand/collapse
				clickedNode.toggleExpanded();
				if (clickedNode.isExpanded()) {
					this.signalExpand.emit(clickedNode);
				} else {
					this.signalCollapse.emit(clickedNode);
				}
				markToRedraw();
				return true;
			}

			// Check which column was clicked
			float colX = 0;
			for (int colIndex = 0; colIndex < this.columns.size();
					colIndex++) {
				final float colWidth =
						this.columns.get(colIndex).getWidth();
				if (contentPos.x() >= colX
						&& contentPos.x() < colX + colWidth) {
					selectNode(clickedNode);
					if (colIndex > 0) {
						this.signalColumnAction.emit(
								new TreeColumnAction<>(clickedNode,
										colIndex));
					}
					return true;
				}
				colX += colWidth;
			}

			// Default: select the node
			selectNode(clickedNode);
			return true;
		}

		// Double-click: expand/collapse non-leaf nodes
		if (event.status() == KeyStatus.pressDouble && event.inputId() == 1) {
			if (contentPos.y() < 0) {
				return false;
			}
			final int rowIndex = hitTestRow(contentPos);
			if (rowIndex < 0 || rowIndex >= this.visibleRows.size()) {
				return false;
			}
			final TreeNode<?> clickedNode = this.visibleRows.get(rowIndex);
			final int clickDepth = clickedNode.getDepth()
					- (this.showRoot ? 0 : 1);

			// Double-click on indent guide: toggle the folder at
			// that guide's depth level
			final TreeNode<?> guideFolder = findGuideFolderAtClick(
					clickedNode, clickDepth, contentPos);
			if (guideFolder != null) {
				guideFolder.toggleExpanded();
				if (guideFolder.isExpanded()) {
					this.signalExpand.emit(guideFolder);
				} else {
					this.signalCollapse.emit(guideFolder);
				}
				markToRedraw();
				return true;
			}

			if (!clickedNode.isLeaf() || clickedNode.hasChildren()) {
				clickedNode.toggleExpanded();
				if (clickedNode.isExpanded()) {
					this.signalExpand.emit(clickedNode);
				} else {
					this.signalCollapse.emit(clickedNode);
				}
				markToRedraw();
				return true;
			}
		}

		return false;
	}

	/**
	 * Update the hovered indent guide depth based on cursor position.
	 * Triggers a redraw if the hovered guide changes.
	 */
	private void updateIndentGuideHover(final Vector2f contentPos) {
		if (contentPos.y() < 0) {
			if (this.hoveredGuideDepth >= 0) {
				this.hoveredGuideDepth = -1;
				this.hoveredGuideParentRowIndex = -1;
				markToRedraw();
			}
			return;
		}

		final int rowIndex = hitTestRow(contentPos);
		if (rowIndex < 0 || rowIndex >= this.visibleRows.size()) {
			if (this.hoveredGuideDepth >= 0) {
				this.hoveredGuideDepth = -1;
				this.hoveredGuideParentRowIndex = -1;
				markToRedraw();
			}
			return;
		}

		final TreeNode<?> node = this.visibleRows.get(rowIndex);
		final int baseDepth = this.showRoot ? 0 : 1;
		final int depth = node.getDepth() - baseDepth;

		// Check which indent guide depth the cursor is over
		int newHoveredDepth = -1;
		int newParentRowIndex = -1;
		for (int d = 0; d < depth; d++) {
			final float guideX = (d + 0.5f) * INDENT_WIDTH;
			if (Math.abs(contentPos.x() - guideX) <= INDENT_WIDTH * 0.4f) {
				if (shouldDrawGuideAtDepth(node, d, baseDepth)) {
					newHoveredDepth = d;
					newParentRowIndex = findGuideParentRowIndex(
							node, d, baseDepth);
					break;
				}
			}
		}

		if (newHoveredDepth != this.hoveredGuideDepth
				|| newParentRowIndex != this.hoveredGuideParentRowIndex) {
			this.hoveredGuideDepth = newHoveredDepth;
			this.hoveredGuideParentRowIndex = newParentRowIndex;
			markToRedraw();
		}
	}

	/**
	 * Handle a click on an indent guide line.
	 * Scrolls to position the next sibling at that depth at the mouse Y.
	 * The next sibling is the node right after the end of the guide bar.
	 * @return true if the click was on an indent guide and was handled.
	 */
	private boolean handleIndentGuideClick(final TreeNode<?> clickedNode,
			final int depth, final Vector2f contentPos,
			final float mouseRelativeY) {
		final int baseDepth = this.showRoot ? 0 : 1;

		for (int d = 0; d < depth; d++) {
			final float guideX = (d + 0.5f) * INDENT_WIDTH;
			if (Math.abs(contentPos.x() - guideX) <= INDENT_WIDTH * 0.4f) {
				if (!shouldDrawGuideAtDepth(clickedNode, d, baseDepth)) {
					continue;
				}
				// Find the next sibling at this depth in the tree
				final TreeNode<?> nextSibling = findNextSiblingAtGuideDepth(
						clickedNode, d, baseDepth);
				if (nextSibling == null) {
					return true;
				}
				// Find the row index of the next sibling in visibleRows.
				// The next sibling should be visible since its parent is
				// expanded. Scan forward from clicked row to find it.
				int siblingRowIndex = -1;
				final int clickedRowIndex = this.visibleRows.indexOf(
						clickedNode);
				if (clickedRowIndex >= 0) {
					for (int i = clickedRowIndex + 1;
							i < this.visibleRows.size(); i++) {
						if (this.visibleRows.get(i) == nextSibling) {
							siblingRowIndex = i;
							break;
						}
					}
				}
				if (siblingRowIndex < 0) {
					return true;
				}
				// Scroll so that the next sibling row's top edge is at the
				// mouse Y position.
				// Rendering formula: startY = viewportTop + scroll
				//                           - rowIndex * rowHeight - rowHeight
				// We want startY = mouseRelativeY:
				// mouseRelativeY = viewportTop + newScroll
				//                - siblingRowIndex * rowHeight - rowHeight
				// => newScroll = mouseRelativeY - viewportTop
				//              + siblingRowIndex * rowHeight + rowHeight
				final float viewportTop = this.size.y() - this.headerHeight;
				final float newScrollY = mouseRelativeY - viewportTop
						+ siblingRowIndex * this.rowHeight + this.rowHeight;
				final float clampedScrollY = FMath.avg(0.0f,
						newScrollY, this.maxSize.y());
				this.originScrooled = new Vector2f(
						this.originScrooled.x(), clampedScrollY);
				markToRedraw();
				return true;
			}
		}
		return false;
	}

	/**
	 * Find the folder node that owns the indent guide at the click position.
	 * Returns the parent folder at the guide's depth, or null if not on a guide.
	 */
	private TreeNode<?> findGuideFolderAtClick(final TreeNode<?> node,
			final int depth, final Vector2f contentPos) {
		final int baseDepth = this.showRoot ? 0 : 1;
		for (int d = 0; d < depth; d++) {
			final float guideX = (d + 0.5f) * INDENT_WIDTH;
			if (Math.abs(contentPos.x() - guideX) <= INDENT_WIDTH * 0.4f) {
				if (!shouldDrawGuideAtDepth(node, d, baseDepth)) {
					continue;
				}
				// The folder that owns this guide is the parent of the
				// ancestor at (baseDepth + d + 1)
				final TreeNode<?> ancestor = findAncestorAtDepth(
						node, d, baseDepth);
				if (ancestor != null && ancestor.getParent() != null) {
					return ancestor.getParent();
				}
				return null;
			}
		}
		return null;
	}

	/**
	 * Check if an X position in content-space is near a column border.
	 * @return the column index whose right border is being targeted,
	 *         or -1 if not near a border.
	 */
	private int hitTestColumnBorder(final float contentX) {
		float borderX = 0;
		for (int colIndex = 0; colIndex < this.columns.size(); colIndex++) {
			borderX += this.columns.get(colIndex).getWidth();
			if (Math.abs(contentX - borderX) <= COLUMN_RESIZE_ZONE) {
				return colIndex;
			}
		}
		return -1;
	}

	private int hitTestRow(final Vector2f contentPos) {
		// contentPos.y is in content-space (Y inverted, scroll adjusted)
		// Header occupies the top of the widget (high Y in content space)
		// Rows start at y = headerHeight in content space
		if (contentPos.y() < 0) {
			return -1;
		}
		return (int) (contentPos.y() / this.rowHeight);
	}

	// ========================================================================
	// Default renderer
	// ========================================================================

	/**
	 * Creates a default cell renderer that displays the node's label as text.
	 * @return A TreeCellRenderer that prints node.getLabel().
	 */
	public static TreeCellRenderer defaultLabelRenderer() {
		return (final CompositingDrawing gc, final CompositingText text,
				final TreeNode<?> node, final Vector2f pos,
				final Vector2f cellSize, final boolean selected) -> {
			text.setPos(pos);
			text.print(node.getLabel());
		};
	}

	// ========================================================================
	// Factory and Fluent API
	// ========================================================================

	public static TreeView create() {
		return new TreeView();
	}

	public TreeView rootNode(final TreeNode<?> root) {
		setRootNode(root);
		return this;
	}

	public TreeView column(final String name, final float width,
			final TreeCellRenderer renderer) {
		addColumn(new TreeColumn(name, width, renderer));
		return this;
	}

	public TreeView showRoot(final boolean show) {
		setShowRoot(show);
		return this;
	}

	public TreeView showHeaders(final boolean show) {
		setShowHeaders(show);
		return this;
	}

	public TreeView alternateRowBackground(final boolean show) {
		setShowAlternateRowBackground(show);
		return this;
	}

	public TreeView onSelect(final Consumer<TreeNode<?>> callback) {
		this.fluentConnections.add(this.signalSelect.connect(callback));
		return this;
	}

	public TreeView onExpand(final Consumer<TreeNode<?>> callback) {
		this.fluentConnections.add(this.signalExpand.connect(callback));
		return this;
	}

	public TreeView onCollapse(final Consumer<TreeNode<?>> callback) {
		this.fluentConnections.add(this.signalCollapse.connect(callback));
		return this;
	}

	@SuppressWarnings("rawtypes")
	public TreeView onColumnAction(final Consumer<TreeColumnAction> callback) {
		this.fluentConnections.add(this.signalColumnAction.connect(callback));
		return this;
	}
}
