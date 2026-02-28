package sample.atriasoft.ewol;

import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.CheckBox;
import org.atriasoft.ewol.widget.Dialog;
import org.atriasoft.ewol.widget.Entry;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Slider;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.ewol.widget.Windows;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Test widget for the Dialog component.
 *
 * Demonstrates how to use Dialog with title, content, footer buttons,
 * close behavior, and custom sizing.
 *
 * IMPORTANT: Do NOT use lambdas for signal callbacks!
 * Lambdas are garbage collected because signals use WeakReferences.
 * Always use connectAuto() with static methods instead.
 */
public class TestWidgetDialog implements TestWidgetInterface {
	private static final Logger LOGGER = LoggerFactory.getLogger(TestWidgetDialog.class);

	private Button openButton;
	private Dialog currentDialog;
	private Entry nameEntry;
	private Slider volumeSlider;
	private CheckBox enableCheck;

	// --- Static callbacks for connectAuto (safe with WeakReferences) ---

	private static void onOpenButtonClick(final TestWidgetDialog self) {
		LOGGER.info("Opening Dialog popup...");

		self.nameEntry = Entry.create().value("").placeholder("Enter your name...");
		self.nameEntry.expand(true, false).fill(true, false);

		final Label volumeLabel = Label.create("50");
		self.volumeSlider = Slider.create().range(0, 100).value(50).step(1);
		self.volumeSlider.expand(true, false).fill(true, false);
		self.volumeSlider.signalValue.connectAuto(self, TestWidgetDialog::onVolumeChanged);

		self.enableCheck = CheckBox.create("Enable feature").checked(false);
		self.enableCheck.expand(true, false).fill(true, false);

		// Build content
		final Sizer content = Sizer.vertical().expand(true, true).fill(true, true);
		content.subWidgetAdd(Label.create("Name:").expand(true, false).fill(true, false));
		content.subWidgetAdd(self.nameEntry);
		content.subWidgetAdd(Label.create("Volume:").expand(true, false).fill(true, false));
		final Sizer sliderRow = Sizer.horizontal().expand(true, false).fill(true, false);
		sliderRow.subWidgetAdd(self.volumeSlider);
		sliderRow.subWidgetAdd(volumeLabel);
		content.subWidgetAdd(sliderRow);
		content.subWidgetAdd(self.enableCheck);

		// Build dialog
		self.currentDialog = Dialog.create().title("Sample Dialog");
		self.currentDialog.content(content);

		// Footer buttons
		final Button btnOk = Button.create("OK");
		btnOk.signalClick.connectAuto(self, TestWidgetDialog::onOkClicked);
		self.currentDialog.footer(btnOk);

		final Button btnCancel = Button.create("Cancel");
		btnCancel.signalClick.connectAuto(self, TestWidgetDialog::onCancelClicked);
		self.currentDialog.footer(btnCancel);

		// Show as popup
		final Windows windows = self.openButton.getWindows();
		if (windows != null) {
			windows.popUpWidgetPush(self.currentDialog);
			LOGGER.info("Dialog pushed to popup stack");
		} else {
			LOGGER.error("Cannot open Dialog: windows is null");
		}
	}

	private static void onVolumeChanged(final TestWidgetDialog self, final Float value) {
		LOGGER.info("Volume changed: {}", value);
	}

	private static void onOkClicked(final TestWidgetDialog self) {
		LOGGER.info("Dialog OK clicked - name={}, volume={}, enabled={}",
				self.nameEntry.getPropertyValue(),
				self.volumeSlider.getPropertyValue(),
				self.enableCheck.isChecked());
		self.currentDialog.destroy();
		self.currentDialog = null;
	}

	private static void onCancelClicked(final TestWidgetDialog self) {
		LOGGER.info("Dialog Cancel clicked");
		self.currentDialog.destroy();
		self.currentDialog = null;
	}

	@Override
	public Widget getWidget() {
		this.openButton = Button.createLabelButton("Open Dialog");
		this.openButton.expand(false, false).fill(false, false);
		this.openButton.signalClick.connectAuto(this, TestWidgetDialog::onOpenButtonClick);
		return this.openButton;
	}

	@Override
	public String getTitle() {
		return "Dialog";
	}

	@Override
	public String getDescription() {
		return "Structured popup with title, content and footer";
	}

	@Override
	public String getCategory() {
		return "Dialog";
	}
}
