%module new_value

%header %{
#include <new_value.hpp>
%}

%include <jsobjects.i>

%newobject createString;
%include "new_value.hpp"

