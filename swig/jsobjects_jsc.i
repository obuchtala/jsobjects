%header %{
#include <jsobjects_jsc.hpp>
%}

namespace jsobjects {

  %typemap(in) JSValuePtr
  %{
    $1 = JSValuePtr(new JSValueJSC(context, $input));
  %}

  %typemap(out) JSValuePtr
  %{
    $result = dynamic_cast<JSValueJSC*>(JSOBJECTS_PTR_GET($1))->value;
  %}

  %typemap(in) JSObjectPtr
  %{
    $1 = JSObjectPtr(new JSObjectJSC(context, $input));
  %}

  %typemap(out) JSObjectPtr
  %{
    $result = dynamic_cast<JSObjectJSC*>(JSOBJECTS_PTR_GET($1))->value;
  %}

  %typemap(in) JSContextPtr
  %{
    $1 = JSContextPtr(new JSContextJSC(context));
  %}
} // namespace jsobjects
