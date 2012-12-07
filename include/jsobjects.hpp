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

  virtual std::string asString() = 0;

  virtual double asDouble() = 0;

  virtual bool asBool() = 0;

  virtual JSValueType getType() = 0;

  virtual JSArrayPtr asArray() = 0;

  virtual JSObjectPtr asObject() = 0;

  virtual JSObjectPtr toObject(JSArrayPtr arr) = 0;

  virtual JSValuePtr toValue(JSArrayPtr arr) = 0;

  virtual JSValuePtr toValue(JSObjectPtr obj) = 0;

  inline int asInteger();

  inline bool isNull();

  inline bool isUndefined();

  inline bool isString();

  inline bool isNumber();

  inline bool isBoolean();

  inline bool isObject();

  inline bool isArray();
};

class JSObject: virtual public JSValue {

public:

  virtual ~JSObject () {}

  virtual JSValuePtr get(const std::string& key) = 0;

  virtual void set(const std::string& key, JSValuePtr val) = 0;

  virtual void set(const std::string& key, const std::string& val) = 0;

  virtual void set(const std::string& key, const char* val) = 0;

  virtual void set(const std::string& key, bool val) = 0;

  virtual void set(const std::string& key, double val) = 0;

  virtual StrVector getKeys() = 0;

  inline void set(const std::string& key, JSArrayPtr val);

  inline void set(const std::string& key, JSObjectPtr val);

};

class JSArray: virtual public JSObject {

public:

  virtual ~JSArray () {}

  virtual unsigned int length() = 0;

  virtual JSValuePtr getAt(unsigned int index) = 0;

  virtual void setAt(unsigned int index, JSValuePtr val) = 0;

  virtual void setAt(unsigned int index, const std::string& val) = 0;

  virtual void setAt(unsigned int index, const char* val) = 0;

  virtual void setAt(unsigned int index, bool val) = 0;

  virtual void setAt(unsigned int index, double val) = 0;

  inline void setAt(unsigned int index, JSArrayPtr val);

  inline void setAt(unsigned int index, JSObjectPtr val);
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

void JSArray::setAt(unsigned int index, JSObjectPtr val) {
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
