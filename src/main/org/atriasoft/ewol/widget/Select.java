/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.dataformat.xml.annotation.JacksonXmlProperty;
import org.atriasoft.esignal.Signal;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.DimensionInsets;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.widget.meta.SelectPopup;
import org.atriasoft.gale.key.KeyStatus;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Select widget (ComboBox/Dropdown) that displays the selected item
 * and opens a popup list for selection.
 *
 * <p>Example usage:</p>
 * <pre>{@code
 * Select select = Select.create()
 *     .items("Option 1", "Option 2", "Option 3")
 *     .selectedIndex(0)
 *     .onSelectionChanged((index, value) -> System.out.println("Selected: " + value));
 * }</pre>
 *
 * Signals emitted:
 * - signalSelectionChanged: when selection changes (emits index)
 */
public class Select extends Box {
	private static final Logger LOGGER = LoggerFactory.getLogger(Select.class);

	private static final Color ARROW_COLOR = new Color(0x60, 0x60, 0x60, 0xFF);

	public Signal<Integer> signalSelectionChanged = new Signal<>();

	// Items list
	protected final List<String> items = new ArrayList<>();

	// Currently selected index (-1 = no selection)
	protected int propertySelectedIndex = -1;

	// Placeholder text when no selection
	protected String propertyPlaceholder = "";

	// Popup open state
	protected boolean popupOpen = false;

	// UI components
	protected Label displayLabel;
	protected Icon arrowIcon;
	protected Sizer contentSizer;

	/**
	 * Default constructor.
	 */
	public Select() {
		this.propertyCanFocus = true;
		setMouseLimit(1);
		setPropertyExpand(Vector2b.FALSE);
		setPropertyFill(Vector2b.FALSE);
		setPropertyBorderWidth(new DimensionInsets(1));
		setPropertyBorderColor(new Color(0x80, 0x80, 0x80, 0xFF));
		setPropertyColor(Color.WHITE);
		setPropertyPadding(new DimensionInsets(4));
		setPropertyMargin(new DimensionInsets(2));
		setPropertyMinSize(new Dimension2f(new Vector2f(100, 28), Distance.PIXEL));

		// Create horizontal layout
		this.contentSizer = Sizer.horizontal();
		this.contentSizer.setPropertyExpand(Vector2b.TRUE);
		this.contentSizer.setPropertyFill(Vector2b.TRUE);

		// Display label for selected item
		this.displayLabel = new Label();
		this.displayLabel.setPropertyFontSize(12);
		this.displayLabel.setPropertyExpand(Vector2b.TRUE_FALSE);
		this.displayLabel.setPropertyFill(Vector2b.TRUE);
		this.displayLabel.setPropertyGravity(Gravity.LEFT);
		this.contentSizer.subWidgetAdd(this.displayLabel);

		// Arrow icon (chevron down when closed, chevron up when open)
		this.arrowIcon = Icon.create("chevron-down")
				.fill(ARROW_COLOR)
				.background(ARROW_COLOR)
				.size(new Dimension2f(new Vector2f(16, 16)))
				.minSize(new Dimension2f(new Vector2f(16, 16)));
		this.arrowIcon.setPropertyExpand(Vector2b.FALSE);
		this.arrowIcon.setPropertyFill(Vector2b.FALSE);
		this.arrowIcon.setPropertyGravity(Gravity.CENTER);
		this.contentSizer.subWidgetAdd(this.arrowIcon);

		setSubWidget(this.contentSizer);
		updateDisplayLabel();
	}

	/**
	 * Updates the arrow icon based on popup state.
	 */
	protected void updateArrowIcon() {
		if (this.arrowIcon == null) {
			return;
		}
		if (this.popupOpen) {
			this.arrowIcon.icon("chevron-up");
		} else {
			this.arrowIcon.icon("chevron-down");
		}
	}

	/**
	 * Updates the display label with the current selection.
	 */
	protected void updateDisplayLabel() {
		if (this.displayLabel == null) {
			return;
		}
		if (this.propertySelectedIndex >= 0 && this.propertySelectedIndex < this.items.size()) {
			this.displayLabel.setPropertyValue(this.items.get(this.propertySelectedIndex));
		} else if (!this.propertyPlaceholder.isEmpty()) {
			this.displayLabel.setPropertyValue("<i>" + this.propertyPlaceholder + "</i>");
		} else {
			this.displayLabel.setPropertyValue("");
		}
	}

