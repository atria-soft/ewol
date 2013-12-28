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


namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class ListFileSystem : public ewol::widget::List {
			public:
				// Event list of properties
				static const char * const eventFileSelect;
				static const char * const eventFileValidate;
				static const char * const eventFolderSelect;
				static const char * const eventFolderValidate;
			private:
				std::vector<etk::FSNode *> m_list;
				std::string m_folder;
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
				bool getTitle(int32_t _colomn, std::string& _myTitle, etk::Color<>& _fg, etk::Color<>& _bg);
				uint32_t getNuberOfRaw(void);
				bool getElement(int32_t _colomn, int32_t _raw, std::string& _myTextToWrite, etk::Color<>& _fg, etk::Color<>& _bg);
				bool onItemEvent(int32_t _IdInput, enum ewol::key::status _typeEvent, int32_t _colomn, int32_t _raw, float _x, float _y);
			public:
				// extern API :
				void setFolder(std::string _newFolder);
				std::string getFolder(void);
				// select the specific file
				void setSelect(std::string _data);
				std::string getSelect(void);
				// regenerate the view ....
				void regenerateView(void);
				void setShowFiles(bool _state);
				void setShowFolder(bool _state);
				void setShowHiddenFiles(bool _state);
				void setShowTemporaryFiles(bool _state);
		};
	};
};

#endif

