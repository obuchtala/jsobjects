#ifndef JSOBJECTS_CPP_HPP
#define JSOBJECTS_CPP_HPP

#include <assert.h>
#include <map>
#include <vector>

#include "jsobjects.hpp"

namespace jsobjects {
  
class JSValueCpp: virtual public JSValue {

protected:

  JSValueCpp() {}

public:

  JSValueCpp(const std::string& val) {
      type = String;
      str = val;
  }

  JSValueCpp(const char* val) {
    type = String;
    str = val;
  }

  explicit JSValueCpp(const bool val) {
      type = Boolean;
      b = val;
  }

  JSValueCpp(const double val) {
      type = Number;
      d = val;
  }

  JSValueCpp(JSObjectPtr val) {
    type = Object;
    obj = val;
  }

  ~JSValueCpp() {
  }

  virtual std::string asString() {
    assert(type == String);
    return str;
  }

  virtual  double asDouble() {
    assert(type == Number);
    return d;
  }
  
  virtual inline JSArrayPtr asArray();

  virtual inline JSObjectPtr asObject();

  
  virtual inline JSObjectPtr toObject(JSArrayPtr arr);

  virtual inline JSValuePtr toValue(JSArrayPtr arr);

  virtual inline JSValuePtr toValue(JSObjectPtr obj);

  virtual  bool asBool() {
    assert(type == Boolean);
    return b;
  }

  virtual  JSValueType getType() {
    return type;
  };

  JSValuePtr  null() {
    JSValueCpp *_null = new JSValueCpp();
    _null->type = Null;
    return JSValuePtr(_null);
  }

  JSValuePtr  undefined() {
    JSValueCpp *_undefined = new JSValueCpp();
    _undefined->type = Undefined;
    return JSValuePtr(_undefined);
  }

protected:

  std::string str;
  bool b;
  double d;
  JSObjectPtr obj;

  JSValueType type;
};

class JSObjectCpp: public JSValueCpp, virtual public JSObject {

public:
  JSObjectCpp() {
    type = Object;
  }

  virtual JSValuePtr get(const std::string& key) {
    return map[key];
  }

  virtual void set(const std::string& key, JSValuePtr val) {
    map[key] = val;
  }

  virtual void set(const std::string& key, const std::string& val) {
    map[key] = JSValuePtr(new JSValueCpp(val));
  }

  virtual void set(const std::string& key, const char* val) {
    map[key] = JSValuePtr(new JSValueCpp(std::string(val)));
  }

  virtual void set(const std::string& key, bool val) {
    map[key] = JSValuePtr(new JSValueCpp(val));
  }

  virtual void set(const std::string& key, double val) {
    map[key] = JSValuePtr(new JSValueCpp(val));
  }

  static JSObjectPtr Create() {
    return JSObjectPtr(new JSObjectCpp());
  }

  virtual StrVector getKeys() {
    StrVector keys;
    for(std::map<std::string, JSValuePtr>::iterator it = map.begin(); it != map.end(); ++it) {
      keys.push_back(it->first);
    }
    return keys;
  }

protected:
  std::map<std::string, JSValuePtr> map;
};

class JSArrayCpp: public JSObjectCpp, virtual public JSArray {

public:

  JSArrayCpp(int length) {
    type = Array;
    vector.resize(length);
    for(size_t idx = 0; idx < length; ++idx) {
      vector[idx] = undefined();
    }
  }

  virtual JSValuePtr getAt(unsigned int index) {
    return vector[index];
  }

  virtual void setAt(unsigned int index, JSValuePtr val) {
    vector[index] = val;
  };

  virtual void setAt(unsigned int index, const std::string& val) {
    vector[index] = JSValuePtr(new JSValueCpp(val));
  }

  virtual void setAt(unsigned int index, const char* val) {
    vector[index] = JSValuePtr(new JSValueCpp(std::string(val)));
  }

  virtual void setAt(unsigned int index, bool val) {
    vector[index] = JSValuePtr(new JSValueCpp(val));
  }

