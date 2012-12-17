/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <appl/Debug.h>
#include <appl/MainWindows.h>

#include <ewol/widget/Button.h>
#include <ewol/widget/CheckBox.h>
#include <ewol/widget/SizerHori.h>
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
#include <appl/TestButton.h>
#include <appl/TestButtonColor.h>
#include <appl/TestLabel.h>
#include <appl/TestScene.h>


static const char * l_eventChangeTheme           = "event-change-theme";
static const char * l_eventChangeWidgetNext      = "event-change-widget-test-next";
static const char * l_eventChangeWidgetPrevious  = "event-change-widget-test-previous";


#undef __class__
#define __class__	"MainWindows"

MainWindows::MainWindows(void) :
	m_idWidget(0),
	m_subWidget(NULL)
{
	APPL_DEBUG("CREATE WINDOWS ... ");
	widget::SizerHori* mySizerHori = NULL;
	widget::Button*    myButton = NULL;
	
	m_sizerVert = new widget::SizerVert();
	if (NULL == m_sizerVert) {
		APPL_DEBUG("Allocation error mySizerVert");
		return;
	}
	SetSubWidget(m_sizerVert);
		
		mySizerHori = new widget::SizerHori();
		if (NULL == mySizerHori) {
			APPL_DEBUG("Allocation error mySizerHori");
			return;
		}
		m_sizerVert->SubWidgetAdd(mySizerHori);
			myButton = new widget::Button("default theme (cube)");
			if (NULL != myButton) {
				myButton->SetToggleMode(true);
				myButton->SetLabelToggle("rounded theme");
				myButton->RegisterOnEvent(this, ewolEventButtonValue, l_eventChangeTheme);
				mySizerHori->SubWidgetAdd(myButton);
			}
			myButton = new widget::Button("Previous Widget test");
			if (NULL != myButton) {
				myButton->RegisterOnEvent(this, ewolEventButtonPressed, l_eventChangeWidgetPrevious);
				mySizerHori->SubWidgetAdd(myButton);
			}
			myButton = new widget::Button("Next Widget test");
			if (NULL != myButton) {
				myButton->RegisterOnEvent(this, ewolEventButtonPressed, l_eventChangeWidgetNext);
				mySizerHori->SubWidgetAdd(myButton);
			}
		widget::Spacer* mySpacer = new widget::Spacer();
		if (NULL != mySpacer) {
			mySpacer->SetExpendX(false);
			mySpacer->SetExpendY(false);
			mySpacer->SetFillX(true);
			mySpacer->SetFillY(false);
			mySpacer->SetSize(3);
			mySpacer->SetColor(0x000000FF);
			m_sizerVert->SubWidgetAdd(mySpacer);
		}
		// add the basic widget with a strange methode ...
		OnReceiveMessage(NULL, NULL, "");
}


MainWindows::~MainWindows(void)
{
	
}


void MainWindows::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
{
	if(    CallerObject != this
	    && CallerObject != NULL) {
		ewol::Windows::OnReceiveMessage(CallerObject, eventId, data);
	}
	APPL_INFO("Receive Event from the main windows ... : \"" << eventId << "\" ==> data=\"" << data << "\"" );
	if (eventId == l_eventChangeTheme) {
		if (data=="1") {
			etk::theme::SetName("GUI", "rounded");
		} else {
			etk::theme::SetName("GUI", "default");
		}
		// Reload shaders and graphic system ...
		ewol::resource::ReLoadResources();
		return;
	} else if (eventId == l_eventChangeWidgetPrevious) {
		m_idWidget--;
	} else if (eventId == l_eventChangeWidgetNext) {
		m_idWidget++;
	}
	
	if (m_subWidget != NULL) {
		delete (m_subWidget);
		// inn theory it must be removed ...
		m_subWidget = NULL;
	}
	// special init forcing ...
	if(CallerObject == NULL) {
		m_idWidget = 4;
	}
	switch(m_idWidget)
	{
		default:
			m_idWidget = 0;
		case 0:
			m_subWidget = (ewol::Widget*)new widget::Label("Test software for EWOL");
			if (NULL != m_subWidget) {
				m_subWidget->SetExpendX(true);
				m_subWidget->SetExpendY(true);
				m_sizerVert->SubWidgetAdd(m_subWidget);
			}
			break;
		case 1:
			m_subWidget = (ewol::Widget*)new TestButton();
			if (NULL != m_subWidget) {
				m_sizerVert->SubWidgetAdd(m_subWidget);
			}
			break;
		case 2:
			m_subWidget = (ewol::Widget*)new TestButtonColor();
			if (NULL != m_subWidget) {
				m_sizerVert->SubWidgetAdd(m_subWidget);
			}
			break;
		case 3:
			m_subWidget = (ewol::Widget*)new TestLabel();
			if (NULL != m_subWidget) {
				m_sizerVert->SubWidgetAdd(m_subWidget);
			}
			break;
		case 4:
			m_subWidget = (ewol::Widget*)new TestScene();
			if (NULL != m_subWidget) {
				m_sizerVert->SubWidgetAdd(m_subWidget);
			}
			break;
	}
}

void MainWindows::OnObjectRemove(ewol::EObject * removeObject)
{
	ewol::Windows::OnObjectRemove(removeObject);
	if (m_subWidget == removeObject) {
		m_subWidget = NULL;
	} else if (m_sizerVert == removeObject) {
		m_sizerVert = NULL;
	}
}
