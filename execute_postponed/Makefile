CXX=g++
INC_DIR = ../ipc
CXXFLAGS=-g -std=c++11 -Wall -I$(INC_DIR) 
OBJDIR=obj
CPPFILES=$(wildcard *.cpp) $(wildcard $(INC_DIR)/*.cpp)
OBJS=$(addprefix $(OBJDIR)/,$(notdir $(CPPFILES:.cpp=.o)))

all:	 $(OBJDIR)/executa_postergado

clean:
	rm -f executa_postergado
	rm -f obj/*

$(OBJDIR)/executa_postergado:	$(OBJS)
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) $(OBJS) -o executa_postergado

$(OBJDIR)/%.o:	%.cpp obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o:	$(INC_DIR)/%.cpp obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR): 
	mkdir -p $(OBJDIR)
