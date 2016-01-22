/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <appl/debug.h>
#include <appl/TestScene.h>

#include <etk/tool.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/CheckBox.h>
#include <ewol/widget/Sizer.h>
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
#include <ewol/widget/Manager.h>

static const char * l_eventAddBox       = "event-add-box";
static const char * l_eventAddSphere    = "event-add-sphere";
static const char * l_eventRotationX    = "event-rotation-X";
static const char * l_eventRotationY    = "event-rotation-Y";
static const char * l_eventRotationZ    = "event-rotation-Z";
static const char * l_eventRotation0    = "event-rotation-0";
static const char * l_eventLunch        = "event-lunch";
static const char * l_eventChangeTimeSpeed4 = "event-speed4";
static const char * l_eventChangeTimeSpeed2 = "event-speed2";
static const char * l_eventChangeTimeSpeed0 = "event-speed0.5";
static const char * l_eventChangeTimeSpeed1 = "event-speed1";
static const char * l_eventUp = "event-up";
static const char * l_eventDown = "event-down";


#undef __class__
#define __class__ "TestScene"

TestScene::TestScene() :
	ewol::widget::Sizer(ewol::widget::Sizer::modeVert)
{
	addObjectType("appl::TestScene");
	/*
	m_ground = new game::Element("DATA:grass.obj");
	if (nullptr != m_ground) {
		m_ground->SetStaticMode(true);
		m_gameEngine.AddElement(m_ground);
	}
	*/
	
	APPL_CRITICAL("Create "__class__" (start)");
	std::shared_ptr<ewol::widget::Sizer> mySizerVert2 = nullptr;
	std::shared_ptr<ewol::widget::Sizer> mySizerHori = nullptr;
	std::shared_ptr<ewol::widget::Button> myButton = nullptr;
	/*
	mySizerHori = new ewol::widget::SizerHori();
	if (nullptr == mySizerHori) {
		APPL_DEBUG("Allocation error mySizerHori");
		return;
	}
	SubWidgetAdd(mySizerHori);
		myButton = new ewol::widget::Button("Add Box");
		if (nullptr != myButton) {
			myButton->RegisterOnEvent(this, ewolEventButtonPressed, l_eventAddBox);
			mySizerHori->SubWidgetAdd(myButton);
		}
		myButton = new ewol::widget::Button("Add Sphere");
		if (nullptr != myButton) {
			myButton->RegisterOnEvent(this, ewolEventButtonPressed, l_eventAddSphere);
			mySizerHori->SubWidgetAdd(myButton);
		}
		myButton = new ewol::widget::Button("Rotation X");
		if (nullptr != myButton) {
			myButton->RegisterOnEvent(this, ewolEventButtonPressed, l_eventRotationX);
			mySizerHori->SubWidgetAdd(myButton);
		}
		myButton = new ewol::widget::Button("Rotation Y"); = nullptr
		if (nullptr != myButton) {
			myButton->RegisterOnEvent(this, ewolEventButtonPressed, l_eventRotationY);
			mySizerHori->SubWidgetAdd(myButton);
		}
		myButton = new ewol::widget::Button("Rotation Z");
		if (nullptr != myButton) {
			myButton->RegisterOnEvent(this, ewolEventButtonPressed, l_eventRotationZ);
			mySizerHori->SubWidgetAdd(myButton);
		}
		myButton = new ewol::widget::Button("Rotation -");
		if (nullptr != myButton) {
			myButton->RegisterOnEvent(this, ewolEventButtonPressed, l_eventRotation0);
			mySizerHori->SubWidgetAdd(myButton);
		}
		myButton = new ewol::widget::Button("UP");
		if (nullptr != myButton) {
			myButton->RegisterOnEvent(this, ewolEventButtonPressed, l_eventUp);
			mySizerHori->SubWidgetAdd(myButton);
		}
		myButton = new ewol::widget::Button("DOWN");
		if (nullptr != myButton) {
			myButton->RegisterOnEvent(this, ewolEventButtonPressed, l_eventDown);
			mySizerHori->SubWidgetAdd(myButton);
		}
	mySizerHori = new ewol::widget::SizerHori();
	if (nullptr == mySizerHori) {
		APPL_DEBUG("Allocation error mySizerHori");
		return;
	}
	SubWidgetAdd(mySizerHori);
		myButton = new ewol::widget::Button("lunch object");
		if (nullptr != myButton) {
			myButton->RegisterOnEvent(this, ewolEventButtonPressed, l_eventLunch);
			mySizerHori->SubWidgetAdd(myButton);
		}
		myButton = new ewol::widget::Button("1x speed");
		if (nullptr != myButton) {
			myButton->RegisterOnEvent(this, ewolEventButtonPressed, l_eventChangeTimeSpeed1);
			mySizerHori->SubWidgetAdd(myButton);
		}
		myButton = new ewol::widget::Button("0.5x speed");
		if (nullptr != myButton) {
			myButton->RegisterOnEvent(this, ewolEventButtonPressed, l_eventChangeTimeSpeed0);
			mySizerHori->SubWidgetAdd(myButton);
		}
		myButton = new ewol::widget::Button("2x speed");
		if (nullptr != myButton) {
			myButton->RegisterOnEvent(this, ewolEventButtonPressed, l_eventChangeTimeSpeed2);
			mySizerHori->SubWidgetAdd(myButton);
		}
		myButton = new ewol::widget::Button("4x speed");
		if (nullptr != myButton) {
			myButton->RegisterOnEvent(this, ewolEventButtonPressed, l_eventChangeTimeSpeed4);
			mySizerHori->SubWidgetAdd(myButton);
		}
	*/
	std::shared_ptr<ewol::widget::Spacer> mySpacer = new widget::Spacer();
	if (nullptr != mySpacer) {
		mySpacer->setExpand(bvec2(false,false));
		mySpacer->setFill(bvec2(true,false));
		mySpacer->setMinSize(vec2(10,10));
		mySpacer->setColor(0xFF000080);
		subWidgetAdd(mySpacer);
	}
	
	mySizerHori = new ewol::widget::Sizer(ewol::widget::Sizer::modeHori);
	if (nullptr == mySizerHori) {
		APPL_DEBUG("Allocation error mySizerHori");
		return;
	}
	subWidgetAdd(mySizerHori);
		mySpacer = new ewol::widget::Spacer();
		if (nullptr != mySpacer) {
			mySpacer->setExpand(bvec2(false,false));
			mySpacer->setFill(bvec2(false,true));
			mySpacer->setMinSize(vec2(10,10));
			mySpacer->setColor(0x00FF0080);
			mySizerHori->subWidgetAdd(mySpacer);
		}
		
		m_testWidget = new ewol::widget::Scene(/*&m_gameEngine*/ nullptr);
		if (nullptr != m_testWidget) {
			m_testWidget->setExpand(bvec2(true,true));
			m_testWidget->setFill(bvec2(true,true));
			mySizerHori->subWidgetAdd(m_testWidget);
		}
		
		mySpacer = new ewol::widget::Spacer();
		if (nullptr != mySpacer) {
			mySpacer->setExpand(bvec2(false,false));
			mySpacer->setFill(bvec2(false,true));
			mySpacer->setMinSize(vec2(10,10));
			mySpacer->setColor(0x0000FF80);
			mySizerHori->subWidgetAdd(mySpacer);
		}
		
	mySpacer = new ewol::widget::Spacer();
	if (nullptr != mySpacer) {
		mySpacer->setExpand(bvec2(false,false));
		mySpacer->setFill(bvec2(true,false));
		mySpacer->setMinSize(vec2(10,10));
		mySpacer->setColor(0x00FFFF80);
		subWidgetAdd(mySpacer);
	}
	APPL_INFO("Create "__class__" (end)");
}


