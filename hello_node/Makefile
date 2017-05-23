CXX=g++
BINDIR=../bin
CXXFLAGS=-g -std=c++11 -Wall
OBJDIR=obj
CPPFILES=$(wildcard *.cpp)
OBJS=$(addprefix $(OBJDIR)/,$(notdir $(CPPFILES:.cpp=.o)))

all:	 $(OBJDIR)/hello_node

clean:
	rm -f $(BINDIR)/hello_node
	rm -f obj/*

$(OBJDIR)/hello_node:	$(OBJS)
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(BINDIR)/hello_node

$(OBJDIR)/%.o:	%.cpp obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR): 
	mkdir -p $(OBJDIR)
