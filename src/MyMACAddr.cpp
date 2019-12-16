/*
* Copyright (C) 2016 SaEeD
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
*
*/
#include "MyMACAddr.h"

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

MyMACAddr::MyMACAddr()
{
    //Seeding for random numbers

    //srand((unsigned) time(nullptr));
}

MyMACAddr::~MyMACAddr()
{
}

//-----------------------------------------------
// Generate Random MAC Addresses
//-----------------------------------------------
string MyMACAddr::genRandMAC()
{
    std::uniform_int_distribution<int> distribution(0, 254);
    stringstream temp;
    int number = 0;
    string result;

    for (int i = 0; i < 6; i++) {
        number = distribution(rd); //rand() % 254;
        temp << setfill('0') << setw(2) << hex << number;
    }
    result = temp.str();

    for (auto &c : result) {
        c = toupper(c);
    }

    return result;
}

string MyMACAddr::genRandMAC(int first)
{
    std::uniform_int_distribution<int> distribution(0, 254);
    stringstream temp;
    int number = 0;
    string result;
    temp << setfill('0') << setw(2) << hex << first;

    for (int i = 1; i < 6; i++) {
        number = distribution(rd); //rand() % 254;
        temp << setfill('0') << setw(2) << hex << number;
    }
    result = temp.str();

    for (auto &c : result) {
        c = toupper(c);
    }

    return result;
}

//-----------------------------------------------
// Original Code from : https://msdn.microsoft.com/en-us/library/windows/desktop/aa366062(v=vs.85).aspx
// Print the details of Network Interfaces  
//-----------------------------------------------

void MyMACAddr::showAdapterList()
{
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = nullptr;
    DWORD dwRetVal = 0;
    UINT i;

    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(sizeof(IP_ADAPTER_INFO));
    if (pAdapterInfo == nullptr) {
        //cerr << "Error allocating memory needed to call GetAdaptersinfo." << endl;
    }
    // Make an initial call to GetAdaptersInfo to get
    // the necessary size into the ulOutBufLen variable
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        FREE(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(ulOutBufLen);
        if (pAdapterInfo == nullptr) {
            //cerr << "Error allocating memory needed to call GetAdaptersinfo" << endl;
        }
    }

    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
        pAdapter = pAdapterInfo;
		while (pAdapter) {
			cout<<"\n\tComboIndex: \t" << pAdapter->ComboIndex << endl;
			cout << "\tAdapter Name: \t" << pAdapter->AdapterName << endl;
			cout << "\tAdapter Desc: \t" << pAdapter->Description <<endl;
			cout << "\tAdapter Addr: \t";
			for (i = 0; i < pAdapter->AddressLength; i++) {
				if (i == (pAdapter->AddressLength - 1))
					printf("%.2X\n", (int)pAdapter->Address[i]);
				else
					printf("%.2X-", (int)pAdapter->Address[i]);
			}
			cout << "\tIP Address: \t" << pAdapter->IpAddressList.IpAddress.String << endl;
			cout << "\tIP Mask: \t" << pAdapter->IpAddressList.IpMask.String << endl;
			cout << "\tGateway: \t" <<pAdapter->GatewayList.IpAddress.String << endl;
			pAdapter = pAdapter->Next;			
		}
    } else {
        //cerr << "GetAdaptersInfo failed with error: " <<  dwRetVal<< endl;
    }
    if (pAdapterInfo)
		FREE(pAdapterInfo);
}

//-----------------------------------------------
// Get Network Adapter's Name and MAC addresses
//-----------------------------------------------
unordered_map<string, string> MyMACAddr::getAdapters()
{
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = nullptr;
    DWORD dwRetVal = 0;

    unordered_map<string, string> result;
    stringstream temp;
	string str_mac;

    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(sizeof(IP_ADAPTER_INFO));
    if (pAdapterInfo == nullptr) {
        //cerr << "Error allocating memory needed to call GetAdaptersinfo" << endl;
    }
    // Make an initial call to GetAdaptersInfo to get
    // the necessary size into the ulOutBufLen variable
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        FREE(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(ulOutBufLen);
        if (pAdapterInfo == nullptr) {
            //cerr << "Error allocating memory needed to call GetAdaptersinfo\n" << endl;
        }
    }

    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
        pAdapter = pAdapterInfo;
        while (pAdapter) {
            for (UINT i = 0; i < pAdapter->AddressLength; i++) {
                temp << setfill('0') << setw(2) << hex << (int) pAdapter->Address[i];
                if (i != pAdapter->AddressLength - 1) {
                    temp << "-";
                }
            }
            str_mac = temp.str();
            temp.str("");
            temp.rdbuf();
            for (auto &c : str_mac) {
                c = toupper(c);
            }

            result.insert({pAdapter->Description, str_mac});
            pAdapter = pAdapter->Next;
        }
    } else {
        //cerr << "GetAdaptersInfo failed with error: " << dwRetVal << endl;
    }
    if (pAdapterInfo)
		FREE(pAdapterInfo);	
	
	return result;
}

