/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <appl/Debug.h>
#include <appl/TestScene.h>

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
#include <ewol/widget/Spacer.h>
#include <ewol/widget/Menu.h>
#include <ewol/widget/meta/FileChooser.h>
#include <ewol/widget/meta/Parameter.h>
#include <ewol/widget/WidgetManager.h>

static const char * l_eventAddBox       = "event-add-box";
static const char * l_eventAddSphere    = "event-add-sphere";
static const char * l_eventRotationX    = "event-rotation-X";
static const char * l_eventRotationY    = "event-rotation-Y";
static const char * l_eventRotationZ    = "event-rotation-Z";




#undef __class__
#define __class__	"TestScene"

TestScene::TestScene(void)
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
		myButton = new widget::Button("Add Box");
		if (NULL != myButton) {
			myButton->RegisterOnEvent(this, ewolEventButtonPressed, l_eventAddBox);
			mySizerHori->SubWidgetAdd(myButton);
		}
		myButton = new widget::Button("Add Sphere");
		if (NULL != myButton) {
			myButton->RegisterOnEvent(this, ewolEventButtonPressed, l_eventAddSphere);
			mySizerHori->SubWidgetAdd(myButton);
		}
		myButton = new widget::Button("Rotation X");
		if (NULL != myButton) {
			myButton->RegisterOnEvent(this, ewolEventButtonPressed, l_eventRotationX);
			mySizerHori->SubWidgetAdd(myButton);
		}
		myButton = new widget::Button("Rotation Y");
		if (NULL != myButton) {
			myButton->RegisterOnEvent(this, ewolEventButtonPressed, l_eventRotationY);
			mySizerHori->SubWidgetAdd(myButton);
		}
		myButton = new widget::Button("Rotation Z");
		if (NULL != myButton) {
			myButton->RegisterOnEvent(this, ewolEventButtonPressed, l_eventRotationZ);
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
		
		m_testWidget = new widget::Scene(&m_gameEngine);
		if (NULL != m_testWidget) {
			m_testWidget->SetExpendX(true);
			m_testWidget->SetExpendY(true);
			m_testWidget->SetFillX(true);
			m_testWidget->SetFillY(true);
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


TestScene::~TestScene(void)
{
	APPL_CRITICAL("Remove "__class__" ...");
}


#include <ewol/game/Element.h>

vec3 baseRotationVect;
class stupidCube : public game::Element
{
	public:
		stupidCube(void) : game::Element("DATA:cube.obj") {};
		
		// herited methode
		virtual bool ArtificialIntelligence(int32_t deltaMicroSecond)
		{
			m_property.Rotate(baseRotationVect, 0.01);
			return false;
		}
	
};








void TestScene::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
{
	widget::SizerVert::OnReceiveMessage(CallerObject, eventId, data);
	
	//APPL_INFO("Receive Event from the main windows ... : \"" << eventId << "\" ==> data=\"" << data << "\"" );
	if (m_testWidget == CallerObject) {
		APPL_WARNING("Receive Event from tested Scene ... : \"" << eventId << "\" ==> data=\"" << data << "\"" );
	}
	if (eventId == l_eventAddBox) {
		stupidCube * tmpp = new stupidCube();
		m_gameEngine.AddElement(tmpp, true);
	} else if (eventId == l_eventAddSphere) {
		if (NULL!=m_testWidget) {
			
		}
	} else if (eventId == l_eventRotationX) {
		baseRotationVect = vec3(1,0,0);
	} else if (eventId == l_eventRotationY) {
		baseRotationVect = vec3(0,1,0);
	} else if (eventId == l_eventRotationZ) {
		baseRotationVect = vec3(0,0,1);
	}
	
	return;
}

void TestScene::OnObjectRemove(ewol::EObject * removeObject)
{
	widget::SizerVert::OnObjectRemove(removeObject);
	if (m_testWidget == removeObject) {
		m_testWidget = NULL;
	}
}
