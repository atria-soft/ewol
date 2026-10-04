/*
 * @author Edouard DUPIN
 * @copyright 2024, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.dataformat.xml.annotation.JacksonXmlProperty;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.compositing.CompositingGC;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.resource.ResourceColorFile;
import org.atriasoft.gale.key.KeyStatus;
import org.atriasoft.gale.key.KeyType;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * ScrollView widget - A container with optional horizontal and vertical scrollbars.
 *
 * This widget can contain a single child widget and provides scrollable area
 * when the child's size exceeds the viewport. Scrollbars can be independently
 * enabled or disabled for horizontal and vertical directions.
 */
public class ScrollView extends Container {
	private static final Logger LOGGER = LoggerFactory.getLogger(ScrollView.class);

	// Scrollbar dimensions
	protected static final float SCROLLBAR_WIDTH = 12.0f;
	protected static final float SCROLLBAR_MIN_LENGTH = 20.0f;
	protected static final float SCROLLBAR_PADDING = 2.0f;

	// Scrollbar visibility options
	protected boolean propertyShowHorizontal = true;
	protected boolean propertyShowVertical = true;

	// Scroll offset
	protected Vector2f scrollOffset = Vector2f.ZERO;

	// Compositing for drawing scrollbars
	protected CompositingGC compositing = new CompositingGC();

	// Colors
	protected ResourceColorFile colorProperty;
	protected int colorScrollbarTrack = -1;
	protected int colorScrollbarThumb = -1;
	protected int colorScrollbarThumbHover = -1;
	protected int colorScrollbarThumbDrag = -1;

	// Drag state
	protected boolean isDraggingVertical = false;
	protected boolean isDraggingHorizontal = false;
	protected boolean isHoveringVertical = false;
	protected boolean isHoveringHorizontal = false;
	protected float dragStartOffset = 0;
	protected float dragStartMouse = 0;

	// Pixel scrolling amount for mouse wheel
	protected float pixelScrolling = 30.0f;

	public ScrollView() {
		this.colorProperty = own(ResourceColorFile.create(new Uri("THEME", "/color/ScrollView.json", "ewol")));
		if (this.colorProperty != null) {
			this.colorScrollbarTrack = this.colorProperty.request("track");
			this.colorScrollbarThumb = this.colorProperty.request("thumb");
			this.colorScrollbarThumbHover = this.colorProperty.request("thumb-hover");
			this.colorScrollbarThumbDrag = this.colorProperty.request("thumb-drag");
		}
	}

	// ========================================================================
	// Properties
	// ========================================================================

	@JsonProperty("show-horizontal")
	@JacksonXmlProperty(isAttribute = true, localName = "show-horizontal")
	public boolean isPropertyShowHorizontal() {
		return this.propertyShowHorizontal;
	}

	public void setPropertyShowHorizontal(final boolean value) {
		if (this.propertyShowHorizontal == value) {
			return;
		}
		this.propertyShowHorizontal = value;
		markToRedraw();
		requestUpdateSize();
	}

	@JsonProperty("show-vertical")
	@JacksonXmlProperty(isAttribute = true, localName = "show-vertical")
	public boolean isPropertyShowVertical() {
		return this.propertyShowVertical;
	}

	public void setPropertyShowVertical(final boolean value) {
		if (this.propertyShowVertical == value) {
			return;
		}
		this.propertyShowVertical = value;
		markToRedraw();
		requestUpdateSize();
	}

	// ========================================================================
	// Size calculation
	// ========================================================================

	@Override
	public void calculateMinMaxSize() {
		// Call Widget's calculateMinMaxSize, NOT Container's
		// We don't want to inherit the child's min size - that's the whole point of scrolling
		this.minSize = getPropertyMinSize().getPixel();
		this.maxSize = getPropertyMaxSize().getPixel();

		// Still calculate child's min/max so it knows its own size
		if (this.subWidget != null) {
			this.subWidget.calculateMinMaxSize();
		}
	}

