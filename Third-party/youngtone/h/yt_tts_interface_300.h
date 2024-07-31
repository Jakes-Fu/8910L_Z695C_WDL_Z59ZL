#ifndef __YT_TTS_INTERFACE_300_HEADER__
#define __YT_TTS_INTERFACE_300_HEADER__


#include "yt_tts_socket_typedef.h"

///////////////////////////////////////////////////////////////////////////
// VERSION SECTION  START
// 2011/04/20~      V2.0


// VERSION SECTION  END
///////////////////////////////////////////////////////////////////////////


//LANGUAGE ID PART: START

#define YT_LANG_ID_RESERVED_300 0

#define YT_LANG_ID_MANDARIN_300 1 //Chinese Mandarin(Pu Tong hua)
//#define YT_LANG_ID_CANTONESE_300 2 //Cantonese(Guang Dong hua)

//#define YT_LANG_ID_US_ENGLISH_300 3//US English
//#define YT_LANG_ID_UK_ENGLISH_300 4//UK English

//#define YT_LANG_ID_FRENCH_300 5//French(Standard French/Continental French)

//#define YT_LANG_ID_SPANISH_300 7//Spanish/European Spanish
//#define YT_LANG_ID_AMERICAN_SPANISH_300 8//American Spanish

//#define YT_LANG_ID_BRAZILIAN_PORTUGUESE_300 10//Brazilian Portuguese

//#define YT_LANG_ID_GERMAN_300 11//German
//#define YT_LANG_ID_RUSSIAN_300 12//Russian
//#define YT_LANG_ID_ITALIAN_300 13//Italian

//#define YT_LANG_ID_INDONESIAN_300 21//Indonesian
//#define YT_LANG_ID_HINDI_300 22//Hindi language

//LANGUAGE ID PART: END
///////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////
//VOICE ID PART: START
#define YT_VOICE_ID_RESERVED_300 0
#define YT_VOICE_ID_FEMALE_300 1
#define YT_VOICE_ID_MALE_300 2
//VOICE ID PART: END
///////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////
//DATE FORMAT PART: START
#define YT_DATE_MM_DD_YYYY_300 0
#define YT_DATE_YYYY_MM_DD_300 1
#define YT_DATE_DD_MM_YYYY_300 2

//DATE FORMAT PART: END
///////////////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////////////////
//TEXT TYPE PART: START
#define YT_TEXT_TYPE_DEFAULT_300 0
#define YT_TEXT_TYPE_NAME_300 1
#define YT_TEXT_TYPE_NAME_APPENDIX_300 2
#define YT_TEXT_TYPE_MESSAGE_CONTENT_300 3
#define YT_TEXT_TYPE_MESSAGE_TITLE_300 4
#define YT_TEXT_TYPE_ICON_CAPTION_300 5
#define YT_TEXT_TYPE_DATE_300 6
#define YT_TEXT_TYPE_TIME_300 7
#define YT_TEXT_TYPE_TELEPHONE_NO_300 8
#define YT_TEXT_TYPE_DIGIT_300 9
#define YT_TEXT_TYPE_NUMBER_300 10
#define YT_TEXT_TYPE_HELP_300 11
#define YT_TEXT_TYPE_GPS_300 20
//TEXT TYPE PART: END
///////////////////////////////////////////////////////////////////////////


#define YT_TTS_MEM_SIZE_IN_BYTE 300*1024//200*1024




///////////////////////////////////////////////////////////////////////////
//API SPECIFICATIONS: START

