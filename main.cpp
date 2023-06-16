#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#define _WIN32_WINNT_NT4						0x0400
#define _WIN32_WINNT_WIN2K						0x0500
#define _WIN32_WINNT_WINXP						0x0501
#define _WIN32_WINNT_WS03						0x0502
#define _WIN32_WINNT_WIN6						0x0600
#define _WIN32_WINNT_VISTA						0x0600
#define _WIN32_WINNT_WS08						0x0600
#define _WIN32_WINNT_LONGHORN					0x0600
#define _WIN32_WINNT_WIN7						0x0601
#define _WIN32_WINNT_WIN8						0x0602
#define _WIN32_WINNT_WIN81						0x0603
#ifndef _WIN32_WINNT_WIN10
#define _WIN32_WINNT_WIN10						0x1000
#endif // !_WIN32_WINNT_WIN10
#define _WIN32_WINNT_WIN10_GRATER				0x1001


bool GetNtVersionNumbers(DWORD& dwMajorVer, DWORD& dwMinorVer, DWORD& dwBuildNumber)
{
    bool bRet = false;
    HMODULE hModNtdll = NULL;
    if (hModNtdll = ::LoadLibrary("ntdll.dll"))
    {
        typedef void (WINAPI* pfRTLGETNTVERSIONNUMBERS)(DWORD*, DWORD*, DWORD*);
        pfRTLGETNTVERSIONNUMBERS pfRtlGetNtVersionNumbers;
        pfRtlGetNtVersionNumbers = (pfRTLGETNTVERSIONNUMBERS)::GetProcAddress(hModNtdll, "RtlGetNtVersionNumbers");
        if (pfRtlGetNtVersionNumbers)
        {
            pfRtlGetNtVersionNumbers(&dwMajorVer, &dwMinorVer, &dwBuildNumber);
            dwBuildNumber &= 0x0ffff;
            bRet = TRUE;
        }

        ::FreeLibrary(hModNtdll);
        hModNtdll = NULL;
    }
    return bRet;
}

typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
LPFN_ISWOW64PROCESS fnIsWow64Process;
bool IsWow64()
{
#ifdef _WIN64
    return true;
#else
    BOOL is_wow64 = false;
		fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
		if (NULL != fnIsWow64Process) {
			if (!fnIsWow64Process(GetCurrentProcess(), &is_wow64))
				return false;
		}

		return is_wow64 ? true : false;
#endif // _WIN64
}
#endif
std::string GetOSVersionString()
{
#ifdef _WIN32
    int nVersion = _WIN32_WINNT_WIN2K;
    DWORD dwMajorVer = 5;
    DWORD dwMinorVer = 0;
    DWORD dwBuildNumber = 0;
    bool bRet = GetNtVersionNumbers(dwMajorVer, dwMinorVer, dwBuildNumber);
    if (bRet) {
        nVersion = dwMajorVer * 256 + dwMinorVer;
    }
    printf("os version: %d, %d, %d......\r\n", dwMajorVer, dwMinorVer, dwBuildNumber);

    std::string strVersion;
    switch (nVersion)
    {
        case 0x500:
            strVersion = "Windows2000";
            break;
        case 0x501:
            strVersion = "WindowsXP";
            break;
        case 0x502:
            strVersion = "WindowsServer2003";
            break;
        case 0x600:
            strVersion = "WindowsVista";
            break;
        case 0x601:
            strVersion = "Windows7";
            break;
        case 0x602:
            strVersion = "Windows8";
            break;
        case 0x603:
            strVersion = "Windows8.1";
            break;
        case 0xA00:
        {
            if (dwBuildNumber >= 22000)
                strVersion = "Windows11";
            else
                strVersion = "Windows10";
            break;
        }
        default:
            char szRet[260];
            sprintf(szRet, "%d", nVersion);
            return szRet;
    }
    if(IsWow64()) {
        strVersion.append("_64");
    }
    switch (dwBuildNumber) {
        case 3000:
        case 6000:
        case 6001:
        case 6002:
        case 7600:
            break;
        case 7601:
            strVersion.append("SP1");
            break;
        case 9200:
            break;
        case 9600:
            strVersion.append("_version_Update 1");
        case 10240:
            break;
        case 10586:
            strVersion.append("_version_1511");
            break;
        case 14393:
            strVersion.append("_version_1607");
            break;
        case 15063:
            strVersion.append("_version_1703");
            break;
        case 16299:
            strVersion.append("_version_1709");
            break;
        case 17134:
            strVersion.append("_version_1803");
            break;
        case 17763:
            strVersion.append("_version_1809");
            break;
        case 18362:
            strVersion.append("_version_1903");
            break;
        case 18363:
            strVersion.append("_version_1909");
            break;
        case 19041:
            strVersion.append("_version_2004");
            break;
        case 19042:
            strVersion.append("_version_20H2");
            break;
        case 19043:
            strVersion.append("_version_21H1");
            break;
        case 19044:
            strVersion.append("_version_21H2");
            break;
        case 19045:
            strVersion.append("_version_22H2");
            break;
        case 22000:
            strVersion.append("_version_21H2");
            break;
        case 22621:
            strVersion.append("_version_22H2");
            break;
    }
    strVersion.append("(")
            .append(std::to_string(dwMajorVer))
            .append(".")
            .append(std::to_string(dwMinorVer))
            .append(".")
            .append(std::to_string(dwBuildNumber))
            .append(")");
    return strVersion;
#else
    return "unknown";
#endif
}

int main() {
    std::cout << GetOSVersionString().c_str() << std::endl;
    system("pause");
    return 0;
}
