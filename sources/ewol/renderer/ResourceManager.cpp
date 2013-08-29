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


// Specific for the resource : 
uint32_t ewol::Resource::valBase=0;


static etk::Vector<ewol::Resource*> l_resourceList;
static etk::Vector<ewol::Resource*> l_resourceListToUpdate;
static bool                         l_contextHasBeenRemoved = true;

void ewol::resource::Init(void)
{
	// nothing to do in theory then, we clean the buffers :
	// NOTE : If we do domething here, then the system does not work corectly
	if (l_resourceList.Size() != 0) {
		EWOL_CRITICAL("Start with a resource manager Not empty, number of resources loaded : " << l_resourceList.Size());
	}
	l_resourceListToUpdate.Clear();
	l_resourceList.Clear();
	l_contextHasBeenRemoved = true;
}

void ewol::resource::UnInit(void)
{
	Display();
	l_resourceListToUpdate.Clear();
	// remove all resources ...
	for (int32_t iii=l_resourceList.Size()-1; iii>=0; iii--) {
		if (l_resourceList[iii] != NULL) {
			EWOL_WARNING("Find a resource that is not removed : [" << l_resourceList[iii]->GetUID() << "]"
			             << "=\"" << l_resourceList[iii]->GetName() << "\" "
			             << l_resourceList[iii]->GetCounter() << " elements");
			delete(l_resourceList[iii]);
			l_resourceList[iii] = NULL;
		}
	}
	l_resourceList.Clear();
}

void ewol::resource::Display(void)
{
	EWOL_INFO("Resources loaded : ");
	// remove all resources ...
	for (int32_t iii=l_resourceList.Size()-1; iii>=0; iii--) {
		if (l_resourceList[iii] != NULL) {
			EWOL_INFO("    [" << l_resourceList[iii]->GetUID() << "]"
			          << l_resourceList[iii]->GetType()
			          << "=\"" << l_resourceList[iii]->GetName() << "\" "
			          << l_resourceList[iii]->GetCounter() << " elements");
		}
	}
	EWOL_INFO("Resources ---");
}

void ewol::resource::ReLoadResources(void)
{
	EWOL_INFO("-------------  Resources re-loaded  -------------");
	// remove all resources ...
	if (l_resourceList.Size() != 0) {
		for (int32_t jjj=0; jjj<MAX_RESOURCE_LEVEL; jjj++) {
			EWOL_INFO("    Reload level : " << jjj << "/" << (MAX_RESOURCE_LEVEL-1));
			for (int32_t iii=l_resourceList.Size()-1; iii>=0; iii--) {
				if(l_resourceList[iii] != NULL) {
					if (jjj==l_resourceList[iii]->GetResourceLevel()) {
						l_resourceList[iii]->Reload();
						EWOL_INFO("        [" << l_resourceList[iii]->GetUID() << "]="<< l_resourceList[iii]->GetType());
					}
				}
			}
		}
	}
	// TODO : UNderstand why it is set here ...
	//ewol::RequestUpdateSize();
	EWOL_INFO("-------------  Resources  -------------");
}

void ewol::resource::Update(ewol::Resource* object)
{
	// chek if not added before
	for (int32_t iii=0; iii<l_resourceListToUpdate.Size(); iii++) {
		if (l_resourceListToUpdate[iii] != NULL) {
			if (l_resourceListToUpdate[iii] == object) {
				// just prevent some double add ...
				return;
			}
		}
	}
	// add it ...
	l_resourceListToUpdate.PushBack(object);
}

// Specific to load or update the data in the openGl context ==> system use only
void ewol::resource::UpdateContext(void)
{
	if (true == l_contextHasBeenRemoved) {
		// need to update all ...
		l_contextHasBeenRemoved = false;
		if (l_resourceList.Size() != 0) {
			for (int32_t jjj=0; jjj<MAX_RESOURCE_LEVEL; jjj++) {
				EWOL_INFO("    UpdateContext level (D) : " << jjj << "/" << (MAX_RESOURCE_LEVEL-1));
				for (int32_t iii=0; iii<l_resourceList.Size(); iii++) {
					if(l_resourceList[iii] != NULL) {
						if (jjj==l_resourceList[iii]->GetResourceLevel()) {
							//EWOL_DEBUG("Update context of " << iii << " named : " << l_resourceList[iii]->GetName());
							l_resourceList[iii]->UpdateContext();
						}
					}
				}
			}
		}
	}else {
		if (l_resourceListToUpdate.Size() != 0) {
			for (int32_t jjj=0; jjj<MAX_RESOURCE_LEVEL; jjj++) {
				EWOL_INFO("    UpdateContext level (U) : " << jjj << "/" << (MAX_RESOURCE_LEVEL-1));
				for (int32_t iii=0; iii<l_resourceListToUpdate.Size(); iii++) {
					if(l_resourceListToUpdate[iii] != NULL) {
						if (jjj==l_resourceListToUpdate[iii]->GetResourceLevel()) {
							l_resourceListToUpdate[iii]->UpdateContext();
						}
					}
				}
			}
		}
	}
	// Clean the update list
	l_resourceListToUpdate.Clear();
}

