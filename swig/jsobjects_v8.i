
%header %{
#include <jsobjects_v8.hpp>
%}

namespace jsobjects {

%typemap(in) JSValuePtr
%{
  $1 = JSValuePtr(new JSValueV8($input));
%}

%typemap(out) JSValuePtr
%{
  $result = dynamic_cast<JSValueV8*>(JSOBJECTS_PTR_GET($1))->value;
%}

%typemap(in) JSObjectPtr
%{
  $1 = JSObjectPtr(new JSObjectV8($input));
%}

%typemap(out) JSObjectPtr
%{
  $result = dynamic_cast<JSObjectV8*>(JSOBJECTS_PTR_GET($1))->value;
%}

%typemap(in) JSArrayPtr
%{
  $1 = JSArrayPtr(new JSArrayV8($input));
%}

%typemap(out) JSArrayPtr
%{
  $result = dynamic_cast<JSArrayV8*>(JSOBJECTS_PTR_GET($1))->value;
%}

%typemap(newfree) JSValuePtr, JSObjectPtr, JSArrayPtr
%{
   delete $1;
%}

%typemap(in) JSContextPtr, boost::shared< jsobjects::JSContext >
%{
  $1 = JSContextPtr(new JSContextV8());
%}

%typemap(check) JSContextPtr {}

}
