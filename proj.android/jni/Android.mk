LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared
LOCAL_MODULE_FILENAME := libcocos2dcpp

#traverse all the directory and subdirectory
define walk
  $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

#traverse Classes Directory
ALLFILES = $(call walk, $(LOCAL_PATH)/../../Classes)

FILE_LIST := hellocpp/main.cpp
FILE_LIST += $(filter %.cpp, $(ALLFILES))
FILE_LIST += $(filter %.c, $(ALLFILES))
FILE_LIST += $(filter %.cc, $(ALLFILES))
#FILE_INCLUDES := $(shell find $(LOCAL_PATH)/../../Classes -type d)


#source file will be compiled
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../Classes/json \
					$(LOCAL_PATH)/../../Classes/UI \
					$(LOCAL_PATH)/../../Classes/DataCenter \
					$(LOCAL_PATH)/../../Classes/sdk \
					$(LOCAL_PATH)/../../Classes/network \					
#add: protobuf
LOCAL_C_INCLUDES +=	$(LOCAL_PATH)/../../Classes/network/google/protobuf \
					$(LOCAL_PATH)/../../Classes/network/google/protobuf/io \
					$(LOCAL_PATH)/../../Classes/network/google/protobuf/stubs \					
#add: analytics
LOCAL_C_INCLUDES +=	$(LOCAL_PATH)/../../Classes/sdk/FXPlugin/TalkingData/TalkingDataGameAnalytics/include \
LOCAL_C_INCLUDES +=	$(LOCAL_PATH)/../../Classes/sdk/FXPlugin/DataEye/DataEyeLib/Android/include \


LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)

