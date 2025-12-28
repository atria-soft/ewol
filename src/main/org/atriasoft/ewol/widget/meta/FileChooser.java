/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget.meta;

import java.io.File;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.aknot.annotation.AknotSignal;
import org.atriasoft.esignal.Signal;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Uri;
import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.CheckBox;
import org.atriasoft.ewol.widget.Composer;
import org.atriasoft.ewol.widget.Entry;
import org.atriasoft.ewol.widget.ImageDisplay;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.ListFileSystem;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * FileChooser is a simple file selector widget for opening or saving files.
 *
 * <p>Example usage:</p>
 * <pre>{@code
 * // Create the file chooser
 * FileChooser fileChooser = FileChooser.create();
 *
 * // Configure it
 * fileChooser.setPropertyLabelTitle("Open file...");
 * fileChooser.setPropertyLabelValidate("Open");
 * fileChooser.setPropertyPath("/home/user");
 *
 * // Register callbacks
 * fileChooser.signalValidate.connectAuto(this, MyClass::onFileSelected);
 * fileChooser.signalCancel.connectAuto(this, MyClass::onFileCanceled);
 *
 * // Show as popup
 * Windows windows = getWindows();
 * if (windows != null) {
 *     windows.popUpWidgetPush(fileChooser);
 * }
 * }</pre>
 *
 * <p>Callback example:</p>
 * <pre>{@code
 * public static void onFileSelected(MyClass self, String filePath) {
 *     System.out.println("Selected file: " + filePath);
 * }
 *
 * public static void onFileCanceled(MyClass self) {
 *     System.out.println("File selection canceled");
 * }
 * }</pre>
 */
public class FileChooser extends Composer {
	private static final Logger LOGGER = LoggerFactory.getLogger(FileChooser.class);
	
	static void onCallbackButtonCancelPressed(final FileChooser self) {
		// == > Auto remove ...
		self.signalCancel.emit();
		self.autoDestroy();
	}
	
	protected static void onCallbackEntryFileChangeValidate(final FileChooser self, final String value) {
		onCallbackListFileValidate(self, value);
	}
	
	protected static void onCallbackEntryFileChangeValue(final FileChooser self, final String value) {
		self.propertyFile = value;
		// Update the selected file in the list
		if (self.getSubObjectNamed(
				"[" + Long.toString(self.getId()) + "]file-chooser:list-files") instanceof final ListFileSystem tmp) {
			tmp.setPropertyFile(new File(self.propertyFile));
		}
	}
	
	protected static void onCallbackEntryFolderChangeValue(final FileChooser self, final String value) {
		// Change the folder if it exists
		final File folder = new File(value);
		if (folder.exists() && folder.isDirectory()) {
			self.propertyPath = value;
			self.propertyFile = "";
			self.updateCurrentFolder();
		}
	}
	
	protected static void onCallbackHidenFileChangeChangeValue(final FileChooser self, final Boolean value) {
		if (self.getSubObjectNamed(
				"[" + Long.toString(self.getId()) + "]file-chooser:list-files") instanceof final ListFileSystem tmp) {
			tmp.setPropertyShowHidden(value);
		}
		if (self.getSubObjectNamed(
				"[" + Long.toString(self.getId()) + "]file-chooser:list-folder") instanceof final ListFileSystem tmp) {
			tmp.setPropertyShowHidden(value);
		}
	}
	
	protected static void onCallbackHomePressed(final FileChooser self) {
		final String tmpUserFolder = System.getProperty("user.home");
		LOGGER.debug("new PATH: '{}'", tmpUserFolder);
		
		self.propertyPath = tmpUserFolder;
		self.propertyFile = "";
		self.updateCurrentFolder();
	}
	
	protected static void onCallbackListFileSelectChange(final FileChooser self, final String value) {
		self.setPropertyFile(value);
	}
	
	protected static void onCallbackListFileValidate(final FileChooser self, final String value) {
		self.setPropertyFile(value);
		LOGGER.trace("Generate a file opening: '{}'", self.propertyFile);
		self.signalValidate.emit(value);
		self.autoDestroy();
	}
	
	protected static void onCallbackListFolderSelectChange(final FileChooser self, final String value) {
		LOGGER.debug("Path change: '{}' ==> '{}'", self.propertyPath, value);
		self.propertyPath = value;
		self.propertyFile = "";
		self.updateCurrentFolder();
	}
	
	protected static void onCallbackListValidate(final FileChooser self) {
		if (self.propertyFile.isEmpty()) {
			LOGGER.warn("Validate with empty file name");
			return;
		}
		LOGGER.debug("Generate file opening: '{}'", self.propertyFile);
		self.signalValidate.emit(self.propertyFile);
		self.autoDestroy();
	}
	
