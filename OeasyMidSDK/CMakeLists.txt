cmake_minimum_required(VERSION 3.2)

set(ProjectName OeasyMidSDK)  
project(${ProjectName})


add_definitions(-D_USE_IPC -D_USE_INTERCOM -D_USE_ALARMHOST)

FILE(GLOB_RECURSE sdk_srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

include_directories( "src/common"
	             "src/device"
                     "src/device/alarmhost"
		     "src/device/alarmhost/hikvision"
                     "src/device/camera"
		     "src/device/camera/dahua"
                     "src/device/camera/hikvision"
		     "src/device/intercom"
                     "src/device/intercom/oeasyintercom"
                     "src/platform"
                     "src/platform/thread"
					 "src/platform/tick"
                     "src/tools"
                     "src/tools/log"
					 "src/tools/json"
)

set(EXECUTABLE_OUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories( "include/" "libs/linux/HikVision/include" "libs/linux/log/include")
include_directories( "libs/linux/jsoncpp/include" "libs/linux/DaHua/include")

link_directories("libs/linux/HikVision/libs" "libs/linux/HikVision/libs/HCNetSDKCom" "libs/linux/log")
link_directories("libs/linux/jsoncpp/libs" "libs/linux/DaHua/libs")
link_libraries(AudioRender HCCore hcnetsdk hpr PlayCtrl SuperRender )
link_libraries(analyzedata HCAlarm HCCoreDevCfg HCDisplay HCGeneralCfgMgr HCIndustry HCPlayBack HCPreview HCVoiceTalk iconv2 StreamTransClient SystemTransform)
link_libraries(log4cpp )
link_libraries(avnetsdk  dhconfigsdk dhnetsdk Infra NetFramework Stream StreamSvr)
link_libraries(json_linux-gcc-5.4.0_libmt)

add_library(OeasyMidSDK SHARED ${sdk_srcs}) 

#add_library(OeasyMidSDK STATIC ${sdk_srcs}) 
#set_target_properties(OeasyMidSDK_static PROPERTIES OUTPUT_NAME "OeasyMidSDK")
#set_target_properties(OeasyMidSDK_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
#set_target_properties(OeasyMidSDK PROPERTIES CLEAN_DIRECT_OUTPUT 1)




