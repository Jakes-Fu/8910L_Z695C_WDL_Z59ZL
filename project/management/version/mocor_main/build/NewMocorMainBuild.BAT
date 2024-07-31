SET VIEW_PATH=E:\guowenli\main_6530
pushd %VIEW_PATH%

rd /s /q %VIEW_PATH%\MS_Code\build
rd /s /q %VIEW_PATH%\MS_Code\lib
rd /s /q %VIEW_PATH%\MS_Code\ARMRomCode\seine\arm\drv_sc6600v\build
rem rd /s /q %VIEW_PATH%\MS_Code\ARMRomCode\seine\arm\drv_sc6600v\8801h_h5
rd /s /q %VIEW_PATH%\MS_Code\ARMRomCode\seine\arm\drv_sc6600v\lib
rd /s /q %VIEW_PATH%\MS_Code\CMMB\source\mtv_adapter\inc
rd /s /q %VIEW_PATH%\MS_Code\sim\source\test\SIMConPad\Debug
rd /s /q %VIEW_PATH%\MS_Code\sim\source\test\SIMConPad\Release
rd /s /q %VIEW_PATH%\MS_Code\MS_MMI\source\winsimulator_tp\win_platform\GUI\Debug
rd /s /q %VIEW_PATH%\MS_Code\MS_MMI\source\winsimulator_tp\win_platform\GUI\Release
rd /s /q %VIEW_PATH%\MS_Code\MS_MMI\source\winsimulator_tp\win_platform\MSKernel\MSBridge\Debug
rd /s /q %VIEW_PATH%\MS_Code\MS_MMI\source\winsimulator_tp\win_platform\MSKernel\MSBridge\Release
rd /s /q %VIEW_PATH%\MS_Code\MS_MMI\source\winsimulator_tp\win_platform\ControlPanel_bl7\Debug
del /s /q %VIEW_PATH%\MS_Code\MS_MMI\source\winsimulator_tp\win_platform\ControlPanel_bl7\MS_Log.txt
rd /s /q %VIEW_PATH%\MS_Code\MS_MMI\source\winsimulator_tp\win_platform\ControlPanel_bl7\Release
rd /s /q %VIEW_PATH%\MS_Code\MS_MMI\Common
rd /s /q %VIEW_PATH%\MS_Code\MS_MMI\source\winsimulator_tp\win_platform\filesys\FileSystem___Win32_Debug_Unicode
rd /s /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\LogelSidedPS\LogelSidedPS___Win32_Release_For_MOCOR_DM
rd /s /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\LogelSidedPS\LogelSidedPS___Win32_Release_For_MOCOR_DM_BASE
rd /s /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\Logel\Logel___Win32_Release_For_MOCOR_DM
rd /s /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\LogelSidedPS\Debug
rd /s /q %VIEW_PATH%\MS_Code\MS_MMI\source\winsimulator_tp\win_platform\filesys\Release
rd /s /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\src
rd /s /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\src
rd /s /q %VIEW_PATH%\MS_Code\PS\source\sdl_project\gen
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\External\rlc_t4code.c
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\Logel\Logel.ncb
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\Logel\Logel.opt
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\Logel\Logel.plg
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\Logel\Logel_Mocor.ncb
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\Logel\Logel_Mocor.opt
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\Logel\Logel_Mocor_DM.ncb
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\Logel\Logel_Mocor_DM.opt
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\LogelSidedPS\LogelSidedPS.plg
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\LogelSidedPS\LogelSidedPS_Mocor_DM.plg
rem del /f /s /q *.keep*

pushd %VIEW_PATH%
rem svn update
cls

rem pause
pushd %VIEW_PATH%\MS_Code

call make p=sc8800g2_openphone_240x400PDA_le new job=16 -j4
rem START "sc8800g2_openphone_240x400PDA_le logel building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc8800g2_openphone_240x400PDA_le m=logel
sleep 1m
START "sc8800g2_openphone_240x400PDA_le simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc8800g2_openphone_240x400PDA_le m=simulator run=1 ib=1
sleep 1m
pushd %VIEW_PATH%\MS_Code\project\management\version\mocor_main\build
call r2_SendMailARM.bat
