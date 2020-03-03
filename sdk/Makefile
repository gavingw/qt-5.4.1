BIN = libsdk.so
PWD=$(shell pwd)
include makeconfig
#CONFIG_USE_LOGCAT = true#must set this at Makefile_sub too

MAKE = make
SUBDIRS_1 = $(shell find . -maxdepth 3 -type d)
DIR_ALL_D =  $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.d)) 
DIR_ALL_O = $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.o))

FILDIRS := ./depend% ./obj%
ifneq ($(CONFIG_USE_LOGCAT), true)
FILDIRS += ./liblog% ./logcat%
endif
SUBDIRS = $(filter-out $(FILDIRS), $(shell find . -maxdepth 3 -type d))  #逐层检索所有子目录，去掉顶层目录下的depend、obj

CSRCS =  $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.c))                    #检索所有子目录下的.c文件
CPPSRCS = $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.cpp))                 #检索所有子目录下的.cpp文件
 
testPath = $(shell find ./ | grep "testMp4Muxer")

CSRCS := $(filter-out $(testPath), $(CSRCS))   

testPath2 = $(shell find ./ | grep "xplayerdemo")

CPPSRCS := $(filter-out $(testPath2), $(CPPSRCS)) 

#修改部分 ，设定存放路径的底层目录名称
OBJROOTDIR = ./obj/
COBJS = $(subst ./,$(OBJROOTDIR),$(CSRCS:.c=.o))                         #把所有.c换成.o再加上目录，即编译结果最终存放的地方
CPPOBJS = $(subst ./,$(OBJROOTDIR),$(CPPSRCS:.cpp=.o))                   #把所有.cpp换成.再加上目录，即编译结果最终存放的地方

OBJDIR = $(subst ./,$(OBJROOTDIR),$(SUBDIRS))                            #由于写入时当目录不存在会报错，因此需要在obj/下建立于顶层目录一样的目录

#与OBJ思路一样
DEFROOTDIR = ./depend/
CDEF = $(subst ./,$(DEFROOTDIR),$(CSRCS:.c=.d))
CPPDEF = $(subst ./,$(DEFROOTDIR),$(CPPSRCS:.cpp=.d))
DEFDIR = $(subst ./,$(DEFROOTDIR),$(SUBDIRS))

#在进行编译前先检测相关目录是否存在，不存在则先创建目录
all: 
	@if [ ! -d "depend" ]; then mkdir "depend" ; fi ;
	@for dir in $(DEFDIR);\
	do \
	if [ ! -d "$$dir" ]; then mkdir "$$dir" ; fi ;\
	done  
	@if [ ! -d "obj" ]; then mkdir "obj" ; fi ;
	@for dir in $(OBJDIR);\
	do \
	if [ ! -d "$$dir" ]; then mkdir "$$dir" ; fi ;\
	done  	
	@$(MAKE) -f Makefile_sub                                                 #调用Makefile_sub进行编译

.PHONY:clean cleanall

clean:            
	@rm -f  $(DIR_ALL_O) $(DIR_ALL_D)
	-@rm -rf $(BIN) $(COBJS) $(CPPOBJS) sdktest.o sdktest  testMp4Muxer.o muxtest  xplayerdemo xplayerdemo.o
#rm *.d
cleanall:
	@rm -f  $(DIR_ALL_O) $(DIR_ALL_D)
	@rm -f $(BIN) $(COBJS) $(CPPOBJS) sdktest.o sdktest  testMp4Muxer.o muxtest  xplayerdemo xplayerdemo.o
	@rm -rf depend
	@rm -rf obj
