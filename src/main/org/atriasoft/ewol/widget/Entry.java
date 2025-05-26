package org.atriasoft.ewol.widget;

import java.util.Arrays;
import java.util.regex.Pattern;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.aknot.annotation.AknotSignal;
import org.atriasoft.esignal.Connection;
import org.atriasoft.esignal.Signal;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.Padding;
import org.atriasoft.ewol.compositing.CompositingText;
import org.atriasoft.ewol.compositing.GuiShape;
import org.atriasoft.ewol.compositing.GuiShapeMode;
import org.atriasoft.ewol.event.EventEntry;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.event.EventTime;
import org.atriasoft.ewol.object.EwolObject;
import org.atriasoft.gale.context.ClipBoard;
import org.atriasoft.gale.context.ClipboardList;
import org.atriasoft.gale.key.KeyKeyboard;
import org.atriasoft.gale.key.KeyStatus;
import org.atriasoft.gale.key.KeyType;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * @ingroup ewolWidgetGroup
 * Entry box display :
 *
 * ~~~~~~~~~~~~~~~~~~~~~~
 * 	----------------------------------------------
 * 	| Editable Text                              |
 * 	----------------------------------------------
 * ~~~~~~~~~~~~~~~~~~~~~~
 */
public class Entry extends Widget {
	private static final Logger LOGGER = LoggerFactory.getLogger(Entry.class);

	/**
	 * Periodic call to update grapgic display
	 * @param _event Time generic event
	 */
	protected static void periodicCall(final Entry self, final EventTime event) {
		LOGGER.trace("Periodic call on Entry(" + event + ")");
		if (!self.shape.periodicCall(event)) {
			self.periodicConnectionHanble.close();
		}
		self.markToRedraw();
	}
	
	/// color property of the text foreground
	private int colorIdTextFg;
	/// Cursor must be display only when the widget has the focus
	private boolean displayCursor = false;
	/// Cursor position in number of Char
	private int displayCursorPos = 0;
	/// Selection position end (can be before or after cursor and == this.displayCursorPos chan no selection availlable
	private int displayCursorPosSelection = 0;
	/// offset in pixel of the display of the UString
	private int displayStartPosition = 0;
	/// offset in pixel of the display of the UString
	private int displayCursorPositionPixel = 0;
	/// text display this.text
	private final CompositingText text = new CompositingText();
	/// text position can have change
	private boolean needUpdateTextPos = true;
	/// Periodic call handle to remove it when needed
	protected Connection periodicConnectionHanble = new Connection();
	private Uri propertyConfig = new Uri("THEME", "shape/Entry.json", "ewol");
	private int propertyMaxCharacter = Integer.MAX_VALUE; //!< number max of Character in the list
	private boolean propertyPassword = false; //!< Disable display of the content of the entry
	
	/// regular expression value
	private String propertyRegex = ".*";
	
	/// Text to display when nothing in in the entry (decorated text...)
	private String propertyTextWhenNothing = null;
	
	private String propertyValue = "Test Text..."; //!< string that must be displayed
	private Pattern regex = null; //!< regular expression to check content
	private GuiShape shape;
	//.create()
	@AknotSignal
	@AknotName(value = "click")
	@AknotDescription("the user Click on the Entry box")
	public SignalEmpty signalClick = new SignalEmpty(); //!< bang on click the entry box
	@AknotSignal
	@AknotName(value = "enter")
	@AknotDescription("The cursor enter inside the button")
	public Signal<String> signalEnter = new Signal<>(); //!< Enter key is pressed
	
	@AknotSignal
	@AknotName(value = "modify")
	@AknotDescription("Entry box value change")
	public Signal<String> signalModify = new Signal<>(); //!< data change
	// element over:
	Vector3f overPositionStart = Vector3f.ZERO;
	
	Vector3f overPositionStop = Vector3f.ZERO;
	
