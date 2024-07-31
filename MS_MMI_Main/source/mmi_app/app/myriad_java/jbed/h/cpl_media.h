/*
 * Copyright 2004-2008 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: Media support (VM-independent)
 *
 * This interface provides multimedia support for the Jbed VM.
 *
 * There are three types of interfaces defined here:
 * <ul>
 * <li>@ref mediaconcepts
 * <li>@ref mediacommon
 * <li>@ref mediafull
 * </ul>
 * Exactly which subject is used is mainly defined in Java by the
 * com.jbed.tina.media.MediaToolkit configuration class.
 */

#ifndef __CPL_MEDIA_H__
#define __CPL_MEDIA_H__

#include <jbed_global.h>
#include <jbed_jbni.h> /* todo remove it */
#include <targ_media.h>

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/**
 * If we use the backwards compatability mode to implement
 * pseudostream on top of the legacy monolithic API then
 * define the controlling define here even if it is not
 * actually defined on the Java level. This to simply transition
 * in the ports who are using the old defines in #ifdef's
 */
#ifdef PSEUDOMONOLITHIC_MEDIA_PLAYER
#define MONOLITHIC_MEDIA_PLAYER
#endif /* PSEUDOMONOLITHIC_MEDIA_PLAYER */

/**
 * @name Return Codes
 * @ingroup mediaconcepts
 *
 * Return codes for media CPL functions. Operation specific return values are
 * documented with the respective function or operation ID.
 */
/*@{*/

/** Return value indicating operation success. */
#define RC_OK (0)

/** Return value indicating operation failure. */
#define RC_ERROR_OPERATION_FAILED (-1)

/** Return value indicating operation irrelavant due to circumstances, so ignored. */
#define RC_ERROR_COMMAND_IGNORED (-2)

#ifdef SUPPORT_RECORD
/** Return value indicating end of data in native recorder */
#define RC_END_OF_DATA (-1)
#endif /* SUPPORT_RECORD */
/*@}*/

/**
 * @name Media Type IDs
 * @ingroup nativeids
 *
 * Unique identification of the native player types. These defines are
 * shorthand for the values in @ref nativeids
 */
/*@{*/
#ifdef AMMS_SUPPORT
/** Media type identifier value for global control player */
#define GLOBALCONTROL_TYPE_ID (19)

#ifdef MEDIA_CAPABILITY_AUDIO3D
/** Media type identifier value for audio3d engine*/
#define AUDIO3DENGINE_TYPE_ID (23)

/** Media type identifier value for SoundSource3D */
#define SOUNDSOURCE3D_TYPE_ID (24)
#endif /* MEDIA_CAPABILITY_AUDIO3D */

#endif /* AMMS_SUPPORT */

/** Media type identifier value for tone sequence media. */
#define TONESEQ_TYPE_ID (1)

/** Media type identifier value for MIDI media. */
#define MIDI_TYPE_ID (2)

/** Media type identifier value for MMF/SMAF media. */
#define MMF_TYPE_ID (7)

/** Media type identifier value for WAVE sampled media. */
#define WAVE_TYPE_ID (3)

/** Media type identifier value for QCP media. */
#define QCP_TYPE_ID (30)

/** Media type identifier value for PHR media. */
#define PHR_TYPE_ID (36)

/** Media type identifier value for AMR  sampled media. */
#define AMR_TYPE_ID (4)

/** Media type identifier value for CMX media. */
#define CMX_TYPE_ID (35)

/** Media type identifier value for CMX media. */
#define CMX_VIDEO_TYPE_ID (44)

/** Media type identifier value for AMC media. */
#define AMC_VIDEO_TYPE_ID (45)

/** Media type identifier value for EVRC media. */
#define EVRC_TYPE_ID (43)

/** Media type identifier value for MPEG video media. */
#define MPEG_TYPE_ID (10)

/** Media type identifier value for MPEG4 audio media. */
#define MP4_AUDIO_TYPE_ID (33)

/** Media type identifier value for MPEG4 video media. */
#define MP4_VIDEO_TYPE_ID (22)

/** Media type identifier value for GIF image media. */
#define GIF_TYPE_ID (14)

/** Media type identifier value for JPEG image media. */
#define JPEG_TYPE_ID (38)

/** Media type identifier value for PNG image media. */
#define PNG_TYPE_ID (39)

/** Media type identifier value for bmp image media. */
#define BMP_TYPE_ID (40)

/** Media type identifier value for wbmp image media. */
#define WBMP_TYPE_ID (41)

/** Media type identifier value for bci image media. */
#define BCI_TYPE_ID (42)

/** Media type identifier value for MP3 audio media. */
#define MP3_TYPE_ID (5)

/** Media type identifier value for AAC audio media and its derivatives */
#define AAC_TYPE_ID (8)

#define IMELODY_TYPE_ID (6)

#define H263_TYPE_ID (11)

/** Media type identifier value for AWB media. */
#define AWB_TYPE_ID (13)

/** Media type identifier value for Quicktime media. */
#define QTIME_TYPE_ID (12)

#define CONTAINER_3GP_AUDIO_TYPE_ID (20)

#define CONTAINER_3GP_VIDEO_TYPE_ID (21)

#define CONTAINER_3GPP2_AUDIO_TYPE_ID (28)

#define CONTAINER_3GPP2_VIDEO_TYPE_ID (29)

/** Media type identifier value for WAVE capture device. */
#define WAVE_CAPTURE_TYPE_ID (3 | 128)

/** Media type identifier value for AMR capture device. */
#define AMR_CAPTURE_TYPE_ID (4 | 128)

/** Media type identifier value for MPEG capture device. */
#define MPEG_CAPTURE_TYPE_ID (10 | 128)

/** Media type identifier value for MPEG capture device. */
#define MP4_VIDEO_CAPTURE_TYPE_ID (22 | 128)

/** Media type identifier value for QCP capture device. */
#define QCP_CAPTURE_TYPE_ID (30 | 128)

/** Media type identifier value for CONTAINER_3GP_VIDEO capture device. */
#define CONTAINER_3GP_VIDEO_CAPTURE_TYPE_ID (21 | 128)

/** Media type identifier value for WMA capture device. */
#define WMA_AUDIO_CAPTURE_TYPE_ID (25 | 128)

/** Media type identifier value for WMV capture device. */
#define WMV_VIDEO_CAPTURE_TYPE_ID (26 | 128)

/** Media type identifier value for camera capture only device if
    we do use real video type. (normally MPEG_TYPE_ID is used for this) */
#define CAMERA_SNAPSHOT_TYPE_ID (16 | 128)

/** Media type identifier value for URI_TYPE_ID  media. */
#ifdef  SUPPORT_URI_STREAM_PLAYER
#define URI_TYPE_ID (88)
#endif /* SUPPORT_URI_STREAM_PLAYER */

#ifdef SUPPORT_RADIO_CAPTURE
/** Media type identifier value for radio capture only device */
#define ANTENNA_RADIO_TYPE_ID (17 | 128)
#endif /* SUPPORT_RADIO_CAPTURE */

/** Media type identifier value for WMA audio media. */
#define WMA_TYPE_ID (25)

#ifdef JBED_MEDIA_RESOURCE_MANAGEMENT

/** Notify VM to release the media resource . */
#define NATIVE_MEDIA_RESOURCE_REVOKE (2209)

/** Notify VM that the media resource is available */
#define NATIVE_MEDIA_RESOURCE_AVAILABLE (2210)

#endif /* JBED_MEDIA_RESOURCE_MANAGEMENT */

/*@}*/
/**
 * @defgroup mediaapi MMAPI and MIDP2-audio media APIs
 */

/**
 * @defgroup mediaconcepts Concepts used in the media playing APIs
 * @ingroup mediaapi
 *
 * This section explains some general terms, functions and constants that are
 * used throughout the media documentation.
 */

/**
 * @defgroup mediacommon Common functionality for all media types
 * @ingroup mediaapi
 *
 * This section describes functionality used by the media subsystem
 * regardless of whether MIDP2 only subset or Full MMAPI subsystem is used
 * and regardless of which Controls are defined for the media type.
 */

/**
 * @defgroup mediafull Extended functionality available with full MMAPI support
 * @ingroup mediaapi
 *
 * Adding full JSR135 by defining FULL_MMAPI_SUPPORT in the build
 * files allows the system to support extra Controls for certain mime
 * types. Exactly what set of Controls is used for which API
 * is determined in Java by com.jbed.tina.media.MediaToolkit.
 */

/**
 * @defgroup ammsSupport Extended functionality available with full AMMS support
 * @ingroup mediaapi
 *
 * Adding JSR234 by defining AMMS_SUPPORT in the build files.
 */

/**
 * @defgroup mediamonolithic Operations/functions for monolithic/file based play
 * @ingroup mediacommon
 *
 * This API is used if one of the players returned from the Java methods
 * com.jbed.tina.media.MediaToolkit#makeBasicPlayer or
 * com.jbed.tina.media.MediaToolkit#makeCorePlayer is of the "file" or
 * "monolithic" type.
 *
 * Use of this API is deprecated as the same functionality can be achieved
 * by specifying an infinite chunk size in @ref mediastreamed.
 *
 * For backwards compatibility purposes the (also deprecated) Jlist define
 * <code>PSEUDOMONOLITHIC_MEDIA_PLAYER</code> can be used to map the
 * @link mediastreamed Streaming interface @endlink to this one
 */

/**
 * @defgroup mediastreamed Operations/functions for streamed/chunked based play
 * @ingroup mediacommon
 *
 * These APIs are used if one of the players returned from the Java methods
 * com.jbed.tina.media.MediaToolkit#makeBasicPlayer or
 * com.jbed.tina.media.MediaToolkit#makeCorePlayer is of the "stream" or
 * "chunked" type.
 */

/**
 * \anchor cpl_media_operations
 * @name Media player operation IDs that are common to all media types.
 * @ingroup mediacommon
 *
 * The operation IDs defined here are applicable to #CPL_media_operation().
 *
 * The parameters and return values given in the description of the following
 * constants refer to the values for <code>nativeId</code>, <code>opId</code>,
 * <code>param1</code>, <code>param2</code>, and <code>param3</code> and the
 * return value for calls to CPL_media_operation().
 *
 * These ID values must match the corresponding values defined in the
 * <code>com.jbed.tina.media.MediaPlayer</code> class.
 */
/*@{*/
/**
 * Resets the underlying media hardware and software. It is assumed
 * that after a reset the native media system is initialized and available
 * to Jbed.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_RESET
 *
 * @return #RC_OK or #RC_ERROR_OPERATION_FAILED.
 */
#define OP_RESET (1)

/**
 * Closes the corresponding native player and release the allocated
 * resources.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_CLOSE
 *
 * @return #RC_OK or #RC_ERROR_OPERATION_FAILED.
 */
#define OP_CLOSE (2)

/**
 * Starts the playback of the corresponding media clip from the current
 * position (media time). The media clip is played asynchronously, i.e. in the
 * background. The program control is returned to the caller immediately. After
 * the playback has reached the end of media it is stopped automatically and
 * the current media time is reset to 0, i.e. to the beginning of the media
 * clip. In addition the event #NATIVE_MEDIA_EVENT_END_OF_MEDIA
 * is signalled to the appropriate Jbed player.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_PLAY
 *
 * @return #RC_OK or #RC_ERROR_OPERATION_FAILED.
 */
#define OP_PLAY (3)

/**
 * Pauses the playback of the corresponding media clip. The current media time
 * is frozen. If the playback is restarted (#OP_PLAY) it will continue from the
 * position (media time) where it was paused before.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_PAUSE
 *
 * @return #RC_OK or #RC_ERROR_OPERATION_FAILED.
 */
#define OP_PAUSE (4)

/**
 * Stops the playback of the corresponding media clip and resets the current
 * media time to 0 (the beginning of the media clip).
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_STOP
 *
 * @return #RC_OK or #RC_ERROR_OPERATION_FAILED.
 */
#define OP_STOP (5)

/**
 * release the allocated resources of native player.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_RELEASEDEVICE
 *
 * @return #RC_OK or #RC_ERROR_OPERATION_FAILED.
 */
#define OP_RELEASEDEVICE (15)

/**
 * Returns the duration of the corresponding media clip in milliseconds.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_GETDURATION
 *
 * @return The duration (>= 0) of the corresponding media clip in
 *  milliseconds, or -1 if the duration is unknown.
 */
#define OP_GETDURATION (6)

/**
 * Returns the current media time (position) of the corresponding native media
 * player in milliseconds.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_GETPOSITION
 *
 * @return The current media time (>= 0) of the corresponding native media
 *  player in milliseconds.
 * @note Returning -1 from this operation is not supported. A best effort
 * approximation must be made if this information is not readily available.
 * A simplistic but adequate implementation is available in the VM if
 * MEDIA_EMULATE_PLAYTIME is defined.
 */
