/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */
#if 0
#include "mmi_app_ia_java_trc.h"
#include "sci_types.h"
#ifdef  JAVA_SUPPORT_IA
#include "jkpimcommon.h"
#include "jktypes.h"
#include "jkpim.h"
#include "pim_common.h"
#include "pim_contact.h"          
#include "ampimutil.h"   
#include "os_api.h"


JKPIMErrCode JkPIMGetListNames(JKSint32 taskId,JKPIMListType listType,JKPIMStringArray* array)
{
    JKPIMErrCode    error = JK_PIM_GENERAL_ERR;
#ifdef MMI_JAVA_PIM
//SCI_TRACE_LOW:"JkPIMGetListNames enter!"
SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_22_112_2_18_2_22_37_262,(uint8*)"");
    //SCI_TraceLow:"JkPIMGetListNames,enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_23_112_2_18_2_22_37_263,(uint8*)"");

    switch( listType )
    {
        case JK_PIM_CONTACT:
            error = PimContactGetListNames(taskId,listType,array);
            break;
        case JK_PIM_EVENT:
           return JK_PIM_FEATURE_NOT_SUPPORTED;
        case JK_PIM_TODO:
           return JK_PIM_FEATURE_NOT_SUPPORTED;      
        default:
            break;
}

    //SCI_TraceLow:"leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_38_112_2_18_2_22_37_264,(uint8*)"");
  #endif
    return error;
}
JKPIMErrCode JkPIMGetDefaultListName(JKSint32 taskId,JKPIMListType listType,JKPIMString* name)
{
    JKPIMErrCode error = JK_PIM_GENERAL_ERR;
#ifdef MMI_JAVA_PIM

    //SCI_TraceLow:"JkPIMGetDefaultListName:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_47_112_2_18_2_22_37_265,(uint8*)"");
    switch (listType) 
   {
        case JK_PIM_CONTACT:
            error = PimContactGetDefaultListName(taskId,listType,name);
            break;
        case JK_PIM_EVENT:
          return JK_PIM_FEATURE_NOT_SUPPORTED;          
        case JK_PIM_TODO:
          return JK_PIM_FEATURE_NOT_SUPPORTED;          
        default:
            break;   
}

    //SCI_TraceLow:"JkPIMGetDefaultListName,leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_61_112_2_18_2_22_37_266,(uint8*)"");
  #endif   
    return error;
}
JKPIMErrCode JkPIMOpenConnection(JKSint32 taskId,JKPIMListType listType,JKPIMAccessMode mode,JKPIMString* name,JKSint32* connectionId)
{

    JKPIMErrCode    error = JK_PIM_GENERAL_ERR;
#ifdef MMI_JAVA_PIM
    //SCI_TraceLow:"JkPIMOpenConnection:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_70_112_2_18_2_22_37_267,(uint8*)"");
    switch( listType )
    {
        case JK_PIM_CONTACT:
            error = PimContactOpenConnection(taskId, listType, mode, name, connectionId);
            break;
        case JK_PIM_EVENT:
            return JK_PIM_FEATURE_NOT_SUPPORTED;       
        case JK_PIM_TODO:
           return JK_PIM_FEATURE_NOT_SUPPORTED;         
        default:
            break;              
}

    //SCI_TraceLow:"JkPIMOpenConnection,leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_84_112_2_18_2_22_37_268,(uint8*)"");
 #endif
    return error;
}
JKPIMErrCode JkPIMCloseConnection(JKSint32 taskId,JKSint32 connectionId,JKBool finalize)
{
    JKPIMErrCode    error = JK_PIM_GENERAL_ERR;
#ifdef MMI_JAVA_PIM    
    //SCI_TraceLow:"JkPIMCloseConnection:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_92_112_2_18_2_22_37_269,(uint8*)"");
    switch( LISTTYPE(connectionId) )
    {
        case JK_PIM_CONTACT:
            error = PimContactCloseConnection(taskId, connectionId,finalize);
            break;
        case JK_PIM_EVENT:
            return JK_PIM_FEATURE_NOT_SUPPORTED;       
        case JK_PIM_TODO:
            return JK_PIM_FEATURE_NOT_SUPPORTED;        
        default:
            break;             
    }
    //SCI_TraceLow:"JkPIMCloseConnection,leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_105_112_2_18_2_22_37_270,(uint8*)"");
#endif    
    return error;
}

