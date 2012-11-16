#include <iostream>
#include <js_shell.h>
#include <example_info.h>

int main(int argc, char* argv[]) {

  int result = 0;

  JSShell* shell = JSShell::Create();
  bool success = shell->ImportModule(MODULE_NAME);

  if (!success) {
    std::cerr << "FAIL: module could not be loaded." << std::endl;
    result = -1;
  } else {
    success = shell->RunScript(JS_SCRIPT_PATH);
    if (!success) {
      std::cerr << "FAIL: Error during execution of script." << std::endl;
    }
  }

  delete shell;
  return result;
}
