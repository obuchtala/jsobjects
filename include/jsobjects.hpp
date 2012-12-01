#ifndef _JSOBJECTS_HPP_
#define _JSOBJECTS_HPP_

#include <string>
#include <vector>

#include <assert.h>

typedef std::vector<std::string> StrVector;

class JSValue;
class JSObject;
class JSArray;
class JSContext;

#ifdef USE_BOOST_SHARED_PTR

typedef boost::shared_ptr< JSValue > JSValuePtr;
typedef boost::shared_ptr< JSObject > JSObjectPtr;
typedef boost::shared_ptr< JSArray > JSArrayPtr;
typedef boost::shared_ptr< JSContext > JSContextPtr;

#define JSOBJECTS_PTR_TYPE(type) boost::shared_ptr< type >
#define JSOBJECTS_PTR_GET(val) val.get()
#define JSOBJECTS_PTR_FREE(val)

#else

typedef JSValue* JSValuePtr;
typedef JSObject* JSObjectPtr;
typedef JSArray* JSArrayPtr;
typedef JSContext* JSContextPtr;

#define JSOBJECTS_PTR_TYPE(type) type*
#define JSOBJECTS_PTR_GET(val) val
#define JSOBJECTS_PTR_FREE(val) delete val

#endif

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

  inline int asInteger() {
    return static_cast<int>(asDouble());
  };

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

  static JSArrayPtr asArray(JSValuePtr val);

  static JSObjectPtr asObject(JSValuePtr val);

  static JSObjectPtr asObject(JSArrayPtr arr);

  static JSValuePtr asValue(JSArrayPtr arr);

  static JSValuePtr asValue(JSObjectPtr obj);
};

class JSObject {

public:

  virtual ~JSObject () {}

  virtual JSValuePtr get(const std::string& key) = 0;

  virtual void set(const std::string& key, JSValuePtr val) = 0;

  virtual void set(const std::string& key, const std::string& val) = 0;

  virtual void set(const std::string& key, const char* val) = 0;

  virtual void set(const std::string& key, bool val) = 0;

  virtual void set(const std::string& key, double val) = 0;

  virtual StrVector getKeys() = 0;

  inline void set(const std::string& key, JSArrayPtr val) {
    set(key, JSValue::asValue(val));
  };

  inline void set(const std::string& key, JSObjectPtr val) {
    set(key, JSValue::asValue(val));
  };

};

class JSArray {

public:

  virtual ~JSArray () {}

  virtual unsigned int length() = 0;

  virtual JSValuePtr getAt(unsigned int index) = 0;

  virtual void setAt(unsigned int index, JSValuePtr val) = 0;

  virtual void setAt(unsigned int index, const std::string& val) = 0;

  virtual void setAt(unsigned int index, const char* val) = 0;

  virtual void setAt(unsigned int index, bool val) = 0;

  virtual void setAt(unsigned int index, double val) = 0;

  inline void setAt(unsigned int index, JSArrayPtr val) {
    setAt(index, JSValue::asValue(val));
  };

  inline void set(unsigned int index, JSObjectPtr val) {
    setAt(index, JSValue::asValue(val));
  };

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

#endif // _JSOBJECTS_HPP_
