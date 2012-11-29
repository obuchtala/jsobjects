#include <jsobjects_v8.hpp>


#ifdef USE_BOOST_SHARED_PTR

#define JSOBJ_V8_PTR_CAST(type, obj) boost::dynamic_cast< type >(obj)

#else

#define JSOBJ_V8_PTR_CAST(type, obj) dynamic_cast< type *> ( (JSValueV8*) obj)

#endif


/*static*/ JSArrayPtr JSValue::asArray(JSValuePtr val) {
  assert(val->getType() == Array);
  JSArrayPtr arrPtr = JSOBJ_V8_PTR_CAST(JSArray, val);
  return arrPtr;
}

/*static*/ JSObjectPtr JSValue::asObject(JSValuePtr val) {
  assert(val->getType() == Object || val->getType() == Array);
  JSObjectPtr objPtr = JSOBJ_V8_PTR_CAST(JSObject, val);
  return objPtr;
}

/*static*/ JSObjectPtr JSValue::asObject(JSArrayPtr arr) {
  JSObjectPtr objPtr = JSOBJ_V8_PTR_CAST(JSObject, arr);
  return objPtr;
}

/*static*/ JSValuePtr JSValue::asValue(JSArrayPtr arr) {
  JSValuePtr valPtr = JSOBJ_V8_PTR_CAST(JSValue, arr);
  return valPtr;
}

/*static*/ JSValuePtr JSValue::asValue(JSObjectPtr obj) {
  JSValuePtr valPtr = JSOBJ_V8_PTR_CAST(JSValue, obj);
  return valPtr;
}

#undef JSOBJ_JSC_PTR_CAST
