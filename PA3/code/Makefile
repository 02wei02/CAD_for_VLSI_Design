## choose the case directory to process
# target ?= case1
ratio ?= 3 

CC = g++ -std=c++11
SRC := .
OBJ := ./obj
# Define input and output directories
TEST_DIR := ./public/$(target)
INPUT_DIR := $(TEST_DIR)/input
INTER_DIR := $(TEST_DIR)/intermediate
OUTPUT_DIR := $(TEST_DIR)/output

SRCS := $(wildcard $(SRC)/*.cpp)
OBJS := $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(SRCS))


netlist := $(wildcard $(INPUT_DIR)/*.netlist) # <netlist_file>
tf := $(wildcard $(INPUT_DIR)/*.tf) # <tech_file>
sym := $(wildcard $(INPUT_DIR)/*.sym) # <symmetry_constraint_file>
group := $(wildcard $(INPUT_DIR)/*.group) # <device_group_file>
block := $(INTER_DIR)/$(target).block # <building_block_file>
ratio_in := $(wildcard $(TEST_DIR)/*.txt) # <expected_aspect_ratio>
output := /home/CAD112/s109501201/PA3/$(OUTPUT_DIR)/$(target).output # <output_file>
kl := /home/CAD112/s109501201/PA3/$(OUTPUT_DIR)/$(target).output.gds

all: 109501201

#	$< = first dependent file,	 $^ = all dependent file,	 $@ = target file. 
# generate executable file named 109501201 (my student ID)

# generate .o (% 匹配文件名中的模式)
$(OBJ)/%.o: $(SRC)/%.cpp | $(OBJ)
	$(CC) -c $< -o $@

109501201: $(OBJS)
	$(CC) $^ -o $@

# Run BuildingBlockGenerator	
bbg: $(netlist) $(tf) $(group) | $(INTER_DIR)
	BuildingBlockGenerator $^ $(block)

# execute my analog placer to generate the placement results
run:  $(netlist) $(sym) $(block) | $(OUTPUT_DIR)
	./109501201 $(ratio) $^ $(output)

# Define pattern rule for running verifier


# 运行 Verifier 命令，并使用 README 文件内容作为参数
verifier: 
	@content=$$(grep 'expected aspect ratio' $(ratio_in) | awk '{print $$NF}'); \
	Verifier "$$content" $(netlist) $(group) $(sym) $(block) $(output)

# convert the placement results into a GDSII File for visualization purposes
gds:
	GdsGenerator $(netlist) $(tf) $(output)

file: $(OBJS) $(INTER_DIR) $(OUTPUT_DIR)

# open .output
open:
	vi $(output)

# Define variables
REMOTE_USER = s109501201
REMOTE_HOST = 140.115.71.192
REMOTE_PATHS = $(kl) $(output)
LOCAL_PATH = ./

# Target to download the files using scp
d: 109501201 run gds
	$(foreach path, $(REMOTE_PATHS), scp $(REMOTE_USER)@$(REMOTE_HOST):$(path) $(LOCAL_PATH);)
	

$(OBJ):
	mkdir -p $(OBJ)
$(INTER_DIR):
	mkdir -p $(INTER_DIR)
$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)
clean:
	rm -rf $(OBJ)/*.o 109501201


.PHONY: all bg run clean
