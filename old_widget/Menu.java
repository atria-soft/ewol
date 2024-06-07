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
 */
public class Menu extends Sizer
{
	public class MenuElement {
		public MenuElement() { };
		public int localId;
		public int parentId;
		public WeakReference<Widget> widgetPointer;
		public String label;
		public String image;
		public String message;
	};
	@EwolSignal(name = "select", description = "menu is selected")
	public Signal<String> signalSelect; // event on a menu button or ...
	public Menu() {
		propertyLockExpand = Vector2b(true,true);
	}
	private void subWidgetRemoveAll(){
		clear();
		ewol::widget::Sizer::subWidgetRemoveAll();
	}
	private int subWidgetAdd(Widget _newWidget){
		LOGGER.error("Not availlable");
		return -1;
	}
	
	private void subWidgetRemove(Widget _newWidget){
		LOGGER.error("Not availlable");
	}
	
	private void subWidgetUnLink(Widget _newWidget){
		LOGGER.error("Not availlable");
	}
	protected static final String eventButtonPressed = "menu-local-pressed";
	private boolean loadXML( exml::Element _node) ;
	private List<MenuElement> listElement;
	private int staticObjectId = 666; // unique ID for every element of the menu ...
	// TODO: set it as week
	
	private ContextMenu widgetContextMenu;
	private int get(String _label) {
		for (auto it : this.listElement) {
			if (it.this.label == _label) {
				return it.this.localId;
			}
		}
		return -1;
	}
	
	public void clear(){
		this.listElement.clear();
	}
	
	public int addTitle( final String _label,  final String _image="",  String _message = ""){
		return add(-1, _label, _image, _message);
	}
	
	public int add(final int _parent,  final String _label,  final String _image="",  String _message = "") {
		// try to find one already created:
		int previous = get(_label);
		if (previous != -1) {
			return previous;
		}
		ewol::widget::MenuElement tmpObject;
		tmpObject.this.localId = this.staticObjectId++;
		tmpObject.this.parentId = _parent;
		tmpObject.this.label = _label;
		tmpObject.this.image = _image;
		tmpObject.this.message = _message;
		if (tmpObject.this.parentId == -1) {
			ewol::widget::Button myButton = ewol::widget::Button::create();
			if (myButton == null) {
				LOGGER.error("Allocation button error");
				return tmpObject.this.localId;
			}
			if (tmpObject.this.image.size()!=0) {
				String composeString ="<sizer mode='HORIZONTAL' expand='true,false' fill='true,true'>\n";
				if (etk::end_with(tmpObject.this.image, ".edf") == true) {
					composeString+="    <image src='" + tmpObject.this.image + "' size='8,8mm' distance-field='true'/>\n";
				} else {
					composeString+="    <image src='" + tmpObject.this.image + "' size='8,8mm'/>\n";
				}
				composeString+="    <label><left>" + tmpObject.this.label + "</left></label>\n";
				composeString+="</sizer>\n";
				myButton.setSubWidget(ewol::widget::composerGenerateString(composeString));
			} else {
				ewol::widget::Label label = ewol::widget::Label::create();
				label.propertyValue.set("<left>" + tmpObject.this.label + "</left>");
				myButton.setSubWidget(label);
			}
			// add it in the widget list
			ewol::widget::Sizer::subWidgetAdd(myButton);
			// keep the specific event ...
			myButton.signalPressed.connect(sharedFromThis(), ewol::widget::Menu::onButtonPressed, ewol::widget::ButtonWeak(myButton));
			tmpObject.this.widgetPointer = myButton;
		}
		this.listElement.pushBack(tmpObject);
		return tmpObject.this.localId;
	}

	
	public int addSpacer(final int _parent=-1){
		ewol::widget::MenuElement tmpObject;
		tmpObject.this.localId = this.staticObjectId++;
		tmpObject.this.parentId = _parent;
		tmpObject.this.label = "";
		tmpObject.this.image = "";
		tmpObject.this.message = "";
		if (tmpObject.this.parentId == -1) {
			ewol::widget::Spacer mySpacer = ewol::widget::Spacer::create();
			if (mySpacer == null) {
				LOGGER.error("Allocation spacer error");
				return tmpObject.this.localId;
			}
			mySpacer.propertyExpand.set(Vector2b(true,true));
			mySpacer.propertyFill.set(Vector2b(true,true));
			mySpacer.propertyMinSize.set(gale::Dimension(Vector2f(2,0), gale::distance::pixel));
			mySpacer.propertyMaxSize.set(gale::Dimension(Vector2f(2,10000), gale::distance::pixel));
			mySpacer.propertyColor.set(etk::Color<>(0,0,0,0xFF));
			// add it in the widget list
			ewol::widget::Sizer::subWidgetAdd(mySpacer);
		}
		this.listElement.pushBack(tmpObject);
		return tmpObject.this.localId;
	}
	
