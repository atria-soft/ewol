## Signal System (esignal)

Widgets communicate via signals. Two signal types:
- `SignalEmpty` - no data
- `Signal<T>` - carries data of type T

### Connecting Signals

```java
// Lambda connection
button.signalClick.connect(() -> System.out.println("clicked"));
slider.signalValue.connect(value -> System.out.println("value: " + value));

// Method reference with auto-destroy (weak pointer, cleaned when object is destroyed)
button.signalClick.connectAuto(this, MyClass::onButtonClick);

// Static method callback pattern (used throughout ewol):
// The static method receives the target object as first parameter
public static void onButtonClick(MyClass self) {
    self.doSomething();
}
button.signalClick.connectAuto(this, MyClass::onButtonClick);

// With data
public static void onSliderChange(MyClass self, Float value) {
    self.updateValue(value);
}
slider.signalValue.connectAuto(this, MyClass::onSliderChange);
```

### Emitting Signals

```java
signalClick.emit();              // SignalEmpty
signalValue.emit(42.0f);        // Signal<Float>
signalModify.emit("new text");  // Signal<String>
```

## Widget Lifecycle & Custom Widgets

### Creating a Custom Widget

```java
public class MyWidget extends Widget {
    private CompositingText textDraw = new CompositingText();

    public MyWidget() {
        propertyCanFocus = true;   // enable focus
        setMouseLimit(1);          // accept mouse events
    }

    @Override
    public void calculateMinMaxSize() {
        super.calculateMinMaxSize();
        // Set minimum size based on content
        this.minSize = Vector2f.max(this.minSize, new Vector2f(100, 30));
    }

    @Override
    protected void onRegenerateDisplay() {
        if (!needRedraw()) return;
        textDraw.clear();
        textDraw.setPos(new Vector2f(10, 10));
        textDraw.print("Hello");
        textDraw.flush();
    }

    @Override
    protected void onDraw() {
        textDraw.draw();
    }

    @Override
    protected boolean onEventInput(EventInput event) {
        if (event.inputId() == 1 && event.status() == KeyStatus.pressSingle) {
            // Handle click
            return true;  // consumed
        }
        return false;
    }

    @Override
    protected boolean onEventEntry(EventEntry event) {
        // Keyboard events
        if (event.type() == KeyKeyboard.CHARACTER && event.status() == KeyStatus.down) {
            char c = event.getChar();
            return true;
        }
        return false;
    }

    @Override
    protected void onGetFocus() { markToRedraw(); }

    @Override
    protected void onLostFocus() { markToRedraw(); }
}
```

### Key Override Methods

| Method | When to Override |
|--------|-----------------|
| `calculateMinMaxSize()` | Set min/max size based on content |
| `onRegenerateDisplay()` | Rebuild rendering data (compositing). Check `needRedraw()` first |
| `onDraw()` | Draw compositing objects to screen |
| `onEventInput(EventInput)` | Handle mouse/touch events |
| `onEventEntry(EventEntry)` | Handle keyboard events |
| `onGetFocus()` / `onLostFocus()` | Focus state changes |
| `onChangeSize()` | Size has been set by parent |

### Rendering with Compositing

```java
// Text rendering
CompositingText text = new CompositingText();
text.clear();
text.setFontSize(14);
text.setPos(new Vector2f(x, y));
text.print("plain text");
text.printDecorated("<b>bold</b> <i>italic</i>");
text.flush();  // upload to GPU
// In onDraw():
text.draw();

// Vector drawing (rectangles, lines)
CompositingGC gc = new CompositingGC();
gc.clear();
gc.setPaintFillColor(Color.RED);
gc.addRectangle(start, stop, new Insets(0), new BorderRadius(5));
gc.flush();
// In onDraw():
gc.draw();

// Image rendering
CompositingImage img = new CompositingImage();
img.setSource(new Uri("DATA", "image.png", "my-app"));
// ...

// SVG rendering
CompositingSVG svg = new CompositingSVG();
svg.setSource(new Uri("DATA", "icon.svg", "my-app"));
// ...
```

## Common Patterns

### Dimension and Distance Types

```java
// Dimension2f: size with distance unit
new Dimension2f(new Vector2f(100, 50), Distance.PIXEL)
new Dimension2f(new Vector2f(3, 2), Distance.CENTIMETER)
new Dimension2f(new Vector2f(80, 80), Distance.POURCENT)
Dimension2f.ZERO

// DimensionInsets: uniform or per-side insets
new DimensionInsets(4)           // uniform 4px
new DimensionInsets(4, 8, 4, 8) // top, right, bottom, left
```

### URI System

```java
// Resources bundled with the library
new Uri("THEME", "color/Button.json", "ewol")

// Application data
new Uri("DATA", "images/logo.png", "my-app")

// Register app resources in main():
Uri.setApplication(Main.class, "my-app/");
```

### Keyboard Shortcuts

```java
// Add shortcuts to a widget
shortCutAdd("ctrl+s", "save");       // ctrl+S triggers "save"
shortCutAdd("ctrl+shift+z", "redo");
shortCutAdd("F5", "refresh");

// Listen for shortcut events
signalShortcut.connect(message -> {
    switch(message) {
        case "save": save(); break;
        case "redo": redo(); break;
    }
});
```

Supported modifiers: `ctrl`, `shift`, `alt`, `meta`
Supported special keys: `F1`-`F12`, `LEFT`, `RIGHT`, `UP`, `DOWN`, `PAGEUP`, `PAGEDOWN`, `START`, `END`, `INSERT`, `PRINT`

