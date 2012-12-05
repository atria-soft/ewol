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
	widget::Spacer* mySpacer = new widget::Spacer();
	if (NULL != mySpacer) {
		mySpacer->SetExpendX(false);
		mySpacer->SetExpendY(false);
		mySpacer->SetFillX(true);
		mySpacer->SetFillY(false);
		mySpacer->SetSize(10);
		mySpacer->SetColor(0xFF000080);
		SubWidgetAdd(mySpacer);
	}
	
	mySizerHori = new widget::SizerHori();
	if (NULL == mySizerHori) {
		APPL_DEBUG("Allocation error mySizerHori");
		return;
	}
	SubWidgetAdd(mySizerHori);
	
		mySpacer = new widget::Spacer();
		if (NULL != mySpacer) {
			mySpacer->SetExpendX(false);
			mySpacer->SetExpendY(false);
			mySpacer->SetFillX(false);
			mySpacer->SetFillY(true);
			mySpacer->SetSize(10);
			mySpacer->SetColor(0x00FF0080);
			mySizerHori->SubWidgetAdd(mySpacer);
		}
		
		m_testWidget = new widget::Button("My <font color=\"#FF0000\">Button</font> <br/> And Some under line<br/> plop <br/> and an other super long line ...");
		if (NULL != m_testWidget) {
			m_testWidget->SetExpendX(false);
			m_testWidget->SetExpendY(false);
			m_testWidget->SetFillX(false);
			m_testWidget->SetFillY(false);
			m_testWidget->RegisterOnEvent(this, ewolEventButtonPressed);
			m_testWidget->RegisterOnEvent(this, ewolEventButtonDown);
			m_testWidget->RegisterOnEvent(this, ewolEventButtonUp);
			m_testWidget->RegisterOnEvent(this, ewolEventButtonEnter);
			m_testWidget->RegisterOnEvent(this, ewolEventButtonLeave);
			m_testWidget->RegisterOnEvent(this, ewolEventButtonValue);
			mySizerHori->SubWidgetAdd(m_testWidget);
		}
		
		mySpacer = new widget::Spacer();
		if (NULL != mySpacer) {
			mySpacer->SetExpendX(false);
			mySpacer->SetExpendY(false);
			mySpacer->SetFillX(false);
			mySpacer->SetFillY(true);
			mySpacer->SetSize(10);
			mySpacer->SetColor(0x0000FF80);
			mySizerHori->SubWidgetAdd(mySpacer);
		}
		
	mySpacer = new widget::Spacer();
	if (NULL != mySpacer) {
		mySpacer->SetExpendX(false);
		mySpacer->SetExpendY(false);
		mySpacer->SetFillX(true);
		mySpacer->SetFillY(false);
		mySpacer->SetSize(10);
		mySpacer->SetColor(0x00FFFF80);
		SubWidgetAdd(mySpacer);
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
	if (m_testWidget == CallerObject) {
		APPL_WARNING("Receive Event from tested Button ... : \"" << eventId << "\" ==> data=\"" << data << "\"" );
	}
	if (eventId == l_eventChangeExpendX) {
		if (NULL!=m_testWidget) {
			if (data=="1") {
				m_testWidget->SetExpendX(true);
			} else {
				m_testWidget->SetExpendX(false);
			}
		}
	} else if (eventId == l_eventChangeExpendY) {
		if (NULL!=m_testWidget) {
			if (data=="1") {
				m_testWidget->SetExpendY(true);
			} else {
				m_testWidget->SetExpendY(false);
			}
		}
	} else if (eventId == l_eventChangeFillX) {
		if (NULL!=m_testWidget) {
			if (data=="1") {
				m_testWidget->SetFillX(true);
			} else {
				m_testWidget->SetFillX(false);
			}
		}
	} else if (eventId == l_eventChangeFillY) {
		if (NULL!=m_testWidget) {
			if (data=="1") {
				m_testWidget->SetFillY(true);
			} else {
				m_testWidget->SetFillY(false);
			}
		}
	} else if (eventId == l_eventChangeToggle) {
		if (NULL!=m_testWidget) {
			if (data=="1") {
				m_testWidget->SetToggleMode(true);
			} else {
				m_testWidget->SetToggleMode(false);
			}
		}
	} else if (eventId == l_eventChangeTextToggle) {
		if (NULL!=m_testWidget) {
			if (data=="1") {
				m_testWidget->SetLabelToggle("A stupid very long text on toggle <br/><br/> and on multiple lines");
			} else {
				m_testWidget->SetLabelToggle("");
			}
		}
	} else if (eventId == l_eventChangeImage) {
		if (NULL!=m_testWidget) {
			if (data=="1") {
				m_testWidget->SetImage("THEME:GUI:icon.png");
				m_testWidget->SetImageSize(50);
			} else {
				m_testWidget->SetImage("");
			}
		}
	} else if (eventId == l_eventChangeImageToggle) {
		if (NULL!=m_testWidget) {
			if (data=="1") {
				m_testWidget->SetImageToggle("THEME:GUI:icon.png");
			} else {
				m_testWidget->SetImageToggle("");
			}
		}
	} else if (eventId == l_eventChangeText) {
		if (NULL!=m_testWidget) {
			static int32_t countTextID = 1;
			switch (countTextID%10)
			{
				case 0:
					m_testWidget->SetLabel("simple Text");
					break;
				case 1:
					m_testWidget->SetLabel("<left>Align Left</left>");
					break;
				case 2:
					m_testWidget->SetLabel("<right>Align right</right>");
					break;
				case 3:
					m_testWidget->SetLabel("simple Text<br/> With Some Other Lines<br/> and more if you want ...<br/> plop");
					break;
				case 4:
					m_testWidget->SetLabel("simple <bold>Text</bold> with bold");
					break;
				case 5:
					m_testWidget->SetLabel("simple <italic>Text</italic> with italic");
					break;
				case 6:
					m_testWidget->SetLabel("simple <italic><bold>Text</bold></italic> with italic bold");
					break;
				case 7:
					m_testWidget->SetLabel("");
					break;
				case 8:
					m_testWidget->SetLabel("simple <font color=\"#FFFF0088\">Text</font> with colored text");
					break;
				default:
					m_testWidget->SetLabel("My <font color=\"#FF0000\">Button</font> <br/> And Some under line<br/> plop <br/> and an other super long line ...");
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
	if (m_testWidget == removeObject) {
		m_testWidget = NULL;
	}
}
