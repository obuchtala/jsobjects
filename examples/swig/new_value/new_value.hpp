#include <jsobjects.hpp>
#include <iostream>

JSValuePtr createString(JSContextPtr context) {
  return context->newString("bla");
}
