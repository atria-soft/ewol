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
The text is markup: a literal `<`, `>` or `&` is written `&lt;`, `&gt;`, `&amp;` (numeric `&#233;` works too), or
`Label.create(Label.escape(plainText))`. Latin-1, Latin Extended and the euro sign are drawn (glyphs are added to the
font atlas on first use). `label.setPropertyTextColor(color)` overrides the theme colour.

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

Shortcuts `ctrl+a` (select all), `ctrl+c`, `ctrl+x`, `ctrl+v` and `ctrl+w` (clean) are caught but do
nothing yet: gale's `ClipBoard` is a stub, and a cut must never erase a selection it did not copy.
Connect `signalShortcut` to `onCallbackShortCut` once `ClipBoard` works.

Text typed: a printable character typed without Control, or with Control and AltGr (X11 hands AltGr
without Control, Windows with it). gale hands a letter typed with Control alone as the letter (Ctrl+Z
comes as `z`): the entry takes it for a shortcut and inserts nothing; Control with the left Alt too.
A key that inserts nothing leaves the selection as it is.

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

### RadioButton / RadioGroup

Exclusive selection radio buttons. A `RadioButton` combines a circular `RadioIndicator` with an arbitrary content widget. A `RadioGroup` manages exclusive selection among multiple `RadioButton` children.

```java
// Simple text radio group
RadioGroup.vertical()
    .addRadio("Option A")
    .addRadio("Option B")
    .addRadio("Option C")
    .selectedIndex(0)
    .onSelectionChanged(index -> System.out.println("Selected: " + index))

// Radio group with custom widget content
RadioGroup group = RadioGroup.vertical();
group.addRadio(new RadioButton(myCustomWidget));
group.addRadio(new RadioButton(anotherWidget));
group.selectedIndex(0);

// Setter API
group.setPropertySelectedIndex(1);
group.getPropertySelectedIndex();   // int (-1 if none)
group.getSelected();                // RadioButton or null
```

**Signals:** `RadioGroup.signalSelectionChanged` (`Signal<Integer>` — selected index), `RadioButton.signalValue` (`Signal<Boolean>`), `RadioButton.signalClick` (`SignalEmpty`).

**RadioIndicator:** Internal circular indicator widget (extends `Box`). Draws an empty circle when unselected, filled dot when selected. Unlike `Tick`, clicking always sets to `true` (deselection managed by `RadioGroup`).

### FieldSet

Grouping container with a legend title, optional icon, and optional checkbox to enable/disable the section. Equivalent to HTML `<fieldset>`.

```java
// Factory
FieldSet.create("General Settings")

// Fluent API
FieldSet.create("Lighting")
    .icon("light")
    .checkable(true)
    .checked(false)
    .add(intensitySlider)
    .add(colorPicker)
    .onActivate(() -> enableLighting())
    .onDeactivate(() -> disableLighting())

// Setter API
FieldSet fs = new FieldSet("title");
fs.setPropertyTitle("New Title");
fs.setEnabled(true);             // check/uncheck tick (no-op if no tick)
fs.isEnabled();                  // true if no tick
```

**Header order:** [Tick] [Icon] Title (left to right).

**Signals:** `signalActivate` (section enabled), `signalDeactivate` (section disabled).

**Content visibility:** When the tick is unchecked, the content sizer is hidden via `setPropertyHide(true)`. The layout collapses automatically.

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

### Dialog

Structured popup with title bar (close button), content area, and footer button bar. Extends `PopUp`.
Colours come from a `DialogSkin` (`LIGHT` by default, `DARK`): `Dialog.setDefaultSkin(DialogSkin.DARK)` makes every
new dialog (and subclass) follow the active theme, `dialog.skin(DialogSkin.DARK)` dresses one. The content widgets
keep their own colours (a `Label` in a dark dialog needs `setPropertyTextColor`).

```java
// Factory + Fluent API
Dialog.create()
    .title("My Dialog")
    .content(myContentWidget)
    .footer(Button.create("OK").onClick(() -> dialog.autoDestroy()))
    .footer(Button.create("Cancel").onClick(() -> dialog.autoDestroy()))
    .dialogSizePercent(50, 50)    // default: 80%x80%
    .onClose(() -> ...)           // close button (X) callback

// Show as popup
windows.popUpWidgetPush(dialog);

// Setter API
Dialog d = new Dialog();
d.setPropertyTitle("Title");
d.setContentWidget(widget);
d.addFooterWidget(button);
d.setDialogSizePercent(50, 50);
d.setDialogSize(400, 300, 800, 600);  // pixel min/max
```

**Signals:** `signalClose` (emitted when close button clicked).

**Subclassing:** Override `handleClose()` to customize close behavior (default: emit `signalClose` + `autoDestroy()`).

```java
public class MyDialog extends Dialog {
    public MyDialog() {
        setPropertyTitle("My Dialog");
        setContentWidget(buildContent());
        addFooterWidget(Button.create("OK"));
    }

    @Override
    protected void handleClose() {
        // custom close logic
        autoDestroy();
    }
}
```

**Structure:** Dialog fills the entire window as an overlay. The inner dialog box (default 80%x80%) is centered and contains: title bar (label + close icon) → content area → footer button bar.

**Color constants** (protected, available to subclasses): `COLOR_OVERLAY`, `COLOR_CONTENT`, `COLOR_HEADER_FOOTER`, `COLOR_BORDER`.

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

