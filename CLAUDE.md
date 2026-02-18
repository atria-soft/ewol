# EWOL - Widget Framework

Java widget framework with OpenGL rendering via GALE. Provides a complete UI toolkit with layout, input widgets, event system, and fluent API.

## Quick Start

### Minimal Application

```java
// 1. Main entry point
public class Main {
    public static void main(String[] args) {
        Ewol.init();
        Uri.setApplication(Main.class, "my-app/");
        Ewol.run(new MyApp(), args);
    }
}

// 2. Application lifecycle (implement EwolApplication)
public class MyApp implements EwolApplication {
    @Override
    public void onCreate(EwolContext context) {
        context.setSize(new Vector2f(800, 600));
        Configs.getConfigFonts().set("FreeSherif", 12);
        context.setWindows(new MyWindows());
    }
    @Override public void onStart(EwolContext context) {}
    @Override public void onStop(EwolContext context) {}
    @Override public void onPause(EwolContext context) {}
    @Override public void onResume(EwolContext context) {}
    @Override public void onDestroy(EwolContext context) {}
}

// 3. Main window
public class MyWindows extends Windows {
    public MyWindows() {
        setPropertyTitle("My Application");
        setSubWidget(
            Sizer.vertical()
                .expand(true, true)
                .fill(true, true)
                .add(new Label("Hello World!"))
                .add(Button.create("Click me").onClick(() -> System.out.println("Clicked!")))
        );
    }
}
```

## Architecture

### Class Hierarchy

```
EwolObject                    -- Base object with signals, naming, parent tracking
  Widget                      -- Base widget (position, size, focus, events, shortcuts)
    Container                 -- Single child widget container
      Box                     -- Container with border/padding/margin/color/radius drawing
        Button                -- Clickable button (contains a sub-widget, typically Label)
        Entry                 -- Text input field
        PopUp                 -- Popup overlay widget
        Select                -- Dropdown / ComboBox
      ScrollView              -- Scrollable single-child container with scrollbars
    ContainerN                -- Multiple children container
      Sizer                   -- Linear layout (HORIZONTAL or VERTICAL)
    Windows                   -- Top-level window (has subWidget + popUp stack + NotificationManager)
    Label                     -- Text display
    Slider                    -- Value slider with range (cached track + dynamic cursor)
    ColorGradient             -- HSL color gradient for color selection
    ImageDisplay              -- Image display widget
    Icon                      -- SVG icon display with color customization
    SplitPane                 -- Resizable split panel (two children)
    CheckBox                  -- Checkbox (Tick + Label composite)
    ProgressBar               -- Progress indicator (0.0 to 1.0)
    Spacer                    -- Empty space widget
    Tick                      -- Toggle indicator (cached SVG, used internally by CheckBox)
    WidgetScrolled            -- Base for scrollable widgets (scroll management, scrollbars)
      TreeView                -- Generic tree view with expand/collapse, columns, indent guides
        TreeFileSystem        -- File system browser (lazy-loading, extends TreeView)
    TreeNode<T>               -- Generic tree data node (parent/children/expanded/leaf)
    TreeColumn                -- Column definition (name, width, renderer)
    TreeCellRenderer          -- Functional interface for rendering a cell
    TreeColumnAction<T>       -- Record for column action signals
  meta/
    ColorPickerPopup          -- Color picker dialog (extends PopUp, pure Java)
    FileChooser               -- File browser dialog (extends PopUp, pure Java)
    SelectPopup               -- Selection popup
```

### Key Packages

