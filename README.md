# UIScripter

Hot-reloadable UI scripting using the embedded language [Tiny](https://github.com/goodpaul6/Tiny). Check out the example for how to incorporate.

# Docs

These are the functions you can use:

```go
func Node(): Node // Creates a new CCNode
func Label(name: str, font: str): Node // Creates a new CCLabelBMFont
func Sprite(name: str): Node // Creates a new CCSprite
func WebSprite(url: str): Node // Create a CCSprite from a given url
func Button(sprite: Node, callback: str): Node // Creates a new button with a sprite and a callback
func ButtonSprite(name: str, sprite: str): Node // Creates a new sprite for a button
func Scale9Sprite(width: float, height: float, sprite: str): Node // Creates a new CCScale9Sprite

func tag(node: Node, tag: int): Node // Sets a tag for a node
func fetch(node: Node, tag: int): Node // Fetches a child node by tag
func visible(node: Node, visible: bool): Node // Sets the visibility of a node
func pos(node: Node, x: float, y: float): Node // Sets the position of a node
func xpos(node: Node, x: float): Node // Sets the x position of a node
func ypos(node: Node, y: float): Node // Sets the y position of a node
func getX(node: Node): float // Gets the x position of a node
func getY(node: Node): float // Gets the y position of a node
func move(node: Node, x: float, y: float): Node // Moves a node by x and y
func size(node: Node, width: float, height: float): Node // Sets the size of a node
func width(node: Node, width: float): Node // Sets the width of a node
func height(node: Node, height: float): Node // Sets the height of a node
func getWidth(node: Node): float // Gets the width of a node
func getHeight(node: Node): float // Gets the height of a node
func anchorPoint(node: Node, x: float, y: float): Node // Sets the anchor point of a node
func scale(node: Node, scale: float): Node // Sets the scale of a node
func scaleX(node: Node, scale: float): Node // Sets the x scale of a node
func scaleY(node: Node, scale: float): Node // Sets the y scale of a node
func scaleBy(node: Node, scale: float): Node // Scales a node by a factor
func rotate(node: Node, angle: float): Node // Sets the rotation of a node
func rotateBy(node: Node, angle: float): Node // Rotates a node by an angle
func z(node: Node, z: int): Node // Sets the z order of a node
func getZ(node: Node): int // Gets the z order of a node
func child(parent: Node, child: Node): Node // Adds a child to a parent
func runAction(node: Node, action: Action): Node // Runs an action on a node
func color(node: Node, r: int, g: int, b: int): Node // Sets the color of a node
func opacity(node: Node, opacity: int): Node // Sets the opacity of a node
func text(node: Node, text: str): Node // Sets the text of a node
func font(node: Node, font: str): Node // Sets the font of a node
func updateLayout(node: Node): Node // Updates the layout of a node

func axis(layout: Layout, axis: int) // Sets the axis of the layout (0 = Row, 1 = Column)
func align(layout: Layout, align: int) // Sets the main axis alignment of the layout (0 = Start, 1 = Center, 2 = End, 3 = Even, 3 = Between)
func crossAlign(layout: Layout, align: int) // Sets the cross axis alignment of the layout
func lineAlign(layout: Layout, align: int) // Sets the cross axis line alignment of the layout
func gap(layout: Layout, gap: float) // Sets the gap between items in the layout
func reverse(layout: Layout, rev: bool) // Sets whether the main axis is reversed
func crossReverse(layout: Layout, rev: bool) // Sets whether the cross axis is reversed
func autoScale(layout: Layout, autoscale: bool) // Sets whether the layout auto-scales its children to fit
func crossGrow(layout: Layout, crossgrow: bool) // Sets whether the layout grows on the cross axis when overflowing
func crossOverflow(layout: Layout, overflow: bool) // Sets whether the layout allows cross axis overflow
func autoGrow(layout: Layout, grows: bool, min: float) // Sets whether the layout auto-grows on the main axis to fit all items in one line, with an optional minimum length
func ignoreInvisible(layout: Layout, ignore: bool) // Sets whether the layout ignores invisible children

func relativeScale(options: LayoutOptions, scale: float) // Sets the relative scale of the node in an auto-scaled layout
func autoScale(options: LayoutOptions, override: bool, autoscale: bool) // Sets a potential override to the layout's auto-scale setting
func length(options: LayoutOptions, override: bool, length: float) // Sets a potential override to the length of the node
func prevGap(options: LayoutOptions, override: bool, gap: float) // Sets a potential override to the layout's gap before this node
func nextGap(options: LayoutOptions, override: bool, gap: float) // Sets a potential override to the layout's gap after this node
func breakLine(options: LayoutOptions, brk: bool) // Sets whether this node should always break into a new line in a growable layout
func sameLine(options: LayoutOptions, same: bool) // Sets whether this node should always be on the same line as the previous node in a growable layout
func scalePrio(options: LayoutOptions, prio: int) // Sets the scale priority of this node in an auto-scaled layout
func crossAlign(options: LayoutOptions, override: bool, align: int) // Sets a potential override to the cross axis alignment of this node

func anchor(options: LayoutOptions, anchor: int) // Sets the anchor point for this node in an anchor layout (0 = Center, 1 = TopLeft, 2 = Top, 3 = TopRight, 4 = Right, 5 = BottomRight, 6 = Bottom, 7 = BottomLeft, 8 = Left)
func offset(options: LayoutOptions, x: float, y: float) // Sets the offset for this node in an anchor layout
func setAnchorOpts(node: Node, anchor: int, x: float, y: float) // Sets both the anchor and offset for this node in an anchor layout

func Sequence(action1: Action, action2: Action): Action // Creates a new sequence action
func Delay(time: float): Action // Creates a new delay action
func MoveTo(time: float, x: float, y: float): Action // Creates a new move to action
func MoveBy(time: float, x: float, y: float): Action // Creates a new move by action
func ScaleTo(time: float, scale: float): Action // Creates a new scale to action
func ScaleBy(time: float, scale: float): Action // Creates a new scale by action
func RotateTo(time: float, angle: float): Action // Creates a new rotate to action
func RotateBy(time: float, angle: float): Action // Creates a new rotate by action
func FadeTo(time: float, opacity: float): Action // Creates a new fade to action
func FadeBy(time: float, opacity: float): Action // Creates a new fade by action

func popup(title: str, message: str, button: str): void // Shows a popup
func prompt(title: str, placeholder: str, callback: str): void // Shows a prompt
func terminate(): void // Terminates the TinyNode, removes it from parent
```

These are the default variables available:
```go
self_menu: Node // The menu of the TinyNode
self: Node // The TinyNode itself
```