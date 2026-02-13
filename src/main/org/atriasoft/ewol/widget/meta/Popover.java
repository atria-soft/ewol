package org.atriasoft.ewol.widget.meta;

import java.util.ArrayList;
import java.util.List;

import org.atriasoft.esignal.Connection;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.DimensionBorderRadius;
import org.atriasoft.etk.DimensionInsets;
import org.atriasoft.etk.math.Matrix4f;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.Ewol;
import org.atriasoft.ewol.compositing.CompositingGC;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.event.InputSystem;
import org.atriasoft.ewol.widget.Box;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.ewol.widget.Windows;
import org.atriasoft.gale.backend3d.OpenGL;
import org.atriasoft.gale.key.KeyStatus;
import org.lwjgl.opengl.GL11;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Popover widget that displays contextual content near an anchor point.
 *
 * <p>The popover automatically positions itself (top, bottom, left, right)
 * based on available space, with an arrow pointing toward the anchor.
 * It supports arbitrary child widgets and closes on outside click.</p>
 *
 * <p>Usage pattern follows {@link SelectPopup}: extends Widget, pushed onto
 * the popup stack, manages its own internal Box for visible content.</p>
 */
public class Popover extends Widget {
	private static final Logger LOGGER = LoggerFactory.getLogger(Popover.class);

	private static final Color DEFAULT_BG_COLOR = Color.WHITE;
	private static final Color DEFAULT_BORDER_COLOR = new Color(0x80, 0x80, 0x80, 0xFF);

	// Configuration
	private PopoverPosition preferredPosition = PopoverPosition.AUTO;
	private Vector2f anchorPoint = Vector2f.ZERO;
	private Vector2f anchorSize = Vector2f.ZERO;
	private boolean closeOnOutsideClick = true;
	private boolean closeOnHoverLeave = false;
	private boolean managedByParent = false;
	private float arrowSize = 10.0f;

	// Internal UI
	private final Box popupBox;
	private final CompositingGC arrowDraw = new CompositingGC();

	// Computed state
	private PopoverPosition resolvedPosition = PopoverPosition.BOTTOM;
	private Vector2f popupOrigin = Vector2f.ZERO;
	private Vector2f popupSize = Vector2f.ZERO;
	private Vector2f arrowTip = Vector2f.ZERO;
	private Vector2f arrowBase1 = Vector2f.ZERO;
	private Vector2f arrowBase2 = Vector2f.ZERO;

	// Bounding box covering popup + arrow for hit testing
	private Vector2f hitBoxOrigin = Vector2f.ZERO;
	private Vector2f hitBoxSize = Vector2f.ZERO;

	// Extended zone covering popup + arrow + anchor + margin for hover leave detection
	private static final float HOVER_MARGIN = 5.0f;
	private Vector2f hoverZoneOrigin = Vector2f.ZERO;
	private Vector2f hoverZoneSize = Vector2f.ZERO;

	// Signals
	public final SignalEmpty signalDismissed = new SignalEmpty();

	// Keep references to signal connections to prevent GC
	private final List<Connection> signalConnections = new ArrayList<>();

	public Popover() {
		this.propertyCanFocus = true;
		setMouseLimit(1);
		setPropertyExpand(Vector2b.TRUE);
		setPropertyFill(Vector2b.TRUE);

		this.popupBox = new Box();
		this.popupBox.setPropertyExpand(Vector2b.FALSE);
		this.popupBox.setPropertyFill(Vector2b.FALSE);
		this.popupBox.setPropertyBorderWidth(new DimensionInsets(2));
		this.popupBox.setPropertyBorderColor(DEFAULT_BORDER_COLOR);
		this.popupBox.setPropertyColor(DEFAULT_BG_COLOR);
		this.popupBox.setPropertyPadding(new DimensionInsets(8));
		this.popupBox.setPropertyMargin(new DimensionInsets(0));
		this.popupBox.setPropertyBorderRadius(new DimensionBorderRadius(8));
	}

	// ========================================================================
	// Content
	// ========================================================================

	/**
	 * Set the content widget displayed inside the popover.
	 */
	public void setContent(final Widget widget) {
		this.popupBox.setSubWidget(widget);
	}

	// ========================================================================
	// Size calculation
	// ========================================================================