| Package | Description |
|---------|-------------|
| `org.atriasoft.ewol` | Main entry point (`Ewol`), `Gravity`, `Padding`, `DrawProperty` |
| `org.atriasoft.ewol.widget` | All widgets |
| `org.atriasoft.ewol.widget.meta` | Composite widgets (`FileChooser`, `SelectPopup`, `ColorPickerPopup`) — all pure Java, no XML |
| `org.atriasoft.ewol.widget.menu` | Menu system (`MenuPopup`, `MenuItem`, `MenuBar`, `MenuSeparator`, `ShortcutBadge`) |
| `org.atriasoft.ewol.widget.notification` | Toast notification system (`NotificationManager`, `Toast`, `ToastType`, `ToastConfig`) |
| `org.atriasoft.ewol.context` | `EwolApplication`, `EwolContext`, `InputManager` |
| `org.atriasoft.ewol.object` | `EwolObject`, `ObjectManager`, `Worker` |
| `org.atriasoft.ewol.event` | `EventInput`, `EventEntry`, `EventShortCut`, `EventTime` |
| `org.atriasoft.ewol.compositing` | Rendering: `CompositingText`, `CompositingImage`, `CompositingSVG`, `CompositingGC`, `CompositingDrawing` |
| `org.atriasoft.ewol.resource` | `ResourceColorFile`, `ResourceFontSvg`, `ResourceTexturedFont` |

## Widget Properties (common to all widgets)

All widgets inherit from `Widget` and share these layout properties:

| Property | Setter | Fluent | Description |
|----------|--------|--------|-------------|
| expand | `setPropertyExpand(Vector2b)` | `.expand(x, y)` | Request expand when space available |
| fill | `setPropertyFill(Vector2b)` | `.fill(x, y)` | Fill available space |
| expand-if-free | `setPropertyExpandIfFree(Vector2b)` | `.expandIfFree(x, y)` | Expand only if free space exists |
| gravity | `setPropertyGravity(Gravity)` | `.gravity(g)` | Alignment within parent |
| hide | `setPropertyHide(boolean)` | `.hide(b)` | Show/hide widget |
| min-size | `setPropertyMinSize(Dimension2f)` | `.minSize(d)` / `.minSizePixel(x,y)` | Minimum size |
| max-size | `setPropertyMaxSize(Dimension2f)` | `.maxSize(d)` / `.maxSizePixel(x,y)` | Maximum size |
| focus | `setPropertyCanFocus(boolean)` | `.canFocus(b)` | Enable focus |

### Gravity Values

`Gravity.CENTER`, `Gravity.LEFT`, `Gravity.RIGHT`, `Gravity.TOP`, `Gravity.BOTTOM`, `Gravity.TOP_LEFT`, `Gravity.TOP_RIGHT`, `Gravity.BOTTOM_LEFT`, `Gravity.BOTTOM_RIGHT`

### Vector2b Constants

`Vector2b.TRUE` = (true,true), `Vector2b.FALSE` = (false,false), `Vector2b.TRUE_FALSE` = (true,false), `Vector2b.FALSE_TRUE` = (false,true)

## Widget Reference

### Windows

Top-level window. Set it via `context.setWindows(myWindows)`.

```java
// Setter API
Windows w = new Windows();
w.setPropertyTitle("My App");
w.setSubWidget(mainLayout);          // Set main content
w.popUpWidgetPush(popupWidget);      // Show popup on top

// Fluent API
new Windows().title("My App").content(mainLayout);
```

### Sizer (Layout)

Linear layout container. Main layout widget.

```java
// Factory methods
Sizer.horizontal()    // horizontal layout
Sizer.vertical()      // vertical layout

// Fluent API
Sizer.vertical()
    .expand(true, true)
    .fill(true, true)
    .border(new Dimension2f(new Vector2f(5, 5), Distance.PIXEL))
    .add(widget1)
    .add(widget2, widget3)   // varargs

// Setter API
Sizer s = new Sizer(DisplayMode.VERTICAL);
s.subWidgetAdd(widget);
s.subWidgetRemove(widget);
s.subWidgetRemoveAll();
```

### Label

Text display. Supports decorated text with HTML-like tags: `<b>bold</b>`, `<i>italic</i>`, `<font color="#FF0000">color</font>`.

```java
// Factory
Label.create("Hello <b>World</b>")

// Fluent API
new Label("text")
    .text("new text")
    .fontSize(14)             // 0 = system default
    .autoTranslate(false)     // disable translation
    .onPressed(() -> ...)     // click callback

// Setter API
label.setPropertyValue("text");
label.setPropertyFontSize(14);
```