	/**
	 * Constructor
	 * @param _newData The USting that might be set in the Entry box (no event generation!!)
	 */
	public Entry() {
		this.propertyCanFocus = true;
		onChangePropertyShaper();
		
		this.regex = Pattern.compile(this.propertyRegex);
		if (this.regex == null) {
			LOGGER.error("can not parse regex for : " + this.propertyRegex);
		}
		markToRedraw();
		shortCutAdd("ctrl+w", "clean");
		shortCutAdd("ctrl+x", "cut");
		shortCutAdd("ctrl+c", "copy");
		shortCutAdd("ctrl+v", "paste");
		shortCutAdd("ctrl+a", "select:all");
		shortCutAdd("ctrl+shift+a", "select:none");
		this.shape = new GuiShape(this.propertyConfig);
		//TODO this.signalShortcut.connect(this, Entry::onCallbackShortCut);
	}
	
	@Override
	public void calculateMinMaxSize() {
		// call main class
		super.calculateMinMaxSize();
		// get generic padding
		Padding padding = Padding.ZERO;
		if (this.shape != null) {
			padding = this.shape.getPadding();
		}
		final int minHeight = (int) this.text.getHeight();//calculateSize('A').y();
		
		Vector3f minimumSizeBase = new Vector3f(20, minHeight, 10);
		// add padding :
		minimumSizeBase = minimumSizeBase.add(padding.x(), padding.y(), padding.z());
		this.minSize = Vector3f.max(this.minSize, minimumSizeBase);
		// verify the min max of the min size ...
		checkMinSize();
		//LOGGER.trace("min size = " + this.minSize);
	}
	
	protected void changeStatusIn(final GuiShapeMode newStatusId) {
		if (this.shape.changeStatusIn(newStatusId)) {
			if (!this.periodicConnectionHanble.isConnected()) {
				//LOGGER.trace("REQUEST: connection on operiodic call");
				this.periodicConnectionHanble = EwolObject.getObjectManager().periodicCall.connect(this,
						Entry::periodicCall);
			}
			markToRedraw();
		}
	}
	
	/**
	 * Copy the selected data on the specify clipboard
	 * @param clipboardID Selected clipboard
	 */
	public void copySelectionToClipBoard(final ClipboardList clipboardID) {
		if (this.displayCursorPosSelection == this.displayCursorPos) {
			// nothing to cut ...
			return;
		}
		int pos1 = this.displayCursorPosSelection;
		int pos2 = this.displayCursorPos;
		if (this.displayCursorPosSelection > this.displayCursorPos) {
			pos2 = this.displayCursorPosSelection;
			pos1 = this.displayCursorPos;
		}
		// Copy
		final String tmpData = this.propertyValue.substring(pos1, pos2);
		ClipBoard.set(clipboardID, tmpData);
	}
	
	public Uri getPropertyConfig() {
		return this.propertyConfig;
	}
	
	public int getPropertyMaxCharacter() {
		return this.propertyMaxCharacter;
	}
	
	public String getPropertyRegex() {
		return this.propertyRegex;
	}
	
	public String getPropertyTextWhenNothing() {
		return this.propertyTextWhenNothing;
	}
	
	public String getPropertyValue() {
		return this.propertyValue;
	}
	
	public boolean isPropertyPassword() {
		return this.propertyPassword;
	}
	
	/**
	 * informe the system thet the text change and the start position change
	 */
	protected void markToUpdateTextPosition() {
		this.needUpdateTextPos = true;
	}
	
	private void onCallbackCopy() {
		copySelectionToClipBoard(ClipboardList.CLIPBOARD_STD);
	}
	
	private void onCallbackCut() {
		copySelectionToClipBoard(ClipboardList.CLIPBOARD_STD);
		removeSelected();
		this.signalModify.emit(this.propertyValue);
	}
	
	private void onCallbackEntryClean() {
		this.propertyValue = "";
		this.displayStartPosition = 0;
		this.displayCursorPos = 0;
		this.displayCursorPosSelection = this.displayCursorPos;
		markToRedraw();
	}
	