	@Override
	public void calculateMinMaxSize() {
		this.minSize = Vector2f.ZERO;
		this.maxSize = new Vector2f(Float.MAX_VALUE, Float.MAX_VALUE);
		this.popupBox.calculateMinMaxSize();
	}

	// ========================================================================
	// Layout / positioning
	// ========================================================================

	@Override
	public void onChangeSize() {
		markToRedraw();

		this.popupBox.calculateMinMaxSize();
		final Vector2f popupMinSize = this.popupBox.getCalculateMinSize();

		final float popupWidth = popupMinSize.x();
		float popupHeight = popupMinSize.y();

		// Resolve position
		this.resolvedPosition = resolvePosition(popupWidth, popupHeight);

		// Calculate anchor center
		final float anchorCenterX = this.anchorPoint.x() + this.anchorSize.x() * 0.5f;
		final float anchorCenterY = this.anchorPoint.y() + this.anchorSize.y() * 0.5f;

		// Calculate popup origin based on resolved position
		// Coordinate system: Y=0 at bottom, Y increases upward
		float originX;
		float originY;

		switch (this.resolvedPosition) {
			case BOTTOM:
				// Popup below anchor, arrow points up
				originX = anchorCenterX - popupWidth * 0.5f;
				originY = this.anchorPoint.y() - this.arrowSize - popupHeight;
				break;
			case TOP:
				// Popup above anchor, arrow points down
				originX = anchorCenterX - popupWidth * 0.5f;
				originY = this.anchorPoint.y() + this.anchorSize.y() + this.arrowSize;
				break;
			case LEFT:
				// Popup left of anchor, arrow points right
				originX = this.anchorPoint.x() - this.arrowSize - popupWidth;
				originY = anchorCenterY - popupHeight * 0.5f;
				break;
			case RIGHT:
				// Popup right of anchor, arrow points left
				originX = this.anchorPoint.x() + this.anchorSize.x() + this.arrowSize;
				originY = anchorCenterY - popupHeight * 0.5f;
				break;
			default:
				originX = anchorCenterX - popupWidth * 0.5f;
				originY = this.anchorPoint.y() - this.arrowSize - popupHeight;
				break;
		}

		// Clamp to window bounds
		originX = Math.max(0, Math.min(originX, this.size.x() - popupWidth));
		originY = Math.max(0, Math.min(originY, this.size.y() - popupHeight));

		this.popupOrigin = new Vector2f(originX, originY);
		this.popupSize = new Vector2f(popupWidth, popupHeight);

		// Calculate arrow vertices
		calculateArrowVertices(anchorCenterX, anchorCenterY);

		// Calculate hit box (bounding box of popup + arrow)
		calculateHitBox();

		// Calculate extended hover zone (hitBox UNION anchor + margin)
		calculateHoverZone();

		// Position and size the popup box
		this.popupBox.setOrigin(this.popupOrigin);
		this.popupBox.setSize(this.popupSize);
		this.popupBox.onChangeSize();
	}

	/**
	 * Fast relocation: move the popover to a new anchor point without
	 * recalculating sizes. Only the position, arrow, and hit boxes are updated.
	 * Used by follow-mouse mode for instant tracking.
	 */
	void relocateToAnchor(final Vector2f newAnchorPoint) {
		this.anchorPoint = newAnchorPoint;
		this.anchorSize = Vector2f.ZERO;

		final float anchorCenterX = newAnchorPoint.x();
		final float anchorCenterY = newAnchorPoint.y();

		// Reuse existing popup size — only recompute origin
		final float popupWidth = this.popupSize.x();
		final float popupHeight = this.popupSize.y();

		float originX;
		float originY;

		switch (this.resolvedPosition) {
			case BOTTOM:
				originX = anchorCenterX - popupWidth * 0.5f;
				originY = newAnchorPoint.y() - this.arrowSize - popupHeight;
				break;
			case TOP:
				originX = anchorCenterX - popupWidth * 0.5f;
				originY = newAnchorPoint.y() + this.arrowSize;
				break;
			case LEFT:
				originX = newAnchorPoint.x() - this.arrowSize - popupWidth;
				originY = anchorCenterY - popupHeight * 0.5f;
				break;
			case RIGHT:
				originX = newAnchorPoint.x() + this.arrowSize;
				originY = anchorCenterY - popupHeight * 0.5f;
				break;
			default:
				originX = anchorCenterX - popupWidth * 0.5f;
				originY = newAnchorPoint.y() - this.arrowSize - popupHeight;
				break;
		}

		// Clamp to window bounds
		originX = Math.max(0, Math.min(originX, this.size.x() - popupWidth));
		originY = Math.max(0, Math.min(originY, this.size.y() - popupHeight));

		this.popupOrigin = new Vector2f(originX, originY);

		// Recalculate arrow, hit box, hover zone
		calculateArrowVertices(anchorCenterX, anchorCenterY);
		calculateHitBox();
		calculateHoverZone();

		// Move the popup box and propagate to children (size unchanged)
		this.popupBox.setOrigin(this.popupOrigin);
		this.popupBox.onChangeSize();

		markToRedraw();
	}

