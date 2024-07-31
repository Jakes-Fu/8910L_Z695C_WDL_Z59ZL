SET VIEW_PATH=D:\Yihui\yihui.hu_MOCOR10A_view
SET MOCOR_LABEL=MOCOR_10A.W11.44

pushd %VIEW_PATH%\MS_Code

START "sc8800g2_openphone_le pclint" /d %VIEW_PATH%\MS_Code /MAX call make p=sc8800g2_openphone_le pclint -j16
sleep 7m
START "sc8800g_dualsim pclint" /d %VIEW_PATH%\MS_Code /MAX call make p=sc8800g_dualsim pclint -j16
sleep 7m
START "fp880xg2_dualsim pclint" /d %VIEW_PATH%\MS_Code /MAX call make p=fp880xg2_dualsim pclint -j16
sleep 7m
START "sc8800g2_sp8803g_le_modem pclint" /d %VIEW_PATH%\MS_Code /MAX call make p=sc8800g2_sp8803g_le_modem pclint -j16
sleep 7m
START "sc8800g2_openphone_240X400_le pclint" /d %VIEW_PATH%\MS_Code /MAX call make p=sc8800g2_openphone_240X400_le pclint -j16
sleep 7m

START "sc6800h_sp6804h_240x400 pclint" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_sp6804h_240x400 pclint -j16
sleep 5m
START "sc6800h_sp6804h pclint" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_sp6804h pclint -j16
sleep 5m
START "sc6800h_sp6801h pclint" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_sp6801h pclint -j16
sleep 5m
START "sc6800h_openphone_240X400 pclint" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_openphone_240X400 pclint -j16
sleep 5m
START "sc6800h_openphone pclint" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_openphone pclint -j16
sleep 5m
START "sc6800h_sp6801h_nor_128X64 pclint" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_sp6801h_nor_128X64 pclint -j16
sleep 5m
START "sc6800h_s509h pclint" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_s509h pclint -j16
sleep 5m
START "sc6800h_sp6804h_240x320 pclint" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_sp6804h_240x320 pclint -j16
sleep 5m

rem START "sc8800g2_sp8803g_le_NFTL pclint" /d %VIEW_PATH%\MS_Code /MAX call make p=sc8800g2_sp8803g_le_NFTL pclint -j16
rem sleep 5m
rem START "sc6800h_sp6801h_nor_128X64 pclint" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_sp6801h_nor_128X64 pclint -j16
rem sleep 5m
rem START "sc8800g_cmmb pclint" /d %VIEW_PATH%\MS_Code /MAX call make p=sc8800g_cmmb pclint -j16
rem sleep 5m
rem START "sc6800h_sp6803h_quasim pclint" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_sp6803h_quasim pclint -j16
rem sleep 5m
rem START "sc6800h_sp6801h_176X220 pclint" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_sp6801h_176X220 pclint -j16
rem sleep 5m
rem START "sc6800h_sp6801h_slide pclint" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_sp6801h_slide pclint -j16
rem sleep 5m

call make p=sc8800g2_sp8803g_le pclint -j16
rem xcopy /e /y /i %VIEW_PATH%\MS_Code\build\sc8800g_sp8803g_builddir\pclint D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Pclint\sc8800g_sp8803g_builddir

xcopy /e /y /i %VIEW_PATH%\MS_Code\build\sc8800g2_openphone_le_builddir\pclint D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Pclint\sc8800g2_openphone_le_builddir
xcopy /e /y /i %VIEW_PATH%\MS_Code\build\sc8800g2_sp8803g_le_builddir\pclint D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Pclint\sc8800g2_sp8803g_le_builddir
xcopy /e /y /i %VIEW_PATH%\MS_Code\build\sc8800g_dualsim_builddir\pclint D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Pclint\sc8800g_dualsim_builddir
xcopy /e /y /i %VIEW_PATH%\MS_Code\build\sc8800g2_sp8803g_le_modem_builddir\pclint D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Pclint\sc8800g2_sp8803g_le_modem_builddir
xcopy /e /y /i %VIEW_PATH%\MS_Code\build\sc8800g2_openphone_240X400_le_builddir\pclint D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Pclint\sc8800g2_openphone_240X400_le_builddir
xcopy /e /y /i %VIEW_PATH%\MS_Code\build\fp880xg2_dualsim_builddir\pclint D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Pclint\fp880xg2_dualsim_builddir