#define OP_GETPOSITION (7)

/**
 * Set the current media time (position) of the corresponding native media
 * player in milliseconds.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_SETPOSITION
 * @param param1 The new media time (position) in milliseconds.
 *
 * @return The media position that was actually set in milliseconds, or -1
 *  if the new media time couldn't be set.
 *
 * @note If this operation always fails #CANNOT_SET_MEDIATIME must be set
 * in #CONST_PLAYER_PROPERTIES
 */
#define OP_SETPOSITION (8)

/**
 * Returns the volume which is currently set for the corresponding native
 * media player.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_GETVOLUME
 *
 * @return The current volume as a percentage, or #RC_ERROR_OPERATION_FAILED.
 */
#define OP_GETVOLUME (9)

/**
 * Returns bit-or of controls supported by native. Valid control bits are
 * #MEDIA_SUPPORT_VIDEOCONTROL, #MEDIA_SUPPORT_FRAMEPOSITIONINGCONTROL and
 * #MEDIA_SUPPORT_METADATACONTROL.
 *
 * In case that the controls supported cannot be determined in java layer, this
 * operation provide a way for native to inform java layer about which controls
 * are supported. This operation is called after CPL_media_prepareDev().
 *
 * For example, a 3GPP file may and may not contain video, so VideoControl is
 * not supported by default. Therefore, #MEDIA_SUPPORT_VIDEOCONTROL can
 * be returned here to make VideoControl available.
 *
 * URL players(e.g. rtp, rtsp) also take advantage of this method to get supported
 * controls from native.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_GETSUPPORTEDCONTROLS
 *
 * @return bit-or of controls supported by native.
 */
#define OP_GETSUPPORTEDCONTROLS (17)

/**
 * Sets the volume for the corresponding native media Player to a new value.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_SETVOLUME
 * @param param1 The new volume as a percentage.
 *
 * @return The volume that was actually set as a percentage, or
 *  #RC_ERROR_OPERATION_FAILED
 */
#define OP_SETVOLUME (10)
/*@}*/

/**
 * @name Media player operation IDs that apply to streamed/chunked players only.
 * @ingroup mediastreamed
 *
 * The operation IDs defined here are applicable to #CPL_media_operation().
 *
 * The parameters and return values given in the description of the following
 * constants refer to the values for <code>nativeId</code>, <code>opId</code>,
 * <code>param1</code>, <code>param2</code>, and <code>param3</code> and the
 * return value for calls to #CPL_media_operation().
 *
 * These ID values must match the corresponding values defined in the
 * <code>com.jbed.tina.media.MediaPlayer</code> class.
 */
/*@{*/
/**
 * Queries a suitable block size to send data to a streamed/chunked
 * player. This determines the size of the data buffers that will be
 * passed to the native player through #CPL_media_streamWrite().  At
 * most this size will be sent to data per write call. Apart from the
 * last call at the end of the data stream, every write will be at
 * least a third of this size.
 *
 * <em>This operation will not be used on a monolithic player.</em>
 *
 * <em>This operations is deprecated and replaced by #CONST_PLAYER_CHUNKSIZE</em>
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_GETCHUNKSIZE
 *
 *
 * @return The native player's chunk size, or #RC_ERROR_OPERATION_FAILED.
 */
#define OP_GETCHUNKSIZE (11)

/**
 * Queries if the data sent so far to a streamed/chunked player has
 * been played. This operation will not be used on a monolithic player.
 * This operation should use the async io mechanism to block and retry if all
 * the data has been played.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_DRAIN
 *
 * @return The operation returns >= 0 if all the data has already been
 *  played, or #RC_ERROR_OPERATION_FAILED.
 *
 *  If the implementation buffers all the data in native memory then
 *  #RC_ERROR_COMMAND_IGNORED * can be returned to indicate an
 *  #NATIVE_MEDIA_EVENT_END_OF_MEDIA will be sent explicitly.
 *  This is options is depcricated and use of async io is recommended.
 *
 * @ingroup mediastreamed
 */
#define OP_DRAIN (12)

/**
 * Set the size of the streamed media data in bytes.
 *
 * In case of recording it sets the record size limit in bytes.
 *
 * For playing, this operation is only
 * used for streamed/chunked players and is only currently needed on some
 * platforms for the streamed WAV player case where the data sent is only
 * the sample data not a WAV file format. Most targets can safely ignore
 * this operation and simply return a success indication.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_SETDATASIZE
 * @param param1 The data size in bytes.
 *
 * @return #RC_OK or #RC_ERROR_OPERATION_FAILED.
 * @note If this operation always fails #CANNOT_SET_SIZELIMIT must be set
 * in #CONST_PLAYER_PROPERTIES
 * @hideinitializer
 * @ingroup streamingrecording
 */
#define OP_SETDATASIZE (14)

/**
 *  Get actual mime from native layer.
 *  The opeation for all players which unknow actual mime type until getting some media data.
 *  @param @param opId #OP_GETMEDIATYPE
 */
#define OP_GETMEDIATYPE (16)

#ifdef SUPPORT_RECORD
/**
 * Write back all natively cached recorded data and return the total size
 * of the recording. This may only be called after recording has been stopped.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_GETRECSIZE
 *
 * @return The total size of the recorded data or #RC_ERROR_OPERATION_FAILED.
 */
#define OP_GETRECSIZE (13)

#endif /* SUPPORT_RECORD */

#ifdef JBED_MEDIA_RESOURCE_MANAGEMENT
/**
 * Set the media resource priority. This operation is called after CPL_media_prepareDev().
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_prepareDev()</code>.
 * @param opId #OP_SETPRIORITY
 * @param The resource priority, 1 is the highest priority and any number above that,
 *  up to the largest value of type int, have lower sequential priority.
 *
 * @return #RC_OK
 */
#define OP_SETPRIORITY (19)
#endif /* JBED_MEDIA_RESOURCE_MANAGEMENT */
/*@}*/

#ifdef SUPPORT_URI_STREAM_PLAYER
/**
 * Set the actual mime type id. The operation is invoked by Java side only when
 * the native player is a url stream player
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_prepareDev()</code>.
 * @param opId #OP_SETMEDIATYPE
 * @param The actual mime type id.
 *
 * @return #RC_OK
 */
#define OP_SETMEDIATYPE (20)
#endif /* SUPPORT_URI_STREAM_PLAYER */

/**
 * Send the media data for getting metadata. The operation is invoked continuously
 * to sent the data to native chunk by chunk until all data are sent or native side
 * returns #RC_ERROR_OPERATION_FAILED or #RC_ERROR_COMMAND_IGNORED.
 *
 * Note that only the data of the streamable media is sent to native with the OP if
 * MediaToolkitBase.sendAllDataForParsingMetaData() returns true.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 * previous call to <code>CPL_media_prepareDev()</code>.
 * @param opId #OP_PARSEMETADATA
 * @param param1 The number of data bytes in the chunk. 0 means all data has been
 * sent to native side.
 * @param paramOther Pointer to the data bytes to be parsed. null means all data
 * has been sent to native side.
 *
 * @return #RC_OK, #RC_ERROR_OPERATION_FAILED or #RC_ERROR_COMMAND_IGNORED.
 */
#define OP_PARSEMETADATA (21)

#ifdef SUPPORT_RECORD
/**
 * Start/resume recording the media.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 * previous call to <code>CPL_media_prepareDev()</code>.
 * @param opId #OP_STARTRECORD
 *
 * @return #RC_OK or #RC_ERROR_OPERATION_FAILED.
 */
#define OP_STARTRECORD (22)

/**
 * Pause recording the media.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 * previous call to <code>CPL_media_prepareDev()</code>.
 * @param opId #OP_PAUSERECORD
 *
 * @return #RC_OK or #RC_ERROR_OPERATION_FAILED.
 */
#define OP_PAUSERECORD (23)

/**
 * Stop recording the media. Calling the opration means that the current recording
 * is completed and any extra information (meta data, end data and so on) can be
 * appended to the data stream.
 *
 * <code>CPL_media_readData()</code> will return RC_END_OF_DATA if all recording
 * data has been read after calling the opration.
 *
 * Calling OP_STARTRECORD again will start a new recording after calling the
 * operation.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 * previous call to <code>CPL_media_prepareDev()</code>.
 * @param opId #OP_STOPRECORD
 *
 * @return #RC_OK or #RC_ERROR_OPERATION_FAILED.
 */
#define OP_STOPRECORD (24)
#endif /* SUPPORT_RECORD */

/**
 * @name  Control bits that is to be returned in #OP_GETSUPPORTEDCONTROLS
 *
 * The constants defined here are applicable only to #OP_GETSUPPORTEDCONTROLS
 */
/*@{*/

/**
 * Control bit for VideoControl that is to be returned in #OP_GETSUPPORTEDCONTROLS
 */
#define MEDIA_SUPPORT_VIDEOCONTROL ((1 << 8) | (1 << 9))

/**
 * Control bit for FramePositioningcontrol that is to be returned in #OP_GETSUPPORTEDCONTROLS
 */
#define MEDIA_SUPPORT_FRAMEPOSITIONINGCONTROL (1 << 7)

/**
 * Control bit for MetaDataControl that is to be returned in #OP_GETSUPPORTEDCONTROLS
 */
#define MEDIA_SUPPORT_METADATACONTROL (1)
/*@}*/

/**
 * @name  Media constants
 *
 * The constants defined here are applicable to CPL_media_getConst().
 */
/*@{*/

/**
 * Send data to media in chunks at least this big.
 *
 * In particular this size is used for the first fragement sent when
 * opening the device. Should be bigger than media header to
 * determine duration, frame rate, size if this can be told from just
 * a partial file for this type.
 *
 * The last fragment transferred can of course be shorter.
 * @ingroup mediastreamed
 */
#define CONST_PLAYER_CHUNKSIZE (205)

/**
 * Request a bit-mask indicating player capabilities for this type.
 */
#define CONST_PLAYER_PROPERTIES (204)
/*@}*/

/**
 * @name Flag bits for return value of CONST_PLAYER_PROPERTIES
 *
 * These values are OR-ed together in the bitmask returned by
 * CPL_media_getConst() for #CONST_PLAYER_PROPERTIES
 */
/*@{*/
/**
 * Set this flag if #OP_SETPOSITION will always fail for this media type
 */
#define CANNOT_SET_MEDIATIME (1)

#ifdef FULL_MMAPI_SUPPORT

#ifdef SUPPORT_RECORD
/**
 * Set this flag if #OP_SETDATASIZE will always fail for this media type
 * (required for recording types only).
 *
 * See #CONST_PLAYER_PROPERTIES
 * @hideinitializer
 * @ingroup streamingrecording
 */
#define CANNOT_SET_SIZELIMIT (2)

#endif /* SUPPORT_RECORD */

#ifdef NEED_MIDIRELATED_CONTROLS

/**
 * Request to return the maximum supported pitch setting in milli-semitones.
 */
#define CONST_MAXPITCH (200)

/**
 * Request to return the minimum supported pitch setting in milli-semitones.
 */
#define CONST_MINPITCH (201)

#endif /* NEED_MIDIRELATED_CONTROLS */

#ifdef NEED_RATE_CONTROL
/**
 * Request to return the maximum supported rate setting in milli-percentage.
 */
#define CONST_MAXRATE (202)

/**
 * Request to return the minimum supported rate setting in milli-percentage.
 */
#define CONST_MINRATE (203)

#endif /* NEED_RATE_CONTROL */

/*@}*/

#ifdef NEED_RATE_CONTROL

/**
 * @name Media operation ID values for RateControl
 */
/*@{*/
/**
 * Operation ID to set the playback rate.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_SETRATE
 * @param param1 The requested rate setting in milli-percentage.
 *
 * @return The rate that was actually set in milli-percentage or
 *  #RC_ERROR_OPERATION_FAILED.
 */
#define OP_SETRATE (102)

/**
 * Operation ID to query the playback rate.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_GETRATE
 *
 * @return the current playback rate in milli-percentage or
 *  #RC_ERROR_OPERATION_FAILED.
 */
#define OP_GETRATE (103)

/*@}*/

#endif /* NEED_RATE_CONTROL */

/**
 * Gets metadata items in an array.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_GETMETADATA
 * @param paramOther an Object array of the size of 1 to hold the return array.
 *   Both "key" and "value" should be in the form of STRING_INSTANCE. "key"
 *   string is supposed to be at index 2N of the returned array, while
 *   corresponding "value" string at array index 2N + 1.
 *
 * @return number of metadata pairs
 */
