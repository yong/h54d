#include "fs.h"
#include "include/cef.h"
#include "include/cef_runnable.h"

#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>

CefRefPtr<CefV8Value> fs_open(const CefV8ValueList& arguments)
{
	if (arguments.size() != 2)
		return CefV8Value::CreateNull();

	const CefString& filename = arguments[0]->GetStringValue();
	int mode = _O_RDWR;

	int fd = _wopen(filename.c_str(), mode);

	return CefV8Value::CreateInt(fd);
}

CefRefPtr<CefV8Value> fs_close(const CefV8ValueList& arguments)
{
	if (arguments.size() != 1)
		return CefV8Value::CreateNull();

	int fd = arguments[0]->GetIntValue();

	int e = _close(fd);

	return CefV8Value::CreateInt(e);
}

CefRefPtr<CefV8Value> fs_readFile(const CefV8ValueList& arguments)
{
	if (arguments.size() != 2)
		return CefV8Value::CreateNull();
	
	if (!(arguments[0]->IsString())) {
		return CefV8Value::CreateNull();
	}

	if (!(arguments[1]->IsFunction())) {
		return CefV8Value::CreateNull();
	}

	CefString path = arguments[0]->GetStringValue();
	const wchar_t* path_s = path.c_str();
	FILE *f = _wfopen(path_s, L"r" );
	if (NULL == f) {
		return CefV8Value::CreateNull(); 
	}

	std::stringstream str;
	char buff[1024] = {0};
	int n;
	while ((n = fread(buff, 1, sizeof(buff) - 1, f)) > 0) {
		buff[n] = '\0';
		str << buff;
	}
	fclose(f);
	
	CefRefPtr<CefV8Value> callback_func = arguments[1];
	CefRefPtr<CefV8Value> retval;
    CefRefPtr<CefV8Exception> exception;
	CefV8ValueList args;
	args.push_back(CefV8Value::CreateNull());
	args.push_back(CefV8Value::CreateString(str.str()));
	callback_func->ExecuteFunctionWithContext(CefV8Context::GetCurrentContext(), NULL, args, retval, exception, true);
	
	return CefV8Value::CreateNull();
}

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
    if (name == "__fs_open") {
      retval = fs_open(arguments);
      return true;
    } else if (name == "__fs_close") {
      retval = fs_close(arguments);
      return true;
    } else if (name == "__fs_readFile") {
      retval = fs_readFile(arguments);
      return true;
    }

    // Function does not exist.
    return false;
  }

  // Provide the reference counting implementation for this class.
  IMPLEMENT_REFCOUNTING(MyV8Handler);
};

void FS::Initialize()
{
	// Define the extension contents.
	std::string extensionCode =
	  "var fs;"
	  "if (!fs)"
	  "  fs = {};"
	  "(function() {"
	  "  fs.open = function(filename, mode) {"
	  "    native function __fs_open(filename, mode);"
	  "    return __fs_open(filename, mode);"
	  "  };"
	  "  fs.close = function(fd) {"
	  "    native function __fs_close(fd);"
	  "    return __fs_close(fd);"
	  "  };"
	  "  fs.readFile = function(filename, callback) {"
	  "    native function __fs_readFile(filename, callback);"
	  "    return __fs_readFile(filename, callback);"
	  "  };"
	  "})();";

	// Create an instance of my CefV8Handler object.
	CefRefPtr<CefV8Handler> handler = new MyV8Handler();

	// Register the extension.
	CefRegisterExtension("v8/fs", extensionCode, handler);
}