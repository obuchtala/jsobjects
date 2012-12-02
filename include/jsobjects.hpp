#ifndef _JSOBJECTS_HPP_
#define _JSOBJECTS_HPP_

#include <string>
#include <vector>
#include <assert.h>

#include <boost/shared_ptr.hpp>

typedef std::vector<std::string> StrVector;

namespace jsobjects {

class JSValue;
class JSObject;
class JSArray;
class JSContext;

typedef boost::shared_ptr< JSValue > JSValuePtr;
typedef boost::shared_ptr< JSObject > JSObjectPtr;
typedef boost::shared_ptr< JSArray > JSArrayPtr;
typedef boost::shared_ptr< JSContext > JSContextPtr;

#define JSOBJECTS_PTR_TYPE(type) boost::shared_ptr< type >
#define JSOBJECTS_PTR_GET(val) val.get()
#define JSOBJECTS_PTR_FREE(val)

class JSValue {

public:

  enum JSValueType {
    Object,
    Array,
    String,
    Number,
    Boolean,
    Null,
    Undefined
  };

public:

  virtual ~JSValue() {}

  inline virtual std::string asString() { throw "don't call me!"; };

  inline virtual double asDouble() { throw "don't call me!"; };

  inline virtual bool asBool() { throw "don't call me!"; };

  inline virtual JSValueType getType() { throw "don't call me!"; };

  inline virtual JSArrayPtr asArray() { throw "don't call me!"; };

  inline virtual JSObjectPtr asObject() { throw "don't call me!"; };

  inline virtual JSObjectPtr toObject(JSArrayPtr arr) { throw "don't call me!"; };

  inline virtual JSValuePtr toValue(JSArrayPtr arr) { throw "don't call me!"; };

  inline virtual JSValuePtr toValue(JSObjectPtr obj) { throw "don't call me!"; };

  inline int asInteger();

  inline bool isNull();

  inline bool isUndefined();

  inline bool isString();

  inline bool isNumber();

  inline bool isBoolean();

  inline bool isObject();

  inline bool isArray();
};

class JSObject: public JSValue {

public:

  virtual ~JSObject () {}

  inline virtual JSValuePtr get(const std::string& key) { throw "don't call me!"; };

  inline virtual void set(const std::string& key, JSValuePtr val) { throw "don't call me!"; };

  inline virtual void set(const std::string& key, const std::string& val) { throw "don't call me!"; };

  inline virtual void set(const std::string& key, const char* val) { throw "don't call me!"; };

  inline virtual void set(const std::string& key, bool val) { throw "don't call me!"; };

  inline virtual void set(const std::string& key, double val) { throw "don't call me!"; };

  inline virtual StrVector getKeys() { throw "don't call me!"; };

  inline void set(const std::string& key, JSArrayPtr val);

  inline void set(const std::string& key, JSObjectPtr val);

};

class JSArray: public JSObject {

public:

  virtual ~JSArray () {}

  inline virtual unsigned int length() { throw "don't call me!"; };

  inline virtual JSValuePtr getAt(unsigned int index) { throw "don't call me!"; };

  inline virtual void setAt(unsigned int index, JSValuePtr val) { throw "don't call me!"; };

  inline virtual void setAt(unsigned int index, const std::string& val) { throw "don't call me!"; };

  inline virtual void setAt(unsigned int index, const char* val) { throw "don't call me!"; };

  inline virtual void setAt(unsigned int index, bool val) { throw "don't call me!"; };

  inline virtual void setAt(unsigned int index, double val) { throw "don't call me!"; };

  inline void setAt(unsigned int index, JSArrayPtr val);

  inline void set(unsigned int index, JSObjectPtr val);
};


class JSContext {

public:

  virtual ~JSContext () {}

  virtual JSValuePtr newString(const std::string& val) = 0;

  virtual JSValuePtr newString(const char* val) = 0;

  virtual JSValuePtr newBoolean(bool val) = 0;

  virtual JSValuePtr newNumber(double val) = 0;

  virtual JSObjectPtr newObject() = 0;

  virtual JSArrayPtr newArray(unsigned int length) = 0;

  virtual JSValuePtr null() = 0;

  virtual JSValuePtr undefined() = 0;

  virtual std::string toJson(JSValuePtr val) = 0;

  virtual JSValuePtr fromJson(const std::string& str) = 0;

};


int JSValue::asInteger() {
  return static_cast<int>(asDouble());
};

bool JSValue::isNull() {
  return(getType() == JSValue::Null);
}

bool JSValue::isUndefined()  {
  return(getType() == JSValue::Undefined);
}

bool JSValue::isString() {
  return(getType() == JSValue::String);
}

bool JSValue::isNumber() {
  return(getType() == JSValue::Number);
}

bool JSValue::isBoolean() {
  return(getType() == JSValue::Boolean);
}

bool JSValue::isObject() {
  return(getType() == JSValue::Object);
}

bool JSValue::isArray() {
  return(getType() == JSValue::Array);
}

void JSObject::set(const std::string& key, JSArrayPtr val) {
  set(key, toValue(val));
};

void JSObject::set(const std::string& key, JSObjectPtr val) {
  set(key, toValue(val));
};

void JSArray::setAt(unsigned int index, JSArrayPtr val) {
  setAt(index, toValue(val));
};

void JSArray::set(unsigned int index, JSObjectPtr val) {
  setAt(index, toValue(val));
};

template <typename A>
class JSVoidFunction {
public:
  typedef JSVoidFunction<A> _JSVoidFunction;
  typedef JSOBJECTS_PTR_TYPE(_JSVoidFunction) Ptr;

  virtual void call(A arg) = 0;
};

template <typename R, typename A>
class JSFunction {

public:
  typedef JSFunction<R,A> _JSFunctionType;
  typedef JSOBJECTS_PTR_TYPE(_JSFunctionType) Ptr;

  virtual R call(A arg) = 0;
};
  
} // namespace jsobjects

#endif // _JSOBJECTS_HPP_
