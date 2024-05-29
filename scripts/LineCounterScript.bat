@echo off
set "directory=../ServerCrow"  REM Specify your directory path here
set "extensions=*.hpp *.cpp *.js *.css *.html"
set "excludedFile=crow_all.hpp"
set /a "totalLines=0"

for /r "%directory%" %%f in (%extensions%) do (
    if /i not "%%~nxf"=="%excludedFile%" (
        echo %%f | findstr /i /c:"\node_modules\" > nul && (
            echo Skipping: %%f
        ) || (
            for /f %%l in ('type "%%f" ^| find /c /v ""') do (
                set /a "totalLines+=%%l"
                echo Processing: %%f - %%l lines
            )
        )
    )
)

echo Total lines of code: %totalLines%
pause
endlocal
