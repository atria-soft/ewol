/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/meta/stdPopUp.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/SizerHori.h>
#include <ewol/widget/SizerVert.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/WidgetManager.h>
#include <etk/Vector.h>

#undef __class__
#define __class__ "ewol::StdPopUp"

extern const char * const ewolEventFileStdPopUpCancel   = "ewol event std_pop_up cancel";
extern const char * const ewolEventFileStdPopUpValidate = "ewol event std_pop_up validate";
extern const char * const ewolEventFileStdPopUpButton1 = "ewol event std_pop_up BT1";
extern const char * const ewolEventFileStdPopUpButton2 = "ewol event std_pop_up BT2";
extern const char * const ewolEventFileStdPopUpButton3 = "ewol event std_pop_up BT3";
extern const char * const ewolEventFileStdPopUpButton4 = "ewol event std_pop_up BT4";

widget::StdPopUp::StdPopUp(void) {
	addEventId(ewolEventFileStdPopUpCancel);
	addEventId(ewolEventFileStdPopUpValidate);
	addEventId(ewolEventFileStdPopUpButton1);
	addEventId(ewolEventFileStdPopUpButton2);
	addEventId(ewolEventFileStdPopUpButton3);
	addEventId(ewolEventFileStdPopUpButton4);
	
	m_widgetTitleId = -1;
	m_widgetValidateId = -1;
	m_widgetCancelId = -1;
	
	
	ewol::sizerVert * mySizerVert = NULL;
	ewol::sizerHori * mySizerHori = NULL;
	ewol::Button * myButton = NULL;
	ewol::Entry * myEntry = NULL;
	ewol::Spacer * mySpacer = NULL;
	fileChooserFileList * myListFile = NULL;
	fileChooserFolderList * myListFolder = NULL;
	ewol::Label * myLabel = NULL;
	
	mySizerVert = new ewol::sizerVert();
		// set it in the pop-up-system : 
		subWidgetSet(mySizerVert);
		
		myLabel = new ewol::Label("File chooser ...");
			m_widgetTitleId = myLabel->getWidgetId();
			mySizerVert->subWidgetAdd(myLabel);
		
		mySizerHori = new ewol::sizerHori();
			mySizerHori->lockExpendContamination(true);
			mySizerVert->subWidgetAdd(mySizerHori);
			myButton = new ewol::Button("<-");
				myButton->externLinkOnEvent("ewol Button Pressed", getWidgetId(), ewolEventFileChooserFolderUp );
				mySizerHori->subWidgetAdd(myButton);
			myEntry = new ewol::Entry("~/");
				m_widgetCurrentFolderId = myEntry->getWidgetId();
				myEntry->setExpendX(true);
				myEntry->setFillX(true);
				myEntry->setWidth(200);
				mySizerHori->subWidgetAdd(myEntry);
			
		mySizerHori = new ewol::sizerHori();
			mySizerHori->lockExpendContamination(true);
			mySizerVert->subWidgetAdd(mySizerHori);
			myListFolder = new fileChooserFolderList();
				m_widgetListFolderId = myListFolder->getWidgetId();
				//myList->setExpendX(true);
				myListFolder->setExpendY(true);
				myListFolder->setFillY(true);
				mySizerHori->subWidgetAdd(myListFolder);
			myListFile = new fileChooserFileList();
				m_widgetListFileId = myListFile->getWidgetId();
				myListFile->setExpendY(true);
				myListFile->setFillX(true);
				myListFile->setExpendY(true);
				myListFile->setFillY(true);
				mySizerHori->subWidgetAdd(myListFile);
			
		mySizerHori = new ewol::sizerHori();
			mySizerHori->lockExpendContamination(true);
			mySizerVert->subWidgetAdd(mySizerHori);
			mySpacer = new ewol::Spacer();
				mySpacer->setExpendX(true);
				mySizerHori->subWidgetAdd(mySpacer);
			myButton = new ewol::Button("Open");
				m_widgetValidateId = myButton->getWidgetId();
				myButton->externLinkOnEvent("ewol Button Pressed", getWidgetId(), ewolEventFileChooserValidate);
				mySizerHori->subWidgetAdd(myButton);
			myButton = new ewol::Button("Cancel");
				m_widgetCancelId = myButton->getWidgetId();
				myButton->externLinkOnEvent("ewol Button Pressed", getWidgetId(), ewolEventFileChooserCancel);
				mySizerHori->subWidgetAdd(myButton);
}

widget::StdPopUp::~StdPopUp(void) {
	
}

void widget::StdPopUp::setTitle(etk::UString _label) {
	ewol::Label * tmpWidget = (ewol::Label*)ewol::widgetManager::get(m_widgetTitleId);
	if (NULL == tmpWidget) {
		return;
	}
	tmpWidget->setLabel(_label);
}

void widget::StdPopUp::setValidateLabel(etk::UString _label) {
	ewol::Button * tmpWidget = (ewol::Button*)ewol::widgetManager::get(m_widgetValidateId);
	if (NULL == tmpWidget) {
		return;
	}
	tmpWidget->setLabel(_label);
}

void widget::StdPopUp::setCancelLabel(etk::UString _label) {
	ewol::Button * tmpWidget = (ewol::Button*)ewol::widgetManager::get(m_widgetCancelId);
	if (NULL == tmpWidget) {
		return;
	}
	tmpWidget->setLabel(_label);
}

void widget::StdPopUp::setFolder(etk::UString _folder) {
	m_folder = _folder;
}

bool widget::StdPopUp::onEventAreaExternal(int32_t _widgetID, const char *_generateEventId, const char *_eventExternId, float _x, float _y) {
	EWOL_INFO("Receive Event from the BT ... : widgetid=" << _widgetID << "\"" << _generateEventId << "\"  == > internalEvent=\"" << _eventExternId << "\"" );
	if (ewolEventFileChooserCancel == _eventExternId) {
		// == > Auto remove ...
		
	}
	return genEventInputExternal(_eventExternId, _x, _y);
}
