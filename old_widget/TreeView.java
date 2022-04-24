
class TreeView extends WidgetList {
	public: // signals
	public: // properties
		eproperty::Value<float> propertyOffsetTreeView; //!< indentation betwwen every new element.
		eproperty::Value<float> propertyIconTreeViewSize; //!< Size of the icon.
		eproperty::Value<bool> propertyTextIsDecorated; //!< Size of the icon.
	protected:
		TreeView();
		void init() ;
	public:
		 ~TreeView();
	protected:
		/**
		 * Calculate an element size to extimate the render size.
		 * @note Does not generate the with the same size.
		 * @param _pos Position of colomn and Raw of the element.
		 * @return The estimate size of the element.
		 */
		Vector2f calculateElementSize( Vector2i _pos) ;
		/**
		 * Draw an element in the specific size and position.
		 * @param _pos Position of colomn and Raw of the element.
		 * @param _start Start display position.
		 * @param _size Render raw size
		 * @return The estimate size of the element.
		 */
		void drawElement( Vector2i _pos,  Vector2f _start,  Vector2f _size) ;
	protected:
		 void onChangePropertyOffsetTreeView();
		 void onChangePropertyTextDecorated();
		
		boolean onItemEvent( ewol::event::Input _event,  Vector2i _pos,  Vector2f _mousePosition) ;
		 void onItemExpandEvent( Vector2i _pos) { };
}

