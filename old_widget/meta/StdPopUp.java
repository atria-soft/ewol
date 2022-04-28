
/**
 * The std pop up widget is a siple message widget to notify user of some simple things, like:
 *
 * [pre]
 * 	+---------------------------------+---+---+---+
 * 	| Windows name...                 | _ | O | X |
 * 	+---------------------------------+---+---+---+
 * 	|                                             |
 * 	|                                             |
 * 	|                                             |
 * 	|            +-------------------+            |
 * 	|            | Title:            |            |
 * 	|            |                   |            |
 * 	|            | Message to diplay |            |
 * 	|            | to user           |            |
 * 	|            |                   |            |
 * 	|            |             Close |            |
 * 	|            +-------------------+            |
 * 	|                                             |
 * 	|                                             |
 * 	|                                             |
 * 	+---------------------------------------------+
 * [/pre]
 */
class StdPopUp : public ewol::widget::PopUp {
	public: // properties:
		String propertyTitle; //!< Title of the pop-up
		String propertyComment; //!< comment in the pop-up (can be decorated text)
	protected:
		/**
		 * std-pop-up ructor.
		 */
		StdPopUp();
	protected:
		ewol::widget::Label this.title; //!< Title Label widget
		/**
		 * property callback when request a change of the title.
		 */
		void onChangePropertyTitle();
		ewol::widget::Label this.comment; //!< Comment label widget
		/**
		 * property callback when request a change of the Comment.
		 */
		void onChangePropertyComment();
	protected:
		ewol::widget::Sizer subBar; //!< subwidget bar containing all the button.
	public:
		/**
		 * Add a buttom button.
		 * @param _text Decorated text to diplay in button.
		 */
		Button addButton( String _text, boolean _autoExit=false);
	public:
		 void onCallBackButtonExit();
}

