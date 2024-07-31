/*=====================================================================================
    FILE NAME   : ememodgen.c
    MODULE NAME : Email engine


    GENERAL DESCRIPTION
        this file is the entrance of email engine mod

    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History

    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-28   jiangdingiqng                     create this file
 
=====================================================================================*/


/*==========================================================================
                     INCLUDES AND VARIABLE DEFINITIONS
  ========================================================================== */
#include "AEEHeap.h"    // Memory allocation Routines
#include "AEEStdLib.h"  // AEE Standard C library functions
#include "ememodgen.h"  // Contains Sample AEEModule declarations
#include "nmdef.h"

#include "aee_emetm_api.h"
/*==========================================================================

                      PUBLIC DATA DECLARATIONS

  ========================================================================== */

/*-------------------------------------------------------------------
            Type Declarations
-------------------------------------------------------------------*/


/*-------------------------------------------------------------------
            Function Prototypes
-------------------------------------------------------------------*/

/*
  || Functions to be provided by the module developers.
*/

static uint32     AEEMod_AddRef(IModule *po);
static uint32     AEEMod_Release(IModule *po);
static int        AEEMod_CreateInstance(IModule *po,IShell *pIShell,
                                        AEECLSID ClsId, void **ppObj);
static void       AEEMod_FreeResources(IModule *po, IHeap *ph, IFileMgr *pfm);


/*-------------------------------------------------------------------
            Global Constant Definitions
-------------------------------------------------------------------*/


/*-------------------------------------------------------------------
            Global Data Definitions
-------------------------------------------------------------------*/
#ifdef AEE_SIMULATOR
   // IMPORTANT NOTE: g_pvtAEEStdLibEntry global variable is defined for 
   //   SDK ONLY! This variable should NOT BE:
   //
   //      (1) overwritten 
   //      (2) USED DIRECTLY by BREW SDK users. 
   //
   //  g_pvtAEEStdLibEntry is used as an entry point to AEEStdLib,
AEEHelperFuncs *g_pvtAEEStdLibEntry;
#endif

/*-------------------------------------------------------------------
            Static variable Definitions
-------------------------------------------------------------------*/


/* =========================================================================
                     FUNCTION DEFINITIONS
   ========================================================================= */

/* =========================================================================
Function:  AEEMod_Load()

Description:
   This is the module load function. It is invoked when the module is being
   loaded. It creates an instance of the EMEMod structure and initializes
   the data members appropriately.

   If running under the AEE Simulator, the function is prefixed with the tag
   __declspec(dllexport) so that this function is exported by the App DLL.
   This function shall be invoked from within the AEE.
   
Prototype:
   boolean AEEMod_Load(IShell *pIShell, char *psz, IModule **ppMod)

Parameters:
   piShell: Pointer to IShell interface
   psz:     String containing name of the resource file for module
   ppMod: [ou]: On return, *ppMod contains a valid pointer to the EMEMod 
               structure

Return Value:
   SUCCESS: If successful
   Error code: IF failed

Comments:
   This function must be the first function in the first module for 
   it to work on the handset!

Side Effects: 
   None

============================================================================*/
#ifdef AEE_LOAD_DLL
__declspec(dllexport) int AEEMod_Load(IShell *pIShell, void *ph, IModule **ppMod)
#else
#if defined(BREW_MODULE) || defined(FLAT_BREW)
extern int module_main(IShell *pIShell, void *ph, IModule **ppMod);
int module_main(IShell *pIShell, void *ph, IModule **ppMod)
#else
int AEEMod_Load(IShell *pIShell, void *ph, IModule **ppMod)
#endif
#endif
{
   // Invoke helper function to do the actual loading.
   return AEEStaticMod_New(sizeof(EMEMod),pIShell,ph,ppMod,NULL,NULL);
}

