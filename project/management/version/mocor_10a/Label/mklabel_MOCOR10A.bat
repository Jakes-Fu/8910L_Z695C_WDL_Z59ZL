SET VIEW_PATH=D:\Yihui\yihui.hu_MOCOR10A_view
SET MOCOR_LABEL=MOCOR_10A.W11.44

#cleartool mklbtype -global -nc %MOCOR_LABEL%@\AdminVOB
#cleartool lock -replace -nusers mingjian.liu,yihui.hu,cmadmin,nora.yu,tyler.song,xiaodong.li,martin.jiang,younan.wang,zhengfei.xiao lbtype:%MOCOR_LABEL%@\AdminVOB

pushd %VIEW_PATH%\MS_Code
cleartool find . -follow -type fd -ele "brtype(mocor_10a)" -exec "cmd /c cleartool mklabel -replace -nc -follow %MOCOR_LABEL% \"%%CLEARCASE_PN%%\""
cleartool mklabel -replace -nc %MOCOR_LABEL% MS_MMI\source\resource\mmi_res_240x320\str_table.xls
cleartool mklabel -replace -nc -r %MOCOR_LABEL% tools
cleartool mklabel -replace -nc -r %MOCOR_LABEL% MS_MMI\source\winsimulator_tp
cleartool mklabel -replace -nc -r %MOCOR_LABEL% P_TianJi2\Source\msd\build\obj\gen
cleartool mklabel -replace -nc %MOCOR_LABEL% P_TianJi2\Source\msd\build\libuetoolsup.lib 
cleartool mklabel -replace -nc %MOCOR_LABEL% P_TianJi2\Source\msd\build\libuetoolsup.so
cleartool mklabel -replace -nc %MOCOR_LABEL% P_TianJi2\Source\infra\Common\Bin\ms_offsize.dat
cleartool mklabel -replace -nc %MOCOR_LABEL% P_TianJi2\Source\infra\Common\Bin\ms_offsize_little_endian.dat
cleartool mklabel -replace -nc %MOCOR_LABEL% P_TianJi2\Source\msd\build\rps_config.mk
cleartool mklabel -replace -r -follow -nc %MOCOR_LABEL% Third-party\formal\ia_java
cleartool mklabel -replace -r -follow -nc %MOCOR_LABEL% Third-party\cstar
cleartool mklabel -replace -r -follow -nc %MOCOR_LABEL% Third-party\Source
cleartool mklabel -replace -r -follow -nc %MOCOR_LABEL% Third-party\dm
cleartool mklabel -replace -r -follow -nc %MOCOR_LABEL% Third-party\hw
cleartool mklabel -replace -r -follow -nc %MOCOR_LABEL% Third-party\sogou
cleartool mklabel -replace -r -follow -nc %MOCOR_LABEL% Third-party\t9
cleartool mklabel -replace -follow -nc %MOCOR_LABEL% %VIEW_PATH%\MS_APP\Third-party
cleartool mklabel -replace -r -follow -nc %MOCOR_LABEL% %VIEW_PATH%\MS_APP\Third-party\IA_JAVA
cleartool mklabel -replace -r -follow -nc %MOCOR_LABEL% %VIEW_PATH%\MS_APP\Third-party\Dm
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\MS_APP\mmk_modu\source\gui
pause
pause
exit
quit
exit
pause
pause
pushd %VIEW_PATH%\MS_Code\make\perl_script\ResGen
cleartool mklabel -replace -recurse -nc %MOCOR_LABEL% .

pushd %VIEW_PATH%\MS_Code\tools
cleartool mklabel -replace -recurse -nc %MOCOR_LABEL% .

pushd %VIEW_PATH%\MS_Code\atc_3g
cleartool mklabel -replace -recurse -nc %MOCOR_LABEL% .

pushd %VIEW_PATH%\MS_Code\dm_2g_layer1
cleartool mklabel -replace -recurse -nc %MOCOR_LABEL% .

