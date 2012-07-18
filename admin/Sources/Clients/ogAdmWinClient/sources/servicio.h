// ******************************************************************************************************************************************************************************
// Aplicaci�n HIDRA
// Copyright 2003-2005 Jos� Manuel Alonso. Todos los derechos reservados.
// Fichero: servicio.h
//	Descripci�n:
//		Este proyecto implementa el servicio hidra en un ordenador con plataforma windows NT. Este fichero aporta las funciones para crear el servicio
// ******************************************************************************************************************************************************************************
//____________________________________________________________________________________________________________________________
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1997  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: service.h
//  AUTHOR: Craig Link
//
//  Comments:  The use of this header file and the accompanying service.c
//  file simplifies the process of writting a service.  You as a developer
//  simply need to follow the TODO's outlined in this header file, and 
//  implement the ServiceStart() and ServiceStop() functions.
//  
//  There is no need to modify the code in service.c.  Just add service.c
//  to your project and link with the following libraries...
//
//  libcmt.lib kernel32.lib advapi.lib shell32.lib
//
//  This code also supports unicode.  Be sure to compile both service.c and
//  and code #include "service.h" with the same Unicode setting.
//
//  Upon completion, your code will have the following command line interface
//
//  <service exe> -?                to display this list
//  <service exe> -install          to install the service
//  <service exe> -remove           to remove the service
//  <service exe> -debug <params>   to run as a console app for debugging
//
//  Note: This code also implements Ctrl+C and Ctrl+Break handlers
//        when using the debug option.  These console events cause
//        your ServiceStop routine to be called
//
//        Also, this code only handles the OWN_SERVICE service type
//        running in the LOCAL_SYSTEM security context.
//
//        To control your service ( start, stop, etc ) you may use the
//        Services control panel applet or the NET.EXE program.
//
//        To aid in writing/debugging service, the
//        SDK contains a utility (MSTOOLS\BIN\SC.EXE) that
//        can be used to control, configure, or obtain service status.
//        SC displays complete status for any service/driver
//        in the service database, and allows any of the configuration
//        parameters to be easily changed at the command line.
//        For more information on SC.EXE, type SC at the command line.
//
//____________________________________________________________________________________________________________________________
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <tchar.h>
#include "registrow.h"

#ifndef _SERVICE_H
#define _SERVICE_H

#ifdef __cplusplus
extern "C" {
#endif

// name of the executable
#define SZAPPNAME "ogAdmWinClient"
// internal name of the service
#define SZSERVICENAME "Cliente Opengnsys"
// displayed name of the service
#define SZSERVICEDISPLAYNAME "Cliente Opengnsys"
// list of service dependencies - "dep1\0dep2\0\0"
#define SZDEPENDENCIES ""

#define SERVIDOR_DEFAULT "0.0.0.0" 
#define PUERTO_DEFAULT	 "2003"	
#define IPLOCAL_DEFAULT "0.0.0.0"

#define CHKREGISTRY(f) if (!(f)) { return 0;}
#define RMVREGISTRY(f) if (!(f)) { return 0;}
#define TOMAPARAMINT(p) p=atoi(&argv[i][3]); 
#define TOMAPARAMSTR(p) strcpy(p,&argv[i][3]);

#define HIVE HKEY_LOCAL_MACHINE				// Rama del registro donde estar�n los parametros de conexi�n
#define BASEKEY "SOFTWARE\\opengnsys"	// Key del registro para parametros de conexi�n
#define BASE "SOFTWARE\\opengnsys\\cliente"	// SubKey del registro para parametros de conexi�n

//____________________________________________________________________________________________________________________________
//
SERVICE_STATUS   ssStatus;       // current status of the service
SERVICE_STATUS_HANDLE   sshStatusHandle;
//
//____________________________________________________________________________________________________________________________
//		ServiceStart()must be defined by in your code.
//		The service should use ReportStatusToSCMgr to indicate
//		progress.  This routine must also be used by StartService()
//		to report to the SCM when the service is running.
//
//		If a ServiceStop procedure is going to take longer than
//		3 seconds to execute, it should spawn a thread to
//		execute the stop code, and return.  Otherwise, the
//		ServiceControlManager will believe that the service has
//		stopped responding
//
//____________________________________________________________________________________________________________________________
VOID ServiceStart(DWORD dwArgc, LPTSTR *lpszArgv);
VOID ServiceStop();
//____________________________________________________________________________________________________________________________
//	The following are procedures which
//	may be useful to call within the above procedures,
//	but require no implementation by the user.
//	They are implemented in service.c
//
//  FUNCTION: ReportStatusToSCMgr()
//
//  PURPOSE: Sets the current status of the service and
//           reports it to the Service Control Manager
//
//  PARAMETERS:
//    dwCurrentState - the state of the service
//    dwWin32ExitCode - error code to report
//    dwWaitHint - worst case estimate to next checkpoint
//
//  RETURN VALUE:
//    TRUE  - success 
//    FALSE - failure
//____________________________________________________________________________________________________________________________
BOOL ReportStatusToSCMgr(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint);
//____________________________________________________________________________________________________________________________
//  FUNCTION: AddToMessageLog(LPTSTR lpszMsg)
//
//  PURPOSE: Allows any thread to log an error message
//
//  PARAMETERS:
//    lpszMsg - text for message
//
//  RETURN VALUE:
//    none
//____________________________________________________________________________________________________________________________
void AddToMessageLog(LPTSTR lpszMsg);
//____________________________________________________________________________________________________________________________
//
#ifdef __cplusplus
}
#endif

#endif
