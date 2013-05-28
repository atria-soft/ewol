/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ewol/UserConfig.h>
#include <ewol/eObject/EObjectManager.h>
#include <etk/os/FSNode.h>
#include <ewol/debug.h>

class UserConfig : public ewol::EObject
{
	private:
		etk::Vector<ewol::EObject*> m_list;
		etk::UString m_fileName;
	public:
		UserConfig(void) : m_fileName("USERDATA:generalConfig.xml")
		{
			m_static = true;  // Note : Set the object static notification( Must be set or assert at the end of process)
		};
		~UserConfig(void) { m_list.Clear(); };
		etk::Vector<ewol::EObject*>& List(void) { return m_list; };
		etk::UString& FileName(void) { return m_fileName; };
		
		void OnObjectRemove(ewol::EObject * _removeObject)
		{
			for( int32_t iii=m_list.Size()-1 ; iii>=0 ; iii--) {
				if (m_list[iii] == _removeObject) {
					m_list.Erase(iii);
				}
			}
		};
};


static UserConfig& l_obj(void)
{
	static UserConfig s_obj;
	return s_obj;
}


void ewol::userConfig::Init(void)
{
	
}

void ewol::userConfig::UnInit(void)
{
	
}


const ewol::EObject* ewol::userConfig::GetUserConfig(const etk::UString& _name)
{
	if (_name == "") {
		return NULL;
	}
	for (int32_t iii=0; iii<l_obj().List().Size() ; iii++) {
		if (l_obj().List()[iii] != NULL) {
			if (l_obj().List()[iii]->GetName() == _name) {
				return l_obj().List()[iii];
			}
		}
	}
	return NULL;
}

void ewol::userConfig::AddUserConfig(ewol::EObject* _newConfig)
{
	if (_newConfig == NULL) {
		return;
	}
	for (int32_t iii=0; iii<l_obj().List().Size() ; iii++) {
		if (l_obj().List()[iii] != NULL) {
			if (l_obj().List()[iii] == _newConfig) {
				return ;
			}
		}
	}
	l_obj().List().PushBack(_newConfig);
}



void ewol::userConfig::SetConfigName(const etk::UString& _fileName)
{
	l_obj().FileName() = _fileName;
}


bool ewol::userConfig::Load(void)
{
	etk::FSNode file(l_obj().FileName());
	if (file.Exist()==false) {
		EWOL_ERROR("Can not load the file : " << l_obj().FileName());
		return false;
	}
	// allocate the document in the stack
	TiXmlDocument XmlDocument;
	int32_t fileSize = file.FileSize();
	if (0==fileSize) {
		EWOL_ERROR("This file is empty : " << file);
		return false;
	}
	if (false == file.FileOpenRead()) {
		EWOL_ERROR("Can not open the file : " << file);
		return false;
	}
	// allocate data
	char * fileBuffer = new char[fileSize+5];
	if (NULL == fileBuffer) {
		EWOL_ERROR("Error Memory allocation size=" << fileSize);
		return false;
	}
	memset(fileBuffer, 0, (fileSize+5)*sizeof(char));
	// load data from the file :
	file.FileRead(fileBuffer, 1, fileSize);
	// close the file:
	file.FileClose();
	// load the XML from the memory
	XmlDocument.Parse((const char*)fileBuffer, 0, TIXML_ENCODING_UTF8);

	TiXmlElement* root = XmlDocument.FirstChildElement("config");
	if (NULL == root ) {
		EWOL_ERROR("(l ?) main node not find: \"config\" in \"" << file << "\"");
		if (NULL != fileBuffer) {
			delete[] fileBuffer;
		}
		return false;
	}
	for(TiXmlNode * pNode = root->FirstChild();
	    NULL != pNode;
	    pNode = pNode->NextSibling() ) {
		if (pNode->Type()==TiXmlNode::TINYXML_COMMENT) {
			// nothing to do, just proceed to next step
		} else {
			bool elementFound = false;
			for (int32_t iii=0; iii<l_obj().List().Size() ; iii++) {
				if (l_obj().List()[iii] != NULL) {
					if (l_obj().List()[iii]->GetName() == pNode->Value()) {
						l_obj().List()[iii]->LoadXML(pNode);
						elementFound = true;
						break;
					}
				}
			}
			if (elementFound==false) {
				EWOL_ERROR("(l "<<pNode->Row()<<") node not suported : \""<<pNode->Value());
			}
		}
	}
	if (NULL != fileBuffer) {
		delete[] fileBuffer;
	}
	return true;
}

#define MAX_SAVING_FILE_HISTORY (9)

bool ewol::userConfig::Save(void)
{
	// step 1 : Move the file to prevent writing error
	//Get the first oldest save :
	for (int32_t iii=MAX_SAVING_FILE_HISTORY-1; iii>0 ; iii--) {
		if (true==etk::FSNodeExist(l_obj().FileName()+"-"+iii) ) {
			etk::FSNodeMove(l_obj().FileName()+"-"+iii,l_obj().FileName()+"-"+(iii+1));
		}
	}
	if (true==etk::FSNodeExist(l_obj().FileName()) ) {
		etk::FSNodeMove(l_obj().FileName(),l_obj().FileName()+"-1");
	}
	// basic create file:
	etk::FSNode myNode(l_obj().FileName());
	// create basic folders ...
	myNode.Touch();
	etk::UString tmpCompleateName = myNode.GetFileSystemName();
	// step 2 : save the file
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	doc.LinkEndChild(decl);
	TiXmlElement * ElementBase = new TiXmlElement("config");
	doc.LinkEndChild(ElementBase);
	for (int32_t iii=0; iii<l_obj().List().Size() ; iii++) {
		if (l_obj().List()[iii] != NULL) {
			if (l_obj().List()[iii]->GetName().Size() != 0) {
				TiXmlElement * element = new TiXmlElement(l_obj().List()[iii]->GetName().c_str());
				if (NULL != element) {
					l_obj().List()[iii]->StoreXML(element);
					ElementBase->LinkEndChild(element);
				}
			}
		}
	}
	//Save Document
	doc.SaveFile( tmpCompleateName.c_str() );
	EWOL_DEBUG("Save in file : " << tmpCompleateName);
	// step 3 : Remove oldest save
	return true;
}