// in this case, it is really too late ...
void ewol::resource::ContextHasBeenDestroyed(void)
{
	for (int32_t iii=0; iii<l_resourceList.Size(); iii++) {
		if (l_resourceList[iii] != NULL) {
			l_resourceList[iii]->RemoveContextToLate();
		}
	}
	ewol::openGL::ContextIsRemoved();
	// no context preent ...
	l_contextHasBeenRemoved = true;
}





// internal generic keeper ...
ewol::Resource* ewol::resource::LocalKeep(const etk::UString& filename)
{
	EWOL_VERBOSE("KEEP (DEFAULT) : file : \"" << filename << "\"");
	for (int32_t iii=0; iii<l_resourceList.Size(); iii++) {
		if (l_resourceList[iii] != NULL) {
			if(l_resourceList[iii]->HasName(filename)) {
				l_resourceList[iii]->Increment();
				return l_resourceList[iii];
			}
		}
	}
	// we did not find it ...
	return NULL;
}

// internal generic keeper ...
void ewol::resource::LocalAdd(ewol::Resource* object)
{
	//Add ... find empty slot
	for (int32_t iii=0; iii<l_resourceList.Size(); iii++) {
		if (l_resourceList[iii] == NULL) {
			l_resourceList[iii] = object;
			return;
		}
	}
	// add at the end if no slot is free
	l_resourceList.PushBack(object);
}

// return the type of the resource ...
bool ewol::resource::Keep(const etk::UString& filename, ewol::TexturedFont*& object)
{
	EWOL_VERBOSE("KEEP : TexturedFont : file : \"" << filename << "\"");
	object = static_cast<ewol::TexturedFont*>(LocalKeep(filename));
	if (NULL != object) {
		return true;
	}
	// need to crate a new one ...
	object = new ewol::TexturedFont(filename);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : " << filename);
		return false;
	}
	LocalAdd(object);
	return true;
}


bool ewol::resource::Keep(const etk::UString& filename, ewol::FontBase*& object)
{
	EWOL_VERBOSE("KEEP : Font : file : \"" << filename << "\"");
	object = static_cast<ewol::FontBase*>(LocalKeep(filename));
	if (NULL != object) {
		return true;
	}
	// need to crate a new one ...
	object = new ewol::FontFreeType(filename);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : " << filename);
		return false;
	}
	LocalAdd(object);
	return true;
}

bool ewol::resource::Keep(const etk::UString& filename, ewol::Program*& object)
{
	EWOL_VERBOSE("KEEP : Program : file : \"" << filename << "\"");
	object = static_cast<ewol::Program*>(LocalKeep(filename));
	if (NULL != object) {
		return true;
	}
	// need to crate a new one ...
	object = new ewol::Program(filename);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : " << filename);
		return false;
	}
	LocalAdd(object);
	return true;
}

bool ewol::resource::Keep(const etk::UString& filename, ewol::Shader*& object)
{
	EWOL_VERBOSE("KEEP : Simpleshader : file : \"" << filename << "\"");
	object = static_cast<ewol::Shader*>(LocalKeep(filename));
	if (NULL != object) {
		return true;
	}
	// need to crate a new one ...
	object = new ewol::Shader(filename);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : " << filename);
		return false;
	}
	LocalAdd(object);
	return true;
}

bool ewol::resource::Keep(ewol::Texture*& object)
{
	// this element create a new one every time ....
	object = new ewol::Texture("");
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : ??TEX??");
		return false;
	}
	LocalAdd(object);
	return true;
}