	private void onCallbackPaste() {
		ClipBoard.request(ClipboardList.CLIPBOARD_STD);
	}
	
	private void onCallbackSelect(final boolean all) {
		if (all) {
			this.displayCursorPosSelection = 0;
			this.displayCursorPos = this.propertyValue.length();
		} else {
			this.displayCursorPosSelection = this.displayCursorPos;
		}
		markToRedraw();
	}
	
	private void onCallbackShortCut(final String value) {
		if (value.equals("clean")) {
			onCallbackEntryClean();
		} else if (value.equals("cut")) {
			onCallbackCut();
		} else if (value.equals("copy")) {
			onCallbackCopy();
		} else if (value.equals("paste")) {
			LOGGER.warn("Request past ...");
			onCallbackPaste();
		} else if (value.equals("select:all")) {
			onCallbackSelect(true);
		} else if (value.equals("select:none")) {
			onCallbackSelect(false);
		} else {
			LOGGER.warn("Unknow event from ShortCut : " + value);
		}
	}
	
	protected void onChangePropertyMaxCharacter() {
		// TODO : check number of char in the data
	}
	
	protected void onChangePropertyPassword() {
		markToRedraw();
	}
	
	protected void onChangePropertyRegex() {
		this.regex = Pattern.compile(this.propertyRegex);
		if (this.regex != null) {
			LOGGER.error("can not parse regex for : " + this.propertyRegex);
		}
		markToRedraw();
	}
	
	protected void onChangePropertyShaper() {
		if (this.shape == null) {
			this.shape = new GuiShape(this.propertyConfig);
		} else {
			this.shape.setSource(this.propertyConfig);
		}
		// this.colorIdTextFg = this.shape.requestColor("text-foreground");
		// this.colorIdTextBg = this.shape.requestColor("text-background");
		// this.colorIdCursor = this.shape.requestColor("text-cursor");
		// this.colorIdSelection = this.shape.requestColor("text-selection");
	}
	
	protected void onChangePropertyTextWhenNothing() {
		markToRedraw();
	}
	
	protected void onChangePropertyValue() {
		String newData = this.propertyValue;
		if ((long) newData.length() > this.propertyMaxCharacter) {
			newData = newData.substring(0, this.propertyMaxCharacter);
			LOGGER.debug("Limit entry set of data... " + newData);
		}
		// set the value with the check of the RegExp ...
		setInternalValue(newData);
		if (newData == this.propertyValue) {
			this.displayCursorPos = this.propertyValue.length();
			this.displayCursorPosSelection = this.displayCursorPos;
			LOGGER.trace("Set : '" + newData + "'");
		}
		markToRedraw();
	}
	
	@Override
	protected void onDraw() {
		if (this.shape != null) {
			this.shape.draw(true);
		}
		this.text.draw();
	}
	
	@Override
	public void onEventClipboard(final ClipboardList clipboardID) {
		// remove curent selected data ...
		removeSelected();
		// get current selection / Copy :
		final String tmpData = ClipBoard.get(clipboardID);
		// add it on the current display:
		if (tmpData.length() != 0) {
			final StringBuilder newData = new StringBuilder(this.propertyValue);
			newData.insert(this.displayCursorPos, tmpData.charAt(0));
			setInternalValue(newData.toString());
			if (this.propertyValue.equals(newData.toString())) {
				if (this.propertyValue.length() == tmpData.length()) {
					this.displayCursorPos = tmpData.length();
				} else {
					this.displayCursorPos += tmpData.length();
				}
				this.displayCursorPosSelection = this.displayCursorPos;
				markToRedraw();
			}
		}
		this.signalModify.emit(this.propertyValue);
	}
	
