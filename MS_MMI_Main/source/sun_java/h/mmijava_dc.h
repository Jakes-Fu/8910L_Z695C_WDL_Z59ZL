
typedef enum
{    
    MMIJAVA_VIDEO_RECORDING, //play
    MMIJAVA_VIDEO_RECORDING_PAUSED,  //pause
    MMIJAVA_VIDEO_RECORDING_STOPPED, //stop
    MMIJAVA_VIDEO_RECORDING_STATE_MAX
} MMIJAVA_VIDEO_RECORDING_STATE_E;


typedef enum
{    
    MMIJAVA_CAMERA_UNINIT, //un initialized
    MMIJAVA_CAMERA_SNAPSHOT_WITHOUT_PREVIEW, // not start preview, just snapthot
    MMIJAVA_CAMERA_SNAPSHOT_WITH_PREVIEW, // start preview, then snapshot
    MMIJAVA_CAMERA_DC_PREVIEWING, // start preview
    MMIJAVA_CAMERA_DC_PAUSED,
    MMIJAVA_CAMERA_DV_RECORDING,
    MMIJAVA_CAMERA_DV_PAUSED,
    MMIJAVA_CAMERA_SHUTDOWN // only used for pending state.
} MMIJAVA_CAMERA_STATE_E;

PUBLIC int32 mmijava_dc_get_state(void);

PUBLIC int32 mmijava_dc_get_rect(GUI_RECT_T *rect);

