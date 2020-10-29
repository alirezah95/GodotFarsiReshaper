## API Reference:

<h4>

``` Java
getReshapedString(text: String) -> String:
``` 
</h4>

* This is the main method which you use to get properly reshaped Farsi text. It gets a **text** parameter of type **```String```** and reshapes it to be correctly shown.


``` Java
disableAutoWrap() -> void:
``` 
Disables auto-wapping, useful for short texts.


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