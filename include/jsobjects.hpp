#ifndef _JAVASCRIPT_H_
#define _JAVASCRIPT_H_

#include <string>
#include <vector>

#include <assert.h>

typedef std::vector<std::string> StrVector;

class JSValue;
class JSObject;
class JSArray;

typedef JSValue* JSValuePtr;
typedef JSObject* JSObjectPtr;
typedef JSArray* JSArrayPtr;

#ifdef USE_BOOST_SHARED_PTR

#define JSOBJECTS_PTR_TYPE(type) boost::shared_ptr< type >
#define JSOBJECTS_PTR_GET(val) val.get()

#else

#define JSOBJECTS_PTR_TYPE(type) type*
#define JSOBJECTS_PTR_GET(val) val

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

  virtual std::string asString() = 0;

  virtual double asDouble() = 0;

  virtual bool asBool() = 0;

  virtual JSValueType getType() = 0;

  inline int asInteger() {
    return static_cast<int>(asDouble());
  };

  static inline JSArrayPtr asArray(JSValuePtr val);

  static inline JSObjectPtr asObject(JSValuePtr val);

  static inline JSObjectPtr asObject(JSArrayPtr arr);

  static inline JSValuePtr asValue(JSArrayPtr arr);

  static inline JSValuePtr asValue(JSObjectPtr obj);

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
    set(key, JSValue::asValue(val));
  };

  inline void set(const std::string& key, JSObjectPtr val) {
    set(key, JSValue::asValue(val));
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
    setAt(index, JSValue::asValue(val));
  };

  inline void set(unsigned int index, JSObjectPtr val) {
    setAt(index, JSValue::asValue(val));
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

#endif
