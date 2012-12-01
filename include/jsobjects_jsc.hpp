#ifndef JSOBJECTS_JSC_HPP
#define JSOBJECTS_JSC_HPP

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

  virtual void set(const std::string& key, const char* val) {
    JSStringRef jskey = JSStringCreateWithUTF8CString(key.c_str());
    JSStringRef jsval = JSStringCreateWithUTF8CString(val);
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

  virtual StrVector getKeys() {
    StrVector keys;
    JSPropertyNameArrayRef names_array = JSObjectCopyPropertyNames(context, object);
    size_t count = JSPropertyNameArrayGetCount(names_array);
    keys.reserve(count);
    for(size_t idx = 0; idx < count; ++idx) {
      JSStringRef str_ref = JSPropertyNameArrayGetNameAtIndex(names_array, idx);
      JSValueJSC val(context, JSValueMakeString(context, str_ref));
      keys[idx] = val.asString();
    }
    JSPropertyNameArrayRelease(names_array);
    return keys;
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

  virtual void setAt(unsigned int index, const char* val) {
    JSStringRef jsval = JSStringCreateWithUTF8CString(val);
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

class JSContextJSC : public JSContext {

public:

  JSContextJSC(JSContextRef context) : context(context) {}

  virtual JSValuePtr newString(const std::string& val) {
    JSStringRef jsval = JSStringCreateWithUTF8CString(val.c_str());
    return JSValuePtr(new JSValueJSC(context, JSValueMakeString(context, jsval)));
  }

  virtual JSValuePtr newString(const char* val) {
    JSStringRef jsval = JSStringCreateWithUTF8CString(val);
    return JSValuePtr(new JSValueJSC(context, JSValueMakeString(context, jsval)));
  };

  virtual JSValuePtr newBoolean(bool val) {
    return JSValuePtr(new JSValueJSC(context, JSValueMakeBoolean(context, val)));
  };

  virtual JSValuePtr newNumber(double val) {
    return JSValuePtr(new JSValueJSC(context, JSValueMakeNumber(context, val)));
  }

  virtual JSObjectPtr newObject() {
    return JSObjectPtr(new JSObjectJSC(context, JSObjectMake(context, 0, 0)));
  }

  virtual JSArrayPtr newArray(unsigned int length) {
    JSValueRef *arguments = new JSValueRef[length];
    for(size_t idx=0; idx < length; ++idx) {
      arguments[idx] = JSValueMakeUndefined(context);
    }
    JSArrayPtr result(new JSArrayJSC(context, JSObjectMakeArray(context, length, arguments, 0)));
    return result;
  }

  virtual JSValuePtr null() {
    return JSValuePtr(new JSValueJSC(context, JSValueMakeNull(context)));
  };

  virtual JSValuePtr undefined() {
    return JSValuePtr(new JSValueJSC(context, JSValueMakeUndefined(context)));
  };

  virtual std::string toJson(JSValuePtr val) {
    JSValueJSC* _val = dynamic_cast<JSValueJSC*>(JSOBJECTS_PTR_GET(val));
    JSStringRef json_str = JSValueCreateJSONString(context, _val->value, 0, 0);
    JSValueJSC strval(context, JSValueMakeString(context, json_str));
    if(strval.isString())
      return strval.asString();

    return "serialisation error";
  };

  virtual JSValuePtr fromJson(const std::string& str) {
    JSStringRef jsstr = JSStringCreateWithUTF8CString(str.c_str());
    JSValuePtr result(new JSValueJSC(context, JSValueMakeFromJSONString(context, jsstr)));
    JSStringRelease(jsstr);
    return result;
  };

private:

  JSContextRef context;

};

#endif //JSOBJECTS_JSC_HPP
