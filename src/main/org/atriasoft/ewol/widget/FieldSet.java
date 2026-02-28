package org.atriasoft.ewol.widget;

import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.BorderRadius;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.DimensionBorderRadius;
import org.atriasoft.etk.DimensionInsets;
import org.atriasoft.etk.Insets;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.Gravity;

/**
 * FieldSet widget, equivalent to HTML {@code <fieldset>} with optional icon and checkbox.
 *
 * <p>Extends {@link Box} to provide a visual frame (border, padding, background color)
 * around a group of widgets, with a legend header.</p>
 *
 * <p>The top border line passes through the vertical center of the header label,
 * with a gap where the header text sits, like a classic HTML fieldset.</p>
 *
 * <p>Structure:</p>
 * <pre>
 * FieldSet (extends Box — provides border, padding, background)
 *   └── mainSizer (Sizer VERTICAL)
 *         ├── headerSizer (Sizer HORIZONTAL)
 *         │     ├── [tick] (Tick, optional)
 *         │     ├── [icon] (Icon, optional)
 *         │     └── titleLabel (Label)
 *         └── contentSizer (Sizer VERTICAL)
 *               └── [user widgets added via add()]
 * </pre>
 *
 * <p>When a Tick is present and unchecked, the content is hidden.</p>
 *
 * <p>Example usage:</p>
 * <pre>{@code
 * FieldSet.create("Lighting")
 *     .icon("light")
 *     .checkable(true)
 *     .checked(false)
 *     .add(intensitySlider)
 *     .add(colorPicker);
 * }</pre>
 */
public class FieldSet extends Box {

	// ========================================================================
	// Default colors
	// ========================================================================

	private static final Color DEFAULT_BORDER_COLOR = new Color(0xC0, 0xC0, 0xC0, 0xFF);
	private static final Color DEFAULT_BACKGROUND_COLOR = Color.NONE;
	private static final float GAP_PADDING = 4.0f;

	// ========================================================================
	// Signals
	// ========================================================================

	public final SignalEmpty signalActivate = new SignalEmpty();
	public final SignalEmpty signalDeactivate = new SignalEmpty();

	// ========================================================================
	// Internal widgets
	// ========================================================================

	private final Label titleLabel;
	private final Sizer headerSizer;
	private final Sizer contentSizer;
	private Tick tick;
	private Icon icon;

	// ========================================================================
	// Callbacks (ewol static convention)
	// ========================================================================

	private static void onTickValueChanged(final FieldSet self, final Boolean value) {
		self.contentSizer.setPropertyHide(!value);
		if (value) {
			self.signalActivate.emit();
		} else {
			self.signalDeactivate.emit();
		}
	}

	// ========================================================================
	// Constructor
	// ========================================================================

	public FieldSet(final String title) {
		// Box visual defaults: light border frame
		setPropertyBorderColor(DEFAULT_BORDER_COLOR);
		setPropertyBorderWidth(new DimensionInsets(1));
		setPropertyBorderRadius(new DimensionBorderRadius(4));
		setPropertyColor(DEFAULT_BACKGROUND_COLOR);
		setPropertyPadding(new DimensionInsets(6));
		setPropertyMargin(new DimensionInsets(2));

		this.titleLabel = Label.create(title).expand(true, false).gravity(Gravity.LEFT);

		this.headerSizer = Sizer.horizontal().expand(true, false).fill(true, false);
		this.headerSizer.setPropertyLockExpand(Vector2b.TRUE);
		this.headerSizer.subWidgetAdd(this.titleLabel);

		this.contentSizer = Sizer.vertical().expand(true, true).fill(true, true);

		final Sizer mainSizer = Sizer.vertical().expand(true, true).fill(true, true);
		mainSizer.setPropertyLockExpand(Vector2b.TRUE);
		mainSizer.subWidgetAdd(this.headerSizer);
		mainSizer.subWidgetAdd(this.contentSizer);

		setSubWidget(mainSizer);
	}

