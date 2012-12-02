#include "jsobjects_cpp.hpp"

#include <rapidjson/encodedstream.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/reader.h>

#include <stack>

using rapidjson::UTF8;
using rapidjson::GenericStringBuffer;
using rapidjson::Writer;
using rapidjson::GenericReader;

namespace jsobjects {
  
void JSValueCpp_toJSON(Writer< GenericStringBuffer< UTF8<char> > > &w, JSValuePtr val);

void JSValueCpp_toJSON_Object(Writer< GenericStringBuffer< UTF8<char> > > &w, JSObjectPtr obj) {
  w.StartObject();
  const StrVector &keys = obj->getKeys();
  for(StrVector::const_iterator it = keys.begin(); it != keys.end(); ++it) {
    const std::string &key = *it;
    w.String(key.c_str());
    JSValueCpp_toJSON(w, obj->get(key));
  }
  w.EndObject();
}

void JSValueCpp_toJSON_Array(Writer< GenericStringBuffer< UTF8<char> > > &w, JSArrayPtr array) {
  size_t len = array->length();
  w.StartArray();
  for(size_t idx = 0; idx < len; ++idx) {
    JSValuePtr val = array->getAt(idx);
    JSValueCpp_toJSON(w, val);
  }
  w.EndArray();
}

void JSValueCpp_toJSON(Writer< GenericStringBuffer< UTF8<char> > > &w, JSValuePtr val) {
  switch(val->getType()) {
    case JSValue::Null:
      w.Null();
      break;
    case JSValue::Undefined:
      break;
    case JSValue::Boolean:
      w.Bool(val->asBool());
      break;
    case JSValue::Number:
      w.Double(val->asDouble());
      break;
    case JSValue::String:
      w.String(val->asString().c_str());
      break;
    case JSValue::Array:
      JSValueCpp_toJSON_Array(w, JSValue::asArray(val));
      break;
    case JSValue::Object:
      JSValueCpp_toJSON_Object(w, JSValue::asObject(val));
      break;
  }
}

class JSObjectReaderHandler {

private:

  typedef std::pair<std::string, JSValuePtr> ObjEntry;

  struct StackElem {
    JSValue::JSValueType type;

    std::vector<ObjEntry> obj;
    const char* key;
  };

public:

  JSObjectReaderHandler(JSContextCpp& context): context(context), root(0) {}

  void append(JSValuePtr val) {
    StackElem *tos = objStack.empty()? 0 : objStack.top();

    if(tos == 0) {
      assert(JSOBJECTS_PTR_GET(root) == 0);
      root = val;
    } else if(tos->type == JSValue::Object) {
      assert(tos->key != 0);
      tos->obj.push_back(ObjEntry(tos->key, val));
      tos->key = 0;
    } else if (tos->type == JSValue::Array) {
      tos->obj.push_back(ObjEntry("", val));
    } else {
      // should not reach
      assert(false);
    }
  }

  // what to do?
  void Default() {}

  void Null() {
    append(context.null());
  }

  void Bool(bool b) {
    append(context.newBoolean(b));
  }

  void Int(int i) {
    append(context.newNumber(i));
  }

  void Uint(unsigned i) {
    append(context.newNumber(i));
  }

  void Int64(int64_t i) {
    append(context.newNumber(i));
  }

  void Uint64(uint64_t i) {
    append(context.newNumber(i));
  }

  void Double(double d) {
    append(context.newNumber(d));
  }

  void String(const char* str, size_t length, bool copy) {
    StackElem *tos = objStack.empty() ? 0 : objStack.top();

    if(tos && tos->type == JSValue::Object && tos->key == 0) {
      char *_str = new char[length+1];
      strcpy(_str, str);
      tos->key = _str;
    } else {
      append(context.newString(str));
    }
  }

  void StartObject() {
    StackElem *elem = new StackElem;
    elem->type = JSValue::Object;
    elem->key = 0;
    objStack.push(elem);
  }

  void EndObject(size_t memberCount) {
    StackElem *elem = objStack.top(); objStack.pop();
    JSObjectPtr obj = context.newObject();
    for(std::vector<ObjEntry>::const_iterator it = elem->obj.begin();
          it != elem->obj.end(); ++it) {
      obj->set(it->first, it->second);
    }
    append(JSValue::asValue(obj));
    delete elem;
  }

  void StartArray() {
    StackElem *elem = new StackElem;
    elem->type = JSValue::Array;
    objStack.push(elem);
  }

  void EndArray(size_t elementCount) {
    StackElem *elem = objStack.top(); objStack.pop();
    JSArrayPtr array = context.newArray(elem->obj.size());
    size_t idx = 0;
    for(std::vector<ObjEntry>::const_iterator it = elem->obj.begin();
          it != elem->obj.end(); ++it) {
      array->setAt(idx++, it->second);
    }
    append(JSValue::asValue(array));
    delete elem;
  }

  JSValuePtr GetResult() {
    return root;
  }

private:

  JSContextCpp& context;
  JSValuePtr root;

  std::stack<StackElem*> objStack;
};

std::string JSContextCpp::toJson(JSValuePtr val)
{
  GenericStringBuffer<UTF8<char> > strbuf;
  Writer< GenericStringBuffer< UTF8<char> > > w(strbuf);
  JSValueCpp_toJSON(w, val);
  return strbuf.GetString();
}

#include <sstream>

class StringStream {

public:
  typedef char Ch;  //!< Character type (byte).

  StringStream(): str(""), ss(0) {}

  StringStream(const std::string& str) : str(str), ss(new std::stringstream(str)) { }

  StringStream(const StringStream& other) : str(other.str), ss(new std::stringstream(other.str)) {
    ss->seekg(other.ss->tellg());
  }

  ~StringStream() {
    delete ss;
  }

  StringStream& operator=(const StringStream& other) {
    delete ss;
    ss = new std::stringstream(other.str);
    ss->seekg(other.ss->tellg());
    return *this;
  }

  Ch Peek() {
    ss->peek();
  }

  Ch Take() {
    return ss->get();
  }

  size_t Tell() {
    return ss->tellg();
  }

  // Not implemented
  void Put(Ch c) { RAPIDJSON_ASSERT(false); }

  void Flush() { RAPIDJSON_ASSERT(false); }

  Ch* PutBegin() { RAPIDJSON_ASSERT(false); return 0; }

  size_t PutEnd(Ch*) { RAPIDJSON_ASSERT(false); return 0; }

  // For encoding detection only.
  const Ch* Peek4() const {
    return 0;
  }

private:
  std::stringstream *ss;
  const std::string &str;
};

JSValuePtr JSContextCpp::fromJson(const std::string& str) {
  JSObjectReaderHandler handler(*this);
  GenericReader<UTF8<char>, UTF8<char> > reader;
  StringStream ss(str);
  reader.Parse<0, StringStream, JSObjectReaderHandler>(ss, handler);

  return handler.GetResult();
}

} // namespace jsobjects
