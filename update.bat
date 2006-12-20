@echo off
cls
zip ..\..\..\devel\share\CodeBlocks\wxsmith.zip manifest.xml
zip ..\..\..\output\share\CodeBlocks\wxsmith.zip manifest.xml
REM md ..\..\..\devel\share\CodeBlocks\images\wxsmith > nul 2>&1
REM md ..\..\..\output\share\CodeBlocks\images\wxsmith > nul 2>&1
REM copy smithicons\*.png ..\..\..\devel\share\CodeBlocks\images\wxsmith\ > nul 2>&1
REM copy smithicons\*.png ..\..\..\output\share\CodeBlocks\images\wxsmith\ > nul 2>&1
exit 0
