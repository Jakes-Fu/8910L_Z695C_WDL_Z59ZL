
#ifndef ___DEVPIMCONTACT_H
#define ___DEVPIMCONTACT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*Prototype Declaration*/
JKPIMErrCode PimContactOpenConnection(const JKSint32 taskId,const JKPIMListType listType,const JKPIMAccessMode mode,const JKPIMString* name,JKSint32* connectionId);
JKPIMErrCode PimContactCloseConnection(const JKSint32 taskId,const JKSint32 connectionId,const JKBool finalize);
JKPIMErrCode PimContactGetSupportedFields(const JKSint32 taskId,const JKSint32 connectionId,JKPIMFieldDefArray* array);
JKPIMErrCode PimContactGetSupportedAttributes(const JKSint32 taskId,const JKSint32 connectionId,JKPIMAttributeDefArray* array);
JKPIMErrCode PimContactGetDefaultListName(const JKSint32 taskId,const JKPIMListType listType,JKPIMString* name);
JKPIMErrCode PimContactGetListNames(const JKSint32 taskId,const JKPIMListType listType,JKPIMStringArray* array);
JKPIMErrCode PimContactReadItem(const JKSint32 taskId,JKSint32 itemId,JKPIMItem* item);
JKPIMErrCode PimContactAddItem(const JKSint32 taskId,const JKSint32 connectionId,JKPIMItem* item,JKSint32* itemId);
JKPIMErrCode PimContactSetItem(const JKSint32 taskId,const JKSint32 connectionId,JKSint32 itemId,JKPIMItem* item);
JKPIMErrCode PimContactDeleteItem(const JKSint32 taskId, JKSint32 itemId,const JKBool commitCtrl);
JKPIMErrCode PimContactIsCategorySupported(const JKSint32 taskId,const JKSint32 connectionId,JKSint32* listMaximum,JKSint32* itemMaximum,JKBool* distinction);
JKPIMErrCode PimContactGetSupportedCategories(const JKSint32 taskId,const JKSint32 connectionId,JKPIMStringArray* categories);
JKPIMErrCode PimContactIsCategory(const JKSint32 taskId,const JKSint32 connectionId,const JKPIMString* category,JKBool* found);
JKPIMErrCode PimContactAddCategory(const JKSint32 taskId,const JKSint32 connectionId,const JKPIMString* category);
JKPIMErrCode PimContactDeleteCategory(const JKSint32 taskId,const JKSint32 connectionId,JKPIMString* category,const JKBool isDelete);
JKPIMErrCode PimContactAddCategoryToItem(const JKSint32 taskId,const JKSint32 connectionId,const JKSint32 itemId,const JKPIMString* category,JKBool* addable);
JKPIMErrCode PimContactRenameCategory(const JKSint32 taskId,const JKSint32 connectionId,const JKPIMString* currentCategory,const JKPIMString* newCategory);
JKPIMErrCode PimContactMoveToNextItem(const JKSint32 taskId,const JKSint32 connectionId,const JKSint32 resultSetId,JKSint32* itemId);
JKPIMErrCode PimContactGetResultSetByAll(const JKSint32 taskId,const JKSint32 connectionId,JKSint32* resultSetId);
JKPIMErrCode PimContactGetResultSetByItem(const JKSint32 taskId,const JKSint32 connectionId,JKPIMItem* pItem,JKSint32* resultSetId);
JKPIMErrCode PimContactGetResultSetByString(const JKSint32 taskId,const JKSint32 connectionId,const JKPIMString* string,JKSint32* resultSetId);
JKPIMErrCode PimContactGetResultSetByCategory(const JKSint32 taskId,const JKSint32 connectionId,const JKPIMString* category,JKSint32* resultSetId);
JKPIMErrCode PimContactCloseResultSet(const JKSint32 taskId,const JKSint32 connectionId,const JKSint32 resultSetId);
//JKPIMErrCode PimContactGetDataFromItem(JKPIMItem* item,PimContact1Data* data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ___DEVPIMCONTACT_H */