	// ========================================================================
	// Custom rendering: border with gap at header center
	// ========================================================================

	@Override
	public void regenerateDisplay() {
		// Recompute positions exactly as Box does
		final Insets localMargin = this.propertyMargin.size();
		Vector2f renderSize = calculateSizeRendering();
		this.overPositionStart = calculateOriginRendering(renderSize).add(localMargin.getOrigin());
		renderSize = renderSize.less(localMargin.toVector2f());
		this.overPositionStart = this.overPositionStart.clipInteger();
		this.overPositionStop = this.overPositionStart.add(renderSize.clipInteger());
		final Insets offsetSubWidget = getBorderInsideAggregation();
		this.insidePositionStart = this.overPositionStart.add(offsetSubWidget.getOrigin());
		this.insidePositionStop = this.overPositionStop.less(offsetSubWidget.getEnd());

		this.vectorialDraw.clear();

		// Draw fill (if any)
		if (this.propertyColor.a() != 0) {
			this.vectorialDraw.setPaintFillColor(this.propertyColor);
			// Fill uses the shifted top Y (at header center)
			final float borderTopY = computeBorderTopY();
			final Vector2f fillStop = this.overPositionStop.withY(borderTopY);
			this.vectorialDraw.setPos(this.overPositionStart);
			this.vectorialDraw.rectangleRadius(fillStop, this.propertyBorderWidth.getPixel(),
					this.propertyBorderRadius.getPixel());
		}

		// Draw border with gap at header
		final Insets thickness = this.propertyBorderWidth.getPixel();
		if (!thickness.isZero()) {
			drawBorderWithGap(thickness);
		}

		this.vectorialDraw.flush();
	}

	/**
	 * Compute the Y coordinate for the top border line (vertical center of header).
	 * Returns a coordinate in local (drawing) space, relative to this widget's origin.
	 */
	private float computeBorderTopY() {
		final Vector2f headerOrigin = this.headerSizer.getOrigin().less(this.origin);
		final Vector2f headerSize = this.headerSizer.getSize();
		return headerOrigin.y() + headerSize.y() * 0.5f;
	}

