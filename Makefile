CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -Iinclude
LDFLAGS = -lm

SRCDIR = src
INCDIR = include
OBJDIR = obj

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
TARGET = cas

.PHONY: all clean install

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Dependencies
$(OBJDIR)/main.o: $(INCDIR)/cli.h
$(OBJDIR)/cli.o: $(INCDIR)/cli.h $(INCDIR)/expression.h $(INCDIR)/polynomial.h $(INCDIR)/complex_number.h $(INCDIR)/matrix.h $(INCDIR)/calculus.h $(INCDIR)/laplace.h $(INCDIR)/fourier.h $(INCDIR)/differential_equations.h $(INCDIR)/numerical_methods.h
$(OBJDIR)/expression.o: $(INCDIR)/expression.h
$(OBJDIR)/polynomial.o: $(INCDIR)/polynomial.h
$(OBJDIR)/complex_number.o: $(INCDIR)/complex_number.h
$(OBJDIR)/matrix.o: $(INCDIR)/matrix.h
$(OBJDIR)/calculus.o: $(INCDIR)/calculus.h $(INCDIR)/expression.h $(INCDIR)/polynomial.h
$(OBJDIR)/laplace.o: $(INCDIR)/laplace.h $(INCDIR)/expression.h $(INCDIR)/complex_number.h
$(OBJDIR)/fourier.o: $(INCDIR)/fourier.h $(INCDIR)/complex_number.h
$(OBJDIR)/differential_equations.o: $(INCDIR)/differential_equations.h $(INCDIR)/expression.h
$(OBJDIR)/numerical_methods.o: $(INCDIR)/numerical_methods.h $(INCDIR)/matrix.h $(INCDIR)/polynomial.h