TestScene::~TestScene() {
	APPL_INFO("Remove "__class__" ...");
}

/*
#include <ewol/game/Element.h>

vec3 baseRotationVect;
vec3 baseMove;
class stupidCube : public game::Element
{
	public:
		stupidCube(float poidKg=0.0f) : game::Element("DATA:cube.obj")
		{
			m_mass = poidKg;
		};
		
		// herited methode
		virtual bool ArtificialIntelligence(float delta)
		{
			if (m_mass == 0.0f) {
				if (baseRotationVect != vec3(0,0,0) ) {
					Rotate(baseRotationVect, 0.5 * delta );
				}
				if (baseMove != vec3(0,0,0) ) {
					Translate(baseMove);
					baseMove = vec3(0,0,0);
				}
			}
			return false;
		}
	
};

class stupidSphere : public game::Element
{
	public:
		stupidSphere(float poidKg=0.0f) : game::Element("DATA:sphere.obj")
		{
			m_mass = poidKg;
		};
		
		// herited methode
		virtual bool ArtificialIntelligence(float delta)
		{
			if (m_mass == 0.0f) {
				if (baseRotationVect != vec3(0,0,0) ) {
					Rotate(baseRotationVect, 0.5 * delta );
				}
				if (baseMove != vec3(0,0,0) ) {
					Translate(baseMove);
					baseMove = vec3(0,0,0);
				}
			}
			return false;
		}
	
};
*/