### Button

Clickable button. Contains a sub-widget (typically a Label).

```java
// Factory methods
Button.create()               // empty button
Button.create("Click me")     // button with label

// Fluent API
Button.create("OK")
    .onClick(() -> doSomething())
    .onDown(() -> ...)
    .onUp(() -> ...)

// Setter API
Button b = Button.createLabelButton("text");
b.signalClick.connect(() -> ...);
b.signalDown.connect(() -> ...);
b.signalUp.connect(() -> ...);
b.signalEnter.connect(() -> ...);  // mouse enters
b.signalLeave.connect(() -> ...);  // mouse leaves
```

### Entry (Text Input)

Editable text field with cursor, selection, clipboard support.

```java
// Fluent API
Entry.create()
    .value("initial text")
    .placeholder("<i>Type here...</i>")
    .password(true)
    .maxCharacters(100)
    .regex("[a-zA-Z]*")
    .onModify(text -> System.out.println("Changed: " + text))
    .onEnter(text -> System.out.println("Enter: " + text))

// Setter API
Entry e = new Entry();
e.setPropertyValue("text");
e.setPropertyTextWhenNothing("<i>placeholder</i>");
e.setPropertyPassword(true);
e.setPropertyMaxCharacter(100);
e.signalModify.connect(text -> ...);   // Signal<String>
e.signalEnter.connect(text -> ...);    // Signal<String>
e.signalClick.connect(() -> ...);
```

Built-in shortcuts: `ctrl+a` select all, `ctrl+c` copy, `ctrl+x` cut, `ctrl+v` paste, `ctrl+w` clean.

### CheckBox

Toggle checkbox with label.

```java
// Factory
CheckBox.create("Accept terms")

// Fluent API
CheckBox.create("Enable feature")
    .checked(true)
    .onValueChange(value -> System.out.println("Checked: " + value))
    .onClick(() -> ...)

// Setter API
CheckBox cb = new CheckBox("label");
cb.setPropertyValue(true);
cb.toggle();
cb.isChecked();
cb.signalValue.connect(value -> ...);   // Signal<Boolean>
```

### Slider

Value slider with range.

```java
// Fluent API
Slider.create()
    .range(0, 100)          // min, max
    .value(50)
    .step(1.0f)
    .onValueChange(v -> System.out.println("Value: " + v))
    .trackColor(Color.GRAY)
    .fillColor(Color.BLUE)
    .cursorColor(Color.WHITE)
    .markers(25f, 50f, 75f)  // marker dots on track

// Setter API
Slider s = new Slider();
s.setPropertyMinimum(0f);
s.setPropertyMaximum(100f);
s.setPropertyStep(1f);
s.setPropertyValue(50f);
s.signalValue.connect(v -> ...);   // Signal<Float>
```

### ColorGradient

HSL color gradient for visual color selection. Horizontal axis = Hue (0-360), Vertical axis = Lightness (white top, black bottom). Rendering is split into a cached gradient background (regenerated only on resize) and a lightweight cursor overlay.

```java
// Factory
ColorGradient.create()
ColorGradient.create(Color.CORAL)

// Setter API
gradient.setFromColor(color);     // position cursor from RGB color
gradient.setHue(0.5f);            // 0.0-1.0
gradient.setLightness(0.5f);      // 0.0-1.0
gradient.setAlpha(200);           // 0-255
Color c = gradient.getCurrentColor();

// Signals
gradient.signalColorChanged   // Signal<Color> — emitted during drag and click
gradient.signalDragEnd        // SignalEmpty — emitted when drag ends (mouse up/leave)
gradient.isDragging()         // true while user is dragging
```

### ColorPickerPopup

Full color picker dialog (extends `PopUp`). Built entirely in pure Java — no XML layout.