	/**
	 * Get the content area size (viewport size minus scrollbars if visible)
	 */
	protected Vector2f getContentAreaSize() {
		float width = this.size.x();
		float height = this.size.y();

		if (needsVerticalScrollbar() && this.propertyShowVertical) {
			width -= SCROLLBAR_WIDTH;
		}
		if (needsHorizontalScrollbar() && this.propertyShowHorizontal) {
			height -= SCROLLBAR_WIDTH;
		}

		return new Vector2f(Math.max(0, width), Math.max(0, height));
	}

	/**
	 * Get the content size (child widget size)
	 */
	protected Vector2f getContentSize() {
		if (this.subWidget == null) {
			return Vector2f.ZERO;
		}
		return this.subWidget.getSize();
	}

	/**
	 * Check if vertical scrollbar is needed
	 */
	protected boolean needsVerticalScrollbar() {
		if (this.subWidget == null) {
			return false;
		}
		final Vector2f contentSize = this.subWidget.getCalculateMinSize();
		return contentSize.y() > this.size.y();
	}

	/**
	 * Check if horizontal scrollbar is needed
	 */
	protected boolean needsHorizontalScrollbar() {
		if (this.subWidget == null) {
			return false;
		}
		final Vector2f contentSize = this.subWidget.getCalculateMinSize();
		float availableWidth = this.size.x();
		if (needsVerticalScrollbar() && this.propertyShowVertical) {
			availableWidth -= SCROLLBAR_WIDTH;
		}
		return contentSize.x() > availableWidth;
	}

	// ========================================================================
	// Size and position
	// ========================================================================

	@Override
	public void onChangeSize() {
		// Don't call Container's onChangeSize - we handle child sizing ourselves
		if (this.propertyHide) {
			return;
		}
		if (this.subWidget == null) {
			return;
		}

		// Calculate content area (viewport)
		final Vector2f contentArea = getContentAreaSize();

		// Get child's minimum size
		Vector2f childSize = this.subWidget.getCalculateMinSize();
		final Vector2b expand = this.subWidget.getPropertyExpand();
		final Vector2b fill = this.subWidget.getPropertyFill();

		// Expand child if needed
		if (expand.x() && childSize.x() < contentArea.x()) {
			childSize = childSize.withX(contentArea.x());
		}
		if (expand.y() && childSize.y() < contentArea.y()) {
			childSize = childSize.withY(contentArea.y());
		}

		// If child has fill property and scrollbar is visible, shrink child to fit content area
		// This prevents scrollbar from overlapping the content
		if (fill.x() && needsVerticalScrollbar() && this.propertyShowVertical && childSize.x() > contentArea.x()) {
			childSize = childSize.withX(contentArea.x());
		}
		if (fill.y() && needsHorizontalScrollbar() && this.propertyShowHorizontal && childSize.y() > contentArea.y()) {
			childSize = childSize.withY(contentArea.y());
		}

		// Clamp scroll offset
		final float maxScrollX = Math.max(0, childSize.x() - contentArea.x());
		final float maxScrollY = Math.max(0, childSize.y() - contentArea.y());
		this.scrollOffset = new Vector2f(
				FMath.avg(0, this.scrollOffset.x(), maxScrollX),
				FMath.avg(0, this.scrollOffset.y(), maxScrollY));

		// Position child at ScrollView's origin (adjusted for horizontal scrollbar)
		float childOriginX = this.origin.x() + this.offset.x();
		float childOriginY = this.origin.y() + this.offset.y();
		if (needsHorizontalScrollbar() && this.propertyShowHorizontal) {
			childOriginY += SCROLLBAR_WIDTH;
		}

		// Standard GUI convention: scrollOffset (0,0) = top-left of content visible
		// In OpenGL coords: Y=0 is bottom, so we need to invert vertical scrolling
		// When scrollOffset.y = 0, show top of content (offset child down by maxScrollY)
		// When scrollOffset.y = maxScrollY, show bottom of content (offset = 0)
		final float invertedScrollY = maxScrollY - this.scrollOffset.y();

		// When content is smaller than viewport, position it at the top (high Y in OpenGL)
		// by adding the difference between viewport and content size to the origin
		final float topAlignOffset = Math.max(0, contentArea.y() - childSize.y());

		this.subWidget.setOrigin(new Vector2f(childOriginX, childOriginY + topAlignOffset));
		this.subWidget.setSize(childSize);
		// Use offset for scrolling: X is normal, Y is inverted for standard GUI behavior
		this.subWidget.setOffset(new Vector2f(-this.scrollOffset.x(), -invertedScrollY));
		this.subWidget.onChangeSize();
		this.subWidget.markToRedraw();
		// Also mark ScrollView for redraw to update scrollbar visibility
		markToRedraw();
	}

