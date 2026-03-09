package org.atriasoft.ewol.widget;

import java.util.ArrayList;
import java.util.List;
import java.util.function.Consumer;

import org.atriasoft.esignal.Connection;
import org.atriasoft.esignal.Signal;

/**
 * RadioGroup manages a group of {@link RadioButton} widgets with exclusive selection.
 * When one radio button is selected, all others are automatically deselected.
 *
 * <p>Extends {@link Sizer} for layout (vertical by default).</p>
 *
 * <p>Example usage:</p>
 * <pre>{@code
 * RadioGroup group = RadioGroup.vertical()
 *     .addRadio("Option A")
 *     .addRadio("Option B")
 *     .addRadio("Option C")
 *     .selectedIndex(0)
 *     .onSelectionChanged(index -> System.out.println("Selected: " + index));
 * }</pre>
 *
 * Signals emitted:
 * - signalSelectionChanged: when the selected index changes (emits the new index)
 */
public class RadioGroup extends Sizer {

	public Signal<Integer> signalSelectionChanged = new Signal<>();

	private final List<RadioButton> radioButtons = new ArrayList<>();
	private final List<Connection> radioConnections = new ArrayList<>();
	private int selectedIndex = -1;
	private boolean updating = false;

	/**
	 * Constructor with default vertical layout.
	 */
	public RadioGroup() {
		super(DisplayMode.VERTICAL);
	}

	/**
	 * Constructor with specified layout mode.
	 * @param mode The display mode (HORIZONTAL or VERTICAL)
	 */
	public RadioGroup(final DisplayMode mode) {
		super(mode);
	}

	/**
	 * Add a RadioButton to this group.
	 * The button is connected for exclusive selection management.
	 * @param radioButton the radio button to add
	 * @return this group for chaining
	 */
	public RadioGroup addRadio(final RadioButton radioButton) {
		final int index = this.radioButtons.size();
		this.radioButtons.add(radioButton);
		subWidgetAdd(radioButton);

		final Connection connection = radioButton.signalValue.connect((final Boolean value) -> {
			if (this.updating) {
				return;
			}
			if (Boolean.TRUE.equals(value)) {
				selectIndex(index);
			}
		});
		this.radioConnections.add(connection);

		return this;
	}

	/**
	 * Add a radio button with a text label.
	 * @param label the label text
	 * @return this group for chaining
	 */
	public RadioGroup addRadio(final String label) {
		return addRadio(new RadioButton(label));
	}

	/**
	 * Add a radio button with an arbitrary widget content.
	 * @param content the widget content
	 * @return this group for chaining
	 */
	public RadioGroup addRadioWidget(final Widget content) {
		return addRadio(new RadioButton(content));
	}

	/**
	 * Select a radio button by index.
	 * @param index the index to select (-1 to deselect all)
	 */
	private void selectIndex(final int index) {
		if (this.selectedIndex == index) {
			return;
		}
		this.updating = true;
		try {
			// Deselect all others
			for (int i = 0; i < this.radioButtons.size(); i++) {
				if (i != index) {
					this.radioButtons.get(i).setPropertyValue(false);
				}
			}
			// Ensure the selected one is selected
			if (index >= 0 && index < this.radioButtons.size()) {
				this.radioButtons.get(index).setPropertyValue(true);
			}
			this.selectedIndex = index;
		} finally {
			this.updating = false;
		}
		this.signalSelectionChanged.emit(index);
	}

	public int getPropertySelectedIndex() {
		return this.selectedIndex;
	}

	public void setPropertySelectedIndex(final int index) {
		if (index < -1 || index >= this.radioButtons.size()) {
			return;
		}
		selectIndex(index);
	}

	/**
	 * Get the currently selected RadioButton (or null if none).
	 * @return the selected radio button
	 */
	public RadioButton getSelected() {
		if (this.selectedIndex >= 0 && this.selectedIndex < this.radioButtons.size()) {
			return this.radioButtons.get(this.selectedIndex);
		}
		return null;
	}

	/**
	 * Get the list of radio buttons in this group.
	 * @return unmodifiable view of radio buttons
	 */
	public List<RadioButton> getRadioButtons() {
		return List.copyOf(this.radioButtons);
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	public static RadioGroup vertical() {
		return new RadioGroup(DisplayMode.VERTICAL);
	}

	public static RadioGroup horizontal() {
		return new RadioGroup(DisplayMode.HORIZONTAL);
	}

	public RadioGroup selectedIndex(final int index) {
		setPropertySelectedIndex(index);
		return this;
	}

	public RadioGroup onSelectionChanged(final Consumer<Integer> callback) {
		this.signalSelectionChanged.connect(callback::accept);
		return this;
	}
}
