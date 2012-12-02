#include <gtest/gtest.h>
#include <iostream>
#include <jsobjects_cpp.hpp>

class JSObjectCppFixture: public testing::Test { 

public: 

  JSObjectCppFixture()
  {
  } 

  ~JSObjectCppFixture( )  { 
  }

  void SetUp() { 
  }

  void TearDown( ) { 
  }
};

TEST_F(JSObjectCppFixture, Create_Bool)
{
  JSContextCpp context;
  JSValuePtr jsval = context.newBoolean(true);
  EXPECT_EQ(JSValue::Boolean, jsval->getType());
  EXPECT_EQ(true, jsval->asBool());
}

TEST_F(JSObjectCppFixture, Create_Number)
{
  JSContextCpp context;
  JSValuePtr jsval = context.newNumber(2.0);
  EXPECT_EQ(JSValue::Number, jsval->getType());
  EXPECT_EQ(2.0, jsval->asDouble());
}

TEST_F(JSObjectCppFixture, Create_String)
{
  JSContextCpp context;
  JSValuePtr jsstr = context.newString("bla");
  EXPECT_EQ(JSValue::String, jsstr->getType());
  EXPECT_STREQ("bla", jsstr->asString().c_str());
}

TEST_F(JSObjectCppFixture, Create_Simple_Object)
{
  JSContextCpp context;
  JSObjectPtr obj = context.newObject();
  JSObjectPtr obj2 = context.newObject();
  obj->set("a", "bla");
  obj->set("b", 2.0);

  EXPECT_EQ(JSValue::String, obj->get("a")->getType());
  EXPECT_EQ(JSValue::Number, obj->get("b")->getType());
}

TEST_F(JSObjectCppFixture, Create_Nested_Object)
{
  JSContextCpp context;
  JSObjectPtr obj = context.newObject();
  JSObjectPtr obj2 = context.newObject();
  obj2->set("foo", 1.0);
  obj->set("a", obj2);
  obj->set("b", 2.0);

  EXPECT_EQ(JSValue::Object, obj->get("a")->getType());
}

TEST_F(JSObjectCppFixture, Create_Simple_Array)
{
  JSContextCpp context;
  JSArrayPtr arr = context.newArray(2);
  arr->setAt(0, "bla");
  arr->setAt(1, 2.0);

  EXPECT_EQ(JSValue::String, arr->getAt(0)->getType());
  EXPECT_EQ(JSValue::Number, arr->getAt(1)->getType());
}

TEST_F(JSObjectCppFixture, Create_Nested_Array)
{
  JSContextCpp context;
  JSArrayPtr arr1 = context.newArray(1);
  JSArrayPtr arr2 = context.newArray(2);
  arr2->setAt(0, "bla");
  arr2->setAt(1, 2.0);
  arr1->setAt(0, arr2);

  JSValuePtr _val = arr1->getAt(0);
  EXPECT_EQ(JSValue::Array, _val->getType());
  JSArrayPtr _arr2 = JSValue::asArray(_val);
  
  EXPECT_EQ(JSValue::String, _arr2->getAt(0)->getType());
  EXPECT_EQ(JSValue::Number, _arr2->getAt(1)->getType());
}

TEST_F(JSObjectCppFixture, Serialize_Simple_Object)
{
  JSContextCpp context;
  JSObjectPtr obj = context.newObject();
  obj->set("a", "bla");
  obj->set("b", 2.0);
  obj->set("c", false);

  const std::string &json = context.toJson(JSValue::asValue(obj));
  EXPECT_STREQ("{\"a\":\"bla\",\"b\":2,\"c\":false}", json.c_str());
}

TEST_F(JSObjectCppFixture, Serialize_Simple_Array)
{
  JSContextCpp context;
  JSArrayPtr array = context.newArray(3);
  array->setAt(0, "bla");
  array->setAt(1, 2.0);
  array->setAt(2, false);

  const std::string &json = context.toJson(JSValue::asValue(array));
  EXPECT_STREQ("[\"bla\",2,false]", json.c_str());
}


