#include "jsobjects.hpp"

#include <JavaScriptCore/JavaScript.h>
#include <assert.h>

class JSValueJSC: public JSValue {

public:

  JSValueJSC(JSContextRef context, JSValueRef val): context(context) {

    if(JSValueIsUndefined(context, val)) {
      type = Undefined;
    } if(JSValueIsNull(context, val)) {
      type = Null;
    } else if(JSValueIsBoolean(context, val)) {
      type = Boolean;
    } else if(JSValueIsNumber(context, val)) {
      type = Number;
    } else if(JSValueIsString(context, val)) {
      type = String;
    } else if(JSValueJSC::IsArray(context, val)) {
      type = Array;
    } else if(JSValueIsObject(context, val)) {
      type = Object;
    } else {
      // Note: there is no IsArray function in JSC
      throw "Not yet supported";
    }

    // make the reference persistent
    JSValueProtect(context, val);
    value = val;
  }

  ~JSValueJSC() {
    // release the persistent reference
    JSValueUnprotect(context, value);
  }

  virtual std::string asString() {
    assert(JSValueIsString(context, value));

    JSStringRef jsstring = JSValueToStringCopy(context, value, /* JSValueRef *exception */ 0);
    unsigned int length = JSStringGetLength(jsstring);
    char *cstr = new char[length+1];
    JSStringGetUTF8CString(jsstring, cstr, length);
    std::string result(cstr);

    JSStringRelease(jsstring);
    delete[] cstr;

    return result;
  }

  virtual double asDouble() {
    assert(JSValueIsNumber(context, value));
    return JSValueToNumber(context, value, /* JSValueRef *exception */ 0);
  }

  virtual bool asBool() {
    assert(JSValueIsBoolean(context, value));
    return JSValueToBoolean(context, value);
  }

  virtual JSValueType getType() { return type; };


  JSContextRef context;
  JSValueRef value;

protected:

  static bool IsArray(JSContextRef context, JSValueRef val);

  JSValueType type;

};

class JSObjectJSC: public JSValueJSC, public JSObject {

public:
  JSObjectJSC(JSContextRef context, JSObjectRef obj): JSValueJSC(context, obj), object(obj) {}

  virtual JSValuePtr get(const std::string& key) {
    JSStringRef jskey = JSStringCreateWithUTF8CString(key.c_str());
    JSValueRef val = JSObjectGetProperty(context, object, jskey, /* JSValueRef *exception */ 0);
    JSStringRelease(jskey);
    return JSValuePtr(new JSValueJSC(context, val));
  }

  virtual void set(const std::string& key, JSValuePtr val) {
    JSStringRef jskey = JSStringCreateWithUTF8CString(key.c_str());
    JSObjectSetProperty(context, object, jskey, static_cast<JSValueJSC*>(JSOBJECTS_PTR_GET(val))->value, kJSPropertyAttributeNone, /* JSValueRef *exception */ 0);
    JSStringRelease(jskey);
  }

  virtual void set(const std::string& key, const std::string& val) {
    JSStringRef jskey = JSStringCreateWithUTF8CString(key.c_str());
    JSStringRef jsval = JSStringCreateWithUTF8CString(val.c_str());
    JSObjectSetProperty(context, object, jskey, JSValueMakeString(context, jsval), kJSPropertyAttributeNone, /* JSValueRef *exception */ 0);
    JSStringRelease(jskey);
  }

  virtual void set(const std::string& key, bool val) {
    JSStringRef jskey = JSStringCreateWithUTF8CString(key.c_str());
    JSObjectSetProperty(context, object, jskey, JSValueMakeBoolean(context, val), kJSPropertyAttributeNone, /* JSValueRef *exception */ 0);
    JSStringRelease(jskey);
  }

  virtual void set(const std::string& key, double val) {
    JSStringRef jskey = JSStringCreateWithUTF8CString(key.c_str());
    JSObjectSetProperty(context, object, jskey, JSValueMakeNumber(context, val), kJSPropertyAttributeNone, /* JSValueRef *exception */ 0);
    JSStringRelease(jskey);
  }

protected:
  JSObjectRef object;
};

class JSArrayJSC: public JSObjectJSC, public JSArray {

public:

  JSArrayJSC(JSContextRef context, JSObjectRef arr): JSObjectJSC(context, arr) { }

  virtual JSValuePtr getAt(unsigned int index) {
    return JSValuePtr(new JSValueJSC(context, JSObjectGetPropertyAtIndex(context, object, index, /* JSValueRef *exception */ 0)));
  }

  virtual void setAt(unsigned int index, JSValuePtr val) {
    JSObjectSetPropertyAtIndex(context, object, index, static_cast<JSValueJSC*>(JSOBJECTS_PTR_GET(val))->value, /* JSValueRef *exception */ 0);
  };

  virtual void setAt(unsigned int index, const std::string& val) {
    JSStringRef jsval = JSStringCreateWithUTF8CString(val.c_str());
    JSObjectSetPropertyAtIndex(context, object, index, JSValueMakeString(context, jsval), /* JSValueRef *exception */ 0);
  }

  virtual void setAt(unsigned int index, bool val) {
    JSObjectSetPropertyAtIndex(context, object, index, JSValueMakeBoolean(context, val), /* JSValueRef *exception */ 0);
  }

  virtual void setAt(unsigned int index, double val) {
    JSObjectSetPropertyAtIndex(context, object, index, JSValueMakeNumber(context, val), /* JSValueRef *exception */ 0);
  }

  virtual unsigned int length();
};

#ifdef USE_BOOST_SHARED_PTR

#define JSOBJ_JSC_PTR_CAST(type, obj) boost::dynamic_cast< type >(obj)

#else

#define JSOBJ_JSC_PTR_CAST(type, obj) dynamic_cast< type *> ( (JSValueJSC*) obj)

#endif


/*static*/ JSArrayPtr JSValue::asArray(JSValuePtr val) {
  assert(val->getType() == Array);
  JSArrayPtr arrPtr = JSOBJ_JSC_PTR_CAST(JSArray, val);
  return arrPtr;
}

/*static*/ JSObjectPtr JSValue::asObject(JSValuePtr val) {
  assert(val->getType() == Object || val->getType() == Array);
  JSObjectPtr objPtr = JSOBJ_JSC_PTR_CAST(JSObject, val);
  return objPtr;
}

/*static*/ JSObjectPtr JSValue::asObject(JSArrayPtr arr) {
  JSObjectPtr objPtr = JSOBJ_JSC_PTR_CAST(JSObject, arr);
  return objPtr;
}

/*static*/ JSValuePtr JSValue::asValue(JSArrayPtr arr) {
  JSValuePtr valPtr = JSOBJ_JSC_PTR_CAST(JSValue, arr);
  return valPtr;
}

/*static*/ JSValuePtr JSValue::asValue(JSObjectPtr obj) {
  JSValuePtr valPtr = JSOBJ_JSC_PTR_CAST(JSValue, obj);
  return valPtr;
}

#undef JSOBJ_JSC_PTR_CAST