```java
// Factory
ColorPickerPopup.create()
ColorPickerPopup.create(Color.CORAL)

// Fluent API
ColorPickerPopup.create(Color.CORAL)
    .title("Pick a color")
    .selectLabel("OK")
    .cancelLabel("Cancel")
    .onColorChanged(color -> ...)     // live color updates during interaction
    .onValidate(color -> ...)         // user confirmed selection
    .onCancel(() -> ...)              // user cancelled

// Show as popup
windows.popUpWidgetPush(picker);

// Signals
picker.signalColorChanged   // Signal<Color>
picker.signalValidate       // Signal<Color>
picker.signalCancel          // SignalEmpty
```

**Internal structure:** ColorGradient (HSL) + 4 Sliders (RGBA 0-255) + hex Entry + preview Spacers (current/original) + Select/Cancel buttons.

### Select (Dropdown / ComboBox)

Dropdown selection widget. Opens a popup with the list of items.

```java
// Factory methods
Select.create("Option 1", "Option 2", "Option 3")
Select.create(List.of("A", "B", "C"))

// Fluent API
Select.create()
    .items("Red", "Green", "Blue")
    .selectedIndex(0)
    .placeholder("Choose a color...")
    .onSelectionChanged(index -> ...)
    .onSelectionChanged((index, value) -> ...)    // with value

// Setter API
Select sel = new Select();
sel.addItem("item");
sel.setPropertySelectedIndex(0);
sel.getSelectedValue();             // returns String or null
sel.setSelectedValue("Green");
sel.signalSelectionChanged.connect(index -> ...);   // Signal<Integer>
```

### ScrollView

Scrollable container with scrollbars.

```java
// Fluent API
ScrollView.create()
    .content(someWidget)
    .showVertical(true)
    .showHorizontal(false)

// Scroll control
scrollView.scrollToTop();
scrollView.scrollToBottom();
scrollView.scrollToLeft();
scrollView.scrollToRight();
scrollView.setScrollOffset(new Vector2f(x, y));
```

Supports mouse wheel scrolling, scrollbar dragging, and `Shift+wheel` for horizontal scroll.

### SplitPane

Resizable split panel with two child widgets and a draggable separator.

```java
// Factory methods
SplitPane.horizontal()      // left | right
SplitPane.vertical()        // top / bottom

// Fluent API
SplitPane.vertical()
    .splitPosition(0.3f)          // 0.0 to 1.0
    .minSizes(100f, 50f)          // min first, min second
    .first(topWidget)
    .second(bottomWidget)

// Signal
splitPane.signalSplitChanged.connect(ratio -> ...);  // Signal<Float>
```

### ImageDisplay

Display images (PNG, JPG, etc.).

```java
ImageDisplay img = new ImageDisplay();
img.setPropertySource(new Uri("DATA", "images/photo.png", "my-app"));
img.setPropertyImageSize(new Dimension2f(new Vector2f(200, 200)));
img.setPropertyKeepRatio(true);
img.setPropertySmooth(true);
img.signalPressed.connect(() -> ...);
```

### Icon

SVG icon display with color customization. Black in SVG is replaced by `fillColor`, white by `backgroundColor`.

```java
// Factory
Icon.create("Home")          // loads icon by name

// Fluent API
Icon.create("Search")
    .fill(Color.RED)              // replaces black in SVG
    .background(Color.TRANSPARENT)  // replaces white in SVG
    .size(new Dimension2f(new Vector2f(32, 32)))
    .onPressed(() -> ...)
```

### ProgressBar

Progress indicator.

```java
ProgressBar pb = new ProgressBar();
pb.setPropertyValue(0.75f);         // 0.0 to 1.0
pb.setPropertyColorOn(Color.GREEN);
pb.setPropertyColorOff(Color.NONE);
pb.setPropertyColorBorder(Color.BLACK);
```

### Container

Single-child container.

```java
Container c = new Container();
c.setSubWidget(child);         // set child
c.subWidgetRemove();           // remove child

// Fluent
container.child(widget);
```

### Box

Container with visual properties (background color, border, padding, margin, border radius).

```java
Box box = new Box();
box.setPropertyColor(Color.WHITE);
box.setPropertyBorderColor(Color.BLACK);
box.setPropertyBorderWidth(new DimensionInsets(2));
box.setPropertyBorderRadius(new DimensionBorderRadius(5));
box.setPropertyPadding(new DimensionInsets(4));
box.setPropertyMargin(new DimensionInsets(2));
box.setSubWidget(child);
```

