/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license BSD v3 (see license file)
 */


#include <appl/debug.hpp>
#include <appl/MainWindows.hpp>

#include <ewol/widget/Button.hpp>
#include <ewol/widget/CheckBox.hpp>
#include <ewol/widget/Sizer.hpp>
#include <ewol/widget/Label.hpp>
#include <ewol/widget/Entry.hpp>
#include <ewol/widget/List.hpp>
#include <ewol/widget/ContextMenu.hpp>
#include <ewol/widget/PopUp.hpp>
#include <ewol/widget/Slider.hpp>
#include <ewol/widget/Menu.hpp>
#include <ewol/widget/meta/FileChooser.hpp>
#include <ewol/widget/meta/Parameter.hpp>
#include <ewol/widget/Select.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/widget/Spin.hpp>
#include <ewol/context/Context.hpp>
#include <appl/TestDistanceField.hpp>
#include <etk/os/FSNode.hpp>
#include <eproperty/Value.hpp>

appl::MainWindows::MainWindows() {
	APPL_DEBUG("CREATE WINDOWS ... ");
	addObjectType("appl::MainWindows");
}

void appl::MainWindows::init() {
	ewol::widget::Windows::init();
	propertyFill.set(bvec2(true, true));
	propertyExpand.set(bvec2(true, true));
	m_composer = ewol::widget::Composer::create();
	m_composer->loadFromFile("DATA:gui.xml");
	setSubWidget(m_composer);
	externSubBind(m_composer, ewol::widget::Button, "appl-past", signalPressed, sharedFromThis(), &appl::MainWindows::onCallbackPast);
	externSubBind(m_composer, ewol::widget::Button, "appl-copy", signalPressed, sharedFromThis(), &appl::MainWindows::onCallbackCopy);
	
	externSubBind(m_composer, ewol::widget::Button, "appl-set-title", signalPressed, sharedFromThis(), &appl::MainWindows::onCallbackSetTitle);
	externSubBind(m_composer, ewol::widget::Button, "appl-close", signalPressed, sharedFromThis(), &appl::MainWindows::onCallbackClose);
	externSubBind(m_composer, ewol::widget::Button, "appl-size", signalPressed, sharedFromThis(), &appl::MainWindows::onCallbackSize);
	externSubBind(m_composer, ewol::widget::Button, "appl-move", signalPressed, sharedFromThis(), &appl::MainWindows::onCallbackMove);
	
	externSubBind(m_composer, ewol::widget::Button, "appl-full-screen", signalValue, sharedFromThis(), &appl::MainWindows::onCallbackFullScreen);
	m_listConnection.push_back(getObjectManager().periodicCall.connect(this, &appl::MainWindows::forceFocusCall));
	keepFocus();
	//shortCutAdd("F12", "menu:reloade-shader");
	//signalShortcut.connect(sharedFromThis(), &appl::MainWindows::onCallbackShortCut);
}

void appl::MainWindows::onCallbackCopy() {
	static int32_t iii = 0;
	APPL_INFO("copy");
	gale::context::clipBoard::set(gale::context::clipBoard::clipboardStd, "bonjour, injection in the clipboard " + etk::to_string(iii++) );
}

void appl::MainWindows::onCallbackPast() {
	APPL_INFO("past");
	gale::context::clipBoard::request(gale::context::clipBoard::clipboardStd);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::string value = gale::context::clipBoard::get(gale::context::clipBoard::clipboardStd);
	APPL_INFO("past : '" << value << "'");
	propertySetOnWidgetNamed("appl-entry-clipboard", "value", value);
}
void appl::MainWindows::onCallbackSetTitle() {
	static int32_t iii = 0;
	APPL_INFO("title");
	gale::getContext().setTitle("set title " + etk::to_string(iii++));
}
void appl::MainWindows::onCallbackClose() {
	APPL_INFO("close");
	gale::getContext().stop();
}
void appl::MainWindows::onCallbackSize() {
	APPL_INFO("size");
	vec2 size = gale::getContext().getSize();
	if (size.x() > 800) {
		gale::getContext().setSize(vec2(450,300));
	} else {
		gale::getContext().setSize(size + vec2(100,100));
	}
}
void appl::MainWindows::onCallbackMove() {
	APPL_INFO("move");
	vec2 pos = gale::getContext().getPos();
	if (pos.x() > 300) {
		gale::getContext().setPos(vec2(0,0));
	} else {
		gale::getContext().setPos(pos + vec2(100,100));
	}
	
}

void appl::MainWindows::onCallbackFullScreen(const bool& _value) {
	APPL_INFO("full screen: " << _value);
	gale::getContext().setFullScreen(_value);
}

bool appl::MainWindows::onEventInput(const ewol::event::Input& _event) {
	APPL_INFO("get Event: " << _event);
	propertySetOnWidgetNamed("appl-special-key-value", "value", etk::to_string(_event.getSpecialKey()));
	propertySetOnWidgetNamed("appl-mouse-value", "value", etk::to_string(_event.getType()) + " " + etk::to_string(_event.getId()) + " " + etk::to_string(_event.getPos()));
	return false;
}

bool appl::MainWindows::onEventEntry(const ewol::event::Entry& _event) {
	APPL_INFO("get Event: " << _event);
	propertySetOnWidgetNamed("appl-special-key-value", "value", etk::to_string(_event.getSpecialKey()));
	propertySetOnWidgetNamed("appl-last-key-pressed", "value", etk::to_string(_event.getType()) + " " + etk::to_string(_event.getStatus()) + " " + etk::to_string(_event.getChar()));
	return false;
}

void appl::MainWindows::forceFocusCall(const ewol::event::Time& _event) {
	keepFocus();
}

