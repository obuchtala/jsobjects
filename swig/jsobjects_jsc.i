%header %{
#include <jsobjects_jsc.hpp>
%}

%typemap(in) JSValue*
%{
  $1 = new JSValueJSC(context, $input);
%}

%typemap(out) JSValue*
%{
  $result = dynamic_cast<JSValueJSC*>(JSOBJECTS_PTR_GET($1))->value;
%}

%typemap(in) JSObject*
%{
  $1 = new JSObjectJSC(context, $input);
%}

%typemap(out) JSObject*
%{
  $result = dynamic_cast<JSObjectJSC*>(JSOBJECTS_PTR_GET($1))->value;
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
  $1 = new JSContextJSC(context);
%}

%typemap(freearg) JSContext*
%{
   delete $1;
%}
