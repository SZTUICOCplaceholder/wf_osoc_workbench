TOP_MODULE    := ysyx_25060166CPU 
VERILATOR     := verilator

VL_SRC_DIR    := vcode
TB_SRC_DIR    := src
BUILD_DIR     := build/verilator
HEADER_BUILD_DIR := $(BUILD_DIR)/header  # 新增头文件目录
INC_DIR       := include      
VINC_DIR      := vcode/include 

VL_SRCS       := $(shell find $(abspath $(VL_SRC_DIR)) -name "*.v" -or -name "*.sv")
SRC	          := $(shell find $(abspath $(TB_SRC_DIR)) -name "*.c" -or -name "*.cc" -or -name "*.cpp")
MAIN_FILE	  := simulation.cpp
MAIN_FILE_PATH:= $(TB_SRC_DIR)/$(MAIN_FILE)
SRC_NOM		  := $(filter-out $(MAIN_FILE_PATH), $(SRC))
TB_SRCS 	   = $(SRC_NOM) $(MAIN_FILE_PATH)

SIM_EXE       := $(BUILD_DIR)/V$(TOP_MODULE)
VL_HEADER     := $(HEADER_BUILD_DIR)/V$(TOP_MODULE).h  # 修改头文件路径
CPU_CORES     ?= 8

# 阶段1参数：仅生成头文件
VL_HEADER_FLAGS := --cc \
                 --top-module $(TOP_MODULE) \
                 --Mdir $(HEADER_BUILD_DIR) \
                 -Wall \
                 -I$(VINC_DIR)

# 阶段2参数：完整编译（移除 --dpi-hdr-only）
VL_FULL_FLAGS  := --cc --exe --build -j $(CPU_CORES) \
                 --trace \
                 --top-module $(TOP_MODULE) \
                 --Mdir $(BUILD_DIR) \
                 -Wall \
                 -I$(INC_DIR) \
                 -I$(VINC_DIR) \
                 -CFLAGS "-I$(INC_DIR) -std=c++17" 

# 阶段1：生成头文件
$(VL_HEADER): $(VL_SRCS)
	@echo "===== 阶段1：生成Verilator头文件 ====="
	@mkdir -p $(HEADER_BUILD_DIR)
	@$(VERILATOR) $(VL_HEADER_FLAGS) $(VL_SRCS)

# 阶段2：完整编译
$(SIM_EXE): $(VL_HEADER) $(TB_SRCS)
	@echo "===== 阶段2：完整编译Verilog+CPP ====="
	@mkdir -p $(BUILD_DIR)
	@$(VERILATOR) $(VL_FULL_FLAGS) $(VL_SRCS) $(TB_SRCS) \
        -CFLAGS "-I$(HEADER_BUILD_DIR) -I$(INC_DIR) -std=c++17"

compile: $(SIM_EXE)

run: compile
	./$(SIM_EXE)

wave:
	gtkwave $(BUILD_DIR)/V$(TOP_MODULE).vcd

clean:
	rm -rf $(BUILD_DIR)

all: run

.PHONY: all compile run wave clean
