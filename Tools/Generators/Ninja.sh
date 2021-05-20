# Copyright 2020 Sapphire development team. All Rights Reserved.

echo ===== Start Ninja Generation =====

# Setup configuration.
config=$1

while [ "$config" != "Debug" ] && [ "$config" != "Release" ]
do
	echo Enter Build Configuration \[Debug\|Release\]:

	read config
done


# Travis config.
travis_config=""

if [ "$4" == "-travis" ]
then
	travis_config="-DCMAKE_TRAVIS=1"
fi


# Create generator folder.
mkdir -p Build/Ninja/$config


# Generate project.
cmake -B Build/Ninja/$config -DCMAKE_BUILD_TYPE=$config -G Ninja $travis_config

# Catch generation failure.
if [ $? != 0 ]; then exit 1; fi


# use -b to trigger build after generation.
if [ "$2" == "-b" ]
then
	echo $'\n===== Start Ninja Build ====='

	# Build project.
	cmake --build Build/Ninja/$config --config $config --verbose

	# Catch build failure.
	if [ $? != 0 ]; then exit 1; fi


	# use -t to trigger ctest after build.
	if [ "$3" == "-t" ]
	then
		echo $'\n===== Start Ninja Test ====='

		# ctest must be triggered in built folder.
		cd Build/Ninja/$config

		ctest -C $config -V

		# Catch test failure.
		if [ $? != 0 ]; then exit 1; fi
	fi
fi

exit 0
