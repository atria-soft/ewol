/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import java.io.File;
import java.io.IOException;
import java.nio.file.DirectoryStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.aknot.annotation.AknotSignal;
import org.atriasoft.esignal.Signal;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.etk.math.Vector3i;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.resource.ResourceColorFile;
import org.atriasoft.ewol.widget.model.ListRole;
import org.atriasoft.gale.key.KeyStatus;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Generic display folder class. This widget display the content of a single folder :
 */
public class ListFileSystem extends WidgetList {
	private static final Logger LOGGER = LoggerFactory.getLogger(ListFileSystem.class);
	@AknotSignal
	@AknotName(value = "file-select")
	@AknotDescription(value = "A file has been selected in the List")
	public Signal<String> signalFileSelect = new Signal<>(); //!< @event "file-select" Generated when a file is selected.

	@AknotSignal
	@AknotName(value = "file-validate")
	@AknotDescription(value = "A file has been validated on the list (double clicked or return pressed)")
	public Signal<String> signalFileValidate = new Signal<>(); //!< @event "file-validate" Generate when the user validate (return) or double click on the element

	@AknotSignal
	@AknotName(value = "folder-select")
	@AknotDescription(value = "A folder has been selected in the List")
	public Signal<String> signalFolderSelect = new Signal<>();

	@AknotSignal
	@AknotName(value = "folder-validate")
	@AknotDescription(value = "A folder has been validated on the list (double clicked or return pressed)")
	public Signal<String> signalFolderValidate = new Signal<>();
	protected String propertyPath = "/"; //!< Current folder that display point on.
	protected File propertyFile = null; //!< current selected file
	protected boolean propertyShowFile = true; //!< Show files elements
	protected boolean propertyShowFolder = true; //!< Display the folders elements
	protected boolean propertyShowHidden = true; //!< Display hidden elements
	protected String propertyFilter = "^.*$"; //!< Regular expression to filter the view (for temporary file:".*(~|.bck|.pyc)\e")

	protected ResourceColorFile colorProperty; //!< theme color property.
	protected int colorIdText = -1; //!< Color of the text.
	protected int colorIdBackground1 = -1; //!< Color of the Background.
	protected int colorIdBackground2 = -1; //!< Color of the Background 2.
	protected int colorIdBackgroundSelected = -1; //!< Color of line selected.
	protected List<File> list = new ArrayList<>(); //!< List of all element in the File. (they are filtered)
	protected int selectedLine; //!< Current Line ID that is selected

	public ListFileSystem() {

		this.colorProperty = new ResourceColorFile(new Uri("THEME", "/color/ListFileSystem.json", "ewol"));
		if (this.colorProperty != null) {
			this.colorIdText = this.colorProperty.request("text");
			this.colorIdBackground1 = this.colorProperty.request("background1");
			this.colorIdBackground2 = this.colorProperty.request("background2");
			this.colorIdBackgroundSelected = this.colorProperty.request("selected");
		}
		setMouseLimit(2);
	}

	/**
	 * Clean the list of element.
	 */
	protected void clearList() {
		this.list.clear();
	}

	@Override
	protected Color getBasicBG() {
		return this.colorProperty.get(this.colorIdBackground1);
	}

	@Override
	protected Object getData(final ListRole role, final Vector2i pos) {
		switch (role) {
			case Text: {
				int offset = 0;
				if (this.propertyShowFolder) {
					if (this.propertyPath.equals("/")) {
						offset = 1;
					} else {
						offset = 2;
					}
					if (pos.y() == 0) {
						return ".";
					} else if (pos.y() == 1 && !this.propertyPath.equals("/")) {
						return "..";
					}
				}
				if (pos.y() - offset >= 0 && pos.y() - offset < this.list.size()) {
					LOGGER.trace("get filename for : {}:'{}'", this.list.get(pos.y() - offset),
							this.list.get(pos.y() - offset).getName());
					return this.list.get(pos.y() - offset).getName();
				}
			}
				return "+<ERROR>>>";
			case FgColor:
				return this.colorProperty.get(this.colorIdText);
			case BgColor:
				if (this.selectedLine == pos.y()) {
					return this.colorProperty.get(this.colorIdBackgroundSelected);
				}
				if (pos.y() % 2 == 0) {
					return this.colorProperty.get(this.colorIdBackground1);
				}
				return this.colorProperty.get(this.colorIdBackground2);
			default:
				break;
		}
		return null;
	}

	@Override
	protected Vector2i getMatrixSize() {
		int offset = 0;
		if (this.propertyShowFolder) {
			if (this.propertyPath.equals("/")) {
				offset = 1;
			} else {
				offset = 2;
			}
		}
		return new Vector2i(1, this.list.size() + offset);
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "select")
	@AknotDescription(value = "selection af a specific file")
	public File getPropertyFile() {
		return this.propertyFile;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "filter")
	@AknotDescription(value = "regex to filter files ...")
	public String getPropertyFilter() {
		return this.propertyFilter;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "Path")
	@AknotDescription(value = "Path to display")
	public String getPropertyPath() {
		return this.propertyPath;
	}

	/**
	 * Get the current selected file/folder/... in the list
	 * @return the String of the element selected.
	 */
	public File getSelect() {
		if (this.selectedLine >= 0) {
			return this.list.get(this.selectedLine);
		}
		return null;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "show-file")
	@AknotDescription(value = "Display files")
	public boolean isPropertyShowFile() {
		return this.propertyShowFile;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "show-folder")
	@AknotDescription(value = "display folders")
	public boolean isPropertyShowFolder() {
		return this.propertyShowFolder;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "show-hidden")
	@AknotDescription(value = "Show the hidden element (file, folder, ...)")
	public boolean isPropertyShowHidden() {
		return this.propertyShowHidden;
	}