	@Override
	public boolean onEventEntry(final EventEntry event) {
		LOGGER.trace("Event on Entry ... " + event);
		if (event.type() == KeyKeyboard.CHARACTER) {
			if (event.status() == KeyStatus.down) {
				// remove current selected data ...
				removeSelected();
				if (event.getChar() == '\n' || event.getChar() == '\r') {
					this.signalEnter.emit(this.propertyValue);
					return true;
				}
				if (event.getChar() == 0x7F) {
					// SUPPR :
					if (this.propertyValue.length() > 0 && this.displayCursorPos < (long) this.propertyValue.length()) {
						final StringBuilder newData = new StringBuilder(this.propertyValue);
						newData.deleteCharAt(this.displayCursorPos);
						this.propertyValue = newData.toString();
						this.displayCursorPos = Math.max(this.displayCursorPos, 0);
						this.displayCursorPosSelection = this.displayCursorPos;
					}
				} else if (event.getChar() == 0x08) {
					// DEL :
					if (this.propertyValue.length() > 0 && this.displayCursorPos != 0) {
						final StringBuilder newData = new StringBuilder(this.propertyValue);
						newData.deleteCharAt(this.displayCursorPos - 1);
						this.propertyValue = newData.toString();
						this.displayCursorPos--;
						this.displayCursorPos = Math.max(this.displayCursorPos, 0);
						this.displayCursorPosSelection = this.displayCursorPos;
					}
				} else if (event.getChar() >= 20) {
					LOGGER.error("get data: '" + event.getChar() + "' = '" + event.getChar() + "'");
					if ((long) this.propertyValue.length() > this.propertyMaxCharacter) {
						LOGGER.info("Reject data for entry : '" + event.getChar() + "'");
					} else {
						final StringBuilder newData = new StringBuilder(this.propertyValue);
						newData.insert(this.displayCursorPos, event.getChar());
						final String newDataGenerated = newData.toString();
						setInternalValue(newDataGenerated);
						if (this.propertyValue.equals(newDataGenerated)) {
							this.displayCursorPos += 1;//inputData.length();
							this.displayCursorPosSelection = this.displayCursorPos;
						}
					}
				}
				this.signalModify.emit(this.propertyValue);
				markToRedraw();
				return true;
			}
			return false;
		}
		if (event.status() == KeyStatus.down) {
			switch (event.type()) {
				case LEFT:
					this.displayCursorPos--;
					break;
				case RIGHT:
					this.displayCursorPos++;
					break;
				case START:
					this.displayCursorPos = 0;
					break;
				case END:
					this.displayCursorPos = this.propertyValue.length();
					break;
				default:
					return false;
			}
			this.displayCursorPos = FMath.avg(0, this.displayCursorPos, this.propertyValue.length());
			this.displayCursorPosSelection = this.displayCursorPos;
			markToRedraw();
			return true;
		}
		return false;
	}
	
