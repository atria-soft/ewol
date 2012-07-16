/**
 *******************************************************************************
 * @file ewol/widget/ListFile.h
 * @brief ewol File lister widget system (header)
 * @author Edouard DUPIN
 * @date 12/07/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#ifndef __EWOL_LIST_FILE_H__
#define __EWOL_LIST_FILE_H__

#include <ewol/widget/List.h>

extern const char * const ewolEventFSFileSelect;
extern const char * const ewolEventFSFileValidate;
extern const char * const ewolEventFSFolderSelect;
extern const char * const ewolEventFSFolderValidate;

//!< EObject name :
extern const char * const TYPE_EOBJECT_WIDGET_FILE_LIST;

namespace ewol {
	typedef enum {
		EFS_FOLDER,
		EFS_FILE,
		EFS_LINK,
	} elementFS_te;
	
	class elementFS
	{
		public :
			etk::UString    m_name;
			elementFS_te    m_type;
			int32_t         m_rights;
			elementFS(etk::UString name, elementFS_te type) :
				m_name(name),
				m_type(type),
				m_rights(0)
			{ };
			~elementFS(void) {};
	};
	class ListFileSystem : public ewol::List
	{
		private:
			etk::VectorType<ewol::elementFS *> m_list;
			etk::UString                       m_folder;
			int32_t                            m_selectedLine;
			bool                               m_showFile;
			bool                               m_showTemporaryFile;
			bool                               m_showFolder;
			bool                               m_showHidden;
		public:
			ListFileSystem(void);
			~ListFileSystem(void);
			// display API :
			virtual color_ts GetBasicBG(void);
			uint32_t GetNuberOfColomn(void);
			bool GetTitle(int32_t colomn, etk::UString &myTitle, color_ts &fg, color_ts &bg);
			uint32_t GetNuberOfRaw(void);
			bool GetElement(int32_t colomn, int32_t raw, etk::UString &myTextToWrite, color_ts &fg, color_ts &bg);
			bool OnItemEvent(int32_t IdInput, ewol::eventInputType_te typeEvent, int32_t colomn, int32_t raw, float x, float y);
			/**
			 * @brief Check if the object has the specific type.
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type of the object we want to check
			 * @return true if the object is compatible, otherwise false
			 */
			bool CheckObjectType(const char * const objectType);
			
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			const char * const GetObjectType(void);
		public:
			// extern API :
			void         SetFolder(etk::UString newFolder);
			etk::UString GetFolder(void);
			// select the specific file
			void         SetSelect(etk::UString data);
			etk::UString GetSelect(void);
			// regenerate the view ....
			void         RegenerateView(void);
			void         SetShowFiles(bool state);
			void         SetShowFolder(bool state);
			void         SetShowHiddenFiles(bool state);
			void         SetShowTemporaryFiles(bool state);
	};
};

#define EWOL_CAST_WIDGET_FILE_LIST(curentPointer) EWOL_CAST(ewol::TYPE_EOBJECT_WIDGET_FILE_LIST,FileChooserFileList,curentPointer)

#endif

