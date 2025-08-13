#
# 'make run dir=file_name'        build executable file 'main'
# 'make clean dir=file_name'  	  removes selected .o and executable files
#

# define the Cpp compiler to use
CXX = g++

# define any compile-time flags
CXXFLAGS	:= -std=c++17 -Wall -Wextra -g

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS = 

# define output directory
OUTPUT	:= output

# define source directory 运行时修改此处路径
SRC		:= src/$(dir) #// 传递 var 变量定义执行文件目录
CLEAN_SRC		:= src/$(dir)/*.o #// 删除所有.o文件

# define include directory
INCLUDE	:= include

# define lib directory
LIB		:= lib

ifeq ($(OS),Windows_NT)
LIBRARIES	:= -lglad -lglfw3dll
MAIN	:= main.exe
SOURCEDIRS	:= $(SRC)
INCLUDEDIRS	:= $(INCLUDE)
LIBDIRS		:= $(LIB)
FIXPATH = $(subst /,\,$1) #这里细节错误
RM			:= del /q /f
MD	:= mkdir
else
LIBRARIES	:= -lglad -lglfw -ldl -lpthread
MAIN	:= main
SOURCEDIRS	:= $(shell find $(SRC) -type d)
INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
LIBDIRS		:= $(shell find $(LIB) -type d)
FIXPATH = $1
RM = rm -f
MD	:= mkdir -p
endif

# define any directories containing header files other than /usr/include
INCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))

# define the C libs
LIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%))

# define the C source files
SOURCES		:= $(wildcard $(patsubst %,%/*.cpp, $(SOURCEDIRS)))

# define the C object files 
OBJECTS		:= $(SOURCES:.cpp=.o)

# define the dependency output files
DEPS		:= $(OBJECTS:.o=.d)

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

OUTPUTMAIN	:= $(call FIXPATH,$(OUTPUT)/$(MAIN))

all: $(OUTPUT) $(MAIN)
	@echo Executing 'all' complete!

$(OUTPUT):
	$(MD) $(OUTPUT)

$(MAIN): $(OBJECTS) 
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS) $(LIBS) $(LIBRARIES)

# include all .d files
-include $(DEPS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# -MMD generates dependency output files same name as the .o file
# (see the gnu make manual section about automatic variables)
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -MMD $<  -o $@

ifeq ($(OS),Windows_NT)
# 转反斜杠并加引号，避免空格/路径错误
fixpath = $(subst /,\,$(1))
RM_FILE = if exist "$(call fixpath,$(1))" del /q /f "$(call fixpath,$(1))"
FIND_CLEAN = for /R "$(call fixpath,$(1))" %%f in ($(2)) do del /q /f "%%f"
MAIN_FILE = $(CURDIR)/$(OUTPUT)/main.exe
else
RM_FILE = rm -f $(1)
FIND_CLEAN = find $(1) -type f -name "$(2)" -delete
MAIN_FILE = $(OUTPUT)/main
endif

.PHONY: clean
clean:
ifeq ($(strip $(dir)),)
	@echo Cleaning all .o and .d in src/...
	@$(call FIND_CLEAN,src,*.o)
	@$(call FIND_CLEAN,src,*.d)
else
	@echo Cleaning .o and .d in src/$(dir)...
	@$(call FIND_CLEAN,src/$(dir),*.o)
	@$(call FIND_CLEAN,src/$(dir),*.d)
endif
	@echo Removing $(MAIN_FILE)...
	@$(call RM_FILE,$(MAIN_FILE))
	@echo Cleanup complete!




# 此处./src/$(dir) 传递main函数 argv 的参数
run: all
	./$(OUTPUTMAIN) src/$(dir)/
	@echo Executing 'run: all' complete!
