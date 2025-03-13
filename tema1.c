#include <stdio.h>
#include <tchar.h>
#include <windows.h>
 
void SearchAndListDrivers(HKEY hKey, const TCHAR* path) {
    HKEY hSubKey;
    LONG result;
    DWORD index = 0;
    TCHAR subKeyName[256]; 
    DWORD subKeyNameLength;
    DWORD typeValue;
    DWORD typeSize;
    TCHAR imagePath[512];  
    DWORD imagePathSize;
 
    result = RegOpenKeyEx(hKey, path, 0, KEY_READ, &hSubKey);
    if (result != ERROR_SUCCESS) {
        return;
    }
 
    while (1) {
        subKeyNameLength = sizeof(subKeyName) / sizeof(TCHAR);
        result = RegEnumKeyEx(hSubKey, index, subKeyName, &subKeyNameLength, NULL, NULL, NULL, NULL);
 
        if (result == ERROR_NO_MORE_ITEMS) {
            break;  
        }
 
        if (result == ERROR_SUCCESS) {
            HKEY hInnerSubKey;
            result = RegOpenKeyEx(hSubKey, subKeyName, 0, KEY_READ, &hInnerSubKey);
            if (result == ERROR_SUCCESS) {
 
                typeSize = sizeof(typeValue);
                result = RegQueryValueEx(hInnerSubKey, TEXT("Type"), NULL, NULL, (LPBYTE)&typeValue, &typeSize);
 
                if (result == ERROR_SUCCESS && (typeValue == 1 || typeValue == 2)) {
 
                    imagePathSize = sizeof(imagePath) / sizeof(TCHAR);
                    result = RegQueryValueEx(hInnerSubKey, TEXT("ImagePath"), NULL, NULL, (LPBYTE)imagePath, &imagePathSize);
 
                    if (result == ERROR_SUCCESS) {
                        _tprintf(TEXT("Subcheia: %s - ImagePath: %s\n"), subKeyName, imagePath);
                    }
                }
                RegCloseKey(hInnerSubKey);
            }
        }
 
        index++;
    }
 
    RegCloseKey(hSubKey);
}
 
int main() {
 
    SearchAndListDrivers(HKEY_LOCAL_MACHINE, TEXT("SYSTEM\\CurrentControlSet\\Services"));
    return 0;
}