	// ========================================================================
	// Scrollbar geometry helpers
	// ========================================================================

	/**
	 * Get vertical scrollbar track rectangle (x, y, width, height)
	 */
	protected float[] getVerticalTrackRect() {
		float x = this.size.x() - SCROLLBAR_WIDTH;
		float y = needsHorizontalScrollbar() && this.propertyShowHorizontal ? SCROLLBAR_WIDTH : 0;
		float height = this.size.y() - y;
		return new float[] { x, y, SCROLLBAR_WIDTH, height };
	}

	/**
	 * Get horizontal scrollbar track rectangle (x, y, width, height)
	 */
	protected float[] getHorizontalTrackRect() {
		float width = this.size.x() - (needsVerticalScrollbar() && this.propertyShowVertical ? SCROLLBAR_WIDTH : 0);
		return new float[] { 0, 0, width, SCROLLBAR_WIDTH };
	}

	/**
	 * Get vertical scrollbar thumb rectangle (x, y, width, height)
	 */
	protected float[] getVerticalThumbRect() {
		final float[] track = getVerticalTrackRect();
		final Vector2f contentSize = getContentSize();
		final Vector2f contentArea = getContentAreaSize();

		if (contentSize.y() <= contentArea.y()) {
			return null;
		}

		final float trackHeight = track[3] - 2 * SCROLLBAR_PADDING;
		final float thumbHeight = Math.max(SCROLLBAR_MIN_LENGTH,
				trackHeight * contentArea.y() / contentSize.y());

		final float maxScroll = contentSize.y() - contentArea.y();
		final float scrollRatio = maxScroll > 0 ? this.scrollOffset.y() / maxScroll : 0;
		// Invert thumb position: scrollRatio=0 (top of content) -> thumb at top of track (high Y)
		// scrollRatio=1 (bottom of content) -> thumb at bottom of track (low Y)
		final float thumbY = track[1] + SCROLLBAR_PADDING + (1 - scrollRatio) * (trackHeight - thumbHeight);

		return new float[] {
				track[0] + SCROLLBAR_PADDING,
				thumbY,
				SCROLLBAR_WIDTH - 2 * SCROLLBAR_PADDING,
				thumbHeight
		};
	}

	/**
	 * Get horizontal scrollbar thumb rectangle (x, y, width, height)
	 */
	protected float[] getHorizontalThumbRect() {
		final float[] track = getHorizontalTrackRect();
		final Vector2f contentSize = getContentSize();
		final Vector2f contentArea = getContentAreaSize();

		if (contentSize.x() <= contentArea.x()) {
			return null;
		}

		final float trackWidth = track[2] - 2 * SCROLLBAR_PADDING;
		final float thumbWidth = Math.max(SCROLLBAR_MIN_LENGTH,
				trackWidth * contentArea.x() / contentSize.x());

		final float maxScroll = contentSize.x() - contentArea.x();
		final float scrollRatio = maxScroll > 0 ? this.scrollOffset.x() / maxScroll : 0;
		final float thumbX = track[0] + SCROLLBAR_PADDING + scrollRatio * (trackWidth - thumbWidth);

		return new float[] {
				thumbX,
				track[1] + SCROLLBAR_PADDING,
				thumbWidth,
				SCROLLBAR_WIDTH - 2 * SCROLLBAR_PADDING
		};
	}

