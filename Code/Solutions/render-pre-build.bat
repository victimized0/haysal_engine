set SrcDir=%1
set DstDir=%2
set Platform=%3

set FreeImageDir="%SrcDir%..\SDKs\FreeImage\%Platform%"

rem xcopy /y/s %FreeImageDir% %DstDir%