void TestScene::onReceiveMessage(const ewol::object::Message& _msg) {
	ewol::widget::Sizer::onReceiveMessage(_msg);
	/*
	//APPL_INFO("Receive Event from the main windows ... : \"" << eventId << "\" ==> data=\"" << data << "\"" );
	if (m_testWidget == CallerObject) {
		APPL_WARNING("Receive Event from tested Scene ... : \"" << eventId << "\" ==> data=\"" << data << "\"" );
	}
	if (eventId == l_eventAddBox) {
		static bool firstTime = true;
		if (firstTime==false) {
			stupidCube * tmpp = new stupidCube();
			vec3 newPos = vec3(etk::tool::frand(-40,40),etk::tool::frand(-40,40),etk::tool::frand(1,8));
			APPL_DEBUG("add a box at the pos : " << newPos);
			tmpp->Translate(newPos);
			tmpp->Scale(etk::tool::frand(0.5,2) );
			float angle = etk::tool::frand(-M_PI,M_PI);
			tmpp->Rotate(vec3(1,0,0), angle);
			angle = etk::tool::frand(-M_PI,M_PI);
			tmpp->Rotate(vec3(0,1,0), angle);
			angle = etk::tool::frand(-M_PI,M_PI);
			tmpp->Rotate(vec3(0,0,1), angle);
			m_gameEngine.AddElement(tmpp);
		} else {
			firstTime = false;
			stupidCube * tmpp = new stupidCube();
			m_gameEngine.AddElement(tmpp);
		}
	} else if (eventId == l_eventAddSphere) {
		if (nullptr!=m_testWidget) {
			stupidSphere * tmpp = new stupidSphere();
			vec3 newPos = vec3(etk::tool::frand(-40,40),etk::tool::frand(-40,40),etk::tool::frand(1,8));
			APPL_DEBUG("add a box at the pos : " << newPos);
			tmpp->Translate(newPos);
			tmpp->Scale(etk::tool::frand(0.5,2) );
			float angle = etk::tool::frand(-M_PI,M_PI);
			tmpp->Rotate(vec3(1,0,0), angle);
			angle = etk::tool::frand(-M_PI,M_PI);
			tmpp->Rotate(vec3(0,1,0), angle);
			angle = etk::tool::frand(-M_PI,M_PI);
			tmpp->Rotate(vec3(0,0,1), angle);
			m_gameEngine.AddElement(tmpp);
		}
	} else if (eventId == l_eventUp) {
		baseMove = vec3(0.1,0,0);
	} else if (eventId == l_eventDown) {
		baseMove = vec3(-0.1,0,0);
	} else if (eventId == l_eventRotationX) {
		baseRotationVect = vec3(1,0,0);
	} else if (eventId == l_eventRotationY) {
		baseRotationVect = vec3(0,1,0);
	} else if (eventId == l_eventRotationZ) {
		baseRotationVect = vec3(0,0,1);
	} else if (eventId == l_eventRotation0) {
		baseRotationVect = vec3(0,0,0);
	} else if (eventId == l_eventLunch) {
		stupidCube * tmpp = new stupidCube(250);
		vec3 newPos = vec3(5,5,100);
		tmpp->Translate(newPos);
		tmpp->SetSpeed(vec3(10,10,50));
		m_gameEngine.AddElement(tmpp);
	} else if (eventId == l_eventChangeTimeSpeed1) {
		if (nullptr!=m_testWidget) {
			m_testWidget->SetRatioTime(1);
		}
	} else if (eventId == l_eventChangeTimeSpeed0) {
		if (nullptr!=m_testWidget) {
			m_testWidget->SetRatioTime(0.5);
		}
	} else if (eventId == l_eventChangeTimeSpeed2) {
		if (nullptr!=m_testWidget) {
			m_testWidget->SetRatioTime(2);
		}
	} else if (eventId == l_eventChangeTimeSpeed4) {
		if (nullptr!=m_testWidget) {
			m_testWidget->SetRatioTime(4);
		}
	}
	*/
	
	return;
}