	public void remove(int _id){
		LOGGER.info("[TODO] NOT remove...");
	}


	
	private void onButtonPressed(ewol::widget::ButtonWeak _button){
		ewol::widget::Button caller = _button.lock();
		if (caller == null) {
			return;
		}
		for (auto it : this.listElement) {
			if (caller != it.this.widgetPointer.lock()) {
				continue;
			}
			// 2 posible case (have a message or have a child ...
			if (it.this.message.size() > 0) {
				LOGGER.debug("Menu  == > generate Event");
				// Send a multicast event ...
				signalSelect.emit(it.this.message);
				ewol::widget::ContextMenu tmpContext = this.widgetContextMenu.lock();
				if (tmpContext != null) {
					LOGGER.debug("Mark the menu to remove ...");
					tmpContext.destroy();
				}
				return;
			}
			LOGGER.debug("Menu  == > load Sub Menu");
			boolean findChild = false;
			for (auto it2 : this.listElement) {
				if (it.this.localId == it2.this.parentId) {
					findChild = true;
					break;
				}
			}
			if (false == findChild) {
				LOGGER.warn("Event on menu element with no child an no event... label=" + it.this.label);
				return;
			}
			// create a context menu:
			ewol::widget::ContextMenu tmpContext = ewol::widget::ContextMenu::create();
			this.widgetContextMenu = tmpContext;
			if (tmpContext == null) {
				LOGGER.error("Allocation Error");
				return;
			}
			// get the button widget:
			Vector2f newPosition;
			Widget eventFromWidget = ememory::dynamicPointerCast<Widget>(caller);
			if (eventFromWidget != null) {
				Vector2f tmpOri  = eventFromWidget.getOrigin();
				Vector2f tmpSize = eventFromWidget.getSize();
				// calculate the correct position
				newPosition.setValue(tmpOri.x() + tmpSize.x()/2,
				                     tmpOri.y() );
			}
			tmpContext.setPositionMark(ewol::widget::ContextMenu::markTop, newPosition);
			ewol::widget::Sizer mySizer;
			ewol::widget::Button myButton;
			mySizer = ewol::widget::Sizer::create();
			if (mySizer != null) {
				mySizer.propertyMode.set(widget::Sizer::modeVert);
				mySizer.propertyLockExpand.set(Vector2f(true,true));
				mySizer.propertyFill.set(Vector2f(true,true));
				// set it in the pop-up-system:
				tmpContext.setSubWidget(mySizer);
				boolean menuHaveImage = false;
				for (auto it2 : this.listElement) {
					if (it.this.localId != it2.this.parentId) {
						continue;
					}
					if (it2.this.image.size()!=0) {
						menuHaveImage = true;
						break;
					}
				}
				for (long iii=this.listElement.size()-1; iii>=0; --iii) {
					if (it.this.localId != this.listElement[iii].this.parentId) {
						continue;
					}
					if (this.listElement[iii].this.message == "" LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.listElement[iii].this.label == "") {
						ewol::widget::Spacer mySpacer = ewol::widget::Spacer::create();
						if (mySpacer == null) {
							LOGGER.error("Allocation spacer error");
							continue;
						}
						mySpacer.propertyExpand.set(Vector2b(true,true));
						mySpacer.propertyFill.set(Vector2b(true,true));
						mySpacer.propertyMinSize.set(gale::Dimension(Vector2f(0,2), gale::distance::pixel));
						mySpacer.propertyMaxSize.set(gale::Dimension(Vector2f(10000,2), gale::distance::pixel));
						mySpacer.propertyColor.set(etk::Color<>(0,0,0,0xFF));
						// add it in the widget list
						mySizer.subWidgetAdd(mySpacer);
					} else {
						myButton = ewol::widget::Button::create();
						if (myButton == null) {
							LOGGER.error("Allocation Error");
							continue;
						}
						myButton.propertyExpand.set(Vector2b(true,true));
						myButton.propertyFill.set(Vector2b(true,true));
						// set callback
						myButton.signalPressed.connect(sharedFromThis(), ewol::widget::Menu::onButtonPressed, ewol::widget::ButtonWeak(myButton));
						// add it in the widget list
						mySizer.subWidgetAdd(myButton);
						if (this.listElement[iii].this.image.size() != 0) {
							String composeString;
							composeString+= "    <sizer mode='hori' expand='true,false' fill='true,true' lock='true'>\n";
							if (etk::end_with(this.listElement[iii].this.image, ".edf") == true) {
								composeString+="        <image src='" + this.listElement[iii].this.image + "' size='8,8mm' distance-field='true'/>\n";
							} else {
								composeString+="        <image src='" + this.listElement[iii].this.image + "' size='8,8mm'/>\n";
							}
							composeString+="        <label exand='true,true' fill='true,true'><left>" + this.listElement[iii].this.label + "</left></label>\n";
							composeString+="    </sizer>\n";
							myButton.setSubWidget(ewol::widget::composerGenerateString(composeString));
						} else {
							if (menuHaveImage == true) {
								myButton.setSubWidget(ewol::widget::composerGenerateString(
								        String() +
								        "	<sizer mode='hori' expand='true,false' fill='true,true' lock='true'>\n"
								        "		<spacer min-size='8,0mm'/>\n"
								        "		<label exand='true,true' fill='true,true'><![CDATA[<left>" + this.listElement[iii].this.label + "</left>]]></label>\n"
								        "	</sizer>\n")
								    );
							} else {
								ewol::widget::Label tmpLabel = widget::Label::create();
								if (tmpLabel != null) {
									tmpLabel.propertyValue.set(String("<left>") + this.listElement[iii].this.label + "</left>\n");
									tmpLabel.propertyExpand.set(Vector2b(true,false));
									tmpLabel.propertyFill.set(Vector2b(true,true));
									myButton.setSubWidget(tmpLabel);
								}
							}
						}
						this.listElement[iii].this.widgetPointer = myButton;
					}
				}
			}
			ewol::widget::Windows currentWindows = getWindows();
			if (currentWindows == null) {
				LOGGER.error("Can not get the curent Windows...");
			} else {
				currentWindows.popUpWidgetPush(tmpContext);
			}
			return;
		}
	}
	/*
	 * 
boolean ewol::widget::Menu::loadXML( exml::Element _node) {
	if (_node.exist() == false) {
		return false;
	}
	// parse generic properties:
	Widget::loadXML(_node);
	// parse all the elements :
	for ( auto nodeIt : _node.nodes) {
		 exml::Element pNode = nodeIt.toElement();
		if (pNode.exist() == false) {
			// trash here all that is not element
			continue;
		}
		String widgetName = pNode.getValue();
		LOGGER.info("Get node : " + pNode);
		if (widgetName == "elem") {
			// <elem title="_T{Title of the button}" image="DATA:///List.svg" event="menu:exit">
			int idMenu = addTitle(pNode.attributes["title"], pNode.attributes["image"], pNode.attributes["event"]);
			for ( auto nodeIt2 : pNode.nodes) {
				
				 exml::Element pNode2 = nodeIt2.toElement();
				if (pNode2.exist() == false) {
					// trash here all that is not element
					continue;
				}
				String widgetName2 = pNode2.getValue();
				if (widgetName2 == "elem") {
					// <elem title="_T{Title of the button}" image="DATA:///List.svg" event="menu:exit">
					add(idMenu, pNode2.attributes["title"], pNode2.attributes["image"], pNode2.attributes["event"]);
				} else if (widgetName2 == "separator") {
					addSpacer(idMenu);
				} else {
					LOGGER.error("[" + getId() + "] {" + getObjectType() + "} (l " + pNode2.getPos() + ") Unknown basic node='" + widgetName2 + "' not in : [elem,separator]" );
				}
			}
		} else if (widgetName == "separator") {
			addSpacer();
		} else {
			LOGGER.error("[" + getId() + "] {" + getObjectType() + "} (l " + pNode.getPos() + ") Unknown basic node='" + widgetName + "' not in : [elem,separator]" );
		}
	}
	return true;
}
	 */
};
	