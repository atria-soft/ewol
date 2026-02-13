/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget.meta;

import java.util.ArrayList;
import java.util.List;

import org.atriasoft.esignal.Signal;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.DimensionInsets;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.event.InputSystem;
import org.atriasoft.ewol.widget.Box;
import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.ScrollView;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.gale.key.KeyStatus;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * SelectPopup is a popup widget displaying a scrollable list of items for selection.
 *
 * <p>Used by the Select widget to display the dropdown list.</p>
 * <p>The popup positions itself below or above the anchor widget depending on available space.</p>
 */
public class SelectPopup extends Widget {
	private static final Logger LOGGER = LoggerFactory.getLogger(SelectPopup.class);

	private static final Color POPUP_BG_COLOR = Color.WHITE;
	private static final Color POPUP_BORDER_COLOR = new Color(0x60, 0x60, 0x60, 0xFF);
	private static final Color ITEM_BG_NORMAL = Color.WHITE;
	private static final Color ITEM_BG_HOVER = new Color(0xE3, 0xF2, 0xFD, 0xFF);
	private static final Color ITEM_BG_SELECTED = new Color(0xBB, 0xDE, 0xFB, 0xFF);
	private static final float POPUP_MAX_HEIGHT = 300.0f;
	private static final float POPUP_MIN_WIDTH = 120.0f;

	public Signal<Integer> signalSelectionChanged = new Signal<>();

	public SignalEmpty signalClosed = new SignalEmpty();

	// Items
	protected final List<String> items = new ArrayList<>();
	protected int selectedIndex = -1;

	// Anchor position (position of the Select widget)
	protected Vector2f anchorOrigin = Vector2f.ZERO;
	protected Vector2f anchorSize = Vector2f.ZERO;

	// UI components - internal popup box
	protected Box popupBox;
	protected Sizer itemsContainer;
	protected ScrollView scrollView;
	protected final List<Button> itemButtons = new ArrayList<>();

	// Keep references to signal connections to prevent GC
	protected final List<org.atriasoft.esignal.Connection> signalConnections = new ArrayList<>();

	// Computed popup position and size
	protected Vector2f popupOrigin = Vector2f.ZERO;
	protected Vector2f popupSize = Vector2f.ZERO;


	/**
	 * Default constructor.
	 */
	public SelectPopup() {
		this.propertyCanFocus = true;
		setMouseLimit(1);
		// This widget fills the whole window
		setPropertyExpand(Vector2b.TRUE);
		setPropertyFill(Vector2b.TRUE);

		// Create popup box with border
		this.popupBox = new Box();
		this.popupBox.setPropertyExpand(Vector2b.FALSE);
		this.popupBox.setPropertyFill(Vector2b.FALSE);
		this.popupBox.setPropertyBorderWidth(new DimensionInsets(2));
		this.popupBox.setPropertyBorderColor(POPUP_BORDER_COLOR);
		this.popupBox.setPropertyColor(POPUP_BG_COLOR);
		this.popupBox.setPropertyPadding(new DimensionInsets(2));
		this.popupBox.setPropertyMargin(new DimensionInsets(0));
		// Don't set parent - we manage events ourselves to avoid bubbling issues

		// Create items container
		this.itemsContainer = Sizer.vertical();
		this.itemsContainer.setPropertyExpand(Vector2b.TRUE);
		this.itemsContainer.setPropertyFill(Vector2b.TRUE);

		// Wrap in ScrollView for scrollable list
		this.scrollView = ScrollView.create();
		this.scrollView.setPropertyExpand(Vector2b.TRUE);
		this.scrollView.setPropertyFill(Vector2b.TRUE);
		this.scrollView.setPropertyShowHorizontal(false);
		this.scrollView.setPropertyShowVertical(true);
		this.scrollView.setSubWidget(this.itemsContainer);

		this.popupBox.setSubWidget(this.scrollView);
	}

	/**
	 * Constructor with items.
	 */
	public SelectPopup(final List<String> items, final int selectedIndex) {
		this();
		setItems(items);
		this.selectedIndex = selectedIndex;
		updateItemsDisplay();
	}

	/**
	 * Set the anchor position (where the Select widget is).
	 * @param origin bottom-left corner of the anchor
	 * @param size size of the anchor
	 */
	public void setAnchorPosition(final Vector2f origin, final Vector2f size) {
		this.anchorOrigin = origin;
		this.anchorSize = size;
	}