### PopUp

Popup overlay. Push it onto the Windows pop-up stack.

```java
PopUp popup = new PopUp();
popup.setSubWidget(contentWidget);
popup.setPropertyCloseOutEvent(true);  // close on outside click
windows.popUpWidgetPush(popup);        // show
windows.popUpWidgetPop();              // hide top popup
```

### Spacer

Empty space widget for layout spacing.

### TreeView

Generic tree view widget with configurable columns. Extends `WidgetScrolled`. Draws everything via `CompositingGC` + `CompositingText` (no child widgets per row). Features: expand/collapse chevrons, column headers with resize, selection highlight, vertical indent guide lines with hover/click.

```java
// Fluent API
TreeView.create()
    .rootNode(rootNode)
    .showRoot(false)
    .showHeaders(true)
    .alternateRowBackground(true)
    .column("Name", 250, TreeView.defaultLabelRenderer())
    .column("Info", 100, myCustomRenderer)
    .onSelect(node -> System.out.println("Selected: " + node.getLabel()))
    .onExpand(node -> loadChildren(node))
    .onCollapse(node -> ...)
    .onColumnAction(action -> ...)

// TreeNode — generic data model
TreeNode<MyData> root = new TreeNode<>(data, "Root");
TreeNode<MyData> child = new TreeNode<>(data, "Child");
root.addChild(child);
child.setExpanded(true);

// TreeCellRenderer — functional interface for custom column rendering
TreeCellRenderer renderer = (gc, text, node, pos, size, selected) -> {
    text.setPos(pos);
    text.print(node.getLabel());
};
```

**Signals:** `signalSelect` (node selected), `signalExpand` (node expanded), `signalCollapse` (node collapsed), `signalColumnAction` (column clicked).

**Indent guide lines:** Vertical gray lines at each indent level for expanded folders with more siblings below. Turn blue on hover. Single-click scrolls to the next sibling at that depth. Double-click toggles expand/collapse of the folder at that depth.

**Color theme:** `resources/ewol/theme/color/TreeView.json` — keys: `text`, `background1`, `background2`, `selected`, `chevron`, `headerBg`, `headerText`, `indentGuide`, `indentGuideHover`.

**Implementation notes:**
- Chevrons use `setPos()` + `addVertex()` (NOT `drawQuad()` which has a color bug)
- Per-cell text clipping bounded by viewport top to prevent bleeding into fixed header
- Column resize via drag on header borders

### TreeFileSystem

File system browser extending `TreeView` with lazy-loading. Uses `TreeNode<Path>` nodes. Placeholder child trick for deferred loading (directories start with a dummy child, replaced on expand).

```java
// Fluent API
TreeFileSystem.create("/home/user")
    .showFiles(true)
    .showFolders(true)
    .showHidden(false)
    .filter("*.java")
    .onFileSelect(path -> openFile(path))
    .onFolderSelect(path -> ...)

// Setter API
TreeFileSystem fs = new TreeFileSystem();
fs.setRootPath("/home/user");
fs.setShowFiles(true);
fs.setShowHidden(false);
fs.setFilter("*.java");
```

**Default columns:** Name (with yellow folder icon), Size, Modified, Type, Permissions (rwxrwxrwx), Owner, Group.

**Signals:** `signalFileSelect(Path)`, `signalFolderSelect(Path)`, plus all `TreeView` signals.

**Properties** (discoverable via ModelWidget): `root-path`, `show-files`, `show-folders`, `show-hidden`, `filter`. All setters trigger `reload()` to rebuild the tree.

### Toast Notifications

Toast notifications display temporary messages on top of the UI. Three types: `INFO` (blue), `WARNING` (orange), `ERROR` (red). Decoupled from the popup system.

