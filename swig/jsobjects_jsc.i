%header %{
#include <jsobjects_jsc.hpp>
%}

%typemap(in) JSValue*
%{
  $1 = new JSValueJSC(context, $input);
%}

%typemap(freearg) JSValue*
%{
   delete $1;
%}

%typemap(out) JSValue*
%{
  $result = dynamic_cast<JSValueJSC*>(JSOBJECTS_PTR_GET($1))->value;
%}

%typemap(newfree) JSValue*
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
