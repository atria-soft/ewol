/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ewol/widget/List.hpp>
#include <ewol/resource/ColorFile.hpp>
#include <esignal/Signal.hpp>

namespace ewol {
	namespace widget {
		class ListFileSystem;
		using ListFileSystem = ememory::Ptr<ewol::widget::ListFileSystem>;
		using ListFileSystemWeak = ememory::WeakPtr<ewol::widget::ListFileSystem>;
		/**
		 * Generic display folder class. This widget display the content of a single folder :
		 */
		class ListFileSystem : public ewol::widget::List {
			public: // signals
				esignal::Signal<etk::Path> signalFileSelect; //!< @event "file-select" Generated when a file is selected.
				esignal::Signal<etk::Path> signalFileValidate; //!< @event "file-validate" Generate when the user validate (return) or double click on the element
				esignal::Signal<etk::Path> signalFolderSelect;
				esignal::Signal<etk::Path> signalFolderValidate;
			public: // properties
				eproperty::Value<etk::Path> propertyPath; //!< Current folder that display point on.
				eproperty::Value<etk::Path> propertyFile; //!< current selected file
				eproperty::Value<bool> propertyShowFile; //!< Show files elements
				eproperty::Value<bool> propertyShowFolder; //!< Display the folders elements
				eproperty::Value<bool> propertyShowHidden; //!< Display hidden elements
				eproperty::Value<String> propertyFilter; //!< Regular expression to filter the view (for temporary file:".*(~|.bck|.pyc)\e")
			protected:
				ListFileSystem();
			public:
				DECLARE_WIDGET_FACTORY(ListFileSystem, "ListFileSystem");
				 ~ListFileSystem();
			protected:
				ememory::Ptr<ewol::resource::ColorFile> this.colorProperty; //!< theme color property.
				int this.colorIdText; //!< Color of the text.
				int this.colorIdBackground1; //!< Color of the Background.
				int this.colorIdBackground2; //!< Color of the Background 2.
				int this.colorIdBackgroundSelected; //!< Color of line selected.
			protected:
				etk::Color<> getBasicBG() ;
				Vector2i getMatrixSize()  ;
				fluorine::Variant getData(int _role,  Vector2i _pos) ;
				boolean onItemEvent( ewol::event::Input _event,  Vector2i _pos,  Vector2f _mousePosition) ;
			protected:
				List<etk::Path> this.list; //!< List of all element in the path. (they are filtered)
				/**
				 * Clean the list of element.
				 */
				void clearList();
				/**
				 * Regenerate the content of the view. this is actually not automation on the system update.
				 */
				 void regenerateView();
			protected:
				int this.selectedLine; //!< Current Line ID that is selected
			public:
				/**
				 * Select a specific file in the path
				 * @param _data File to selested.
				 */
				 void setSelect( etk::Path _data);
				/**
				 * Get the current selected file/folder/... in the list
				 * @return the String of the element selected.
				 */
				etk::Path getSelect()  ;
			protected:
				 void onChangePropertyPath();
				 void onChangePropertyFile();
				 void onChangePropertyShowFile();
				 void onChangePropertyShowFolder();
				 void onChangePropertyShowHidden();
				 void onChangePropertyFilter();
		};
	};
};


