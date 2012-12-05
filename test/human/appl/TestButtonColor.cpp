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
		
		m_testWidget = new widget::ButtonColor(draw::color::olive);
		if (NULL != m_testWidget) {
			m_testWidget->SetExpendX(false);
			m_testWidget->SetExpendY(false);
			m_testWidget->SetFillX(false);
			m_testWidget->SetFillY(false);
			m_testWidget->RegisterOnEvent(this, ewolEventButtonColorChange);
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
}


TestButtonColor::~TestButtonColor(void)
{
	
}


void TestButtonColor::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
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
	}
	
	return;
}

void TestButtonColor::OnObjectRemove(ewol::EObject * removeObject)
{
	widget::SizerVert::OnObjectRemove(removeObject);
	if (m_testWidget == removeObject) {
		m_testWidget = NULL;
	}
}
