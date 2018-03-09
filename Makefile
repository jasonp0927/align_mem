APPNAME					= simple_gpu_player
DESTDIR			        = ./

DEL_FILE				= rm -rf
INSTALL_FILE            = install -m 644 -p
INSTALL_PROGRAM         = install -m 755 -p
INSTALL_DIR             = cp -f -R
CP_FILE			        = cp -rf      
ROOTFS_DIR              = /opt/fsl-imx-fb/3.14.52-1.1.1/sysroots/cortexa9hf-vfp-neon-poky-linux-gnueabi

TARGET_PATH_LIB 	    = $(ROOTFS_DIR)/usr/lib
TARGET_PATH_INCLUDE     = $(ROOTFS_DIR)/usr/include

#APP_INC                 = ./

DEFINES                 = -DLINUX
CFLAGS                  = -pipe $(OE_QMAKE_CFLAGS) -std=c++11 -g -DLINUX=1 -DEGL_API_FB=1 -Wall -W -D_REENTRANT -fPIC $(DEFINES) -fpermissive -g

CFLAGS                 += -DEGL_API_FB -DGPU_TYPE_VIV -DGL_GLEXT_PROTOTYPES -DENABLE_GPU_RENDER_20 \
			              -I. \
			              -I$(TARGET_PATH_INCLUDE)/HAL \
			              -I$(TARGET_PATH_INCLUDE) \
                          -I$(TARGET_PATH_INCLUDE)/glib-2.0 \
			              -I$(TARGET_PATH_LIB)/glib-2.0/include \
						  -fdiagnostics-color=auto	\
						  -I${ROOTFS_DIR}/usr/include/gstreamer-1.0
#						  -I$(APP_INC)
			
LFLAGS			        = -Wl,--library-path=$(TARGET_PATH_LIB),-rpath-link=$(TARGET_PATH_LIB) -lm \
			              -lEGL -lCLC -lGLSLC -lVDK -lGLESv2 -lpthread \
			              -lgobject-2.0 -lgmodule-2.0  -lrt -lxml2 -lglib-2.0  -lstdc++ -lGAL -lpng -lz
			   
LFLAGS                 += -lg2d -lgstreamer-1.0 -lgstapp-1.0

# LFLAGS += -lopencv_calib3d
# LFLAGS += -lopencv_highgui
# LFLAGS += -lopencv_core
# LFLAGS += -lopencv_imgproc
# LFLAGS += -lopencv_gpu
# LFLAGS += -lopencv_objdetect

#

LFLAGS += -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui  -lopencv_videoio -lopencv_video

LFLAGS += -lopencv_objdetect -ltbbmalloc_proxy -ltbbmalloc -ltbb -lBSDNet


STRIP= arm-poky-linux-gnueabi-strip


#OBJ

AllDirs := $(shell ls -R | grep '^\./.*:$$' | awk '{gsub(":","");print}') .  
Sources := $(foreach n,$(AllDirs) , $(wildcard $(n)/*.cpp))  
OBJECTS := $(patsubst %.cpp,%.o, $(Sources))
Sources := $(foreach n,$(AllDirs) , $(wildcard $(n)/*.c))  
OBJECTS += $(patsubst %.c,%.o, $(Sources))
Sources := $(foreach n,$(AllDirs) , $(wildcard $(n)/*.S))  
OBJECTS += $(patsubst %.S,%.o, $(Sources))

# OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
# OBJECTS += $(patsubst %.cpp, %.o, $(wildcard *.cpp))
# OBJECTS += $(patsubst %.S, %.o, $(wildcard *.S))

first: all

all: $(APPNAME)

$(APPNAME): $(OBJECTS)
	$(CXX) $(LFLAGS) -o $@ $^
		
%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.o : %.S
	$(CXX) $(CFLAGS) -o $@ -c $<
	
install: all
	@test -d /tmp/approot || mkdir -p /tmp/approot
	#-$(INSTALL_DIR) skin appres /tmp/approot/
	-$(STRIP) $(APPNAME)
	-$(INSTALL_PROGRAM) $(APPNAME) /tmp/approot/
	
	
updatebyNet:all
	#scp -r $(APPNAME) skin/ appres/ root@192.168.16.73:/home/root/car360/
	#scp ${APPNAME}  root@192.168.16.73:/mnt/mmcblk1p3/testbsd

clean:
	-$(DEL_FILE) $(OBJECTS) $(APPNAME)

distclean: clean

