CXX=g++
CXXFLAGS=-g -std=c++11 -Wall 
OBJDIR=obj
CPPFILES=$(wildcard *.cpp)
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

$(OBJDIR): 
	mkdir -p $(OBJDIR)
