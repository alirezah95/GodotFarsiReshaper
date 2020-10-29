## API Reference:

<h4>

``` Python
getReshapedString(text: String) -> String:
``` 
</h4>

* This is the main method which you use to get properly reshaped Farsi text. It gets a **text** parameter of type ***```String```*** and reshapes it to be correctly shown.


<h4>

``` Python
disableAutoWrap(text: String) -> String:
``` 
</h4>

* Disables auto-wapping, useful for short texts.


<h4>

``` Python
enableAutoWrap(font: Font, maxLineWidth: int) -> bool:
``` 
</h4>

* Enables auto-wrapping and sets **font** and **maxLineWidth** property used in auto-wrapping. If **font** is **null** or **maxLineWidth** is 0, returns ***`false`*** and disables auto-wrapping, otherwise returns ***`true`*** and auto-wrapping is enabled.


<h4>

``` Python
setMaxLineWidth(maxLineWidth: int) -> void:
``` 
</h4>

* Sets **maxLineWidth** property, no effects on reshaping result if auto wrapping is disabled.


<h4>

``` Python
setFont(font: Font) -> void:
``` 
</h4>

* Sets **font** property, no effects on reshaping result if auto wrapping is disabled.


<h4>

``` Python
setContentMargin(margin: int) -> void:
``` 
</h4>

* Sets **contentMargin** property, no effects on reshaping result if auto wrapping is disabled.


<h4>

``` Python
getContentMargin() -> int:
``` 
</h4>

* Returns **contentMargin** value.



<h4> <code class="lang-python"> getContentMargin() -> int: </code>
</h4>