	// ========================================================================
	// Items management
	// ========================================================================

	/**
	 * Get the list of items.
	 * @return copy of items list
	 */
	public List<String> getItems() {
		return new ArrayList<>(this.items);
	}

	/**
	 * Set the list of items.
	 * @param items the items to set
	 */
	public void setItems(final List<String> items) {
		this.items.clear();
		if (items != null) {
			this.items.addAll(items);
		}
		// Reset selection if out of bounds
		if (this.propertySelectedIndex >= this.items.size()) {
			this.propertySelectedIndex = this.items.isEmpty() ? -1 : 0;
		}
		updateDisplayLabel();
		markToRedraw();
	}

	/**
	 * Add an item to the list.
	 * @param item the item to add
	 */
	public void addItem(final String item) {
		if (item != null) {
			this.items.add(item);
			updateDisplayLabel();
			markToRedraw();
		}
	}

	/**
	 * Remove an item from the list.
	 * @param index the index to remove
	 */
	public void removeItem(final int index) {
		if (index >= 0 && index < this.items.size()) {
			this.items.remove(index);
			if (this.propertySelectedIndex >= this.items.size()) {
				this.propertySelectedIndex = this.items.isEmpty() ? -1 : this.items.size() - 1;
			}
			updateDisplayLabel();
			markToRedraw();
		}
	}

	/**
	 * Clear all items.
	 */
	public void clearItems() {
		this.items.clear();
		this.propertySelectedIndex = -1;
		updateDisplayLabel();
		markToRedraw();
	}

	/**
	 * Get the number of items.
	 * @return the item count
	 */
	public int getItemCount() {
		return this.items.size();
	}

	// ========================================================================
	// Property accessors
	// ========================================================================

	@JsonProperty("selected-index")
	@JacksonXmlProperty(isAttribute = true, localName = "selected-index")
	public int getPropertySelectedIndex() {
		return this.propertySelectedIndex;
	}

	public void setPropertySelectedIndex(final int index) {
		final int newIndex = Math.max(-1, Math.min(index, this.items.size() - 1));
		if (this.propertySelectedIndex == newIndex) {
			return;
		}
		this.propertySelectedIndex = newIndex;
		updateDisplayLabel();
		markToRedraw();
	}

	/**
	 * Get the currently selected value.
	 * @return the selected value, or null if no selection
	 */
	public String getSelectedValue() {
		if (this.propertySelectedIndex >= 0 && this.propertySelectedIndex < this.items.size()) {
			return this.items.get(this.propertySelectedIndex);
		}
		return null;
	}

	/**
	 * Set selection by value.
	 * @param value the value to select
	 */
	public void setSelectedValue(final String value) {
		final int index = this.items.indexOf(value);
		if (index >= 0) {
			setPropertySelectedIndex(index);
		}
	}

	@JsonProperty("placeholder")
	@JacksonXmlProperty(isAttribute = true, localName = "placeholder")
	public String getPropertyPlaceholder() {
		return this.propertyPlaceholder;
	}

	public void setPropertyPlaceholder(final String placeholder) {
		if (Objects.equals(this.propertyPlaceholder, placeholder)) {
			return;
		}
		this.propertyPlaceholder = placeholder != null ? placeholder : "";
		updateDisplayLabel();
		markToRedraw();
	}

	// ========================================================================
	// Event handling
	// ========================================================================

	@Override
	public Widget getWidgetAtPos(final Vector2f pos) {
		if (this.propertyHide) {
			return null;
		}
		final Vector2f relPos = relativePosition(pos);
		if (isInside(relPos)) {
			return this;
		}
		return null;
	}

	@Override
	public boolean onEventInput(final EventInput event) {
		final Vector2f relPos = relativePosition(event.pos());
		final boolean over = isInside(relPos);

		if (event.status() == KeyStatus.leave) {
			markToRedraw();
			return true;
		}

		if (event.inputId() != 1) {
			return over;
		}

		if (KeyStatus.pressSingle == event.status() && over) {
			LOGGER.debug("Select clicked - opening popup");
			keepFocus();
			openSelectPopup();
			return true;
		}

		return over;
	}

