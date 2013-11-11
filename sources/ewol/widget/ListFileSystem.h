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
	/**
	 * @ingroup ewolWidgetGroup
	 */
	class ListFileSystem : public widget::List {
		private:
			std::vector<etk::FSNode *> m_list;
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
			virtual etk::Color<> getBasicBG(void);
			uint32_t getNuberOfColomn(void);
			bool getTitle(int32_t _colomn, etk::UString& _myTitle, etk::Color<>& _fg, etk::Color<>& _bg);
			uint32_t getNuberOfRaw(void);
			bool getElement(int32_t _colomn, int32_t _raw, etk::UString& _myTextToWrite, etk::Color<>& _fg, etk::Color<>& _bg);
			bool onItemEvent(int32_t _IdInput, enum ewol::keyEvent::status _typeEvent, int32_t _colomn, int32_t _raw, float _x, float _y);
			const char * const getObjectType(void) {
				return "widget::ListFileSystem";
			};
		public:
			// extern API :
			void setFolder(etk::UString _newFolder);
			etk::UString getFolder(void);
			// select the specific file
			void setSelect(etk::UString _data);
			etk::UString getSelect(void);
			// regenerate the view ....
			void regenerateView(void);
			void setShowFiles(bool _state);
			void setShowFolder(bool _state);
			void setShowHiddenFiles(bool _state);
			void setShowTemporaryFiles(bool _state);
	};
};

#endif

