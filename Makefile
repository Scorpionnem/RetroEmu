NAME := RetroEmu

CXX := g++
CXXFLAGS := -g -MP -MMD -Wall -Wextra -Werror -std=c++26 -freflection -O2

INC_DIR := include
SRC_DIR := src
OBJ_DIR := .obj

SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LIBS := $(shell sdl2-config --libs)

CXXFLAGS += $(SDL_CFLAGS) -I$(INC_DIR)
LFLAGS := $(SDL_LIBS)

_ := $(shell find $(SRC_DIR)/ -type f -name '*.cpp' | LC_ALL=C sort | sed 's/^src\//SRCS += /' > sources.mk)
_ := $(shell find $(SRC_DIR)/ -type d | LC_ALL=C sort | sed 's/^src\//SRCS_DIR += /' >> sources.mk)

CXXFLAGS += $(SRCS_DIR:%=-I%)

include sources.mk

OBJS := $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
DEPS := $(SRCS:%.cpp=$(OBJ_DIR)/%.d)
SRCS := $(addprefix $(SRC_DIR)/,$(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

compile_commands.json: fclean
	bear -- $(MAKE) -j $(OBJS)

.PHONY: all clean fclean re compile_commands.json

-include $(DEPS)
