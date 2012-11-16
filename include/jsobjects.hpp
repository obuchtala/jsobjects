#ifndef _JAVASCRIPT_H_
#define _JAVASCRIPT_H_

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>


typedef std::vector<std::string> StrVector;

class JSValue;
typedef boost::shared_ptr<JSValue> JSValuePtr;
class JSObject;
typedef boost::shared_ptr<JSObject> JSObjectPtr;
class JSArray;
typedef boost::shared_ptr<JSArray> JSArrayPtr;

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
  
  virtual std::string asString() = 0;

  virtual double asDouble() = 0;
  
  virtual bool asBool() = 0;

  virtual JSValueType getType() = 0;

  inline int asInteger() {
    return static_cast<int>(asDouble());
  };
  
  static JSArrayPtr asArray(JSValuePtr val) {
    assert(val->getType() == Array);
    JSArrayPtr arrPtr = boost::dynamic_pointer_cast<JSArray>(val);
    return arrPtr;
  }

  static JSObjectPtr asObject(JSValuePtr val) {
    assert(val->getType() == Object || val->getType() == Array);
    JSObjectPtr objPtr = boost::dynamic_pointer_cast<JSObject>(val);
    return objPtr;
  }

  static JSObjectPtr asObject(JSArrayPtr arr) {
    JSObjectPtr objPtr = boost::dynamic_pointer_cast<JSObject>(arr);
    return objPtr;
  }

  static JSValuePtr asValue(JSArrayPtr arr) {
    JSValuePtr valPtr = boost::dynamic_pointer_cast<JSValue>(arr);
    return valPtr;
  }

  static JSValuePtr asValue(JSObjectPtr obj) {
    JSValuePtr valPtr = boost::dynamic_pointer_cast<JSValue>(obj);
    return valPtr;
  }

  inline bool isNull() {
   return(getType() == JSValue::Null);
  }

  inline bool isUndefined()  {
   return(getType() == JSValue::Undefined);
  }
  
  inline bool isString() {
   return(getType() == JSValue::String);
  }

  inline bool isNumber() {
   return(getType() == JSValue::Number);
  }

  inline bool isBoolean() {
   return(getType() == JSValue::Boolean);
  }

  inline bool isObject() {
   return(getType() == JSValue::Object);
  }

  inline bool isArray() {
   return(getType() == JSValue::Array);
  }
  
};

class JSObject {

public:

  virtual JSValuePtr get(const std::string& key) = 0;

  virtual void set(const std::string& key, JSValuePtr val) = 0;

  virtual void set(const std::string& key, const std::string& val) = 0;

  virtual void set(const std::string& key, const char* val) = 0;

  virtual void set(const std::string& key, bool val) = 0;

  virtual void set(const std::string& key, double val) = 0;

  virtual StrVector getKeys() = 0;

  inline void set(const std::string& key, JSArrayPtr val) {
    set(key, boost::dynamic_pointer_cast<JSValue>(val));
  };

  inline void set(const std::string& key, JSObjectPtr val) {
    set(key, boost::dynamic_pointer_cast<JSValue>(val));
  };

};

class JSArray {

public:

  virtual unsigned int length() = 0;

  virtual JSValuePtr getAt(unsigned int index) = 0;

  virtual void setAt(unsigned int index, JSValuePtr val) = 0;

  virtual void setAt(unsigned int index, const std::string& val) = 0;

  virtual void setAt(unsigned int index, const char* val) = 0;

  virtual void setAt(unsigned int index, bool val) = 0;

  virtual void setAt(unsigned int index, double val) = 0;

  inline void setAt(unsigned int index, JSArrayPtr val) {
    setAt(index, boost::dynamic_pointer_cast<JSValue>(val));
  };

  inline void set(unsigned int index, JSObjectPtr val) {
    setAt(index, boost::dynamic_pointer_cast<JSValue>(val));
  };

};


class JSContext {

public:

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

template <typename A>
class JSVoidFunction {
public:
  typedef typename boost::shared_ptr< JSVoidFunction<A> > Ptr;

  virtual void call(A arg) = 0;
};

template <typename R, typename A>
class JSFunction {
public:
  typedef typename boost::shared_ptr<JSFunction<R, A> > Ptr;
  
  virtual R call(A arg) = 0;
};

#endif