#define OP_GETMETADATA (18)


#ifdef NEED_MIDIRELATED_CONTROLS
/**
 * @name Media operation ID values for PitchControl and TempoControl
 * @ingroup mediafull
 *
 * The operation IDs defined here are applicable to #CPL_media_operation().
 *
 * The parameters and return values given in the description of the following
 * constants refer to the values for <code>param1, param2 and param3</code> and
 * the return value for calls to #CPL_media_operation().
 *
 * These ID values must match the corresponding values defined in
 * <code>com.jbed.tina.media.player.TinaExpandedMidiPlayer</code>.
 */
/*@{*/
/**
 * Operation ID to set the playback pitch.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_SETPITCH
 * @param param1 The new pitch value to be set in milli-semitones.
 *
 * @return The pitch that was actually set in milli-semitones or
 *  #RC_ERROR_OPERATION_FAILED.
 */
#define OP_SETPITCH (100)

/**
 * Operation ID to query the playback pitch.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_GETPITCH
 *
 * @return The current playback pitch in milli-semitones or
 *  #RC_ERROR_OPERATION_FAILED.
 */
#define OP_GETPITCH (101)

/**
 * Operation ID to set the playback instantaneous tempo.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_SETTEMPO
 * @param param1 The requested tempo setting in milli-beats-per-minute.
 *
 * @return The tempo that was actually set in milli-beats-per-minute or
 *  #RC_ERROR_OPERATION_FAILED.
 */
#define OP_SETTEMPO (104)

/**
 * Operation ID to query the playback tempo.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_GETTEMPO
 *
 * @return The current playback temp in milli-beats-per-minute or
 *  #RC_ERROR_OPERATION_FAILED.
 */
#define OP_GETTEMPO (105)

/*@}*/


/**
 * @defgroup midicontrol MidiControl
 * @ingroup mediafull
 * These operations and functions are called on a midi player if MidiControl
 * is enabled for this player. MidiControl optionally supports synthesizer
 * query functions which are enabled if MIDI_BANK_QUERY_SUPPORT is defined.
 */

/**
 * @name Operations for MidiControl
 * @ingroup midicontrol
 *
 * Some of the midi functionality is defined via the generic media operations interface.
 */
/*@{*/

/**
 * Operation ID to query the MIDI playback volume of a single MIDI channel.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_GETCHNVOL
 * @param param1 The channel number.
 *
 * @return The current playback volume for that channel in the range 0 to 127
 *  or #RC_ERROR_OPERATION_FAILED.
 */
#define OP_GETCHNVOL (106)

/**
 * Operation ID to set the MIDI playback volume of a single MIDI channel.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_SETCHNVOL
 * @param param1 The channel number.
 *
 * @return The current playback volume for that channel in the range 0 to 127
 *  or #RC_ERROR_OPERATION_FAILED.
 */
#define OP_SETCHNVOL (107)

#ifdef MIDI_BANK_QUERY_SUPPORT
/**
 * Operation ID to query program assigned to channel. It represents the
 * current state of the channel. During playback of a MIDI file, the
 * program may change due to program change events in the MIDI file.
 *
 * Note that each "get" consists of two calls. First call(param3 == 0) is to
 * get the count of programs. The second call(param3 != 0) will pass down
 * allocated memory which is supposed to be filled with data.
 *
 * part of MIDI_BANK_QUERY_SUPPORT
 *
 * @param nativeId
 * @param opId #OP_GETPROGRAM
 * @param param1 - channel - 0-15
 * @param param3 - "0" means getting the count of programs. Non-zero value is
 * the pointer to the allocated memory.
 *
 * @return count of programs
 */
#define OP_GETPROGRAM (108)

/**
 * Operation ID to query list of installed banks. If the custom parameter
 * is true, a list of custom banks is returned. Otherwise, a list of all
 * banks (custom and internal) is returned.
 *
 * Note that each "get" consists of two calls. First call(param3 == 0) is to
 * get the count of banks. The second call(param3 != 0) will pass down
 * allocated memory which is supposed to be filled with data.
 *
 * part of MIDI_BANK_QUERY_SUPPORT
 *
 * @param nativeId
 * @param opId #OP_GETBANKLIST
 * @param param1 - if set to true, returns list of custom banks.
 * @param param3 - "0" means getting the count of banks. Non-zero value is
 * the pointer to the allocated memory.
 *
 * @return count of banks
 */
#define OP_GETBANKLIST (109)

/**
 * Operation ID to query list of program numbers by given bank. If and only
 * if this bank is not installed, an empty array is returned.
 *
 * Note that each "get" consists of two calls. First call(param3 == 0) is to
 * get the count of programs. The second call(param3 != 0) will pass down
 * allocated memory which is supposed to be filled with data.
 *
 * part of MIDI_BANK_QUERY_SUPPORT
 *
 * @param nativeId
 * @param opId #OP_GETPROGRAMLIST
 * @param param1 - bank, 0..16383
 * @param param3 - "0" means getting the count of programs. Non-zero value is
 * the pointer to the allocated memory.
 *
 * @return count of programs
 */
#define OP_GETPROGRAMLIST (110)

/**
 * Operation ID to get name of program by given bank and program. For
 * space-saving reasons, an implementation may return an empty string
 *
 * Note that each "get" consists of two calls. First call(param3 == 0) is to
 * get the length of program name. The second call(param3 != 0) will pass down
 * allocated memory which is supposed to be filled with data.
 *
 * part of MIDI_BANK_QUERY_SUPPORT
 *
 * @param nativeId
 * @param opId #OP_GETPROGRAMNAME
 * @param param1 - bank, 0-16383
 * @param param2 - program, 0-127
 * @param param3 - "0" means getting the length of program name. Non-zero value
 * is the pointer to the allocated memory.
 *
 * @return the length of program name
 */
#define OP_GETPROGRAMNAME (111)

/**
 * Operation ID to query name of key by given bank, program and key. This method
 * applies to key-mapped banks (i.e. percussive banks or effect banks) only.
 * A return value of null means that the specified key is not mapped to a sound.
 * For melodic banks, where each key (=note) produces the same sound at different
 * pitch, this method always returns null. For space-saving reasons, an
 * implementation may return an empty string instead of the key name. To find
 * out which keys in a specific program are mapped to a sound, iterate
 * through all keys (0-127) and compare the return value of getKeyName to
 * non-null.
 *
 * Note that each "get" consists of two calls. First call(param3 == 0) is to
 * get the length of key name. The second call(param3 != 0) will pass down
 * allocated memory which is supposed to be filled with data.
 *
 * part of MIDI_BANK_QUERY_SUPPORT
 *
 * @param nativeId
 * @param opId #OP_GETKEYNAME
 * @param param1 - bank, 0-16383
 * @param param2 - upper 16-bit: program, 0-127; lower 16-bit: key, 0-127
 * @param param3 - "0" means getting the length of key name. Non-zero value
 * is the pointer to the allocated memory.
 *
 * @return the length of key name
 */
#define OP_GETKEYNAME (112)
#endif /* MIDI_BANK_QUERY_SUPPORT */
#endif /* NEED_MIDIRELATED_CONTROLS */
/*@}*/

#ifdef NEED_EQUALIZER_CONTROL
/*
 *  EqualizerControl Operations
 */

/**
 * Enables/disables the equalizer effect.
 *
 * @param opId #OP_SET_EQ_ENABLED
 * @param param1 - non-zero to enable, 0 to disable
 * @return #RC_OK
 */
#define OP_SET_EQ_ENABLED (471)

/**
 * Operation ID to enforce the equalizer effect to be in use.
 *
 * @param opId #OP_SET_EQ_ENFORCED
 * @param param1 non-zero if the reverberation effect cannot be dropped,
 *               zero if the reverberation effect can be dropped.
 * @return #RC_OK
 */
#define OP_SET_EQ_ENFORCED (472)

/**Sets the given equalizer band to the given gain value.
  *@param level - the new gain in millibels that will be set to the given band.
  *     getMinBandLevel() and getMaxBandLevel() will define the maximum and minimum values.
  *@param band - the frequency band that will have the new gain.
  *     The numbering of the bands starts from 0 and ends at (getNumberOfBands() - 1).
  */
#define OP_SETBANDLEVEL (462)

/**
 *  @param band - the frequency band whose gain is asked.
 *           The numbering of the bands starts from 0 and ends at (getNumberOfBands() - 1).
 *  @return:
 *      the gain set for the given band in millibels
 */
#define OP_GETBANDLEVEL (463)

/**
 *  Sets the bass level using a linear point scale with values between 0 and 100:
 *  a value of 0 applies the maximum available attenuation to frequencies in the bass band;
 *  a value of 50 gives a flat equalization of the bass band; and
 *  a value of 100 applies the maximum available amplification to frequencies in the bass band.
 *  All the previous settings will be lost on the bass band.
 *
 * @param level - the new level on a linear point scale that will be set to the bass band, 0 -- 100
 * @return:
 *      the level that was actually set
 */
#define OP_SETBASS (467)

/**
 * Sets the treble level using a linear point scale with values between 0 and 100:
 *  a value of 0 applies the maximum available attenuation to frequencies in the treble band;
 *  a value of 50 gives a flat equalization of the treble band; and
 *  a value of 100 applies the maximum available amplification to frequencies in the treble band.
 *  All the previous settings will be lost on the treble band.
 *
 *  @param level - the new level on a linear point scale that will be set to the treble band, 0 -- 100
 *  Returns:
 *      the level that was actually set
 */
#define OP_SETTREBLE (468)

/**
 * Gets the bass level.
 *  @returns:
 *      the current level that is set to the bass band. If the bass level cannot been defined EqualizerControl.UNDEFINED will be returned.
 */
#define OP_GETBASS (469)

/**
 * Gets the treble level.
 *  @returns:
 *      the current level that is set to the treble band. If the treble level cannot been defined EqualizerControl.UNDEFINED will be returned.
 */
#define OP_GETTREBLE (470)

/**
 * Gets the minimum band level supported.
 *
 * @return the minimum band level in millibels.
 */
#define OP_GETMINBANDLEVEL (460)

/**
 * Gets the maximum band level supported.
 *
 * @return the maximum band level in millibels.
 */
#define OP_GETMAXBANDLEVEL (461)

/**
 * Gets the number of frequency bands that the equalizer supports.
 *
 * @return the number of frequency bands that the equalizer supports.
 */
#define OP_GETNUMBEROFBANDS (464)

/**
 * Gets the center frequency of the given band.
 * @param param1 the frequency band whose center frequency is asked.
 *
 * @return the center frequency in milliHertz
 */
#define OP_GETCENTERFREQ (465)

/**
 * Gets the band that has the most effect on the given frequency.
 * @param1 the frequency in milliHertz which is to be equalized via the returned band.
 *
 * @return the frequency band that has most effect on the given frequency
 *      or -1 if no band has effect on the given frequency
 */
#define OP_GETBAND (466)

#endif /* NEED_EQUALIZER_CONTROL */

#ifdef NEED_CAMERA_CONTROL

/**
 * Returns the rotation of the camera device
 * Rotation refers to the direction the specific camera is pointing to
 * @param opId #OP_GET_CAMERA_ROTATION
 * @return:
 *     rotation of the camera device or UNKNOWN if unknown. ROTATE_NONE, ROTATE_LEFT, ROTATE_RIGHT or UNKNOWN
 */
#define OP_GET_CAMERA_ROTATION (600)

/**
 * Sets the exposure mode of the camera device. A method call with null parameter will be ignored
 * @param opId #OP_SET_EXPOSURE_MODE
 * @param paramOther - String instance, stands for the new exposure mode to be set.
 * @return #RC_OK
 */
#define OP_SET_EXPOSURE_MODE (601)

/**
 * Sets the video resolution
 * @param opId #OP_SET_VIDEO_RESOLUTION
 * @param param1 - the index of the video resolution to be set
 * Index 0 refers to the first pair returned by getSupportedStillResolutions() and index 1 refers
 * to the second pair and so on.
 * @return #RC_OK or #RC_ERROR_OPERATION_FAILED if changing resolution is not supported during
 * recording or playback
 */
#define OP_SET_VIDEO_RESOLUTION (603)

/**
 * Sets the still image resolution
 * @param opId #OP_SET_STILL_RESOLUTION
 * @param param1 - the index of the video resolution to be set
 *   Index 0 refers to the first pair returned by getSupportedStillResolutions() and index 1 refers to the second pair and so on.
 * @return #RC_OK
 */
#define OP_SET_STILL_RESOLUTION (604)

