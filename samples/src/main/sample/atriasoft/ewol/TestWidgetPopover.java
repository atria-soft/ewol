package sample.atriasoft.ewol;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.Icon;
import org.atriasoft.ewol.widget.ImageDisplay;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Sizer.DisplayMode;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.ewol.widget.meta.Popover;
import org.atriasoft.ewol.widget.meta.PopoverPosition;
import org.atriasoft.ewol.widget.meta.PopoverTrigger;

/**
 * Test widget for Popover.
 * Demonstrates positions, rich content with images and decorated text.
 */
public class TestWidgetPopover implements TestWidgetInterface {

	private final int[] clickCounters = new int[8];

	@Override
	public Widget getWidget() {
		final Sizer mainSizer = new Sizer(DisplayMode.VERTICAL);
		mainSizer.setPropertyExpand(Vector2b.TRUE);
		mainSizer.setPropertyFill(Vector2b.TRUE);

		// Title
		final Label titleLabel = new Label("<b>Popover Widget Demo</b>");
		titleLabel.setPropertyExpand(Vector2b.TRUE_FALSE);
		titleLabel.setPropertyFill(Vector2b.TRUE_FALSE);
		mainSizer.subWidgetAdd(titleLabel);

		// Row 1: Position modes (with click counters)
		final Label row1Label = new Label("Position modes (click count in label):");
		row1Label.setPropertyExpand(Vector2b.TRUE_FALSE);
		mainSizer.subWidgetAdd(row1Label);

		final Sizer row1 = new Sizer(DisplayMode.HORIZONTAL);
		row1.setPropertyExpand(Vector2b.TRUE_FALSE);
		row1.setPropertyFill(Vector2b.TRUE);
		mainSizer.subWidgetAdd(row1);

		// Buttons ordered so each direction has space to display its popover
		row1.subWidgetAdd(createCounterButton("Right", 0,
				(btn) -> showPositionPopover(btn, PopoverPosition.RIGHT)));
		row1.subWidgetAdd(createCounterButton("Bottom", 1,
				(btn) -> showPositionPopover(btn, PopoverPosition.BOTTOM)));
		row1.subWidgetAdd(createCounterButton("Auto", 2,
				(btn) -> showPositionPopover(btn, PopoverPosition.AUTO)));
		row1.subWidgetAdd(createCounterButton("Top", 3,
				(btn) -> showPositionPopover(btn, PopoverPosition.TOP)));
		row1.subWidgetAdd(createCounterButton("Left", 4,
				(btn) -> showPositionPopover(btn, PopoverPosition.LEFT)));

		// Row 2: Rich content (with click counters)
		final Label row2Label = new Label("Rich content (click count in label):");
		row2Label.setPropertyExpand(Vector2b.TRUE_FALSE);
		mainSizer.subWidgetAdd(row2Label);

		final Sizer row2 = new Sizer(DisplayMode.HORIZONTAL);
		row2.setPropertyExpand(Vector2b.TRUE_FALSE);
		row2.setPropertyFill(Vector2b.TRUE);
		mainSizer.subWidgetAdd(row2);

		row2.subWidgetAdd(createCounterButton("Image", 5,
				(btn) -> showImagePopover(btn)));
		row2.subWidgetAdd(createCounterButton("Decorated", 6,
				(btn) -> showDecoratedTextPopover(btn)));
		row2.subWidgetAdd(createCounterButton("Complex", 7,
				(btn) -> showComplexPopover(btn)));

		// Row 3: Hover triggers (popover on mouse hover with delay)
		final Label row3Label = new Label("Hover triggers (200ms delay):");
		row3Label.setPropertyExpand(Vector2b.TRUE_FALSE);
		mainSizer.subWidgetAdd(row3Label);

		final Sizer row3 = new Sizer(DisplayMode.HORIZONTAL);
		row3.setPropertyExpand(Vector2b.TRUE_FALSE);
		row3.setPropertyFill(Vector2b.TRUE);
		mainSizer.subWidgetAdd(row3);

		row3.subWidgetAdd(PopoverTrigger.create(new Label("Hover me!"))
				.popoverContent(() -> new Label("Simple tooltip-like popover"))
				.hoverDelay(0.2f));

		row3.subWidgetAdd(PopoverTrigger.create(Button.create("Hover Button"))
				.popoverContent(() -> Sizer.vertical()
						.add(new Label("<b>Info</b>"))
						.add(new Label("Details about this button")))
				.hoverDelay(0.2f)
				.position(PopoverPosition.TOP));

		row3.subWidgetAdd(PopoverTrigger.create(
				Icon.create("settings").fill(Color.GRAY))
				.popoverContent(() -> new Label("Settings icon"))
				.hoverDelay(0.3f));

		// Row 4: Follow-mouse hover (popover follows cursor)
		final Label row4Label = new Label("Follow mouse (popover tracks cursor):");
		row4Label.setPropertyExpand(Vector2b.TRUE_FALSE);
		mainSizer.subWidgetAdd(row4Label);

		final Sizer row4 = new Sizer(DisplayMode.HORIZONTAL);
		row4.setPropertyExpand(Vector2b.TRUE_FALSE);
		row4.setPropertyFill(Vector2b.TRUE);
		mainSizer.subWidgetAdd(row4);

		row4.subWidgetAdd(PopoverTrigger.create(new Label("Follow me!"))
				.popoverContent(() -> new Label("I follow the cursor"))
				.hoverDelay(0.15f)
				.followMouse(true));

		row4.subWidgetAdd(PopoverTrigger.create(Button.create("Hover + Follow"))
				.popoverContent(() -> Sizer.vertical()
						.add(new Label("<b>Tracking</b>"))
						.add(new Label("This popover follows your mouse")))
				.hoverDelay(0.2f)
				.followMouse(true)
				.position(PopoverPosition.TOP));

		return mainSizer;
	}

