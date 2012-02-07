>Edit box properties
>===================
>
text
----
>Set or get the text entered in the edit box.
placeholder
-----------
>Set a text in the edit box that acts as a placeholder when an edit box is empty.
showKeyboard
------------
>Shows or hides the virtual keyboard. If shown the focus will be set to this widget.
editMode
--------
>Specifies what editing mode the edit box should have.
inputMode
---------
>Specifies what editing mode the edit box should have. Those flags are mutual exclusive, so the previous value is always ignored.
inputFlag
---------
>Specifies the editing flags applied to the edit box. Those flags are mutual exclusive, so the previous value is always ignored.
fontColor
---------
>Sets the font color of the edit box's text.
linesNumber
-----------
>Set the number of lines. Makes the edit box exactly this many lines tall. Note that setting this value overrides any other (minimum / maximum) number of lines or height setting. A single line edit box will set this value to 1. This property will automatically set MAW_EDIT_BOX_TYPE_ANY input mode, so that the edit box will become multiline. Available on Android only.
maxLines
--------
>Makes the edit box at most this many lines tall. Setting this value overrides any other (maximum) height setting. Available on Android only.
minLines
--------
>Makes the edit box at least this many lines tall. Setting this value overrides any other (minimum) height setting. Available on Android only.
maxLength
---------
>Sets or gets the maximum input lenght of the edit box. Setting this value enables multiline input mode by default. Available on Android only.
placeholderFontColor
--------------------
>Sets the font color of the placeholder text when an edit box is empty.
