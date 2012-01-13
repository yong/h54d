#include "os.h"
#include "include/cef.h"
#include "include/cef_runnable.h"

class MyV8Handler : public CefV8Handler
{
public:
  MyV8Handler() {}

  virtual bool Execute(const CefString& name,
                       CefRefPtr<CefV8Value> object,
                       const CefV8ValueList& arguments,
                       CefRefPtr<CefV8Value>& retval,
                       CefString& exception) OVERRIDE
  {
    if (name == "myfunc") {
      // Return my string value.
      retval = CefV8Value::CreateString("My Value!");
      return true;
    }

    // Function does not exist.
    return false;
  }

  // Provide the reference counting implementation for this class.
  IMPLEMENT_REFCOUNTING(MyV8Handler);
};

void OS::Initialize()
{
	// Define the extension contents.
	std::string extensionCode =
	  "var test;"
	  "if (!test)"
	  "  test = {};"
	  "(function() {"
	  "  test.myfunc = function() {"
	  "    native function myfunc();"
	  "    return myfunc();"
	  "  };"
	  "})();";

	// Create an instance of my CefV8Handler object.
	CefRefPtr<CefV8Handler> handler = new MyV8Handler();

	// Register the extension.
	CefRegisterExtension("v8/test", extensionCode, handler);
}