	/**
	 * Resolve the best position for the popover based on available space.
	 */
	private PopoverPosition resolvePosition(final float popupWidth, final float popupHeight) {
		if (this.preferredPosition != PopoverPosition.AUTO) {
			return this.preferredPosition;
		}

		final float neededVertical = popupHeight + this.arrowSize;
		final float neededHorizontal = popupWidth + this.arrowSize;

		// Available space in each direction
		final float spaceBelow = this.anchorPoint.y();
		final float spaceAbove = this.size.y() - (this.anchorPoint.y() + this.anchorSize.y());
		final float spaceLeft = this.anchorPoint.x();
		final float spaceRight = this.size.x() - (this.anchorPoint.x() + this.anchorSize.x());

		// Priority: BOTTOM > TOP > RIGHT > LEFT
		if (spaceBelow >= neededVertical) {
			return PopoverPosition.BOTTOM;
		}
		if (spaceAbove >= neededVertical) {
			return PopoverPosition.TOP;
		}
		if (spaceRight >= neededHorizontal) {
			return PopoverPosition.RIGHT;
		}
		if (spaceLeft >= neededHorizontal) {
			return PopoverPosition.LEFT;
		}

		// Nothing fits perfectly - pick the direction with most space
		float maxSpace = spaceBelow;
		PopoverPosition best = PopoverPosition.BOTTOM;
		if (spaceAbove > maxSpace) {
			maxSpace = spaceAbove;
			best = PopoverPosition.TOP;
		}
		if (spaceRight > maxSpace) {
			maxSpace = spaceRight;
			best = PopoverPosition.RIGHT;
		}
		if (spaceLeft > maxSpace) {
			best = PopoverPosition.LEFT;
		}
		return best;
	}

	/**
	 * Calculate the three vertices of the arrow triangle.
	 */
	private void calculateArrowVertices(final float anchorCenterX, final float anchorCenterY) {
		final float halfArrow = this.arrowSize * 0.5f;

		// Clamp arrow tip position to the popup edge range
		switch (this.resolvedPosition) {
			case BOTTOM:
				// Arrow points UP from top edge of popup toward anchor
				float tipX = Math.max(this.popupOrigin.x() + halfArrow,
						Math.min(anchorCenterX, this.popupOrigin.x() + this.popupSize.x() - halfArrow));
				float tipY = this.popupOrigin.y() + this.popupSize.y() + this.arrowSize;
				this.arrowTip = new Vector2f(tipX, tipY);
				this.arrowBase1 = new Vector2f(tipX - halfArrow, this.popupOrigin.y() + this.popupSize.y());
				this.arrowBase2 = new Vector2f(tipX + halfArrow, this.popupOrigin.y() + this.popupSize.y());
				break;
			case TOP:
				// Arrow points DOWN from bottom edge of popup toward anchor
				tipX = Math.max(this.popupOrigin.x() + halfArrow,
						Math.min(anchorCenterX, this.popupOrigin.x() + this.popupSize.x() - halfArrow));
				tipY = this.popupOrigin.y() - this.arrowSize;
				this.arrowTip = new Vector2f(tipX, tipY);
				this.arrowBase1 = new Vector2f(tipX - halfArrow, this.popupOrigin.y());
				this.arrowBase2 = new Vector2f(tipX + halfArrow, this.popupOrigin.y());
				break;
			case LEFT:
				// Arrow points RIGHT from right edge of popup toward anchor
				tipX = this.popupOrigin.x() + this.popupSize.x() + this.arrowSize;
				tipY = Math.max(this.popupOrigin.y() + halfArrow,
						Math.min(anchorCenterY, this.popupOrigin.y() + this.popupSize.y() - halfArrow));
				this.arrowTip = new Vector2f(tipX, tipY);
				this.arrowBase1 = new Vector2f(this.popupOrigin.x() + this.popupSize.x(), tipY - halfArrow);
				this.arrowBase2 = new Vector2f(this.popupOrigin.x() + this.popupSize.x(), tipY + halfArrow);
				break;
			case RIGHT:
				// Arrow points LEFT from left edge of popup toward anchor
				tipX = this.popupOrigin.x() - this.arrowSize;
				tipY = Math.max(this.popupOrigin.y() + halfArrow,
						Math.min(anchorCenterY, this.popupOrigin.y() + this.popupSize.y() - halfArrow));
				this.arrowTip = new Vector2f(tipX, tipY);
				this.arrowBase1 = new Vector2f(this.popupOrigin.x(), tipY - halfArrow);
				this.arrowBase2 = new Vector2f(this.popupOrigin.x(), tipY + halfArrow);
				break;
			default:
				break;
		}
	}

