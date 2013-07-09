/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_LIST_FILE_H__
#define __EWOL_LIST_FILE_H__

#include <ewol/widget/List.h>
#include <etk/os/FSNode.h>

extern const char * const ewolEventFSFileSelect;
extern const char * const ewolEventFSFileValidate;
extern const char * const ewolEventFSFolderSelect;
extern const char * const ewolEventFSFolderValidate;

namespace widget {
	class ListFileSystem : public widget::List
	{
		private:
			etk::Vector<etk::FSNode *> m_list;
			etk::UString m_folder;
			int32_t m_selectedLine;
			bool m_showFile;
			bool m_showTemporaryFile;
			bool m_showFolder;
			bool m_showHidden;
		public:
			ListFileSystem(void);
			~ListFileSystem(void);
			// Derived function
			virtual etk::Color<> GetBasicBG(void);
			uint32_t GetNuberOfColomn(void);
			bool GetTitle(int32_t _colomn, etk::UString& _myTitle, etk::Color<>& _fg, etk::Color<>& _bg);
			uint32_t GetNuberOfRaw(void);
			bool GetElement(int32_t _colomn, int32_t _raw, etk::UString& _myTextToWrite, etk::Color<>& _fg, etk::Color<>& _bg);
			bool OnItemEvent(int32_t _IdInput, ewol::keyEvent::status_te _typeEvent, int32_t _colomn, int32_t _raw, float _x, float _y);
			const char * const GetObjectType(void) { return "EwolListFileSystem"; };
		public:
			// extern API :
			void SetFolder(etk::UString _newFolder);
			etk::UString GetFolder(void);
			// select the specific file
			void SetSelect(etk::UString _data);
			etk::UString GetSelect(void);
			// regenerate the view ....
			void RegenerateView(void);
			void SetShowFiles(bool _state);
			void SetShowFolder(bool _state);
			void SetShowHiddenFiles(bool _state);
			void SetShowTemporaryFiles(bool _state);
	};
};

#endif