	@Override
	public boolean onEventInput(final EventInput event) {
		final Vector3f absolutePosition = new Vector3f(event.pos().x(), event.pos().y(), 0);
		final Vector3f relPos = relativePosition(absolutePosition);
		LOGGER.trace("Event on Input ... " + event + " relPos = " + relPos);
		if (event.inputId() == 0) {
			if (!isFocused()) {
				if (KeyStatus.leave == event.status()) {
					changeStatusIn(GuiShapeMode.NORMAL);
				} else {
					LOGGER.trace("Detect Over : " + this.overPositionStart + " -> " + this.overPositionStop);
					if (relPos.x() > this.overPositionStart.x() && relPos.y() > this.overPositionStart.y()
							&& relPos.x() < this.overPositionStop.x() && relPos.y() < this.overPositionStop.y()) {
						changeStatusIn(GuiShapeMode.OVER);
					} else {
						changeStatusIn(GuiShapeMode.NORMAL);
					}
				}
			}
		}
		if (relPos.x() < this.overPositionStart.x() || relPos.y() < this.overPositionStart.y()
				|| relPos.x() > this.overPositionStop.x() || relPos.y() > this.overPositionStop.y()) {
			LOGGER.warn("Reject {}", relPos);
			return false;
		}
		if (event.inputId() == 1) {
			if (KeyStatus.pressSingle == event.status()) {
				keepFocus();
				this.signalClick.emit();
				//nothing to do ...
				return true;
			}
			if (KeyStatus.pressDouble == event.status()) {
				keepFocus();
				// select word
				this.displayCursorPosSelection = this.displayCursorPos - 1;
				// search forward
				for (int iii = this.displayCursorPos; iii <= this.propertyValue.length(); iii++) {
					if (iii == this.propertyValue.length()) {
						this.displayCursorPos = iii;
						break;
					}
					if (!((this.propertyValue.charAt(iii) >= 'a' && this.propertyValue.charAt(iii) <= 'z')
							|| (this.propertyValue.charAt(iii) >= 'A' && this.propertyValue.charAt(iii) <= 'Z')
							|| (this.propertyValue.charAt(iii) >= '0' && this.propertyValue.charAt(iii) <= '9')
							|| this.propertyValue.charAt(iii) == '_' || this.propertyValue.charAt(iii) == '-')) {
						this.displayCursorPos = iii;
						break;
					}
				}
				// search backward
				for (int iii = this.displayCursorPosSelection; iii >= -1; iii--) {
					if (iii == -1) {
						this.displayCursorPosSelection = 0;
						break;
					}
					if (!((this.propertyValue.charAt(iii) >= 'a' && this.propertyValue.charAt(iii) <= 'z')
							|| (this.propertyValue.charAt(iii) >= 'A' && this.propertyValue.charAt(iii) <= 'Z')
							|| (this.propertyValue.charAt(iii) >= '0' && this.propertyValue.charAt(iii) <= '9')
							|| this.propertyValue.charAt(iii) == '_' || this.propertyValue.charAt(iii) == '-')) {
						this.displayCursorPosSelection = iii + 1;
						break;
					}
				}
				// Copy to clipboard Middle ...
				copySelectionToClipBoard(ClipboardList.CLIPBOARD_SELECTION);
				markToRedraw();
			} else if (KeyStatus.pressTriple == event.status()) {
				keepFocus();
				this.displayCursorPosSelection = 0;
				this.displayCursorPos = this.propertyValue.length();
			} else if (KeyStatus.down == event.status()) {
				keepFocus();
				updateCursorPosition(absolutePosition);
				markToRedraw();
			} else if (KeyStatus.move == event.status()) {
				keepFocus();
				updateCursorPosition(absolutePosition, true);
				markToRedraw();
			} else if (KeyStatus.up == event.status()) {
				keepFocus();
				updateCursorPosition(absolutePosition, true);
				// Copy to clipboard Middle ...
				copySelectionToClipBoard(ClipboardList.CLIPBOARD_SELECTION);
				markToRedraw();
			}
		} else if (KeyType.mouse == event.type() && event.inputId() == 2) {
			if (event.status() == KeyStatus.down || event.status() == KeyStatus.move
					|| event.status() == KeyStatus.up) {
				keepFocus();
				// updatethe cursor position :
				updateCursorPosition(absolutePosition);
			}
			// Paste current selection only when up button
			if (event.status() == KeyStatus.up) {
				keepFocus();
				// middle button => past data...
				ClipBoard.request(ClipboardList.CLIPBOARD_SELECTION);
			}
		}
		return false;
	}
	
	@Override
	protected void onGetFocus() {
		this.displayCursor = true;
		changeStatusIn(GuiShapeMode.SELECT);
		showKeyboard();
		markToRedraw();
	}
	
	@Override
	protected void onLostFocus() {
		this.displayCursor = false;
		changeStatusIn(GuiShapeMode.NORMAL);
		hideKeyboard();
		markToRedraw();
	}
	
