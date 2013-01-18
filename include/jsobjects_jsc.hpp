#ifndef JSOBJECTS_JSC_HPP
#define JSOBJECTS_JSC_HPP

#include "jsobjects.hpp"

#include <JavaScriptCore/JavaScript.h>
#include <assert.h>

namespace jsobjects {

class JSObjectJSC;
class JSArrayJSC;

class JSValueJSC: virtual public JSValue {

public:

  JSValueJSC(JSContextRef context, JSValueRef val): context(context) {

    if(val == 0) {
      val = JSValueMakeNull(context);
      type = Null;
    } else if(JSValueIsUndefined(context, val)) {
      type = Undefined;
    } else if(JSValueIsNull(context, val)) {
      type = Null;
    } else if(JSValueIsBoolean(context, val)) {
      type = Boolean;
    } else if(JSValueIsNumber(context, val)) {
      type = Number;
    } else if(JSValueIsString(context, val)) {
      type = String;
    } else if(JSValueJSC::_IsArray(context, val)) {
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

  virtual ~JSValueJSC() {
    // release the persistent reference
    JSValueUnprotect(context, value);
  }

  inline virtual std::string asString() {
    assert(JSValueIsString(context, value));

    JSStringRef jsstring = JSValueToStringCopy(context, value, /* JSValueRef *exception */ 0);
    size_t len = JSStringGetMaximumUTF8CStringSize(jsstring);
    char *cstr = new char[len];
    JSStringGetUTF8CString(jsstring, cstr, len);
    std::string result(cstr);

    JSStringRelease(jsstring);
    delete[] cstr;

    return result;
  }

  inline virtual double asDouble() {
    assert(JSValueIsNumber(context, value));
    return JSValueToNumber(context, value, /* JSValueRef *exception */ 0);
  }

  inline virtual bool asBool() {
    assert(JSValueIsBoolean(context, value));
    return JSValueToBoolean(context, value);
  }

  inline virtual JSObjectPtr toObject(JSArrayPtr arr);

  inline virtual JSValuePtr toValue(JSArrayPtr arr);

  inline virtual JSValuePtr toValue(JSObjectPtr obj);

  inline virtual JSValueType getType() { return type; };

  inline virtual JSArrayPtr asArray();

  inline virtual JSObjectPtr asObject();

  JSContextRef context;
  JSValueRef value;

protected:

  inline bool _IsArray(JSContextRef context, JSValueRef val);
  inline JSObjectRef _GetArrayClassObj(JSContextRef context);

  JSValueType type;

};

class JSObjectJSC: public JSValueJSC, virtual public JSObject {

public:

  JSObjectJSC(JSContextRef context, JSObjectRef obj): JSValueJSC(context, obj), object(obj) {}

  JSObjectJSC(JSContextRef context, JSValueRef val): JSValueJSC(context, val) {
      assert(JSValueIsObject(context, val));
      object = JSValueToObject(context, val, 0);
  }

  virtual ~JSObjectJSC() { }

  virtual JSValuePtr get(const std::string& key) {
    JSStringRef jskey = JSStringCreateWithUTF8CString(key.c_str());
    JSValueRef val = JSObjectGetProperty(context, object, jskey, /* JSValueRef *exception */ 0);
    JSStringRelease(jskey);
    return JSValuePtr(new JSValueJSC(context, val));
  }

  virtual void set(const std::string& key, JSValuePtr val) {
    JSStringRef jskey = JSStringCreateWithUTF8CString(key.c_str());
    JSValueJSC* jscval = dynamic_cast<JSValueJSC*>(JSOBJECTS_PTR_GET(val));
    JSObjectSetProperty(context, object, jskey, jscval->value, kJSPropertyAttributeNone, /* JSValueRef *exception */ 0);
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

  JSObjectRef object;
};

class JSArrayJSC: public JSObjectJSC, virtual public JSArray {

public:

  JSArrayJSC(JSContextRef context, JSObjectRef arr): JSObjectJSC(context, arr) { }

  virtual JSValuePtr getAt(unsigned int index) {
    return JSValuePtr(new JSValueJSC(context, JSObjectGetPropertyAtIndex(context, object, index, /* JSValueRef *exception */ 0)));
  }

  virtual void setAt(unsigned int index, JSValuePtr val) {
    JSObjectSetPropertyAtIndex(context, object, index, dynamic_cast<JSValueJSC*>(JSOBJECTS_PTR_GET(val))->value, /* JSValueRef *exception */ 0);
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

  inline virtual unsigned int length();
};

class JSContextJSC : public JSContext {

public:

  JSContextJSC(JSContextRef context) : context(context) {
  }

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
    JSObjectJSC* obj = new JSObjectJSC(context, JSObjectMake(context, 0, 0));
    return JSObjectPtr(obj);
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
    JSValueRef undefined = JSValueMakeUndefined(context);
    return JSValuePtr(new JSValueJSC(context, undefined));
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
    JSValueRef val = JSValueMakeFromJSONString(context, jsstr);

    // TODO: throw exception
    if (val == 0) return undefined();

    JSValuePtr result(new JSValueJSC(context, val));
    JSStringRelease(jsstr);
    return result;
  };


private:

  JSContextRef context;

};


JSObjectRef JSValueJSC::_GetArrayClassObj(JSContextRef context)
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

bool JSValueJSC::_IsArray(JSContextRef context, JSValueRef value)
{
  static JSObjectRef array_class_obj = _GetArrayClassObj(context);

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

JSArrayPtr JSValueJSC::asArray() {
  assert(isArray());
  return JSArrayPtr(new JSArrayJSC(context, const_cast<JSObjectRef>(value)));
}

JSObjectPtr JSValueJSC::asObject() {
  assert(isObject());
  return JSObjectPtr(new JSObjectJSC(context, const_cast<JSObjectRef>(value)));
}

JSObjectPtr JSValueJSC::toObject(JSArrayPtr arr) {
  return boost::dynamic_pointer_cast<JSObjectJSC>(arr);
}

JSValuePtr JSValueJSC::toValue(JSArrayPtr arr) {
  return boost::dynamic_pointer_cast<JSValueJSC>(arr);
}

JSValuePtr JSValueJSC::toValue(JSObjectPtr obj) {
  return boost::dynamic_pointer_cast<JSValueJSC>(obj);
}


} // namespace jsobjects

#endif //JSOBJECTS_JSC_HPP