```java
// Simple usage (from any widget with access to Windows)
getWindows().getNotification().info("Title", "Description");
getWindows().getNotification().warning("Title", "Description");
getWindows().getNotification().error("Title", "Description");

// Per-toast overrides
getWindows().getNotification()
    .showToast(ToastType.ERROR, "Critical", "Database connection lost")
    .overrideTimeout(15.0f)       // custom timeout (seconds)
    .overridePosition(Gravity.TOP_LEFT);

// Global configuration (set once, applies to all toasts)
getWindows().getNotification().getConfig()
    .position(Gravity.BOTTOM_RIGHT)   // default position
    .width(350f)                      // toast width in pixels
    .timeoutSeconds(5.0f)             // auto-dismiss delay
    .maxVisibleToasts(5)              // max stacked toasts
    .stackSpacing(8f)                 // spacing between toasts
    .edgeMargin(new Vector2f(16, 16)) // margin from screen edges
    .titleFontSize(14)
    .descriptionFontSize(11)
    .borderRadius(8f)
    .borderWidth(2f)
    .padding(10f);

// Custom notification manager (override DefaultNotificationManager)
myWindows.setNotificationManager(new MyCustomNotificationManager());
```

**Architecture:** `NotificationManager` (abstract) defines the interface. `DefaultNotificationManager` is the default implementation. `Windows.getNotification()` lazily creates a `DefaultNotificationManager`. Toasts are rendered after popups in `Windows.systemDraw()`, so they appear on top of everything. Timeouts are managed via `ObjectManager.periodicCall`.

**Key classes** in `org.atriasoft.ewol.widget.notification`:
- `ToastType` - enum: `INFO`, `WARNING`, `ERROR` (with colors)
- `ToastConfig` - global configuration with fluent setters
- `Toast` - individual toast widget (Box + Labels + close Icon)
- `NotificationManager` - abstract base class
- `DefaultNotificationManager` - default implementation

### Menu System

Context menus, dropdown menus, and menu bars. Located in `org.atriasoft.ewol.widget.menu`.

**Key classes:**
- `MenuPopup` - Popup overlay displaying a vertical list of items (follows `SelectPopup` pattern)
- `MenuItem` - A single menu item with optional icon, text, and shortcut badges
- `MenuSeparator` - Horizontal separator line between groups
- `MenuBar` - Horizontal bar of buttons, each opening a `MenuPopup` dropdown
- `ShortcutBadge` - Widget displaying a keyboard key in a rounded rectangle
- `ShortcutFormatter` - Utility to parse `"ctrl+shift+s"` into display tokens `["Ctrl", "Shift", "S"]`

#### Context Menu (right-click or programmatic)

```java
// On right-click
@Override
public boolean onEventInput(final EventInput event) {
    if (event.inputId() == 3 && event.status() == KeyStatus.pressSingle) {
        final Windows windows = Ewol.getContext().getWindows();
        if (windows != null) {
            MenuPopup.create()
                .item("Cut", null, "ctrl+x", () -> cut())
                .item("Copy", null, "ctrl+c", () -> copy())
                .item("Paste", null, "ctrl+v", () -> paste())
                .separator()
                .disabledItem("Undo")
                .item("Select All", null, "ctrl+a", () -> selectAll())
                .anchorAt(event.pos())
                .show(windows);
        }
        return true;
    }
    return false;
}

// Programmatic (e.g. from a button click)
MenuPopup.create()
    .item("Option 1", () -> doOption1())
    .item("Option 2", "icon-name", () -> doOption2())
    .separator()
    .disabledItem("Not Available")
    .anchorAt(new Vector2f(300, 200))
    .show(windows);
```

#### Menu Bar with Dropdowns

```java
MenuBar.create()
    .menu("File", () -> MenuPopup.create()
        .item("New", null, "ctrl+n", () -> newFile())
        .item("Open", null, "ctrl+o", () -> openFile())
        .item("Save", null, "ctrl+s", () -> save())
        .separator()
        .item("Quit", null, "alt+F4", () -> quit()))
    .menu("Edit", () -> MenuPopup.create()
        .item("Undo", null, "ctrl+z", () -> undo())
        .item("Redo", null, "ctrl+shift+z", () -> redo())
        .separator()
        .item("Cut", null, "ctrl+x", () -> cut())
        .item("Copy", null, "ctrl+c", () -> copy())
        .item("Paste", null, "ctrl+v", () -> paste()))
    .menu("Help", () -> MenuPopup.create()
        .item("About", () -> showAbout()));
```