	@Override
	public void onRegenerateDisplay() {
		if (!needRedraw()) {
			//return;
		}
		//LOGGER.trace("Regenerate Display ==> is needed: '" + this.propertyValue + "'");
		this.shape.clear();
		this.text.clear();
		if (this.colorIdTextFg >= 0) {
			//this.text.setDefaultColorFg(this.shape.getColor(this.colorIdTextFg));
			//this.text.setDefaultColorBg(this.shape.getColor(this.colorIdTextBg));
			//this.text.setCursorColor(this.shape.getColor(this.colorIdCursor));
			//this.text.setSelectionColor(this.shape.getColor(this.colorIdSelection));
		}
		updateTextPosition();
		final Padding padding = this.shape.getPadding();
		
		Vector3f tmpSizeShaper = this.minSize;
		Vector3f delta = this.propertyGravity.gravityGenerateDelta(this.size.less(this.minSize));
		if (this.propertyFill.x()) {
			tmpSizeShaper = tmpSizeShaper.withX(this.size.x());
			delta = delta.withX(0.0f);
		}
		if (this.propertyFill.y()) {
			tmpSizeShaper = tmpSizeShaper.withY(this.size.y());
			delta = delta.withY(0.0f);
		}
		if (this.propertyFill.z()) {
			tmpSizeShaper = tmpSizeShaper.withZ(this.size.z());
			delta = delta.withZ(0.0f);
		}
		Vector3f tmpOriginShaper = delta;
		//Vector3f tmpOriginShaper = this.size.less(tmpSizeShaper).multiply(0.5f);
		Vector3f tmpSizeText = tmpSizeShaper.less(padding.x(), padding.y(), padding.z());
		Vector3f tmpOriginText = tmpOriginShaper.add(padding.bottom(), padding.left(), padding.back()); //this.size.less(tmpSizeText).multiply(0.5f);
		//Vector3f tmpOriginText = new Vector3f(0, this.text.getSize(), 0);
		// sometimes, the user define an height bigger than the real size needed  == > in this case we need to center the text in the shaper ...
		
		final int minHeight = (int) this.text.getHeight();
		if (tmpSizeText.y() > minHeight) {
			tmpOriginText = tmpOriginText.add(0, (tmpSizeText.y() - minHeight) * 0.5f, 0);
		}
		// fix all the position in the int class:
		tmpSizeShaper = Vector3f.clipInt(tmpSizeShaper);
		tmpOriginShaper = Vector3f.clipInt(tmpOriginShaper);
		tmpSizeText = Vector3f.clipInt(tmpSizeText);
		tmpOriginText = Vector3f.clipInt(tmpOriginText);
		
		this.text.clear();
		//this.text.setSize((int) tmpSizeText.x(), (int) tmpSizeText.y());
		this.text.setClippingWidth(tmpOriginText, tmpSizeText);
		this.text.setPos(tmpOriginText.add(this.displayStartPosition, 0, 0));
		if (this.displayCursorPosSelection != this.displayCursorPos) {
			this.text.setCursorSelection(this.displayCursorPos, this.displayCursorPosSelection);
		} else {
			this.text.setCursorPos(this.displayCursorPos);
		}
		final char[] valueToDisplay = this.propertyValue.toCharArray();
		if (this.propertyPassword) {
			Arrays.fill(valueToDisplay, '*');
		}
		
		//final Vector2f plop = new Vector2f(tmpOriginText.x() + this.displayStartPosition, tmpOriginText.y());
		if (valueToDisplay.length != 0) {
			this.text.print(new String(valueToDisplay));
		} else if (this.propertyTextWhenNothing != null) {
			this.text.printDecorated(this.propertyTextWhenNothing);
		}
		this.text.setClippingMode(false);
		this.overPositionStart = tmpOriginShaper;
		this.overPositionStop = tmpOriginShaper.add(tmpSizeShaper);
		this.shape.setShape(tmpOriginShaper, tmpSizeShaper, tmpOriginText, tmpSizeText);
		this.text.flush();
		this.shape.flush();
		
	}
	
