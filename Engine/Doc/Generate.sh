# Copyright 2020 Sapphire development team. All Rights Reserved.

# Check valid current directory.
CurrDirName=${PWD##*/}

if [[ "${CurrDirName}" == "Doc" ]]; then
	# Move to Root directory
	cd ../..
elif [[ "${CurrDirName}" == "Engine" ]]; then
	# Move to Root directory
	cd ../
elif [[ "${CurrDirName}" != "Sapphire" ]]; then
	echo Wrong directory: Must be either in Sapphire/, Sapphire/Engine/ or Sapphire/Engine/Doc/
	exit 1
fi


echo ===== Generate Documentation =====

echo Delete obsolete documentation...

rm -rf Engine/Doc/html
rm -rf Engine/Doc/index

echo Generate Sapphire Engine documentation...

# Generate documentation using Doxygen tool
# http://www.doxygen.nl/index.html
doxygen Engine/Doc/doxygen.config

# Open the generated dcumentation.
start Engine/Doc/html/index.html

exit 0