	/**
	 * Calculate bounding box covering both popup and arrow for hit testing.
	 */
	private void calculateHitBox() {
		final float minX = Math.min(this.popupOrigin.x(),
				Math.min(this.arrowTip.x(), Math.min(this.arrowBase1.x(), this.arrowBase2.x())));
		final float minY = Math.min(this.popupOrigin.y(),
				Math.min(this.arrowTip.y(), Math.min(this.arrowBase1.y(), this.arrowBase2.y())));
		final float maxX = Math.max(this.popupOrigin.x() + this.popupSize.x(),
				Math.max(this.arrowTip.x(), Math.max(this.arrowBase1.x(), this.arrowBase2.x())));
		final float maxY = Math.max(this.popupOrigin.y() + this.popupSize.y(),
				Math.max(this.arrowTip.y(), Math.max(this.arrowBase1.y(), this.arrowBase2.y())));
		this.hitBoxOrigin = new Vector2f(minX, minY);
		this.hitBoxSize = new Vector2f(maxX - minX, maxY - minY);
	}

	/**
	 * Calculate extended hover zone = bounding box of (hitBox UNION anchor) + margin.
	 * This covers the gap between popup and anchor (arrow zone) so the mouse
	 * can travel between them without triggering a close.
	 */
	private void calculateHoverZone() {
		final float minX = Math.max(0, Math.min(this.hitBoxOrigin.x(), this.anchorPoint.x()) - HOVER_MARGIN);
		final float minY = Math.max(0, Math.min(this.hitBoxOrigin.y(), this.anchorPoint.y()) - HOVER_MARGIN);
		final float maxX = Math.min(this.size.x(),
				Math.max(this.hitBoxOrigin.x() + this.hitBoxSize.x(),
						this.anchorPoint.x() + this.anchorSize.x()) + HOVER_MARGIN);
		final float maxY = Math.min(this.size.y(),
				Math.max(this.hitBoxOrigin.y() + this.hitBoxSize.y(),
						this.anchorPoint.y() + this.anchorSize.y()) + HOVER_MARGIN);
		this.hoverZoneOrigin = new Vector2f(minX, minY);
		this.hoverZoneSize = new Vector2f(maxX - minX, maxY - minY);
	}

	// ========================================================================
	// Rendering
	// ========================================================================

	@Override
	public void onRegenerateDisplay() {
		if (!needRedraw()) {
			return;
		}
		// Regenerate popup box
		this.popupBox.markToRedraw();
		this.popupBox.systemRegenerateDisplay();

		// Regenerate arrow triangle
		this.arrowDraw.clear();
		this.arrowDraw.setColor(this.popupBox.getPropertyColor());
		this.arrowDraw.setPos(this.arrowTip);
		this.arrowDraw.addVertex();
		this.arrowDraw.setPos(this.arrowBase1);
		this.arrowDraw.addVertex();
		this.arrowDraw.setPos(this.arrowBase2);
		this.arrowDraw.addVertex();
		this.arrowDraw.flush();
	}

