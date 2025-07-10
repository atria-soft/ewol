package org.atriasoft.ewol.widget;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.aknot.annotation.AknotSignal;
import org.atriasoft.esignal.Connection;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension1f;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.event.EventEntry;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.event.EventTime;
import org.atriasoft.gale.key.KeyKeyboard;
import org.atriasoft.gale.key.KeyStatus;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

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
public class Button extends Box {
	private static final Logger LOGGER = LoggerFactory.getLogger(Button.class);

	public static Button createLabelButton(final String label) {
		final Button out = new Button();
		final Label labelWidget = new Label();
		labelWidget.setPropertyFontSize(12);
		labelWidget.setPropertyFill(Vector2b.FALSE);
		labelWidget.setPropertyExpand(Vector2b.FALSE);
		labelWidget.setPropertyGravity(Gravity.CENTER);
		labelWidget.setPropertyValue(label);
		out.setSubWidget(labelWidget);
		return out;
	}
	
	/**
	 * Periodic call to update graphic display
	 * @param event Time generic event
	 */
	protected static void periodicCall(final Button self, final EventTime event) {
		LOGGER.trace("Periodic call on Entry(" + event + ")");
		//		if (!self.shape.periodicCall(event)) {
		//			self.periodicConnectionHanble.close();
		//		}
		self.markToRedraw();
	}
	
	/// Periodic call handle to remove it when needed
	protected Connection periodicConnectionHanble = new Connection();
	
	private Uri propertyConfig = new Uri("THEME", "shape/Button.json", "ewol");
	
	@AknotSignal
	@AknotName(value = "down")
	@AknotDescription("Button is Down")
	public SignalEmpty signalDown = new SignalEmpty();
	@AknotSignal
	@AknotName(value = "up")
	@AknotDescription("Button is Up")
	public SignalEmpty signalUp = new SignalEmpty();
	@AknotSignal
	@AknotName(value = "click")
	@AknotDescription("Button is Clicked")
	public SignalEmpty signalClick = new SignalEmpty();
	@AknotSignal
	@AknotName(value = "enter")
	@AknotDescription("The cursor enter inside the button")
	public SignalEmpty signalEnter = new SignalEmpty();
	@AknotSignal
	@AknotName(value = "leave")
	@AknotDescription("The cursor leave the button")
	public SignalEmpty signalLeave = new SignalEmpty();
	
	private boolean buttonPressed = false;
	private final boolean mouseHover = false;
	
	/**
	 * Constructor
	 */
	public Button() {
		this.propertyCanFocus = true;
		//onChangePropertyShaper();
		// can not support multiple click...
		setMouseLimit(1);
		setPropertyExpand(Vector2b.TRUE);
		setPropertyFill(Vector2b.TRUE);
		setPropertyBorderWidth(new Dimension1f(4));
		//setPropertyBorderRadius(new Dimension1f(15));
		setPropertyBorderColor(Color.BLACK);
		setPropertyColor(Color.WHITE);
		setPropertyPadding(new Dimension2f(new Vector2f(3, 3)));
		setPropertyMargin(new Dimension2f(new Vector2f(0, 0)));

	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "config")
	@AknotDescription(value = "configuration of the widget")
	public Uri getPropertyConfig() {
		return this.propertyConfig;
	}

	@Override
	protected boolean onEventEntry(final EventEntry event) {
		//LOGGER.debug("BT PRESSED : \"" << UTF8_data << "\" size=" << strlen(UTF8_data));
		if (event.type() == KeyKeyboard.CHARACTER && event.status() == KeyStatus.down && event.getChar() == '\r') {
			this.signalEnter.emit();
			return true;
		}
		return super.onEventEntry(event);
	}
	
	@Override
	public boolean onEventInput(final EventInput event) {
		final Vector2f relPos = relativePosition(event.pos());
		//LOGGER.warn("Event on Input ... " + event + " relPos = " + relPos);
		final boolean over = isInside(relPos);
		//filter if outside the element...
		if (event.status() == KeyStatus.leave) {
			//			changeStatusIn(GuiShapeMode.NORMAL);
			this.buttonPressed = false;
			return true;
		}
		if (event.inputId() == 0) {
			if (!this.buttonPressed) {
				//				if (KeyStatus.leave == event.status()) {
				//					changeStatusIn(GuiShapeMode.NORMAL);
				//				} else {
				//					LOGGER.trace("Detect Over : " + this.shapeProperty);
				//					if (over) {
				//						changeStatusIn(GuiShapeMode.OVER);
				//					} else {
				//						changeStatusIn(GuiShapeMode.NORMAL);
				//					}
				//				}
				return true;
			}
		}
		if (event.inputId() != 1) {
			return false;
		}
		if (KeyStatus.pressSingle == event.status() && over) {
			keepFocus();
			this.signalClick.emit();
			LOGGER.info("Generate click event !!!!!!!!!!");
			/////// setPropertyValue(!this.propertyValue);
			return true;
		}
		if (KeyStatus.down == event.status() && over) {
			keepFocus();
			this.buttonPressed = true;
			/////   changeStatusIn(GuiShapeMode.SELECT);
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
			//////  changeStatusIn(GuiShapeMode.OVER);
			markToRedraw();
			return true;
		}
		return false;
	}
	
	@Override
	protected void onLostFocus() {
		this.buttonPressed = false;
		LOGGER.trace(this.name + " : Remove Focus ...");
		//checkStatus();
	}
	
	public void setPropertyConfig(final Uri propertyConfig) {
		if (this.propertyConfig.equals(propertyConfig)) {
			return;
		}
		this.propertyConfig = propertyConfig;
		//onChangePropertyShaper();
	}
}