/**
 * Toggles the native shutter sound and visual shutter feedback on and off.
 * @param opId #OP_ENABLE_SHUTTER_FEEDBACK
 * @param param1 - non-zero to enable the shutter feedback
                   zero to disable the shutter feedback
 * @return #RC_OK
 */
#define OP_ENABLE_SHUTTER_FEEDBACK (605)

#endif /* NEED_CAMERA_CONTROL */

#ifdef NEED_FOCUS_CONTROL
/**
 * Autofocus.
 */
#define MEDIA_FOCUSCONTROL_AUTO (-1000)

/**
 * Locks the autofocus.
 */
#define MEDIA_FOCUSCONTROL_AUTO_LOCK (-1005)

/**
 * Moves the setting to the next supported level.
 */
#define MEDIA_FOCUSCONTROL_NEXT (-1001)

/**
 * Moves the setting to the previous supported level.
 */
#define MEDIA_FOCUSCONTROL_PREVIOUS (-1002)

/**
 * Unknown value.
 */
#define MEDIA_FOCUSCONTROL_UNKNOWN (-1004)
/**
 * Operation ID to set focus.
 * @param opID #OP_SET_FOCUS
 * @param param1 - distance of focus.
 */
#define OP_SET_FOCUS (666)

/**
 * Operation ID to set macro.
 * @param opID #OP_SET_MACRO
 * @param param1 - non-zero to enable macro
                   zero to disable macro
 */
#define OP_SET_MACRO (667)

#endif /* NEED_FOCUS_CONTROL */

#ifdef SUPPORT_CAPTURE_ENCODING
/**
 * @name Media player operation IDs that are used for format control operations
 * @ingroup TODO
 *
 * The operation IDs defined here are applicable to #CPL_media_operation().
 *
 * The parameters and return values given in the description of the following
 * constants refer to the values for <code>nativeId</code>, <code>opId</code>,
 * <code>param1</code>, <code>param2</code>, and <code>param3</code> and the
 * return value for calls to CPL_media_operation().
 *
 * These ID values must match the corresponding values defined in the
 * <code>com.jbed.tina.media.Player.ExpandedStreamRecorder</code> class.
 */
/*@{*/

/**
 * Const for audio format type
 */
#define FORMAT_AUDIOFORMAT (0)
/**
 * Const for video format type
 */
#define FORMAT_VIDEOFORMAT (1)
/**
 * Const for container format type
 */
#define FORMAT_CONTAINERFORMAT (2)
/**
 * Const for image format type
 */
#define FORMAT_IMAGEFORMAT (3)

/**
 * Sets format of recording for corresponding native media player
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_SET_FORMAT
 * @param param1 The type of media format to be set, could be
 *      FORMAT_AUDIOFORMAT, FORMAT_VIDEOFORMAT and FORMAT_CONTAINERFORMAT
 * @param paramOther a String instance which is the format to be set.
 *
 * @return #RC_OK or #RC_ERROR_OPERATION_FAILED
 */
#define OP_SET_FORMAT (500)

/**
 * Sets string parameter value of recording for corresponding native media player
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_SET_STRPARAMETER
 * @param param1 The type of media format to be set, could be
 *      FORMAT_AUDIOFORMAT, FORMAT_VIDEOFORMAT and FORMAT_CONTAINERFORMAT
 * @param paramOther an Object array. Parameter name is at index 0, while
 *      value is at index 1.
 *
 * @return #RC_OK or #RC_ERROR_OPERATION_FAILED
 */
#define OP_SET_STRPARAMETER (502)

/**
 * Sets integer parameter value of recording for corresponding native media player
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_SET_INTPARAMETER
 * @param param1 The type of media format to be set, could be
 *      FORMAT_AUDIOFORMAT, FORMAT_VIDEOFORMAT and FORMAT_CONTAINERFORMAT
 * @param param2 the value to be set
 * @param paramOther a String which is parameter name
 *
 * @return the value which is actually set.
 */
#define OP_SET_INTPARAMETER (504)

/**
 * Sets metadata value of recording for corresponding native media player
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_SET_METADATA
 * @param param1 The type of media format to be set, could be
 *      FORMAT_AUDIOFORMAT, FORMAT_VIDEOFORMAT and FORMAT_CONTAINERFORMAT
 * @param paramOther an Object array containing String instances. Metadata key
 *  name is at index 0, while  value is at index 1.
 *
 * @return #RC_OK or #RC_ERROR_OPERATION_FAILED
 */
#define OP_SET_METADATA (506)

/**
 * Sets metadata override value of recording for corresponding native media player
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_SET_OVERRIDE
 * @param param1 The type of media format to be set, could be
 *      FORMAT_AUDIOFORMAT, FORMAT_VIDEOFORMAT and FORMAT_CONTAINERFORMAT
 * @param param2 boolean value for override.
 *
 * @return #RC_OK or #RC_ERROR_OPERATION_FAILED
 */
#define OP_SET_OVERRIDE (508)

/**
 * Gets string parameter value of recording for corresponding native media player
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_GET_STRPARAMETER
 * @param param1 The type of media format to be set, could be
 *      FORMAT_AUDIOFORMAT, FORMAT_VIDEOFORMAT and FORMAT_CONTAINERFORMAT
 * @param paramOther an Object array which contains String instance. Parameter name
 * is at index 0, while value at index 1 is to be filled with parameter value.
 *
 *
 * @return #RC_OK or #RC_ERROR_OPERATION_FAILED
 */
#define OP_GET_STRPARAMETER (512)

/**
 * Gets integer parameter value of recording for corresponding native media player
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_GET_INTPARAMETER
 * @param param1 The type of media format to be set, could be
 *      FORMAT_AUDIOFORMAT, FORMAT_VIDEOFORMAT and FORMAT_CONTAINERFORMAT
 * @param paramOther parameter name
 *
 * @return value of the parameter
 */
#define OP_GET_INTPARAMETER (514)

/**
 * Gets estimated bit rate of recording for corresponding native media player
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_GET_ESTIMATEBITRATE
 * @param param1 The type of media format to be set, could be
 *      FORMAT_AUDIOFORMAT, FORMAT_VIDEOFORMAT and FORMAT_CONTAINERFORMAT
 *
 * @return value of estimated bit rate or #RC_ERROR_OPERATION_FAILED
 */
#define OP_GET_ESTIMATEBITRATE (516)

/**
 * Gets override of metadata of recording for corresponding native media player
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_GET_OVERRIDE
 * @param param1 The type of media format to be set, could be
 *      FORMAT_AUDIOFORMAT, FORMAT_VIDEOFORMAT and FORMAT_CONTAINERFORMAT
 *
 * @return 0 for false, 1 for true or #RC_ERROR_OPERATION_FAILED
 */
#define OP_GET_OVERRIDE (518)
/*@}*/
#endif /* SUPPORT_CAPTURE_ENCODING */

#ifdef NEED_VIDEO_INFRASTRUCTURE
/**
 * @name Mpeg-video-specific media operation ID values:
 * @ingroup mediavideo
 *
 * The operation ID values defined here are applicable to #CPL_media_operation().
 *
 * These ID values must match the corresponding values defined in
 * <code>com.jbed.tina.media.player.TinaExpandedVideoPlayer</code>.
 *
 * There are four rectangles involved in video playback:
 * <pre>
 *   MPEG Video File:                       Mobile Device:
 *
 *                                          +-----------------------------+
 *                                          |SCREEN                       |
 *                                          |  +--------------------+     |
 *   +---------------+                      |  |VIDEO REGION        |     |
 *   |SOURCE ////////|                      |  |  +-----------+     |     |
 *   |///////////////|                      |  |  |DISPLAY ///|     |     |
 *   |/////// ===========[Video Renderer]==============> /////|     |     |
 *   |///////////////|                      |  |  |///////////|     |     |
 *   |///////////////|                      |  |  +-----------+     |     |
 *   +---------------+                      |  +--------------------+     |
 *                                          +-----------------------------+
 * </pre>
 * 1) SOURCE:<br>
 *    The SOURCE rectangle reflects the dimensions of the video clip within
 *    the MPEG file.
 *
 * 2) SCREEN:<br>
 *    The physical SCREEN of the mobile device. The top left corner is the
 *    origin for SCREEN coordinates.
 *
 * 3) VIDEO REGION:<br>
 *    The region that may be used by the video renderer. Its location
 *    (top left corner) is set with respect to the SCREEN origin.
 *
 * 4) DISPLAY:<br>
 *    The DISPLAY is the destination rectangle for the video renderer. Its
 *    location (top left corner) is set with respect to the VIDEO REGION.
 *
 *    The DISPLAY may be located partially or fully outside the VIDEO
 *    REGION. When that happens, the renderer must clip the video to the
 *    boundaries of the VIDEO REGION.
 *
 * Note: All coordinates are specified in pixels.
 */
/*@{*/
/**
 * Operation ID to query the video playback natural display height.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_GET_SOURCE_HEIGHT
 *
 * @return returns The natural display height in pixels (a positive value), or
 *  #RC_ERROR_OPERATION_FAILED.
 */
#define OP_GET_SOURCE_HEIGHT (400)

/**
 * Operation ID to query the video playback natural display width.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_GET_SOURCE_WIDTH
 *
 * @return returns The natural display width in pixels (a positive value), or
 *  #RC_ERROR_OPERATION_FAILED.
 */
#define OP_GET_SOURCE_WIDTH (401)

/**
 * Operation ID to set the area of the screen which is available for video
 * playback. Note that there is only one video region which is shared among all
 * video players. The initial values are (0,0).
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_SET_VIDEOREGION_LOCATION
 * @param param1 The x coordinate of the video region's top left corner.
 * @param param2 The y coordinate of the video region's top left corner.
 *
 * @return #RC_OK or #RC_ERROR_OPERATION_FAILED.
 */
#define OP_SET_VIDEOREGION_LOCATION (402)

/**
 * Operation ID to set the size of the screen which is available for video
 * playback. Note that there is only one video region which is shared among all
 * video players. The initial values are (0,0).
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_SET_VIDEOREGION_SIZE
 * @param param1 The width coordinate of the video.
 * @param param2 The height coordinate of the video.
 *
 * @return #RC_OK or #RC_ERROR_OPERATION_FAILED.
 */
#define OP_SET_VIDEOREGION_SIZE (403)

/**
 * Operation ID to set the location of the video display with respect to the
 * video region. The location is specified in pixel values relative to the upper
 * left hand corner of the video region. By default, the video appears at
 * location (0,0). The location can be given in negative values or can be
 * greater than the actual size of the video region. When that happens, the
 * video should be clipped to the boundaries of the video region.
 *
 * @param playerId Unique identification of a native MPEG player, which was
 *  previously obtained by a call to OPL_media_fileOpen() or
 *  OPL_media_streamOpen().
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_SET_DISPLAY_LOCATION
 * @param param1 The x coordinate (in pixels) of the video display.
 * @param param2 The y coordinate (in pixels) of the video display.
 *
 * @return #RC_OK or #RC_ERROR_OPERATION_FAILED.
 */
#define OP_SET_DISPLAY_LOCATION (404)

/**
 * Operation ID to resize the current video display. The actual scaling
 * algorithm is left up to the underlying implementation. If the dimensions of
 * the requested video display size are smaller than the dimensions of the video
 * clip, some implementations may choose to merely clip the video while other
 * implementations may resize the video. If the dimensions of the requested
 * video display size are bigger than the dimensions of the video clip,
 * some implementations may resize the video while others may leave the
 * video clip in the original size and just enlarge the video display.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_SET_DISPLAY_SIZE
 * @param param1 Desired width (in pixels) of the video display.
 * @param param2 Desired height (in pixels) of the video display.
 *
 * @return #RC_OK or #RC_ERROR_OPERATION_FAILED.
 */
#define OP_SET_DISPLAY_SIZE (405)

/**
 * Operation ID to show or hide the video. Note that the video playback
 * continues. This function only affects the video rendering.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_SET_VISIBLE
 * @param param1 Zero to disable the video playback display and non-zero to
 *  enable it.
 *
 * @return #RC_OK or #RC_ERROR_OPERATION_FAILED.
 */
#define OP_SET_VISIBLE (406)

#ifdef NEED_FRAMEPOSITIONING_CONTROL
/**
 * Operation ID to set the current postition accurately to a specfic frame.
 *
 * This may be called on a started player. If the position is before or after
 * the end of media the position will be set to the first or last frame respectively.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_SET_FRAMEPOS
 * @param param1 frame number to position to (non-negative)
 *
 * @return the actual frame that the player has been positioned too.
 */
#define OP_SET_FRAMEPOS (407)

