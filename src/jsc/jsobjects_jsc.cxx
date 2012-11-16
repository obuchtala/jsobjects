#include <jsobjects_jsc.hpp>
#include <assert.h>


JSObjectRef JSValueJSC_GetArrayClassObj(JSContextRef context)
{
    JSValueRef array_class_val = NULL;
    static JSStringRef ARRAY =
        JSStringCreateWithUTF8CString("Array");

    JSObjectRef global = JSContextGetGlobalObject(context);
    bool has_array_prop = JSObjectHasProperty(context, global, ARRAY);

    if (has_array_prop) {
      JSValueRef exception = NULL;
      array_class_val = JSObjectGetProperty(context, global, ARRAY, &exception);
      if(JSValueIsObject(context, array_class_val)) {
        JSObjectRef array_ctor = JSValueToObject(context, array_class_val, &exception);
        if(exception == 0 && JSObjectIsConstructor(context, array_ctor)) {
          return array_ctor;
        }
      }
    }
    assert(false);
    return NULL;
}

bool JSValueJSC::IsArray(JSContextRef context, JSValueRef value)
{
  static JSObjectRef array_class_obj = JSValueJSC_GetArrayClassObj(context);

  JSValueRef exception = 0;
  bool is_array =  JSValueIsInstanceOfConstructor(
        context, value, array_class_obj, &exception);
  return (exception == 0 && is_array);
}

unsigned int JSArrayJSC::length() {
  static JSStringRef LENGTH =
      JSStringCreateWithUTF8CString("length");

  JSValueRef exception = NULL;
  JSValueRef _length =
      JSObjectGetProperty(context, object, LENGTH, &exception);

  JSValueJSC js_length(context, _length);
  if (exception == 0 && js_length.isNumber()) {
    double length = js_length.asDouble();
    return length >= 0 ? static_cast<unsigned int>(length) : 0;
  }

  return 0;
}
