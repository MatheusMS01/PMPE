CXX=g++
CXXFLAGS=-g -std=c++11 -Wall 
OBJDIR=obj
CPPFILES=$(wildcard *.cpp)
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

$(OBJDIR): 
	mkdir -p $(OBJDIR)