/*===========================================================================

Function:  AEEStaticMod_New()

Description:
   This is the module load function for static modules. It is directly 
   invoked by static modules when the static module is being loaded. 
   It also serves as a helper function for dynamic modules. 
   It creates an instance of the EMEMod structure and initializes the data
   members appropriately. The only difference between this function and the
   one used for dynamic mods is that this function takes an additional 
   parameter that specifies the CreateInstance function of the static module.

Prototype:
   boolean AEEStaticMod_New(int nSize, IShell *pIShell, void *ph, 
                            IModule **ppMod,PFNMODCREATEINST pfn)

Parameters:
   nSize: Specifies the size (in bytes) of the structure to be allocated for 
           the module.  Typically, this is the size of the EMEMod structure
   piShell: Pointer to IShell interface
   ph:  Not used on phone. In SDK, this is a pointer to the AEEHelperFuncs 
           structure. 
   ppMod: [ou]: On return, *ppMod contains a valid pointer to the EMEMod 
           structure
   pfn: Pointer to the modules' CreateInstance() function

Return Value:
   SUCCESS: If successful
   Error code: IF failed

Comments:  None

Side Effects: None

==============================================================================*/
int AEEStaticMod_New(int16 nSize, IShell *pIShell, void *ph, IModule **ppMod,
                     PFNMODCREATEINST pfnMC,PFNFREEMODDATA pfnMF)
{
   EMEMod *pMe = NULL;
   VTBL(IModule) *modFuncs;

   if (!ppMod || !pIShell) {
      return EFAILED;
   }

   if (nSize < 0) {
      return EBADPARM;
   }
   *ppMod = NULL;
  
#ifdef AEE_SIMULATOR
   // IMPORTANT NOTE: g_pvtAEEStdLibEntry global variable is defined for 
   //   SDK ONLY! This variable should NOT BE:
   //
   //      (1) overwritten 
   //      (2) USED DIRECTLY by BREW SDK users. 
   //
   //  g_pvtAEEStdLibEntry is used as an entry point to AEEStdLib,
   //   DO NOT REMOVE the next five lines.
   if (!ph) {
      return EFAILED;
   } else {
      g_pvtAEEStdLibEntry = (AEEHelperFuncs *)ph;
   }
#endif

   //Allocate memory for the EMEMod object

   if (nSize < sizeof(EMEMod)) {
      nSize += sizeof(EMEMod);
   }

   if (NULL == (pMe = (EMEMod *)MALLOC(nSize + sizeof(IModuleVtbl)))) {
      return ENOMEMORY;
   }
  
   // Allocate the vtbl and initialize it. Note that the modules and apps 
   // must not have any static data. Hence, we need to allocate the vtbl as 
   // well.

   modFuncs = (IModuleVtbl *)((byte *)pMe + nSize);

   // Initialize individual entries in the VTBL
   modFuncs->AddRef         = AEEMod_AddRef;
   modFuncs->Release        = AEEMod_Release;
   modFuncs->CreateInstance = AEEMod_CreateInstance;
   modFuncs->FreeResources  = AEEMod_FreeResources;


   // initialize the vtable
   INIT_VTBL(pMe, IModule, *modFuncs);

   // initialize the data members

   // Store address of Module's CreateInstance function
   pMe->pfnModCrInst = pfnMC;

   // Store Address of Module's FreeData function
   pMe->pfnModFreeData = pfnMF;

   pMe->m_nRefs = 1;
   pMe->m_pIShell = pIShell;
   ISHELL_AddRef(pIShell);

   /************************************************************************/
   /* add vtbl for EMEngine                                               */                   
   /************************************************************************/
   AEEEMETM_SetVtbl(&(pMe->AEEEMETMVtbl));

   MEMSET(&pMe->stMemHdr , 0 , sizeof(St_EME_MemHandle));
   pMe->stMemHdr.stHeader.dwCC1 = 0xCDCDCDCD;
   pMe->stMemHdr.stHeader.dwCC2 = 0xCDCDCDCD;

   pMe->stMemHdr.pTail = NULL;   // Set the pointer in the parameter

   *ppMod = (IModule*)pMe;

   return SUCCESS;
}