/**
 * Operation ID to set the current postition accurately to a specfic frame.
 *
 * This may be called on a started player. If the position is before or after
 * the end of media the position will be set to the first or last frame respectively.
 * If called on a started player the returned value will not be exact.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_SKIP_FRAMES
 * @param param1 number of frames to skip (negative for backwards)
 *
 * @return the actual number of frames skipped.
 */
#define OP_SKIP_FRAMES (408)

/**
 * Operation ID to convert from a media time value a frame number
 *
 * This operation is not allowed to fail for input time 0 and must
 * then report the number of the first frame.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_GET_FRAME_FROM_MS
 * @param param1 the media time in milliseconds to convert (non-negative).
 *
 * @return #RC_ERROR_OPERATION_FAILED or the frame number
 */
#define OP_GET_FRAME_FROM_MS (409)

/**
 * Operation ID to convert a frame number into a media time
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_GET_MS_FROM_FRAME
 * @param param1 frame number to convert (non-negative).
 *
 * @return #RC_ERROR_OPERATION_FAILED or the media time in millseconds
 */
#define OP_GET_MS_FROM_FRAME (410)

#endif /* NEED_FRAMEPOSITIONING_CONTROL */

/**
 * Operation ID to set full screen mode.
 * The operation is available when MediaToolkit.supportedNativeFullScreenOperation() return true.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_SET_FULL_SCREEN
 * @param param1 int non-zero - set full screen mode.
 *                       zero - set normal mode.
 * @return #RC_OK
 */
#define OP_SET_FULL_SCREEN (412)


#ifdef JBED_SPRINT_EXTENSION_21
/**
 * @defgroup sprint_landscape_mode Operation ID to enable/disable the video landscape mode.
 *
 * To enable the landscape mode:
 *  1) Rotate video 90 degrees to landscape view.
 *  2) Scale video up or down from its original size to fully fill the screen,
 *     without cropping or distorting, while maintaining the original aspect ratio.
 * To disable the landscape mode:
 *  1) Rotate the video -90 degrees back to portrait view.
 *
 * NOTE: When the landscape mode is disabled, the video scaling will be done in
 *       the following invoking of OP_SET_DISPLAY_LOCATION and OP_SET_DISPLAY_SIZE.
 *       It's not need to be done in this operation.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param opId #OP_SET_LANDSCAPEMODE
 * @param param1 int non-zero - enable the landscape mode
 *                       zero - disable the landscape mode
 *
 * @return #RC_OK
 */
#define OP_SET_LANDSCAPEMODE (411)
#endif /* JBED_SPRINT_EXTENSION_21 */

/*@}*/
#endif /* NEED_VIDEO_INFRASTRUCTURE */
#endif /* FULL_MMAPI_SUPPORT */

#ifdef SUPPORT_RADIO_CAPTURE
/**
 * @defgroup tuner Various tuner settings
 * @ingroup ammsSupport
 * There are Controls for various tuner settings. These Controls, if they are supported,
 * can typically be fetched from a radio Player
 * (for example a Player created by Manager.createPlayer("capture://radio");).
 */
/**
 * @name TunerControl operation IDs
 * @ingroup tuner
 * The operation IDs defined here are applicable to CPL_media_operation().
 *
 * The parameters and return values given in the description of the following
 * constants refer to the values for param1, param2 and param3 and the return
 * value for calls to #CPL_media_operation().
 *
 * These ID values must match the corresponding values defined in
 * <code>com.jbed.tina.media.player.CoreRadioPlayer</code>.
 * @{
 */
/**
 * Operation ID to get the min frequency the native radio player supported
 * @param opId #OP_GET_MINFREQ
 * @param param1 int modulationID, MODULATION_AM and MODULATION_fM
 * @return the min frequency in 100Hz.
 */
#define OP_GET_MINFREQ (301)
/**
 * Operation ID to get the max frequency the native radio player supported
 * @param opId #OP_GET_MAXFREQ
 * @param param1 int modulationID, MODULATION_AM and MODULATION_fM
 * @return the max frequency in 100Hz.
 */
#define OP_GET_MAXFREQ (302)
/**
 * Operation ID to set the frequency of the radio player
 * @param opId #OP_SET_FREQ
 * @param param1 int frequency user want to set in 100Hz units
 * @param param2 int modulationID, MODULATION_AM and MODULATION_fM
 * @return the frequency set in 100Hz.
 */
#define OP_SET_FREQ (303)
/**
 * Operation ID to get the frequency of the radio player
 * @param opId #OP_GET_FREQ
 * @return the current frequency in 100Hz.
 */
#define OP_GET_FREQ (334)
/**
 * Operation ID to seek the frequency of some channel which is on air
 * @param opId #OP_SEEK
 * @param param1 int start frequency in 100Hz units
 * @param param2 int modulationID, MODULATION_AM and MODULATION_fM
 * @param param3 if 1, the scan proceeds towards higher frequencies,
 *        else if 0, towards lower frequencies
 * @return int the seeked frequency or #RC_ERROR_OPERATION_FAILED if
 *  the seek functionality is not available for the given modulation
 */
#define OP_SEEK (304)
/**
 * Operation ID to set squelching on or off. Squelching means
 * muting the frequencies that do not contain radio broadcast
 * @param opId #OP_SET_SQUELCH
 * @param param1 if 1 sets squelching on,else if 0 sets it off
 * @return #RC_OK or #RC_ERROR_OPERATION_FAILED if the given squelch setting is not supported
 */
#define OP_SET_SQUELCH (305)
/**
 * Operation ID to get the strength of the recepted signal
 * @param opId #OP_GET_SIGNAL_STRENGTH
 * @return the signal strength or #RC_ERROR_OPERATION_FAILED if
 *  querying the signal strength is not supported
 */
#define OP_GET_SIGNAL_STRENGTH (306)
/**
 * Operation ID to get the stereo mode in use
 * @param opId #OP_SET_STEREO_MODE
 * @param param1 1 - set MONO;
 *               2 - set STEREO;
 *               3 - set AUTO.
 * @return #RC_OK
 */
#define OP_SET_STEREO_MODE (307)
/**
 * Operation ID to get the number of presets
 * @param opId #OP_GET_NUMBER_OF_PRESETS
 * @return the number of presets
 */
#define OP_GET_NUMBER_OF_PRESETS (308)
/**
 * Operation ID to use the preset channel
 * @param opId #OP_USE_PRESET
 * @param param1 int preset number
 * @return #RC_OK
 */
#define OP_USE_PRESET (309)
/**
 * Operation ID to set preset
 * @param opId #OP_SET_PRESET
 * @param param1 int preset number
 * @return #RC_OK
 */
#define OP_SET_PRESET (310)
/**
 * Operation ID to get preset frequency
 * @param opId #OP_GET_PRESET_FREQ
 * @param param1 int preset number
 * @return the frequency of the preset number
 */
#define OP_GET_PRESET_FREQ (312)
/**
 * Operation ID to get preset modulation
 * @param opId #OP_GET_PRESET_MODULATION
 * @param param1 int preset number
 * @return the modulation of the preset number
 */
#define OP_GET_PRESET_MODULATION (313)
/**
 * Operation ID to get preset mode
 * @param opId #OP_GET_PRESET_MODE
 * @param param1 int preset number
 * @return the mode of the preset number or
 *         #RC_ERROR_OPERATION_FAILED if the presets do not support storing of the stereo mode
 */
#define OP_GET_PRESET_MODE (314)
/**
 * Operation ID to get preset name
 * @param opId #OP_GET_PRESET_NAME
 * @param param1 int preset number
 * @param paramOther Object[1] to reserve preset name(string) to return
 * @return #RC_OK
 */
#define OP_GET_PRESET_NAME (315)
/**
 * Operation ID to set preset name
 * @param opId #OP_SET_PRESET_NAME
 * @param param1 int preset number
 * @param paramOther STRING_INSTANCE, preset name to be set
 * @return #RC_OK
 */
#define OP_SET_PRESET_NAME (316)
/*@}*/
/**
 * @name Constants for Tuner
 * @ingroup tuenr
 * @{
 */
/**
 * Constant to stand for "am"
 */
#define MODULATION_AM (1)
/**
 * Constant to stand for "fm"
 */
#define MODULATION_FM (2)
/*@}*/
/**
 * @name Constants for AMMS implementations
 * @ingroup ammsSupport
 * @{
 */
/**
 * Constant to stand for true
 */
#define IS_TRUE (1)
/**
 * Constant to stand for false
 */
#define IS_FALSE (0)
/*@}*/

/**----------------------------For RDSControl-----------------------------*/
/**
 * @name RDSControl operation IDs
 * @ingroup tuner
 * The operation IDs defined here are applicable to CPL_media_operation().
 *
 * The parameters and return values given in the description of the following
 * constants refer to the values for param1, param2 and param3 and the return
 * value for calls to #CPL_media_operation().
 *
 * These ID values must match the corresponding values defined in
 * <code>com.jbed.tina.media.player.CoreRadioPlayer</code>.
 * @{
 */
/**
 * Operation ID to set the automatic switching of the program in
 * case of the presence of Traffic Anouncement in another program
 * @param opId #OP_RDS_SET_ATA
 * @param param1 IS_TRUE  - turn on the automatic switching;
 *               IS_FALSE - turn off the automatic switching.
 * @return #RC_ERROR_OPERATION_FAILED if setting the given value is not supported
 *         else #RC_OK
 */
#define OP_RDS_SET_ATA (317)
/**
 * Operation ID to get the mode of the automatic switching of the program
 * @param opId #OP_RDS_GET_ATA
 * @return param1 IS_TRUE  - the automatic switching is on;
 *                IS_FALSE - the automatic switching is off.
 */
#define OP_RDS_GET_ATA (318)
/**
 * Operation ID to set the automatic switching of the transmitter in
 * the case of a stronger transmitter with the same PI presence
 * @param opId #OP_RDS_SET_AS
 * @param param1 IS_TRUE  - turn on the automatic switching;
 *               IS_FALSE - turn off the automatic switching.
 * @return #RC_ERROR_OPERATION_FAILED if setting the given value is not supported
 *         else #RC_OK
 */
#define OP_RDS_SET_AS (319)
/**
 * Operation ID to get the mode of the automatic switching of the transmitter
 * @param opId #OP_RDS_GET_AS
 * @return IS_TRUE  the automatic switching is on;
 *         IS_FALSE the automatic switching is off.
 */
#define OP_RDS_GET_AS (320)
/**
 * Operation ID to get the current Clock Time and date (CT)
 * @param opId #OP_RDS_GET_CT
 * @param paramOther LONGARRAY with length 1, reserve time data to return
 * @return #RC_OK
 */
#define OP_RDS_GET_CT (321)
/**
 * Operation ID to get the current status of the Traffic Programme switch
 * @param opId #OP_RDS_GET_TP
 * @return IS_TRUE  the automatic switching is on
 *         IS_FALSE the automatic switching is off
 */
#define OP_RDS_GET_TP (322)
/**
 * Operation ID to get the current status of the Traffic Anouncement switch
 * @param opId #OP_RDS_GET_TA
 * @return IS_TRUE  the automatic switching is on
 *         IS_FALSE the automatic switching is off
 */
#define OP_RDS_GET_TA (323)
/**
 * Operation ID to get the current Programme Service name
 * @param opId #OP_RDS_GET_PS
 * @param paramOther Object[1] to return name(String) of PS
 * @return #RC_OK
 */
#define OP_RDS_GET_PS (324)
/**
 * Operation ID to get the Programme Service names sending the given Programme TYpe
 * @param opId #OP_RDS_GET_PSBYPTY
 * @param short pty
 * @param paramOther Object[1] to return names(String) of PS
 * @return #RC_OK
 */
#define OP_RDS_GET_PSBPTY (325)
/**
 * Operation ID to get the Programme Service names sending Traffic Anouncements
 * @param opId #OP_RDS_GET_PSBYTA
 * @param int TRUE  TA is on
 *            FALSE TA is off
 * @param paramOther Object[1] to return names(String) of PS
 * @return #RC_OK
 */
#define OP_RDS_GET_PSBTA (326)
/**
 * Operation ID to get the current Programme Identification code
 * @param opId #OP_RDS_GET_PI
 * @return #RC_OK
 */
#define OP_RDS_GET_PI (327)
/**
 * Operation ID to get the frequencies sending the given Programme TYpe
 * @param opId #OP_RDS_GET_FBPTY
 * @param param1 short programme type
 * @param paramOther Object[1] to return frequencies(int[])
 * @return #RC_OK
 */
#define OP_RDS_GET_FBPTY (328)
/**
 * Operation ID to get the frequencies sending Traffic Anouncements (TA)
 * @param opId #OP_RDS_GET_FBTA
 * @param param1 int TRUE   - TA is on
                     FALSE  - TA is off
 * @param paramOther Object[1] to return the frequencis(int[][])
 * @return #RC_OK
 */
