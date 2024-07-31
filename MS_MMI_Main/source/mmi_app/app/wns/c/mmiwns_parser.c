/****************************************************************************
** File Name:      mmiwns_parser.c                                            *
** Author:         grant.peng                                              *
** Date:           17/07/2015                                               *
** Copyright:      2015 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the wns interface for java application.       *
*****************************************************************************
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 17/07/2015    grant.peng      Create
****************************************************************************/
#ifdef WIN32
#include "std_header.h"
#endif
#include "expat_external.h"
#include "expat.h"


#include "mmiidle_export.h"

#ifdef JAVA_SUPPORT_MYRIAD
#include "mmijava_export.h"
#include "jbed_exported_defines.h"
#include "cpl_tpal.h"
#endif //JAVA_SUPPORT_MYRIAD

#include "mmiwns_export.h"
#include "mmiwns_manager.h"

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/* toast 的八种类型: https://msdn.microsoft.com/en-us/library/windows/apps/hh761494.aspx

<toast>
    <visual>
        <binding template="ToastText01">
            <text id="1">bodyText</text>
        </binding>  
    </visual>
</toast>

<toast>
    <visual>
        <binding template="ToastText02">
            <text id="1">headlineText</text>
            <text id="2">bodyText</text>
        </binding>  
    </visual>
</toast>


<toast>
    <visual>
        <binding template="ToastText03">
            <text id="1">headlineText</text>
            <text id="2">bodyText</text>
        </binding>  
    </visual>
</toast>

<toast>
    <visual>
        <binding template="ToastText04">
            <text id="1">headlineText</text>
            <text id="2">bodyText1</text>
            <text id="3">bodyText2</text>
        </binding>  
    </visual>
</toast>


<toast>
    <visual>
        <binding template="ToastImageAndText01">
            <image id="1" src="image1" alt="image1"/>
            <text id="1">bodyText</text>
        </binding>  
    </visual>
</toast>

<toast>
    <visual>
        <binding template="ToastImageAndText02">
            <image id="1" src="image1" alt="image1"/>
            <text id="1">headlineText</text>
            <text id="2">bodyText</text>
        </binding>  
    </visual>
</toast>

<toast>
    <visual>
        <binding template="ToastImageAndText03">
            <image id="1" src="image1" alt="image1"/>
            <text id="1">headlineText</text>
            <text id="2">bodyText</text>
        </binding>  
    </visual>
</toast>

<toast>
    <visual>
        <binding template="ToastImageAndText04">
            <image id="1" src="image1" alt="image1"/>
            <text id="1">headlineText</text>
            <text id="2">bodyText1</text>
            <text id="3">bodyText2</text>
        </binding>  
    </visual>
</toast>

*/

#define WNSXML_TOAST_TYPE1      "ToastText01"
#define WNSXML_TOAST_TYPE2      "ToastText02"
#define WNSXML_TOAST_TYPE3      "ToastText03"
#define WNSXML_TOAST_TYPE4      "ToastText04"
#define WNSXML_TOAST_TYPE5      "ToastImageAndText01"
#define WNSXML_TOAST_TYPE6      "ToastImageAndText02"
#define WNSXML_TOAST_TYPE7      "ToastImageAndText03"
#define WNSXML_TOAST_TYPE8      "ToastImageAndText04"

typedef enum _MMIWNSXML_TOAST_TYPE_ID_E
{                          
    WNSXML_TOAST_ID_TYPE_UNKOWN = 0,
    WNSXML_TOAST_ID_TYPE1,    
    WNSXML_TOAST_ID_TYPE2,
    WNSXML_TOAST_ID_TYPE3,
    WNSXML_TOAST_ID_TYPE4,
    WNSXML_TOAST_ID_TYPE5,
    WNSXML_TOAST_ID_TYPE6,
    WNSXML_TOAST_ID_TYPE7,
    WNSXML_TOAST_ID_TYPE8,
    WNSXML_TOAST_ID_TYPE_MAX    
} MMIWNSXML_TOAST_TYPE_ID_E;


#define WNSXML_ELEMENT_TOAST      "toast"                  //<toast></toast>
#define WNSXML_ELEMENT_VISUAL      "visual"                 //<visual></visual>
#define WNSXML_ELEMENT_BINGDING      "binding"                  //<binding></binding>
#define WNSXML_ELEMENT_IMAGE      "image"                  //<image></image>
#define WNSXML_ELEMENT_TEXT      "text"                  //<text></text>


