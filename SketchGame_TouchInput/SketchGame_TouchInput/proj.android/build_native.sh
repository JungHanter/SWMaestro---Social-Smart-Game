# set params
NDK_ROOT=/Installation/android-ndk-r8b/
COCOS2DX_ROOT=/Installation/cocos2d/cocos2d-2.0-rc2-x-2.0.1
GAME_ROOT=/Users/hanter/Programming/SWMaestro/firststep_1st/Social_Smart_Game/SWMaestro---Social-Smart-Game/SketchGame_TouchInput/SketchGame_TouchInput
GAME_ANDROID_ROOT=$GAME_ROOT/proj.android
RESOURCE_ROOT=$GAME_ROOT/Resources

buildexternalsfromsource=

usage(){
cat << EOF
usage: $0 [options]

Build C/C++ native code using Android NDK

OPTIONS:
   -s	Build externals from source
   -h	this help
EOF
}

while getopts "s" OPTION; do
	case "$OPTION" in
		s)
			buildexternalsfromsource=1
			;;
		h)
			usage
			exit 0
			;;
	esac
done

# make sure assets is exist
if [ -d $GAME_ANDROID_ROOT/assets ]; then
    rm -rf $GAME_ANDROID_ROOT/assets
fi

mkdir $GAME_ANDROID_ROOT/assets

# copy resources
for file in $RESOURCE_ROOT/*
do
    if [ -d "$file" ]; then
        cp -rf "$file" $GAME_ANDROID_ROOT/assets
    fi

    if [ -f "$file" ]; then
        cp "$file" $GAME_ANDROID_ROOT/assets
    fi
done

# copy icons (if they exist)
file=$GAME_ANDROID_ROOT/assets/Icon-72.png
if [ -f "$file" ]; then
	cp $file $GAME_ANDROID_ROOT/res/drawable-hdpi/icon.png
fi
file=$GAME_ANDROID_ROOT/assets/Icon-48.png
if [ -f "$file" ]; then
	cp $file $GAME_ANDROID_ROOT/res/drawable-mdpi/icon.png
fi
file=$GAME_ANDROID_ROOT/assets/Icon-32.png
if [ -f "$file" ]; then
	cp $file $GAME_ANDROID_ROOT/res/drawable-ldpi/icon.png
fi


if [[ $buildexternalsfromsource ]]; then
    echo "Building external dependencies from source"
    $NDK_ROOT/ndk-build -C $GAME_ANDROID_ROOT \
        NDK_MODULE_PATH=${COCOS2DX_ROOT}:${COCOS2DX_ROOT}/cocos2dx/platform/third_party/android/source
else
    echo "Using prebuilt externals"
    $NDK_ROOT/ndk-build -C $GAME_ANDROID_ROOT \
        NDK_MODULE_PATH=${COCOS2DX_ROOT}:${COCOS2DX_ROOT}/cocos2dx/platform/third_party/android/prebuilt
fi