	public List<File> listSelectedFiles(
			final String dir,
			final boolean showFiles,
			final boolean showFolder,
			final boolean showHidden) throws IOException {
		final List<File> fileList = new ArrayList<>();
		try (DirectoryStream<Path> stream = Files.newDirectoryStream(Paths.get(dir))) {
			for (final Path path : stream) {
				if (Files.isHidden(path) && !showHidden) {
					continue;
				}
				if (Files.isDirectory(path) && showFolder) {
					LOGGER.error("Add Directory '{}'", path);
					fileList.add(new File(path.toString()));
				}
				if (!Files.isDirectory(path) && showFiles) {
					LOGGER.error("Add File      '{}'", path);
					fileList.add(new File(path.toString()));
				}
			}
		}
		return fileList;
	}

	@Override
	protected boolean onItemEvent(final EventInput event, final Vector3i pos, final Vector3f mousePosition) {
		int offset = 0;
		if (this.propertyShowFolder) {
			if (this.propertyPath.equals("/")) {
				offset = 1;
			} else {
				offset = 2;
			}
		}
		if (event.status() == KeyStatus.pressSingle || event.status() == KeyStatus.pressDouble) {
			LOGGER.trace("Event on List : IdInput=" + event.inputId() + " _pos=" + pos);
			if (1 == event.inputId()) {
				if (pos.y() > this.list.size() + offset) {
					this.selectedLine = -1;
				} else {
					this.selectedLine = pos.y();
				}
				if (this.propertyShowFolder && this.selectedLine == 0) {
					// "." folder
					if (event.status() == KeyStatus.pressSingle) {
						this.signalFolderSelect.emit(this.propertyPath);
					} else {
						this.signalFolderValidate.emit(this.propertyPath);
					}
				} else if (this.propertyShowFolder && this.selectedLine == 1) {
					// ".." folder
					if (event.status() == KeyStatus.pressSingle) {
						this.signalFolderSelect.emit(new File(this.propertyPath).getParent());
					} else {
						this.signalFolderValidate.emit(new File(this.propertyPath).getParent());
					}
				} else if (this.selectedLine - offset >= 0 && this.selectedLine - offset < this.list.size()) {
					// generate event extern:
					if (this.list.get(this.selectedLine - offset).isDirectory()) {
						if (event.status() == KeyStatus.pressSingle) {
							this.signalFolderSelect.emit(this.list.get(this.selectedLine - offset).getPath());
						} else {
							this.signalFolderValidate.emit(this.list.get(this.selectedLine - offset).getPath());
						}
					} else if (event.status() == KeyStatus.pressSingle) {
						this.signalFileSelect.emit(this.list.get(this.selectedLine - offset).getPath());
					} else {
						this.signalFileValidate.emit(this.list.get(this.selectedLine - offset).getPath());
					}
				}
				// need to regenerate the display of the list :
				markToRedraw();
				return true;
			}
		}
		return false;
	}

	/**
	 * Regenerate the content of the view. this is actually not automation on the system update.
	 */
	protected void regenerateView() {
		clearList();
		this.selectedLine = -1;
		this.list.clear();
		this.originScrooled = new Vector2f(0, 0);
		final int flags = 0;
		try {
			this.list = listSelectedFiles(this.propertyPath, this.propertyShowFile, this.propertyShowFolder,
					this.propertyShowHidden);
		} catch (final IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			this.list = new ArrayList<>();
		}
		Collections.sort(this.list, Comparator.comparing(File::getName));//.reversed());
		// request a redraw ...
		markToRedraw();
	}

	public void setPropertyFile(final File propertyFile) {
		if (this.propertyFile.equals(propertyFile)) {
			return;
		}
		this.propertyFile = propertyFile;
		regenerateView();
	}

	public void setPropertyFilter(final String propertyFilter) {
		if (!this.propertyFilter.equals(propertyFilter)) {
			return;
		}
		this.propertyFilter = propertyFilter;
		regenerateView();
	}

	public void setPropertyPath(final String propertyPath) {
		if (this.propertyPath.equals(propertyPath)) {
			return;
		}
		this.propertyPath = propertyPath;
		regenerateView();
	}

	public void setPropertyShowFile(final boolean propertyShowFile) {
		if (this.propertyShowFile == propertyShowFile) {
			return;
		}
		this.propertyShowFile = propertyShowFile;
		regenerateView();
	}

	public void setPropertyShowFolder(final boolean propertyShowFolder) {
		if (this.propertyShowFolder == propertyShowFolder) {
			return;
		}
		this.propertyShowFolder = propertyShowFolder;
		regenerateView();
	}

	public void setPropertyShowHidden(final boolean propertyShowHidden) {
		if (this.propertyShowHidden == propertyShowHidden) {
			return;
		}
		this.propertyShowHidden = propertyShowHidden;
		regenerateView();
	}

	/**
	 * Select a specific file in the File
	 * @param data File to selected.
	 */
	public void setSelect(final File data) {
		// remove selected line
		this.selectedLine = -1;
		// search the coresponding file :
		for (int iii = 0; iii < this.list.size(); ++iii) {
			if (this.list.get(iii).equals(data)) {
				// we find the line :
				this.selectedLine = iii;
				break;
			}
		}
		markToRedraw();
	}
}
