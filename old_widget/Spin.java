
		/**
		 * a composed Spin is a Spin with an inside composed with the specify XML element 
		 * ==> this permit to generate standard element simple
		 */
		class Spin : public ewol::widget::SpinBase {
			public:
				// Event list of properties
				esignal::Signal<long> signalValue;
				esignal::Signal<double> signalValueDouble;
			public:
				eproperty::Value<long> propertyValue; //!< Current value of the Spin.
				eproperty::Value<long> propertyMin; //!< Minimum value
				eproperty::Value<long> propertyMax; //!< Maximum value
				eproperty::Value<long> propertyIncrement; //!< Increment value
				eproperty::Value<int8_t> propertyMantis; //!< number of value under '.' value
			protected:
				/**
				 * Constructor
				 * @param _mode mode to display the spin
				 * @param _shaperName Shaper file properties
				 */
				Spin();
			public:
				DECLARE_WIDGET_FACTORY(Spin, "Spin");
				/**
				 * Destructor
				 */
				 ~Spin();
			protected:
				 void checkValue(long _value);
				 void updateGui();
			protected:
				void onCallbackUp();
				void onCallbackDown();
			protected:
				esignal::Connection this.connectionEntry;
				esignal::Connection this.connectionButtonUp;
				esignal::Connection this.connectionButtonDown;
			protected:
				 void onChangePropertyValue();
				 void onChangePropertyMin();
				 void onChangePropertyMax();
				 void onChangePropertyIncrement();
				 void onChangePropertyMantis();
}
