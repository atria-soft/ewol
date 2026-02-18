package sample.atriasoft.ewol;

import org.atriasoft.ewol.widget.TreeFileSystem;
import org.atriasoft.ewol.widget.Widget;

public class TestWidgetTreeFileSystem implements TestWidgetInterface {

	@Override
	public Widget getWidget() {
		final String homePath = System.getProperty("user.home");
		final TreeFileSystem treeFs = TreeFileSystem.create(homePath)
				.showFiles(true)
				.showHidden(false);

		treeFs.expand(true, true).fill(true, true);

		return treeFs;
	}

	@Override
	public String getTitle() {
		return "TreeFileSystem";
	}

	@Override
	public String getDescription() {
		return "File system browser as tree view";
	}

	@Override
	public String getCategory() {
		return "Data";
	}
}
