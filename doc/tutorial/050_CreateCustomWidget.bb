=?=Create a custum widget=?=

To create a custum widget, this is as simple as complex.
The first things to do is to choice a methode to display you widget:
:** [b]Direct mode:[/b] display on openGL engine with your prefered methode (some help for shaders)
:** [b]Compositing:[/b] display with a toolbox for drawing on openGL
:** [b]Shaper:[/b] this is a special mode of compositing
:** [b]Add capacity:[/b] this could be interesting to increase some capacity of a widget...

==Create the widget structure==
===Header===
[code style=c++]
	#include <ewol/widget/Widget.h>
	namespace appl {
		class myWidget : public ewol::Widget {
			public:
				myWidget(void) {};
				~myWidget(void) {};
			public: // herited function
				void draw(void);
				void onRegenerateDisplay(void);
		}
	}
[/code]

We can show that we had two function, the first is call every time we render the widget (as the number of fps) "draw()".
And the second that is call only when we need to redraw the widget (after the user call markToRedraw() ) "onRegenerateDisplay()".