	@AknotSignal
	@AknotName(value = "cancel")
	@AknotDescription(value = "Cancel button is pressed")
	public SignalEmpty signalCancel = new SignalEmpty(); //!< abort the display of the pop-up or press cancel button
	
	@AknotSignal
	@AknotName(value = "validate")
	@AknotDescription(value = "Validate button is pressed")
	public Signal<String> signalValidate = new Signal<>(); //!< select file(s)
	// properties
	public String propertyPath = System.getProperty("user.home"); //!< Current path to explore
	
	public String propertyFile = ""; //!< Selected file
	public String propertyLabelTitle = "_T{FileChooser}"; //!< Label of the pop-up (can use translation)
	
	public String propertyLabelValidate = "_T{Validate}"; //!< Label of validate button of the pop-up (can use translation)
	public String propertyLabelCancel = "_T{Cancel}"; //!< Label of cancel/close button of the pop-up (can use translation)
	
	public FileChooser() {
		// Load file with replacing the "{ID}" with the local ID of the widget ==> obtain unique ID
		loadFromFile(new Uri("DATA", "ewol-gui-file-chooser.xml", "ewol"));
		// Basic replacement of labels
		onChangePropertyLabelTitle();
		onChangePropertyLabelValidate();
		onChangePropertyLabelCancel();
		
		if (getSubObjectNamed(
				"[" + Long.toString(getId()) + "]file-chooser:show-hiden-file") instanceof final CheckBox tmp) {
			tmp.signalValue.connectAuto(this, FileChooser::onCallbackHidenFileChangeChangeValue);
		}
		if (getSubObjectNamed(
				"[" + Long.toString(getId()) + "]file-chooser:button-validate") instanceof final Button tmp) {
			tmp.signalClick.connectAuto(this, FileChooser::onCallbackListValidate);
		}
		if (getSubObjectNamed(
				"[" + Long.toString(getId()) + "]file-chooser:button-cancel") instanceof final Button tmp) {
			tmp.signalClick.connectAuto(this, FileChooser::onCallbackButtonCancelPressed);
		}
		if (getSubObjectNamed(
				"[" + Long.toString(getId()) + "]file-chooser:list-folder") instanceof final ListFileSystem tmp) {
			tmp.signalFolderValidate.connectAuto(this, FileChooser::onCallbackListFolderSelectChange);
		}
		if (getSubObjectNamed(
				"[" + Long.toString(getId()) + "]file-chooser:list-files") instanceof final ListFileSystem tmp) {
			tmp.signalFileSelect.connectAuto(this, FileChooser::onCallbackListFileSelectChange);
			tmp.signalFileValidate.connectAuto(this, FileChooser::onCallbackListFileValidate);
		}
		if (getSubObjectNamed("[" + Long.toString(getId()) + "]file-chooser:entry-file") instanceof final Entry tmp) {
			tmp.signalModify.connectAuto(this, FileChooser::onCallbackEntryFileChangeValue);
			tmp.signalEnter.connectAuto(this, FileChooser::onCallbackEntryFileChangeValidate);
		}
		if (getSubObjectNamed("[" + Long.toString(getId()) + "]file-chooser:entry-folder") instanceof final Entry tmp) {
			tmp.signalModify.connectAuto(this, FileChooser::onCallbackEntryFolderChangeValue);
		}
		if (getSubObjectNamed(
				"[" + Long.toString(getId()) + "]file-chooser:img-home") instanceof final ImageDisplay tmp) {
			tmp.signalPressed.connectAuto(this, FileChooser::onCallbackHomePressed);
		}
		// set the default Folder properties:
		updateCurrentFolder();
		setPropertyCanFocus(true);
	}
	
	public String getPropertyFile() {
		return this.propertyFile;
	}
	
	public String getPropertyLabelCancel() {
		return this.propertyLabelCancel;
	}
	
	// callback functions:
	public String getPropertyLabelTitle() {
		return this.propertyLabelTitle;
	}
	
	public String getPropertyLabelValidate() {
		return this.propertyLabelValidate;
	}
	
	public String getPropertyPath() {
		return this.propertyPath;
	}
	
	protected void onChangePropertyFile() {
		if (getSubObjectNamed(
				"[" + Long.toString(getId()) + "]file-chooser:entry-file") instanceof final Entry tmp) {
			tmp.setPropertyValue(this.propertyFile);
		}
	}
	
	protected void onChangePropertyLabelCancel() {
		if (getSubObjectNamed("[" + Long.toString(getId()) + "]file-chooser:cancel-label") instanceof final Label tmp) {
			tmp.setPropertyValue(this.propertyLabelCancel);
		}
	}
	
