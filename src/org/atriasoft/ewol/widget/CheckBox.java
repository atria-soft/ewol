package org.atriasoft.ewol.widget;

import org.atriasoft.esignal.Signal;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.math.Vector3b;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.annotation.EwolDescription;
import org.atriasoft.ewol.annotation.EwolSignal;
import org.atriasoft.ewol.widget.Sizer.DisplayMode;
import org.atriasoft.exml.annotation.XmlAttribute;
import org.atriasoft.exml.annotation.XmlManaged;
import org.atriasoft.exml.annotation.XmlName;

public class CheckBox extends Container {
	
	protected static void eventLabelClick(final CheckBox self) {
		self.tick.setPropertyValue(!self.tick.getPropertyValue());
		self.signalClick.emit();
	}
	
	protected static void eventTickClick(final CheckBox self) {
		self.signalClick.emit();
	}
	
	protected static void eventTickDown(final CheckBox self) {
		self.signalDown.emit();
	}
	
	protected static void eventTickUp(final CheckBox self) {
		self.signalUp.emit();
	}
	
	protected static void eventTickValue(final CheckBox self, final Boolean value) {
		self.signalValue.emit(value);
	}
	
	@EwolSignal(name = "down", description = "CheckBox is Down")
	public SignalEmpty signalDown = new SignalEmpty();
	@EwolSignal(name = "up", description = "CheckBox is Up")
	public SignalEmpty signalUp = new SignalEmpty();
	@EwolSignal(name = "click", description = "CheckBox is Clicked")
	public SignalEmpty signalClick = new SignalEmpty();
	@EwolSignal(name = "value", description = "CheckBox value change")
	public Signal<Boolean> signalValue = new Signal<>();
	final Tick tick;
	final Label label;
	
	public CheckBox() {
		this("No Label");
	}
	
	public CheckBox(final String basicLabel) {
		final Sizer subs = new Sizer(DisplayMode.modeHori);
		subs.setPropertyLockExpand(Vector3b.TRUE);
		subs.setPropertyGravity(Gravity.CENTER);
		setSubWidget(subs);
		
		this.tick = new Tick();
		this.tick.setPropertyExpand(new Vector3b(false, true, true));
		this.tick.setPropertyFill(Vector3b.FALSE);
		this.tick.setPropertyGravity(Gravity.CENTER);
		subs.subWidgetAdd(this.tick);
		this.tick.signalClick.connectAuto(this, CheckBox::eventTickClick);
		this.tick.signalUp.connectAuto(this, CheckBox::eventTickUp);
		this.tick.signalDown.connectAuto(this, CheckBox::eventTickDown);
		this.tick.signalValue.connectAuto(this, CheckBox::eventTickValue);
		
		this.label = new Label(basicLabel);
		this.label.setPropertyExpand(Vector3b.TRUE);
		this.label.setPropertyFill(Vector3b.FALSE);
		this.label.setPropertyGravity(Gravity.LEFT);
		subs.subWidgetAdd(this.label);
		this.label.signalPressed.connectAuto(this, CheckBox::eventLabelClick);
	}
	
	@XmlManaged
	@XmlAttribute
	@XmlName(value = "label")
	@EwolDescription(value = "value of the label")
	public String getPropertyLabel() {
		return this.label.getPropertyValue();
	}
	
	@XmlManaged
	@XmlAttribute
	@XmlName(value = "value")
	@EwolDescription(value = "State of the checkbox")
	public Boolean getPropertyValue() {
		return this.tick.getPropertyValue();
	}
	
	public void setPropertyLabel(final String value) {
		this.label.setPropertyValue(value);
	}
	
	public void setPropertyValue(final Boolean value) {
		this.tick.setPropertyValue(value);
	}
	
}