JKPIMErrCode JkPIMGetSupportedFields(JKSint32 taskId,JKSint32 connectionId,JKPIMFieldDefArray* array)
{
    JKPIMErrCode    error = JK_PIM_GENERAL_ERR;
#ifdef MMI_JAVA_PIM
    //SCI_TraceLow:"JkPIMGetSupportedFields:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_114_112_2_18_2_22_37_271,(uint8*)"");
    switch( LISTTYPE(connectionId) )
   {
        case JK_PIM_CONTACT:
            error = PimContactGetSupportedFields(taskId, connectionId, array);
            break;
        case JK_PIM_EVENT:
           return JK_PIM_FEATURE_NOT_SUPPORTED;          
        case JK_PIM_TODO:
           return JK_PIM_FEATURE_NOT_SUPPORTED;            
        default:
            break;              
    }
    //SCI_TraceLow:"JkPIMGetSupportedFields,leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_127_112_2_18_2_22_37_272,(uint8*)"");
 #endif
    return error;
}

JKPIMErrCode JkPIMGetSupportedAttributes(JKSint32 taskId,JKSint32 connectionId,JKPIMAttributeDefArray* array)
{
    JKPIMErrCode error = JK_PIM_GENERAL_ERR;
#ifdef MMI_JAVA_PIM
    //SCI_TraceLow:"JkPIMGetSupportedAttributes:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_136_112_2_18_2_22_37_273,(uint8*)"");
    switch( LISTTYPE(connectionId) )
    {
        case JK_PIM_CONTACT:
            error = PimContactGetSupportedAttributes(taskId, connectionId, array);
            break;
        case JK_PIM_EVENT:
            return JK_PIM_FEATURE_NOT_SUPPORTED;           
        case JK_PIM_TODO:
            return JK_PIM_FEATURE_NOT_SUPPORTED;                       
        default:
            break;              
    }
    //SCI_TraceLow:"JkPIMGetSupportedAttributes,leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_149_112_2_18_2_22_37_274,(uint8*)"");
#endif    
    return error;
}

JKPIMErrCode JkPIMReadItem(JKSint32 taskId,JKSint32 connectionId,JKSint32 itemId,JKPIMItem* item)
{

    JKPIMErrCode    error = JK_PIM_FEATURE_NOT_SUPPORTED;
#ifdef MMI_JAVA_PIM
    //SCI_TraceLow:"JkPIMReadItem:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_159_112_2_18_2_22_37_275,(uint8*)"");
    switch( LISTTYPE(connectionId) )
   {
        case JK_PIM_CONTACT:
            error = PimContactReadItem(taskId,connectionId,item);
            break;
        case JK_PIM_EVENT:
            return JK_PIM_FEATURE_NOT_SUPPORTED;           
        case JK_PIM_TODO:
            return JK_PIM_FEATURE_NOT_SUPPORTED;           
        default:
            break;             
}

    //SCI_TraceLow:"JkPIMReadItem,leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_173_112_2_18_2_22_37_276,(uint8*)"");
 #endif
    return error;
}
JKPIMErrCode JkPIMAddItem(JKSint32 taskId,JKSint32 connectionId,JKPIMItem* item,JKSint32* itemId)
{
    JKPIMErrCode    error = JK_PIM_FEATURE_NOT_SUPPORTED;
#ifdef MMI_JAVA_PIM

    //SCI_TraceLow:"JkPIMAddItem:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_182_112_2_18_2_22_37_277,(uint8*)"");
    switch( LISTTYPE(connectionId) )
   {
        case JK_PIM_CONTACT:
            error = PimContactAddItem(taskId,connectionId,item,itemId);
            break;
        case JK_PIM_EVENT:
            return JK_PIM_FEATURE_NOT_SUPPORTED;           
        case JK_PIM_TODO:
            return JK_PIM_FEATURE_NOT_SUPPORTED;           
        default:
            break;            
}

    //SCI_TraceLow:"JkPIMAddItem,leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_196_112_2_18_2_22_38_278,(uint8*)"");
 #endif   
    return error;
}
JKPIMErrCode JkPIMSetItem(JKSint32 taskId,JKSint32 connectionId,JKSint32 itemId,JKPIMItem* item)
{
    JKPIMErrCode    error = JK_PIM_FEATURE_NOT_SUPPORTED;
#ifdef MMI_JAVA_PIM

    //SCI_TraceLow:"JkPIMSetItem:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_205_112_2_18_2_22_38_279,(uint8*)"");
    switch( LISTTYPE(connectionId) )
    {
        case JK_PIM_CONTACT:
            error = PimContactSetItem(taskId,connectionId,itemId,item);
            break;
        case JK_PIM_EVENT:
            return JK_PIM_FEATURE_NOT_SUPPORTED;           
        case JK_PIM_TODO:
            return JK_PIM_FEATURE_NOT_SUPPORTED;           
        default:
            break;            
}

    //SCI_TraceLow:"JkPIMSetItem,leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_219_112_2_18_2_22_38_280,(uint8*)"");
 #endif  
    return error;
}
JKPIMErrCode JkPIMDeleteItem(JKSint32 taskId,JKSint32 connectionId,JKSint32 itemId)
{
    JKPIMErrCode    error = JK_PIM_FEATURE_NOT_SUPPORTED;
#ifdef MMI_JAVA_PIM
    //SCI_TraceLow:"JkPIMDeleteItem:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_227_112_2_18_2_22_38_281,(uint8*)"");
    switch( LISTTYPE(connectionId) )
    {
        case JK_PIM_CONTACT:
            error = PimContactDeleteItem(taskId, connectionId, JK_TRUE);
            break;
        case JK_PIM_EVENT:
            return JK_PIM_FEATURE_NOT_SUPPORTED;           
        case JK_PIM_TODO:
            return JK_PIM_FEATURE_NOT_SUPPORTED;                     
        default:
            break;              
    }
#endif
    //SCI_TraceLow:"JkPIMDeleteItem,leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_241_112_2_18_2_22_38_282,(uint8*)"");
    return error;
}

