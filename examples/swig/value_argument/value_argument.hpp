#include <jsobjects.hpp>
#include <iostream>

void acceptValue(JSValuePtr val) {
  switch (val->getType()) {
    case JSValue::String:
      std::cout << "Received String:" << val->asString() << std::endl;
      break;
    case JSValue::Boolean:
      std::cout << "Received Bool:" << val->asBool() << std::endl;
      break;
    case JSValue::Number:
      std::cout << "Received Number:" << val->asDouble() << std::endl;
      break;
    case JSValue::Object:
      std::cout << "Received Object." << std::endl;
      break;
    case JSValue::Array:
      std::cout << "Received Array." << std::endl;
      break;
    case JSValue::Undefined:
      std::cout << "Received undefined." << std::endl;
      break;
    case JSValue::Null:
      std::cout << "Received null." << std::endl;
      break;
    default:
      std::cout << "Received unknown jsvalue type." << std::endl;
      break;
  }
}
