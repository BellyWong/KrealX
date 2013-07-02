LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                  ../../Classes/b2dJson/b2dJson.cpp \
		   ../../Classes/b2dJson/b2dJsonImage.cpp \
		   ../../Classes/b2dJson/jsoncpp.cpp \
		   ../../Classes/BaseRubeLayer.cpp \
		   ../../Classes/Extensions/SlidingMenu.cpp \
		  ../../Classes/BikeLayer.cpp \
		   ../../Classes/RubeImageInfo.cpp \
		   ../../Classes/AppDelegate.cpp \
		   ../../Classes/HelloWorldScene.cpp \
		   ../../Classes/GLES-Render.cpp \
		   ../../Classes/B2DebugDrawLayer.cpp \
		  ../../Classes/Extensions/SlidingMenuTestScene.cpp

# for some reason you need box2d in cocos2d-2.1rc0-x-2.1.3/external/Box2D
# and in KrealX/libs/Box2D
# don't forget to update Android.mk file when changing versions

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../libs/Box2D \
$(LOCAL_PATH)/../../libs/b2dJson \
$(LOCAL_PATH)/../../Classes                   

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static box2d_static
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions) $(call import-module,external/Box2D) 
