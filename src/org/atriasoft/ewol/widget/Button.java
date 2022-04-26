package org.atriasoft.ewol.widget;

import org.atriasoft.esignal.Connection;
import org.atriasoft.esignal.Signal;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector3b;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.Padding;
import org.atriasoft.ewol.annotation.EwolDescription;
import org.atriasoft.ewol.annotation.EwolSignal;
import org.atriasoft.ewol.compositing.GuiShape;
import org.atriasoft.ewol.compositing.GuiShapeMode;
import org.atriasoft.ewol.compositing.ShapeBox;
import org.atriasoft.ewol.event.EventEntry;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.event.EventTime;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.object.EwolObject;
import org.atriasoft.exml.annotation.XmlAttribute;
import org.atriasoft.exml.annotation.XmlManaged;
import org.atriasoft.exml.annotation.XmlName;
import org.atriasoft.gale.key.KeyKeyboard;
import org.atriasoft.gale.key.KeyStatus;

/**
 * @ingroup ewolWidgetGroup
 * Entry box display :
 *
 * ~~~~~~~~~~~~~~~~~~~~~~
 * 	----------------------------------------------
 * 	|                Text Label                  |
 * 	----------------------------------------------
 * ~~~~~~~~~~~~~~~~~~~~~~
 */
public class Button extends ContainerToggle {
	public enum ButtonLock {
		LOCK_NONE, //!< normal status of the button
		LOCK_WHEN_PRESSED, //!< When the state is set in pressed, the status stay in this one
		LOCK_WHEN_RELEASED, //!< When the state is set in not pressed, the status stay in this one
		LOCK_ACCESS, //!< all event are trashed  == > acctivity of the button is disable
	}
	
	public static Button createLabelButton(final String label) {
		final Button out = new Button();
		final Label labelWidget = new Label();
		labelWidget.setPropertyFontSize(12);
		labelWidget.setPropertyFill(Vector3b.FALSE);
		labelWidget.setPropertyExpand(Vector3b.FALSE);
		labelWidget.setPropertyGravity(Gravity.CENTER);
		labelWidget.setPropertyValue(label);
		out.setSubWidget(labelWidget, 0);
		return out;
	}
	
	public static Button createToggleLabelButton(final String label0, final String label1) {
		final Button out = new Button();
		{
			final Label labelWidget = new Label();
			labelWidget.setPropertyFill(Vector3b.FALSE);
			labelWidget.setPropertyExpand(Vector3b.FALSE);
			labelWidget.setPropertyGravity(Gravity.CENTER);
			labelWidget.setPropertyValue(label0);
			out.setSubWidget(labelWidget, 0);
		}
		{
			final Label labelWidget = new Label();
			labelWidget.setPropertyFill(Vector3b.FALSE);
			labelWidget.setPropertyExpand(Vector3b.FALSE);
			labelWidget.setPropertyGravity(Gravity.CENTER);
			labelWidget.setPropertyValue(label1);
			out.setSubWidget(labelWidget, 1);
		}
		out.setPropertyToggleMode(true);
		return out;
	}
	
	/**
	 * Periodic call to update grapgic display
	 * @param event Time generic event
	 */
	protected static void periodicCall(final Button self, final EventTime event) {
		Log.verbose("Periodic call on Entry(" + event + ")");
		if (!self.shape.periodicCall(event)) {
			self.periodicConnectionHanble.close();
		}
		self.markToRedraw();
	}
	
	/// Periodic call handle to remove it when needed
	protected Connection periodicConnectionHanble = new Connection();
	
	private Uri propertyConfig = new Uri("THEME", "shape/Button.json", "ewol");
	private boolean propertyValue = false;
	private ButtonLock propertyLock = ButtonLock.LOCK_NONE;
	private boolean propertyToggleMode = false;
	private boolean propertyEnableSingle = false;
	
	protected ShapeBox shapeProperty = ShapeBox.ZERO;
	private GuiShape shape;
	
	@EwolSignal(name = "down", description = "Button is Down")
	public SignalEmpty signalDown = new SignalEmpty();
	@EwolSignal(name = "up", description = "Button is Up")
	public SignalEmpty signalUp = new SignalEmpty();
	@EwolSignal(name = "click", description = "Button is Clicked")
	public SignalEmpty signalClick = new SignalEmpty();
	@EwolSignal(name = "enter", description = "The cursor enter inside the button")
	public SignalEmpty signalEnter = new SignalEmpty();
	@EwolSignal(name = "leave", description = "The cursor leave the button")
	public SignalEmpty signalLeave = new SignalEmpty();
	@EwolSignal(name = "value", description = "The button value change")
	public Signal<Boolean> signalValue = new Signal<>();
	
	private boolean buttonPressed = false;
	private boolean mouseHover = false;
	