	/**
	 * remove the selected area
	 * @note This request a regeneration of the display
	 */
	public void removeSelected() {
		if (this.displayCursorPosSelection == this.displayCursorPos) {
			// nothing to cut ...
			return;
		}
		int pos1 = this.displayCursorPosSelection;
		int pos2 = this.displayCursorPos;
		if (this.displayCursorPosSelection > this.displayCursorPos) {
			pos2 = this.displayCursorPosSelection;
			pos1 = this.displayCursorPos;
		}
		// remove data ...
		this.displayCursorPos = pos1;
		this.displayCursorPosSelection = pos1;
		final StringBuilder tmp = new StringBuilder(this.propertyValue);
		if (pos1 < pos2) {
			tmp.delete(pos1, pos2);
		} else if (pos1 > pos2) {
			tmp.delete(pos2, pos2);
		}
		this.propertyValue = tmp.toString();
		markToRedraw();
	}
	
	/**
	 * internal check the value with RegExp checking
	 * @param newData The new string to display
	 */
	protected void setInternalValue(final String newData) {
		final String previous = this.propertyValue;
		// check the RegExp :
		if (newData.length() > 0) {
			/*
			if (this.regex.parse(_newData, 0, _newData.size()) == false) {
				LOGGER.info("The input data does not match with the regExp '" + _newData + "' Regex='" + propertyRegex + "'" );
				return;
			}
			if (this.regex.start() != 0) {
				LOGGER.info("The input data does not match with the regExp '" + _newData + "' Regex='" + propertyRegex + "' (start position error)" );
				return;
			}
			if (this.regex.stop() != _newData.size()) {
				LOGGER.info("The input data does not match with the regExp '" + _newData + "' Regex='" + propertyRegex + "' (stop position error)" );
				return;
			}
			*/
		}
		this.propertyValue = newData;
		markToRedraw();
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "config")
	@AknotDescription(value = "configuration of the widget")
	public void setPropertyConfig(final Uri propertyConfig) {
		if (this.propertyConfig.equals(propertyConfig)) {
			return;
		}
		this.propertyConfig = propertyConfig;
		onChangePropertyShaper();
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "max")
	@AknotDescription(value = "Maximum char that can be set on the Entry")
	public void setPropertyMaxCharacter(final int propertyMaxCharacter) {
		if (this.propertyMaxCharacter == propertyMaxCharacter) {
			return;
		}
		this.propertyMaxCharacter = propertyMaxCharacter;
		onChangePropertyMaxCharacter();
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "password")
	@AknotDescription(value = "Not display content in password mode")
	public void setPropertyPassword(final boolean propertyPassword) {
		if (this.propertyPassword == propertyPassword) {
			return;
		}
		this.propertyPassword = propertyPassword;
		onChangePropertyPassword();
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "regex")
	@AknotDescription(value = "Control what it is write with a regular expression")
	public void setPropertyRegex(final String propertyRegex) {
		if (this.propertyRegex.equals(propertyRegex)) {
			return;
		}
		this.propertyRegex = propertyRegex;
		onChangePropertyRegex();
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "empty-text")
	@AknotDescription(value = "Text when nothing is written")
	public void setPropertyTextWhenNothing(final String propertyTextWhenNothing) {
		if (this.propertyTextWhenNothing.equals(propertyTextWhenNothing)) {
			return;
		}
		this.propertyTextWhenNothing = propertyTextWhenNothing;
		onChangePropertyTextWhenNothing();
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "value")
	@AknotDescription(value = "Value display in the entry (decorated text)")
	public void setPropertyValue(final String propertyValue) {
		if (this.propertyValue.equals(propertyValue)) {
			return;
		}
		this.propertyValue = propertyValue;
		onChangePropertyValue();
	}
	
