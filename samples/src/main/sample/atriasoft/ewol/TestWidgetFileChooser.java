package sample.atriasoft.ewol;

import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.ewol.widget.Windows;
import org.atriasoft.ewol.widget.meta.FileChooser;

public class TestWidgetFileChooser implements TestWidgetInterface {

	@Override
	public Widget getWidget() {
		// Button to open file chooser popup
		// Signal (click) is automatically connected by ModelWidget
		// FileChooser signals are shown in popup, not in central log
		final Button openButton = Button.createLabelButton("Open File Chooser");
		openButton.expand(false, false).fill(false, false);
		openButton.signalClick.connect(() -> openFileChooser(openButton));
		return openButton;
	}

	private void openFileChooser(final Widget sourceWidget) {
		final FileChooser fileChooser = new FileChooser();
		fileChooser.setPropertyLabelTitle("Select a file");
		fileChooser.setPropertyLabelValidate("Open");

		// Get the windows and show popup
		final Windows windows = sourceWidget.getWindows();
		if (windows != null) {
			windows.popUpWidgetPush(fileChooser);
		}
	}

	@Override
	public String getTitle() {
		return "FileChooser";
	}
}