	/**
	 * Constructor
	 */
	public Button() {
		this.propertyCanFocus = true;
		onChangePropertyShaper();
		// can not support multiple click...
		setMouseLimit(1);
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
		calculateMinMaxSizePadded(padding);
		Log.verbose("[{}] Result min size : {}", getId(), this.minSize);
	}
	
	protected void changeStatusIn(final GuiShapeMode newStatusId) {
		if (this.shape.changeStatusIn(newStatusId)) {
			if (!this.periodicConnectionHanble.isConnected()) {
				//Log.error("REQUEST: connection on periodic call");
				this.periodicConnectionHanble = EwolObject.getObjectManager().periodicCall.connect(this, Button::periodicCall);
			}
			markToRedraw();
		}
		
	}
	
	void checkStatus() {
		if (this.buttonPressed) {
			changeStatusIn(GuiShapeMode.SELECT);
			return;
		}
		if (this.mouseHover) {
			changeStatusIn(GuiShapeMode.OVER);
			return;
		}
		if (this.propertyValue) {
			changeStatusIn(GuiShapeMode.NORMAL);
		}
		changeStatusIn(GuiShapeMode.NONE);
	}
	
	@XmlManaged
	@XmlAttribute
	@XmlName(value = "config")
	@EwolDescription(value = "configuration of the widget")
	public Uri getPropertyConfig() {
		return this.propertyConfig;
	}
	
	@XmlManaged
	@XmlAttribute
	@XmlName(value = "lock")
	@EwolDescription(value = "Lock the button in a special state to permit changing state only by the coder")
	public ButtonLock getPropertyLock() {
		return this.propertyLock;
	}
	
	@XmlManaged
	@XmlAttribute
	@XmlName(value = "value")
	@EwolDescription(value = "Value display in the entry (decorated text)")
	public boolean getPropertyValue() {
		return this.propertyValue;
	}
	
	@XmlManaged
	@XmlAttribute
	@XmlName(value = "enable-single")
	@EwolDescription(value = "If one element set in the Button ==> display only set")
	public boolean isPropertyEnableSingle() {
		return this.propertyEnableSingle;
	}
	
	@XmlManaged
	@XmlAttribute
	@XmlName(value = "toggle")
	@EwolDescription(value = "The button can toggle")
	public boolean isPropertyToggleMode() {
		return this.propertyToggleMode;
	}
	
	void onChangePropertyEnableSingle() {
		if (this.propertyEnableSingle) {
			if (this.idWidgetDisplayed == 0 && this.subWidget[0] == null && this.subWidget[1] != null) {
				this.idWidgetDisplayed = 1;
			} else if (this.idWidgetDisplayed == 1 && this.subWidget[1] == null && this.subWidget[0] != null) {
				this.idWidgetDisplayed = 0;
			} else if (this.subWidget[0] == null && this.subWidget[1] == null) {
				this.idWidgetDisplayed = 0;
			}
		}
	}
	
	void onChangePropertyLock() {
		if (ButtonLock.LOCK_ACCESS == this.propertyLock) {
			this.buttonPressed = false;
			this.mouseHover = false;
		}
		checkStatus();
		markToRedraw();
	}
	
	protected void onChangePropertyShaper() {
		if (this.shape == null) {
			this.shape = new GuiShape(this.propertyConfig);
		} else {
			this.shape.setSource(this.propertyConfig);
		}
		markToRedraw();
	}
	
	void onChangePropertyToggleMode() {
		this.propertyValue = !this.propertyValue;
		this.signalValue.emit(this.propertyValue);
		if (!this.propertyToggleMode) {
			this.idWidgetDisplayed = 0;
		} else {
			if (!this.propertyValue) {
				this.idWidgetDisplayed = 0;
			} else {
				this.idWidgetDisplayed = 1;
			}
		}
		if (this.propertyEnableSingle) {
			if (this.idWidgetDisplayed == 0 && this.subWidget[0] == null && this.subWidget[1] != null) {
				this.idWidgetDisplayed = 1;
			} else if (this.idWidgetDisplayed == 1 && this.subWidget[1] == null && this.subWidget[0] != null) {
				this.idWidgetDisplayed = 0;
			}
		}
		checkStatus();
		markToRedraw();
	}
	
	protected void onChangePropertyValue() {
		if (this.propertyToggleMode) {
			if (!this.propertyValue) {
				this.idWidgetDisplayed = 0;
			} else {
				this.idWidgetDisplayed = 1;
			}
		}
		if (this.propertyEnableSingle) {
			if (this.idWidgetDisplayed == 0 && this.subWidget[0] == null && this.subWidget[1] != null) {
				this.idWidgetDisplayed = 1;
			} else if (this.idWidgetDisplayed == 1 && this.subWidget[1] == null && this.subWidget[0] != null) {
				this.idWidgetDisplayed = 0;
			}
		}
		checkStatus();
		markToRedraw();
	}
	
	@Override
	public void onChangeSize() {
		final Padding padding = this.shape.getPadding();
		onChangeSizePadded(padding);
	}
	
