# Compiler
CXX = g++

# Compiler flags
# -Wall -Wextra: Display warnings for safer code
# -std=c++17: Use C++17 standard
# -O2: Performance optimization
CXXFLAGS = -Wall -Wextra -std=c++17 -O2

# Include paths
# -I.: allows #include "core/...", #include "utils/...", #include "tools/..." from project root
INC = -I .
# Libraries to link
# -lpcap: Required for pcap library
LIBS = -lpcap

# ─── Shared core object files (reused by every tool) ──────────────────────────
CORE_SRCS = core/logger/logger.cpp \
            core/network/networkUtils.cpp \
            core/utils/utils.cpp
CORE_OBJS = $(CORE_SRCS:.cpp=.o)

# ─── anti_DOS tool ─────────────────────────────────────────────────────────────
PACKETFLOWSTATE_TARGET = packets_flow_rate_tracker
PACKETFLOWSTATE_SRCS   = capture/main.cpp \
                		 capture/capture.cpp
PACKETFLOWSTATE_OBJS   = $(PACKETFLOWSTATE_SRCS:.cpp=.o)

# ─── Default target: build all tools ──────────────────────────────────────────
# Add new tool targets here as the project grows
all: $(PACKETFLOWSTATE_TARGET)

# Link anti_DOS: its own objects + shared core objects
$(PACKETFLOWSTATE_TARGET): $(PACKETFLOWSTATE_OBJS) $(CORE_OBJS)
	$(CXX) $(CXXFLAGS) $(INC) -o $(PACKETFLOWSTATE_TARGET) $(PACKETFLOWSTATE_OBJS) $(CORE_OBJS) $(LIBS)

# Rule to compile any .cpp file to a .o file in the same directory
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

# Clean command to remove all objects and executables
clean:
	find . -name "*.o" -delete
	rm -f $(PACKETFLOWSTATE_TARGET) 

.PHONY: all clean