	protected void onChangePropertyLabelTitle() {
		if (getSubObjectNamed("[" + Long.toString(getId()) + "]file-chooser:title-label") instanceof final Label tmp) {
			tmp.setPropertyValue(this.propertyLabelTitle);
		}
	}
	
	protected void onChangePropertyLabelValidate() {
		if (getSubObjectNamed(
				"[" + Long.toString(getId()) + "]file-chooser:validate-label") instanceof final Label tmp) {
			tmp.setPropertyValue(this.propertyLabelValidate);
		}
	}
	
	protected void onChangePropertyPath() {
		this.propertyPath = this.propertyPath + "/";
		updateCurrentFolder();
	}
	
	@Override
	public void onGetFocus() {
		// transfert focus on a specific widget...
		if (getSubObjectNamed("[" + Long.toString(getId()) + "]file-chooser:entry-folder") instanceof final Entry tmp) {
			tmp.keepFocus();
		}
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "file")
	public void setPropertyFile(final String propertyFile) {
		if (this.propertyFile.equals(propertyFile)) {
			return;
		}
		this.propertyFile = propertyFile;
		onChangePropertyFile();
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "label-cancel")
	@AknotDescription(value = "Label for cancel button")
	public void setPropertyLabelCancel(final String propertyLabelCancel) {
		if (this.propertyLabelCancel.equals(propertyLabelCancel)) {
			return;
		}
		this.propertyLabelCancel = propertyLabelCancel;
		onChangePropertyLabelCancel();
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "title")
	@AknotDescription(value = "Titile of the Pop-up")
	public void setPropertyLabelTitle(final String propertyLabelTitle) {
		if (this.propertyLabelTitle.equals(propertyLabelTitle)) {
			return;
		}
		this.propertyLabelTitle = propertyLabelTitle;
		onChangePropertyLabelTitle();
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "label-validate")
	@AknotDescription(value = "Label for validate button")
	public void setPropertyLabelValidate(final String propertyLabelValidate) {
		if (this.propertyLabelValidate.equals(propertyLabelValidate)) {
			return;
		}
		this.propertyLabelValidate = propertyLabelValidate;
		onChangePropertyLabelValidate();
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "path")
	@AknotDescription(value = "Path of the File chooser")
	public void setPropertyPath(final String propertyPath) {
		if (this.propertyPath.equals(propertyPath)) {
			return;
		}
		this.propertyPath = propertyPath;
		onChangePropertyPath();
	}
	
	private void updateCurrentFolder() {
		if (getSubObjectNamed(
				"[" + Long.toString(getId()) + "]file-chooser:list-files") instanceof final ListFileSystem tmp) {
			tmp.setPropertyPath(this.propertyPath);
		}
		if (getSubObjectNamed(
				"[" + Long.toString(getId()) + "]file-chooser:list-folder") instanceof final ListFileSystem tmp) {
			tmp.setPropertyPath(this.propertyPath);
		}
		if (getSubObjectNamed("[" + Long.toString(getId()) + "]file-chooser:entry-folder") instanceof final Entry tmp) {
			tmp.setPropertyValue(this.propertyPath);
		}
		markToRedraw();
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	/**
	 * Create a new FileChooser.
	 * @return a new FileChooser instance
	 */
	public static FileChooser create() {
		return new FileChooser();
	}

	/**
	 * Fluent method to set the dialog title.
	 * @param title the title text
	 * @return this FileChooser for chaining
	 */
	public FileChooser title(final String title) {
		setPropertyLabelTitle(title);
		return this;
	}

	/**
	 * Fluent method to set the validate button label.
	 * @param label the validate button text
	 * @return this FileChooser for chaining
	 */
	public FileChooser validateLabel(final String label) {
		setPropertyLabelValidate(label);
		return this;
	}

	/**
	 * Fluent method to set the cancel button label.
	 * @param label the cancel button text
	 * @return this FileChooser for chaining
	 */
	public FileChooser cancelLabel(final String label) {
		setPropertyLabelCancel(label);
		return this;
	}

	/**
	 * Fluent method to set the initial path.
	 * @param path the initial directory path
	 * @return this FileChooser for chaining
	 */
	public FileChooser path(final String path) {
		setPropertyPath(path);
		return this;
	}

	/**
	 * Fluent method to set the initial file name.
	 * @param file the initial file name
	 * @return this FileChooser for chaining
	 */
	public FileChooser file(final String file) {
		setPropertyFile(file);
		return this;
	}

	/**
	 * Get the full path of the selected file (path + file).
	 * @return the complete file path
	 */
	public String getFullPath() {
		if (this.propertyFile.isEmpty()) {
			return this.propertyPath;
		}
		final String separator = this.propertyPath.endsWith("/") ? "" : "/";
		return this.propertyPath + separator + this.propertyFile;
	}
}
