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
 * a composed Select is a Select with an inside composed with the specify XML element 
 * ==> this permit to generate standard element simple
 */
class Select extends SpinBase {
	@EwolSignal(name = "value", description = "Select value change")
	public Signal<int> signalValue = new Signal<int>();
	/*
	  propertyValue(this, "value",
              "Value of the Select",
              ewol::widget::Select::onChangePropertyValue) {
	*/
	protected int propertyValue = -1; //!< Current state of the Select.
		/**
		 * Constructor
		 * @param _shaperName Shaper file properties
		 */
	public Select() {
		setPropertyShape( new Uri("THEME","/shape/Select.json", "ewol"));
		
		propertySpinMode = SpinPosition.noneRight);
		/*
		propertySpinMode.changeDefault(ewol::widget::spinPosition_noneRight);
		propertySpinMode.rename("none-none", "none");
		propertySpinMode.rename("none-right", "right");
		propertySpinMode.rename("left-none", "left");
		propertySpinMode.remove("left-right");
		propertySpinMode.remove("left-left");
		propertySpinMode.remove("right-right");
		*/
	}
	protected class Element {
			public:
				int value;
				String name;
				boolean selected;
			public:
				// TODO: Remove this: due to the fact my List is not full implemented
				Element() {}
				Element(int _value, String _name, boolean _selected=false) {
				  this.value = _value;
				  this.name = _name;
				  this.selected = _selected;
				}
		};
		protected List<ewol::widget::Select::Element> this.listElement;
	public void optionSelectDefault(){
		if (this.widgetEntry == null) {
			LOGGER.error("Can not acces at entry ...");
			return;
		}
		for (auto it : this.listElement) {
			if (it.this.selected == true) {
				return;
			}
		}
		if (this.listElement.size() == 0) {
			this.widgetEntry.propertyValue.set("");
		}
		this.widgetEntry.propertyValue.set(this.listElement[0].this.name);
	}
	public void optionRemove(int _value){
		for (auto it=this.listElement.begin(); it != this.listElement.end(); ++it) {
			if (_value == it.this.value) {
				LOGGER.debug("remove element: " + _value);
				this.listElement.erase(it);
				break;
			}
		}
		optionSelectDefault();
	}

	public void optionClear() {
		this.listElement.clear();
		optionSelectDefault();
	}

	public void optionAdd(int _value, String _name) {
		for (auto it : this.listElement) {
			if (_value == it.this.value) {
				LOGGER.debug("replace element: " + _value + " with: '" + _data + "'");
				it.this.name = _data;
			}
		}
		this.listElement.pushBack(ewol::widget::Select::Element(_value, _data, false));
	}
	/*
	protected boolean loadXML( exml::Element _node){
		if (_node.exist() == false) {
			return false;
		}
		// parse generic properties:
		ewol::widget::SpinBase::loadXML(_node);
		// remove previous element:
		//subWidgetRemove();
		// parse all the elements:
		for( auto it : _node.nodes) {
			exml::Element pNode = it.toElement();
			if (pNode.exist() == false) {
				// trash here all that is not element
				continue;
			}
			if (pNode.getValue() != "option") {
				LOGGER.error("(l " + pNode.getPos() + ") Unknown basic node='" + pNode.getValue() + "' not in : [option]" );
				continue;
			}
			String valId = pNode.attributes["id"];
			String valIsSelected = pNode.attributes["select"];
			String valText = pNode.getText();
			int id = etk::string_to_int(valId);
			boolean select = etk::string_to_bool(valIsSelected);
			optionAdd(id, valText);
			if (select == true) {
				propertyValue.set(id);
			}
			LOGGER.warn("Add option : id='" + valId + "' select='" + valIsSelected + "' text='" + valText + "'");
		}
		return true;
	}
	*/
	protected void updateGui() {
		ewol::widget::SpinBase::updateGui();
		
		if (    this.widgetEntry != null
		     && this.connectionEntry.isConnected() == false) {
			
		}
		if (    this.widgetButtonUp != null
		     && this.connectionButton.isConnected() == false) {
			this.connectionButton = this.widgetButtonUp.signalPressed.connect(this, ewol::widget::Select::onCallbackOpenMenu);
		}
		
	}
	protected void onCallbackOpenMenu() {
		// create a context menu:
		ewol::widget::ContextMenu tmpContext = ewol::widget::ContextMenu::create();
		if (tmpContext == null) {
			LOGGER.error("Allocation Error");
			return;
		}
		// auto-select mark position:
		tmpContext.setPositionMarkAuto(this.origin, this.size);
		ewol::widget::Sizer mySizer;
		mySizer = ewol::widget::Sizer::create();
		if (mySizer == null) {
			LOGGER.error("Allocation Error or sizer");
			return;
		}
		mySizer.propertyMode.set(widget::Sizer::modeVert);
		mySizer.propertyLockExpand.set(Vector2f(true,true));
		mySizer.propertyFill.set(Vector2f(true,true));
		// set it in the pop-up-system:
		tmpContext.setSubWidget(mySizer);
		for (auto it : this.listElement) {
			ewol::widget::Label myLabel = ewol::widget::Label::create();
			if (myLabel == null) {
				LOGGER.error("Allocation Error");
				continue;
			}
			if (it.this.selected == true) {
				myLabel.propertyValue.set(String("<b>") + it.this.name + "</b>");
			} else {
				myLabel.propertyValue.set(it.this.name);
			}
			myLabel.propertyExpand.set(Vector2b(true,true));
			myLabel.propertyFill.set(Vector2b(true,true));
			// set callback
			myLabel.signalPressed.connect(sharedFromThis(), ewol::widget::Select::onCallbackLabelPressed, it.this.value);
			myLabel.signalPressed.connect(tmpContext, ewol::widget::ContextMenu::destroy);
			// add it in the widget list
			mySizer.subWidgetAddStart(myLabel);
		}
		ewol::widget::Windows currentWindows = getWindows();
		if (currentWindows == null) {
			LOGGER.error("Can not get the curent Windows...");
		} else {
			currentWindows.popUpWidgetPush(tmpContext);
		}
	}
	protected void onCallbackLabelPressed(int _value){
		LOGGER.trace("User select:" + _value);
		propertyValue.set(_value);
	}
	protected esignal::Connection connectionEntry = null;
	protected esignal::Connection connectionButton = null;
	protected void onChangePropertyValue(){
		markToRedraw();
		if (this.widgetEntry == null) {
			LOGGER.error("Can not acces at entry ...");
			return;
		}
		for (auto it : this.listElement) {
			if (it.this.value == propertyValue.get()) {
				if (it.this.selected == false) {
					it.this.selected = true;
					this.widgetEntry.propertyValue.set(it.this.name);
					signalValue.emit(propertyValue.get());
				}
			} else {
				it.this.selected = false;
			}
		}
	}
}