	/**
	 * Draw the border with the top edge shifted to the header center Y,
	 * and a gap in the top border where the header sits.
	 *
	 * This replicates CompositingDrawing.rectangleBorderRadius() but:
	 * - Top Y is at header center, not at overPositionStop.y()
	 * - Top border is split into two segments with a gap for the header
	 */
	private void drawBorderWithGap(final Insets thickness) {
		final BorderRadius radius = this.propertyBorderRadius.getPixel();
		final float borderTopY = computeBorderTopY();

		// The border rectangle: bottom at overPositionStart, top at borderTopY
		final Vector3f posStart = this.overPositionStart.toVector3f();
		final float destX = this.overPositionStop.x();
		final float destY = borderTopY;

		this.vectorialDraw.setPos(this.overPositionStart);
		this.vectorialDraw.setDrawingColor(this.propertyBorderColor);

		// Corner centers
		final Vector3f centerBottomLeft = posStart.add(radius.bottomLeft(), radius.bottomLeft(), 0);
		final Vector3f centerBottomRight = new Vector3f(destX - radius.bottomRight(),
				posStart.y() + radius.bottomRight(), 0);
		final Vector3f centerTopRight = new Vector3f(destX - radius.topRight(),
				destY - radius.topRight(), 0);
		final Vector3f centerTopLeft = new Vector3f(posStart.x() + radius.topLeft(),
				destY - radius.topLeft(), 0);

		// --- Bottom-left corner arc ---
		this.vectorialDraw.circleBorderRaw(centerBottomLeft, radius.bottomLeft(),
				thickness.left(), thickness.bottom(), FMath.PI, FMath.PI * 0.5f);

		// --- Bottom-right corner arc ---
		this.vectorialDraw.circleBorderRaw(centerBottomRight, radius.bottomRight(),
				thickness.bottom(), thickness.right(), FMath.PI * 1.5f, FMath.PI * 0.5f);

		// --- Top-right corner arc ---
		this.vectorialDraw.circleBorderRaw(centerTopRight, radius.topRight(),
				thickness.right(), thickness.top(), 0, FMath.PI * 0.5f);

		// --- Top-left corner arc ---
		this.vectorialDraw.circleBorderRaw(centerTopLeft, radius.topLeft(),
				thickness.top(), thickness.left(), FMath.PI * 0.5f, FMath.PI * 0.5f);

		// --- Bottom border ---
		this.vectorialDraw.drawQuad(
				centerBottomLeft.less(0, radius.bottomLeft(), 0),
				centerBottomLeft.less(0, radius.bottomLeft() - thickness.bottom(), 0),
				centerBottomRight.less(0, radius.bottomRight() - thickness.bottom(), 0),
				centerBottomRight.less(0, radius.bottomRight(), 0));

		// --- Right border ---
		this.vectorialDraw.drawQuad(
				centerBottomRight.add(radius.bottomRight(), 0, 0),
				centerBottomRight.add(radius.bottomRight() - thickness.right(), 0, 0),
				centerTopRight.add(radius.topRight() - thickness.right(), 0, 0),
				centerTopRight.add(radius.topRight(), 0, 0));

		// --- Left border ---
		this.vectorialDraw.drawQuad(
				centerBottomLeft.less(radius.bottomLeft(), 0, 0),
				centerBottomLeft.less(radius.bottomLeft() - thickness.left(), 0, 0),
				centerTopLeft.less(radius.topLeft() - thickness.left(), 0, 0),
				centerTopLeft.less(radius.topLeft(), 0, 0));

		// --- Top border with gap for header ---
		final Vector2f headerOrigin = this.headerSizer.getOrigin().less(this.origin);
		final Vector2f headerSize = this.headerSizer.getSize();
		final float gapStartX = headerOrigin.x() - GAP_PADDING;
		final float gapEndX = headerOrigin.x() + headerSize.x() + GAP_PADDING;

		final float topOuterY = destY;
		final float topInnerY = destY - thickness.top();

		// Left segment of top border: from top-left corner center to gap start
		final float topLeftEndX = centerTopLeft.x();
		if (gapStartX > topLeftEndX) {
			this.vectorialDraw.drawQuad(
					new Vector3f(topLeftEndX, topOuterY, 0),
					new Vector3f(topLeftEndX, topInnerY, 0),
					new Vector3f(gapStartX, topInnerY, 0),
					new Vector3f(gapStartX, topOuterY, 0));
		}

		// Right segment of top border: from gap end to top-right corner center
		final float topRightStartX = centerTopRight.x();
		if (gapEndX < topRightStartX) {
			this.vectorialDraw.drawQuad(
					new Vector3f(gapEndX, topOuterY, 0),
					new Vector3f(gapEndX, topInnerY, 0),
					new Vector3f(topRightStartX, topInnerY, 0),
					new Vector3f(topRightStartX, topOuterY, 0));
		}
	}

	// ========================================================================
	// Header management
	// ========================================================================

	private void rebuildHeader() {
		this.headerSizer.subWidgetRemoveAll();
		if (this.tick != null) {
			this.headerSizer.subWidgetAdd(this.tick);
		}
		if (this.icon != null) {
			this.headerSizer.subWidgetAdd(this.icon);
		}
		this.headerSizer.subWidgetAdd(this.titleLabel);
	}

	// ========================================================================
	// Public API
	// ========================================================================

	/**
	 * Set the title text.
	 * @param title the new title
	 */
	public void setPropertyTitle(final String title) {
		this.titleLabel.setPropertyValue(title);
	}

