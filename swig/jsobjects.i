
typedef std::vector<std::string> StrVector;
class JSValue;
class JSObject;
class JSArray;

#ifdef USE_BOOST_SHARED_PTR

typedef boost::shared_ptr< JSValue > JSValuePtr;
typedef boost::shared_ptr< JSObject > JSObjectPtr;
typedef boost::shared_ptr< JSArray > JSArrayPtr;
typedef boost::shared_ptr< JSContext > JSContextPtr;

#define WR

#else

typedef JSValue* JSValuePtr;
typedef JSObject* JSObjectPtr;
typedef JSArray* JSArrayPtr;
typedef JSContext* JSContextPtr;

#endif

#ifdef SWIG_JAVASCRIPT_V8
%include <jsobjects_v8.i>
#else
%include <jsobjects_jsc.i>
#endif
