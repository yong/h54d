#include "os.h"
#include "include/cef.h"
#include "include/cef_runnable.h"

#ifndef WIN32
#undef OVERRIDE
#define OVERRIDE
#endif

//extern CefRefPtr<CefV8Value> getNetworkInterface();

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
    /*if (name == "networkInterfaces") {
      // Return my string value.
      retval = getNetworkInterface();
      return true;
    }*/

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
	  "var os;"
	  "if (!os)"
	  "  os = {};"
	  "(function() {"
#ifdef WIN32
	  "  os.type = function() {return 'windows'};"
#elif __APPLE__
	  "  os.type = function() {return 'mac'};"
#endif
	  "  os.networkInterfaces = function() {"
	  "    native function networkInterfaces();"
	  "    return networkInterfaces();"
	  "  };"
	  "})();";

	// Create an instance of my CefV8Handler object.
	CefRefPtr<CefV8Handler> handler = new MyV8Handler();

	// Register the extension.
	CefRegisterExtension("v8/os", extensionCode, handler);
}