	/**
	 * Create a button that increments a click counter on each click.
	 * The counter value is displayed in the button label.
	 */
	private Button createCounterButton(final String name, final int counterIndex,
			final java.util.function.Consumer<Button> action) {
		final Button button = Button.create(name + " (0)");
		button.onClick(() -> {
			this.clickCounters[counterIndex]++;
			final Label label = (Label) button.getSubWidget();
			label.setPropertyValue(name + " (" + this.clickCounters[counterIndex] + ")");
			action.accept(button);
		});
		return button;
	}

	private void showPositionPopover(final Widget anchor, final PopoverPosition position) {
		final Sizer content = Sizer.vertical()
				.add(new Label("<b>Popover " + position + "</b>"))
				.add(new Label("Position: " + position));
		Popover.create()
				.content(content)
				.anchor(anchor.getOrigin(), anchor.getSize())
				.position(position)
				.show();
	}

	private void showImagePopover(final Widget anchor) {
		final Sizer content = Sizer.vertical();

		// Image
		final ImageDisplay img = new ImageDisplay();
		img.setPropertySource(new Uri("DATA", "mireA.png"));
		img.setPropertyMinSize(new Dimension2f(new Vector2f(100, 100), Distance.PIXEL));
		img.setPropertyExpand(Vector2b.FALSE);
		content.subWidgetAdd(img);

		// Decorated text
		content.subWidgetAdd(new Label(
				"<b>Image Preview</b><br/>"
						+ "<font color=\"#0066CC\">This is a popover</font> with an image<br/>"
						+ "and <b>bold</b>, <i>italic</i> text."));

		// Icons row
		final Sizer iconRow = Sizer.horizontal();
		iconRow.subWidgetAdd(Icon.create("home").fill(new Color(0x21, 0x96, 0xF3, 0xFF)));
		iconRow.subWidgetAdd(Icon.create("settings").fill(Color.GRAY));
		iconRow.subWidgetAdd(Icon.create("search").fill(new Color(0xF4, 0x43, 0x36, 0xFF)));
		content.subWidgetAdd(iconRow);

		Popover.showAt(anchor.getOrigin(), anchor.getSize(), content, PopoverPosition.AUTO);
	}

	private void showDecoratedTextPopover(final Widget anchor) {
		final Sizer content = Sizer.vertical();
		content.subWidgetAdd(new Label(
				"<font color=\"#CC0000\"><b>Warning!</b></font><br/>"
						+ "This popover contains <b>decorated text</b>:<br/>"
						+ "- <font color=\"#009900\">Green text</font><br/>"
						+ "- <font color=\"#0000CC\">Blue text</font><br/>"
						+ "- <b><i>Bold italic</i></b>"));

		Popover.create()
				.content(content)
				.anchor(anchor.getOrigin(), anchor.getSize())
				.position(PopoverPosition.TOP)
				.color(new Color(0xFF, 0xFD, 0xE0, 0xFF))
				.borderColor(new Color(0xCC, 0xAA, 0x00, 0xFF))
				.show();
	}

	private void showComplexPopover(final Widget anchor) {
		final Sizer content = Sizer.vertical();

		// Header row with icon + title
		final Sizer headerRow = Sizer.horizontal();
		headerRow.subWidgetAdd(Icon.create("account").fill(new Color(0x1A, 0x23, 0x7E, 0xFF)));
		headerRow.subWidgetAdd(new Label("<b>User Profile</b>"));
		content.subWidgetAdd(headerRow);

		// Description
		content.subWidgetAdd(new Label(
				"<font color=\"#666666\">john.doe@example.com</font><br/>"
						+ "Member since <b>2024</b>"));

		// Action buttons
		final Sizer buttonRow = Sizer.horizontal();
		buttonRow.subWidgetAdd(Button.create("Edit"));
		buttonRow.subWidgetAdd(Button.create("Settings"));
		content.subWidgetAdd(buttonRow);

		Popover.showAt(anchor.getOrigin(), anchor.getSize(), content, PopoverPosition.AUTO);
	}

	@Override
	public String getTitle() {
		return "Popover";
	}

	@Override
	public String getDescription() {
		return "Floating overlay anchored to widget";
	}

	@Override
	public String getCategory() {
		return "Overlay";
	}

	@Override
	public boolean isMetaWidget() {
		return true;
	}
}
