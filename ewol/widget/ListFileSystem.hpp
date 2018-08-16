/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ewol/widget/List.hpp>
#include <etk/os/FSNode.hpp>
#include <ewol/resource/ColorFile.hpp>
#include <esignal/Signal.hpp>

namespace ewol {
	namespace widget {
		class ListFileSystem;
		using ListFileSystemShared = ememory::SharedPtr<ewol::widget::ListFileSystem>;
		using ListFileSystemWeak = ememory::WeakPtr<ewol::widget::ListFileSystem>;
		/**
		 * @brief Generic display folder class. This widget display the content of a single folder :
		 */
		class ListFileSystem : public ewol::widget::List {
			public: // signals
				esignal::Signal<etk::String> signalFileSelect; //!< @event "file-select" Generated when a file is selected.
				esignal::Signal<etk::String> signalFileValidate; //!< @event "file-validate" Generate when the user validate (return) or double click on the element
				esignal::Signal<etk::String> signalFolderSelect;
				esignal::Signal<etk::String> signalFolderValidate;
			public: // properties
				eproperty::Value<etk::String> propertyPath; //!< Current folder that display point on.
				eproperty::Value<etk::String> propertyFile; //!< current selected file
				eproperty::Value<bool> propertyShowFile; //!< Show files elements
				eproperty::Value<bool> propertyShowFolder; //!< Display the folders elements
				eproperty::Value<bool> propertyShowHidden; //!< Display hidden elements
				eproperty::Value<etk::String> propertyFilter; //!< Regular expression to filter the view (for temporary file:".*(~|.bck|.pyc)\e")
			protected:
				ListFileSystem();
			public:
				DECLARE_WIDGET_FACTORY(ListFileSystem, "ListFileSystem");
				virtual ~ListFileSystem();
			protected:
				ememory::SharedPtr<ewol::resource::ColorFile> m_colorProperty; //!< theme color property.
				int32_t m_colorIdText; //!< Color of the text.
				int32_t m_colorIdBackground1; //!< Color of the Background.
				int32_t m_colorIdBackground2; //!< Color of the Background 2.
				int32_t m_colorIdBackgroundSelected; //!< Color of line selected.
			protected:
				etk::Color<> getBasicBG() override;
				uint32_t getNuberOfColomn() override;
				bool getTitle(int32_t _colomn, etk::String& _myTitle, etk::Color<>& _fg, etk::Color<>& _bg) override;
				uint32_t getNuberOfRaw() override;
				fluorine::Variant getData(int32_t _role, const ivec2& _pos) override;
				bool onItemEvent(int32_t _IdInput, enum gale::key::status _typeEvent, int32_t _colomn, int32_t _raw, float _x, float _y) override;
			protected:
				// TODO: use shred_ptr
				etk::Vector<etk::FSNode *> m_list; //!< List of all element in the path. (they are filtered)
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
				virtual void setSelect(const etk::String& _data);
				/**
				 * @brief Get the current selected file/folder/... in the list
				 * @return the String of the element selected.
				 */
				etk::String getSelect() const ;
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