#ifdef __cplusplus
extern "C"
{
#endif



//5 file I/O callback fuctions API definitions
int yt_tts_set_cb_fopen(int (*yt_fopen)(char *strFileName, char *strMode));
int yt_tts_set_cb_fread(int (*yt_fread)(void *buffer, size_t size, size_t count, int fid));
int yt_tts_set_cb_fseek(int (*yt_fseek)(int fid, long offset, int origin));
int yt_tts_set_cb_fwrite(int (*yt_fwrite)(void *buffer, size_t size, size_t count, int fid));
int yt_tts_set_cb_fclose(int (*yt_fclose)(int fid));


void yt_tts_set_file_one(char *strFileOne);
void yt_tts_set_file_two(char *strFileTwo);


//for Linux-style networking routines
int yt_tts_set_cb_gethostbyname(struct yt_hostent* (*yt_gethostbyname)(const char *strHostName));
int yt_tts_set_cb_socket(int (*yt_socket)(int af, int type, int protocol));
int yt_tts_set_cb_inet_addr(unsigned int (*yt_inet_addr)(const char *strPtr));
int yt_tts_set_cb_connect(int (*yt_connect)(int s, struct yt_sockaddr *name, yt_socklen_t namelen));
int yt_tts_set_cb_send(int (*yt_send)(int s, const void *dataptr, size_t size, int nTimeout));
int yt_tts_set_cb_recv(int (*yt_recv)(int s, void *mem, size_t len, int nTimeout));
int yt_tts_set_cb_closesocket(int (*yt_closesocket)(int s));




/*-------------------------------------------------------------------------------
 |yt_tts_set_cb_getchipinfo(...)
 |PURPOSE
 |	This API intends to set callback function for getting chipInfo
 |
 |INPUT
 |	yt_get_chip_info: a callback function for getting chip info.
 |              pBufferForChipInfo: maximum allowable 16-byte buffer for holding chip info
 |				pInfoSize: pointer to actual size of chip info 
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |  none
 |
 |CODING
 |	Shanghai YoungTone Tech.
 *-------------------------------------------------------------------------------*/

void yt_tts_set_cb_getchipinfo(void (*yt_get_chip_info)(char* pBufferForChipInfo, int *pInfoSize));




void yt_tts_set_cb_ucs2gb2312_300(unsigned char* (*yt_ucs2gb2312)(unsigned short ucs2, unsigned char *gb2312_ptr));


















/*-------------------------------------------------------------------------------
 |yt_tts_set_memory_buffer_300(...)
 |PURPOSE
 |	This API intends to set memory for TTS engine...
 |
 |BACKGROUND
 |	memory manangement is subtle under embedded OS. 
 |	So we avoid memory allocation if user can provide a global memory heap...
 |
 |INPUT
 |	pBufferForTTSEngine: buffer for memory
 |	nMemSize: memory size in byte, say YT_TTS_MEM_SIZE_IN_BYTE(300KB) defined in the header file
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |		It returns 0;
 |CODING
 |	YoungTone Inc.
 *-------------------------------------------------------------------------------*/
int yt_tts_set_memory_buffer_300(char *pBufferForTTSEngine,unsigned int nMemSize);




/*---------------------------------------------------------------------------------------
 |yt_tts_initialize_300(...)
 |PURPOSE
 |	This API intends to initialize TTS engine.
 |INPUT
 |	strReserved: reserved string variable...
 |	nLangID: language ID, which is defined in language ID section
 |	strArgOne: the first data argument, which can be a front-end file name or a data buffer
 |	nVoiceID: voice ID, which is defined in voice ID section
 |	strArgTwo: the second data argument, which can be a back-end file name or a data buffer
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	It returns 0 if successful; else it returns -1.
 |CODING
 |	YoungTone Inc.
 *--------------------------------------------------------------------------------------*/
int  yt_tts_initialize_300( char *strReserved,
							  int nLangID,
							  char *strArgOne, 
							  int nVoiceID, 
							  char *strArgTwo);
						    



/*-------------------------------------------------------------------------------
 |yt_tts_input_text_utf16_300(...)
 |PURPOSE
 |	This API intends to feed text in Unicode(UCS16) to TTS engine.
 |INPUT
 |	strText_U16: buffer for holding UTF16 text
 |	nTextLen_U16: number of character in strText_U16
 |	DATE_FORMAT: date format, which is defined in date format section
 |	nTextType: text type,which is defined in date format section. 
 |			    In general, user can set it to default value, 0
 |
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	It returns 0 if successful; else it returns -1.
 |
 |CODING
 |	YoungTone Inc.
 *-------------------------------------------------------------------------------*/
int  yt_tts_input_text_utf16_300(unsigned short *strText_U16,//[in]
							  unsigned int nTextLen_U16,//[in]
							  unsigned int DATE_FORMAT,
							  unsigned int nTextType);	
					

/*-------------------------------------------------------------------------------
 |yt_tts_input_text_mbcs_300(...)
 |PURPOSE
 |	This API intends to feed text in MBCS/ANSI to TTS engine.
 |INPUT
 |	strText: buffer for holding  text
 |	nTextLen: number of character in strText_U16
 |	DATE_FORMAT: date format, which is defined in date format section
 |	nTextType: text type,which is defined in date format section. 
 |			    In general, user can set it to default value, 0
 |
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	It returns 0 if successful; else it returns -1.
 |
 |CODING
 |	YoungTone Inc.
 *-------------------------------------------------------------------------------*/
int  yt_tts_input_text_mbcs_300(char *strText,//[in]
							  unsigned int nTextLen,//[in]
							  unsigned int DATE_FORMAT,
							  unsigned int nTextType);





/*-------------------------------------------------------------------------------
 |yt_tts_free_resource_300(...)
 |PURPOSE
 |	This API intends to free resources occupied by TTS engine
 |	in most cases.
 |
 |INPUT
 | none
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	none
 |
 |CODING
 |	YoungTone Inc.
 *-------------------------------------------------------------------------------*/
void yt_tts_free_resource_300();






/*-------------------------------------------------------------------------------
 |yt_tts_set_rate_300(...)
 |PURPOSE
 |	This API intends to set speech rate(speed).
 |
 |INPUT
 | nRateFactor: the target speech rate, which ranges 50~200. 
 |				100 is the normal rate. 50 is the fastest speed while 200 is the slowest one.
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	the acutal speech rate.
 |
 |CODING
 |	YoungTone Inc.
 *-------------------------------------------------------------------------------*/
int yt_tts_set_rate_300(int nRateFactor);





/*-------------------------------------------------------------------------------
 |yt_tts_set_pitch_300(...)
 |PURPOSE
 |	This API intends to set pitch.
 |
 |INPUT
 | nPitchFactor: the target pitch factor, which ranges 50~200. 
 |				100 is the normal pitch. 50 is the lowest pitch while 200 is the highest one.
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	the acutal pitch.
 |
 |CODING
 |	YoungTone Inc.
 *-------------------------------------------------------------------------------*/
int yt_tts_set_pitch_300(int nPitchFactor);




/*-------------------------------------------------------------------------------
 |yt_tts_set_volume_300(...)
 |PURPOSE
 |	This API intends to set volume.
 |
 |INPUT
 |nMaxVolume: the target volume factor, which ranges 0~32768. 
 |				32768 means the loudest speech while 0 means silence
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	the acutal volume
 |
 |CODING
 |	YoungTone Inc.
 *-------------------------------------------------------------------------------*/
int yt_tts_set_volume_300(int nMaxVolume);



/*-------------------------------------------------------------------------------
 |yt_tts_get_rate_300(...)
 |PURPOSE
 |	This API intends to get speech rate(speed).
 |
 |INPUT
 | none
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	the acutal speech rate, which ranges 50~200. 
 |	100 is the normal rate. 50 is the fastest speed while 200 is the slowest one.
 |
 |CODING
 |	Shanghai YoungTone Tech.
 *-------------------------------------------------------------------------------*/
int yt_tts_get_rate_300(void);




/*-------------------------------------------------------------------------------
 |yt_tts_get_pitch_300(...)
 |PURPOSE
 |	This API intends to get pitch(tone).
 |
 |INPUT
 | none
 |				
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	the acutal pitch scale, which ranges 50~200. 
 |  100 is the normal pitch. 50 is the lowest pitch while 200 is the highest one.
 |
 |CODING
 |	Shanghai YoungTone Tech.
 *-------------------------------------------------------------------------------*/
int yt_tts_get_pitch_300(void);




/*-------------------------------------------------------------------------------
 |yt_tts_get_volume_300(...)
 |PURPOSE
 |	This API intends to get volume, say energy density scale
 |
 |INPUT
 | none
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	the acutal volume,which ranges 0~32768. 
 |	32768 means the loudest speech while 0 means silence
 |
 |CODING
 |	Shanghai YoungTone Tech.
 *-------------------------------------------------------------------------------*/
int yt_tts_get_volume_300(void);





/*-------------------------------------------------------------------------------
 |yt_tts_get_sampling_rate_300(...)
 |PURPOSE
 |	This API intends to get sampling rate
 |
 |INPUT
 |nSamplingRate: the target sampling rate 
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	the acutal sampling rate, which is 16000Hz for the current engine
 |
 |CODING
 |	Shanghai YoungTone Tech.
 *-------------------------------------------------------------------------------*/
int yt_tts_get_sampling_rate_300(void);




/*-------------------------------------------------------------------------------
 |yt_tts_utf8_to_utf16_300(...)
 |PURPOSE
 |	This API intends to conduct encoding conversion from UTF8 to UTF16
 |
 |INPUT
 |	strText_UTF8: buffer for the input text string in UTF8 encoding
 |	nTextLen_UTF8: length of strText_UTF8 in byte
 |
 |OUTPUT
 |	strText_U16: buffer for holding text in UTF16 encoding
 |	pTextLen_U16: pointer to character number of strText_U16
 |
 |RETURN VALUE
 |	none
 |
 |CODING
 |	YoungTone Inc.
 *-------------------------------------------------------------------------------*/
void  yt_tts_utf8_to_utf16_300(char *strText_UTF8,
								unsigned int nTextLen_UTF8,
								unsigned short *strText_U16,
								unsigned int  *pTextLen_U16);





/*-------------------------------------------------------------------------------
 |yt_tts_get_speech_frame_300(...)
 |PURPOSE
 |	This API intends to get speech frame from TTS engine...
 |
 |BACKGROUND
 |	Several customers need to return speech frame by frame,  
 |	So we need to provide such kind of API.
 |
 |INPUT
 |	none
 |
 |OUTPUT
 |	pSpeechFrame: buffer for holding speech samples, which should be writable
 |				  Typical  pSpeechFrame contains 1000 short integer
 |	pSampleNumber: pointer to sample number of pSpeechFrame
 |
 |RETURN VALUE
 |	It returns a flag indicating TTS engine status.
 |		0: indicates the engine reaches text end, say all the text has been processed...
 |		1: indicates a sentence has been completed
 |		2: indicates the engine needs to continue
 |		10: indicates the engine reaches phrase boundary.
 |
 |CODING
 |	YoungTone Inc.
 *-------------------------------------------------------------------------------*/
int yt_tts_get_speech_frame_300(short *pSpeechFrame, unsigned int *pSampleNumber);
												  






void yt_tts_stop_play_300();
void yt_tts_start_play_300();





/*-------------------------------------------------------------------------------
 |yt_tts_set_cb_print_info_300(...)
 |PURPOSE
 |	This API intends to print debugging information from TTS engine...
 |
 |INPUT
 |	print_info: a callback function for printing message.
 |              This function uses two arguments, where 
 |					strMessage is an input string while pTime is a output pointer to an integer 
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |  none
 |
 |CODING
 |	Shanghai YoungTone Tech.
 *-------------------------------------------------------------------------------*/
void yt_tts_set_cb_print_info_300(void (*print_info)(char *strMessage,unsigned int *pTime));





/*-------------------------------------------------------------------------------
 |yt_tts_start_activation_300(...)
 |PURPOSE
 |	This API intends to start activation procedure
 |
 |
 |INPUT
 |	none
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	It returns an integer flag indicating TTS engine status.
 |		0: TTS engine is activated; otherwise the TTS engine is NOT activated
 |
 |CODING
 |	Shanghai YoungTone Tech.
 *-------------------------------------------------------------------------------*/
int yt_tts_start_activation_300(void);


/*-------------------------------------------------------------------------------
 |yt_tts_is_active_300(...)
 |PURPOSE
 |	This API intends to return the status of TTS engine, say active or inactive status
 |
 |INPUT
 |	none
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	It returns an integer flag indicating TTS engine status.
 |		0: TTS engine is activated; otherwise the TTS engine is NOT activated
 |
 |CODING
 |	Shanghai YoungTone Tech.
 *-------------------------------------------------------------------------------*/
int yt_tts_is_active_300(void); //0: activated; others for inactivated





int yt_tts_set_platform_version_300(char *pPlatformVersion, int nSizeInByte);





#ifdef __cplusplus
}
#endif


#endif
