package org.atriasoft.ewol.widget.notification;

import org.atriasoft.esignal.Connection;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.DimensionBorderRadius;
import org.atriasoft.etk.DimensionInsets;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.widget.Box;
import org.atriasoft.ewol.widget.Icon;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.Sizer;

/**
 * Individual toast notification widget.
 * Composed of a Box containing a title, description, and close button.
 */
public class Toast {
	/** Signal emitted when the toast requests dismissal (close clicked or timeout) */
	public final SignalEmpty signalDismiss = new SignalEmpty();

	private final ToastType type;
	private final String title;
	private final String description;
	private float timeoutSeconds;
	private float elapsedSeconds = 0.0f;

	private final Box rootBox;
	@SuppressWarnings("unused")
	private final Connection closeConnection;

	// Per-toast overrides (null = use global default)
	private Gravity overridePosition;
	private Float overrideWidth;
	private Float overrideTimeout;

	public Toast(final ToastType type, final String title, final String description,
			final ToastConfig config) {
		this.type = type;
		this.title = title;
		this.description = description;
		this.timeoutSeconds = config.getTimeoutSeconds();

		final String textColorHex = colorToHex(type.getTextColor());

		// Title label (bold, colored)
		final Label titleLabel = new Label("<font color=\"" + textColorHex + "\"><b>" + escapeXml(title) + "</b></font>");
		titleLabel.setPropertyFontSize(config.getTitleFontSize());
		titleLabel.setPropertyAutoTranslate(false);
		titleLabel.setPropertyExpand(Vector2b.TRUE_FALSE);
		titleLabel.setPropertyFill(Vector2b.TRUE_FALSE);

		// Description label (colored)
		final Label descriptionLabel = new Label("<font color=\"" + textColorHex + "\">" + escapeXml(description) + "</font>");
		descriptionLabel.setPropertyFontSize(config.getDescriptionFontSize());
		descriptionLabel.setPropertyAutoTranslate(false);
		descriptionLabel.setPropertyExpand(Vector2b.TRUE_FALSE);
		descriptionLabel.setPropertyFill(Vector2b.TRUE_FALSE);

		// Close icon
		final Icon closeIcon = Icon.create("close")
				.fill(type.getTextColor())
				.background(Color.NONE);
		closeIcon.setPropertyMinSize(new Dimension2f(
				new Vector2f(16f, 16f), Distance.PIXEL));
		this.closeConnection = closeIcon.signalPressed.connect(this::onCloseClicked);

		// Text column: vertical sizer with title + description
		final Sizer textColumn = Sizer.vertical()
				.add(titleLabel)
				.add(descriptionLabel);
		textColumn.setPropertyExpand(Vector2b.TRUE_FALSE);
		textColumn.setPropertyFill(Vector2b.TRUE_FALSE);

		// Main horizontal layout: text column + close icon
		final Sizer mainRow = Sizer.horizontal()
				.add(textColumn)
				.add(closeIcon);
		mainRow.setPropertyExpand(Vector2b.TRUE_FALSE);
		mainRow.setPropertyFill(Vector2b.TRUE_FALSE);

		// Root box with type-specific colors
		this.rootBox = new Box();
		this.rootBox.setPropertyColor(type.getBackgroundColor());
		this.rootBox.setPropertyBorderColor(type.getBorderColor());
		this.rootBox.setPropertyBorderWidth(new DimensionInsets(config.getBorderWidth()));
		this.rootBox.setPropertyBorderRadius(new DimensionBorderRadius(config.getBorderRadius()));
		this.rootBox.setPropertyPadding(new DimensionInsets(config.getPadding()));
		this.rootBox.setPropertyMargin(DimensionInsets.ZERO);
		this.rootBox.setPropertyExpand(Vector2b.FALSE);
		this.rootBox.setPropertyFill(Vector2b.TRUE_FALSE);
		this.rootBox.setPropertyMinSize(new Dimension2f(new Vector2f(config.getWidth(), 0), Distance.PIXEL));
		this.rootBox.setPropertyMaxSize(new Dimension2f(new Vector2f(config.getWidth(), Float.MAX_VALUE), Distance.PIXEL));
		this.rootBox.setSubWidget(mainRow);
	}

	/**
	 * Advance the toast elapsed time.
	 * @param deltaSeconds seconds since last call
	 * @return true if the toast has timed out
	 */
	public boolean advanceTime(final float deltaSeconds) {
		if (this.timeoutSeconds <= 0) {
			return false;
		}
		this.elapsedSeconds += deltaSeconds;
		return this.elapsedSeconds >= this.timeoutSeconds;
	}

	private void onCloseClicked() {
		this.signalDismiss.emit();
	}

	public ToastType getType() {
		return this.type;
	}

	public String getTitle() {
		return this.title;
	}

	public String getDescription() {
		return this.description;
	}

	public Box getRootBox() {
		return this.rootBox;
	}

	/**
	 * Position and size the toast at the given location.
	 */
	public void layoutAt(final Vector2f origin, final Vector2f size) {
		this.rootBox.calculateMinMaxSize();
		this.rootBox.setOrigin(origin);
		this.rootBox.setSize(size);
		this.rootBox.onChangeSize();
	}

	/**
	 * Get the calculated minimum size of the toast.
	 */
	public Vector2f getCalculatedMinSize() {
		this.rootBox.calculateMinMaxSize();
		return this.rootBox.getCalculateMinSize();
	}

	/**
	 * Regenerate the display of the toast.
	 */
	public void regenerate() {
		this.rootBox.markToRedraw();
		this.rootBox.systemRegenerateDisplay();
	}

	/**
	 * Draw the toast.
	 */
	public void draw(final DrawProperty displayProp) {
		this.rootBox.systemDraw(displayProp);
	}

	// Per-toast overrides (fluent API)

	public Toast overridePosition(final Gravity position) {
		this.overridePosition = position;
		return this;
	}

	public Toast overrideWidth(final float width) {
		this.overrideWidth = width;
		return this;
	}

	public Toast overrideTimeout(final float timeout) {
		this.overrideTimeout = timeout;
		this.timeoutSeconds = timeout;
		return this;
	}

	public Gravity getOverridePosition() {
		return this.overridePosition;
	}

	public Float getOverrideWidth() {
		return this.overrideWidth;
	}

	public Float getOverrideTimeout() {
		return this.overrideTimeout;
	}

	private static String escapeXml(final String text) {
		if (text == null) {
			return "";
		}
		return text.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;");
	}

	private static String colorToHex(final Color color) {
		return String.format("#%02X%02X%02X",
				(int) (color.r() * 255),
				(int) (color.g() * 255),
				(int) (color.b() * 255));
	}
}
