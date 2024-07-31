// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__D6E8EFE4_7B62_4AEC_9972_7FC00BE2713B__INCLUDED_)
#define AFX_STDAFX_H__D6E8EFE4_7B62_4AEC_9972_7FC00BE2713B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // mfc core and standard components
#include <afxext.h>         // mfc extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // mfc ole classes
#include <afxodlgs.h>       // mfc ole dialog classes
#include <afxdisp.h>        // mfc automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// mfc odbc database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// mfc dao database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// mfc support for internet explorer 4 common controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// mfc support for windows common controls
#endif // _AFX_NO_AFXCMN_SUPPORT


/////////////////////////////////////////////////////////////////////////////
// common header
#include "common.h"

// ST Style Button
#include ".\\cbuttonst\\btnst.h"

// thread messages
#define WM_ETHERNET_RECV_PACKET     (WM_USER + 1000)
#define WM_ETHERNET_RECV_TIMEOUT    (WM_USER + 1001)



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__D6E8EFE4_7B62_4AEC_9972_7FC00BE2713B__INCLUDED_)