rem xcopy /e /y /i %VIEW_PATH%\MS_Code\build\sc8800g2_sp8803g_le_modem_builddir\pclint D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Pclint\sc8800g2_sp8803g_le_modem_builddir
rem xcopy /e /y /i %VIEW_PATH%\MS_Code\build\sc8800g2_sp8803g_le_iajava_builddir\pclint D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Pclint\sc8800g2_sp8803g_le_iajava_builddir
rem xcopy /e /y /i %VIEW_PATH%\MS_Code\build\sc8800g_cmmb_builddir\pclint D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Pclint\sc8800g_cmmb_builddir
rem xcopy /e /y /i %VIEW_PATH%\MS_Code\build\sc8800g2_openphone_240X400_le_builddir\pclint D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Pclint\sc8800g2_openphone_240X400_le_builddir

xcopy /e /y /i %VIEW_PATH%\MS_Code\build\sc6800h_s509h_builddir\pclint D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Pclint\sc6800h_s509h_builddir
xcopy /e /y /i %VIEW_PATH%\MS_Code\build\sc6800h_sp6801h_builddir\pclint D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Pclint\sc6800h_sp6801h_builddir
xcopy /e /y /i %VIEW_PATH%\MS_Code\build\sc6800h_sp6801h_nor_128X64_builddir\pclint D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Pclint\sc6800h_sp6801h_nor_128X64_builddir
xcopy /e /y /i %VIEW_PATH%\MS_Code\build\sc6800h_openphone_240X400_builddir\pclint D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Pclint\sc6800h_openphone_240X400_builddir
xcopy /e /y /i %VIEW_PATH%\MS_Code\build\sc6800h_openphone_builddir\pclint D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Pclint\sc6800h_openphone_builddir
xcopy /e /y /i %VIEW_PATH%\MS_Code\build\sc6800h_sp6804h_builddir\pclint D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Pclint\sc6800h_sp6804h_builddir
xcopy /e /y /i %VIEW_PATH%\MS_Code\build\sc6800h_sp6804h_240x400_builddir\pclint D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Pclint\sc6800h_sp6804h_240x400_builddir
xcopy /e /y /i %VIEW_PATH%\MS_Code\build\sc6800h_sp6804h_240x320_builddir\pclint D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Pclint\sc6800h_sp6804h_240x320_builddir

rem xcopy /e /y /i %VIEW_PATH%\MS_Code\build\sc6800h_sp6803h_quasim_builddir\pclint D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Pclint\sc6800h_sp6803h_quasim_builddir
rem xcopy /e /y /i %VIEW_PATH%\MS_Code\build\sc6800h_sp6801h_slide_builddir\pclint D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Pclint\sc6800h_sp6801h_slide_builddir
rem xcopy /e /y /i %VIEW_PATH%\MS_Code\build\sc6800h_sp6801h_176X220_builddir\pclint D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Pclint\sc6800h_sp6801h_176X220_builddir

pushd D:\Yihui\TMP_VERSION
"C:\Program Files\winrar\rar" a -r %MOCOR_LABEL%_Pclint.rar %MOCOR_LABEL%_Pclint

copy /y %MOCOR_LABEL%_Pclint.rar %VIEW_PATH%\MS_Code\project\management\report\pc-lint\MOCOR_10A
pushd %VIEW_PATH%\MS_Code\project\management\report\pc-lint\MOCOR_10A
cleartool co -nc .
cleartool mkelem -c "add %MOCOR_LABEL%_Pclint.rar" -ci %MOCOR_LABEL%_Pclint.rar
cleartool ci -c "add %MOCOR_LABEL%_Pclint.rar" .
cleartool mklabel -replace %MOCOR_LABEL% . .\%MOCOR_LABEL%_Pclint.rar

cleartool unlock brtype:mocor_10a@\AdminVOB