bool ewol::resource::Keep(ewol::Colored3DObject*& _object)
{
	EWOL_VERBOSE("KEEP : direct Colored3DObject");
	etk::UString filename = "?metaObject?Colored3DObject";
	_object = static_cast<ewol::Colored3DObject*>(LocalKeep(filename));
	if (NULL != _object) {
		return true;
	}
	// need to crate a new one ...
	_object = new ewol::Colored3DObject(filename);
	if (NULL == _object) {
		EWOL_ERROR("allocation error of a resource : Colored3DObject ");
		return false;
	}
	LocalAdd(_object);
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

bool ewol::resource::Keep(const etk::UString& _filename, ewol::TextureFile*& _object, ivec2 _size)
{
	EWOL_INFO("KEEP : TextureFile : file : " << _filename << " basic size=" << _size);
	if (_filename == "") {
		_object = new ewol::TextureFile("");
		if (NULL == _object) {
			EWOL_ERROR("allocation error of a resource : ??TEX??");
			return false;
		}
		LocalAdd(_object);
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
	_object = static_cast<ewol::TextureFile*>(LocalKeep(TmpFilename));
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
	LocalAdd(_object);
	return true;
}


bool ewol::resource::Keep(const etk::UString& _meshName, ewol::Mesh*& _object)
{
	_object = static_cast<ewol::Mesh*>(LocalKeep(_meshName));
	if (NULL != _object) {
		return true;
	}
	_object = new ewol::Mesh(_meshName);
	if (NULL == _object) {
		EWOL_ERROR("allocation error of a resource : ??Mesh??" << _meshName);
		return false;
	}
	LocalAdd(_object);
	return true;
}


bool ewol::resource::Keep(const etk::UString& _accesMode, ewol::VirtualBufferObject*& _object)
{
	// this element create a new one every time ....
	_object = new ewol::VirtualBufferObject(_accesMode);
	if (NULL == _object) {
		EWOL_ERROR("allocation error of a resource : ??VBO??");
		return false;
	}
	LocalAdd(_object);
	return true;
}

bool ewol::resource::Keep(const etk::UString& filename, ewol::ConfigFile*& object)
{
	EWOL_INFO("KEEP : SimpleConfig : file : \"" << filename << "\"");
	object = static_cast<ewol::ConfigFile*>(LocalKeep(filename));
	if (NULL != object) {
		return true;
	}
	// this element create a new one every time ....
	object = new ewol::ConfigFile(filename);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : ??Mesh.obj??");
		return false;
	}
	LocalAdd(object);
	return true;
}



void ewol::resource::Release(ewol::Resource*& object)
{
	if (NULL == object) {
		EWOL_ERROR("Try to remove a resource that have null pointer ...");
		return;
	}
	for (int32_t iii=0; iii<l_resourceListToUpdate.Size(); iii++) {
		if (l_resourceListToUpdate[iii] == object) {
			l_resourceListToUpdate[iii] = NULL;
			//l_resourceListToUpdate.Erase(iii);
		}
	}
	EWOL_VERBOSE("RELEASE (default) : file : \"" << object->GetName() << "\"");
	for (int32_t iii=l_resourceList.Size()-1; iii>=0; iii--) {
		if (l_resourceList[iii] != NULL) {
			if(l_resourceList[iii] == object) {
				if (true == l_resourceList[iii]->Decrement()) {
					// delete element
					delete(l_resourceList[iii]);
					// remove element from the list :
					l_resourceList[iii] = NULL;
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


void ewol::resource::Release(ewol::TexturedFont*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	Release(object2);
	object = NULL;
}
void ewol::resource::Release(ewol::FontBase*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	Release(object2);
	object = NULL;
}
void ewol::resource::Release(ewol::Program*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	Release(object2);
	object = NULL;
}
void ewol::resource::Release(ewol::Shader*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	Release(object2);
	object = NULL;
}
void ewol::resource::Release(ewol::Texture*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	Release(object2);
	object = NULL;
}
void ewol::resource::Release(ewol::TextureFile*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	//EWOL_INFO("RELEASE : TextureFile :  nb=" << object2->GetCounter());
	Release(object2);
	object = NULL;
}

void ewol::resource::Release(ewol::Mesh*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	Release(object2);
	object = NULL;
}

void ewol::resource::Release(ewol::ConfigFile*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	Release(object2);
	object = NULL;
}

void ewol::resource::Release(ewol::Colored3DObject*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	Release(object2);
	object = NULL;
}

void ewol::resource::Release(ewol::VirtualBufferObject*& object)
{
	ewol::Resource* object2 = static_cast<ewol::Resource*>(object);
	Release(object2);
	object = NULL;
}