pushd %VIEW_PATH%\MS_Code\dm_2g_layer1_dsp
cleartool mklabel -replace -recurse -nc %MOCOR_LABEL% .

pushd %VIEW_PATH%\MS_Code\dm_2g_ps
cleartool mklabel -replace -recurse -nc %MOCOR_LABEL% .

pushd %VIEW_PATH%\MS_Code\common\nv_parameters\DownloadParam
cleartool mklabel -replace -recurse  -nc %MOCOR_LABEL% .

pushd %VIEW_PATH%\MS_Code\common\nv_parameters\Calibrationparam
cleartool mklabel -replace -recurse -nc %MOCOR_LABEL% .

pushd %VIEW_PATH%\MS_Code\common\nv_parameters\nv_type_td
cleartool mklabel -replace -recurse -nc %MOCOR_LABEL% .

cleartool mklabel -replace %MOCOR_LABEL% -nc %VIEW_PATH%\MS_Code\MS_MMI\source\resource\mmi_res_240x320\str_table.xls

pushd %VIEW_PATH%\Tools\Diagnostic\Logel\Source_tt_2
cleartool mklabel -replace -recurse -nc %MOCOR_LABEL% .

pushd %VIEW_PATH%\MS_Code\MS_MMI\source\winsimulator_tp\win_platform
cleartool mklabel -replace -recurse -nc %MOCOR_LABEL% .

cleartool mklabel -replace %MOCOR_LABEL% -nc %VIEW_PATH%\MS_Code\make\perl_script\create_msdev_ini.pl
cleartool mklabel -replace %MOCOR_LABEL% -nc %VIEW_PATH%\Tools\Common\Bin\MsgDefBuild\Build_dm_2g.bat

pushd %VIEW_PATH%\MS_Code\common\export\inc
cleartool mklabel -replace -recurse -nc %MOCOR_LABEL% .

pushd %VIEW_PATH%\MS_Code\common\nv_parameters\audio
cleartool mklabel -replace -recurse -nc %MOCOR_LABEL% .

pushd %VIEW_PATH%\MS_Code\common\nv_parameters\nv_type
cleartool mklabel -replace -recurse -nc %MOCOR_LABEL% .

pushd %VIEW_PATH%\MS_Code\common\nv_parameters\ProductionParam
cleartool mklabel -replace -recurse -nc %MOCOR_LABEL% .

pushd %VIEW_PATH%\MS_Code\common\nv_parameters\nv_type_td
cleartool mklabel -replace -recurse -nc %MOCOR_LABEL% .

cleartool mklabel -replace -recurse -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Diagnostic\ChannelServer\Source
cleartool mklabel -replace -recurse -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Diagnostic\Logel
cleartool mklabel -replace -recurse -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Release\Logel
cleartool mklabel -replace -recurse -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Resource
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Lib\LogelSidedPS.lib
cleartool mklabel -replace -recurse -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Include
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Bin\Message_Define.def
cleartool mklabel -replace -recurse -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Bin\MsgDefBuild
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Bin\task_id_header_gen.h
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Bin\signal_id_header_gen.h
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Bin\LogelSidedPS.dll
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Bin\Logel.exe
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Bin\CJ609Libd.dll
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Bin\CommSock.dll
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Bin\DPSrv.dll
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Bin\logel.ini
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Lib\CJ609Libd.lib
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Lib\DPSrv.lib
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Lib\CommSock.lib
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Lib\CJ609Lib.lib
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Lib\DPSrvd.lib
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Lib\LogelSidedPSd.lib
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Lib\CommSockd.lib
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Bin\CommSockd.dll
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Bin\DPSrvd.dll
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Bin\LogelSidedPSd.dll
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Lib\CrashReportd.lib
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Lib\tcl80.lib
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Bin\ASN1DPSrv.lib
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Bin\ASN1DPSrv_sys.lib
cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\Tools\Common\Lib\CrashReport.lib

cleartool mklabel -replace -nc %MOCOR_LABEL% %VIEW_PATH%\MS_Code\make\perl_script\create_msdev_ini.pl
