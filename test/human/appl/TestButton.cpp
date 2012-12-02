/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <appl/Debug.h>
#include <appl/TestButton.h>

#include <ewol/widget/Button.h>
#include <ewol/widget/CheckBox.h>
#include <ewol/widget/SizerHori.h>
#include <ewol/widget/SizerVert.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/List.h>
#include <ewol/widget/ContextMenu.h>
#include <ewol/widget/PopUp.h>
#include <ewol/widget/Slider.h>
#include <ewol/widget/Menu.h>
#include <ewol/widget/meta/FileChooser.h>
#include <ewol/widget/meta/Parameter.h>
#include <ewol/widget/WidgetManager.h>


static const char * l_eventChangeExpendX     = "event-change-expend-X";
static const char * l_eventChangeExpendY     = "event-change-expend-Y";
static const char * l_eventChangeFillX       = "event-change-fill-X";
static const char * l_eventChangeFillY       = "event-change-fill-Y";
static const char * l_eventChangeToggle      = "event-change-toggle-mode";
static const char * l_eventChangeText        = "event-change-text";
static const char * l_eventChangeTextToggle  = "event-change-text-toggle";
static const char * l_eventChangeImage       = "event-change-image";
static const char * l_eventChangeImageToggle = "event-change-image-toggle";

#undef __class__
#define __class__	"TestButton"