JKPIMErrCode JkPIMIsCategorySupported(JKSint32 taskId,JKSint32 connectionId,JKSint32* listMaximum,JKSint32* itemMaximum,JKBool* distinction)
{
    JKPIMErrCode    error = JK_PIM_FEATURE_NOT_SUPPORTED;
#ifdef MMI_JAVA_PIM
    //SCI_TraceLow:"JkPIMIsCategorySupported:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_249_112_2_18_2_22_38_283,(uint8*)"");
    switch( LISTTYPE(connectionId) )
    {
        case JK_PIM_CONTACT:
            error = PimContactIsCategorySupported(taskId, connectionId, listMaximum, itemMaximum, distinction);
            break;
        case JK_PIM_EVENT:
            return JK_PIM_FEATURE_NOT_SUPPORTED;                    
        case JK_PIM_TODO:
            return JK_PIM_FEATURE_NOT_SUPPORTED;                    
        default:
            break;             
    }
#endif
    //SCI_TraceLow:"JkPIMIsCategorySupported,leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_263_112_2_18_2_22_38_284,(uint8*)"");
    return error;
}

JKPIMErrCode JkPIMGetSupportedCategories(JKSint32 taskId,JKSint32 connectionId,JKPIMStringArray* categories)
{
    JKPIMErrCode    error = JK_PIM_FEATURE_NOT_SUPPORTED;
#ifdef MMI_JAVA_PIM
    //SCI_TraceLow:"JkPIMGetSupportedCategories:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_271_112_2_18_2_22_38_285,(uint8*)"");
    switch( LISTTYPE(connectionId) )
    {
        case JK_PIM_CONTACT:
            error = PimContactGetSupportedCategories(taskId, connectionId, categories);
            break;
        case JK_PIM_EVENT:
            return JK_PIM_FEATURE_NOT_SUPPORTED;           
        case JK_PIM_TODO:
            return JK_PIM_FEATURE_NOT_SUPPORTED;           
        default:
            break;             
    }
#endif
    //SCI_TraceLow:"JkPIMGetSupportedCategories,leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_285_112_2_18_2_22_38_286,(uint8*)"");
    return error;
}

JKPIMErrCode JkPIMIsCategory(JKSint32 taskId,JKSint32 connectionId,JKPIMString* category,JKBool* found)
{
    JKPIMErrCode    error = JK_PIM_FEATURE_NOT_SUPPORTED;
    //SCI_TraceLow:"JkPIMIsCategory:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_292_112_2_18_2_22_38_287,(uint8*)"");
#ifdef MMI_JAVA_PIM    
    switch( LISTTYPE(connectionId) )
   {
        case JK_PIM_CONTACT:
            error = PimContactIsCategory(taskId,connectionId,category,found);
            break;
        case JK_PIM_EVENT:
            return JK_PIM_FEATURE_NOT_SUPPORTED;           
        case JK_PIM_TODO:
            return JK_PIM_FEATURE_NOT_SUPPORTED;                     
        default:
            break;              
    }
#endif    
    //SCI_TraceLow:"JkPIMIsCategory,leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_307_112_2_18_2_22_38_288,(uint8*)"");
    return error;
}