#define OP_RDS_GET_FBTA (333)
/**
 * Operation ID to get the current Programme TYpe as a String
 * with the maximum of 8 or 16 characters in English
 * @param opId #OP_RDS_GET_PTYS
 * @param param1 int TRUE   - maximum of 8
 *                   FALSE  - maximum of 16
 * @param paramOther Object[1] to return the PTY String
 * @return #RC_OK
 */
#define OP_RDS_GET_PTYS (329)
/**
 * Operation ID to return the status of the RDS reception
 * @param opId #OP_RDS_IRDSS
 * @return IS_TRUE,  RDS signal can be recepted
 *         IS_FALSE, RDS signal can not be recepted
 */
#define OP_RDS_IRDSS (330)
/**
 * Operation ID to get the current Radio Text
 * @param opId #OP_RDS_GET_RT
 * @param paramOther Object[1] to return the Radio Text(String)
 * @return #RC_OK
 */
#define OP_RDS_GET_RT (331)
/**
 * Operation ID to get the current Programme TYpe as short
 * @param opId #OP_RDS_GET_PTY
 * @return short, current pty
 */
#define OP_RDS_GET_PTY (332)
/*@}*/
#endif /* SUPPORT_RADIO_CAPTURE */
/*-------------------for ExposureControl---------------------*/
/**
 * @name ExposureControl operation IDs
 * @ingroup ammsSupport
 * The operation IDs defined here are applicable to CPL_media_operation().
 *
 * The parameters and return values given in the description of the following
 * constants refer to the values for param1, param2 and param3 and the return
 * value for calls to #CPL_media_operation().
 *
 * These ID values must match the corresponding values defined in
 * <code>com.jbed.tina.media.player.ExpandedStreamVideoRecorder</code>.
 * @{
 */
#ifdef NEED_EXPOSURE_CONTROL
/**
 * Operation ID to get the current exposure compensation
 * @param opId #OP_GET_EXPOSURE_COMPENSATION
 * @return the current exposure compensation values multiplied by 100.
 *  For example, a returned value 100 means 1.0 (that means doubling the light exposure).
 */
#define OP_GET_EXPOSURE_COMPENSATION (606)
/**
 * Operation ID to get the current shutter speed
 * @param opId #OP_GET_EXPOSURE_TIME
 * @return the current exposure time in microseconds
 */
#define OP_GET_EXPOSURE_TIME (607)
/**
 * Operation ID to return the amount of light received by the sensor due to current
 * settings for aperture, shutter speed and sensitivity
 * @param opId #OP_GET_EXPOSURE_VALUE
 * @return the current exposure value (EV)
 */
#define OP_GET_EXPOSURE_VALUE (608)
/**
 * Operation ID to return the current aperture
 * @param opId #OP_GET_FSTOP
 * @return the current aperture as an F-Stop number multiplied by 100.
 *  For example, returned value 280 would mean an F-Stop number f/2.8
 */
#define OP_GET_FSTOP (609)
/**
 * Operation ID to get the current sensitivity
 * @param opId #OP_GET_ISO
 * @return the current sensitivity as ISO value.
 *  For example, value 200 means ISO 200
 */
#define OP_GET_ISO (610)
/**
 * Operation ID to return the current light metering mode of the camera device
 * @param opId #OP_GET_LIGHTMETER
 * @return offset into array returned by MediaToolkit.getSupportedLightMeterings()
 */
#define OP_GET_LIGHTMETER (611)
/**
 * Operation ID to set the exposure compensation. This only affects when using automatic exposure settings
 * @param opId #OP_SET_EXPOSURE_COMPENSATION
 * @param param1 - the wanted exposure compensation value multiplied by 100. For example, a
 *  value 100 means 1.0 (that means doubling the light exposure).
 * @return #RC_OK
 */
#define OP_SET_EXPOSURE_COMPENSATION (612)
/**
 * Operation ID to set the shutter speed
 * @param opId #OP_GET_EXPOSURE_TIME
 * @param param1 - exposure time in microseconds or 0 for automatic exposure time
 * @return exposure time that was actually set
 */
#define OP_SET_EXPOSURE_TIME (613)
/**
 * Operation ID to set the aperture. Adjusting the optical zoom (via ZoomControl) might change the value. Therefore,
 * it is recommended to set the optical zoom first and then set the aperture if needed
 * @param opId#OP_SET_FSTOP
 * @param param1 - aperture as an F-Stop number multiplied by 100 or 0 for automatic aperture.
 *  For example, returned value 280 would mean an F-Stop number f/2.8
 * @return #RC_OK
 */
#define OP_SET_FSTOP (614)
/**
 * Operation ID to set the sensitivity
 * @param opId #OP_SET_ISO
 * @param param1 - sensitivity as an ISO value. For example, value 200 means ISO 200.
 *  0 means automatic sensitivity
 * @return #RC_OK
 */
#define OP_SET_ISO (615)
/**
 * Operation ID to set the metering mode for the automatic exposure of the camera device. This only affects
 * when using automatic exposure settings
 * @param opId #OP_SET_LIGHTMETER
 * @param param1 - offset of the  array returned by MediaToolkit.getSupportedLightMeterings()
 * @return #RC_OK
 */
#define OP_SET_LIGHTMETER (616)

#endif /* NEED_EXPOSURE_CONTROL */
/*@}*/
#ifdef NEED_FLASH_CONTROL
/**
 * Operation ID to get the used flash mode
 * @param opId #OP_GET_MODE
 * @return offset into array returned by MediaToolkit.getSupportedFlashModes()
 */
#define OP_GET_MODE (617)
/**
 * Operation ID to set the flash mode of a camera device.
 * @param opId #OP_SET_MODE
 * @param mode's offset into array returned by MediaToolkit.getSupportedFlashModes()
 */
#define OP_SET_MODE (619)
/**
 * Tells if the flash device is ready.
 * Setting up the flash device usually takes some time. This method tells if the flash device is ready for use.
 * @param opId #OP_ISFLASHREADY
 * @return non-zero  if the flash is ready
 *         zero if the flash is not ready
 */
#define OP_ISFLASHREADY (618)

#endif /* NEED_FLASH_CONTROL */
#ifdef NEED_ZOOM_CONTROL
/**
 * Sets the digital zoom of the camera device.
 * @param opId #OP_SET_DIGITALZOOM
 * @param param1 int level - digital zoom value which is arrayed in MediaToolkitBase.getSupportedDigitalLevels()
 * @return int digital zoom that was actually set.
 */
#define OP_SET_DIGITALZOOM (620)
/**
 * Sets the optical zoom of the camera device.
 * Setting the optical zoom is a mechanical operation which it takes some time to complete.
 * The method only returns when the setting of the zoom has been completed.
 * @param opId #OP_SET_OPTICALZOOM
 * @param param1 level - the new optical zoom value which is arrayed in MediaToolkitBase.getSupportedOpticalLevels()
 * @return int optical zoom value that was actually set.
 */
#define OP_SET_OPTICALZOOM (621)

#endif /* NEED_ZOOM_CONTROL */
/*-------------------for SnapshotControl---------------------*/
#ifdef NEED_SNAPSHOT_CONTROL
/**
 * Freeze the viewfinder. There are two types of freezing: freeze for a while and
 * freeze until OP_UNFREEZE_VIEWFINDER is called.
 *
 * "Freeze for a while" is like when user takes a picture, the captured picture
 * stay for a while on screen (1 or 2 seconds) then the viewfinder resumes itself.
 *
 * The other type is freezing the viewfinder until OP_UNFREEZE_VIEWFINDER is called.
 *
 * @param opId #OP_FREEZE_VIEWFINDER
 * @param param1 1 - need confirm later
 *               0 - need not confirm
 * @return #RC_OK
 */
#define OP_FREEZE_VIEWFINDER (671)
/**
 * Unfreeze the viewfinder.
 * @param opId #OP_UNFREEZE_VIEWFINDER
 * @return #RC_OK
 */
#define OP_UNFREEZE_VIEWFINDER (672)

/**
 * Sets the file directory where the images will be stored.
 * @param opId #OP_SET_DIRECTORY
 * @param paramOther - String instance of the storage directory
 * @see SnapshotControl
 */
#define OP_SET_DIRECTORY (695)

/**
 * Sets the filename prefix.
 * @param opId #OP_SET_FILE_PREFIX
 * @param paramOther - String instance of the prefix for the files to be created.
 * @see SnapshotControl
 */
#define OP_SET_FILE_PREFIX (696)

/**
 * Sets the filename suffix.
 * @param opId #OP_SET_FILE_SUFFIX
 * @param paramOther - String instance of the suffix for the files to be created.
 * @see SnapshotControl
 */
#define OP_SET_FILE_SUFFIX (697)

#endif /* NEED_SNAPSHOT_CONTROL */

#ifdef MEDIA_CAPABILITY_AUDIO3D
/**
 * @defgroup audio3d Support Extended functionality available with AMMS Audio3D support
 * @ingroup ammsSupport
 *
 * Adding JSR234 Audio3D by defining MEDIA_CAPABILITY_AUDIO3D in the build files.
 */

/**
 * @name Audio3D common operation IDs
 * @ingroup audio3d
 * The operation IDs defined here are applicable to CPL_media_operation().
 *
 * The parameters and return values given in the description of the following
 * constants refer to the values for param1, param2 and param3 and the return
 * value for calls to #CPL_media_operation().
 *
 * These ID values must match the corresponding values defined in
 * <code>com.jbed.microedition.media.player.Audio3DEngine</code>.
 * @{
 */
/**
 * Operation ID to create a new SoundSource3D.
 *
 * @param nativeId Native ID of the audio3d engine which was returned by a previous call
 *      to <code>CPL_media_initializeAudio3dEngine()</code>.
 * @param opId #OP_CREATE_SOUNDSOURCE3D
 * @return The handle of the new SoundSource3D.
 */
#define OP_CREATE_SOUNDSOURCE3D (901)

/**
 * Operation ID to destroy a SoundSource3D.
 * @param opId #OP_DESTROY_SOUNDSOURCE3D
 * @param param1 jint, the ID of SoundSource3D to be destroyed
 * @param param2 unused
 * @param param3 unused
 * @param paramOther unused
 * @return RC_OK
 */
#define OP_DESTROY_SOUNDSOURCE3D (902)
/*@}*/

/**
 * @name LocationControl operation IDs
 * @ingroup audio3d
 * The operation IDs defined here are applicable to CPL_media_operation().
 *
 * The parameters and return values given in the description of the following
 * constants refer to the values for param1, param2 and param3 and the return
 * value for calls to #CPL_media_operation().
 *
 * These ID values must match the corresponding values defined in
 * <code>com.jbed.microedition.media.player.Audio3DEngine</code>.
 * @{
 */
/**
 * Operation ID to set the location using cartesian right-handed coordinates that are
 * relative to the origin.
 *
 * @param nativeId Native ID of the Audio3dEngine or SoundSource3D which was returned by a
 *      previous call to <code>CPL_media_initializeAudio3dEngine()</code> or
 *      <code>OP_CREATE_SOUNDSOURCE3D</code>.
 * @param mediaType #AUDIO3DENGINE_TYPE_ID indicates Specator and #SOUNDSOURCE3D_TYPE_ID
 *      indicates SoundSource3D.
 * @param opId #OP_SET_LOCATION_CARTESIAN
 * @param param1 the x-coordinate of the new location in millimeters
 * @param param2 the y-coordinate of the new location in millimeters
 * @param param3 the z-coordinate of the new location in millimeters
 *
 * @return #RC_OK
 */
#define OP_SET_LOCATION_CARTESIAN (905)
/*@}*/
/**
 * @name OrientationControl operation IDs
 * @ingroup audio3d
 * The operation IDs defined here are applicable to CPL_media_operation().
 *
 * The parameters and return values given in the description of the following
 * constants refer to the values for param1, param2 and param3 and the return
 * value for calls to #CPL_media_operation().
 *
 * These ID values must match the corresponding values defined in
 * <code>com.jbed.microedition.media.player.Audio3DEngine</code>.
 * @{
 */
/**
 * Operation ID to turn the object to the new orientation using two vectors.
 * @param nativeId Native ID of the Audio3DEngine which was returned by a previous
 *      call to <code>CPL_media_initializeAudio3dEngine()</code>.
 * @param opId #OP_SET_ORIENTATION_VECTOR
 * @param paramOther an int array of the size of 6 that has first the front vector
 *      and then the up vector
 *
 * @return #RC_OK
 */