	/**
	 * Set the items list.
	 */
	public void setItems(final List<String> items) {
		this.items.clear();
		if (items != null) {
			this.items.addAll(items);
		}
		updateItemsDisplay();
	}

	/**
	 * Update the items display.
	 */
	protected void updateItemsDisplay() {
		if (this.itemsContainer == null) {
			return;
		}

		// Clear existing
		this.itemsContainer.subWidgetRemoveAll();
		this.itemButtons.clear();
		this.signalConnections.clear();

		// Create buttons for each item
		for (int i = 0; i < this.items.size(); i++) {
			final int index = i;
			final String item = this.items.get(i);

			final Button itemButton = new Button();
			itemButton.setPropertyExpand(Vector2b.TRUE_FALSE);
			itemButton.setPropertyFill(Vector2b.TRUE);
			itemButton.setPropertyBorderWidth(new DimensionInsets(0));
			itemButton.setPropertyPadding(new DimensionInsets(6, 10, 6, 10));
			itemButton.setPropertyMargin(new DimensionInsets(0));

			// Set background based on selection
			if (index == this.selectedIndex) {
				itemButton.setPropertyColor(ITEM_BG_SELECTED);
			} else {
				itemButton.setPropertyColor(ITEM_BG_NORMAL);
			}

			// Create label for item
			final Label label = new Label(item);
			label.setPropertyFontSize(12);
			label.setPropertyExpand(Vector2b.TRUE_FALSE);
			label.setPropertyFill(Vector2b.TRUE);
			label.setPropertyGravity(Gravity.LEFT);
			itemButton.setSubWidget(label);

			// Connect click handler - store connections to prevent GC
			this.signalConnections.add(itemButton.signalClick.connect(() -> onItemClicked(index)));
			this.signalConnections.add(itemButton.signalEnter.connect(() -> onItemHover(index, true)));
			this.signalConnections.add(itemButton.signalLeave.connect(() -> onItemHover(index, false)));

			this.itemButtons.add(itemButton);
			this.itemsContainer.subWidgetAdd(itemButton);
		}

		markToRedraw();
	}

	/**
	 * Handle item click.
	 */
	protected void onItemClicked(final int index) {
		LOGGER.info("Item clicked: {} ({})", index, this.items.get(index));
		this.signalSelectionChanged.emit(index);
		closePopup();
	}

	/**
	 * Handle item hover.
	 */
	protected void onItemHover(final int index, final boolean hovering) {
		if (index >= 0 && index < this.itemButtons.size()) {
			final Button button = this.itemButtons.get(index);
			if (hovering) {
				button.setPropertyColor(ITEM_BG_HOVER);
			} else {
				button.setPropertyColor(index == this.selectedIndex ? ITEM_BG_SELECTED : ITEM_BG_NORMAL);
			}
		}
	}

	/**
	 * Close the popup and emit closed signal.
	 */
	protected void closePopup() {
		this.signalClosed.emit();
		autoDestroy();
	}

	/**
	 * Check if a position is inside the popup box.
	 */
	protected boolean isInsidePopup(final Vector2f pos) {
		return pos.x() >= this.popupOrigin.x()
				&& pos.x() <= this.popupOrigin.x() + this.popupSize.x()
				&& pos.y() >= this.popupOrigin.y()
				&& pos.y() <= this.popupOrigin.y() + this.popupSize.y();
	}

	@Override
	public void calculateMinMaxSize() {
		// This widget fills the whole window
		this.minSize = Vector2f.ZERO;
		this.maxSize = new Vector2f(Float.MAX_VALUE, Float.MAX_VALUE);

		// Calculate popup box size
		this.popupBox.calculateMinMaxSize();
	}