	/**
	 * Get the title text.
	 * @return the current title
	 */
	public String getPropertyTitle() {
		return this.titleLabel.getPropertyValue();
	}

	/**
	 * Set the enabled state (check/uncheck the tick).
	 * No-op if the FieldSet has no tick.
	 * @param enabled true to enable (check), false to disable (uncheck)
	 */
	public void setEnabled(final boolean enabled) {
		if (this.tick != null) {
			this.tick.setPropertyValue(enabled);
		}
	}

	/**
	 * Get the enabled state.
	 * @return true if enabled or if no tick is present
	 */
	public boolean isEnabled() {
		if (this.tick != null) {
			return this.tick.isChecked();
		}
		return true;
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	/**
	 * Create a new FieldSet with the given title.
	 * @param title the legend text
	 * @return a new FieldSet
	 */
	public static FieldSet create(final String title) {
		return new FieldSet(title);
	}

	/**
	 * Fluent method to set an icon in the header.
	 * @param iconName the icon name (resolved from THEME)
	 * @return this FieldSet for chaining
	 */
	public FieldSet icon(final String iconName) {
		if (this.icon != null) {
			return this;
		}
		this.icon = Icon.create(iconName).expand(false, false).fill(false, true).gravity(Gravity.CENTER);
		rebuildHeader();
		return this;
	}

	/**
	 * Fluent method to add a checkbox (Tick) to the header.
	 * When checked, the content is visible. When unchecked, the content is hidden.
	 * @param checkable true to add a tick
	 * @return this FieldSet for chaining
	 */
	public FieldSet checkable(final boolean checkable) {
		if (!checkable || this.tick != null) {
			return this;
		}
		this.tick = Tick.create().checked(true);
		this.tick.setPropertyExpand(Vector2b.FALSE);
		this.tick.setPropertyFill(Vector2b.FALSE_TRUE);
		this.tick.setPropertyGravity(Gravity.CENTER);
		this.tick.signalValue.connectAuto(this, FieldSet::onTickValueChanged);
		rebuildHeader();
		return this;
	}

	/**
	 * Fluent method to set the initial checked state of the tick.
	 * Must be called after {@link #checkable(boolean)}.
	 * @param checked true for checked (content visible), false for unchecked (content hidden)
	 * @return this FieldSet for chaining
	 */
	public FieldSet checked(final boolean checked) {
		if (this.tick != null) {
			this.tick.setPropertyValue(checked);
			this.contentSizer.setPropertyHide(!checked);
		}
		return this;
	}

	/**
	 * Fluent method to set the title.
	 * @param title the legend text
	 * @return this FieldSet for chaining
	 */
	public FieldSet title(final String title) {
		setPropertyTitle(title);
		return this;
	}

	/**
	 * Fluent method to add a widget to the content area.
	 * @param widget the widget to add
	 * @return this FieldSet for chaining
	 */
	public FieldSet add(final Widget widget) {
		this.contentSizer.subWidgetAdd(widget);
		return this;
	}

	/**
	 * Fluent method to add multiple widgets to the content area.
	 * @param widgets the widgets to add
	 * @return this FieldSet for chaining
	 */
	public FieldSet add(final Widget... widgets) {
		for (final Widget widget : widgets) {
			this.contentSizer.subWidgetAdd(widget);
		}
		return this;
	}

	/**
	 * Fluent method to connect an activation callback.
	 * @param callback the callback to invoke when the section is activated
	 * @return this FieldSet for chaining
	 */
	public FieldSet onActivate(final Runnable callback) {
		this.signalActivate.connect(callback);
		return this;
	}

	/**
	 * Fluent method to connect a deactivation callback.
	 * @param callback the callback to invoke when the section is deactivated
	 * @return this FieldSet for chaining
	 */
	public FieldSet onDeactivate(final Runnable callback) {
		this.signalDeactivate.connect(callback);
		return this;
	}
}
