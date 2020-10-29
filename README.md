# Godot FarsiReshaper
Provides the ability to write Farsi in games made with [Godot game engine](https://godotengine.org/).

## About FarsiReshaper library:
**FarsiReshaper** library is a simple C++ code that reshapes the RTL Farsi (Persian) texts so that they are correctly shown in games made with [Godot game engine](https://godotengine.org/). It supports **auto-wraping** too, for that **FarsiReshaper** needs a [**Font**](https://docs.godotengine.org/en/stable/classes/class_font.html) and an **integer** value as **maximum line width**.


# Getting Started:
To use this library, simply download the [latest release](https://github.com/alirezah95/GodotFarsiReshaper/releases), uncompress and paste contents of FarsiReshaper folder into **`res::Scripts/`** folder, make an instance of **FarsiReshaper.gdns** and use it. There is two situations:
* **If you want auto-wrapping enabled:** In this case use <code style="font-weight:bold">enableAutoWrap(font: <span style="color:purple">Font</span>, maxLineWidth: <span style="color:purple">int</span>) -> <span style="color:purple">bool</span></code> with proper Font object and float value for maxLineWidth,to enable auto-wrapping then use <code style="font-weight:bold">getReshapedString(text: <span style="color:purple">String</span>) -> <span style="color:purple">String</span></code> to get properly reshaped string text for use in **Godot's** node.
* **If you don't want auto-wrapping for any reason - for example if you're sure that your text will fit into its area - :** In this case just use <code style="font-weight:bold">getReshapedString(text: <span style="color:purple">String</span>) -> <span style="color:purple">String</span></code> method to get your string.
<span style="color:purple;font-weight:bold">Note: </span>You should call <code style="font-weight:bold">disableAutoWrap() -> <span style="color:purple">void</span>:</code> before **```getReshapedString()```** to prevent unnecessary parts of code from executing - which is only intended for when auto-wrapping is enabled -.