JKPIMErrCode JkPIMAddCategory(JKSint32 taskId,JKSint32 connectionId,JKPIMString* category)
{
    JKPIMErrCode    error = JK_PIM_FEATURE_NOT_SUPPORTED;
    //SCI_TraceLow:"JkPIMAddCategory:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_314_112_2_18_2_22_38_289,(uint8*)"");
#ifdef MMI_JAVA_PIM
    switch( LISTTYPE(connectionId) )
    {
        case JK_PIM_CONTACT:
            error = PimContactAddCategory(taskId,connectionId,category);
            break;
        case JK_PIM_EVENT:
            return JK_PIM_FEATURE_NOT_SUPPORTED;                      
        case JK_PIM_TODO:
            return JK_PIM_FEATURE_NOT_SUPPORTED;                       
        default:
            break;            
    }
 #endif   
    //SCI_TraceLow:"JkPIMAddCategory,leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_329_112_2_18_2_22_38_290,(uint8*)"");
    return error;
}

 JKPIMErrCode JkPIMDeleteCategory(JKSint32 taskId,JKSint32 connectionId,JKPIMString* category,JKBool delete)
{
    JKPIMErrCode    error = JK_PIM_FEATURE_NOT_SUPPORTED;
    //SCI_TraceLow:"JkPIMDeleteCategory:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_336_112_2_18_2_22_38_291,(uint8*)"");
#ifdef MMI_JAVA_PIM
    switch( LISTTYPE(connectionId) )
    {
        case JK_PIM_CONTACT:
            error = PimContactDeleteCategory(taskId,connectionId,category,delete);
            break;
        case JK_PIM_EVENT:
            return JK_PIM_FEATURE_NOT_SUPPORTED;                     
        case JK_PIM_TODO:
            return JK_PIM_FEATURE_NOT_SUPPORTED;                      
        default:
            break;              
    }
 #endif   
    //SCI_TraceLow:"JkPIMDeleteCategory,leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_351_112_2_18_2_22_38_292,(uint8*)"");
    return error;
}

JKPIMErrCode JkPIMRenameCategory(JKSint32 taskId,JKSint32 connectionId,JKPIMString* currentCategory,JKPIMString* newCategory)
{
    JKPIMErrCode    error = JK_PIM_FEATURE_NOT_SUPPORTED;
    //SCI_TraceLow:"JkPIMRenameCategory:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_358_112_2_18_2_22_38_293,(uint8*)"");
#ifdef MMI_JAVA_PIM
    switch( LISTTYPE(connectionId) )
    {
        case JK_PIM_CONTACT:
            error = PimContactRenameCategory(taskId,connectionId,currentCategory,newCategory);
            break;
        case JK_PIM_EVENT:
            return JK_PIM_FEATURE_NOT_SUPPORTED;                       
        case JK_PIM_TODO:
            return JK_PIM_FEATURE_NOT_SUPPORTED;                       
        default:
            break;             
    }
 #endif
    //SCI_TraceLow:"JkPIMRenameCategory,leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_373_112_2_18_2_22_38_294,(uint8*)"");
    return error;
}

JKPIMErrCode JkPIMAddCategoryToItem(JKSint32 taskId,JKSint32 connectionId,JKSint32 itemId,JKPIMString* category,JKBool* addable)
{
    JKPIMErrCode    error = JK_PIM_FEATURE_NOT_SUPPORTED;
    //SCI_TraceLow:"JkPIMAddCategoryToItem:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_380_112_2_18_2_22_38_295,(uint8*)"");
#ifdef MMI_JAVA_PIM    
    switch( LISTTYPE(connectionId) )
    {
        case JK_PIM_CONTACT:
            error = PimContactAddCategoryToItem(taskId,connectionId,itemId,category,addable);
            break;
        case JK_PIM_EVENT:
            return JK_PIM_FEATURE_NOT_SUPPORTED;                       
        case JK_PIM_TODO:
            return JK_PIM_FEATURE_NOT_SUPPORTED;                       
        default:
            break;            
    }
 #endif
    //SCI_TraceLow:"JkPIMAddCategoryToItem,leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_395_112_2_18_2_22_38_296,(uint8*)"");
    return error;
}

