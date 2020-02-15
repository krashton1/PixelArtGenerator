start godot --path D:\Code\GitHub\PixelArtGenerator\Godot -waitForDebug
timeout 3

rem Get the process ID
for /f "TOKENS=1,2,*" %%a in ('tasklist /FI "WINDOWTITLE eq PixelArtGenerator"') do set MyPID=%%b
rem attach to it with selected debugger
echo %mypid%
vsjitDebugger -p %mypid%

