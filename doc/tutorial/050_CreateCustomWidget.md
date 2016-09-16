EWOL: Create a custom Widget                                {#ewol_tutorial_create_custom_widget}
============================

@tableofcontents

Objectifs:                                {#ewol_tutorial_create_custom_widget_objectif}
==========

To create a custum widget, this is as simple as complex.
The first things to do is to choice a methode to display you widget:
  - [b]Direct mode:[/b] display on openGL engine with your prefered methode (some help for shaders)
  - [b]Compositing:[/b] display with a toolbox for drawing on openGL
  - [b]Shaper:[/b] this is a special mode of compositing
  - [b]Add capacity:[/b] this could be interesting to increase some capacity of a widget...

Create the widget structure                                {#ewol_tutorial_create_custom_widget_structure}
===========================

Header
------

```{.cpp}
	#include <ewol/widget/Widget.h>
	#include <ewol/compisiting/Widget.h>
	namespace appl {
		class myWidget : public ewol::Widget {
			private:
				ewol::compositing::Drawing m_draw; //!< simple openGL drawing tool
			public:
				myWidget(void) {};
				~myWidget(void) {};
			public: // herited function
				void onDraw(void);
				void onRegenerateDisplay(void);
		}
	}
```

We can show that we have two function, the first is call every time we render the widget (as the number of fps) "onDraw()".
And the second that is call only when we need to redraw the widget (after the user call markToRedraw() ) "onRegenerateDisplay()".

basic code
----------

we can define some basic functions:

The constructor:
```{.cpp}
appl::myWidget::myWidget() {
	addObjectType("appl::widget::VectorDisplay");
}
```

The draw function:
```{.cpp}
void appl::myWidget::onDraw() {
	m_draw.draw();
}
```

The drawing area function (where we create the patern.).
```{.cpp}
void appl::myWidget::onRegenerateDisplay() {
	//!< Check if we really need to redraw the display, if not needed, we redraw the previous data ...
	if (needRedraw() == false) {
		return;
	}
	// remove previous data
	m_draw.clear();
	// set background
	m_draw.setColor(etk::color::black);
	m_draw.setPos(vec2(0,0));
	m_draw.rectangleWidth(m_size);
	m_draw.setColor(etk::color::green);
	m_draw.setPos(m_size*0.2);
	m_draw.rectangleWidth(m_size*0.5);
}
```


A more complex sample is availlable in **"ewol-sample-CustomWidget"**