JKPIMErrCode JkPIMMoveToNextItem(JKSint32 taskId,JKSint32 connectionId,JKSint32 resultSetId,JKSint32* itemId)
{
    JKPIMErrCode    error = JK_PIM_FEATURE_NOT_SUPPORTED;
    //SCI_TraceLow:"JkPIMMoveToNextItem:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_402_112_2_18_2_22_38_297,(uint8*)"");
#ifdef MMI_JAVA_PIM
    switch( LISTTYPE(connectionId) )
   {
        case JK_PIM_CONTACT:
            error = PimContactMoveToNextItem(taskId,connectionId,resultSetId,itemId);
            break;
        case JK_PIM_EVENT:
            return JK_PIM_GENERAL_ERR;                       
        case JK_PIM_TODO:
            return JK_PIM_GENERAL_ERR;           
        default:
            break;             
    }
#endif
    //SCI_TraceLow:"JkPIMMoveToNextItem,leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_417_112_2_18_2_22_38_298,(uint8*)"");
    return error;
}

JKPIMErrCode JkPIMGetResultSetByAll(JKSint32 taskId,JKSint32 connectionId,JKSint32* resultSetId)
{
    JKPIMErrCode    error = JK_PIM_FEATURE_NOT_SUPPORTED;
    //SCI_TraceLow:"JkPIMGetResultSetByAll:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_424_112_2_18_2_22_38_299,(uint8*)"");
#ifdef MMI_JAVA_PIM
    switch( LISTTYPE(connectionId) )
    {
        case JK_PIM_CONTACT:
            error = PimContactGetResultSetByAll(taskId,connectionId,resultSetId);
            break;
        case JK_PIM_EVENT:
            return JK_PIM_GENERAL_ERR;           
        case JK_PIM_TODO:
            return JK_PIM_GENERAL_ERR;           
        default:
            break;             
    }
#endif
    //SCI_TraceLow:"JkPIMGetResultSetByAll,leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_439_112_2_18_2_22_38_300,(uint8*)"");
    return error;
}

JKPIMErrCode JkPIMGetResultSetByItem(JKSint32 taskId,JKSint32 connectionId,JKPIMItem* pItem,JKSint32* resultSetId)
{
    JKPIMErrCode    error = JK_PIM_FEATURE_NOT_SUPPORTED;
#ifdef MMI_JAVA_PIM
    //SCI_TraceLow:"JkPIMGetResultSetByItem:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_447_112_2_18_2_22_38_301,(uint8*)"");
    switch( LISTTYPE(connectionId) )
   {
        case JK_PIM_CONTACT:
            error = PimContactGetResultSetByItem(taskId,connectionId,pItem,resultSetId);
            break;
        case JK_PIM_EVENT:
            return JK_PIM_GENERAL_ERR;           
        case JK_PIM_TODO:
            return JK_PIM_GENERAL_ERR;           
        default:
            break;           
    }
 #endif   
    //SCI_TraceLow:"JkPIMGetResultSetByAll,leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_461_112_2_18_2_22_38_302,(uint8*)"");
    return error;
}

JKPIMErrCode JkPIMGetResultSetByString(JKSint32 taskId,JKSint32 connectionId,JKPIMString* value,JKSint32* resultSetId)
{
    JKPIMErrCode    error = JK_PIM_FEATURE_NOT_SUPPORTED;
#ifdef MMI_JAVA_PIM
    //SCI_TraceLow:"JkPIMGetResultSetByString:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_469_112_2_18_2_22_38_303,(uint8*)"");
    switch( LISTTYPE(connectionId) )
   {
        case JK_PIM_CONTACT:
            error = PimContactGetResultSetByString(taskId,connectionId,value,resultSetId);
            break;
        case JK_PIM_EVENT:
            return JK_PIM_GENERAL_ERR;           
        case JK_PIM_TODO:
            return JK_PIM_GENERAL_ERR;           
        default:
            break;            
    }
 #endif   
    //SCI_TraceLow:"JkPIMGetResultSetByString,leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_483_112_2_18_2_22_38_304,(uint8*)"");
    return error;
}

