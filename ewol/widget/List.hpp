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
		using ListShared = ememory::SharedPtr<ewol::widget::List>;
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
			// Every other role must be set here:
			EndOfEwolRole
		};
		
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class List : public ewol::widget::WidgetScrolled {
			protected:
				List();
			public:
				virtual ~List();
				void calculateMinMaxSize() override;
				void setLabel(etk::String _newLabel);
			// drawing capabilities ....
			private:
				etk::Vector<ewol::Compositing*> m_listOObject; //!< generic element to display...
				etk::Vector<int32_t> m_listSizeX; //!< size of every colomns
				etk::Vector<int32_t> m_listSizeY; //!< size of every rows
			public:
				void addOObject(ewol::Compositing* _newObject, int32_t _pos=-1);
				void clearOObjectList();
			// list properties ...
			private:
				int32_t m_paddingSizeX;
				int32_t m_paddingSizeY;
				int32_t m_displayStartRaw; //!< Current starting diaplayed raw
				int32_t m_displayCurrentNbLine; //!< Number of line in the display
				int32_t m_nbVisibleRaw; // set the number of visible raw (calculate don display)
			protected:
				// function call to display the list :
				virtual etk::Color<> getBasicBG() {
					return etk::Color<>(0xFF, 0xFF, 0xFF, 0xFF);
				}
				
				/**
				 * @brief Get the number of colomn and row availlable in the list
				 * @return Number of colomn and row
				 */
				virtual ivec2 getMatrixSize() const;
				
				virtual fluorine::Variant getData(int32_t _role, const ivec2& _pos) {
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
				virtual vec2 calculateElementSize(const ivec2& _pos);
				/**
				 * @brief Draw an element in the specific size and position.
				 * @param[in] _pos Position of colomn and Raw of the element.
				 * @param[in] _start Start display position.
				 * @param[in] _size Render raw size
				 * @return The estimate size of the element.
				 */
				virtual void drawElement(const ivec2& _pos, const vec2& _start, const vec2& _size);
				/**
				 * @brief Draw the background
				 */
				virtual void drawBackground();
				
				virtual bool onItemEvent(int32_t _IdInput, enum gale::key::status _typeEvent, const ivec2& _pos, const vec2& _mousePosition) {
					return false;
				}
				/**
				 * @brief set a raw visible in the main display
				 * @param[in] _id Id of the raw that might be visible.
				 */
				//void setRawVisible(int32_t _id);
			protected:
				void onGetFocus() override;
				void onLostFocus() override;
				void onDraw() override;
			public:
				void onRegenerateDisplay() override;
				bool onEventInput(const ewol::event::Input& _event) override;
		};
	};
};