	/**
	 * change the cursor position with the curent position requested on the display
	 * @param pos Absolute position of the event
	 * @note The display is automaticly requested when change apear.
	 */
	protected void updateCursorPosition(final Vector3f pos) {
		updateCursorPosition(pos, false);
	}
	
	protected void updateCursorPosition(final Vector3f pos, final boolean selection/*=false*/) {
		final Padding padding = this.shape.getPadding();
		
		final Vector3f relPos = relativePosition(pos).less(this.overPositionStart);
		// reject when outside ...
		
		// try to find the new cursor position :
		if (this.displayStartPosition > this.propertyValue.length()) {
			this.displayStartPosition = this.propertyValue.length();
		}
		if (this.displayStartPosition < 0) {
			LOGGER.error("wring cursor position : " + this.displayStartPosition + "/" + this.propertyValue.length());
			this.displayStartPosition = 0;
		}
		String tmpDisplay = this.propertyValue.substring(0, this.displayStartPosition);
		final int displayHidenSize = (int) this.text.calculateSize(tmpDisplay).x();
		//LOGGER.debug("hidenSize : " + displayHidenSize);
		int newCursorPosition = -1;
		final int tmpTextOriginX = (int) padding.left();
		for (int iii = 0; iii < this.propertyValue.length(); iii++) {
			tmpDisplay = this.propertyValue.substring(0, iii);
			final int tmpWidth = (int) (this.text.calculateSize(tmpDisplay).x() - displayHidenSize);
			if (tmpWidth >= relPos.x() - tmpTextOriginX) {
				newCursorPosition = iii;
				break;
			}
		}
		if (newCursorPosition == -1) {
			newCursorPosition = this.propertyValue.length();
		}
		if (!selection) {
			this.displayCursorPos = newCursorPosition;
			this.displayCursorPosSelection = this.displayCursorPos;
			markToRedraw();
		} else {
			if (this.displayCursorPos == this.displayCursorPosSelection) {
				this.displayCursorPosSelection = this.displayCursorPos;
			}
			this.displayCursorPos = newCursorPosition;
			markToRedraw();
		}
		markToUpdateTextPosition();
	}
	
	/**
	 * update the display position start  == > depending of the position of the Cursor and the size of the Data inside
	 * @change this.displayStartPosition < ==  updated
	 */
	protected void updateTextPosition() {
		if (!this.needUpdateTextPos) {
			return;
		}
		final Padding padding = this.shape.getPadding();
		
		int tmpSizeX = (int) this.minSize.x();
		if (this.propertyFill.x()) {
			tmpSizeX = (int) this.size.x();
		}
		final int tmpUserSize = (int) (tmpSizeX - padding.x());
		final int totalWidth = (int) this.text.calculateSize(this.propertyValue).x();
		// all can not be set :
		final String tmpDisplay = this.propertyValue.substring(0, this.displayCursorPos);
		this.displayCursorPositionPixel = (int) this.text.calculateSize(tmpDisplay).x();
		// Check if the data inside the display can be contain in the entry box
		if (totalWidth < tmpUserSize) {
			// all can be display :
			this.displayStartPosition = 0;
		} else {
			// check if the Cursor is visible at 10px nearest the border :
			final int tmp1 = this.displayCursorPositionPixel + this.displayStartPosition;
			LOGGER.trace(
					"cursorPos=" + this.displayCursorPositionPixel + "px maxSize=" + tmpUserSize + "px tmp1=" + tmp1);
			if (tmp1 < 10) {
				// set the cursor on the left
				this.displayStartPosition = Math.min(-this.displayCursorPositionPixel + 10, 0);
			} else if (tmp1 > tmpUserSize - 10) {
				// set the cursor of the Right
				this.displayStartPosition = Math.min(-this.displayCursorPositionPixel + tmpUserSize - 10, 0);
			}
			// else : the cursor is inside the display
			//this.displayStartPosition = -totalWidth + tmpUserSize;
		}
	}
	
}