TestButton::TestButton(void)
{
	APPL_CRITICAL("Create "__class__" (start)");
	widget::SizerVert* mySizerVert2 = NULL;
	widget::SizerHori* mySizerHori = NULL;
	widget::Button*    myButton = NULL;
	
	mySizerHori = new widget::SizerHori();
	if (NULL == mySizerHori) {
		APPL_DEBUG("Allocation error mySizerHori");
		return;
	}
	SubWidgetAdd(mySizerHori);
		myButton = new widget::Button("Expend X <br/> (false)");
		if (NULL != myButton) {
			myButton->SetToggleMode(true);
			myButton->SetLabelToggle("Expend X <br/> (true)");
			myButton->RegisterOnEvent(this, ewolEventButtonValue, l_eventChangeExpendX);
			mySizerHori->SubWidgetAdd(myButton);
		}
		myButton = new widget::Button("Expend Y <br/> (false)");
		if (NULL != myButton) {
			myButton->SetToggleMode(true);
			myButton->SetLabelToggle("Expend Y <br/> (true)");
			myButton->RegisterOnEvent(this, ewolEventButtonValue, l_eventChangeExpendY);
			mySizerHori->SubWidgetAdd(myButton);
		}
		myButton = new widget::Button("Toggle<br/>(false)");
		if (NULL != myButton) {
			myButton->SetToggleMode(true);
			myButton->SetLabelToggle("Toggle<br/><b>(true)</b>");
			myButton->RegisterOnEvent(this, ewolEventButtonValue, l_eventChangeToggle);
			mySizerHori->SubWidgetAdd(myButton);
		}
		myButton = new widget::Button("Text On toggle state<br/>(false)");
		if (NULL != myButton) {
			myButton->SetToggleMode(true);
			myButton->SetLabelToggle("Text On toggle state<br/><b>(true)</b>");
			myButton->RegisterOnEvent(this, ewolEventButtonValue, l_eventChangeTextToggle);
			mySizerHori->SubWidgetAdd(myButton);
		}
	
	mySizerHori = new widget::SizerHori();
	if (NULL == mySizerHori) {
		APPL_DEBUG("Allocation error mySizerHori");
		return;
	}
	SubWidgetAdd(mySizerHori);
		myButton = new widget::Button("Fill X (false)");
		if (NULL != myButton) {
			myButton->SetToggleMode(true);
			myButton->SetLabelToggle("Fill X (true)");
			myButton->RegisterOnEvent(this, ewolEventButtonValue, l_eventChangeFillX);
			mySizerHori->SubWidgetAdd(myButton);
		}
		myButton = new widget::Button("Fill Y (false)");
		if (NULL != myButton) {
			myButton->SetToggleMode(true);
			myButton->SetLabelToggle("Fill Y (true)");
			myButton->RegisterOnEvent(this, ewolEventButtonValue, l_eventChangeFillY);
			mySizerHori->SubWidgetAdd(myButton);
		}
		myButton = new widget::Button("Image (false)");
		if (NULL != myButton) {
			myButton->SetToggleMode(true);
			myButton->SetLabelToggle("Image (true)");
			myButton->RegisterOnEvent(this, ewolEventButtonValue, l_eventChangeImage);
			mySizerHori->SubWidgetAdd(myButton);
		}
		myButton = new widget::Button("Image Toggle (false)");
		if (NULL != myButton) {
			myButton->SetToggleMode(true);
			myButton->SetLabelToggle("Image Toggle (true)");
			myButton->RegisterOnEvent(this, ewolEventButtonValue, l_eventChangeImageToggle);
			mySizerHori->SubWidgetAdd(myButton);
		}
		myButton = new widget::Button("Change Text");
		if (NULL != myButton) {
			myButton->RegisterOnEvent(this, ewolEventButtonPressed, l_eventChangeText);
			mySizerHori->SubWidgetAdd(myButton);
		}
	int32_t idSpacer=0;
	m_spacer[idSpacer] = new widget::Spacer();
	if (NULL != m_spacer[idSpacer]) {
		m_spacer[idSpacer]->SetExpendX(false);
		m_spacer[idSpacer]->SetExpendY(false);
		m_spacer[idSpacer]->SetFillX(true);
		m_spacer[idSpacer]->SetFillY(false);
		m_spacer[idSpacer]->SetSize(10);
		m_spacer[idSpacer]->SetColor(0xFF000080);
		SubWidgetAdd(m_spacer[idSpacer]);
	}
	
	mySizerHori = new widget::SizerHori();
	if (NULL == mySizerHori) {
		APPL_DEBUG("Allocation error mySizerHori");
		return;
	}
	SubWidgetAdd(mySizerHori);
	
		idSpacer++;
		m_spacer[idSpacer] = new widget::Spacer();
		if (NULL != m_spacer[idSpacer]) {
			m_spacer[idSpacer]->SetExpendX(false);
			m_spacer[idSpacer]->SetExpendY(false);
			m_spacer[idSpacer]->SetFillX(false);
			m_spacer[idSpacer]->SetFillY(true);
			m_spacer[idSpacer]->SetSize(10);
			m_spacer[idSpacer]->SetColor(0x00FF0080);
			mySizerHori->SubWidgetAdd(m_spacer[idSpacer]);
		}
		
		m_button = new widget::Button("My <font color=\"#FF0000\">Button</font> <br/> And Some under line<br/> plop <br/> and an other super long line ...");
		if (NULL != m_button) {
			m_button->SetExpendX(false);
			m_button->SetExpendY(false);
			m_button->SetFillX(false);
			m_button->SetFillY(false);
			m_button->RegisterOnEvent(this, ewolEventButtonPressed);
			m_button->RegisterOnEvent(this, ewolEventButtonDown);
			m_button->RegisterOnEvent(this, ewolEventButtonUp);
			m_button->RegisterOnEvent(this, ewolEventButtonEnter);
			m_button->RegisterOnEvent(this, ewolEventButtonLeave);
			m_button->RegisterOnEvent(this, ewolEventButtonValue);
			mySizerHori->SubWidgetAdd(m_button);
		}
		
		idSpacer++;
		m_spacer[idSpacer] = new widget::Spacer();
		if (NULL != m_spacer[idSpacer]) {
			m_spacer[idSpacer]->SetExpendX(false);
			m_spacer[idSpacer]->SetExpendY(false);
			m_spacer[idSpacer]->SetFillX(false);
			m_spacer[idSpacer]->SetFillY(true);
			m_spacer[idSpacer]->SetSize(10);
			m_spacer[idSpacer]->SetColor(0x0000FF80);
			mySizerHori->SubWidgetAdd(m_spacer[idSpacer]);
		}
		
	idSpacer++;
	m_spacer[idSpacer] = new widget::Spacer();
	if (NULL != m_spacer[idSpacer]) {
		m_spacer[idSpacer]->SetExpendX(false);
		m_spacer[idSpacer]->SetExpendY(false);
		m_spacer[idSpacer]->SetFillX(true);
		m_spacer[idSpacer]->SetFillY(false);
		m_spacer[idSpacer]->SetSize(10);
		m_spacer[idSpacer]->SetColor(0x00FFFF80);
		SubWidgetAdd(m_spacer[idSpacer]);
	}
	APPL_CRITICAL("Create "__class__" (end)");
}


