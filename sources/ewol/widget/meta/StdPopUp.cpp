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
#define __class__	"ewol::StdPopUp"


extern const char * const ewolEventFileStdPopUpCancel   = "ewol event std_pop_up cancel";
extern const char * const ewolEventFileStdPopUpValidate = "ewol event std_pop_up validate";
extern const char * const ewolEventFileStdPopUpButton1 = "ewol event std_pop_up BT1";
extern const char * const ewolEventFileStdPopUpButton2 = "ewol event std_pop_up BT2";
extern const char * const ewolEventFileStdPopUpButton3 = "ewol event std_pop_up BT3";
extern const char * const ewolEventFileStdPopUpButton4 = "ewol event std_pop_up BT4";


widget::StdPopUp::StdPopUp(void)
{
	AddEventId(ewolEventFileStdPopUpCancel);
	AddEventId(ewolEventFileStdPopUpValidate);
	AddEventId(ewolEventFileStdPopUpButton1);
	AddEventId(ewolEventFileStdPopUpButton2);
	AddEventId(ewolEventFileStdPopUpButton3);
	AddEventId(ewolEventFileStdPopUpButton4);
	
	m_widgetTitleId = -1;
	m_widgetValidateId = -1;
	m_widgetCancelId = -1;
	
	
	ewol::SizerVert * mySizerVert = NULL;
	ewol::SizerHori * mySizerHori = NULL;
	ewol::Button * myButton = NULL;
	ewol::Entry * myEntry = NULL;
	ewol::Spacer * mySpacer = NULL;
	FileChooserFileList * myListFile = NULL;
	FileChooserFolderList * myListFolder = NULL;
	ewol::Label * myLabel = NULL;
	
	mySizerVert = new ewol::SizerVert();
		// set it in the pop-up-system : 
		SubWidgetSet(mySizerVert);
		
		myLabel = new ewol::Label("File chooser ...");
			m_widgetTitleId = myLabel->GetWidgetId();
			mySizerVert->SubWidgetAdd(myLabel);
		
		mySizerHori = new ewol::SizerHori();
			mySizerHori->LockExpendContamination(true);
			mySizerVert->SubWidgetAdd(mySizerHori);
			myButton = new ewol::Button("<-");
				myButton->ExternLinkOnEvent("ewol Button Pressed", GetWidgetId(), ewolEventFileChooserFolderUp );
				mySizerHori->SubWidgetAdd(myButton);
			myEntry = new ewol::Entry("~/");
				m_widgetCurrentFolderId = myEntry->GetWidgetId();
				myEntry->SetExpendX(true);
				myEntry->SetFillX(true);
				myEntry->SetWidth(200);
				mySizerHori->SubWidgetAdd(myEntry);
			
		mySizerHori = new ewol::SizerHori();
			mySizerHori->LockExpendContamination(true);
			mySizerVert->SubWidgetAdd(mySizerHori);
			myListFolder = new FileChooserFolderList();
				m_widgetListFolderId = myListFolder->GetWidgetId();
				//myList->SetExpendX(true);
				myListFolder->SetExpendY(true);
				myListFolder->SetFillY(true);
				mySizerHori->SubWidgetAdd(myListFolder);
			myListFile = new FileChooserFileList();
				m_widgetListFileId = myListFile->GetWidgetId();
				myListFile->SetExpendY(true);
				myListFile->SetFillX(true);
				myListFile->SetExpendY(true);
				myListFile->SetFillY(true);
				mySizerHori->SubWidgetAdd(myListFile);
			
		mySizerHori = new ewol::SizerHori();
			mySizerHori->LockExpendContamination(true);
			mySizerVert->SubWidgetAdd(mySizerHori);
			mySpacer = new ewol::Spacer();
				mySpacer->SetExpendX(true);
				mySizerHori->SubWidgetAdd(mySpacer);
			myButton = new ewol::Button("Open");
				m_widgetValidateId = myButton->GetWidgetId();
				myButton->ExternLinkOnEvent("ewol Button Pressed", GetWidgetId(), ewolEventFileChooserValidate);
				mySizerHori->SubWidgetAdd(myButton);
			myButton = new ewol::Button("Cancel");
				m_widgetCancelId = myButton->GetWidgetId();
				myButton->ExternLinkOnEvent("ewol Button Pressed", GetWidgetId(), ewolEventFileChooserCancel);
				mySizerHori->SubWidgetAdd(myButton);
}


widget::StdPopUp::~StdPopUp(void)
{
	
}


void widget::StdPopUp::SetTitle(etk::UString label)
{
	ewol::Label * tmpWidget = (ewol::Label*)ewol::widgetManager::Get(m_widgetTitleId);
	if (NULL == tmpWidget) {
		return;
	}
	tmpWidget->SetLabel(label);
}

void widget::StdPopUp::SetValidateLabel(etk::UString label)
{
	ewol::Button * tmpWidget = (ewol::Button*)ewol::widgetManager::Get(m_widgetValidateId);
	if (NULL == tmpWidget) {
		return;
	}
	tmpWidget->SetLabel(label);
}

void widget::StdPopUp::SetCancelLabel(etk::UString label)
{
	ewol::Button * tmpWidget = (ewol::Button*)ewol::widgetManager::Get(m_widgetCancelId);
	if (NULL == tmpWidget) {
		return;
	}
	tmpWidget->SetLabel(label);
}

void widget::StdPopUp::SetFolder(etk::UString folder)
{
	m_folder = folder;
}



bool widget::StdPopUp::OnEventAreaExternal(int32_t widgetID, const char * generateEventId, const char * eventExternId, float x, float y)
{
	EWOL_INFO("Receive Event from the BT ... : widgetid=" << widgetID << "\"" << generateEventId << "\" ==> internalEvent=\"" << eventExternId << "\"" );
	if (ewolEventFileChooserCancel == eventExternId) {
		//==> Auto remove ...
		
	}
	return GenEventInputExternal(eventExternId, x, y);
};
