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
 *  File Chooser is a simple selector of file for opening, saving, and what you want ...
 *
 *  As all other pop-up methode ( wost case we can have) the creating is simple , but event back is not all the time simple:
 *
 *  Fist global static declaration and inclusion:
 *  [code style=c++]
 *  package org.atriasoft.ewol.widget.meta.FileChooser;
 *  [/code]
 *
 *  The first step is to create the file chooser pop-up : (never in the ructor!!!)
 *  [code style=c++]
 *  ewol::widget::FileChooser tmpWidget = ewol::widget::FileChooser::create();
 *  if (tmpWidget == null) {
 *  	APPL_ERROR("Can not open File chooser !!! ");
 *  	return -1;
 *  }
 *  // register on the Validate event:
 *  tmpWidget.signalValidate.connect(sharedFromThis(), ****::onCallbackOpenFile);
 *  // no need of this event watching ...
 *  tmpWidget.signalCancel.connect(sharedFromThis(), ****::onCallbackClosePopUp);
 *  // set the title:
 *   tmpWidget.propertyLabelTitle.set("Open files ...");
 *  // Set the validate Label:
 *  tmpWidget.propertyLabelValidate.set("Open");
 *  // simply set a folder (by default this is the home folder)
 *  //tmpWidget.propertyPath.set("/home/me");
 *  // add the widget as windows pop-up ...
 *  ewol::widget::Windows tmpWindows = getWindows();
 *  if (tmpWindows == null) {
 *  	APPL_ERROR("Can not get the current windows !!! ");
 *  	return -1;
 *  }
 *  tmpWindows.popUpWidgetPush(tmpWidget);
 *  [/code]
 *
 *  Now we just need to wait the the open event message.
 *
 *  [code style=c++]
 *  void ****::onCallbackOpenFile( String _value) {
 *  	APPL_INFO("Request open file : '" + _value + "'");
 *  }
 *  void ****::onCallbackClosePopUp() {
 *  	APPL_INFO("The File chooser has been closed");
 *  }
 *  [/code]
 *  This is the best example of a Meta-widget.
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
		// == > change the file name.get(.get(
		self.propertyFile = value;
		// update the selected file in the list :
		if (self.getSubObjectNamed(
				"[" + Long.toString(self.getId()) + "]file-chooser:list-files") instanceof final ListFileSystem tmp) {
			tmp.setPropertyFile(new File(self.propertyFile));
		}
	}

	protected static void onCallbackEntryFolderChangeValue(final FileChooser self, final String value) {
		// == > change the folder name
		// TODO : change the folder, if it exit ...
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
		/*
		String tmpFileCompleatName = this.folder;
		tmpFileCompleatName += this.file;
		// TODO : generateEventId(_msg.getMessage(), tmpFileCompleatName);
		*/
	}

	protected static void onCallbackListFileValidate(final FileChooser self, final String value) {
		// select the file  == > generate a validate
		self.setPropertyFile(value);
		LOGGER.trace(" generate a file opening : '{}'", self.propertyFile);
		self.signalValidate.emit(value);
		self.autoDestroy();
	}

	protected static void onCallbackListFolderSelectChange(final FileChooser self, final String value) {
		// == > this is an internal event ...
		LOGGER.debug(" old PATH: '{}' ==> '{}'", self.propertyPath, value);
		self.propertyPath = value;
		LOGGER.debug("new PATH: '{}'", self.propertyPath);
		self.propertyFile = "";
		self.updateCurrentFolder();
	}

	protected static void onCallbackListValidate(final FileChooser self) {
		if (self.propertyFile.isEmpty()) {
			LOGGER.warn(" Validate : '{}' ==> error No name ...", self.propertyFile);
			return;
		}
		LOGGER.debug(" generate a file opening : '{}'", self.propertyFile);
		self.signalValidate.emit(self.propertyFile);
		self.autoDestroy();
	}

	@AknotSignal
	@AknotName(value = "cancel")
	@AknotDescription(value = "Cancel button is pressed")
	public SignalEmpty signalCancel; //!< abort the display of the pop-up or press cancel button

	@AknotSignal
	@AknotName(value = "validate")
	@AknotDescription(value = "Validate button is pressed")
	public Signal<String> signalValidate; //!< select file(s)
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
				"[" + Long.toString(getId()) + "]file-chooser:entry-file") instanceof final ListFileSystem tmp) {
			tmp.setPropertyFile(new File(this.propertyFile));
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
}
