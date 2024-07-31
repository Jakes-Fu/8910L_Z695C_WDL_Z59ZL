
//flash type flash,maybe change in future
SFS_SET_IS_M8_FLASH("FALSE")

#ifdef JAVA_SUPPORT_IA
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
SFS_ADD_FOLDER("preload\\@java")
#endif
#endif

#if defined(NANDBOOT_SUPPORT) && defined(JAVA_SUPPORT_MYRIAD)
SFS_ADD_FOLDER("preload\\Java")
#endif

#ifdef FLASH_SUPPORT
SFS_ADD_FOLDER("preload\\APP")
SFS_ADD_FOLDER("preload\\FONT")
#endif

#ifdef ORANGE_SUPPORT
SFS_ADD_FOLDER("preload\\SSL_CERT")
#endif
#ifdef WRE_SUPPORT
SFS_ADD_FOLDER("preload\\wre")
#endif

#ifdef BROWSER_SUPPORT_WEBKIT
SFS_ADD_FOLDER("preload\\webkit")
#endif
