SET VIEW_PATH=E:\guowenli\main_6530
SET MOCOR_LABEL=MOCOR_W11.50

pushd %VIEW_PATH%\MS_Code

START "sc8800g2_openphone_240X400PDA_le pclint" /d %VIEW_PATH%\MS_Code /MAX call make p=sc8800g2_openphone_240X400PDA_le pclint -j16
sleep 5m

xcopy /e /y /i %VIEW_PATH%\MS_Code\build\sc8800g2_openphone_240X400PDA_le_builddir\pclint E:\guowenli\TEMP_VERSION\%MOCOR_LABEL%_Pclint\sc8800g2_openphone_240X400PDA_le_builddir

pushd E:\guowenli\TEMP_VERSION
"C:\Program Files\winrar\rar" a -r %MOCOR_LABEL%_Pclint.rar %MOCOR_LABEL%_Pclint

copy /y %MOCOR_LABEL%_Pclint.rar %VIEW_PATH%\MS_Code\project\management\report\pc-lint\MOCOR_MAINTRUNK
pushd %VIEW_PATH%\MS_Code\project\management\report\pc-lint\MOCOR_MAINTRUNK