#define OP_SET_ORIENTATION_VECTOR (915)
/*@}*/
/**
 * @name SoundSource3D operation IDs
 * @ingroup audio3d
 * The operation IDs defined here are applicable to CPL_media_operation().
 *
 * The parameters and return values given in the description of the following
 * constants refer to the values for param1, param2 and param3 and the return
 * value for calls to #CPL_media_operation().
 *
 * These ID values must match the corresponding values defined in
 * <code>com.jbed.microedition.media.player.Audio3DEngine</code>.
 * @{
 */
/**
 * Operation ID to add a Player to the SoundSource3D.
 * @param nativeId Native ID of the SoundSource3D which was returned by a
 *      previous call to <code>OP_CREATE_SOUNDSOURCE3D</code>.
 * @param mediaType #SOUNDSOURCE3D_TYPE_ID indicates SoundSource3D.
 * @param opId #OP_SOUNDSOURCE3D_ADD_PLAYER
 * @param param1 the ID of the Player to be added
 * @param param2 unused
 * @param param3 unused
 * @param paramOther unused
 * @return #RC_OK
 */
#define OP_SOUNDSOURCE3D_ADD_PLAYER (903)

/**
 * Operation ID to remove a Player from the SoundSource3D.
 * @param nativeId Native ID of the SoundSource3D which was returned by a
 *      previous call to <code>OP_CREATE_SOUNDSOURCE3D</code>.
 * @param mediaType #SOUNDSOURCE3D_TYPE_ID indicates SoundSource3D.
 * @param opId #OP_SOUNDSOURCE3D_REMOVE_PLAYER
 * @param param1 the ID of the Player to be removed
 * @param param2 unused
 * @param param3 unused
 * @param paramOther unused
 * @return #RC_OK
 */
#define OP_SOUNDSOURCE3D_REMOVE_PLAYER (904)
/*@}*/

/**
 * @name DistanceAttenuationControl operation IDs
 * @ingroup audio3d
 * The operation IDs defined here are applicable to CPL_media_operation().
 *
 * The parameters and return values given in the description of the following
 * constants refer to the values for param1, param2 and param3 and the return
 * value for calls to #CPL_media_operation().
 *
 * These ID values must match the corresponding values defined in
 * <code>com.jbed.microedition.media.player.Audio3DEngine</code>.
 * @{
 */
/**
 * Operation ID to set the distance attenuation parameters.
 *
 * @param nativeId Native ID of the SoundSource3D which was returned by a
 *      previous call to <code>OP_CREATE_SOUNDSOURCE3D</code>.
 * @param mediaType #SOUNDSOURCE3D_TYPE_ID indicates SoundSource3D.
 * @param opId #OP_SET_DISTANCEATTENUATION_PARAMETERS
 * @param param1 minDistance the minimum distance, below which the distance
 *        gain is clipped to its maximum value of 1.0
 * @param param2 maxDistance the maximum distance, beyond which the distance
 *        gain does not decrease any more
 * @param param3 rolloffFactor the rolloff factor, specified in thousandths
 *        (<code>1000</code> representing a rolloff factor of 1.0, <code>2000</code>
 *        representing 2.0 and <code>500</code> representing 0.5). Higher values
 *        cause the distance gain to attenuate more quickly.
 * @param paramOther INTARRY with length 1, if data[0] equals 0 it should
 *        keep the volume as large as in the <code>maxDistance</code> and do not
 *        decrease when beyond the <code>maxDistance</code>. Otherwise it should
 *        mute when beyond the <code>maxDistance</code>.
 * @return #RC_OK
 */
#define OP_SET_DISTANCEATTENUATION_PARAMETERS (914)
/*@}*/
#endif /* MEDIA_CAPABILITY_AUDIO3D */

#ifdef NEED_REVERB_CONTROL
 /**
 * @name ReverbControl operation IDs
 * @ingroup ammsSupport
 * The operation IDs defined here are applicable to CPL_media_operation().
 *
 * The parameters and return values given in the description of the following
 * constants refer to the values for param1, param2 and param3 and the return
 * value for calls to #CPL_media_operation().
 *
 * These ID values must match the corresponding values defined in
 * <code>com.jbed.microedition.media.player.Audio3DEngine</code>.
 * @{
 */
/**
 * Operation ID to get the gain level of the reverberation.
 *
 * @param opId #OP_GET_REVERB_LEVEL
 *
 * @return the level of the reverberation in millibels
 */
#define OP_GET_REVERB_LEVEL (906)
/**
 * Operation ID to set the gain level of the reverberation.
 *
 * @param opId #OP_SET_REVERB_LEVEL
 * @param param1 the new level of the reverberation in millibels
 *
 * @return the value that was actually set
 */
#define OP_SET_REVERB_LEVEL (907)
/**
 * Operation ID to get the reverberation time.
 *
 * @param opId #OP_GET_REVERB_TIME
 *
 * @return the current reverberation time in milliseconds or
 *     #RC_ERROR_OPERATION_FAILED if querying the reverb time is not supported.
 */
#define OP_GET_REVERB_TIME (908)
/**
 * Operation ID to set the reverberation time of the reverb.
 *
 * @param opId #OP_SET_REVERB_TIME
 * @param param1 new reverberation time in milliseconds
 *
 * @return #RC_OK or #RC_ERROR_OPERATION_FAILED if the changing of the reverb time is not supported.
 */
#define OP_SET_REVERB_TIME (909)
/**
 * Operation ID to set the reverberation effect according to the given preset.
 *
 * @param opId #OP_SET_REVERB_PRESET
 * @param paramOther a STRING_INSTANCE of the new preset that will be taken into use
 *
 * @return #RC_OK
 */
#define OP_SET_REVERB_PRESET (910)
/**
 * Operation ID to enable/disable the reverberation effect.
 *
 * @param opId #OP_SET_REVERB_ENABLED
 * @param param1 non-zero to enable the reverberation, zero to disable it
 *
 * @return #RC_OK
 */
#define OP_SET_REVERB_ENABLED (911)
/**
 * Operation ID to enforce the reverberation effect to be in use.
 *
 * @param opId #OP_SET_REVERB_ENFORCED
 * @param param1 non-zero if the reverberation effect cannot be dropped,
 *               zero if the reverberation effect can be dropped
 *
 * @return #RC_OK
 */
#define OP_SET_REVERB_ENFORCED (912)
/**
 * Operation ID to get the available reverberation preset names.
 *
 * @param opId #OP_GET_REVERB_PRESETNAMES
 * @param paramOther an Object array of the size of 1 to hold the return array.
 *                   Each preset name should be in the form of STRING_INSTANCE.
 *
 * @return the number of reverb names.
 */
#define OP_GET_REVERB_PRESETNAMES (913)
/*@}*/
#endif /* NEED_REVERB_CONTROL */

/**
 * @name Starting/Shutting down the media subsystem
 * @ingroup mediacommon
 */
/*@{*/
/**
 * Allow any initialisation required for the native media mechanism. This
 * function will be called before any other function of this module can be
 * applied.
 *
 * @return nothing
 */
extern void CPL_media_init(void);

/**
 * Allow any finalialisation required for the native media mechanism. It should
 * be called when the native media player is no longer needed. All resources
 * currently assigned to the native media player should be released. After a
 * successful call the native media players are no longer available.
 *
 * @warning {It is currently up to the platform port to call this function at VM shutdown}
 *
 * @return nothing
 */
extern void CPL_media_term(void);
/*@}*/

/**
 * @name Starting/Shutting down the audio3d engine
 * @ingroup audio3d
 */
/*@{*/

/**
 * Initialize the native audio3d engine.
 *
 * @return The handle of the engine.
 */
extern jint CPL_media_initializeAudio3dEngine(void);

/**
 * Finalize the native audio3d engine. All resources allocated for the native audio3d
 * engine should be released here.
 *
 * @param nativeInstance The handle of the engine returned by #CPL_media_initializeAudio3dEngine()
 */
extern void CPL_media_finalizeAudio3dEngine(jint nativeInstance);
/*@}*/

/**
 * @defgroup mediaops Generic media operations
 * @ingroup mediaconcepts
 * @ingroup mediacommon
 *
 * A lot of the media functionality is written in terms of calling
 * abstract operations named by an ID value on  nativeID handles.
 */

/**
 * @name Abstract operation methods
 * @ingroup mediaops
 *
 * These functions are used to dispatch generic media operations on
 * native handles created before by one of #CPL_media_openStreamDev or
 * #CPL_media_openMonolithicDev
 */
/*@{*/
/**
 * Perform an operation on the specified media player.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *  previous call to <code>CPL_media_open*Dev()</code>.
 * @param mediaType type code for the media being played. See @ref nativeids
 * @param opId The operation to be performed. See the operation constant
 *  description in mediasystem.h for details, e.g. OP_PLAY.
 * @param param1 The first parameter (if required) for the operation.
 * @param param2 The second parameter (if required) for the operation.
 * @param param3 The third parameter (if required) for the operation.
 * @param paramOther reference to remaining parameter(s) (if required)
 *       for the operation. This is typically an array of extra parameters.
 *
 * @return An operation-specific value. See the operation constant description
 *  for details, see \ref cpl_media_operations "list of operations" for details.
 */
extern jint CPL_media_operation(
  jint nativeId
, jint mediaType
, jint opId
, jint param1
, jint param2
, jint param3
/* TODO remove INSTANCE usage */
, INSTANCE paramOther
);

/**
 * Common constant value query function.
 *
 * @param mediaType Id of the media type the constant is for.
 * @param code What constant value is to be provided.
 *
 * @return Value requested. For unknown values of 'code' return 0
 */
extern jint CPL_media_getConst(
  jint mediaType
, jint code
);
/*@}*/

#ifdef MONOLITHIC_MEDIA_PLAYER
/**
 * @name Functions for monolithic/file based playing
 */
/*@{@*/
/**
 *
 * Aquire resources to play a media item in a monolithic way (media data must
 * be provided in one go).
 *
 * @param mediaType ID of the media type of the media item.
 * @param callbackId Value to be remember and quoted back in any events sent
 *  to the VM by this player instance.
 * @param arrHandle Java byte array instance which holds all the media data
 *  for the media item
 * @param len The number of media data bytes in the media item.
 *
 * @return An ID value for the native player instance if successful or
 *  zero for failure.
 * @ingroup mediamonolithic
 */
extern jint CPL_media_openMonolithicDev(
  jint      mediaType
, jint      callbackId
, BYTEARRAY arrHandle
, jint      len
);
/*@}*/
#endif /* MONOLITHIC_MEDIA_PLAYER */

#ifdef NEED_STREAM_INFRASTRUCTURE
/**
 * @name Functions for streamed/chunked based playing
 */
 /*@{@*/

#ifdef MEDIA_MULTISTAGE_DEVICEOPEN
/**
 * Setup initial data chunk for parsing media item
 *
 * This sets up the native side for decoding the header of the data stream
 * and ready to answer questions about duration, size, play rate contained
 * therein.
 *
 * If a valid handle is returned the initial chunk is concidered consumed.
 * Otherwise it will be sent again via CPL_media_openStreamDev()
 *
 * Implemetation may ignore this function by returning 0, however, if it tries to
 * open device and fails, -1 should be returned to signal error occured.
 *
 * If the mediaType is URI_TYPE_ID, the parameter buf would be the URI string(For file URI,
 * it would be the native path)
 *
 * @param mediaType ID of the media type of the media item callbackId or -1
 *        if this ID is still to be allocated. A media ID will then be provided
 *        in a following CPL_media_openStreamDev() call.
 * @param callbackId Value to be remembered and quoted back in any events sent
 *  to the VM by this player instance.
 * @param buf Buffer with initial chunk of the data.
 * @param buflen size of initial data
 *
 * @return An ID value for the native player instance if successful or -1 for
 *  failure or zero for ignoring this function.
 * @ingroup mediastreamed
 */
extern jint CPL_media_prepareDev(
  jint            mediaType
, jint            callbackId
, BYTEARRAY       buf
, jint            buflen
);
#endif /* MEDIA_MULTISTAGE_DEVICEOPEN */

/**
 * Aquire resources to play a media item in a streamed/chunked way.
 *
 * @param mediaType ID of the media type of the media item callbackId.
 * @param callbackId Value to be remembered and quoted back in any events sent
 *  to the VM by this player instance.
 * @param nativeHandle native handle if an earlier call to to prepareDev succeeded.
 * @param buf Buffer with initial chunk of the data (or NULL).
 * @param len size of initial data (0 if already consumed before)
 *
 * @return An ID value for the native player instance if successful or zero for
 *  failure or -1 for device busy
 * @ingroup mediastreamed
 */
