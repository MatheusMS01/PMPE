CXX=g++
BINDIR = ../bin
INC_DIR = ../ipc
CXXFLAGS=-g -std=c++11 -Wall -I$(INC_DIR) 
OBJDIR=obj
CPPFILES=$(wildcard *.cpp) $(wildcard $(INC_DIR)/*.cpp)
OBJS=$(addprefix $(OBJDIR)/,$(notdir $(CPPFILES:.cpp=.o)))

all:	 $(OBJDIR)/shutdown

clean:
	rm -f shutdown
	rm -f obj/*

$(OBJDIR)/shutdown:	$(OBJS)
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(BINDIR)/shutdown

$(OBJDIR)/%.o:	%.cpp obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o:	$(INC_DIR)/%.cpp obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR): 
	mkdir -p $(OBJDIR)