	@Override
	protected void onDraw() {
		if (this.shape != null) {
			this.shape.draw(true);
		}
		super.onDraw();
	}
	
	@Override
	protected boolean onEventEntry(final EventEntry event) {
		//Log.debug("BT PRESSED : \"" << UTF8_data << "\" size=" << strlen(UTF8_data));
		if (event.type() == KeyKeyboard.CHARACTER && event.status() == KeyStatus.down && event.getChar() == '\r') {
			this.signalEnter.emit();
			return true;
		}
		return super.onEventEntry(event);
	}
	
	@Override
	public boolean onEventInput(final EventInput event) {
		final Vector3f relPos = relativePosition(new Vector3f(event.pos().x(), event.pos().y(), 0));
		Log.warning("Event on Input ... " + event + " relPos = " + relPos);
		final boolean over = this.shapeProperty.isInside(relPos);
		//filter if outside the element...
		if (event.status() == KeyStatus.leave) {
			changeStatusIn(GuiShapeMode.NORMAL);
			this.buttonPressed = false;
			return true;
		}
		if (event.inputId() == 0) {
			if (!this.buttonPressed) {
				if (KeyStatus.leave == event.status()) {
					changeStatusIn(GuiShapeMode.NORMAL);
				} else {
					Log.verbose("Detect Over : " + this.shapeProperty);
					if (over) {
						changeStatusIn(GuiShapeMode.OVER);
					} else {
						changeStatusIn(GuiShapeMode.NORMAL);
					}
				}
				return true;
			}
		}
		if (event.inputId() != 1) {
			return false;
		}
		if (KeyStatus.pressSingle == event.status() && over) {
			keepFocus();
			this.signalClick.emit();
			if (this.propertyToggleMode) {
				this.setPropertyValue(!this.propertyValue);
			}
			return true;
		}
		if (KeyStatus.down == event.status() && over) {
			keepFocus();
			this.buttonPressed = true;
			changeStatusIn(GuiShapeMode.SELECT);
			markToRedraw();
			this.signalDown.emit();
			return true;
		}
		if (KeyStatus.move == event.status() && over) {
			keepFocus();
			markToRedraw();
			return true;
		}
		if (KeyStatus.up == event.status() && this.buttonPressed) {
			keepFocus();
			this.buttonPressed = false;
			this.signalUp.emit();
			changeStatusIn(GuiShapeMode.OVER);
			markToRedraw();
			return true;
		}
		return false;
	}
	
	@Override
	protected void onLostFocus() {
		this.buttonPressed = false;
		Log.verbose(this.name + " : Remove Focus ...");
		checkStatus();
	}
	
	@Override
	public void onRegenerateDisplay() {
		super.onRegenerateDisplay();
		if (!needRedraw()) {
			//return;
		}
		//Log.verbose("Regenerate Display ==> is needed: '" + this.propertyValue + "'");
		this.shape.clear();
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
			tmpSizeShaper = tmpSizeShaper.withZ(this.size.y());
			delta = delta.withZ(0.0f);
		}
		
		Vector3f tmpOriginShaper = delta;
		Vector3f tmpSizeText = tmpSizeShaper.less(padding.x(), padding.y(), padding.z());
		//Vector3f tmpOriginText = this.size.less(tmpSizeText).multiply(0.5f);
		Vector3f tmpOriginText = new Vector3f(0, 0, 0);
		// not sure this is needed...
		tmpSizeShaper = tmpSizeShaper.clipInteger();
		tmpOriginShaper = tmpOriginShaper.clipInteger();
		tmpSizeText = tmpSizeText.clipInteger();
		tmpOriginText = tmpOriginText.clipInteger();
		
		this.shapeProperty = new ShapeBox(tmpOriginShaper, tmpSizeShaper, padding);
		this.shape.setShape(tmpOriginShaper, tmpSizeShaper, tmpOriginText, tmpSizeText);
		this.shape.flush();
		
	}
	
	public void setPropertyConfig(final Uri propertyConfig) {
		if (this.propertyConfig.equals(propertyConfig)) {
			return;
		}
		this.propertyConfig = propertyConfig;
		onChangePropertyShaper();
	}
	
	public void setPropertyEnableSingle(final boolean propertyEnableSingle) {
		this.propertyEnableSingle = propertyEnableSingle;
		markToRedraw();
	}
	
	public void setPropertyLock(final ButtonLock propertyLock) {
		this.propertyLock = propertyLock;
		markToRedraw();
	}
	
	public void setPropertyToggleMode(final boolean propertyToggleMode) {
		this.propertyToggleMode = propertyToggleMode;
		markToRedraw();
	}
	
	public void setPropertyValue(final boolean propertyValue) {
		if (this.propertyValue == propertyValue) {
			return;
		}
		this.propertyValue = propertyValue;
		this.signalValue.emit(this.propertyValue);
		onChangePropertyValue();
	}
	
}
