@echo off

if "%1"=="" (
echo please specify cmake directory
) else (

set src_dir=%1

cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release %src_dir%
)



