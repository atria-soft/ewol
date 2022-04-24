import org.atriasoft.etk.math.Vector2f;

/**
 * Widget to integrate a scrool bar in a widget. This is not a stadalone widget.
 */
class WidgetScrolled extends Widget {
	public static final int CALCULATE_SIMULTANEOUS_FINGER = 5;
	protected etk::Uri propertyShapeVert; //!< Vertical shaper name
	protected etk::Uri propertyShapeHori; //!< Horizontal shaper name
	public enum scrollingMode {
		scroolModeNormal, //!< No Zoom , can UP and down, left and right
		scroolModeCenter, //!< Zoom enable, no move left and right
		scroolModeGame, //!< Zoom enable, no move left and right
	};
	private ewol::compositing::Shaper shaperH; //!< Compositing theme Horizontal.
	private ewol::compositing::Shaper shaperV; //!< Compositing theme Vertical.
	protected Vector2f originScrooled = Vector2f.ZERO; //!< pixel distance from the origin of the display (Bottum left)
	protected Vector2f maxSize; //!< Maximum size of the Widget ==> to display scrollbar
	protected Vector2f limitScrolling = Vector2f.ZERO; //!< Mimit scrolling represent the propertion of the minimel scrolling activate (0.2 ==> 20% migt all time be visible)
	// Mouse section :
	private enum scrollingMode scroollingMode = scrollingMode.scroolModeNormal; //!< mode of management of the scrooling
	private float pixelScrolling = 20;
	private Vector2f highSpeedStartPos;
	private enum Scroll::highSpeedMode highSpeedMode = Scroll::speedModeDisable;
	private int highSpeedButton = -1;
	private KeyType highSpeedType = KeyType.unknow;
	// finger section:
	private boolean singleFingerMode = true; //!< in many case the moving in a subwidget is done with one finger, it is enought ==> the user select...
	public 
		/**
		 * Set the single finger capabilities/
		 * @param _status True if single inger mode, two otherwise/
		 */
		void setSingleFinger(boolean _status);
		/**
		 * Get the single finger capabilities
		 * @return true The single finger mode is active
		 * @return false The To finger mode is active
		 */
		boolean getSingleFinger() {
			return this.singleFingerMode;
		}
		/**
		 * Reset the scoll of the subWidget
		 */
		void resetScrollOrigin() {
			this.originScrooled = Vector2f(0,0);
		}
		private boolean fingerPresent[] = {false, false, false, false, false};
	private boolean fingerScoolActivated = false;
	private Vector2f fingerMoveStartPos[] = new Vector2f[CALCULATE_SIMULTANEOUS_FINGER];
		/**
		 * Scroll Widget main ructor to be herited from an other widget (this is not a stand-alone widget)
		 * @param _shaperName Shaper name if the scrolled widget.
		 */
	public WidgetScrolled() {
		onChangePropertyShapeVert();
		onChangePropertyShapeHori();
	}
	protected void onDraw() ;
		public void onRegenerateDisplay() ;
	public boolean onEventInput( ewol::event::Input _event) ;
	public void systemDraw( ewol::DrawProperty _displayProp) ;
		/**
		 * For mouse event when we have a scrolling UP and dows, specify the number of pixel that we scrooled
		 * @param _nbPixel number of pixel scrolling
		 */
		protected void setScrollingSize(float _nbPixel) {
			this.pixelScrolling = _nbPixel;
		};
		/**
		 * Specify the mode of scrolling for this windows
		 * @param _newMode the selected mode for the scrolling...
		 */
		protected void scroolingMode(enum scrollingMode _newMode);
		/**
		 * set the specific mawimum size of the widget
		 * @param _localSize new Maximum size
		 */
		protected void setMaxSize( Vector2f _localSize) {
			this.maxSize = _localSize;
		};
		/**
		 * Request a specific position for the scrolling of the current windows.
		 * @param _borderWidth size of the border that requested the element might not to be
		 * @param _currentPosition Position that is requested to view
		 * @param _center True if the position might be at the center of the widget
		 */
		protected void setScrollingPositionDynamic(Vector2f _borderWidth,  Vector2f _currentPosition, boolean _center = false);
		/**
		 * set the scrolling limit when arriving at he end of the widget
		 * @param _poucentageLimit pourcent of the limit of view nothing in the widget when arriving at the end ...
		 */
		protected void setLimitScrolling(float _poucentageLimit) {
			_poucentageLimit = etk::avg(0.1f, _poucentageLimit,1.0f);
			this.limitScrolling = Vector2f(_poucentageLimit, _poucentageLimit);
		};
		/**
		 * set the scrolling limit when arriving at he end of the widget
		 * @param _poucentageLimit pourcent of the limit of view nothing in the widget when arriving at the end for axis specific...
		 */
		protected void setLimitScrolling( Vector2f _poucentageLimit) {
			this.limitScrolling = Vector2f(etk::avg(0.1f, _poucentageLimit.x(),1.0f), etk::avg(0.1f, _poucentageLimit.y(),1.0f));
		};
	protected void onChangePropertyShapeVert();
	protected void onChangePropertyShapeHori();
}
