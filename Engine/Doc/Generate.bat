:: Copyright 2020 Sapphire development team. All Rights Reserved.

@echo off

:: Check valid current directory.
for %%I in (.) do set CurrDirName=%%~nxI

if "%CurrDirName%"=="Doc" (
	:: Move to Root directory
	cd ../../
) else (
	if "%CurrDirName%"=="Engine" (
		:: Move to Root directory
		cd ../
	) else (
		if not "%CurrDirName%"=="Sapphire" (
			echo Wrong directory: Must be either in Sapphire/, Sapphire/Engine/ or Sapphire/Engine/Doc/
			exit 1
		)
	)
)


echo ===== Generate Documentation =====

echo Delete obsolete documentation...

rmdir /s /q Engine\Doc\html
rmdir /s /q Engine\Doc\latex


echo Generate Sapphire Engine documentation...

:: Generate documentation using Doxygen tool
:: Reference: http://www.doxygen.nl/index.html
doxygen Engine\Doc\doxygen.config

:: Open the generated dcumentation.
start Engine\Doc\html\index.html

exit 0
