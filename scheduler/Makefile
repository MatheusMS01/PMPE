CXX=g++
INC_DIR = ../ipc
CXXFLAGS=-g -std=c++11 -Wall -I$(INC_DIR) 
OBJDIR=obj
CPPFILES=$(wildcard *.cpp) $(wildcard $(INC_DIR)/*.cpp)
OBJS=$(addprefix $(OBJDIR)/,$(notdir $(CPPFILES:.cpp=.o)))

all:	 $(OBJDIR)/escalonador

clean:
	rm -f escalonador
	rm -f obj/*

$(OBJDIR)/escalonador:	$(OBJS)
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) $(OBJS) -o escalonador

$(OBJDIR)/%.o:	%.cpp obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o:	$(INC_DIR)/%.cpp obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR): 
	mkdir -p $(OBJDIR)