string MyMACAddr::getAdaptersOrigAddr(string pAdapterName)
{
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = nullptr;
    DWORD dwRetVal = 0;

    stringstream temp;
    string str_mac;

    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(sizeof(IP_ADAPTER_INFO));
    if (pAdapterInfo == nullptr) {
        //cerr << "Error allocating memory needed to call GetAdaptersinfo" << endl;
    }
    // Make an initial call to GetAdaptersInfo to get
    // the necessary size into the ulOutBufLen variable
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        FREE(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(ulOutBufLen);
        if (pAdapterInfo == nullptr) {
            //cerr << "Error allocating memory needed to call GetAdaptersinfo\n" << endl;
        }
    }

    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
        pAdapter = pAdapterInfo;
        while (pAdapter) {
            if (pAdapter->Description == pAdapterName) {
                for (UINT i = 0; i < pAdapter->AddressLength; i++) {
                    temp << setfill('0') << setw(2) << hex << (int) pAdapter->Address[i];
                }
                str_mac = temp.str();
                temp.str("");
                temp.rdbuf();
                for (auto &c : str_mac) {
                    c = toupper(c);
                }
                break;
            } else {
                pAdapter = pAdapter->Next;
            }
        }
    } else {
        //cerr << "GetAdaptersInfo failed with error: " << dwRetVal << endl;
    }
    if (pAdapterInfo)
        FREE(pAdapterInfo);

    return str_mac;
}

//-----------------------------------------------
// Assing Random MAC address to Network Interface
//-----------------------------------------------
void MyMACAddr::assignRndMAC(vector<string> *list)
{
    //unordered_map<string, string> result;
    wstring *wstr;
    const wchar_t *wAdapterName;
    bool bRet; // = false;
    HKEY hKey; // = nullptr;

    for (auto &item : *list) {
        //-------- Converting to Wide characters
        wstr = new wstring(item.begin(), item.end());
        wAdapterName = wstr->c_str();
        //-------- Registry Key for Network Interfaces
        hKey = nullptr;
        if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                         _T("SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-")
                         _T("08002bE10318}"),
                         0,
                         KEY_ALL_ACCESS,
                         &hKey)
            == ERROR_SUCCESS) {
            DWORD dwIndex = 0;
            TCHAR Name[1024];
            DWORD cName = 1024;
            while (RegEnumKeyEx(hKey, dwIndex, Name, &cName, nullptr, nullptr, nullptr, nullptr)
                   == ERROR_SUCCESS) {
                HKEY hSubKey = nullptr;
                if (RegOpenKeyEx(hKey, Name, 0, KEY_ALL_ACCESS, &hSubKey) == ERROR_SUCCESS) {
                    BYTE Data[1204];
                    DWORD cbData = 1024;
                    if (RegQueryValueEx(hSubKey, _T("DriverDesc"), nullptr, nullptr, Data, &cbData)
                        == ERROR_SUCCESS) {
                        if (_tcscmp((TCHAR *) Data, wAdapterName) == 0) {
                            string temp = genRandMAC();
                            uint8_t tried = 0;
                        attempt:
                            //temp.erase(std::remove(temp.begin(), temp.end(), '-'), temp.end());
                            wstring wstr_newMAC(temp.begin(), temp.end());
                            const wchar_t *newMACAddr = wstr_newMAC.c_str();

                            //--------Add new MAC to Registry Subkey and disable and re-enable the interface
                            // to effect changes
                            if (RegSetValueEx(hSubKey,
                                              _T("NetworkAddress"),
                                              0,
                                              REG_SZ,
                                              (const BYTE *) newMACAddr,
                                              sizeof(TCHAR) * ((DWORD) _tcslen(newMACAddr) + 1))
                                == ERROR_SUCCESS) {
                                //result.insert({item, temp});
                                disableEnableConnections(false, wAdapterName);
                                disableEnableConnections(true, wAdapterName);
                            }
                        }
                    }
                    RegCloseKey(hSubKey);
                }
                cName = 1024;
                dwIndex++;
            }
            RegCloseKey(hKey);
        }
    }
}

