/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <ewol/widget/List.h>
#include <etk/os/FSNode.h>
#include <ewol/resource/ColorFile.h>
#include <esignal/ISignal.h>

namespace ewol {
	namespace widget {
		/**
		 * @brief Generic display folder class. This widget display the content of a single folder :
		 */
		class ListFileSystem : public ewol::widget::List {
			public: // signals
				esignal::ISignal<std::string> signalFileSelect; //!< @event "file-select" Generated when a file is selected.
				esignal::ISignal<std::string> signalFileValidate; //!< @event "file-validate" Generate when the user validate (return) or double click on the element
				esignal::ISignal<std::string> signalFolderSelect;
				esignal::ISignal<std::string> signalFolderValidate;
			public: // properties
				eproperty::Value<std::string> propertyPath; //!< Current folder that display point on.
				eproperty::Value<std::string> propertyFile; //!< current selected file
				eproperty::Value<bool> propertyShowFile; //!< Show files elements
				eproperty::Value<bool> propertyShowFolder; //!< Display the folders elements
				eproperty::Value<bool> propertyShowHidden; //!< Display hidden elements
				eproperty::Value<std::string> propertyFilter; //!< Regular expression to filter the view (for temporary file:".*(~|.bck|.pyc)\e")
			protected:
				ListFileSystem();
				void init();
			public:
				DECLARE_WIDGET_FACTORY(ListFileSystem, "ListFileSystem");
				virtual ~ListFileSystem();
			protected:
				std::shared_ptr<ewol::resource::ColorFile> m_colorProperty; //!< theme color property.
				int32_t m_colorIdText; //!< Color of the text.
				int32_t m_colorIdBackground1; //!< Color of the Background.
				int32_t m_colorIdBackground2; //!< Color of the Background 2.
				int32_t m_colorIdBackgroundSelected; //!< Color of line selected.
			protected : // Derived function from the List...
				virtual etk::Color<> getBasicBG();
				virtual uint32_t getNuberOfColomn();
				virtual bool getTitle(int32_t _colomn, std::string& _myTitle, etk::Color<>& _fg, etk::Color<>& _bg);
				virtual uint32_t getNuberOfRaw();
				virtual bool getElement(int32_t _colomn, int32_t _raw, std::string& _myTextToWrite, etk::Color<>& _fg, etk::Color<>& _bg);
				virtual bool onItemEvent(int32_t _IdInput, enum gale::key::status _typeEvent, int32_t _colomn, int32_t _raw, float _x, float _y);
			protected:
				// TODO: use shred_ptr
				std::vector<etk::FSNode *> m_list; //!< List of all element in the path. (they are filtered)
				/**
				 * @brief Clean the list of element.
				 */
				void clearList();
				/**
				 * @brief Regenerate the content of the view. this is actually not automation on the system update.
				 */
				virtual void regenerateView();
			protected:
				int32_t m_selectedLine; //!< Current Line ID that is selected
			public:
				/**
				 * @brief Select a specific file in the path
				 * @param[in] _data File to selested.
				 */
				virtual void setSelect(const std::string& _data);
				/**
				 * @brief Get the current selected file/folder/... in the list
				 * @return the String of the element selected.
				 */
				std::string getSelect() const ;
			protected:
				virtual void onChangePropertyPath();
				virtual void onChangePropertyFile();
				virtual void onChangePropertyShowFile();
				virtual void onChangePropertyShowFolder();
				virtual void onChangePropertyShowHidden();
				virtual void onChangePropertyFilter();
		};
	};
};