JKPIMErrCode JkPIMGetResultSetByCategory(JKSint32 taskId,JKSint32 connectionId,JKPIMString*category,JKSint32* resultSetId)
{
    JKPIMErrCode    error = JK_PIM_FEATURE_NOT_SUPPORTED;
#ifdef MMI_JAVA_PIM
    //SCI_TraceLow:"JkPIMGetResultSetByCategory:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_491_112_2_18_2_22_38_305,(uint8*)"");
    switch( LISTTYPE(connectionId) )
    {
        case JK_PIM_CONTACT:
           error = PimContactGetResultSetByCategory(taskId,connectionId,category,resultSetId);
            break;
        case JK_PIM_EVENT:
            return JK_PIM_GENERAL_ERR;           
        case JK_PIM_TODO:
            return JK_PIM_GENERAL_ERR;           
        default:
            break;             
    }
#endif    
    //SCI_TraceLow:"JkPIMGetResultSetByCategory,leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_505_112_2_18_2_22_38_306,(uint8*)"");
    return error;
}

JKPIMErrCode JkPIMGetResultSetForEvent(JKSint32 taskId,JKSint32 connectionId,JKSint32 searchType,JKPIMCalendar* startDate,JKPIMCalendar* endDate,JKBool initialEventOnly,JKSint32* resultSetId)
{
    JKPIMErrCode    error = JK_PIM_FEATURE_NOT_SUPPORTED;
#ifdef MMI_JAVA_PIM
    //SCI_TraceLow:"JkPIMGetResultSetForEvent:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_513_112_2_18_2_22_38_307,(uint8*)"");
    switch( LISTTYPE(connectionId) )
    {
        case JK_PIM_EVENT:          
            return JK_PIM_FEATURE_NOT_SUPPORTED;           
        default:
            break;              
    }
#endif    
    //SCI_TraceLow:"JkPIMGetResultSetForEvent,leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_522_112_2_18_2_22_38_308,(uint8*)"");
    return error;
}

JKPIMErrCode JkPIMGetResultSetForToDoDate(JKSint32 taskId,JKSint32 connectionId,JKSint32 field,JKPIMDate* startDate,JKPIMDate* endDate,JKSint32* resultSetId)
{
    JKPIMErrCode    error = JK_PIM_FEATURE_NOT_SUPPORTED;
#ifdef MMI_JAVA_PIM
    //SCI_TraceLow:"JkPIMGetResultSetForToDoDate:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_530_112_2_18_2_22_38_309,(uint8*)"");
    switch( LISTTYPE(connectionId) )
    {
        case JK_PIM_TODO:
            return JK_PIM_FEATURE_NOT_SUPPORTED;           
        default:
            break;              
    }
#endif    
    //SCI_TraceLow:"JkPIMGetResultSetForToDoDate,leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_539_112_2_18_2_22_38_310,(uint8*)"");
    return error;
}

JKPIMErrCode JkPIMGetResultSetForToDoCalendar(JKSint32 taskId,JKSint32 connectionId,JKSint32 fieldId,JKPIMCalendar* startDate,JKPIMCalendar* endDate,JKSint32* resultSetId)
{
    JKPIMErrCode    error = JK_PIM_FEATURE_NOT_SUPPORTED;
#ifdef MMI_JAVA_PIM
    //SCI_TraceLow:"JkPIMGetResultSetForToDoCalendar:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_547_112_2_18_2_22_38_311,(uint8*)"");
    switch( LISTTYPE(connectionId) )
    {
        case JK_PIM_TODO:
            return JK_PIM_FEATURE_NOT_SUPPORTED;           
        default:
            break;              
    }
#endif    
   //SCI_TraceLow:"JkPIMGetResultSetForToDoCalendar,leave"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_556_112_2_18_2_22_39_312,(uint8*)"");
    return error;
}

JKPIMErrCode JkPIMCloseResultSet(JKSint32 taskId,JKSint32 connectionId,JKSint32 resultSetId)
{
    JKPIMErrCode    error = JK_PIM_FEATURE_NOT_SUPPORTED;
#ifdef MMI_JAVA_PIM
    //SCI_TraceLow:"JkPIMCloseResultSet:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_564_112_2_18_2_22_39_313,(uint8*)"");
    switch( LISTTYPE(connectionId) )
    {
        case JK_PIM_CONTACT:
            error = PimContactCloseResultSet(taskId,connectionId,resultSetId);
            break;
        case JK_PIM_EVENT:
            return JK_PIM_FEATURE_NOT_SUPPORTED;           
        case JK_PIM_TODO:
            return JK_PIM_FEATURE_NOT_SUPPORTED;           
        default:
            break;            
    }
#endif
   //SCI_TraceLow:"JkPIMCloseResultSet,leave"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_578_112_2_18_2_22_39_314,(uint8*)"");
    return error;
}

