/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/renderer/ResourceManager.h>
#include <ewol/renderer/resources/FontFreeType.h>
#include <ewol/ewol.h>
#include <ewol/renderer/openGL.h>
#include <ewol/renderer/eSystem.h>


// Specific for the resource : 
uint32_t ewol::Resource::valBase=0;



ewol::ResourceManager::ResourceManager(void) :
	m_contextHasBeenRemoved(true)
{
	// nothing to do ...
}

ewol::ResourceManager::~ResourceManager(void)
{
	Display();
	m_resourceListToUpdate.Clear();
	// remove all resources ...
	for (int32_t iii=m_resourceList.Size()-1; iii>=0; iii--) {
		if (m_resourceList[iii] != NULL) {
			EWOL_WARNING("Find a resource that is not removed : [" << m_resourceList[iii]->GetUID() << "]"
			             << "=\"" << m_resourceList[iii]->GetName() << "\" "
			             << m_resourceList[iii]->GetCounter() << " elements");
			delete(m_resourceList[iii]);
			m_resourceList[iii] = NULL;
		}
	}
	m_resourceList.Clear();
}

void ewol::ResourceManager::Display(void)
{
	EWOL_INFO("Resources loaded : ");
	// remove all resources ...
	for (int32_t iii=m_resourceList.Size()-1; iii>=0; iii--) {
		if (m_resourceList[iii] != NULL) {
			EWOL_INFO("    [" << m_resourceList[iii]->GetUID() << "]"
			          << m_resourceList[iii]->GetType()
			          << "=\"" << m_resourceList[iii]->GetName() << "\" "
			          << m_resourceList[iii]->GetCounter() << " elements");
		}
	}
	EWOL_INFO("Resources ---");
}

void ewol::ResourceManager::ReLoadResources(void)
{
	EWOL_INFO("-------------  Resources re-loaded  -------------");
	// remove all resources ...
	if (m_resourceList.Size() != 0) {
		for (int32_t jjj=0; jjj<MAX_RESOURCE_LEVEL; jjj++) {
			EWOL_INFO("    Reload level : " << jjj << "/" << (MAX_RESOURCE_LEVEL-1));
			for (int32_t iii=m_resourceList.Size()-1; iii>=0; iii--) {
				if(m_resourceList[iii] != NULL) {
					if (jjj==m_resourceList[iii]->GetResourceLevel()) {
						m_resourceList[iii]->Reload();
						EWOL_INFO("        [" << m_resourceList[iii]->GetUID() << "]="<< m_resourceList[iii]->GetType());
					}
				}
			}
		}
	}
	// TODO : UNderstand why it is set here ...
	//ewol::RequestUpdateSize();
	EWOL_INFO("-------------  Resources  -------------");
}

void ewol::ResourceManager::Update(ewol::Resource* object)
{
	// chek if not added before
	for (int32_t iii=0; iii<m_resourceListToUpdate.Size(); iii++) {
		if (m_resourceListToUpdate[iii] != NULL) {
			if (m_resourceListToUpdate[iii] == object) {
				// just prevent some double add ...
				return;
			}
		}
	}
	// add it ...
	m_resourceListToUpdate.PushBack(object);
}

// Specific to load or update the data in the openGl context ==> system use only
void ewol::ResourceManager::UpdateContext(void)
{
	if (true == m_contextHasBeenRemoved) {
		// need to update all ...
		m_contextHasBeenRemoved = false;
		if (m_resourceList.Size() != 0) {
			for (int32_t jjj=0; jjj<MAX_RESOURCE_LEVEL; jjj++) {
				EWOL_INFO("    UpdateContext level (D) : " << jjj << "/" << (MAX_RESOURCE_LEVEL-1));
				for (int32_t iii=0; iii<m_resourceList.Size(); iii++) {
					if(m_resourceList[iii] != NULL) {
						if (jjj==m_resourceList[iii]->GetResourceLevel()) {
							//EWOL_DEBUG("Update context of " << iii << " named : " << l_resourceList[iii]->GetName());
							m_resourceList[iii]->UpdateContext();
						}
					}
				}
			}
		}
	}else {
		if (m_resourceListToUpdate.Size() != 0) {
			for (int32_t jjj=0; jjj<MAX_RESOURCE_LEVEL; jjj++) {
				EWOL_INFO("    UpdateContext level (U) : " << jjj << "/" << (MAX_RESOURCE_LEVEL-1));
				for (int32_t iii=0; iii<m_resourceListToUpdate.Size(); iii++) {
					if(m_resourceListToUpdate[iii] != NULL) {
						if (jjj==m_resourceListToUpdate[iii]->GetResourceLevel()) {
							m_resourceListToUpdate[iii]->UpdateContext();
						}
					}
				}
			}
		}
	}
	// Clean the update list
	m_resourceListToUpdate.Clear();
}

