package sample.atriasoft.ewol;

import org.atriasoft.ewol.widget.ListFileSystem;
import org.atriasoft.ewol.widget.Widget;

public class TestWidgetListFileSystem implements TestWidgetInterface {

	@Override
	public Widget getWidget() {
		// ListFileSystem - showing home directory
		// Signals (file-select, file-validate, folder-select, folder-validate)
		// are automatically connected by ModelWidget and displayed in the central log
		final String homePath = System.getProperty("user.home");
		final ListFileSystem listFileSystem = ListFileSystem.create(homePath)
				.showFiles(true)
				.showFolders(true)
				.showHidden(false);

		listFileSystem.expand(true, true).fill(true, true);

		return listFileSystem;
	}

	@Override
	public String getTitle() {
		return "ListFileSystem";
	}
}
