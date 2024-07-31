set VIEW_PATH=%1
set RESULT_PATH=%2
rd /q/s %RESULT_PATH%\Report
del /f %RESULT_PATH%\assertInfoFile.xls
del /f %RESULT_PATH%\verifiedAssertInfoFile.xls
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\chip_drv %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\CMMB %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\common %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\DAPS %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\dm_2g_layer1 %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\dm_2g_layer1_dsp %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\dm_2g_ps %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\DSP_CEVAX %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\external %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\fdl_bootloader %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\MS_Customize %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\MS_MMI %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\MS_Ref %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\PARSER %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\pri_code %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\RTOS %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\spload %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\Third-party %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\tools %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\version %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\wre %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\atc %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\char_lib %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\l4 %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\Layer1 %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\layer1_dsp %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\PS %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\logsave %RESULT_PATH% new mt module 0
cqperl assertMacroCheck.pl %VIEW_PATH%\MS_Code\sim %RESULT_PATH% new mt module 0
