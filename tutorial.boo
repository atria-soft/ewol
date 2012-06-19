## Build the NDK part ...
cd /home/edupin/progperso/android/ndk-r7/
export NDK_PROJECT_PATH=/home/edupin/progperso/android/ndk-r7/samples/hello-jni/
./ndk-build
## ==> now all is OK


# basic SDK folder :
export PATH=$PATH:/home/edupin/dev/perso/android/sdk/tools/:/home/edupin/dev/perso/android/sdk/platform-tools/

# configure angroide SDK :
# run :
cd ~/dev/perso/android/sdk/tools
sh -x android
#   ==> add the SDK needed for android you will use like android 2.3.3 for xperia X10
# ... wait download ...

#Display list of target ...
android list targets

# need to generate the application APK ...
cd /home/edupin/progperso/android/ndk-r7/samples/hello-jni/
sh -x ~/dev/perso/android/sdk/tools/android update project -p . -s --target 1

#install ant with aptitude ...

ant -Dsdk.dir=/home/edupin/progperso/android/sdk-r15/ release

# pb : warning: 'includeantruntime' is not set
# solve with set :     <javac .... includeantruntime="false" .../> 

# missing javac
# solve : install openjdk

# kill adb
adb kill-server
# install application
sudo /home/edupin/progperso/android/sdk-r15/platform-tools/adb  install ./bin/HelloJni-release-unsigned.apk
# result : Failure [INSTALL_PARSE_FAILED_NO_CERTIFICATES]


export PATH=/usr/lib/lightdm/lightdm:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/home/edupin/.bin:/prog/prog/hexViewer:/home/edupin/progperso/android/sdk-r15/tools/:/home/edupin/progperso/android/sdk-r15/platform-tools/


pb avec la table Sony S:

Faire la manip : 
The following procedure has confirmed that ubuntu 11.04.
1. Enable debug mode for sony tablet S
2. Add line /etc/udev/rules.d/51-android.rules
SUBSYSTEM=="usb", ATTR{idVendor}=="054c", MODE="0666", GROUP="plugdev"
3. Change file mode
sudo chmod a+r /etc/udev/rules.d/51-android.rules
4. Add line idVendor to ~/.android/adb_usb.ini
0x054C
5. adb kill-server ; sudo adb start-server



