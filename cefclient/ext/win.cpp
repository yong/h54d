#include "include/cef.h"
#include "include/cef_runnable.h"

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>

CefRefPtr<CefV8Value> getNetworkInterface()
{
	return CefV8Value::CreateNull();
}

/*
CefRefPtr<CefV8Value> getNetworkInterface()
{
	SOCKET sd = WSASocket(AF_INET, SOCK_DGRAM, 0, 0, 0, 0);
    if (sd == SOCKET_ERROR) {
        return CefV8Value::CreateNull();
    }

    INTERFACE_INFO InterfaceList[20];
    unsigned long nBytesReturned;
    if (WSAIoctl(sd, SIO_GET_INTERFACE_LIST, 0, 0, &InterfaceList,
			sizeof(InterfaceList), &nBytesReturned, 0, 0) == SOCKET_ERROR) {
        closesocket(sd);
		return CefV8Value::CreateNull();
    }

	CefRefPtr<CefV8Value> arr = CefV8Value::CreateArray();

    int nNumInterfaces = nBytesReturned / sizeof(INTERFACE_INFO);
    for (int i = 0; i < nNumInterfaces; ++i) {
        CefRefPtr<CefV8Value> v = CefV8Value::CreateObject(NULL);
		arr->SetValue(i, v);
		
		v->SetValue("name", CefV8Value::CreateString(pCurrAddresses->FriendlyName), V8_PROPERTY_ATTRIBUTE_NONE);

        sockaddr_in *pAddress;
        pAddress = (sockaddr_in *) & (InterfaceList[i].iiAddress);
		v->SetValue("ip", CefV8Value::CreateString(inet_ntoa(pAddress->sin_addr)), V8_PROPERTY_ATTRIBUTE_NONE);

        pAddress = (sockaddr_in *) & (InterfaceList[i].iiBroadcastAddress);
		v->SetValue("broadcast", CefV8Value::CreateString(inet_ntoa(pAddress->sin_addr)), V8_PROPERTY_ATTRIBUTE_NONE);

        pAddress = (sockaddr_in *) & (InterfaceList[i].iiNetmask);
		v->SetValue("netmask", CefV8Value::CreateString(inet_ntoa(pAddress->sin_addr)), V8_PROPERTY_ATTRIBUTE_NONE);
        

        cout << " Iface is ";
        u_long nFlags = InterfaceList[i].iiFlags;
        if (nFlags & IFF_UP) cout << "up";
        else                 cout << "down";
        if (nFlags & IFF_POINTTOPOINT) cout << ", is point-to-point";
        if (nFlags & IFF_LOOPBACK)     cout << ", is a loopback iface";
        cout << ", and can do: ";
        if (nFlags & IFF_BROADCAST) cout << "bcast ";
        if (nFlags & IFF_MULTICAST) cout << "multicast ";
        cout << endl;
    }

    return arr;
}


CefRefPtr<CefV8Value> getNetworkInterface()
{
	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	DWORD dwRetVal = 0;
    ULONG Iterations = 0; 
    ULONG outBufLen = WORKING_BUFFER_SIZE; // Allocate a 15 KB buffer to start with.
    do {
        pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);
        if (pAddresses == NULL) {
            return CefV8Value::CreateNull();
        }

        dwRetVal = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, pAddresses, &outBufLen);
        if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
            FREE(pAddresses);
            pAddresses = NULL;
        } else {
            break;
        }

        Iterations++;
    } while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

    if (dwRetVal != NO_ERROR) {
		if (pAddresses)
			FREE(pAddresses);
        return CefV8Value::CreateNull();
	}

	CefRefPtr<CefV8Value> arr = CefV8Value::CreateArray();
	int index = 0;
	PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses;
	while (pCurrAddresses) {
		CefRefPtr<CefV8Value> v = CefV8Value::CreateObject(NULL);
		
		v->SetValue("name", CefV8Value::CreateString(pCurrAddresses->FriendlyName), V8_PROPERTY_ATTRIBUTE_NONE);

		PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pCurrAddresses->FirstUnicastAddress;
		if (pUnicast) {
			char buff[100] = {0};
			if (pUnicast->Address.lpSockaddr->sa_family == AF_INET) {
				sockaddr_in *sa_in = (sockaddr_in *)pUnicast->Address.lpSockaddr;
				v->SetValue("ip", CefV8Value::CreateString(inet_ntop(AF_INET,&(sa_in->sin_addr),buff,sizeof(buff) - 1)), V8_PROPERTY_ATTRIBUTE_NONE);
			}
			else if (pUnicast->Address.lpSockaddr->sa_family == AF_INET6)
			{
				sockaddr_in6 *sa_in6 = (sockaddr_in6 *)pUnicast->Address.lpSockaddr;
				v->SetValue("ip", CefV8Value::CreateString(inet_ntop(AF_INET6,&(sa_in6->sin6_addr),buff,sizeof(buff) - 1)), V8_PROPERTY_ATTRIBUTE_NONE);
			}
			v->SetValue("netmask", CefV8Value::CreateString("up"), V8_PROPERTY_ATTRIBUTE_NONE);
		}

		PIP_ADAPTER_GATEWAY_ADDRESS_LH pGateway = pCurrAddresses->FirstGatewayAddress;
		if (pGateway) {
			v->SetValue("broadcast", CefV8Value::CreateString(inet_ntoa(*(struct in_addr*)pGateway->Address.lpSockaddr)), V8_PROPERTY_ATTRIBUTE_NONE);
		}

		switch (pCurrAddresses->OperStatus) {
		case IfOperStatusUp:
			v->SetValue("status", CefV8Value::CreateString("up"), V8_PROPERTY_ATTRIBUTE_NONE);
			break;
		case IfOperStatusDown:
			v->SetValue("status", CefV8Value::CreateString("down"), V8_PROPERTY_ATTRIBUTE_NONE);
			break;
		default:
			v->SetValue("status", CefV8Value::CreateString("other"), V8_PROPERTY_ATTRIBUTE_NONE);
		}
		
		pCurrAddresses = pCurrAddresses->Next;
		arr->SetValue(index++, v);
	}           
    
    if (pAddresses) {
        FREE(pAddresses);
    }

    return arr;
}

*/

#endif //win32