	// ========================================================================
	// Event handling
	// ========================================================================

	@Override
	public boolean onEventInput(final EventInput event) {
		final Vector2f relativePos = relativePosition(new Vector2f(event.pos().x(), event.pos().y()));
		// Use widget-local coordinates in OpenGL system (Y=0 at bottom)
		// This matches the scrollbar rect coordinates from getVerticalTrackRect/getHorizontalTrackRect
		final float localX = relativePos.x();
		final float localY = relativePos.y();

		// Handle mouse events
		if (event.type() == KeyType.mouse) {
			// Handle drag move first (must work even if mouse moves outside scrollbar)
			if (this.isDraggingVertical) {
				if (event.status() == KeyStatus.move) {
					final Vector2f contentSize = getContentSize();
					final Vector2f contentArea = getContentAreaSize();
					final float[] track = getVerticalTrackRect();
					final float[] thumb = getVerticalThumbRect();
					if (thumb != null && contentSize.y() > contentArea.y()) {
						final float trackHeight = track[3] - 2 * SCROLLBAR_PADDING;
						final float thumbHeight = thumb[3];
						final float maxScroll = contentSize.y() - contentArea.y();
						final float availableTrack = trackHeight - thumbHeight;

						if (availableTrack > 0) {
							final float mouseDelta = localY - this.dragStartMouse;
							// Invert: moving thumb up (positive Y delta) = scrolling up = decreasing scrollOffset
							final float scrollDelta = -mouseDelta * maxScroll / availableTrack;
							this.scrollOffset = this.scrollOffset
									.withY(FMath.avg(0, this.dragStartOffset + scrollDelta, maxScroll));
							onChangeSize();
							markToRedraw();
						}
					}
					return true;
				}
				if (event.inputId() == 1 && (event.status() == KeyStatus.up || event.status() == KeyStatus.upAfter)) {
					this.isDraggingVertical = false;
					markToRedraw();
					return true;
				}
			}

			if (this.isDraggingHorizontal) {
				if (event.status() == KeyStatus.move) {
					final Vector2f contentSize = getContentSize();
					final Vector2f contentArea = getContentAreaSize();
					final float[] track = getHorizontalTrackRect();
					final float[] thumb = getHorizontalThumbRect();
					if (thumb != null && contentSize.x() > contentArea.x()) {
						final float trackWidth = track[2] - 2 * SCROLLBAR_PADDING;
						final float thumbWidth = thumb[2];
						final float maxScroll = contentSize.x() - contentArea.x();
						final float availableTrack = trackWidth - thumbWidth;

						if (availableTrack > 0) {
							final float mouseDelta = localX - this.dragStartMouse;
							final float scrollDelta = mouseDelta * maxScroll / availableTrack;
							this.scrollOffset = this.scrollOffset
									.withX(FMath.avg(0, this.dragStartOffset + scrollDelta, maxScroll));
							onChangeSize();
							markToRedraw();
						}
					}
					return true;
				}
				if (event.inputId() == 1 && (event.status() == KeyStatus.up || event.status() == KeyStatus.upAfter)) {
					this.isDraggingHorizontal = false;
					markToRedraw();
					return true;
				}
			}

			// Check for horizontal scrollbar interaction FIRST (it's at the bottom, check before vertical)
			if (this.propertyShowHorizontal && needsHorizontalScrollbar()) {
				final float[] track = getHorizontalTrackRect();
				final float[] thumb = getHorizontalThumbRect();

				if (thumb != null) {
					final boolean inTrack = localX >= track[0] && localX < track[0] + track[2]
							&& localY >= track[1] && localY < track[1] + track[3];
					final boolean inThumb = localX >= thumb[0] && localX < thumb[0] + thumb[2]
							&& localY >= thumb[1] && localY < thumb[1] + thumb[3];

					// Handle hover
					if (event.status() == KeyStatus.move) {
						final boolean wasHovering = this.isHoveringHorizontal;
						this.isHoveringHorizontal = inThumb || inTrack;
						if (wasHovering != this.isHoveringHorizontal) {
							markToRedraw();
						}
					}

					// Handle drag start on thumb
					if (event.inputId() == 1 && event.status() == KeyStatus.down && inThumb) {
						this.isDraggingHorizontal = true;
						this.dragStartOffset = this.scrollOffset.x();
						this.dragStartMouse = localX;
						return true;
					}

					// Handle click on track (jump to position)
					if (event.inputId() == 1 && event.status() == KeyStatus.down && inTrack && !inThumb) {
						final Vector2f contentSize = getContentSize();
						final Vector2f contentArea = getContentAreaSize();
						final float maxScroll = contentSize.x() - contentArea.x();
						final float trackWidth = track[2] - 2 * SCROLLBAR_PADDING;
						final float thumbWidth = thumb[2];
						// Calculate where we clicked relative to the track
						final float clickPosInTrack = localX - track[0] - SCROLLBAR_PADDING - thumbWidth / 2;
						final float availableTrack = trackWidth - thumbWidth;
						if (availableTrack > 0) {
							final float clickRatio = FMath.avg(0, clickPosInTrack / availableTrack, 1);
							this.scrollOffset = this.scrollOffset.withX(clickRatio * maxScroll);
							onChangeSize();
							markToRedraw();
						}
						return true;
					}
				}
			}

			// Check for vertical scrollbar interaction
			if (this.propertyShowVertical && needsVerticalScrollbar()) {
				final float[] track = getVerticalTrackRect();
				final float[] thumb = getVerticalThumbRect();

				if (thumb != null) {
					final boolean inTrack = localX >= track[0] && localX < track[0] + track[2]
							&& localY >= track[1] && localY < track[1] + track[3];
					final boolean inThumb = localX >= thumb[0] && localX < thumb[0] + thumb[2]
							&& localY >= thumb[1] && localY < thumb[1] + thumb[3];

					// Handle hover
					if (event.status() == KeyStatus.move) {
						final boolean wasHovering = this.isHoveringVertical;
						this.isHoveringVertical = inThumb || inTrack;
						if (wasHovering != this.isHoveringVertical) {
							markToRedraw();
						}
					}

					// Handle drag start on thumb
					if (event.inputId() == 1 && event.status() == KeyStatus.down && inThumb) {
						this.isDraggingVertical = true;
						this.dragStartOffset = this.scrollOffset.y();
						this.dragStartMouse = localY;
						return true;
					}

					// Handle click on track (jump to position)
					if (event.inputId() == 1 && event.status() == KeyStatus.down && inTrack && !inThumb) {
						final Vector2f contentSize = getContentSize();
						final Vector2f contentArea = getContentAreaSize();
						final float maxScroll = contentSize.y() - contentArea.y();
						final float trackHeight = track[3] - 2 * SCROLLBAR_PADDING;
						final float thumbHeight = thumb[3];
						// In OpenGL: high Y = top of screen
						// thumb is positioned with (1 - scrollRatio) so thumb at top = scrollOffset 0
						// We want: click at top (high localY) -> scrollOffset 0 (show top of content)
						//          click at bottom (low localY) -> scrollOffset max (show bottom of content)
						final float trackBottom = track[1] + SCROLLBAR_PADDING;
						final float clickPosFromBottom = localY - trackBottom - thumbHeight / 2;
						final float availableTrack = trackHeight - thumbHeight;
						if (availableTrack > 0) {
							// clickPosFromBottom high (top of track) -> low scrollOffset
							// clickPosFromBottom low (bottom of track) -> high scrollOffset
							final float clickRatio = FMath.avg(0, 1 - clickPosFromBottom / availableTrack, 1);
							this.scrollOffset = this.scrollOffset.withY(clickRatio * maxScroll);
							onChangeSize();
							markToRedraw();
						}
						return true;
					}
				}
			}

			// Handle mouse wheel - vertical scrolling (or horizontal with Shift)
			// inputId 4 = wheel up (scroll content up = decrease scrollOffset = show earlier content)
			// inputId 5 = wheel down (scroll content down = increase scrollOffset = show later content)
			if (event.inputId() == 4 && event.status() == KeyStatus.up) {
				// Shift + wheel up = horizontal scroll right
				if (event.specialKey().getShift() && needsHorizontalScrollbar()) {
					final Vector2f contentSize = getContentSize();
					final Vector2f contentArea = getContentAreaSize();
					final float maxScroll = contentSize.x() - contentArea.x();
					this.scrollOffset = this.scrollOffset
							.withX(FMath.avg(0, this.scrollOffset.x() + this.pixelScrolling, maxScroll));
					onChangeSize();
					markToRedraw();
					return true;
				}
				// Normal wheel up = scroll up = show content higher up = decrease scrollOffset
				if (needsVerticalScrollbar()) {
					final Vector2f contentSize = getContentSize();
					final Vector2f contentArea = getContentAreaSize();
					final float maxScroll = contentSize.y() - contentArea.y();
					this.scrollOffset = this.scrollOffset
							.withY(FMath.avg(0, this.scrollOffset.y() + this.pixelScrolling, maxScroll));
					onChangeSize();
					markToRedraw();
					return true;
				}
			}

			if (event.inputId() == 5 && event.status() == KeyStatus.up) {
				// Shift + wheel down = horizontal scroll left
				if (event.specialKey().getShift() && needsHorizontalScrollbar()) {
					final Vector2f contentSize = getContentSize();
					final Vector2f contentArea = getContentAreaSize();
					final float maxScroll = contentSize.x() - contentArea.x();
					this.scrollOffset = this.scrollOffset
							.withX(FMath.avg(0, this.scrollOffset.x() - this.pixelScrolling, maxScroll));
					onChangeSize();
					markToRedraw();
					return true;
				}
				// Normal wheel down = scroll down = show content lower down = increase scrollOffset
				if (needsVerticalScrollbar()) {
					final Vector2f contentSize = getContentSize();
					final Vector2f contentArea = getContentAreaSize();
					final float maxScroll = contentSize.y() - contentArea.y();
					this.scrollOffset = this.scrollOffset
							.withY(FMath.avg(0, this.scrollOffset.y() - this.pixelScrolling, maxScroll));
					onChangeSize();
					markToRedraw();
					return true;
				}
			}

			// Handle mouse wheel - horizontal scrolling (native horizontal wheel)
			if (event.inputId() == 11 && event.status() == KeyStatus.up) {
				if (needsHorizontalScrollbar()) {
					final Vector2f contentSize = getContentSize();
					final Vector2f contentArea = getContentAreaSize();
					final float maxScroll = contentSize.x() - contentArea.x();
					this.scrollOffset = this.scrollOffset
							.withX(FMath.avg(0, this.scrollOffset.x() - this.pixelScrolling, maxScroll));
					onChangeSize();
					markToRedraw();
					return true;
				}
			}

			if (event.inputId() == 10 && event.status() == KeyStatus.up) {
				if (needsHorizontalScrollbar()) {
					final Vector2f contentSize = getContentSize();
					final Vector2f contentArea = getContentAreaSize();
					final float maxScroll = contentSize.x() - contentArea.x();
					this.scrollOffset = this.scrollOffset
							.withX(FMath.avg(0, this.scrollOffset.x() + this.pixelScrolling, maxScroll));
					onChangeSize();
					markToRedraw();
					return true;
				}
			}

			// Handle leave
			if (event.status() == KeyStatus.leave) {
				this.isDraggingVertical = false;
				this.isDraggingHorizontal = false;
				this.isHoveringVertical = false;
				this.isHoveringHorizontal = false;
				markToRedraw();
			}
		}

		return false;
	}