	/**
	 * Opens the selection popup.
	 */
	protected void openSelectPopup() {
		final Windows windows = getWindows();
		if (windows == null) {
			LOGGER.error("Cannot open popup: no windows found");
			return;
		}

		if (this.items.isEmpty()) {
			LOGGER.debug("No items to display");
			return;
		}

		// Calculate position for popup (below or above the Select widget)
		// origin is the bottom-left corner of the widget in window coordinates
		final Vector2f selectOrigin = this.origin;
		final Vector2f selectSize = this.size;

		// Create the popup with anchor position
		final SelectPopup popup = SelectPopup.create(this.items, this.propertySelectedIndex)
				.anchorPosition(selectOrigin, selectSize);
		popup.signalSelectionChanged.connectAuto(this, Select::onCallbackSelectionChanged);
		popup.signalClosed.connectAuto(this, Select::onCallbackPopupClosed);

		// Update state
		this.popupOpen = true;
		updateArrowIcon();

		windows.popUpWidgetPush(popup);
	}

	// Static callback for signal connection
	private static void onCallbackSelectionChanged(final Select self, final Integer index) {
		if (index != null && index >= 0 && index < self.items.size()) {
			self.setPropertySelectedIndex(index);
			self.signalSelectionChanged.emit(index);
		}
	}

	private static void onCallbackPopupClosed(final Select self) {
		self.popupOpen = false;
		self.updateArrowIcon();
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	/**
	 * Create a new Select.
	 * @return a new Select
	 */
	public static Select create() {
		return new Select();
	}

	/**
	 * Create a new Select with items.
	 * @param items the items
	 * @return a new Select
	 */
	public static Select create(final List<String> items) {
		final Select select = new Select();
		select.setItems(items);
		return select;
	}

	/**
	 * Create a new Select with items.
	 * @param items the items
	 * @return a new Select
	 */
	public static Select create(final String... items) {
		final Select select = new Select();
		if (items != null) {
			for (final String item : items) {
				select.addItem(item);
			}
		}
		return select;
	}

	/**
	 * Fluent method to set items from a list.
	 * @param items the items
	 * @return this select for chaining
	 */
	public Select items(final List<String> items) {
		setItems(items);
		return this;
	}

	/**
	 * Fluent method to set items from varargs.
	 * @param items the items
	 * @return this select for chaining
	 */
	public Select items(final String... items) {
		this.items.clear();
		if (items != null) {
			for (final String item : items) {
				if (item != null) {
					this.items.add(item);
				}
			}
		}
		updateDisplayLabel();
		markToRedraw();
		return this;
	}

	/**
	 * Fluent method to add an item.
	 * @param item the item to add
	 * @return this select for chaining
	 */
	public Select item(final String item) {
		addItem(item);
		return this;
	}

	/**
	 * Fluent method to set selected index.
	 * @param index the index to select
	 * @return this select for chaining
	 */
	public Select selectedIndex(final int index) {
		setPropertySelectedIndex(index);
		return this;
	}

	/**
	 * Fluent method to set selected value.
	 * @param value the value to select
	 * @return this select for chaining
	 */
	public Select selectedValue(final String value) {
		setSelectedValue(value);
		return this;
	}

	/**
	 * Fluent method to set placeholder.
	 * @param placeholder the placeholder text
	 * @return this select for chaining
	 */
	public Select placeholder(final String placeholder) {
		setPropertyPlaceholder(placeholder);
		return this;
	}

	/**
	 * Fluent method to connect a selection changed callback.
	 * @param callback the callback to invoke when selection changes
	 * @return this select for chaining
	 */
	public Select onSelectionChanged(final java.util.function.Consumer<Integer> callback) {
		this.signalSelectionChanged.connect(callback);
		return this;
	}

	/**
	 * Fluent method to connect a selection changed callback with value.
	 * @param callback the callback to invoke when selection changes (receives index and value)
	 * @return this select for chaining
	 */
	public Select onSelectionChanged(final java.util.function.BiConsumer<Integer, String> callback) {
		this.signalSelectionChanged.connect(index -> {
			final String value = (index >= 0 && index < this.items.size()) ? this.items.get(index) : null;
			callback.accept(index, value);
		});
		return this;
	}
}
