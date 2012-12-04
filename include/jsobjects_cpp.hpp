#ifndef JSOBJECTS_CPP_HPP
#define JSOBJECTS_CPP_HPP

#include <assert.h>
#include <map>
#include <vector>

#include "jsobjects.hpp"

namespace jsobjects {

class JSValueCpp: virtual public JSValue {

protected:

  class _Data {

  public:

    std::string str;
    bool b;
    double d;
    std::map<std::string, JSValuePtr> map;
    std::vector<JSValuePtr> vector;
  };

  typedef boost::shared_ptr<_Data> DataPtr;

  JSValueCpp(): data(new _Data()) { }

  JSValueCpp(JSValueType type): data(new _Data()), type(type) { }

public:

  JSValueCpp(const std::string& val): data(new _Data()) {
      type = String;
      data->str = val;
  }

  JSValueCpp(const char* val): data(new _Data()) {
    type = String;
    data->str = val;
  }

  explicit JSValueCpp(const bool val): data(new _Data()) {
      type = Boolean;
      data->b = val;
  }

  JSValueCpp(const double val): data(new _Data()) {
      type = Number;
      data->d = val;
  }

  //JSValueCpp(JSObjectPtr val);

  ~JSValueCpp() {
  }

  virtual std::string asString() {
    assert(type == String);
    return data->str;
  }

  virtual  double asDouble() {
    assert(type == Number);
    return data->d;
  }

  virtual inline JSArrayPtr asArray();

  virtual inline JSObjectPtr asObject();

  virtual inline JSObjectPtr toObject(JSArrayPtr arr);

  virtual inline JSValuePtr toValue(JSArrayPtr arr);

  virtual inline JSValuePtr toValue(JSObjectPtr obj);

  virtual  bool asBool() {
    assert(type == Boolean);
    return data->b;
  }

  virtual  JSValueType getType() {
    return type;
  };

  JSValuePtr  null() {
    static JSValuePtr _null(new JSValueCpp(Null));
    return _null;
  }

  JSValuePtr  undefined() {
    static JSValuePtr _undefined(new JSValueCpp(Undefined));
    return _undefined;
  }

protected:

  JSValueType type;
  DataPtr data;
};

class JSObjectCpp: public JSValueCpp, virtual public JSObject {

public:

  JSObjectCpp(): JSValueCpp(Object) { }

  JSObjectCpp(DataPtr data): JSValueCpp(Object) {
    this->data = data;
  }

  virtual JSValuePtr get(const std::string& key) {
    return data->map[key];
  }

  virtual void set(const std::string& key, JSValuePtr val) {
    data->map[key] = val;
  }

  virtual void set(const std::string& key, const std::string& val) {
    data->map[key] = JSValuePtr(new JSValueCpp(val));
  }

  virtual void set(const std::string& key, const char* val) {
    data->map[key] = JSValuePtr(new JSValueCpp(std::string(val)));
  }

  virtual void set(const std::string& key, bool val) {
    data->map[key] = JSValuePtr(new JSValueCpp(val));
  }

  virtual void set(const std::string& key, double val) {
    data->map[key] = JSValuePtr(new JSValueCpp(val));
  }

  virtual StrVector getKeys() {
    StrVector keys;
    for(std::map<std::string, JSValuePtr>::iterator it = data->map.begin();
	it != data->map.end(); ++it) {
      keys.push_back(it->first);
    }
    return keys;
  }

};

class JSArrayCpp: public JSObjectCpp, virtual public JSArray {

public:

  JSArrayCpp(int length) {
    type = Array;
    data->vector.resize(length);
    for(size_t idx = 0; idx < length; ++idx) {
      data->vector[idx] = undefined();
    }
  }

  JSArrayCpp(DataPtr data) {
    type = Array;
    this->data = data;
  }

  virtual JSValuePtr getAt(unsigned int index) {
    return data->vector[index];
  }

  virtual void setAt(unsigned int index, JSValuePtr val) {
    data->vector[index] = val;
  };

  virtual void setAt(unsigned int index, const std::string& val) {
    data->vector[index] = JSValuePtr(new JSValueCpp(val));
  }

  virtual void setAt(unsigned int index, const char* val) {
    data->vector[index] = JSValuePtr(new JSValueCpp(std::string(val)));
  }

  virtual void setAt(unsigned int index, bool val) {
    data->vector[index] = JSValuePtr(new JSValueCpp(val));
  }

  virtual void setAt(unsigned int index, double val) {
    data->vector[index] = JSValuePtr(new JSValueCpp(val));
  }

  virtual unsigned int length() {
    return data->vector.size();
  }
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
  return JSArrayPtr(new JSArrayCpp(data));
}

JSObjectPtr JSValueCpp::asObject() {
  return JSObjectPtr(new JSObjectCpp(data));
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