TEST_F(JSObjectCppFixture, Serialize_Nested_Object)
{
  JSContextCpp context;
  JSObjectPtr obj = context.newObject();
  JSObjectPtr obj2 = context.newObject();
  // NOTE: obj items are always sorted by their keys
  obj2->set("bar", 1.0);
  obj2->set("foo", 2.0);
  
  obj->set("a", obj2);
  obj->set("b", 2.0);
  obj->set("c", false);

  const std::string &json = context.toJson(JSValue::asValue(obj));
  EXPECT_STREQ("{\"a\":{\"bar\":1,\"foo\":2},\"b\":2,\"c\":false}", json.c_str());
}

TEST_F(JSObjectCppFixture, Serialize_Nested_Array)
{
  JSContextCpp context;
  JSObjectPtr obj = context.newObject();
  JSArrayPtr arr = context.newArray(2);

  arr->setAt(0, 1.0);
  arr->setAt(1, 2.0);
  
  obj->set("a", arr);

  const std::string &json = context.toJson(JSValue::asValue(obj));
  EXPECT_STREQ("{\"a\":[1,2]}", json.c_str());
}

TEST_F(JSObjectCppFixture, Deserialize_Simple_Object)
{
  JSContextCpp context;
  const char* data = "{\"a\":\"bla\",\"b\":2,\"c\":false}";
  JSValuePtr val = context.fromJson(data);
  ASSERT_TRUE(val->isObject());  
  
  JSObjectPtr obj = JSValue::asObject(val);
  JSValuePtr val_a = obj->get("a");
  JSValuePtr val_b = obj->get("b");
  JSValuePtr val_c = obj->get("c");

  ASSERT_TRUE(val_a->isString());
  ASSERT_TRUE(val_b->isNumber());
  ASSERT_TRUE(val_c->isBoolean());
  
  EXPECT_STREQ("bla", val_a->asString().c_str());
  EXPECT_EQ(2.0, val_b->asDouble());
  EXPECT_EQ(0, val_c->asBool());
}

TEST_F(JSObjectCppFixture, Deserialize_Simple_Array)
{
  JSContextCpp context;
  const char* data = "[\"bla\",2,false]";
  JSValuePtr val = context.fromJson(data);
  ASSERT_TRUE(val->isArray());  
  
  JSArrayPtr arr = JSValue::asArray(val);
  JSValuePtr val_a = arr->getAt(0);
  JSValuePtr val_b = arr->getAt(1);
  JSValuePtr val_c = arr->getAt(2);

  ASSERT_TRUE(val_a->isString());
  ASSERT_TRUE(val_b->isNumber());
  ASSERT_TRUE(val_c->isBoolean());
  
  EXPECT_STREQ("bla", val_a->asString().c_str());
  EXPECT_EQ(2.0, val_b->asDouble());
  EXPECT_EQ(0, val_c->asBool());
}

TEST_F(JSObjectCppFixture, Deserialize_Nested_Object)
{
  JSContextCpp context;
  const char* data = "{\"a\":{\"bar\":1,\"foo\":2}}";
  JSValuePtr val = context.fromJson(data);
  ASSERT_TRUE(val->isObject());  
  
  JSObjectPtr obj = JSValue::asObject(val);
  JSValuePtr val_a = obj->get("a");

  ASSERT_TRUE(val_a->isObject());
  JSObjectPtr obj2 = JSValue::asObject(val_a);

  JSValuePtr val_1 = obj2->get("bar");
  JSValuePtr val_2 = obj2->get("foo");

  ASSERT_TRUE(val_1->isNumber());
  ASSERT_TRUE(val_2->isNumber());
}

TEST_F(JSObjectCppFixture, Deserialize_Nested_Array)
{
  JSContextCpp context;
  const char* data = "[[1,2]]";
  JSValuePtr val = context.fromJson(data);
  ASSERT_TRUE(val->isArray());  
  JSArrayPtr arr1 = JSValue::asArray(val);
  
  val = arr1->getAt(0);
  ASSERT_TRUE(val->isArray());
  JSArrayPtr arr2 = JSValue::asArray(val);

  JSValuePtr val_1 = arr2->getAt(0);
  JSValuePtr val_2 = arr2->getAt(1);

  ASSERT_TRUE(val_1->isNumber());
  ASSERT_TRUE(val_2->isNumber());
}