	@Override
	public void onChangeSize() {
		markToRedraw();

		// Set dynamic min size based on anchor width (with minimum of POPUP_MIN_WIDTH)
		final float minWidth = Math.max(POPUP_MIN_WIDTH, this.anchorSize.x());
		this.popupBox.setPropertyMinSize(new Dimension2f(new Vector2f(minWidth, 200), Distance.PIXEL));

		// Calculate popup box min size
		this.popupBox.calculateMinMaxSize();
		final Vector2f popupMinSize = this.popupBox.getCalculateMinSize();

		// Popup width: at least anchor width or content width
		float popupWidth = Math.max(minWidth, popupMinSize.x());

		// Popup height: content height clamped to max
		float popupHeight = Math.min(popupMinSize.y(), POPUP_MAX_HEIGHT);

		// Determine if popup should appear below or above the anchor
		// In ewol, Y=0 is at the bottom, Y increases upward
		final float spaceBelow = this.anchorOrigin.y();
		final float spaceAbove = this.size.y() - (this.anchorOrigin.y() + this.anchorSize.y());

		if (spaceBelow >= popupHeight || spaceBelow >= spaceAbove) {
			// Position below the anchor
			this.popupOrigin = new Vector2f(this.anchorOrigin.x(), this.anchorOrigin.y() - popupHeight);
			if (this.popupOrigin.y() < 0) {
				popupHeight = this.anchorOrigin.y();
				this.popupOrigin = new Vector2f(this.anchorOrigin.x(), 0);
			}
		} else {
			// Position above the anchor
			this.popupOrigin = new Vector2f(this.anchorOrigin.x(), this.anchorOrigin.y() + this.anchorSize.y());
			if (this.popupOrigin.y() + popupHeight > this.size.y()) {
				popupHeight = this.size.y() - this.popupOrigin.y();
			}
		}

		// Clamp X position
		if (this.popupOrigin.x() + popupWidth > this.size.x()) {
			this.popupOrigin = this.popupOrigin.withX(this.size.x() - popupWidth);
		}
		if (this.popupOrigin.x() < 0) {
			this.popupOrigin = this.popupOrigin.withX(0);
		}

		this.popupSize = new Vector2f(popupWidth, popupHeight);

		// Set popup box position and size
		this.popupBox.setOrigin(this.popupOrigin);
		this.popupBox.setSize(this.popupSize);
		this.popupBox.onChangeSize();
	}

	@Override
	public void onRegenerateDisplay() {
		if (!needRedraw()) {
			return;
		}
		this.popupBox.systemRegenerateDisplay();
	}

	@Override
	public void systemDraw(final DrawProperty displayProp) {
		if (this.propertyHide) {
			return;
		}
		// Only draw the popup box (not the whole overlay)
		final DrawProperty prop = displayProp.withLimit(this.popupOrigin, this.popupSize);
		this.popupBox.systemDraw(prop);
	}

	@Override
	public Widget getWidgetAtPos(final Vector2f pos) {
		if (this.propertyHide) {
			return null;
		}
		// Always return this - we handle event dispatching ourselves
		return this;
	}

	@Override
	public boolean systemEventInput(final InputSystem event) {
		final Vector2f pos = event.event().pos();
		final boolean inside = isInsidePopup(pos);

		if (onEventInput( event.event())) {
			return true;
		}
		// If inside popup, find the target widget and forward the event
		if (inside) {
			final Widget target = this.popupBox.getWidgetAtPos(pos);
			if (target != null && target != this && target != this.popupBox) {
				// Forward event - no parent set on popupBox so no bubbling back
				return target.systemEventInput(event);
			}
		}
		return false;
	}

	@Override
	protected boolean onEventInput(final EventInput event) {
		final boolean inside = isInsidePopup(event.pos());
		// Close popup when clicking outside
		if (!inside && event.inputId() == 1 && event.status() == KeyStatus.pressSingle) {
			LOGGER.debug("Click outside popup - closing");
			closePopup();
			return true;
		}
		return false;
	}

	// ========================================================================
	// Factory methods
	// ========================================================================

	/**
	 * Create a new SelectPopup.
	 * @return a new SelectPopup
	 */
	public static SelectPopup create() {
		return new SelectPopup();
	}

	/**
	 * Create a new SelectPopup with items.
	 * @param items the items to display
	 * @param selectedIndex the currently selected index
	 * @return a new SelectPopup
	 */
	public static SelectPopup create(final List<String> items, final int selectedIndex) {
		return new SelectPopup(items, selectedIndex);
	}

	/**
	 * Fluent method to set items.
	 * @param items the items
	 * @return this popup for chaining
	 */
	public SelectPopup items(final List<String> items) {
		setItems(items);
		return this;
	}

	/**
	 * Fluent method to set selected index.
	 * @param index the selected index
	 * @return this popup for chaining
	 */
	public SelectPopup selectedIndex(final int index) {
		this.selectedIndex = index;
		updateItemsDisplay();
		return this;
	}

	/**
	 * Fluent method to set anchor position.
	 * @param origin bottom-left corner of the anchor
	 * @param size size of the anchor
	 * @return this popup for chaining
	 */
	public SelectPopup anchorPosition(final Vector2f origin, final Vector2f size) {
		setAnchorPosition(origin, size);
		return this;
	}
}
