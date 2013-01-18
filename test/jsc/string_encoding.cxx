#include <JavaScriptCore/JavaScript.h>
#include <jsobjects_jsc.hpp>
#include <iostream>

using namespace jsobjects;

bool simple_ascii(JSContextJSC& context) {
	std::cout << "    -- Test:  simple_ascii... ";
	const char* str = "Hello";
	JSValuePtr val = context.newString(str);

	const char* act = val->asString().c_str();

	if (strcmp(str, act) != 0) goto fail;

	std::cout << "ok." << std::endl;
	return true;

	fail:
	std::cout << "failed." << std::endl;
	return false;
}


bool unicode(JSContextJSC& context) {
	std::cout << "    -- Test:  unicode... ";
	const char* str = "olé";
	JSValuePtr val = context.newString(str);

	const char* act = val->asString().c_str();

	if (strcmp(str, act) != 0) goto fail;

	std::cout << "ok." << std::endl;
	return true;

	fail:
	std::cout << "failed." << std::endl;
	return false;
}

int main() {

	int err = 0;

	JSGlobalContextRef context = JSGlobalContextCreate(NULL);
	JSContextJSC jscontext(context);


	if(!simple_ascii(jscontext)) err = 1;
	if(!unicode(jscontext)) err = 1;

  	JSGlobalContextRelease(context);

  	return err;
}