typedef enum _MMIWNSXML_ELEMENT_ID_E
{                          
    WNSXML_ELEMENT_ID_UNUSE = 0, 
    
    //                       
    WNSXML_ELEMENT_ID_TOAST,      
    WNSXML_ELEMENT_ID_VISUAL,      
    WNSXML_ELEMENT_ID_BINGDING,      
    WNSXML_ELEMENT_ID_IMAGE,      
    WNSXML_ELEMENT_ID_TEXT,      
    
    WNSXML_ELEMENT_ID_MAX    
} MMIWNSXML_ELEMENT_ID_E;

#define  WNSXML_ATTRIBUTE_TEMPLATE   "template"
#define  WNSXML_ATTRIBUTE_ID                  "id"
#define  WNSXML_ATTRIBUTE_SRC               "src"
#define  WNSXML_ATTRIBUTE_ALT                "alt"

typedef enum
{
    WNSXML_ATTRIBUTE_ID_UNKNOWN,
    WNSXML_ATTRIBUTE_ID_TEMPLATE,
    WNSXML_ATTRIBUTE_ID_ID,
    WNSXML_ATTRIBUTE_ID_SRC,
    WNSXML_ATTRIBUTE_ID_ALT,
    
    WNSXML_ATTRIBUTE_ID_MAX
}MMIWNSXML_ATTRIBUTE_ID_E;

typedef enum
{
    WNSXML_TEXT_ID_UNKNOWN,
    WNSXML_TEXT_ID_1,
    WNSXML_TEXT_ID_2,
    WNSXML_TEXT_ID_3,
    
    WNSXML_TEXT_ID_MAX
}MMIWNSXML_TEXT_ID_E;

typedef struct pns_notification_xml_tag
{
    int depth;
    MMIWNSXML_ELEMENT_ID_E cur_element_id;
    MMIWNSXML_TEXT_ID_E cur_text_id;
    MMIWNSXML_TOAST_TYPE_ID_E toast_type_id;
    char *text_1;
    char *text_2;
    char *text_3;
}PNS_NOTIFICATION_XML;



typedef enum APP_STATE_E {
    /* the specific application is not found or not installed */
    STATE_APP_NOT_FOUND = 0,
    /* no application is running */
    STATE_NO_APP_RUNNING,
    /* other applicaiton is running, but specific application is in down state */
    STATE_RUNNING_OTHER_APP,
    /* the specific application is running in background */
    STATE_RUNNING_IN_BG,
    /* the specific application is running in foreground*/
    STATE_RUNNING_IN_FG,
    STATE_MAX
} APP_STATE;