// in this case, it is really too late ...
void ewol::ResourceManager::ContextHasBeenDestroyed(void)
{
	for (int32_t iii=0; iii<m_resourceList.Size(); iii++) {
		if (m_resourceList[iii] != NULL) {
			m_resourceList[iii]->RemoveContextToLate();
		}
	}
	// no context preent ...
	m_contextHasBeenRemoved = true;
}





// internal generic keeper ...
ewol::Resource* ewol::ResourceManager::LocalKeep(const etk::UString& filename)
{
	EWOL_VERBOSE("KEEP (DEFAULT) : file : \"" << filename << "\"");
	for (int32_t iii=0; iii<m_resourceList.Size(); iii++) {
		if (m_resourceList[iii] != NULL) {
			if(m_resourceList[iii]->HasName(filename)) {
				m_resourceList[iii]->Increment();
				return m_resourceList[iii];
			}
		}
	}
	// we did not find it ...
	return NULL;
}

// internal generic keeper ...
void ewol::ResourceManager::LocalAdd(ewol::Resource* object)
{
	//Add ... find empty slot
	for (int32_t iii=0; iii<m_resourceList.Size(); iii++) {
		if (m_resourceList[iii] == NULL) {
			m_resourceList[iii] = object;
			return;
		}
	}
	// add at the end if no slot is free
	m_resourceList.PushBack(object);
}

// return the type of the resource ...
bool ewol::ResourceManager::Keep(const etk::UString& filename, ewol::TexturedFont*& object)
{
	EWOL_VERBOSE("KEEP : TexturedFont : file : \"" << filename << "\"");
	object = static_cast<ewol::TexturedFont*>(ewol::eSystem::GetSystem().GetResourcesManager().LocalKeep(filename));
	if (NULL != object) {
		return true;
	}
	// need to crate a new one ...
	object = new ewol::TexturedFont(filename);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : " << filename);
		return false;
	}
	ewol::eSystem::GetSystem().GetResourcesManager().LocalAdd(object);
	return true;
}


bool ewol::ResourceManager::Keep(const etk::UString& filename, ewol::FontBase*& object)
{
	EWOL_VERBOSE("KEEP : Font : file : \"" << filename << "\"");
	object = static_cast<ewol::FontBase*>(ewol::eSystem::GetSystem().GetResourcesManager().LocalKeep(filename));
	if (NULL != object) {
		return true;
	}
	// need to crate a new one ...
	object = new ewol::FontFreeType(filename);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : " << filename);
		return false;
	}
	ewol::eSystem::GetSystem().GetResourcesManager().LocalAdd(object);
	return true;
}

bool ewol::ResourceManager::Keep(const etk::UString& filename, ewol::Program*& object)
{
	EWOL_VERBOSE("KEEP : Program : file : \"" << filename << "\"");
	object = static_cast<ewol::Program*>(ewol::eSystem::GetSystem().GetResourcesManager().LocalKeep(filename));
	if (NULL != object) {
		return true;
	}
	// need to crate a new one ...
	object = new ewol::Program(filename);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : " << filename);
		return false;
	}
	ewol::eSystem::GetSystem().GetResourcesManager().LocalAdd(object);
	return true;
}

bool ewol::ResourceManager::Keep(const etk::UString& filename, ewol::Shader*& object)
{
	EWOL_VERBOSE("KEEP : Simpleshader : file : \"" << filename << "\"");
	object = static_cast<ewol::Shader*>(ewol::eSystem::GetSystem().GetResourcesManager().LocalKeep(filename));
	if (NULL != object) {
		return true;
	}
	// need to crate a new one ...
	object = new ewol::Shader(filename);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : " << filename);
		return false;
	}
	ewol::eSystem::GetSystem().GetResourcesManager().LocalAdd(object);
	return true;
}