	@Override
	public Widget getWidgetAtPos(final Vector2f pos) {
		if (this.propertyHide) {
			return null;
		}

		// Check if position is over scrollbars
		// Use widget-local coordinates in OpenGL system (Y=0 at bottom)
		// This matches the scrollbar rect coordinates from getVerticalTrackRect/getHorizontalTrackRect
		final Vector2f relativePos = relativePosition(pos);
		final float localX = relativePos.x();
		final float localY = relativePos.y();

		// Check vertical scrollbar
		if (this.propertyShowVertical && needsVerticalScrollbar()) {
			final float[] track = getVerticalTrackRect();
			if (localX >= track[0] && localX < track[0] + track[2]
					&& localY >= track[1] && localY < track[1] + track[3]) {
				return this;
			}
		}

		// Check horizontal scrollbar
		if (this.propertyShowHorizontal && needsHorizontalScrollbar()) {
			final float[] track = getHorizontalTrackRect();
			if (localX >= track[0] && localX < track[0] + track[2]
					&& localY >= track[1] && localY < track[1] + track[3]) {
				return this;
			}
		}

		// Check child widget
		if (this.subWidget != null) {
			return this.subWidget.getWidgetAtPos(pos);
		}

		return this;
	}

	// ========================================================================
	// Drawing
	// ========================================================================

