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

appl::MainWindows::MainWindows() :
  m_gravity(ewol::gravity_buttomLeft),
  m_idWidget(-1) {
	APPL_DEBUG("CREATE WINDOWS ... ");
	addObjectType("appl::MainWindows");
}

void appl::MainWindows::init() {
	ewol::widget::Windows::init();
	
	m_composer = ewol::widget::Composer::create();
	m_composer->loadFromFile("DATA:gui.xml");
	setSubWidget(m_composer);
	externSubBind(m_composer, ewol::widget::Button, "appl-past", signalPressed, sharedFromThis(), &appl::MainWindows::onCallbackCopy);
	externSubBind(m_composer, ewol::widget::Button, "appl-copy", signalPressed, sharedFromThis(), &appl::MainWindows::onCallbackPast);
	propertySetOnWidgetNamed("appl-entry-clipboard", "value", "Hello");
	
	propertySetOnWidgetNamed("appl-last-key-pressed", "value", "plop");
	propertySetOnWidgetNamed("appl-special-key-value", "value", "plouf");
	propertySetOnWidgetNamed("appl-mouse-value", "value", "patapouf");
	
	externSubBind(m_composer, ewol::widget::Button, "appl-set-title", signalPressed, sharedFromThis(), &appl::MainWindows::onCallbackSetTitle);
	externSubBind(m_composer, ewol::widget::Button, "appl-close", signalPressed, sharedFromThis(), &appl::MainWindows::onCallbackClose);
	externSubBind(m_composer, ewol::widget::Button, "appl-size", signalPressed, sharedFromThis(), &appl::MainWindows::onCallbackSize);
	externSubBind(m_composer, ewol::widget::Button, "appl-move", signalPressed, sharedFromThis(), &appl::MainWindows::onCallbackMove);
	
	externSubBind(m_composer, ewol::widget::Button, "appl-full-screen", signalValue, sharedFromThis(), &appl::MainWindows::onCallbackFullScreen);
	
	//shortCutAdd("F12", "menu:reloade-shader");
	//signalShortcut.connect(sharedFromThis(), &appl::MainWindows::onCallbackShortCut);
}

void appl::MainWindows::onCallbackCopy() {
	//propertySetOnWidgetNamed("appl-next-gravity-label", "value", "Next gravity<br/>(" + ewol::gravityToString(m_gravity) + ")");
	APPL_INFO("copy");
}

void appl::MainWindows::onCallbackPast() {
	APPL_INFO("past");
	
}
void appl::MainWindows::onCallbackSetTitle() {
	APPL_INFO("title");
	
}
void appl::MainWindows::onCallbackClose() {
	APPL_INFO("close");
	
}
void appl::MainWindows::onCallbackSize() {
	APPL_INFO("size");
	
}
void appl::MainWindows::onCallbackMove() {
	APPL_INFO("move");
	
}

void appl::MainWindows::onCallbackFullScreen(const bool& _value) {
	APPL_INFO("full screen: " << _value);
	
}