bool ewol::ResourceManager::Keep(ewol::Texture*& object)
{
	// this element create a new one every time ....
	object = new ewol::Texture("");
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : ??TEX??");
		return false;
	}
	ewol::eSystem::GetSystem().GetResourcesManager().LocalAdd(object);
	return true;
}

bool ewol::ResourceManager::Keep(ewol::Colored3DObject*& _object)
{
	EWOL_VERBOSE("KEEP : direct Colored3DObject");
	etk::UString filename = "?metaObject?Colored3DObject";
	_object = static_cast<ewol::Colored3DObject*>(ewol::eSystem::GetSystem().GetResourcesManager().LocalKeep(filename));
	if (NULL != _object) {
		return true;
	}
	// need to crate a new one ...
	_object = new ewol::Colored3DObject(filename);
	if (NULL == _object) {
		EWOL_ERROR("allocation error of a resource : Colored3DObject ");
		return false;
	}
	ewol::eSystem::GetSystem().GetResourcesManager().LocalAdd(_object);
	return true;
}
#ifdef __TARGET_OS__Android
/**
 * @brief get the next power 2 if the input
 * @param[in] _value Value that we want the next power of 2
 * @return result value
 */
static int32_t nextP2(int32_t _value)
{
	int32_t val=1;
	for (int32_t iii=1; iii<31; iii++) {
		if (_value <= val) {
			return val;
		}
		val *=2;
	}
	EWOL_CRITICAL("impossible CASE.... request P2 of " << _value);
	return val;
}
#endif

bool ewol::ResourceManager::Keep(const etk::UString& _filename, ewol::TextureFile*& _object, ivec2 _size)
{
	EWOL_INFO("KEEP : TextureFile : file : " << _filename << " basic size=" << _size);
	if (_filename == "") {
		_object = new ewol::TextureFile("");
		if (NULL == _object) {
			EWOL_ERROR("allocation error of a resource : ??TEX??");
			return false;
		}
		ewol::eSystem::GetSystem().GetResourcesManager().LocalAdd(_object);
		return true;
	}
	if (_size.x()==0) {
		_size.setX(-1);
		//EWOL_ERROR("Error Request the image size.x() =0 ???");
	}
	if (_size.y()==0) {
		_size.setY(-1);
		//EWOL_ERROR("Error Request the image size.y() =0 ???");
	}
	etk::UString TmpFilename = _filename;
	if (false == _filename.EndWith(".svg") ) {
		_size = ivec2(-1,-1);
	}
	#ifdef __TARGET_OS__MacOs
		EWOL_ERROR("TODO : Remove this strange hack");
		_size = ivec2(64,64);
	#endif
	if (_size.x()>0 && _size.y()>0) {
		EWOL_VERBOSE("    ==> specific size : " << _size);
		#ifdef __TARGET_OS__Android
			_size.setValue(nextP2(_size.x()), nextP2(_size.y()));
		#endif
		TmpFilename += ":";
		TmpFilename += _size.x();
		TmpFilename += "x";
		TmpFilename += _size.y();
	}
	
	EWOL_INFO("KEEP : TextureFile : file : \"" << TmpFilename << "\" new size=" << _size);
	_object = static_cast<ewol::TextureFile*>(ewol::eSystem::GetSystem().GetResourcesManager().LocalKeep(TmpFilename));
	if (NULL != _object) {
		return true;
	}
	EWOL_INFO("        ==> create new one...");
	// need to crate a new one ...
	_object = new ewol::TextureFile(TmpFilename, _filename, _size);
	if (NULL == _object) {
		EWOL_ERROR("allocation error of a resource : " << _filename);
		return false;
	}
	ewol::eSystem::GetSystem().GetResourcesManager().LocalAdd(_object);
	return true;
}


bool ewol::ResourceManager::Keep(const etk::UString& _meshName, ewol::Mesh*& _object)
{
	_object = static_cast<ewol::Mesh*>(ewol::eSystem::GetSystem().GetResourcesManager().LocalKeep(_meshName));
	if (NULL != _object) {
		return true;
	}
	_object = new ewol::Mesh(_meshName);
	if (NULL == _object) {
		EWOL_ERROR("allocation error of a resource : ??Mesh??" << _meshName);
		return false;
	}
	ewol::eSystem::GetSystem().GetResourcesManager().LocalAdd(_object);
	return true;
}


