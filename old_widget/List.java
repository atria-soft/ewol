
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
class List extends WidgetScrolled {
	public List();
	public void calculateMinMaxSize() ;
	// drawing capabilities ....
	protected List<ememory::Ptr<ewol::Compositing>> listOObject; //!< generic element to display...
	protected List<Integer> listSizeX; //!< size of every colomns
	protected List<Integer> listSizeY; //!< size of every rows
	protected etk::Map<String, ememory::Ptr<ewol::Compositing>> compositingElements;
	protected void addComposeElemnent( String _name,  ememory::Ptr<ewol::Compositing> _element);
	protected void clearComposeElemnent();
	protected void removeComposeElemnent();
	protected ememory::Ptr<ewol::Compositing> getComposeElemnent( String _name);
	public void clearOObjectList();
	// list properties ...
	protected int paddingSizeX;
	protected int paddingSizeY;
	protected int displayStartRaw; //!< Current starting diaplayed raw
	protected int displayCurrentNbLine; //!< Number of line in the display
	protected int nbVisibleRaw; // set the number of visible raw (calculate don display)
	// function call to display the list :
	protected etk::Color<> getBasicBG() {
			return etk::Color<>(0xFF, 0xFF, 0xFF, 0xFF);
		}
		
		/**
		 * Get the number of colomn and row availlable in the list
		 * @return Number of colomn and row
		 */
	protected Vector2i getMatrixSize() ;
		
	protected fluorine::Variant getData(int _role,  Vector2i _pos) {
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
		 * Calculate an element size to extimate the render size.
		 * @note Does not generate the with the same size.
		 * @param _pos Position of colomn and Raw of the element.
		 * @return The estimate size of the element.
		 */
		protected  Vector2f calculateElementSize( Vector2i _pos);
		/**
		 * Draw an element in the specific size and position.
		 * @param _pos Position of colomn and Raw of the element.
		 * @param _start Start display position.
		 * @param _size Render raw size
		 * @return The estimate size of the element.
		 */
		protected void drawElement( Vector2i _pos,  Vector2f _start,  Vector2f _size);
		/**
		 * Draw the background
		 */
		protected void drawBackground();
		
		protected boolean onItemEvent( ewol::event::Input _event,  Vector2i _pos,  Vector2f _mousePosition) {
			return false;
		}
		/**
		 * set a raw visible in the main display
		 * @param _id Id of the raw that might be visible.
		 */
		//void setRawVisible(int _id);
		protected void onGetFocus() ;
	protected void onLostFocus() ;
	protected void onDraw() ;
	public void onRegenerateDisplay() ;
	public boolean onEventInput( ewol::event::Input _event) ;
}