void MyMACAddr::assignOrigMAC(string name, string temp)
{
    //unordered_map<string, string> result;
    wstring *wstr;
    const wchar_t *wAdapterName;
    bool bRet; // = false;
    HKEY hKey; // = nullptr;

    //-------- Converting to Wide characters
    wstr = new wstring(name.begin(), name.end());
    wAdapterName = wstr->c_str();
    //-------- Registry Key for Network Interfaces
    hKey = nullptr;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                     _T("SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-")
                     _T("08002bE10318}"),
                     0,
                     KEY_ALL_ACCESS,
                     &hKey)
        == ERROR_SUCCESS) {
        DWORD dwIndex = 0;
        TCHAR Name[1024];
        DWORD cName = 1024;
        while (RegEnumKeyEx(hKey, dwIndex, Name, &cName, nullptr, nullptr, nullptr, nullptr)
               == ERROR_SUCCESS) {
            HKEY hSubKey = nullptr;
            if (RegOpenKeyEx(hKey, Name, 0, KEY_ALL_ACCESS, &hSubKey) == ERROR_SUCCESS) {
                BYTE Data[1204];
                DWORD cbData = 1024;
                if (RegQueryValueEx(hSubKey, _T("DriverDesc"), nullptr, nullptr, Data, &cbData)
                    == ERROR_SUCCESS) {
                    if (_tcscmp((TCHAR *) Data, wAdapterName) == 0) {
                        //temp.erase(std::remove(temp.begin(), temp.end(), '-'), temp.end());
                        wstring wstr_newMAC(temp.begin(), temp.end());
                        const wchar_t *newMACAddr = wstr_newMAC.c_str();

                        //--------Add new MAC to Registry Subkey and disable and re-enable the interface
                        // to effect changes
                        if (RegSetValueEx(hSubKey,
                                          _T("NetworkAddress"),
                                          0,
                                          REG_SZ,
                                          (const BYTE *) newMACAddr,
                                          sizeof(TCHAR) * ((DWORD) _tcslen(newMACAddr) + 1))
                            == ERROR_SUCCESS) {
                            //result.insert({item, temp});
                            disableEnableConnections(false, wAdapterName);
                            disableEnableConnections(true, wAdapterName);
                        }
                    }
                }
                RegCloseKey(hSubKey);
            }
            cName = 1024;
            dwIndex++;
        }
        RegCloseKey(hKey);
    }
}

//-----------------------------------------------
// Original Code: https://social.msdn.microsoft.com/Forums/en-US/ad3ae21d-515d-4f67-8519-216f1058e390/enabledisable-network-card?forum=netfxnetcom
// Modified to only Disable and Re-enable the given Adapter
//-----------------------------------------------
HRESULT MyMACAddr::disableEnableConnections(BOOL bEnable, const wchar_t *AdapterName)
{
    HRESULT hr = E_FAIL;

    CoInitialize(nullptr);

    INetConnectionManager *pNetConnectionManager = nullptr;
    hr = CoCreateInstance(CLSID_ConnectionManager,
                          nullptr,
                          CLSCTX_LOCAL_SERVER | CLSCTX_NO_CODE_DOWNLOAD,
                          IID_INetConnectionManager,
                          reinterpret_cast<LPVOID *>(&pNetConnectionManager));
    if (SUCCEEDED(hr)) {
        /*
		Get an enumurator for the set of connections on the system
		*/
        IEnumNetConnection* pEnumNetConnection;
		pNetConnectionManager->EnumConnections(NCME_DEFAULT, &pEnumNetConnection);

		ULONG ulCount = 0;
		BOOL fFound = FALSE;
		hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);

		HRESULT hrT = S_OK;

        /*
		Enumurate through the list of adapters on the system and look for the one we want
		NOTE: To include per-user RAS connections in the list, you need to set the COM
		Proxy Blanket on all the interfaces. This is not needed for All-user RAS
		connections or LAN connections.
		*/
        do {
            NETCON_PROPERTIES *pProps = nullptr;
            INetConnection *pConn;

            /*
			Find the next (or first connection)
			*/
            hrT = pEnumNetConnection->Next(1, &pConn, &ulCount);

            if (SUCCEEDED(hrT) && 1 == ulCount) {
                /*
				Get the connection properties
				*/
                hrT = pConn->GetProperties(&pProps);

                if (S_OK == hrT) {
                    //printf("* %S\n", pProps->pszwName);

                    if (bEnable && (_tcscmp((TCHAR *) pProps->pszwDeviceName, AdapterName) == 0)) {
                        //printf("      Enabling adapter: %S...\n", pProps->pszwName);
                        //printf("[+]Enabling adapter: %S...\n", pProps->pszwDeviceName);
                        hr = pConn->Connect();
                    } else if (_tcscmp((TCHAR *) pProps->pszwDeviceName, AdapterName) == 0) {
                        //printf("[+]Disabling adapter: %S...\n", pProps->pszwDeviceName);
                        hr = pConn->Disconnect();
                    }

                    CoTaskMemFree(pProps->pszwName);
                    CoTaskMemFree(pProps->pszwDeviceName);
                    CoTaskMemFree(pProps);
                }

                pConn->Release();
                pConn = nullptr;
            }

        } while (SUCCEEDED(hrT) && 1 == ulCount && !fFound);

        if (FAILED(hrT)) {
            hr = hrT;
        }

        pEnumNetConnection->Release();
    }

    if (FAILED(hr) && hr != HRESULT_FROM_WIN32(ERROR_RETRY)) {
        //printf("Could not enable or disable connection (0x%08lx)\r\n", hr);
    }

    pNetConnectionManager->Release();
	CoUninitialize();

	return hr;
}
