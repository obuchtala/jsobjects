%header %{
#include <jsobjects_jsc.hpp>
#include <memory>
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
  $result = $1->value;
%}

%typemap(newfree) JSValue*
%{
   delete $1;
%}
