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
          Dialog              -- Structured dialog (title bar + content + footer buttons)
        FieldSet              -- Grouping container with border, legend, optional icon and checkbox
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
    RadioButton               -- Radio button (RadioIndicator + arbitrary Widget composite)
    RadioGroup                -- Exclusive selection group of RadioButtons (extends Sizer)
    ProgressBar               -- Progress indicator (0.0 to 1.0)
    Spacer                    -- Empty space widget
    Tick                      -- Toggle indicator (cached SVG, used internally by CheckBox)
    RadioIndicator            -- Circular radio indicator (used internally by RadioButton)
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
