/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/Color.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/WidgetScrolled.hpp>
#include <ewol/compositing/Compositing.hpp>
#include <fluorine/Variant.hpp>

namespace ewol {
	namespace widget {
		class List;
		using List = ememory::Ptr<ewol::widget::List>;
		using ListWeak = ememory::WeakPtr<ewol::widget::List>;
		
		enum ListRole {
			Text = 11234, // string
			IsSelected, // bool
			IsExpand, // bool
			Icon, // string
			ChildCount, // uint_t
			HaveChild, // bool
			ParentId, // uint_t
			BgColor, // color
			FgColor, // color
			DistanceToRoot, // uint_t
			// Every other role must be set here:
			EndOfEwolRole
		};
		
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class List : public ewol::widget::WidgetScrolled {
			protected:
				List();
				void init() ;
			public:
				 ~List();
				void calculateMinMaxSize() ;
			// drawing capabilities ....
			protected:
				List<ememory::Ptr<ewol::Compositing>> this.listOObject; //!< generic element to display...
				List<int> this.listSizeX; //!< size of every colomns
				List<int> this.listSizeY; //!< size of every rows
			protected:
				etk::Map<String, ememory::Ptr<ewol::Compositing>> this.compositingElements;
				void addComposeElemnent( String _name,  ememory::Ptr<ewol::Compositing> _element);
				void clearComposeElemnent();
				void removeComposeElemnent();
				ememory::Ptr<ewol::Compositing> getComposeElemnent( String _name);
			public:
				void clearOObjectList();
			// list properties ...
			protected:
				int this.paddingSizeX;
				int this.paddingSizeY;
				int this.displayStartRaw; //!< Current starting diaplayed raw
				int this.displayCurrentNbLine; //!< Number of line in the display
				int this.nbVisibleRaw; // set the number of visible raw (calculate don display)
			protected:
				// function call to display the list :
				 etk::Color<> getBasicBG() {
					return etk::Color<>(0xFF, 0xFF, 0xFF, 0xFF);
				}
				
				/**
				 * @brief Get the number of colomn and row availlable in the list
				 * @return Number of colomn and row
				 */
				 Vector2i getMatrixSize() ;
				
				 fluorine::Variant getData(int _role,  Vector2i _pos) {
					switch (_role) {
						case ListRole::Text:
							return "";
						case ListRole::FgColor:
							return etk::Color<>(0x00, 0x00, 0x00, 0xFF);
						case ListRole::BgColor:
							if (_pos.y() % 2 == 0) {
								return etk::Color<>(0xFF, 0xFF, 0xFF, 0xFF);
							}
							return etk::Color<>(0x7F, 0x7F, 0x7F, 0xFF);
					}
					return fluorine::Variant();
				};
				/**
				 * @brief Calculate an element size to extimate the render size.
				 * @note Does not generate the with the same size.
				 * @param[in] _pos Position of colomn and Raw of the element.
				 * @return The estimate size of the element.
				 */
				 Vector2f calculateElementSize( Vector2i _pos);
				/**
				 * @brief Draw an element in the specific size and position.
				 * @param[in] _pos Position of colomn and Raw of the element.
				 * @param[in] _start Start display position.
				 * @param[in] _size Render raw size
				 * @return The estimate size of the element.
				 */
				 void drawElement( Vector2i _pos,  Vector2f _start,  Vector2f _size);
				/**
				 * @brief Draw the background
				 */
				 void drawBackground();
				
				 boolean onItemEvent( ewol::event::Input _event,  Vector2i _pos,  Vector2f _mousePosition) {
					return false;
				}
				/**
				 * @brief set a raw visible in the main display
				 * @param[in] _id Id of the raw that might be visible.
				 */
				//void setRawVisible(int _id);
			protected:
				void onGetFocus() ;
				void onLostFocus() ;
				void onDraw() ;
			public:
				void onRegenerateDisplay() ;
				boolean onEventInput( ewol::event::Input _event) ;
		};
	};
};