void* JkPIMAllocPointer(JKSint32 size)
{
//SCI_TRACE_LOW:"JkPIMAllocPointer enter!"
SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_584_112_2_18_2_22_39_315,(uint8*)"");
    return (void*)SCI_ALLOC(size);
}

void JkPIMFreePointer(void* p)
{
//SCI_TRACE_LOW:"JkPIMFreePointer enter!"
SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_590_112_2_18_2_22_39_316,(uint8*)"");
    if(p != PNULL)
{
        SCI_FREE(p);
        p = PNULL;
    }
    return;
}

JKPIMErrCode JkPIMGetSupportedSerialFormats(JKPIMListType listType,JKPIMStringArray* array)
{
    //SCI_TraceLow:"JkPIMGetSupportedSerialFormats:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_601_112_2_18_2_22_39_317,(uint8*)"");
    return (JKPIMErrCode)0;
}

/*****************************************************************************/
JKPIMErrCode JkPIMStartFromSerialFormat(JKPIMString* enc)
{
    //SCI_TraceLow:"JkPIMStartFromSerialFormat:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_607_112_2_18_2_22_39_318,(uint8*)"");
    return (JKPIMErrCode)0;
}

JKPIMErrCode JkPIMEndFromSerialFormat(void)
{
    //SCI_TraceLow:"JkPIMEndFromSerialFormat:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_613_112_2_18_2_22_39_319,(uint8*)"");
    return (JKPIMErrCode)0;
}

/*****************************************************************************/
JKPIMErrCode JkPIMReceiveSerialByte(JKSint8 data)
{
    //SCI_TraceLow:"JkPIMReceiveSerialByte:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_619_112_2_18_2_22_39_320,(uint8*)"");
    return (JKPIMErrCode)0;
}

/*****************************************************************************/
JKPIMErrCode JkPIMReceiveSerialBytes(JKSint8* data,JKSint32 size)
{
    //SCI_TraceLow:"JkPIMReceiveSerialBytes:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_625_112_2_18_2_22_39_321,(uint8*)"");
    return (JKPIMErrCode)0;
}

/*****************************************************************************/
const JKSint8* JkPIMGetSerialFormatHandlerClassName(void)
{
    //SCI_TraceLow:"JkPIMGetSerialFormatHandlerClassName:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_631_112_2_18_2_22_39_322,(uint8*)"");
    return (const JKSint8*)JK_NULL;
}

/*****************************************************************************/
JKPIMErrCode JkPIMExecuteToSerialFormat(JKSint32 taskId,JKPIMItem* pItem,JKPIMString* enc,JKPIMString* format,JKSint8** data,JKSint32* size)
{
    //SCI_TraceLow:"JkPIMExecuteToSerialFormat:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_637_112_2_18_2_22_39_323,(uint8*)"");
    return (JKPIMErrCode)0;
}

/*****************************************************************************/
JKPIMErrCode JkPIMExecuteFromSerialFormat(JKSint32 taskId,JKPIMItemArray* itemArray)
{
    //SCI_TraceLow:"JkPIMExecuteFromSerialFormat:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_643_112_2_18_2_22_39_324,(uint8*)"");
    return (JKPIMErrCode)0;
}

/*****************************************************************************/
JKPIMErrCode JkPIMGetSupportedRepeatRuleFields(JKSint32 taskId,JKSint32 connectionId,JKPIMFrequencyDef* frequencies)
{
    JKPIMErrCode    error = JK_PIM_GENERAL_ERR;
    //SCI_TraceLow:"JkPIMGetSupportedRepeatRuleFields:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_650_112_2_18_2_22_39_325,(uint8*)"");
    return error;
}

/*****************************************************************************/
JKPIMErrCode JkPIMImportItem(JKSint32 taskId,JKSint32 connectionId,JKPIMItem* destItem,JKPIMItem* srcItem)
{
    JKPIMErrCode    error = JK_PIM_FEATURE_NOT_SUPPORTED;
    //SCI_TraceLow:"JkPIMImportItem:enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKPIM_657_112_2_18_2_22_39_326,(uint8*)"");
    return error;
}
#endif
#endif