TestButton::~TestButton(void)
{
	APPL_CRITICAL("Remove "__class__" ...");
}


void TestButton::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
{
	widget::SizerVert::OnReceiveMessage(CallerObject, eventId, data);
	
	//APPL_INFO("Receive Event from the main windows ... : \"" << eventId << "\" ==> data=\"" << data << "\"" );
	if (m_button == CallerObject) {
		APPL_WARNING("Receive Event from tested Button ... : \"" << eventId << "\" ==> data=\"" << data << "\"" );
	}
	if (eventId == l_eventChangeExpendX) {
		if (NULL!=m_button) {
			if (data=="1") {
				m_button->SetExpendX(true);
			} else {
				m_button->SetExpendX(false);
			}
		}
	} else if (eventId == l_eventChangeExpendY) {
		if (NULL!=m_button) {
			if (data=="1") {
				m_button->SetExpendY(true);
			} else {
				m_button->SetExpendY(false);
			}
		}
	} else if (eventId == l_eventChangeFillX) {
		if (NULL!=m_button) {
			if (data=="1") {
				m_button->SetFillX(true);
			} else {
				m_button->SetFillX(false);
			}
		}
	} else if (eventId == l_eventChangeFillY) {
		if (NULL!=m_button) {
			if (data=="1") {
				m_button->SetFillY(true);
			} else {
				m_button->SetFillY(false);
			}
		}
	} else if (eventId == l_eventChangeToggle) {
		if (NULL!=m_button) {
			if (data=="1") {
				m_button->SetToggleMode(true);
			} else {
				m_button->SetToggleMode(false);
			}
		}
	} else if (eventId == l_eventChangeTextToggle) {
		if (NULL!=m_button) {
			if (data=="1") {
				m_button->SetLabelToggle("A stupid very long text on toggle <br/><br/> and on multiple lines");
			} else {
				m_button->SetLabelToggle("");
			}
		}
	} else if (eventId == l_eventChangeImage) {
		if (NULL!=m_button) {
			if (data=="1") {
				m_button->SetImage("THEME:GUI:icon.png");
				m_button->SetImageSize(50);
			} else {
				m_button->SetImage("");
			}
		}
	} else if (eventId == l_eventChangeImageToggle) {
		if (NULL!=m_button) {
			if (data=="1") {
				m_button->SetImageToggle("THEME:GUI:icon.png");
			} else {
				m_button->SetImageToggle("");
			}
		}
	} else if (eventId == l_eventChangeText) {
		if (NULL!=m_button) {
			static int32_t countTextID = 1;
			switch (countTextID%10)
			{
				case 0:
					m_button->SetLabel("simple Text");
					break;
				case 1:
					m_button->SetLabel("<left>Align Left</left>");
					break;
				case 2:
					m_button->SetLabel("<right>Align right</right>");
					break;
				case 3:
					m_button->SetLabel("simple Text<br/> With Some Other Lines<br/> and more if you want ...<br/> plop");
					break;
				case 4:
					m_button->SetLabel("simple <bold>Text</bold> with bold");
					break;
				case 5:
					m_button->SetLabel("simple <italic>Text</italic> with italic");
					break;
				case 6:
					m_button->SetLabel("simple <italic><bold>Text</bold></italic> with italic bold");
					break;
				case 7:
					m_button->SetLabel("");
					break;
				case 8:
					m_button->SetLabel("simple <font color=\"#FFFF0088\">Text</font> with colored text");
					break;
				default:
					m_button->SetLabel("My <font color=\"#FF0000\">Button</font> <br/> And Some under line<br/> plop <br/> and an other super long line ...");
					break;
			}
			countTextID++;
		}
	}
	
	return;
}

void TestButton::OnObjectRemove(ewol::EObject * removeObject)
{
	widget::SizerVert::OnObjectRemove(removeObject);
	if (m_button == removeObject) {
		m_button = NULL;
	}
	if (m_spacer[0] == removeObject) {
		m_spacer[0] = NULL;
	}
	if (m_spacer[1] == removeObject) {
		m_spacer[1] = NULL;
	}
	if (m_spacer[2] == removeObject) {
		m_spacer[2] = NULL;
	}
	if (m_spacer[3] == removeObject) {
		m_spacer[3] = NULL;
	}
}