	@Override
	public void systemDraw(final DrawProperty displayProp) {
		if (this.propertyHide) {
			return;
		}
		// Draw the popup box clipped to its region (same pattern as SelectPopup)
		final DrawProperty clippedProp = displayProp.withLimit(this.popupOrigin, this.popupSize);
		this.popupBox.systemDraw(clippedProp);

		// Draw arrow in a viewport limited to the hitBox area (popup + arrow zone).
		// We replicate the viewport/matrix setup from Widget.systemDrawWidget()
		// scoped to the hitBox to avoid setting a full-window viewport.
		final DrawProperty arrowClip = displayProp.withLimit(this.hitBoxOrigin, this.hitBoxSize);
		if (arrowClip.size().x() > 0 && arrowClip.size().y() > 0) {
			OpenGL.setViewPort(arrowClip.origin(), arrowClip.size());
			final Vector2i downOffset = Vector2i.min(
					new Vector2i((int) (this.hitBoxOrigin.x() - arrowClip.origin().x()),
							(int) (this.hitBoxOrigin.y() - arrowClip.origin().y())),
					Vector2i.ZERO);
			final Matrix4f tmpTranslate = Matrix4f.createMatrixTranslate(
					new Vector3f(-arrowClip.size().x() / 2 + downOffset.x(),
							-arrowClip.size().y() / 2 + downOffset.y(), -1.0f).clipInteger());
			final Matrix4f tmpProjection = Matrix4f.createMatrixOrtho(
					-arrowClip.size().x() / 2, arrowClip.size().x() / 2,
					-arrowClip.size().y() / 2, arrowClip.size().y() / 2, -500, 500);
			OpenGL.push();
			OpenGL.setMatrix(tmpProjection);
			OpenGL.setCameraMatrix(tmpTranslate);
			this.arrowDraw.draw(true);
			OpenGL.pop();
			GL11.glFinish();
		}
	}

	// ========================================================================
	// Event handling
	// ========================================================================

	@Override
	public Widget getWidgetAtPos(final Vector2f pos) {
		if (this.propertyHide) {
			return null;
		}
		// In managed mode, the popover is purely visual — it never intercepts
		// events. The parent (PopoverTrigger) handles the full lifecycle.
		if (this.managedByParent) {
			return null;
		}
		return this;
	}

	@Override
	public boolean systemEventInput(final InputSystem event) {
		final Vector2f pos = event.event().pos();
		final boolean insidePopup = isInsidePopup(pos);

		if (onEventInput(event.event())) {
			return true;
		}
		// If inside popup, forward event to the popup box children
		if (insidePopup) {
			final Widget target = this.popupBox.getWidgetAtPos(pos);
			if (target != null && target != this && target != this.popupBox) {
				return target.systemEventInput(event);
			}
		}
		return false;
	}

	@Override
	protected boolean onEventInput(final EventInput event) {
		final boolean insideHitBox = isInsideHitBox(event.pos());
		final boolean insideAnchor = isInsideAnchor(event.pos());
		final boolean insideAny = insideHitBox || insideAnchor;

		// Close on outside click
		if (!insideAny && this.closeOnOutsideClick
				&& event.inputId() == 1
				&& event.status() == KeyStatus.pressSingle) {
			LOGGER.debug("Click outside popover - closing");
			closePopup();
			return true;
		}
		// Close when hover leaves the extended zone (popup + arrow + anchor + margin)
		if (this.closeOnHoverLeave && event.inputId() == 0) {
			if (!isInsideHoverZone(event.pos())) {
				LOGGER.debug("Hover left extended zone - closing");
				closePopup();
				return true;
			}
		}
		return false;
	}

	/**
	 * Check if position is inside the popup box.
	 */
	private boolean isInsidePopup(final Vector2f pos) {
		return pos.x() >= this.popupOrigin.x()
				&& pos.x() <= this.popupOrigin.x() + this.popupSize.x()
				&& pos.y() >= this.popupOrigin.y()
				&& pos.y() <= this.popupOrigin.y() + this.popupSize.y();
	}