**MenuPopup fluent API:**
| Method | Description |
|--------|-------------|
| `.item(text, action)` | Add item with text and click action |
| `.item(text, icon, action)` | Add item with icon |
| `.item(text, icon, shortcut, action)` | Add item with icon and shortcut badge |
| `.item(menuItem)` | Add a pre-built `MenuItem` instance |
| `.separator()` | Add horizontal separator |
| `.disabledItem(text)` | Add grayed-out non-clickable item |
| `.disabledItem(text, icon)` | Add disabled item with icon |
| `.anchorAt(Vector2f)` | Position popup at absolute coordinates |
| `.anchorBelow(Widget)` | Position popup below a widget (for dropdowns) |
| `.show(Windows)` | Push popup onto the popup stack |
| `.close()` | Close and remove from popup stack |

**MenuItem fluent API:**
| Method | Description |
|--------|-------------|
| `MenuItem.create(text)` | Factory to create an item |
| `.icon(iconName)` | Set SVG icon from `THEME/icon/` |
| `.shortcut(shortcut)` | Set shortcut badge (e.g. `"ctrl+s"`) |
| `.enabled(boolean)` | Enable/disable the item |
| `.onSelect(Runnable)` | Connect a click callback |

**Behaviors:**
- **Hover highlighting** on `MenuItem` (blue background) and `MenuBar` buttons
- **Walk-through**: hovering between `MenuBar` buttons while a menu is open switches the dropdown automatically
- **Escape key** closes the popup
- **Click outside** closes the popup
- **Icon alignment**: if any item in a menu has an icon, all items reserve icon space for alignment
- **Disabled items**: grayed out, no hover highlight, clicks ignored
- Uses `Supplier<MenuPopup>` factory to create fresh widgets each time (avoids widget reuse corruption)

**Architecture:** `MenuPopup` follows the `SelectPopup` pattern: full-window overlay that claims all events via `getWidgetAtPos()` returning `this`, routes hover events to internal items via `updateHover()`, and closes on outside click or Escape. Rendering is clipped to the popup zone via `displayProp.withLimit()`. `onRegenerateDisplay()` always propagates to children (no `needRedraw()` gate) so that hover state changes on `MenuItem` are rendered immediately.

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

## Common Pitfalls

Problems encountered during development — read these to avoid repeating the same mistakes.

### Event system

- **`getWidgetAtPos()` controls who receives events.** If an overlay widget returns `this` for the entire window, it intercepts ALL mouse events and the widget tree below receives nothing. For overlay layers that should be transparent to events (e.g. managed popovers), return `null` from `getWidgetAtPos()`.

- **`InputManager` sends `KeyStatus.leave` when the mouse exits the *target widget* bounds, not the parent.** If a `Container` wraps a `Label`, leaving the `Label` triggers a `leave` event even if the mouse is still inside the `Container`. Always check if the mouse is still inside the parent before closing on `leave`:
  ```java
  if (event.status() == KeyStatus.leave) {
      final Vector2f relPos = relativePosition(event.pos());
      if (isInsideTrigger(relPos)) {
          return; // Still inside — don't close
      }
      // Actually left
  }
  ```

- **`systemEventInput()` bubbles up from child to parent.** Override `systemEventInput()` (not `onEventInput()`) to observe events passing through a Container without consuming them. Call `super.systemEventInput(event)` to let the child handle normally.

- **`inputId == 0` means cursor movement** (no button pressed). Use this to detect hover enter/leave.

### Layout and positioning

- **Widget children use absolute coordinates.** Calling `setOrigin()` on a parent Box without calling `onChangeSize()` moves the box background but NOT its children. Always call `parent.onChangeSize()` after `setOrigin()` to propagate position changes to children.