	@Override
	protected void onDraw() {
		this.compositing.draw();
	}

	@Override
	public void onRegenerateDisplay() {
		// Regenerate child first
		if (this.subWidget != null) {
			this.subWidget.systemRegenerateDisplay();
		}

		if (!needRedraw()) {
			return;
		}

		this.compositing.clear();

		// Draw vertical scrollbar
		if (this.propertyShowVertical && needsVerticalScrollbar()) {
			final float[] track = getVerticalTrackRect();
			final float[] thumb = getVerticalThumbRect();

			// Draw track
			this.compositing.setColor(this.colorProperty.get(this.colorScrollbarTrack));
			this.compositing.setPos(new Vector2f(track[0], track[1]));
			this.compositing.rectangle(new Vector2f(track[0] + track[2], track[1] + track[3]));

			// Draw thumb
			if (thumb != null) {
				Color thumbColor;
				if (this.isDraggingVertical) {
					thumbColor = this.colorProperty.get(this.colorScrollbarThumbDrag);
				} else if (this.isHoveringVertical) {
					thumbColor = this.colorProperty.get(this.colorScrollbarThumbHover);
				} else {
					thumbColor = this.colorProperty.get(this.colorScrollbarThumb);
				}
				this.compositing.setColor(thumbColor);
				this.compositing.setPos(new Vector2f(thumb[0], thumb[1]));
				this.compositing.rectangleRadius(new Vector2f(thumb[0] + thumb[2], thumb[1] + thumb[3]),
						(SCROLLBAR_WIDTH - 2 * SCROLLBAR_PADDING) / 2);
			}
		}

		// Draw horizontal scrollbar
		if (this.propertyShowHorizontal && needsHorizontalScrollbar()) {
			final float[] track = getHorizontalTrackRect();
			final float[] thumb = getHorizontalThumbRect();

			// Draw track
			this.compositing.setColor(this.colorProperty.get(this.colorScrollbarTrack));
			this.compositing.setPos(new Vector2f(track[0], track[1]));
			this.compositing.rectangle(new Vector2f(track[0] + track[2], track[1] + track[3]));

			// Draw thumb
			if (thumb != null) {
				Color thumbColor;
				if (this.isDraggingHorizontal) {
					thumbColor = this.colorProperty.get(this.colorScrollbarThumbDrag);
				} else if (this.isHoveringHorizontal) {
					thumbColor = this.colorProperty.get(this.colorScrollbarThumbHover);
				} else {
					thumbColor = this.colorProperty.get(this.colorScrollbarThumb);
				}
				this.compositing.setColor(thumbColor);
				this.compositing.setPos(new Vector2f(thumb[0], thumb[1]));
				this.compositing.rectangleRadius(new Vector2f(thumb[0] + thumb[2], thumb[1] + thumb[3]),
						(SCROLLBAR_WIDTH - 2 * SCROLLBAR_PADDING) / 2);
			}
		}

		// Flush the compositing to GPU
		this.compositing.flush();
	}

