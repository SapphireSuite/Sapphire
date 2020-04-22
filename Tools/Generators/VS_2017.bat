:: Copyright 2020 Sapphire development team. All Rights Reserved.

@echo off

echo ===== Start Visual Studio 2017 Generation =====

:: Query parameters.
set op_build=0
set op_test=0
set op_npause=0
set op_nexit=0

for %%x in (%*) do (
	:: use -b to trigger build after generation.
	if "%%x"=="-b" set op_build=1

	:: use -t to trigger ctest after build.
	if "%%x"=="-t" set op_test=1

	:: use -np to never trigger pause.
	if "%%x"=="-np" set op_npause=1

	:: use -ne to never trigger exit.
	if "%%x"=="-ne" set op_nexit=1
)


:: Setup configuration.
set config=%1

:loop_config
echo Build Configuration: %config%

if NOT "%config%"=="Debug" (
	if NOT "%config%"=="Release" (
		set /p config=Enter Build Configuration [Debug^|Release]:
		goto loop_config
	)
)

:: Create generator folder.
mkdir Build\VS_2017\%config%


:: Generate project.
cmake -B Build\VS_2017\%config% -DCMAKE_BUILD_TYPE=%config% -G "Visual Studio 15 2017"

:: Catch generation failure.
call :catch_error


if %op_build%==1 (
	echo. & echo ===== Start Visual Studio 2017 Build =====

	:: Build project.
	cmake --build Build\VS_2017\%config% --config %config% --verbose

	:: Catch build failure.
	call :catch_error

	if %op_test%==1 (
		echo. & echo ===== Start Visual Studio 2017 Test =====
		
		:: ctest must be triggered in built folder.
		cd Build\VS_2017\%config%

		ctest -C %config% -V

		:: Move back to root folder.
		cd ..\..\..\

		:: Catch test failure.
		call :catch_error
	)
)

call :try_pause

if %op_nexit%==0 exit 0
goto:eof



:: Catch error function
:catch_error
if errorlevel==1 (
	call :try_pause
	if %op_nexit%==0 exit 1
)

goto:eof
::catch_error end


:: Try to pause (ie: "-np option not set").
:try_pause
if %op_npause%==0 pause

goto:eof
::try_pause end