- **`onChangeSize()` is heavy** — it triggers full layout recalculation (`calculateMinMaxSize()` etc.). For per-frame position updates (e.g. follow-mouse), create a fast path that only recalculates the position, arrow, and hit-box without full relayout. See `Popover.relocateToAnchor()`.

- **Popup stack triggers `onChangeSize()` on ALL widgets** (main widget + all popups). Don't use `Windows.popUpWidgetPush()` for lightweight overlays. Use a dedicated layer (like `PopoverManager`) instead.

### Widget reuse

- **Don't reuse Widget instances across show/dismiss cycles.** Internal state (especially `TextBase` parsing in Labels) gets corrupted. Use a `Supplier<Widget>` factory pattern to create fresh widgets each time:
  ```java
  .popoverContent(() -> new Label("Fresh content"))
  ```

### Rendering

- **`systemDrawWidget()` sets `OpenGL.setViewPort()` OUTSIDE push/pop** — the viewport persists after the method returns. When drawing overlay widgets, use `displayProp.withLimit()` to clip to the widget zone (see `SelectPopup` pattern).

- **Layer order in `Windows.systemDraw()`** (bottom to top): Main widget → Popup stack → Popover layer → Toast notifications. New overlay layers must be drawn in the correct order.

- **Split static and dynamic rendering.** For widgets with expensive backgrounds (gradients, track shapes), split into a cached background `CompositingGC` (regenerated only on resize) and a lightweight cursor/fill overlay (regenerated every frame). See `ColorGradient` (gradient + cursor) and `Slider` (track + cursor/fill) for the pattern.

- **Cache resource data.** `Uri.getAllDataString()` has no internal cache — it reads from classpath every call. Cache the result in a field (see `Tick.cachedSvgData`).

### Container

- **`Container(Widget)` constructor does NOT call `setParent()`.** Use `setSubWidget()` to properly set the parent. The constructor just stores the reference without registering the parent relationship.

## Dependencies

| Library | Purpose |
|---------|---------|
| `gale` | Windowing wrapper, OpenGL rendering, input events |
| `etk` | Core utilities (Vector2f, Color, Dimension2f, Uri, etc.) |
| `esignal` | Signal/slot event system (weak pointer based) |
| `aknot` | Annotation system for properties (@AknotAttribute, @AknotSignal, etc.) |
| `esvg` | SVG rendering |
| `egami` | Image handling (ToolImage wraps javax.imageio) |
| `ejson` | JSON parsing (configs, color files) |

## File Structure

```
ewol/
  src/main/org/atriasoft/ewol/
    Ewol.java                    -- Main entry point: init(), run()
    Gravity.java                 -- Layout gravity enum
    DrawProperty.java            -- Drawing clipping properties
    Padding.java                 -- Padding record
    context/
      EwolApplication.java       -- Application lifecycle interface
      EwolContext.java           -- Main context (extends GaleApplication)
      InputManager.java          -- Mouse/touch input routing
    widget/                      -- All widget classes
      TreeView.java              -- Generic tree view (expand/collapse, columns, indent guides)
      TreeFileSystem.java        -- File system browser (extends TreeView, lazy-loading)
      TreeNode.java              -- Generic tree data node
      TreeColumn.java            -- Column definition
      TreeCellRenderer.java      -- Functional interface for cell rendering
      TreeColumnAction.java      -- Record for column action signals
      meta/                      -- Composite widgets (FileChooser, SelectPopup, ColorPickerPopup)
      menu/                      -- Menu system (MenuPopup, MenuItem, MenuBar, MenuSeparator, ShortcutBadge, ShortcutFormatter)
      notification/              -- Toast notification system (NotificationManager, Toast, ToastType, ToastConfig)
    object/
      EwolObject.java            -- Base object with ID, name, parent, signals
      ObjectManager.java         -- Lifecycle management
      Worker.java                -- Background tasks
    event/                       -- Event types
    compositing/                 -- Rendering primitives
    resource/                    -- Resource loaders (fonts, colors, configs)
  samples/                       -- Example applications
  src/resources/ewol/            -- Built-in resources (shaders, icons, color configs)
```
