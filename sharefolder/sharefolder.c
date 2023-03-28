#include <windows.h>
#include "beacon.h"
#include <lm.h>


DECLSPEC_IMPORT WINBASEAPI BOOL WINAPI KERNEL32$CreateDirectoryA(LPCSTR,LPSECURITY_ATTRIBUTES);
DECLSPEC_IMPORT WINBASEAPI BOOL WINAPI ADVAPI32$InitializeSecurityDescriptor(PSECURITY_DESCRIPTOR,DWORD);
DECLSPEC_IMPORT WINBASEAPI BOOL WINAPI ADVAPI32$SetSecurityDescriptorDacl(PSECURITY_DESCRIPTOR,BOOL,PACL,BOOL);
DECLSPEC_IMPORT NET_API_STATUS WINAPI NETAPI32$NetShareAdd(LPWSTR,DWORD,PBYTE,PDWORD);
DECLSPEC_IMPORT NET_API_STATUS WINAPI NETAPI32$NetShareDel(LPWSTR,LPWSTR,DWORD);
DECLSPEC_IMPORT WINBASEAPI BOOL WINAPI KERNEL32$RemoveDirectoryA(LPCSTR);

void go(char * args, int len) {
      //Establish variables
      datap parser;
      BeaconDataParse(&parser, args, len);
      char * path;
      wchar_t * shareName;
      int deleteFlag = 0;

      //Parse args
      path = BeaconDataExtract(&parser, NULL);
      shareName = (wchar_t *)BeaconDataExtract(&parser, NULL);
      deleteFlag = BeaconDataInt(&parser);

      if(deleteFlag==1) {
         //Delete directory
         int removeResult = 0;
         removeResult = KERNEL32$RemoveDirectoryA(path);
         if(removeResult!=0){
            BeaconPrintf(CALLBACK_OUTPUT, "Folder successfully deleted: %s", path);
         } else {
            BeaconPrintf(CALLBACK_ERROR, "Folder couldn't be deleted, check that it's empty before trying to delete.");
         }
         NET_API_STATUS res;
         res = NETAPI32$NetShareDel(NULL, shareName, 0);
         if(res==0) {
            BeaconPrintf(CALLBACK_OUTPUT, "Share successfuly deleted");
         }
         else {
            BeaconPrintf(CALLBACK_ERROR, "Error, check this code on http://msdn2.microsoft.com/en-us/library/aa370674.aspx: %d", res);
         }
         
      }
      else {
         //Create directory
         DWORD dwResponse;
         dwResponse = KERNEL32$CreateDirectoryA(path, NULL);
         if(dwResponse!=0) {
            BeaconPrintf(CALLBACK_OUTPUT, "Folder successfully created: %s", path);
            //Share directory
            NET_API_STATUS res;
            SHARE_INFO_2 p;
            DWORD parm_err = 0;
            wchar_t widePath[200];
            toWideChar(path, widePath, 200); 

            //Populate share info
            p.shi2_netname = shareName;    
            p.shi2_type = STYPE_DISKTREE; // disk drive
            p.shi2_remark = L"";
            p.shi2_permissions = ACCESS_ALL;    
            p.shi2_max_uses = -1;
            p.shi2_current_uses = 0;   
            p.shi2_path = widePath;
            p.shi2_passwd = NULL; // no password

            res = NETAPI32$NetShareAdd(NULL, 2, (LPBYTE) &p, &parm_err);

            if(res==0) {
               BeaconPrintf(CALLBACK_OUTPUT, "Folder successfully shared");
            }
            else {
               BeaconPrintf(CALLBACK_ERROR, "Error, check this code on http://msdn2.microsoft.com/en-us/library/aa370674.aspx: %d", res);
            }
         }
         else {
            BeaconPrintf(CALLBACK_ERROR, "Failed to create folder, folder may already exist or the path might not have been found.");
         }

         
      }

}