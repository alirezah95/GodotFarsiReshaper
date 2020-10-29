# Godot FarsiReshaper
Provides the ability to write Farsi in games made with [Godot game engine](https://godotengine.org/).

## About FarsiReshaper library:
**FarsiReshaper** library is a simple C++ code that reshapes the RTL Farsi (Persian) texts so that they are correctly shown in games made with [Godot game engine](https://godotengine.org/). It supports **auto-wraping** too, for that **FarsiReshaper** needs a [**Font**](https://docs.godotengine.org/en/stable/classes/class_font.html) and an **integer** value as **maximum line width**.


<details open> <summary style="font-weight:bold;font-size:36px">API Reference:</summary>

* <details> <summary style="font-weight:bold"> getReshapedString(text: <span style="color:purple">String</span>) -> <span style="color:purple">String</span>:</summary> <ul style="list-style-type:none;">
This is the main method which you use to get properly reshaped Farsi text. It gets a **text** parameter of type <span style="color:purple;font-weight:bold">String</span> and reshapes it to be correctly shown.
</ul>

* <details> <summary style="font-weight:bold"> disableAutoWrap() -> <span style="color:purple">void</span>:</summary> <ul style="list-style-type:none;">

Disables auto-wapping, useful for short texts.
</ul>

* <details> <summary style="font-weight:bold"> enableAutoWrap(font: <span style="color:purple">Font</span>, maxLineWidth: <span style="color:purple">int</span>) -> <span style="color:purple">bool</span>:</summary> <ul style="list-style-type:none;">

Enables auto-wrapping and sets **font** and **maxLineWidth** property used in auto-wrapping. If **font** is **null** or **maxLineWidth** is 0, returns <span style="color:purple;font-weight:bold">false</span> and disables auto-wrapping, otherwise returns <span style="color:purple;font-weight:bold">true</span> and auto-wrapping is enabled.
</ul>


* <details> <summary style="font-weight:bold"> setMaxLineWidth(maxLineWidth: <span style="color:purple">int</span>) -> <span style="color:purple">void</span>:</summary> <ul style="list-style-type:none;">
Sets **maxLineWidth** property, no effects on reshaping result if auto wrapping is disabled.

</ul>

* <details> <summary style="font-weight:bold"> setFont(font: <span style="color:purple">Font</span>) -> <span style="color:purple">void</span>:</summary> <ul style="list-style-type:none;">
Sets **font** property, no effects on reshaping result if auto wrapping is disabled.
</ul>

* <details> <summary style="font-weight:bold"> setContentMargin(margin: <span style="color:purple">int</span>) -> <span style="color:purple">void</span>:</summary> <ul style="list-style-type:none;">
Sets **contentMargin** property, no effects on reshaping result if auto wrapping is disabled.
</ul>

* <details> <summary style="font-weight:bold"> getContentMargin() -> <span style="color:purple">int</span>:</summary> <ul style="list-style-type:none;">
Returns **contentMargin** value.
</ul>

</details>

# Getting Started:
To use this library, simply download the [latest release](https://github.com/alirezah95/GodotFarsiReshaper/releases), uncompress and paste contents of FarsiReshaper folder into **`res::Scripts/`** folder, make an instance of **FarsiReshaper.gdns** and use it. There is two situations:
* **If you want auto-wrapping enabled:** In this case use <code style="font-weight:bold">enableAutoWrap(font: <span style="color:purple">Font</span>, maxLineWidth: <span style="color:purple">int</span>) -> <span style="color:purple">bool</span></code> with proper Font object and float value for maxLineWidth,to enable auto-wrapping then use <code style="font-weight:bold">getReshapedString(text: <span style="color:purple">String</span>) -> <span style="color:purple">String</span></code> to get properly reshaped string text for use in **Godot's** node.
* **If you don't want auto-wrapping for any reason - for example if you're sure that your text will fit into its area - :** In this case just use <code style="font-weight:bold">getReshapedString(text: <span style="color:purple">String</span>) -> <span style="color:purple">String</span></code> method to get your string.
<span style="color:purple;font-weight:bold">Note: </span>You should call <code style="font-weight:bold">disableAutoWrap() -> <span style="color:purple">void</span>:</code> before **```getReshapedString()```** to prevent unnecessary parts of code from executing - which is only intended for when auto-wrapping is enabled -.