extern jint CPL_media_openStreamDev(
  jint            mediaType
, jint            callbackId
#ifdef MEDIA_MULTISTAGE_DEVICEOPEN
, jint            nativeHandle
, BYTEARRAY       buf
, jint            len
#endif /* MEDIA_MULTISTAGE_DEVICEOPEN */
);

/** chunk state for stream write */
#define CPL_CHUNK_ERROR (-1)
/** chunk state for stream write */
#define CPL_CHUNK_MORE_DATA (0)
/** chunk state for stream write */
#define CPL_CHUNK_EOF (1)

/**
 * Send a chunk of data to a streamed/chunked player. If not all the data can
 * be accepted, then none of the data should be used at some later time when
 * more space has become available to accept data, the asynchronous
 * notification mechanism should be used.
 *
 * @param nativeId The ID value for the native player instance, as returned by
 *  the open call for the player.
 * @param mediaType ID of the media type of the media item callbackId.
 * @param dataPtr Pointer to the data bytes to be played.
 * @param len The number of data bytes in the chunk.
 * @param chunkState current state of this chunk of data, could be CPL_CHUNK_ERROR,
 *    CPL_CHUNK_MORE_DATA or CPL_CHUNK_EOF.
 *
 * @return Number of bytes accepted for success, -1 for failure.
 */
extern jint CPL_media_streamWrite(
  jint        nativeId
, jint mediaType
, const jbyte *dataPtr
, jint        len
, jint        chunkState
);

#ifdef JAVA_WAV_HEADER_PARSING

/**
 * Aquire resources to play a WAV media item in a streamed/chunked way.
 *
 * @param callbackId Value to be remembered and quoted back in any events sent
 *  to the VM by this player instance.
 * @param sampleRate The WAV sample rate in samples per second.
 * @param channels The number of channels (1=mono, 2=stereo).
 * @param bits The size of each sample in bits.
 * @param isSigned true if the samples are signed, false if they are unsigned.
 * @param bigend true if the sample values are in bigendian form, false if they
 *  are littleendian
 * @param natId native handle if an earlier call to to prepareDev succeeded.
 * @param buf Buffer with initial chunk of the data (or NULL).
 * @param len size of initial data (0 if already consumed before)
 *
 * @return An ID value for the native player instance if successful or zero for
 *  failure
 * @ingroup mediastreamed
 */
extern jint CPL_media_openWavStreamDev(
  jint     callbackId
, jint     sampleRate
, jint     bits
, jint     channels
, jboolean isSigned
, jboolean bigend
#ifdef MEDIA_MULTISTAGE_DEVICEOPEN
, jint            natId
, BYTEARRAY       buf
, jint            len
#endif /* MEDIA_MULTISTAGE_DEVICEOPEN */
);
#endif /* JAVA_WAV_HEADER_PARSING */

/*@}*/

#endif  /* NEED_STREAM_INFRASTRUCTURE */

#ifdef NEED_MIDIRELATED_CONTROLS
/**
 * @name Additional functions for MIDI players
 * @ingroup midicontrol
 *
 * These functions will only be called for MIDI players.
 */
/*@{*/
/**
 * Send a single short MIDI event to the MIDI device/sequencer.
 *
 * @param nativeId The ID value for the native player instance, as returned by
 *  the open call for the player.
 * @param op The MIDI event ID byte.
 * @param data1 The first parameter byte value for the MIDI event.
 * @param data2 The second parameter byte value for the MIDI event.
 *
 * @return nothing
 */
extern void CPL_media_sendShortMidi(
  jint nativeId
, jint op
, jint data1
, jint data2
);

/**
 * Send a long MIDI event or several MIDI events to the NIDI device/sequencer.
 *
 * @param nativeId The ID value for the native player instance, as returned by
 *  the open call for the player.
 * @param data Pointer to the MIDI event data bytes to be sent.
 * @param len The number of MIDI event data bytes to be sent.
 *
 * @return The number of midi event bytes sent, or -1 for failure.
 */
extern jint CPL_media_sendLongMidi(
  jint  nativeId
, jbyte *data
, jint  len
);

#ifdef MIDI_BANK_QUERY_SUPPORT

/**
 * Perform a midi operation on the specified media player.
 *
 * Note that AsyncIO is not available on this function.
 *
 * part of MIDI_BANK_QUERY_SUPPORT
 *
 * @param nativeId native player handle
 * @param code midi operation code
 * @param param1 first parameter
 * @param param2 second parameter
 * @param param3 third parameter
 *
 * @return An operation-specific value.
 */
extern jint CPL_media_midiOperation(
  jint  nativeId
, jint  code
, jint  param1
, jint  param2
, jint  param3
);

#endif /* MIDI_BANK_QUERY_SUPPORT */

/*@}*/
#endif /* NEED_MIDIRELATED_CONTROLS */

#ifdef NEED_VIDEO_INFRASTRUCTURE
/**
 * @defgroup mediavideo Video media support
 * @ingroup mediafull
 */

/**
 * @name Functions called on video players
 * @ingroup mediavideo
 */
/*@{*/
/**
 * Calculate the required storage size for taking a snapshot of the currently
 * displaying video in the specified format.
 *
 * This function is designed to work together with #CPL_media_getSnapshot. It is
 * supposed that implementation takes picture when this function is called and
 * returns size of the picture, then fills picture data into the buffer passed
 * by subsequent #CPL_media_getSnapshot,
 *
 * NOTE: If AsyncIO is used in the implementation, -1 must be returned in order
 * to make its caller work properly.
 *
 * @param nativeId The ID value for the native player instance, as returned by
 *  the open call for the player.
 * @param type String describing the desired encoding format. See the MMAPI
 *  spec for the string format.
 *
 * @return The required storage size in bytes or -1 for failure and if AsyncIO
 * is used.
 */
extern jint CPL_media_getSnapshotSize(
  jint nativeId
, char *type
);

/**
 * Take a snapshot of the currently displaying video in the specified format.
 *
 * NOTE: Do NOT use AsyncIO in the implementation. Refer to
 * #CPL_media_getSnapshotSize for detail.
 *
 * @param nativeId The ID value for the native player instance, as returned by
 *  the open call for the player.
 * @param type String describing the desired encoding format. See the MMAPI
 *  spec for the string format
 * @param data Pointer to where to store the snapshot.
 * @param len The maximum number of snapshot bytes that can be stored.
 *
 * @return The number of snaphot bytes stored, or -1 for failure.
 */
extern jint CPL_media_getSnapshot(
  jint  nativeId
, char  *type
, jbyte *data
, jint  len
);
/*@}*/
#endif /* NEED_VIDEO_INFRASTRUCTURE */

#ifdef SUPPORT_RECORD
/**
 * @defgroup streamingrecording Streaming audio recording
 * @ingroup mediafull
 */

/**
 * @name Functions
 * @ingroup streamingrecording
 */
/*@{*/

/**
 * Read len bytes from native recorder.
 *
 * @param nativeId The an ID value for the native recorder instance,
 *                 as returned by the open call for the recorder
 * @param data Allocated memory
 * @param len Size of allocated data
 *
 * @return Amount of read bytes
 * @ingroup streamingrecording
 */
extern jint CPL_media_readData(
  jint  nativeId
, jbyte *data
, jint  len
);

/*@}*/
#endif /* SUPPORT_RECORD */

#ifdef MEDIA_UPCALL_EVENT

/**
 * @defgroup nativemediaevent Native media event delivery
 */

/**
 * Maximum number of characters that the two strings in the proprietary String
 * Event can have together.
 * If the proprietary String Event is not supported this value should be 0.
 * A port can override this value in targ_media.h.
 */
#ifndef MEDIA_UPCALL_MAX_STRING_LEN
#define MEDIA_UPCALL_MAX_STRING_LEN 0
#endif /* MEDIA_UPCALL_MAX_STRING_LEN */

/**
 * Size of an event in words. The largest complex event is made of 2 strings that are together
 * maximum #MEDIA_UPCALL_MAX_STRING_LEN characters long, and 2 integer.
 * The 2 integer are needed for standard media upcall events with two integer values.
 * See Jbed_upcall_createQueue() on how to calculate the size of a complex event.
 */
#define MEDIA_UPCALL_EVENT_SIZE (JBED_UPCALL_MINEVENTSIZE + 2*JBED_UPCALL_OBJECTHEADERSIZE + JBED_UPCALL_STRINGSIZE(MEDIA_UPCALL_MAX_STRING_LEN) + 2)

/**
 * Maximum number of events that the upcall handler event queue needs to be able to hold.
 * A port can override this value in targ_media.h.
 */
#ifndef MEDIA_UPCALL_NUM_EVENTS
#define MEDIA_UPCALL_NUM_EVENTS 6
#endif /* MEDIA_UPCALL_NUM_EVENTS */

/**
 * Required size of the media upcall handler event queue in words.
 * See Jbed_upcall_createQueue() on how to calculate the size of a queue.
 */
#define MEDIA_UPCALL_EVENT_QUEUE_SIZE (JBED_UPCALL_SPAREQUEUESPACE + (MEDIA_UPCALL_NUM_EVENTS * MEDIA_UPCALL_EVENT_SIZE))

/**
 * The priority of the Media upcall event queue (bigger number means higher priority).
 * A port can override this value in targ_media.h.
 */
#ifndef MEDIA_UPCALL_EVENT_QUEUE_PRIORITY
#define MEDIA_UPCALL_EVENT_QUEUE_PRIORITY 10
#endif /* MEDIA_UPCALL_EVENT_QUEUE_PRIORITY */

/**
 * @name Native event types
 * @ingroup nativemediaevent
 */
/*@{*/
/** native media event type for ERROR*/
#define NATIVE_MEDIA_EVENT_ERROR (0)
/** native media event type for STARTED*/
#define NATIVE_MEDIA_EVENT_STARTED (1)
/** native media event type for STOPPED*/
#define NATIVE_MEDIA_EVENT_STOPPED (2)
/** native media event type for END OF MEDIA*/
#define NATIVE_MEDIA_EVENT_END_OF_MEDIA (3)
/** native media event type for DEVICE UNAVAILABLE*/
#define NATIVE_MEDIA_EVENT_DEVICE_UNAVAILABLE (4)
/** native media event type for DEVICE AVAILABLE*/
#define NATIVE_MEDIA_EVENT_DEVICE_AVAILABLE (5)
/** native media event type for DURATION UPDATE, event data is the media duration in milliseconds */
#define NATIVE_MEDIA_EVENT_DEVICE_DURATION_UPDATE (12)
/** native media event type for VOLUME CHANGED, event data is the new volume level */
#define NATIVE_MEDIA_EVENT_DEVICE_NATIVE_VOLUME_CHANGED (9)
/** native media event type to mute all media */
#define NATIVE_MEDIA_EVENT_MUTE_ALL_MEDIA (14)
/** native media event type to unmute all media */
#define NATIVE_MEDIA_EVENT_UNMUTE_ALL_MEDIA (15)
/*@}*/

/**
 * @name Function
 * @ingroup nativemediaevent
 */
/*@{*/
/**
 * Post native media event to VM.
 *
 * It can safely be called from a non-VM thread, but it is up to the
 * port to make sure no more than one thread calls it at the same time.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *      previous call to <code>CPL_media_open*Dev()</code>
 * @param eventType see native event types
 * @param eventData it is up to eventType to decide what eventData means
 *
 * @ingroup nativemediaevent
 */
GLOBAL void Jbed_mediaSendEvent(
  int nativeId
, int eventType
, int eventData
);

/**
 * Post proprietary native media event to VM.
 *
 * It can safely be called from a non-VM thread, but it is up to the
 * port to make sure no more than one thread calls it at the same time.
 *
 * @param nativeId Native ID value for the media playing slot as returned by a
 *      previous call to <code>CPL_media_open*Dev()</code>
 * @param eventType pointer to jchar array which holds event type string
 * @param typeLength length of eventType
 * @param eventData pointer to jchar array which holds event data string
 * @param dataLength length of eventData
 *
 * @ingroup nativemediaevent
 */
GLOBAL void Jbed_mediaSendProprietaryStringEvent(
  int nativeId
, jchar * eventType
, int typeLength
, jchar * eventData
, int dataLength
);

/**
 * Post simple customized native media event to VM.
 *
 * It can safely be called from a non-VM thread, but it is up to the
 * port to make sure no more than one thread calls it at the same time.
 *
 * @param eventType customized event type
 * @param payload event payload
 *
 * @ingroup nativemediaevent
 */
GLOBAL void Jbed_mediaSendSimpleCustomizedEvent(
  int eventType
, int payload
);

#endif /* MEDIA_UPCALL_EVENT */
/*@}*/

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /*__CPL_MEDIA_H__*/
