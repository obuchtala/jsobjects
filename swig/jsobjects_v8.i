
%header %{
#include <jsobjects_v8.hpp>
%}

%typemap(in) JSValue*
%{
  $1 = new JSValueV8($input);
%}

%typemap(freearg) JSValue*
%{
   delete $1;
%}

%typemap(out) JSValue*
%{
  $result = dynamic_cast<JSValueV8*>(JSOBJECTS_PTR_GET($1))->value;
%}

%typemap(newfree) JSValue*
%{
   delete $1;
%}

%typemap(in) JSObject*
%{
  $1 = new JSObjectV8($input);
%}

%typemap(freearg) JSObject*
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
