PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
M2F=0

OBJS = main.o md5sum.o common.o
ifeq ($(M2F), 1)
#for arm64
CROSS_COMPILE=aarch64-linux-gnu-
CC :=$(CROSS_COMPILE)gcc
CXX :=$(CROSS_COMPILE)g++

TARGET_NAME = m2f
OBJS += m2f.o
CFLAGS = -DM2F
else
TARGET_NAME = f2m
OBJS += f2m.o
endif


INCLUDE_PATH = -I./
CFLAGS += $(INCLUDE_PATH)

LIB_PATH = 
LIB_LINK = 
LFLAGS += $(LIB_PATH) $(LIB_LINK)  
 
all:	$(TARGET_NAME)
	cp $(TARGET_NAME) ./bin/.

$(TARGET_NAME):	$(OBJS) 
	$(CXX) $^ -o $@ $(LFLAGS)	

%.o:	$(PROJECT_ROOT)%.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

%.o:	$(PROJECT_ROOT)%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	rm -f *.o 
	rm -f $(TARGET_NAME) 
