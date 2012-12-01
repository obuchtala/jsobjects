
%header %{
#include <jsobjects_v8.hpp>
%}

%typemap(in) JSValue*
%{
  $1 = new JSValueV8($input);
%}

%typemap(out) JSValue*
%{
  $result = dynamic_cast<JSValueV8*>(JSOBJECTS_PTR_GET($1))->value;
%}

%typemap(in) JSObject*
%{
  $1 = new JSObjectV8($input);
%}

%typemap(out) JSObject*
%{
  $result = dynamic_cast<JSObjectV8*>(JSOBJECTS_PTR_GET($1))->value;
%}

%typemap(freearg) JSValue*, JSObject*, JSArray*
%{
   delete $1;
%}

%typemap(newfree) JSValue*, JSObject*, JSArray*
%{
   delete $1;
%}

%typemap(in) JSContext*
%{
  $1 = new JSContextV8();
%}

%typemap(freearg) JSContext*
%{
   delete $1;
%}
