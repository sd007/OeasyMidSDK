cmake_minimum_required(VERSION 3.2)

set(ProjectName OeasyMidSDK)  
project(${ProjectName})

aux_source_directory(src src_dir)

set(EXECUTABLE_OUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories( "include/" "libs/linux/HikVision/include" "libs/linux/log/include")

link_directories("libs/linux/HikVision/libs" "libs/linux/HikVision/libs/HCNetSDKCom" "libs/linux/log")
link_libraries(AudioRender HCCore hcnetsdk hpr PlayCtrl SuperRender )
link_libraries(analyzedata HCAlarm HCCoreDevCfg HCDisplay HCGeneralCfgMgr HCIndustry HCPlayBack HCPreview HCVoiceTalk iconv2 StreamTransClient SystemTransform)
link_libraries(log4cpp )

set_target_properties(OeasyMidSDK_static PROPERTIES OUTPUT_NAME "OeasyMidSDK")
set_target_properties(OeasyMidSDK_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(OeasyMidSDK PROPERTIES CLEAN_DIRECT_OUTPUT 1)

add_library(OeasyMidSDK SHARED ${src_dir}) 
add_library(OeasyMidSDK STATIC ${src_dir}) 