	/**
	 * Check if position is inside the anchor widget area.
	 */
	private boolean isInsideAnchor(final Vector2f pos) {
		return pos.x() >= this.anchorPoint.x()
				&& pos.x() <= this.anchorPoint.x() + this.anchorSize.x()
				&& pos.y() >= this.anchorPoint.y()
				&& pos.y() <= this.anchorPoint.y() + this.anchorSize.y();
	}

	/**
	 * Check if position is inside the bounding box covering popup + arrow.
	 */
	boolean isInsideHitBox(final Vector2f pos) {
		return pos.x() >= this.hitBoxOrigin.x()
				&& pos.x() <= this.hitBoxOrigin.x() + this.hitBoxSize.x()
				&& pos.y() >= this.hitBoxOrigin.y()
				&& pos.y() <= this.hitBoxOrigin.y() + this.hitBoxSize.y();
	}

	/**
	 * Check if position is inside the extended hover zone
	 * (popup + arrow + anchor + margin).
	 */
	private boolean isInsideHoverZone(final Vector2f pos) {
		return pos.x() >= this.hoverZoneOrigin.x()
				&& pos.x() <= this.hoverZoneOrigin.x() + this.hoverZoneSize.x()
				&& pos.y() >= this.hoverZoneOrigin.y()
				&& pos.y() <= this.hoverZoneOrigin.y() + this.hoverZoneSize.y();
	}

	// ========================================================================
	// Lifecycle
	// ========================================================================

	/**
	 * Close the popover and remove from the popover layer.
	 */
	public void closePopup() {
		this.signalDismissed.emit();
		final Windows windows = Ewol.getContext().getWindows();
		if (windows != null) {
			windows.getPopoverManager().remove(this);
		}
	}

	/**
	 * Show the popover via the dedicated popover layer (not the popup stack).
	 * @return this popover for chaining
	 */
	public Popover show() {
		final Windows windows = Ewol.getContext().getWindows();
		if (windows != null) {
			windows.getPopoverManager().show(this);
		}
		return this;
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	public static Popover create() {
		return new Popover();
	}

	/**
	 * Show a popover at a mouse position.
	 */
	public static Popover showAt(final Vector2f mousePos, final Widget content) {
		return Popover.create()
				.content(content)
				.anchor(mousePos)
				.show();
	}

	/**
	 * Show a popover anchored to a widget.
	 */
	public static Popover showAt(final Vector2f origin, final Vector2f size,
			final Widget content, final PopoverPosition position) {
		return Popover.create()
				.content(content)
				.anchor(origin, size)
				.position(position)
				.show();
	}

	public Popover content(final Widget widget) {
		setContent(widget);
		return this;
	}

	public Popover anchor(final Vector2f point) {
		this.anchorPoint = point;
		this.anchorSize = Vector2f.ZERO;
		return this;
	}

	public Popover anchor(final Vector2f origin, final Vector2f size) {
		this.anchorPoint = origin;
		this.anchorSize = size;
		return this;
	}

	public Popover position(final PopoverPosition position) {
		this.preferredPosition = position;
		return this;
	}

	public Popover closeOnOutside(final boolean close) {
		this.closeOnOutsideClick = close;
		return this;
	}

	public Popover closeOnHoverLeave(final boolean close) {
		this.closeOnHoverLeave = close;
		return this;
	}

	/**
	 * Set managed mode: the popover lifecycle is controlled by an external parent
	 * (e.g. PopoverTrigger). In managed mode, the popover only intercepts events
	 * inside the popup content area, letting events outside pass through to the
	 * widget tree below.
	 */
	public Popover managed(final boolean managed) {
		this.managedByParent = managed;
		return this;
	}

	public Popover arrowSize(final float size) {
		this.arrowSize = size;
		return this;
	}

	public Popover color(final Color color) {
		this.popupBox.setPropertyColor(color);
		return this;
	}

	public Popover borderColor(final Color color) {
		this.popupBox.setPropertyBorderColor(color);
		return this;
	}

	public Popover borderWidth(final DimensionInsets width) {
		this.popupBox.setPropertyBorderWidth(width);
		return this;
	}

	public Popover borderRadius(final DimensionBorderRadius radius) {
		this.popupBox.setPropertyBorderRadius(radius);
		return this;
	}

	public Popover padding(final DimensionInsets padding) {
		this.popupBox.setPropertyPadding(padding);
		return this;
	}
}
