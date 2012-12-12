%header %{
#include <jsobjects_jsc.hpp>
%}

%wrapper %{
JSContextPtr SWIGJSC_theContext;

extern "C" bool SWIGJSC_INIT (JSGlobalContextRef context);
#define CONCAT(A,B) A##B
#define JSOBJECTS(A) CONCAT(A,_jsobjects)

extern "C" bool JSOBJECTS(SWIGJSC_INIT) (JSGlobalContextRef context)
{
  SWIGJSC_theContext = JSContextPtr(new JSContextJSC(context));
  return SWIGJSC_INIT(context);
}
#undef CONCAT
#undef JSOBJECTS
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

  %typemap(in) JSArrayPtr
  %{
    $1 = JSArrayPtr(new JSArrayJSC(context, $input));
  %}

  %typemap(out) JSArrayPtr
  %{
    $result = dynamic_cast<JSArrayJSC*>(JSOBJECTS_PTR_GET($1))->value;
  %}

  %typemap(in) JSContextPtr, boost::shared< jsobjects::JSContext >
  %{
    $1 = SWIGJSC_theContext;
  %}

  %typemap(check) JSContextPtr {}

} // namespace jsobjects
