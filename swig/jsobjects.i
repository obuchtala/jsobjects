
typedef std::vector<std::string> StrVector;

namespace jsobjects {

class JSValue;
class JSObject;
class JSArray;

typedef boost::shared_ptr< JSValue > JSValuePtr;
typedef boost::shared_ptr< JSObject > JSObjectPtr;
typedef boost::shared_ptr< JSArray > JSArrayPtr;
typedef boost::shared_ptr< JSContext > JSContextPtr;

} // namespace jsobjects

#ifdef SWIG_JAVASCRIPT_V8
%include <jsobjects_v8.i>
#else
%include <jsobjects_jsc.i>
#endif