  virtual void setAt(unsigned int index, double val) {
    vector[index] = JSValuePtr(new JSValueCpp(val));
  }

  virtual unsigned int length() {
    return vector.size();
  }

protected:
  std::vector<JSValuePtr> vector;
};

class JSContextCpp : public JSContext {

public:

  JSContextCpp() {
    JSValueCpp *creator = new JSValueCpp(0.0);
    _null = creator->null();
    _undefined = creator->undefined();
    delete creator;
  }

  virtual JSValuePtr newString(const std::string& val) {
    return JSValuePtr(new JSValueCpp(val));
  }

  virtual JSValuePtr newString(const char* val) {
    return JSValuePtr(new JSValueCpp(val));
  }

  virtual JSValuePtr newBoolean(bool val) {
    return JSValuePtr(new JSValueCpp(val));
  }

  virtual JSValuePtr newNumber(double val) {
    return JSValuePtr(new JSValueCpp(val));
  }

  virtual JSObjectPtr newObject() {
    return JSObjectPtr(new JSObjectCpp());
  }

  virtual JSArrayPtr newArray(unsigned int length) {
    return JSArrayPtr(new JSArrayCpp(length));
  }

  virtual JSValuePtr null() {
    return _null;
  }

  virtual JSValuePtr undefined() {
    return _undefined;
  }

  JSObjectPtr newObject(const std::map<std::string, JSValuePtr> &vals) {
    JSObjectPtr obj(new JSObjectCpp());
    for(std::map<std::string, JSValuePtr>::const_iterator it = vals.begin();
          it != vals.end(); ++it) {
      obj->set(it->first, it->second);
    }
    return obj;
  }

  JSArrayPtr newArray(const std::vector<JSValuePtr> vals) {
    JSArrayPtr array(new JSArrayCpp(vals.size()));
    for(size_t idx=0; idx < vals.size(); ++idx) {
      array->setAt(idx, vals[idx]);
    }
    return array;
  }

  virtual std::string toJson(JSValuePtr val);

  virtual JSValuePtr fromJson(const std::string& str);

private:

  JSValuePtr _null;
  
  JSValuePtr _undefined;
};

JSArrayPtr JSValueCpp::asArray() {
  throw "Not supported";
}

JSObjectPtr JSValueCpp::asObject() {
  throw "Not supported";
}

JSObjectPtr JSValueCpp::toObject(JSArrayPtr arr) {
  return boost::dynamic_pointer_cast<JSObjectCpp>(arr);
}

JSValuePtr JSValueCpp::toValue(JSArrayPtr arr) {
  return boost::dynamic_pointer_cast<JSValueCpp>(arr);
}

JSValuePtr JSValueCpp::toValue(JSObjectPtr obj) {
  return boost::dynamic_pointer_cast<JSValueCpp>(obj);
}


template<typename A>
class JSVoidFunctionCpp: public JSVoidFunction<A> {
public:
  JSVoidFunctionCpp(void (*f) (A val)): f(f) {
  }

  virtual void call(A val) {
    f(val);
  }

private:
  void (*f) (A val);
};

template<typename O, typename A>
class JSVoidMemberFunctionCpp: public JSVoidFunction<A> {
public:
  JSVoidMemberFunctionCpp(O& obj, void (O::*f) (A val)): obj(obj), f(f) {
  }

  virtual void call(A val) {
    ((obj).*(f)) (val);
  }

private:
  void (O::*f) (A val);
  O& obj;
};

template<typename A>
typename JSVoidFunction<A>::Ptr CreateVoidFunction(void (*f) (A val) ) {
  return typename JSVoidFunction<A>::Ptr(new JSVoidFunctionCpp<A>(f));
}

template<typename O, typename A>
typename JSVoidFunction<A>::Ptr CreateMemberVoidFunction(O& obj, void (O::*f) (A val) ) {
  return typename JSVoidFunction<A>::Ptr(new JSVoidMemberFunctionCpp<O, A>(obj, f));
}


} // namespace jsobjects

#endif // JSOBJECTS_CPP_HPP
