/*****************************************************************************
** File Name:      mmicom_trace.h                                            *
** Author:         chunjuan.liang                                            *
** Date:           2/20/2020                                                 *
** Copyright:                                                                *
** Description:    This file is used to define common part for king          *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2/2020       chunjuan.liang       Create
******************************************************************************/
#ifndef _MMICOM_TRACE_H_
#define _MMICOM_TRACE_H_

#ifndef WIN32
#define TRACE_APP_CAMERA(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "Camera", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)
#define TRACE_APP_SETTINGS(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "Settings", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)

#define TRACE_APP_LAUNCHER(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "Launcher", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)

#define TRACE_APP_ENGINEERMODE(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "Engineer Mode", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)

#define TRACE_APP_GPS(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "GPS", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)

#define TRACE_APP_MESSAGES(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "Messages", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)

#define TRACE_APP_VOICERECORDER(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "TRACE_APP_VoiceRecorder", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)

#define TRACE_APP_CALLCTRL(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "Call Ctrl", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)
#define TRACE_APP_VIDEOS(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "VIDEOS", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)
#define TRACE_APP_COMMON(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "Common", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)
#define TRACE_APP_GALLERY(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "Gallery", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)
#define TRACE_APP_STOPWATCH(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "StopWatch", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)
#define TRACE_APP_STARTUP(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "Startup", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)
#define TRACE_APP_WLAN(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "Wlan", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)
#else
#define TRACE_APP_CAMERA        SCI_TRACE_LOW
#define TRACE_APP_SETTINGS      SCI_TRACE_LOW
#define TRACE_APP_LAUNCHER      SCI_TRACE_LOW
#define TRACE_APP_ENGINEERMODE  SCI_TRACE_LOW
#define TRACE_APP_GPS           SCI_TRACE_LOW
#define TRACE_APP_MESSAGES      SCI_TRACE_LOW
#define TRACE_APP_VOICERECORDER SCI_TRACE_LOW
#define TRACE_APP_CALLCTRL      SCI_TRACE_LOW
#define TRACE_APP_STOPWATCH     SCI_TRACE_LOW
#define TRACE_APP_VIDEOS        SCI_TRACE_LOW
#define TRACE_APP_COMMON        SCI_TRACE_LOW
#define TRACE_APP_GALLERY       SCI_TRACE_LOW
#define TRACE_APP_STOPWATCH     SCI_TRACE_LOW
#define TRACE_APP_STARTUP       SCI_TRACE_LOW
#define TRACE_APP_WLAN          SCI_TRACE_LOW
#endif

#endif