/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
PNS_NOTIFICATION_XML g_pns_xml_user_data = {0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
LOCAL void    *expat_Malloc(size_t s);
LOCAL void    *expat_realloc(void *p, size_t ns);
LOCAL void    expat_Free(void *p);
size_t  expat_AllocationSize(void *p);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
LOCAL void *expat_Malloc(size_t s)
{
    char *p = PNULL; 
    
    if(s == 0)
    {
        return PNULL;
    }

    //SCI_TRACE_LOW("dcd_xml_parser:expat_Malloc, size=%d", s);
    p = (char *)SCI_ALLOC_APP(s+8);

    if( p != PNULL )
    {
        *(size_t *)p = s;
        p += 8;
    }    


    return (void *)p;
}
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
LOCAL void *expat_realloc(void *p, size_t ns)
{
    char      *p2 = PNULL;
    size_t     len = 0;

    if(ns == 0)
    {
        return PNULL;
    }
#ifdef WIN32
    SCI_TRACE_LOW(("[wns]:expat_realloc(), new size=%d", ns));  /*lint !e64*/
#endif
    if (PNULL == p)
    {
        p2 = expat_Malloc(ns);
    }
    else
    {
    p2 = ((char *)p - 8);
    len = expat_AllocationSize(p);


    SCI_ASSERT(len>0);  /*assert verified*/
    SCI_ASSERT(ns>=len);  /*assert verified*/
    p2 = (char*)SCI_ALLOC_APP(ns+8);
    SCI_ASSERT( PNULL != p2 ); /*assert verified*/
    SCI_MEMSET( p2, 0, (ns+8) );
    *(size_t *)p2 = ns;
    p2 += 8;
    SCI_MEMCPY(p2,p,len);
    expat_Free(p);


    }
    return (void *)p2;
}
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
LOCAL void expat_Free(void *p)
{
    void *rp = PNULL;

    if (PNULL != p)
    {
        rp = (void *)(((char*)p)-8);
        SCI_FREE(rp);
    }

}

LOCAL size_t expat_AllocationSize(void *p)
{
    return (PNULL != p ? *(size_t *)((char *)p - 8) : 0);
}
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
LOCAL const XML_Memory_Handling_Suite memsuite =
{
    expat_Malloc,
    expat_realloc,
    expat_Free
};


/*****************************************************************************/
//  Description :  the functioms above convert the string to enum
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMIWNSXML_ELEMENT_ID_E GetElementID(const char *name)
{
             
    if(!stricmp(name, WNSXML_ELEMENT_TOAST))          return      WNSXML_ELEMENT_ID_TOAST;                                                          
    if(!stricmp(name, WNSXML_ELEMENT_VISUAL))         return      WNSXML_ELEMENT_ID_VISUAL;                                                          
    if(!stricmp(name, WNSXML_ELEMENT_BINGDING))    return      WNSXML_ELEMENT_ID_BINGDING;                                                          
    if(!stricmp(name, WNSXML_ELEMENT_IMAGE))    return      WNSXML_ELEMENT_ID_IMAGE;                                                          
    if(!stricmp(name, WNSXML_ELEMENT_TEXT))             return      WNSXML_ELEMENT_ID_TEXT;                                                          

    return WNSXML_ELEMENT_ID_UNUSE;
}     

/*****************************************************************************/
//  Description :  the functioms above convert the string to enum
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMIWNSXML_ATTRIBUTE_ID_E GetAttributeID(const char *name)
{
    if(PNULL == name)
    {
        return WNSXML_ATTRIBUTE_ID_MAX;
    }
    
    if(!stricmp(name,WNSXML_ATTRIBUTE_TEMPLATE))  return WNSXML_ATTRIBUTE_ID_TEMPLATE;
    if(!stricmp(name,WNSXML_ATTRIBUTE_ID))                 return WNSXML_ATTRIBUTE_ID_ID;
    if(!stricmp(name,WNSXML_ATTRIBUTE_SRC))              return WNSXML_ATTRIBUTE_ID_SRC;
    if(!stricmp(name,WNSXML_ATTRIBUTE_ALT))              return WNSXML_ATTRIBUTE_ID_ALT;

    SCI_TRACE_LOW("[wns]:GetAttributeID() str='%s'", name);
    
    return WNSXML_ATTRIBUTE_ID_MAX;
}   

/*****************************************************************************/
//  Description :  the functioms above convert the string to enum
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMIWNSXML_TOAST_TYPE_ID_E GetToastTypeID(const char *name)
{
    if(PNULL == name)
    {
        return WNSXML_TOAST_ID_TYPE_MAX;
    }
    
    if(!stricmp(name,WNSXML_TOAST_TYPE1))          return WNSXML_TOAST_ID_TYPE1;
    if(!stricmp(name,WNSXML_TOAST_TYPE2))          return WNSXML_TOAST_ID_TYPE2;
    if(!stricmp(name,WNSXML_TOAST_TYPE3))          return WNSXML_TOAST_ID_TYPE3;
    if(!stricmp(name,WNSXML_TOAST_TYPE4))          return WNSXML_TOAST_ID_TYPE4;
    if(!stricmp(name,WNSXML_TOAST_TYPE5))          return WNSXML_TOAST_ID_TYPE5;
    if(!stricmp(name,WNSXML_TOAST_TYPE6))          return WNSXML_TOAST_ID_TYPE6;
    if(!stricmp(name,WNSXML_TOAST_TYPE7))          return WNSXML_TOAST_ID_TYPE7;
    if(!stricmp(name,WNSXML_TOAST_TYPE8))          return WNSXML_TOAST_ID_TYPE8;

    SCI_TRACE_LOW("[wns]:GetToastTypeID() str='%s'", name);
    
    return WNSXML_TOAST_ID_TYPE_MAX;
}   

/*****************************************************************************/
//  Description :   Reset global_meta
//  Global resource dependence : the entry exist
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UserDataReset(PNS_NOTIFICATION_XML *userdata_ptr)
{
    if(PNULL != userdata_ptr)
    {
        userdata_ptr->depth = 0;
        userdata_ptr->toast_type_id = WNSXML_TOAST_ID_TYPE_UNKOWN;
        if(PNULL !=userdata_ptr->text_1)
        {
            SCI_FREE(userdata_ptr->text_1);
            userdata_ptr->text_1 = PNULL;
        }
        if(PNULL !=userdata_ptr->text_2)
        {
            SCI_FREE(userdata_ptr->text_2);
            userdata_ptr->text_2 = PNULL;
        }
        if(PNULL !=userdata_ptr->text_3)
        {
            SCI_FREE(userdata_ptr->text_3);
            userdata_ptr->text_3 = PNULL;
        }
    }
}

/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL void XMLCALL TextDataHandler(void *data,const XML_Char *s,int len)
{
    PNS_NOTIFICATION_XML *userdata = (PNS_NOTIFICATION_XML*)data;
    
    if(PNULL != userdata)// 加上对传入参数的检查...
    {
        //对得到的char数据进行取舍，转化，保存。。。
        switch(userdata->cur_element_id)
        {
          case WNSXML_ELEMENT_ID_TEXT:
            if( len > 0 )//有数据，则需要处理，否则不处理
            {
                uint8*  buf_ptr = NULL;
                
                buf_ptr = (uint8*)SCI_ALLOC_APP(len + 1);
                if(PNULL != buf_ptr)
                {
                    SCI_MEMSET(buf_ptr, 0, len + 1);
                    SCI_MEMCPY(buf_ptr, (uint8*)s, len);
                    if(WNSXML_TEXT_ID_1 == userdata->cur_text_id)
                    {
                        if(PNULL != userdata->text_1)
                        {
                            int text1_len = strlen(userdata->text_1);
                            userdata->text_1 = SCI_REALLOC(userdata->text_1, text1_len + len + 1);
                            if(PNULL != userdata->text_1)
                            {
                                SCI_MEMCPY(userdata->text_1 + text1_len, buf_ptr, len);
                                userdata->text_1 [text1_len + len] = '\0';
                            }
                            else
                            {
                                SCI_TRACE_LOW("[wns]: TextDataHandler(), SCI_REALLOC failed, len(%d).", text1_len + len + 1);
                            }
                            
                            SCI_FREE(buf_ptr);
                        }
                        else
                        {
                            userdata->text_1 = buf_ptr;
                        }
                    }
                    else if(WNSXML_TEXT_ID_2 == userdata->cur_text_id)
                    {
                        if(PNULL != userdata->text_2)
                        {
                            int text2_len = strlen(userdata->text_2);
                            userdata->text_2 = SCI_REALLOC(userdata->text_2, text2_len + len + 1);
                            if(PNULL != userdata->text_2)
                            {
                                SCI_MEMCPY(userdata->text_2 + text2_len, buf_ptr, len);
                                userdata->text_2 [text2_len + len] = '\0';
                            }
                            else
                            {
                                SCI_TRACE_LOW("[wns]: TextDataHandler(), SCI_REALLOC failed, len(%d).", text2_len + len + 1);
                            }
                            
                            SCI_FREE(buf_ptr);
                        }
                        else
                        {
                            userdata->text_2 = buf_ptr;
                        }
                    }
                    else if(WNSXML_TEXT_ID_3 == userdata->cur_text_id)
                    {
                        if(PNULL != userdata->text_3)
                        {
                            int text3_len = strlen(userdata->text_3);
                            userdata->text_3 = SCI_REALLOC(userdata->text_3, text3_len + len + 1);
                            if(PNULL != userdata->text_3)
                            {
                                SCI_MEMCPY(userdata->text_3 + text3_len, buf_ptr, len);
                                userdata->text_3 [text3_len + len] = '\0';
                            }
                            else
                            {
                                SCI_TRACE_LOW("[wns]: TextDataHandler(), SCI_REALLOC failed, len(%d).", text3_len + len + 1);
                            }
                            
                            SCI_FREE(buf_ptr);
                        }
                        else
                        {
                            userdata->text_3 = buf_ptr;
                        }
                    }
                    else
                    {
                        SCI_FREE(buf_ptr);
                    }
                }
            }
            break;
            
          case WNSXML_ELEMENT_ID_MAX:
            SCI_TRACE_LOW("[wns]: XML INVALID  MAX ID Parser.");
            break;
            
          default:
            break;
        }
    }
    
    return;
}  





/*****************************************************************************/
//  Description :  set the attributes for meta_generator
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetAttribute(PNS_NOTIFICATION_XML *userdata, 
                                        const char **attr_pptr
                                        )
{    
    BOOLEAN ret = TRUE;
    uint16 i    = 0;
    MMIWNSXML_ATTRIBUTE_ID_E attr_id = WNSXML_ATTRIBUTE_ID_MAX;

    if(PNULL == userdata)
    {
        return FALSE;
    }

    for(i=0; PNULL != attr_pptr[i]; i+=2)
    {
        attr_id = GetAttributeID(attr_pptr[i]);
        switch(attr_id)
        {
            case WNSXML_ATTRIBUTE_ID_TEMPLATE:
                userdata->toast_type_id = GetToastTypeID(attr_pptr[i + 1]);
                break;

            case WNSXML_ATTRIBUTE_ID_ID:
                if(WNSXML_ELEMENT_ID_TEXT == userdata->cur_element_id)
                {
                     if(!stricmp(attr_pptr[i + 1],"1"))  userdata->cur_text_id = WNSXML_TEXT_ID_1;
                     if(!stricmp(attr_pptr[i + 1],"2"))  userdata->cur_text_id = WNSXML_TEXT_ID_2;
                     if(!stricmp(attr_pptr[i + 1],"3"))  userdata->cur_text_id = WNSXML_TEXT_ID_3;
                }
                else if(WNSXML_ELEMENT_ID_IMAGE == userdata->cur_element_id)
                {

                }

                break;
           
             case WNSXML_ATTRIBUTE_ID_SRC:
                break;

            case WNSXML_ATTRIBUTE_ID_ALT:
                break;
           
            default:
                ret = FALSE;
                break;                                                  
        }
    }
    return   ret;
}

/****************************************************************************/
//  Description: StartElementHandler
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
LOCAL void XMLCALL StartElementHandler(
                                        void *data,
                                        const XML_Char *name,
                                        const XML_Char **attrs
                                      )
{
    PNS_NOTIFICATION_XML* userdata = (PNS_NOTIFICATION_XML*) data;
    
    //设置当前分析元素
    userdata->cur_element_id = GetElementID(name);
  
    //<内部处理>构建&设置相关属性
    switch(userdata->cur_element_id)
    {
    case WNSXML_ELEMENT_ID_TOAST:
    case WNSXML_ELEMENT_ID_VISUAL:
        break;

    case WNSXML_ELEMENT_ID_BINGDING:
    case WNSXML_ELEMENT_ID_IMAGE:
    case WNSXML_ELEMENT_ID_TEXT:
        SetAttribute(userdata, attrs);
        break;     


    case WNSXML_ELEMENT_ID_MAX:
        SCI_TRACE_LOW("[wns]: XML INVALID  MAX ID Parser.");
        break;

    default:
        SCI_TRACE_LOW("[wns]: XML INVALID  Parser");
        break;
            
    }
}

/****************************************************************************/
//  Description: StartElementHandler
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
LOCAL void XMLCALL EndElementHandler(void *data, const XML_Char *name)
{
    PNS_NOTIFICATION_XML* userdata = (PNS_NOTIFICATION_XML*)data; 

    MMIWNSXML_ELEMENT_ID_E end_element_id = GetElementID(name);

    if(PNULL == userdata)
    {
        return;
    }

    userdata->cur_element_id = WNSXML_ELEMENT_ID_UNUSE;

    switch(end_element_id)
    {
    case WNSXML_ELEMENT_ID_TOAST:
    case WNSXML_ELEMENT_ID_VISUAL:
        break;

    case WNSXML_ELEMENT_ID_IMAGE:
        break;
        
    case WNSXML_ELEMENT_ID_TEXT:
        userdata->cur_text_id = WNSXML_TEXT_ID_MAX;
        break;

    default:
        break;
    }

    userdata->depth -= 1;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:grant.peng
//  Note:
/*****************************************************************************/
PUBLIC PNS_NOTIFICATION_XML *MMIWNS_GetXmlData(void)
{
    return &g_pns_xml_user_data;
}

/****************************************************************************/
//  Description: Parses a XML buffer
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMIWNS_XmlParse(const char *xml_ptr,uint32 len)
{
    BOOLEAN           ret = TRUE;
    uint32            charsremain = 0;    
    uint32            charsread = 0;
    const char*            readptr = PNULL;    
    XML_Parser        parser = PNULL;
    int               done = 0;
    char              buf[BUFSIZ];    
    
    SCI_TRACE_LOW("[wns]:MMIWNS_XmlParse() enter");
    if( PNULL == xml_ptr)
    {
        return FALSE;
    }
    
    charsremain = len;
    readptr = xml_ptr;
    
    UserDataReset(&g_pns_xml_user_data);
    
    parser = XML_ParserCreate_MM(NULL, &memsuite, NULL);
    if (!parser) 
    {
        SCI_TRACE_LOW("[wns]:create xml parser fail");
        return FALSE;
    }
    
    
    //===
    // 注册回调函数
    //  设置用户数据，如果需要传给处理函数附加的信息，那就使用它，不然就不用
    XML_SetUserData(parser,&g_pns_xml_user_data);
    
    //  如果是utf-8 us-ascii ucs2 iso8859-1 那不用使用下面这个函数，
    //  不然要使用，不使用会报字符集不支持的错误,
    //  读xml头部时会调用到<?xml version="1.0" encoding="gb2312" ?>
    //XML_SetUnknownEncodingHandler(parser,EncodingHandle,0);
    
    //  设置事件处理函数,当碰到开始元素和结可元素时调用哪个函数来处理
    XML_SetElementHandler(parser,StartElementHandler,EndElementHandler);
    
    
    //  设置当碰到数据时的处理函数
    //  如<img> "hello.gif"</img> 那么碰到img时会调用startElementHandler,
    //  读完"hello.gif"后会调用characterDataHandler,碰到/img后会调用endElementHandler
    XML_SetCharacterDataHandler(parser,TextDataHandler);
    
    
    // 开始解析
    do
    {
        // 同步读文件
        SCI_MEMSET(buf, 0, BUFSIZ);
        //ffs_error = SFS_ReadFile(xml_file_stream,buf,sizeof(buf),&charsread,PNULL);
        readptr += charsread;
        if(charsremain > BUFSIZ)
        {
            charsread = BUFSIZ;
            charsremain -= charsread;
            SCI_MEMCPY(buf, readptr, charsread);
        }
        else
        {
            charsread = charsremain;
            charsremain -= charsread;
            SCI_MEMCPY(buf, readptr, charsread);
        }
        
        if(  (0 < charsread))
        {
            done = charsread < sizeof(buf);
            if( XML_STATUS_ERROR == XML_Parse(parser, buf, charsread, done) )
            {
                SCI_TRACE_LOW("[wns]:MMIWNS_XmlParse():Parse error.");
                ret = FALSE;
                break;
            }
        }
    } while(ret && (0 < charsread));
    //===
    
    XML_ParserFree(parser);        
    
    return ret;
}

/*****************************************************************************/
//     Description : get app show name
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
//       MIDlet-Name是J2ME规范中规定的应用的名称，
//                               就是应用在安装完成后会显示出来的名称。
//                               对用户而言是直观感受。
//      WNS Identifier是为WNS引用的一个property，
//                              作为一个特有的属性来标识这个应用唯一性。 
//                              对用户而言为不可见，用于JVM跟WNS之间的一种映射。
//      WNS identifier 就是create_push_notification_channel_for_applicationasync接口中的appname
//      MIDlet ID       是JVM内部使用，用来管理应用，
//                              包括删除，安装，运行等，应用本身并不知道它的存在。
/*****************************************************************************/
PUBLIC BOOLEAN MMIWNS_GetAppShowName(const char *appname, wchar *name_buf, int buf_len)
{
    BOOLEAN b_ret = FALSE;
    SCI_TRACE_LOW("[wns]:MMIWNS_GetAppShowName(): enter. appname(%s), buf len(%d).", appname, buf_len);
    if(PNULL != appname && PNULL != name_buf)
    {
        int suiteId = 0;
        int nameSize = 0;
        uint16 *midletName = PNULL;

#ifdef JAVA_SUPPORT_MYRIAD
        MMIJAVA_EnvironmentInit();
        
        suiteId = Jbed_nAms_getSuiteIdByWnsIdentifier(appname);
        if (suiteId < 0)
        {
            SCI_TRACE_LOW("[wns]:MMIWNS_GetAppShowName():Can't find corresponding midlet.");
        }
        else
        {
            nameSize = Jbed_nAms_querySuitesInfo(suiteId, NAMS_SI_NAME, (void
            *)&midletName);
            // if getting midletName is not NULL means you can use it.
            // it is wchar format and nameSize is the length of it in wchar.
            SCI_TRACE_LOW("[wns]:MMIWNS_GetAppShowName():nameSize(%d).", nameSize);

            if(PNULL != midletName)
            {
                if(nameSize > buf_len)
                {
                    nameSize = buf_len -1;
                }
                
                MMIAPICOM_Wstrncpy(name_buf, midletName, nameSize);
                b_ret = TRUE;
            }
            else
            {
                SCI_TRACE_LOW("[wns]:MMIWNS_GetAppShowName():midletName is NULL.");
            }
            SCI_FREEIF(midletName); // remember to free it.
        }
#endif //JAVA_SUPPORT_MYRIAD

    }
    return b_ret;
}



/*****************************************************************************/
//     Description : get app icon path which will show as idle notification
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWNS_GetAppIconPath(const char *appname, uint16 *icon_path_buf, uint16 path_buf_len)
{
    int pathLen = 0;
    BOOLEAN bRet = FALSE;
    uint16 *temp_icon_path = PNULL; 

    SCI_TRACE_LOW("[wns]:MMIWNS_GetAppIconPath(): enter. appname(%s). buf len(%d)", appname, path_buf_len);

#ifdef JAVA_SUPPORT_MYRIAD

    ///这里的内存在通知消息清除时释放。
    if(PNULL != appname && PNULL != icon_path_buf)
    {
        do
        {
            MMIJAVA_EnvironmentInit();
            
            pathLen = MMIJAVA_GetIconPathByWnsIdentifier(appname, &temp_icon_path);
            SCI_TRACE_LOW("[wns]:MMIWNS_GetAppIconPath(): MMIJAVA_GetIconPathByWnsIdentifier, pathLen(%d).", pathLen);
            if(0 >= pathLen)
            {
                SCI_TRACE_LOW("[wns]:MMIWNS_GetAppIconPath(): MMIJAVA_GetIconPathByWnsIdentifier failed.");
                bRet = FALSE;
                break;
            }
            if(pathLen > path_buf_len)
            {
                SCI_TRACE_LOW("[wns]:MMIWNS_GetAppIconPath(): the path is biger than buffer.");
                bRet = FALSE;
                break;
            }

            MMIAPICOM_Wstrncpy(icon_path_buf, temp_icon_path, pathLen);
            bRet = TRUE;
        }while(0);


       //释放MMIJAVA_GetIconPathByWnsIdentifier接口中申请的内存
        if(PNULL != temp_icon_path)
        {
            SCI_FREE(temp_icon_path);
            temp_icon_path = PNULL;
        }

    }
    
#endif //JAVA_SUPPORT_MYRIAD

    return bRet;
}


/*****************************************************************************/
//     Description : clear pns notification
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
LOCAL void MMIWNS_ClearNotification(void *param1, void *param2)
{
    char *appname = (char *)param1;
    SCI_TRACE_LOW("[wns]:MMIWNS_ClearNotification(): enter.");

    MMIWNS_ClearNotificationByAppName(appname);
}



/*****************************************************************************/
//     Description : enter app by idle notification
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC void MMIWNS_AppEntry(void *param1, void *param2)
{
    int ret = 0;
    char *wnsIdentifier = (char *)param1;
    SCI_TRACE_LOW("[wns]:MMIWNS_AppEntry(): enter.");

    if(PNULL != wnsIdentifier)
    {
        SCI_TRACE_LOW("[wns]:MMIWNS_AppEntry(): wnsIdentifier(%s).", wnsIdentifier);

#ifdef JAVA_SUPPORT_MYRIAD

        MMIJAVA_EnvironmentInit();
        
        ret = MMIJAVA_LaunchMIDletSuiteByWnsPushMessage(wnsIdentifier);
        
#endif //JAVA_SUPPORT_MYRIAD

        if(0 == ret)
        {
            MMIWNS_ClearNotification(param1, param2);
        }
        
        SCI_TRACE_LOW("[wns]:MMIWNS_AppEntry(): MMIJAVA_LaunchMIDletSuiteByWnsPushMessage result(%d).", ret);
    }
}

/*****************************************************************************/
//  Discription: Is wns application on front ground state
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWNS_IsAppOnFgState(const char *appname)
{
    BOOLEAN b_ret = FALSE;
    
    SCI_TRACE_LOW("[wns]:MMIWNS_IsAppOnFgState(): enter");
    
#ifdef JAVA_SUPPORT_MYRIAD

    //APP_STATE state = STATE_MAX;

    MMIJAVA_EnvironmentInit();
    
   // state = MMIJAVA_QueryWnsAppState(appname);
    if(STATE_RUNNING_IN_FG == MMIJAVA_QueryWnsAppState(appname))
    {
        SCI_TRACE_LOW("[wns]:MMIWNS_IsAppOnFgState(): application is in running front ground state");
        b_ret = TRUE;
    }
    
#endif //JAVA_SUPPORT_MYRIAD

    return b_ret;
}


/*****************************************************************************/
//     Description : show saved push notification on idle screen
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC void MMIWNS_ParsePushMsg(PNS_MGR_APP_INFO_T *app_info_ptr, int index)
{
    BOOLEAN b_ret = FALSE;
    MMI_STRING_T tmp_str = {0};
        
    SCI_TRACE_LOW("[wns]:MMIWNS_ParsePushMsg(): enter.");

    if(PNULL != app_info_ptr && index < app_info_ptr->push_notification_count)
    {
        IDLE_PUSH_MSG_INFO_T  idle_push_msg_info = {0};
        
        SCI_TRACE_LOW("[wns]:MMIWNS_ParsePushMsg(): index(%d), push_notification_count(%d).", index, app_info_ptr->push_notification_count);

        idle_push_msg_info.e_push_msg_type = MMI_IDLE_PUSH_MSG_WNS;

        //根据appname获取应用在Idle界面上通知显示的小图标
        b_ret = MMIWNS_GetAppIconPath(app_info_ptr->appname, idle_push_msg_info.icon_data.full_path_wstr_ptr, MMIIDLE_LIST_ICON_PATH_MAX_LEN);
        if(b_ret)
        {
            idle_push_msg_info.icon_data_type = MMI_IDLE_ICON_DATA_ANIM_PATH;
        }
        else
        {
            idle_push_msg_info.icon_data_type = MMI_IDLE_ICON_DATA_NONE;
        }

        //因为这里appname最大长度为WNS_APP_NAME_MAX_LEN 64
        //所以不用检查长度
        strcpy(idle_push_msg_info.appname, app_info_ptr->appname);
        
        b_ret = MMIWNS_GetAppShowName(app_info_ptr->appname, idle_push_msg_info.title, MMIIDLE_LIST_MAX_STRLEN + 1); 
        if(b_ret)
        {
            //根据appname获取Idle界面上通知进入应用的入口函数
#if 0
            idle_push_msg_info.app_entry_func_ptr = MMIWNS_AppEntry;
            idle_push_msg_info.app_left_focus_action_func_ptr = PNULL;
            idle_push_msg_info.app_right_focus_action_func_ptr = PNULL;
#else
            idle_push_msg_info.app_entry_e = IDLE_APP_ENTRY_WNS;
            idle_push_msg_info.app_left_focus_action_e = IDLE_APP_ENTRY_NULL;
            idle_push_msg_info.app_right_focus_action_e = IDLE_APP_ENTRY_NULL;
#endif
            if(WNS_TYPE_TOAST == app_info_ptr->push_notification_list[index].notification_type)
            {
                PNS_NOTIFICATION_XML *user_data = PNULL;
                int contents_len = strlen(app_info_ptr->push_notification_list[index].contents);
                contents_len = MIN(contents_len, CONTENT_MAX_LEN);

                if(MMIWNS_XmlParse(app_info_ptr->push_notification_list[index].contents, contents_len))
                {
                    user_data = MMIWNS_GetXmlData();
                    if(PNULL != user_data->text_1)
                    {
                        if(MMIIDLE_LIST_MAX_STRLEN < strlen(user_data->text_1))
                        {
                            user_data->text_1[MMIIDLE_LIST_MAX_STRLEN - 1] = '\0';
                        }
                        //MMIAPICOM_StrToWstr(user_data->text_1, idle_push_msg_info.contents); 
                        //GUI_UTF8ToWstr(wchar * wstr_ptr, uint32 wstr_len, const uint8 * utf8_ptr, uint32 utf8_len);
                        GUI_UTF8ToWstr(idle_push_msg_info.contents, MMIIDLE_LIST_MAX_STRLEN, user_data->text_1, contents_len);
                    }
                    else
                    {
                        MMI_GetLabelTextByLang(STR_ILIST_NEW_NOTIFICATION_JT, &tmp_str);
                        MMIAPICOM_Wstrcpy(idle_push_msg_info.contents, tmp_str.wstr_ptr); 
                    }
                }
                else
                {
                    MMI_GetLabelTextByLang(STR_ILIST_NEW_NOTIFICATION_JT, &tmp_str);
                    MMIAPICOM_Wstrcpy(idle_push_msg_info.contents, tmp_str.wstr_ptr); 
                }
                MMIIDLE_AppendPushMsg(&idle_push_msg_info);
            }
            else if(WNS_TYPE_RAW == app_info_ptr->push_notification_list[index].notification_type)
            {
                MMI_GetLabelTextByLang(STR_ILIST_NEW_NOTIFICATION_JT, &tmp_str);
                MMIAPICOM_Wstrcpy(idle_push_msg_info.contents, tmp_str.wstr_ptr); 
                MMIIDLE_AppendPushMsg(&idle_push_msg_info);
            }
            else
            {
                SCI_TRACE_LOW("[wns]:MMIWNS_ParsePushMsg(): Unsupported notification type.");
            }
        }
        
    }
}



