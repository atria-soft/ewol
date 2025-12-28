package sample.atriasoft.ewol;

import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.ewol.widget.Windows;
import org.atriasoft.ewol.widget.meta.FileChooser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Test widget for FileChooser popup.
 *
 * IMPORTANT: Do NOT use lambdas for signal callbacks!
 * Lambdas are garbage collected because signals use WeakReferences.
 * Always use connectAuto() with static methods instead.
 */
public class TestWidgetFileChooser implements TestWidgetInterface {
	private static final Logger LOGGER = LoggerFactory.getLogger(TestWidgetFileChooser.class);

	private Button openButton;

	/**
	 * Static callback for button click - opens the FileChooser popup.
	 * Must be static for connectAuto() to work properly with WeakReferences.
	 */
	public static void onOpenButtonClick(final TestWidgetFileChooser self) {
		LOGGER.info("Opening FileChooser popup...");
		final FileChooser fileChooser = new FileChooser();
		fileChooser.setPropertyLabelTitle("Select a file");
		fileChooser.setPropertyLabelValidate("Open");

		// Get the windows and show popup
		final Windows windows = self.openButton.getWindows();
		LOGGER.info("Windows: {}", windows);
		if (windows != null) {
			windows.popUpWidgetPush(fileChooser);
			LOGGER.info("FileChooser pushed to popup stack");
		} else {
			LOGGER.error("Cannot open FileChooser: windows is null");
		}
	}

	@Override
	public Widget getWidget() {
		// Button to open file chooser popup
		// Signal (click) is automatically connected by ModelWidget
		// FileChooser signals are shown in popup, not in central log
		this.openButton = Button.createLabelButton("Open File Chooser");
		this.openButton.expand(false, false).fill(false, false);
		// Use connectAuto with static method - NOT lambdas (they get GC'd)
		this.openButton.signalClick.connectAuto(this, TestWidgetFileChooser::onOpenButtonClick);
		return this.openButton;
	}

	@Override
	public String getTitle() {
		return "FileChooser";
	}
}