bool ewol::ResourceManager::Keep(const etk::UString& _accesMode, ewol::VirtualBufferObject*& _object)
{
	// this element create a new one every time ....
	_object = new ewol::VirtualBufferObject(_accesMode);
	if (NULL == _object) {
		EWOL_ERROR("allocation error of a resource : ??VBO??");
		return false;
	}
	ewol::eSystem::GetSystem().GetResourcesManager().LocalAdd(_object);
	return true;
}

bool ewol::ResourceManager::Keep(const etk::UString& filename, ewol::ConfigFile*& object)
{
	EWOL_INFO("KEEP : SimpleConfig : file : \"" << filename << "\"");
	object = static_cast<ewol::ConfigFile*>(ewol::eSystem::GetSystem().GetResourcesManager().LocalKeep(filename));
	if (NULL != object) {
		return true;
	}
	// this element create a new one every time ....
	object = new ewol::ConfigFile(filename);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : ??Mesh.obj??");
		return false;
	}
	ewol::eSystem::GetSystem().GetResourcesManager().LocalAdd(object);
	return true;
}



void ewol::ResourceManager::Release(ewol::Resource*& object)
{
	if (NULL == object) {
		EWOL_ERROR("Try to remove a resource that have null pointer ...");
		return;
	}
	for (int32_t iii=0; iii<m_resourceListToUpdate.Size(); iii++) {
		if (m_resourceListToUpdate[iii] == object) {
			m_resourceListToUpdate[iii] = NULL;
			//l_resourceListToUpdate.Erase(iii);
		}
	}
	EWOL_VERBOSE("RELEASE (default) : file : \"" << object->GetName() << "\"");
	for (int32_t iii=m_resourceList.Size()-1; iii>=0; iii--) {
		if (m_resourceList[iii] != NULL) {
			if(m_resourceList[iii] == object) {
				if (true == m_resourceList[iii]->Decrement()) {
					// delete element
					delete(m_resourceList[iii]);
					// remove element from the list :
					m_resourceList[iii] = NULL;
				}
				// insidiously remove the pointer for the caller ...
				object = NULL;
				return;
			}
		}
	}
	EWOL_ERROR("Can not find the resources in the list : " << (int64_t)object);
	// insidiously remove the pointer for the caller ...
	object = NULL;
}


void ewol::ResourceManager::Release(ewol::TexturedFont*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	ewol::eSystem::GetSystem().GetResourcesManager().Release(object2);
	object = NULL;
}
void ewol::ResourceManager::Release(ewol::FontBase*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	ewol::eSystem::GetSystem().GetResourcesManager().Release(object2);
	object = NULL;
}
void ewol::ResourceManager::Release(ewol::Program*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	ewol::eSystem::GetSystem().GetResourcesManager().Release(object2);
	object = NULL;
}
void ewol::ResourceManager::Release(ewol::Shader*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	ewol::eSystem::GetSystem().GetResourcesManager().Release(object2);
	object = NULL;
}
void ewol::ResourceManager::Release(ewol::Texture*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	ewol::eSystem::GetSystem().GetResourcesManager().Release(object2);
	object = NULL;
}
void ewol::ResourceManager::Release(ewol::TextureFile*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	//EWOL_INFO("RELEASE : TextureFile :  nb=" << object2->GetCounter());
	ewol::eSystem::GetSystem().GetResourcesManager().Release(object2);
	object = NULL;
}

void ewol::ResourceManager::Release(ewol::Mesh*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	ewol::eSystem::GetSystem().GetResourcesManager().Release(object2);
	object = NULL;
}

void ewol::ResourceManager::Release(ewol::ConfigFile*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	ewol::eSystem::GetSystem().GetResourcesManager().Release(object2);
	object = NULL;
}

void ewol::ResourceManager::Release(ewol::Colored3DObject*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	ewol::eSystem::GetSystem().GetResourcesManager().Release(object2);
	object = NULL;
}

void ewol::ResourceManager::Release(ewol::VirtualBufferObject*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	ewol::eSystem::GetSystem().GetResourcesManager().Release(object2);
	object = NULL;
}
