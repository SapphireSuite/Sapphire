# Copyright 2020 Sapphire development team. All Rights Reserved.

echo ===== Start Visual Studio 2019 Generation =====

# Setup configuration.
config=$1

while [ "$config" != "Debug" ] && [ "$config" != "Release" ]
do
	echo Enter Build Configuration \[Debug\|Release\]:

	read config
done


# Create generator folder.
mkdir -p Build/VS_2019/$config


# Generate project.
cmake -B Build/VS_2019/$config -DCMAKE_BUILD_TYPE=$config -G "Visual Studio 16 2019"

# Catch generation failure.
if [ $? != 0 ]; then exit 1; fi


# use -b to trigger build after generation.
if [ "$2" == "-b" ]
then
	echo $'\n===== Start Visual Studio 2019 Build ====='
	
	# Build project.
	cmake --build Build/VS_2019/$config --config $config --verbose

	# Catch build failure.
	if [ $? != 0 ]; then exit 1; fi


	# use -t to trigger ctest after build.
	if [ "$3" == "-t" ]
	then
		echo $'\n===== Start Visual Studio 2019 Test ====='

		# ctest must be triggered in built folder.
		cd Build/VS_2019/$config
		
		ctest -C $config -V

		# Catch test failure.
		if [ $? != 0 ]; then exit 1; fi
	fi
fi

exit 0
