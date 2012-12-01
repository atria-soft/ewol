/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <appl/Debug.h>
#include <appl/TestButtonColor.h>

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

#undef __class__
#define __class__	"TestButton"

TestButtonColor::TestButtonColor(void)
{
	APPL_DEBUG("CREATE "__class__" ... ");
	widget::SizerVert* mySizerVert2 = NULL;
	widget::SizerHori* mySizerHori = NULL;
	widget::Button*    myButton = NULL;
	
	mySizerHori = new widget::SizerHori();
	if (NULL == mySizerHori) {
		APPL_DEBUG("Allocation error mySizerHori");
		return;
	}
	SubWidgetAdd(mySizerHori);
		myButton = new widget::Button("Expend X (false)");
		if (NULL != myButton) {
			myButton->SetToggleMode(true);
			myButton->SetLabelToggle("Expend X (true)");
			myButton->RegisterOnEvent(this, ewolEventButtonValue, l_eventChangeExpendX);
			mySizerHori->SubWidgetAdd(myButton);
		}
		myButton = new widget::Button("Expend Y (false)");
		if (NULL != myButton) {
			myButton->SetToggleMode(true);
			myButton->SetLabelToggle("Expend Y (true)");
			myButton->RegisterOnEvent(this, ewolEventButtonValue, l_eventChangeExpendY);
			mySizerHori->SubWidgetAdd(myButton);
		}
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
		
		m_button = new widget::ButtonColor(draw::color::olive);
		if (NULL != m_button) {
			m_button->SetExpendX(false);
			m_button->SetExpendY(false);
			m_button->SetFillX(false);
			m_button->SetFillY(false);
			m_button->RegisterOnEvent(this, ewolEventButtonColorChange);
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
}


TestButtonColor::~TestButtonColor(void)
{
	
}


void TestButtonColor::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
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
	}
	
	return;
}

void TestButtonColor::OnObjectRemove(ewol::EObject * removeObject)
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
