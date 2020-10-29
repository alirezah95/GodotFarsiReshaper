# Godot FarsiReshaper
Provides the ability to write Farsi in games made with [Godot game engine](https://godotengine.org/).

## About FarsiReshaper library
**FarsiReshaper** library is a simple C++ code that reshapes the RTL Farsi (Persian) texts so that they are correctly shown in games made with [Godot game engine](https://godotengine.org/). It supports **auto-wraping** too, for that **FarsiReshaper** needs a [**Font**](https://docs.godotengine.org/en/stable/classes/class_font.html) and an **integer** value as **maximum line width**.

## API Reference
API reference is available in **[this](https://github.com/alirezah95/GodotFarsiReshaper/blob/main/docs/reference.md)** file, you should read it first.

# Getting Started
To use this library, simply download the [latest release](https://github.com/alirezah95/GodotFarsiReshaper/releases), uncompress and paste contents of FarsiReshaper folder into **`res::Scripts/`** folder, make an instance of **FarsiReshaper.gdns** and use it. There is two situations:
* **If you want auto-wrapping enabled:** In this case use *`enableAutoWrap(font: Font, maxLineWidth: int) -> bool`* with proper Font object and float value for maxLineWidth,to enable auto-wrapping then use *`getReshapedString(text: String) -> String`* to get properly reshaped string text for use in **Godot's** node.
* **If you don't want auto-wrapping for any reason - for example if you're sure that your text will fit into its area - :** In this case just use *`getReshapedString(text: String) -> String`* method to get your string.
***Note:*** You should call *`disableAutoWrap() -> void`* before *`getReshapedString()`* to prevent unnecessary parts of code from executing - which is only intended for when auto-wrapping is enabled -.