	@Override
	public void systemDraw(final DrawProperty displayProp) {
		if (this.propertyHide) {
			return;
		}

		// Draw child with clipping to content area
		if (this.subWidget != null) {
			final Vector2f contentArea = getContentAreaSize();
			// Clip origin starts at ScrollView origin, adjusted for horizontal scrollbar
			float clipOriginY = this.origin.y();
			if (needsHorizontalScrollbar() && this.propertyShowHorizontal) {
				clipOriginY += SCROLLBAR_WIDTH;
			}
			final Vector2f clipOrigin = new Vector2f(this.origin.x(), clipOriginY);
			final Vector2f clipSize = contentArea;

			final DrawProperty prop = displayProp.withLimit(clipOrigin, clipSize);
			this.subWidget.systemDraw(prop);
		}

		// Draw scrollbars (on top of child) - use systemDrawWidget directly
		// to ensure scrollbars are drawn at fixed position without any parent offset
		systemDrawWidget(displayProp);
	}

	// ========================================================================
	// Scroll control methods
	// ========================================================================

	/**
	 * Get current scroll offset
	 */
	public Vector2f getScrollOffset() {
		return this.scrollOffset;
	}

	/**
	 * Set scroll offset
	 */
	public void setScrollOffset(final Vector2f offset) {
		if (this.subWidget == null) {
			return;
		}
		final Vector2f contentSize = getContentSize();
		final Vector2f contentArea = getContentAreaSize();
		final float maxScrollX = Math.max(0, contentSize.x() - contentArea.x());
		final float maxScrollY = Math.max(0, contentSize.y() - contentArea.y());

		this.scrollOffset = new Vector2f(
				FMath.avg(0, offset.x(), maxScrollX),
				FMath.avg(0, offset.y(), maxScrollY));
		onChangeSize();
		markToRedraw();
	}