/*===========================================================================

Function:  AEEMod_AddRef()

Description:
   This function increases the referecne count for the EMEMod object

Prototype:
   uint32 AEEMod_AddRef(IModule *po)

Parameters:
   po: Pointer to IModule interface whos reference count is to be incremented

Return Value:
   The updated reference count

Comments:  None

Side Effects: None

==============================================================================*/
static uint32 AEEMod_AddRef(IModule *po)
{
   return (++((EMEMod *)po)->m_nRefs);
}


/*===========================================================================

Function:  AEEMod_Release()

Description:
   This function decreases the referecne count for the IModule object. If
   the reference count reaches zero, it does the required cleanup

Prototype:
   uint32 AEEMod_Release(IModule *po)

Parameters:
   po: Pointer to the IModule interface whose reference count needs to be
     decremented.

Return Value:
   The updated reference count

Comments:  None

Side Effects: None

==============================================================================*/
static uint32 AEEMod_Release(IModule *po)
{
   EMEMod *pMe = (EMEMod *)po;

   if (--pMe->m_nRefs != 0) {
      return pMe->m_nRefs;
   }

   // Ref count is zero. So, release memory associated with this object.


   // First, release user-specific data if any
   // Invoke User's FreeData function if they have registered
   if (pMe->pfnModFreeData) {
      pMe->pfnModFreeData(po);
   }
   if (pMe->m_pIShell) {
      ISHELL_Release(pMe->m_pIShell);
      pMe->m_pIShell = NULL;
   }

   EME_UTILS_OutputMemUsed(&pMe->stMemHdr);

   //Free the object itself
   FREE_VTBL(pMe, IModule);
   FREE(pMe);
  
   return 0;
}


/*===========================================================================

Function:  AEEMod_CreateInstance()

Description:
   This function is invoked so as to create the individual classes belonging
   to this module. It is invoked once for each class this module contains. 
   This function invokes the AEEClsCreateInstance() function that the module
   implementer must provide.

Prototype:
   int AEEMod_CreateInstance(IModule *pIModule,IShell *pIShell,AEECLSID ClsId,void **ppObj)

Parameters:
   pIModule: [in]: Pointer to this module
   pIShell: [in]: Pointer to the AEE Shell interface
   ClsID:   [in]: ClassId of the class that needs to be created. 
   ppobj:   [out]: If ClsID points to an applet, then *ppobj must point to a 
            valid structure that implemetns the IApplet interface. If ClsID 
            points to a class that is not an applet, *ppobj must point to a 
            valid IBase*.

Return Value:
   SUCCESS: If successful
   Error code: If failed

Comments:  None

Side Effects: None

==============================================================================*/
static int AEEMod_CreateInstance(IModule *pIModule,IShell *pIShell,
                                 AEECLSID ClsId,void **ppObj)
{
   EMEMod    *pme = (EMEMod *)pIModule;
   int        nErr = EFAILED;

   // For a dynamic module, they must supply the AEEClsCreateInstance() 
   //   function. Hence, invoke it. For a static app, they will have 
   //   registered the create Instance function. Invoke it.
   if (pme->pfnModCrInst) {
      nErr = pme->pfnModCrInst(ClsId, pIShell, pIModule, ppObj);
#if !defined(AEE_STATIC)
   } else {
      nErr = AEEClsCreateInstance(ClsId, pIShell, pIModule, ppObj);
#endif
   }

   return nErr;
}

/*===========================================================================

Function:  AEEMod_FreeResources()

Description:
   This function is invoked so that the module can free any of the resources
   that it has loaded.

Prototype:
   void AEEMod_FreeResources(IModule *po, IHeap *ph, IFileMgr *pfm)

Parameters:
   po: [in]: Pointer to this module
   ph: [in]: Pointer to the AEE IHeap interface
   pfm: Pointer to the resource file

Return Value:
   None

Comments:  None

Side Effects: None

==============================================================================*/
static void AEEMod_FreeResources(IModule *po, IHeap *ph, IFileMgr *pfm)
{
   (void)po,(void)ph,(void)pfm; /* all unused */
}



/*========================End of File ================================================*/
