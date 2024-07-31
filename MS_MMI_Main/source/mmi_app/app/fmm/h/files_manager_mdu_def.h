

//[[define image infomation
//RES_ADD_IMG(IMAGE_FMM_FILE_SD_ICON,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_FILE_SD_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_FILE_UDISK_ICON,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_FILE_UDISK_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_FOLDER_ICON,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_FOLDER_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_FMM_NORMAL_SD_ICON,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_NORMAL_SD_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_FMM_NORMAL_UDISK_ICON,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_NORMAL_UDISK_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_FMM_FOLDER_2L_ICON,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_FOLDER_2L_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_FMM_FOLDER,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_FOLDER.png",IMG_CMP_PNG_ARGB_RLE,0)

/* add for file explorer. Start*/
RES_ADD_IMG(IMAGE_FMM_FOLDER,"\\MMI_RES_##\\IMAG\\IMAGE_FILE_EXPLORER_FOLDER.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_MEMORY_CARD,"\\MMI_RES_##\\IMAG\\IMAGE_MEMORYCARD.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_PHONE_MEMORY,"\\MMI_RES_##\\IMAG\\IMAGE_PHONEMEMORY.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_COPY,"\\MMI_RES_##\\IMAG\\IMAGE_COMMON_COPY.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_MOVE,"\\MMI_RES_##\\IMAG\\IMAGE_COMMON_MOVE.png",IMG_CMP_PNG_ARGB_RLE,0)
/* End*/

#ifdef MAINLCD_SIZE_320X480
//FMM TAB FUCUS IMAGE
RES_ADD_IMG(IMAGE_FMM_UDISK_FOCUSED,"\\MMI_RES_##\\IMAG\\tab\\IMAGE_FMM_UDISK_FOCUSED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_SDCARD_FOCUSED,"\\MMI_RES_##\\IMAG\\tab\\IMAGE_FMM_SDCARD_FOCUSED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_SDCARD1_FOCUSED,"\\MMI_RES_##\\IMAG\\tab\\IMAGE_FMM_SDCARD1_FOCUSED.png",IMG_CMP_PNG_ARGB_RLE,0)

//FMM TAB RELEASED IMAGE
RES_ADD_IMG(IMAGE_FMM_UDISK_RELEASED,"\\MMI_RES_##\\IMAG\\tab\\IMAGE_FMM_UDISK_RELEASED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_SDCARD_RELEASED,"\\MMI_RES_##\\IMAG\\tab\\IMAGE_FMM_SDCARD_RELEASED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_SDCARD1_RELEASED,"\\MMI_RES_##\\IMAG\\tab\\IMAGE_FMM_SDCARD1_RELEASED.png",IMG_CMP_PNG_ARGB_RLE,0)
#else
//FMM TAB FUCUS IMAGE 320x480以外的尺寸，加载的tab键都不会显示，这里加载保证编译通过
RES_ADD_IMG(IMAGE_FMM_UDISK_FOCUSED,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_UDISK_FOCUSED.png",IMG_CMP_PNG_ARGB_RLE,0)
#ifndef DUAL_TCARD_SUPPORT
RES_ADD_IMG(IMAGE_FMM_SDCARD_FOCUSED,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_SDCARD_FOCUSED.png",IMG_CMP_PNG_ARGB_RLE,0)
#else
RES_ADD_IMG(IMAGE_FMM_SDCARD0_FOCUSED,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_SDCARD0_FOCUSED.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
RES_ADD_IMG(IMAGE_FMM_SDCARD1_FOCUSED,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_SDCARD1_FOCUSED.png",IMG_CMP_PNG_ARGB_RLE,0)

//FMM TAB RELEASED IMAGE
RES_ADD_IMG(IMAGE_FMM_UDISK_RELEASED,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_UDISK_RELEASED.png",IMG_CMP_PNG_ARGB_RLE,0)
#ifndef DUAL_TCARD_SUPPORT
RES_ADD_IMG(IMAGE_FMM_SDCARD_RELEASED,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_SDCARD_RELEASED.png",IMG_CMP_PNG_ARGB_RLE,0)
#else
RES_ADD_IMG(IMAGE_FMM_SDCARD0_RELEASED,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_SDCARD0_RELEASED.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
RES_ADD_IMG(IMAGE_FMM_SDCARD1_RELEASED,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_SDCARD1_RELEASED.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif

#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
//FMM TAB FUCUS IMAGE
RES_ADD_IMG(ISTYLE_IMAGE_FMM_UDISK_FOCUSED,"\\MMI_RES_##\\IMAG\\istyle\\ISTYLE_IMAGE_FMM_UDISK_FOCUSED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(ISTYLE_IMAGE_FMM_SDCARD_FOCUSED,"\\MMI_RES_##\\IMAG\\istyle\\ISTYLE_IMAGE_FMM_SDCARD_FOCUSED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(ISTYLE_IMAGE_FMM_SDCARD1_FOCUSED,"\\MMI_RES_##\\IMAG\\istyle\\ISTYLE_IMAGE_FMM_SDCARD1_FOCUSED.png",IMG_CMP_PNG_ARGB_RLE,0)

//FMM TAB RELEASED IMAGE
RES_ADD_IMG(ISTYLE_IMAGE_FMM_UDISK_RELEASED,"\\MMI_RES_##\\IMAG\\istyle\\ISTYLE_IMAGE_FMM_UDISK_RELEASED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(ISTYLE_IMAGE_FMM_SDCARD_RELEASED,"\\MMI_RES_##\\IMAG\\istyle\\ISTYLE_IMAGE_FMM_SDCARD_RELEASED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(ISTYLE_IMAGE_FMM_SDCARD1_RELEASED,"\\MMI_RES_##\\IMAG\\istyle\\ISTYLE_IMAGE_FMM_SDCARD1_RELEASED.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif

//pic prew icon
#if  (defined(MMI_PDA_SUPPORT) || !defined( MMI_BROWSER_MINI_SUPPORT)) && defined MMI_ISTYLE_SUPPORT
RES_ADD_IMG(ISTYLE_IMAGE_FMM_PIC_PREVIEW_OPTION,"\\MMI_RES_##\\IMAG\\istyle\\ISTYLE_IMAGE_FMM_PIC_PREVIEW_OPTION.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(ISTYLE_IMAGE_FMM_PIC_PREVIEW_RETURN,"\\MMI_RES_##\\IMAG\\istyle\\ISTYLE_IMAGE_FMM_PIC_PREVIEW_RETURN.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
/*
RES_ADD_IMG(IMAGE_FMM_PNG,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_PNG.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_BMP,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_BMP.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_GIF,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_GIF.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_JPEG,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_JPEG.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_JPG,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_JPG.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_WBMP,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_WBMP.png",IMG_CMP_PNG_ARGB_RLE,0)

RES_ADD_IMG(IMAGE_FMM_WMA,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_WMA.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_AAC,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_AAC.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_AMR,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_AMR.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_MID,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_MID.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_MIDI,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_MIDI.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_MP3,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_MP3.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_MP4,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_MP4.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_M4A,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_M4A.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_LRC,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_LRC.png",IMG_CMP_PNG_ARGB_RLE,0)


RES_ADD_IMG(IMAGE_FMM_3GP,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_3GP.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_AVI,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_AVI.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_FLV,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_FLV.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_MTV,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_MTV.png",IMG_CMP_PNG_ARGB_RLE,0)

RES_ADD_IMG(IMAGE_FMM_HTM,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_HTM.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_HTML,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_HTML.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_XML,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_XML.png",IMG_CMP_PNG_ARGB_RLE,0)

RES_ADD_IMG(IMAGE_FMM_TXT,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_TXT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_VCF,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_VCF.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_WAV,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_WAV.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_JAD,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_JAD.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_JAR,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_JAR.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_LIB,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_LIB.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FMM_SZIP,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_SZIP.png",IMG_CMP_PNG_ARGB_RLE,0)
*/
#ifdef MMI_PDA_SUPPORT
RES_ADD_IMG(IMG_FMM_NEW_FOLDER_MENU_ICON,"\\MMI_RES_##\\IMAG\\IMG_FMM_NEW_FOLDER_MENU_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_FMM_PHONE_FOCUSED,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_PHONE_FOCUSED.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMAGE_FMM_PHONE_RELEASE,"\\MMI_RES_##\\IMAG\\IMAGE_FMM_PHONE_RELEASE.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif

//DC do not support PNG photo
#if defined(CAMERA_SUPPORT)
#ifndef MMI_RES_LOW_COST
    RES_ADD_IMG(IMG_SD_TIP,"\\MMI_RES_##\\IMAG\\disk\\IMG_SD_TIP.bmp",IMG_CMP_BMP_16_565_RLE,0)
#endif    
    #if defined(DUAL_TCARD_SUPPORT)
        #ifndef MMI_RES_LOW_COST
        	RES_ADD_IMG(IMG_SD2_TIP,"\\MMI_RES_##\\IMAG\\disk\\IMG_SD2_TIP.bmp",IMG_CMP_BMP_16_565_RLE,0)
        #endif
    #endif
#ifndef MMI_RES_LOW_COST    
    RES_ADD_IMG(IMG_UDISK_TIP,"\\MMI_RES_##\\IMAG\\disk\\IMG_UDISK_TIP.bmp",IMG_CMP_BMP_16_565_RLE,0)
#endif

    #if defined(MMIDC_DESKTOP_ICON_BAR)
        RES_ADD_IMG(IMG_ICON_SD,"\\MMI_RES_##\\IMAG\\disk\\IMG_ICON_SD.bmp",IMG_CMP_BMP_16_565_RLE,0)
        #if defined(DUAL_TCARD_SUPPORT)
            RES_ADD_IMG(IMG_ICON_SD2,"\\MMI_RES_##\\IMAG\\disk\\IMG_ICON_SD2.bmp",IMG_CMP_BMP_16_565_RLE,0)
        #endif
        RES_ADD_IMG(IMG_ICON_UDISK,"\\MMI_RES_##\\IMAG\\disk\\IMG_ICON_UDISK.bmp",IMG_CMP_BMP_16_565_RLE,0)
        RES_ADD_IMG(IMG_ICON_NO_STORE,"\\MMI_RES_##\\IMAG\\disk\\IMG_ICON_NO_STORE.bmp",IMG_CMP_BMP_16_565_RLE,0)
    #endif //#if defined(MMIDC_DESKTOP_ICON_BAR)
#endif //#if defined(CAMERA_SUPPORT)


//]]define image infomation


//[[define anim infomation
//]]define anim infomation


//[[define string infomation
#if 1
#ifdef TCARD_PB_SUPPORT
    RES_ADD_STRING(TXT_FMM_FORMAT_TCARD_CONTACT_ALERT,"Format will lost the contact imformation store in tcard, continue?")
#endif    
//RES_ADD_STRING(TXT_FMM_OPEN,"Open")  //replaced by common ID TXT_COMM_OPEN
RES_ADD_STRING(TXT_FMM_NEW_FOLDER,"New folder")
//RES_ADD_STRING(TXT_FMM_RENAME,"Rename")  //replaced by common ID TXT_COMMON_RENAME
//RES_ADD_STRING(TXT_FMM_COPY,"Copy")         //replaced by common ID TXT_COMMON_COPY
//RES_ADD_STRING(TXT_FMM_MOVE,"Move")         //replaced by common ID TXT_COMMON_CUT
//RES_ADD_STRING(TXT_FMM_SORT,"Sort")         //replaced by common ID TXT_COMMON_SORT
//RES_ADD_STRING(TXT_FMM_SEARCH,"Search")  //replaced by common ID TXT_COMM_SEARCH
//RES_ADD_STRING(TXT_FMM_MARK,"Mark") //replaced by common ID STXT_SOFTKEY_MARK_MK
//RES_ADD_STRING(TXT_FMM_MARK_ALL,"Mark all") //replaced by common TXT_MARK_ALL
//RES_ADD_STRING(TXT_FMM_SEND,"Send")  //replaced by common ID TXT_SEND
//RES_ADD_STRING(TXT_FMM_RING,"Set as ring")        //replaced by common ID TXT_COMMON_SET_CALL_RING
//RES_ADD_STRING(TXT_FMM_WALLPAPER,"Wallpaper")     //replaced by common ID TXT_COMMON_SETAS_WALLPAPER
//RES_ADD_STRING(TXT_FMM_MMS,"MMS")  //replaced by common ID TXT_MMS
//RES_ADD_STRING(TXT_FMM_BLUETOOTH,"Bluetooth")     //replaced by common ID TXT_BLUETOOTH
//RES_ADD_STRING(TXT_FMM_NAME,"Name")  //replaced by common ID TXT_COMMON_NAME
//RES_ADD_STRING(TXT_FMM_DATE,"Date")  //replaced by common ID TXT_TIME
//RES_ADD_STRING(TXT_FMM_SIZE,"Size")  //replaced by common ID TXT_COMMON_SIZE
//RES_ADD_STRING(TXT_FMM_TYPE,"Type")  //replaced by common ID TXT_COMMON_TYPE
//RES_ADD_STRING(TXT_FMM_CANCEL_MARK,"Cancel mark")     //replaced by common ID TXT_CANCEL_MARK
//RES_ADD_STRING(TXT_FMM_CANCEL_ALL_MARK,"Cancel all mark")     //replaced by common ID TXT_CANCEL_MARK_ALL
//RES_ADD_STRING(TXT_FMM_UDISK,"U Disk")       //case3：defined but not used
//RES_ADD_STRING(TXT_FMM_SDCARD,"SD card")  //replaced by common ID TXT_COMMON_SD_CARD
//RES_ADD_STRING(TXT_FMM_CHOOSE_PATH,"Choose path")      //case3：defined but not used
//RES_ADD_STRING(TXT_FMM_STOP,"Stop")         //case3：defined but not used
//RES_ADD_STRING(TXT_FMM_FILESYSTEM_ERROR,"Open file failed")      //case3：defined but not used
//RES_ADD_STRING(TXT_FMM_PICTURE,"Picture")       //case3：defined but not used
//RES_ADD_STRING(TXT_FMM_MUSIC,"Music")        //case3：defined but not used
//RES_ADD_STRING(TXT_FMM_MOVIE,"Movie")       //case3：defined but not used
//RES_ADD_STRING(TXT_FMM_CHOOSE_FILE,"Choose file")            //case3：defined but not used  
//RES_ADD_STRING(TXT_FMM_REPEAT_REPLACE,"Repeat, replace?")
//RES_ADD_STRING(TXT_FMM_MAX_DEPTH,"Max depth")    //replaced by common ID TXT_MAX_DEPTH
//RES_ADD_STRING(TXT_FMM_EXISTED_FOLDER,"Existed folder")    //replaced by common ID TXT_FOLDER_EXISTED
//RES_ADD_STRING(TXT_FMM_EXISTED_FILE,"Existed file")    //replaced by common ID TXT_FILE_EXISTED
//RES_ADD_STRING(TXT_FMM_DETAIL_DATE,"Date:")  //replaced by common ID TXT_COMMON_DATE
//RES_ADD_STRING(TXT_FMM_DETAIL_SIZE,"Size:")  //replaced by common ID TXT_DETAIL_SIZE
//RES_ADD_STRING(TXT_FMM_DETAIL_LOCATION,"Location:")    //replaced by common ID TXT_LOCATION  
//RES_ADD_STRING(TXT_FMM_DETAIL_RESOLUTION,"Resolution:")  //replaced by common ID  TXT_COMM_RESOLUTION
RES_ADD_STRING(TXT_FMM_FILE_DETAIL,"File detail")
//RES_ADD_STRING(TXT_FMM_FOLDER_DETAIL,"Folder detail")     //case3：defined but not used
RES_ADD_STRING(TXT_FMM_SORT_MANNER,"Manner")
RES_ADD_STRING(TXT_FMM_SUB_FOLDER,"Sub-Folder:")
RES_ADD_STRING(TXT_FMM_SUB_FILE_NUM,"Sub-File NUM:")
RES_ADD_STRING(TXT_FMM_CANNOT_COPY,"Can't copy")
//RES_ADD_STRING(TXT_FMM_MANAGE,"Manage")        //replaced by common ID TXT_COMMON_SETTING_AS
//RES_ADD_STRING(TXT_FMM_POWER_ON_ANIMATION,"Power On Animation")               //replaced by common ID TXT_COMMON_POWER_ON_ANIMATION
//RES_ADD_STRING(TXT_FMM_POWER_OFF_ANIMATION,"Power Off Animation")             //replaced by common ID TXT_COMMON_POWER_OFF_ANIMATION
//RES_ADD_STRING(TXT_SAVE_VCARD,"Save Vcard")       //case3：defined but not used
//RES_ADD_STRING(TXT_FMM_SCREENSAVE,"ScreenSave")     //replaced by common ID TXT_COMMON_SCREENSAVE
//RES_ADD_STRING(TXT_FMM_VIDEO_WALLPAPER,"Video wallpaper")    //replaced by common ID TXT_VIDEO_WALLPAPER
//RES_ADD_STRING(TXT_FMM_FILE_LIST,"File list")
//RES_ADD_STRING(TXT_FMM_NO_PIC_FILE,"No picture file")       //case3：defined but not used
//RES_ADD_STRING(TXT_FMM_INSTALL,"Install") //replaced by common ID TXT_COM_LOCALINSTALL
#ifdef DYNAMIC_MODULE_SUPPORT
RES_ADD_STRING(TXT_FMM_STORE_TYPE,"Store")

RES_ADD_STRING(TXT_FMM_NEED_SPACE,"Need space")
//RES_ADD_STRING(TXT_FMM_PHONE,"Phone")          //replaced by common ID TXT_COMMON_UDISK
//RES_ADD_STRING(TXT_FMM_CARD,"Card")       //case3：defined but not used
RES_ADD_STRING(TXT_FMM_FREE_SPACE,"Free")
RES_ADD_STRING(TXT_FMM_APP_NOT_VALID,"The application is not valid, install it?")
#endif
RES_ADD_STRING(TXT_FMM_SPACE_IS_NOT_ENOUGH,"Space is not enough")
//RES_ADD_STRING(TXT_FMM_INSTALL_FAILED,"Install Failed!") //replaced by common ID TXT_COMM_INSTALL_FAILED
#ifdef DYNAMIC_MODULE_SUPPORT
RES_ADD_STRING(TXT_FMM_INSTALL_OVERWRITE,"The file has been exist, overwrite?")
RES_ADD_STRING(TXT_FMM_INSTALL_SUCC,"Install success!")
#endif
//RES_ADD_STRING(TXT_FMM_DETAIL_TIME,"Time:")  //replaced by common ID TXT_COMMON_TIME
//RES_ADD_STRING(TXT_FMM_DETAIL_MEMORY,"Memory status")          //replaced by common ID TXT_COMMON_DETAIL_MEMORY
//RES_ADD_STRING(TXT_FMM_DETAIL_USED_SPACE,"Used:")             //case3：defined but not used
//RES_ADD_STRING(TXT_FMM_DETAIL_UNUSED_SPACE,"Unused:")      //case3：defined but not used
//RES_ADD_STRING(TXT_FMM_DETAIL_TOTAL_SPACE,"Total:")
RES_ADD_STRING(STR_MMC_FORMAT,"Memory card not support, suggest formatting it?")
//RES_ADD_STRING(TXT_FMM_IS_RENAME,"File exist, rename or not?")     //case3：defined but not used
RES_ADD_STRING(TXT_FMM_FILES_TOO_MANY,"Files and folders too many")
RES_ADD_STRING(TXT_FMM_RESERVE_FOLDER,"The system folder be left")
RES_ADD_STRING(TXT_COMMON_MORE,"More")    
RES_ADD_STRING(TXT_COMMON_UDISK_MEMORY,"U Disk capacity")
#ifndef DUAL_TCARD_SUPPORT
RES_ADD_STRING(TXT_COMMON_SD_MEMORY,"SD Card capacity")
#else
RES_ADD_STRING(TXT_COMMON_SD0_MEMORY,"SD Card1 capacity")
#endif
RES_ADD_STRING(TXT_COMMON_UDISK_FORMAT,"Format u disk")
#ifndef DUAL_TCARD_SUPPORT
RES_ADD_STRING(TXT_COMMON_SD_FORMAT,"Format SD card")
#else
RES_ADD_STRING(TXT_COMMON_SD0_FORMAT,"Format SD card1")
#endif
RES_ADD_STRING(TXT_FMM_NAME_DUPLICATE,"Name duplicate")
RES_ADD_STRING(TXT_UDISK_START,"Udisk started")
RES_ADD_STRING(TXT_UDISK_STOP,"Udisk Stopped")
RES_ADD_STRING(TXT_COMMON_SD1_MEMORY,"SD Card2 capacity")
RES_ADD_STRING(TXT_COMMON_SD1_FORMAT,"Format SD card2")
RES_ADD_STRING(TXT_COMMON_SD_RENAME,"Rename SD card")
RES_ADD_STRING(TXT_COMMON_SD0_RENAME,"Rename SD1 card")
RES_ADD_STRING(TXT_COMMON_SD1_RENAME,"Rename SD2 card")
RES_ADD_STRING(TXT_SD_NO_USED_SPACE,"SD not Used Space:")
RES_ADD_STRING(TXT_UDISK_NO_USED_SPACE,"Phone not Used Space:")
RES_ADD_STRING(TXT_COMMON_SD1_FREESPACE,"MemoryCard2 not used space:")
#ifdef MMI_PDA_SUPPORT
RES_ADD_STRING(TXT_PLS_SELECT_APPLICATION,"Please select application:")
#endif
//RES_ADD_STRING(TXT_FMM_MOVE_NOT_SUPPORT_BETWEEN_TWO_DEVICE,"Can't move between defferent device!")
RES_ADD_STRING(TXT_FMM_MOVE_SOURCE_EQU_DEST,"Can't move,Source path equal to dest path")

//]]define string infomation
#if defined ( MMI_PDA_SUPPORT ) ||defined( MMIPB_ADD_EXIST_CONTACT_SUPPORT )
RES_ADD_STRING(TXT_FMM_PHOTO_SET_PB,"Set Photo for PB")
#endif
//[[define data infomation
//]]define data infomation
//RES_ADD_STRING(TXT_FMM_SEND_BY_BT,"Send via bluetooth")      //case3：defined but not used
//RES_ADD_STRING(TXT_FMM_SEND_BY_MMS,"Send via MMS")          //case3：defined but not used
#ifndef MMI_MULTI_SIM_SYS_SINGLE
RES_ADD_STRING(TXT_FMM_SET_RING_SIM1_SELECT_CALL,"Set SIM1 call ring")

RES_ADD_STRING(TXT_FMM_SET_RING_SIM2_SELECT_CALL,"Set SIM2 call ring")
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
RES_ADD_STRING(TXT_FMM_SET_RING_SIM3_SELECT_CALL,"Set SIM3 call ring")
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
RES_ADD_STRING(TXT_FMM_SET_RING_SIM4_SELECT_CALL,"Set SIM14 call ring")
#endif
#endif
#ifdef MMI_PDA_SUPPORT
RES_ADD_STRING(TXT_FMM_SELECT_THIS_FOLDER,"Select This Folder")
#endif
RES_ADD_STRING(TXT_FMM_CC_IN_CALL_STATE_CANNOT_CHOOSE_MUSIC," In call state,can't select a music file")

//RES_ADD_STRING(TXT_FMM_PLS_SELECT_FOLDER,"Please Select a folder")
#if defined (BLUETOOTH_SUPPORT)
RES_ADD_STRING(TXT_FMM_SEND_RESELECT,"You can not select a folder or an empty file, please re-select")
#endif

/* add for file explorer. Start*/
//RES_ADD_STRING(TXT_FMM_MEMORY_CARD,"Memory card")
//RES_ADD_STRING(TXT_FMM_PHONE_MEMORY,"Phone memory")
RES_ADD_STRING(STR_NO_APP_TO_OPEN_EXT01,"There is no app to open this file.")
RES_ADD_STRING(STR_FILES_FOLDER_EMPTY_EXT01,"There is nothing here right now")
RES_ADD_STRING(STXT_SOFTKEY_RESTORE_MK,"Restore")
RES_ADD_STRING(STR_QUERY_DELETE_FOLDER_EXT01,"Delete this folder and all its content?")
RES_ADD_STRING(STR_NOTE_FOLDER_DELETED_EXT01,"Folder and all its content are deleted.")
RES_ADD_STRING(STR_NOTE_FOLDER_MOVED_EXT01,"Folder moved to")
RES_ADD_STRING(STR_NOTE_FILE_MOVED_EXT01,"File moved to")
RES_ADD_STRING(STR_FOLDER_MOVE_CANCELLED_EXT01,"Folder move cancelled.")
RES_ADD_STRING(STR_FILE_MOVE_CANCELLED_EXT01,"File move cancelled.")
RES_ADD_STRING(STXT_SOFTKEY_MOVE_TO_MK,"Move to")
RES_ADD_STRING(STR_FILES_WAIT_MOVING_EXT01,"Moving to")
RES_ADD_STRING(STR_FOLDER_EXISTING_EXT01,"Folder already exists in")
RES_ADD_STRING(STR_FILE_EXISTING_EXT01,"File already exists in")
RES_ADD_STRING(STXT_SOFTKEY_COPY_TO_MK,"Copy to")
RES_ADD_STRING(STR_FILES_WAIT_COPYING_EXT01,"Copying to")
RES_ADD_STRING(STR_NOTE_FOLDER_COPIED_EXT01,"Folder copied to")
RES_ADD_STRING(STR_NOTE_FILE_COPIED_EXT01,"File copied to")
RES_ADD_STRING(STR_FOLDER_COPY_CANCELLED_EXT01,"Folder copied cancelled.")
RES_ADD_STRING(STR_FILE_COPY_CANCELLED_EXT01,"File copied cancelled.")
RES_ADD_STRING(STR_FILES_NOT_ENOUGH_MEMORY_EXT01,"Not enough free memory at")
RES_ADD_STRING(STR_FILES_HDR_MOVE_TO_EXT01,"Move to")
RES_ADD_STRING(STR_FILES_HDR_COPY_TO_EXT01,"Copy to")
RES_ADD_STRING(STR_NOTE_FILES_DELETED_EXT01,"files deleted.")

RES_ADD_STRING(STR_GAL_LIST_REMOVE_FOLDER,"Delete folder")
RES_ADD_STRING(STR_FILES_DELETE_FOLDERS,"Delete folders")
RES_ADD_STRING(STR_GAL_LIST_DELETE_FILES_EXT01,"Delete files")
RES_ADD_STRING(STR_GAL_LIST_DELETE_FILE_EXT01,"Delete file")
RES_ADD_STRING(STR_GAL_LIST_MOVE_FOLDER,"Move folder")
RES_ADD_STRING(STR_GAL_LIST_COPY_FOLDER,"Copy folder")
RES_ADD_STRING(STR_GAL_LIST_RENAME_FOLDER_EXT01,"Rename folder")
RES_ADD_STRING(STR_GAL_LIST_ADD_FOLDER,"Add folder")
RES_ADD_STRING(STR_GAL_LIST_MOVE_TO,"Move")
RES_ADD_STRING(STR_GAL_LIST_COPY_TO,"Copy")
//RES_ADD_STRING(FMM_STR_GAL_LIST_RENAME_EX01,"Rename")// redefined. add fmm_
//RES_ADD_STRING(FMM_STR_GAL_LIST_DETAILS,"Details")// redefined. add fmm_
RES_ADD_STRING(STR_GAL_LIST_OPEN_FOLDER_EXT01,"Open folder")


//RES_ADD_STRING(STXT_SOFTKEY_DELETE_MK,"Delete")
//RES_ADD_STRING(STR_FILES_NAME_IN_USE_EXT01,"Name is already in use.Choose another name.")
//RES_ADD_STRING(STR_FILES_INVALID_CHARACTER_EXT01,"Invalid characters.Choose another name.")
RES_ADD_STRING(STR_GAL_LIST_CREATED,"Created:")
RES_ADD_STRING(STR_GAL_LAST_MODIFIED_EXT01,"Last modified:")
RES_ADD_STRING(STR_GAL_EXTENSION_EXT01,"Extension:")
RES_ADD_STRING(STR_GAL_FILE_TYPE_EXT01,"File type:")
RES_ADD_STRING(STR_GAL_PATH_EXT01,"Path:")
RES_ADD_STRING(STR_GAL_LIST_DURATION,"Length:")
RES_ADD_STRING(STR_GAL_HDR_FILES,"Files")
RES_ADD_STRING(STR_GAL_HDR_DETAILS_EXT01,"Details")
RES_ADD_STRING(STR_GAL_RESOLUTION_EXT01,"Resolution:")
RES_ADD_STRING(STR_GAL_HDR_DELETE_EXT02,"Delete")
RES_ADD_STRING(STR_GAL_HDR_DELETE_NUM_EXT01,"Delete (%N)")

RES_ADD_STRING(STR_PROMPT_FOLDER_NAME_EXT01,"Folder name")
RES_ADD_STRING(TXT_FMM_CREATE_NEW_FOLDER,"Folder")

RES_ADD_STRING(STR_HDR_MEMORY_CARD,"Memory Card")
RES_ADD_STRING(STR_INFO_HDR_PHONE,"Phone Memory")

//RES_ADD_STRING(STR_MCD_DTAIL_COD_NAME,"Name:")
//RES_ADD_STRING(STR_MCD_DTAIL_COD_SIZE,"Size:")


//RES_ADD_STRING(STR_LIST_IMAGE_EXT01,"Image")
RES_ADD_STRING(STR_LIST_VIDEO_EXT01,"Video")
RES_ADD_STRING(STR_LIST_AUDIO_EXT01,"Audio")
RES_ADD_STRING(STR_LIST_SOUND_EXT01,"Sound file")
RES_ADD_STRING(STR_LIST_BACKUP_FILE_EXT01,"Backup file")
RES_ADD_STRING(STR_FILE_OTHER_TYPE,"Other type")

RES_ADD_STRING(STR_LIST_FORMAT_EXT01,"Format")
RES_ADD_STRING(STR_FORMAT_QUERY_EXT01,"Format will delete all data on the memory card. Continue?")
RES_ADD_STRING(STR_FORMAT_PHONE_MEMORY,"Formatting will delete all data on the phone memory. Continue?")
RES_ADD_STRING(STR_FORMAT_WAIT_EXT01,"Formatting...")

RES_ADD_STRING(STR_BT_IMPORT_VCARD,"Load vcard")
RES_ADD_STRING(STR_BT_SAVE_VCALENDAR,"Save vCalendar")

RES_ADD_STRING(STR_BT_PHONE_SPACE,"Phone not used space:")
RES_ADD_STRING(STR_BT_MMC_SPACE,"SD not used space:")

RES_ADD_STRING(STR_QUERY_DELETE_FOLDERS,"Delete marked folders? It will also delete all content you have in these folders.")
RES_ADD_STRING(STR_QUERY_DELETE_FILES_FOLDERS,"Delete marked files and folders? It will also delete all content you have in these folders.")
RES_ADD_STRING(STR_NOTE_FILE_DELETED_EXT01,"1 file deleted.")
RES_ADD_STRING(STR_FOLDER_DELETED,"1 folder deleted.")
RES_ADD_STRING(STR_FOLDERS_DELETED,"%N folders deleted.")
RES_ADD_STRING(STR_FILE_FOLDER_DELETED,"1 file and 1 folders deleted.")
RES_ADD_STRING(STR_FILES_FOLDERS_DELETED,"%0N files and %1N folders deleted.")
RES_ADD_STRING(STR_FILES_FOLDER_DELETED,"%N files and 1 folder deleted")
RES_ADD_STRING(STR_FILE_FOLDERS_DELETED,"1 file and %N folders deleted")
/*End*/
// add
RES_ADD_STRING(TXT_FMM_RECORD_LIST_EMPTY,"You have not recorded anything yet.")
RES_ADD_STRING(TXT_FMM_RECORD_DELETE_ALL,"Delete all recordings?")
RES_ADD_STRING(TXT_FMM_SEND_BY_MMS,"Multi media message")
RES_ADD_STRING(TXT_FMM_SEND_BY_BLUETOOTH,"Slam via bluetooth")
RES_ADD_STRING(TXT_FMM_SEND_BY_EMAIL,"Email account")
RES_ADD_STRING(TXT_FMM_SEND_SHARE,"Share via")
RES_ADD_STRING(TXT_FMM_OPT_SHARE,"Share")
RES_ADD_STRING(TXT_FMM_OPT_RENAME,"Rename")
RES_ADD_STRING(TXT_FMM_OPT_DELETE_ALL,"Delete all")
//end
//longjunhao
RES_ADD_STRING(STR_QUERY_DELETE_ALL_REC_EXT01,"Delete all recordings?")
RES_ADD_STRING(STR_QUERY_DELETE_RECORDING_EXT01	,"Delete this recording?")
RES_ADD_STRING(STR_GAL_LIST_DELETE_ALL,"Delete All")
RES_ADD_STRING(STR_RECORDINGS_DELETED_EXT01,"Recordings deleted.")
RES_ADD_STRING(STR_RECORDINGS_FOLDER_EMPTY_EXT01,"You have not recorded anything yet.")
RES_ADD_STRING(STR_MENU_SINGLE_GAL_VOICE_MEMO,"Recordings")
//end
#endif