	/**
	 * Scroll to top
	 */
	public void scrollToTop() {
		setScrollOffset(new Vector2f(this.scrollOffset.x(), 0));
	}

	/**
	 * Scroll to bottom
	 */
	public void scrollToBottom() {
		if (this.subWidget == null) {
			return;
		}
		final Vector2f contentSize = getContentSize();
		final Vector2f contentArea = getContentAreaSize();
		final float maxScrollY = Math.max(0, contentSize.y() - contentArea.y());
		setScrollOffset(new Vector2f(this.scrollOffset.x(), maxScrollY));
	}

	/**
	 * Scroll to left
	 */
	public void scrollToLeft() {
		setScrollOffset(new Vector2f(0, this.scrollOffset.y()));
	}

	/**
	 * Scroll to right
	 */
	public void scrollToRight() {
		if (this.subWidget == null) {
			return;
		}
		final Vector2f contentSize = getContentSize();
		final Vector2f contentArea = getContentAreaSize();
		final float maxScrollX = Math.max(0, contentSize.x() - contentArea.x());
		setScrollOffset(new Vector2f(maxScrollX, this.scrollOffset.y()));
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	/**
	 * Create a new ScrollView.
	 */
	public static ScrollView create() {
		return new ScrollView();
	}

	/**
	 * Fluent method to set horizontal scrollbar visibility.
	 */
	public ScrollView showHorizontal(final boolean show) {
		setPropertyShowHorizontal(show);
		return this;
	}

	/**
	 * Fluent method to set vertical scrollbar visibility.
	 */
	public ScrollView showVertical(final boolean show) {
		setPropertyShowVertical(show);
		return this;
	}

	/**
	 * Fluent method to set the content widget.
	 */
	public ScrollView content(final Widget widget) {
		setSubWidget(widget);
		return this;
	}
}
