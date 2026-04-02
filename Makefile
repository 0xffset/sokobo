# Detect OS
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
else
    DETECTED_OS := $(shell uname -s)
endif

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -O3 -Isource

# OS-specific settings
ifeq ($(DETECTED_OS),Windows)
    # Windows specific settings
    LDFLAGS = -lm
    TARGET = sokobo.exe
    MKDIR = if not exist "$(1)" mkdir "$(1)"
    RM = rmdir /s /q
    CP = copy
    INSTALL_DIR = C:\Program Files\sokobo
    PATH_SEP = \\
    TEST_RUN = @for %%t in ($^) do (echo Running %%t... && %%t || exit /b 1)
else
    # Unix-like (Linux/macOS) specific settings
    LDFLAGS = -lm
    TARGET = sokobo
    MKDIR = mkdir -p $(1)
    RM = rm -rf
    CP = cp
    INSTALL_DIR = /usr/local/bin
    PATH_SEP = /
    TEST_RUN = @for test in $^; do echo "Running $$test..."; ./$$test || exit 1; done
endif

SRCDIR = source
INCDIR = source$(PATH_SEP)include
OBJDIR = obj
TESTDIR = test
TESTOBJDIR = obj$(PATH_SEP)test

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

OBJECTS_NO_MAIN = $(filter-out $(OBJDIR)/main.o, $(OBJECTS))

TEST_SOURCES = $(wildcard $(TESTDIR)/*_test.cpp)
TEST_OBJECTS = $(TEST_SOURCES:$(TESTDIR)/%.cpp=$(TESTOBJDIR)/%.o)
TEST_BINARIES = $(TEST_SOURCES:$(TESTDIR)/%.cpp=$(TESTOBJDIR)/%)

$(TEST_BINARIES): $(TESTOBJDIR)/%: $(TESTOBJDIR)/%.o $(OBJECTS_NO_MAIN)
	$(call MKDIR,$(TESTOBJDIR))
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)



.PHONY: all clean install test

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(call MKDIR,$(OBJDIR))
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TEST_BINARIES)
	$(TEST_RUN)

$(TESTOBJDIR)/%: $(TESTOBJDIR)/%.o $(OBJECTS)
	$(call MKDIR,$(TESTOBJDIR))
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(TESTOBJDIR)/%.o: $(TESTDIR)/%.cpp
	$(call MKDIR,$(TESTOBJDIR))
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
ifeq ($(DETECTED_OS),Windows)
	$(RM) $(OBJDIR) 2>nul || echo.
	del /q $(TARGET) 2>nul || echo.
else
	$(RM) $(OBJDIR) $(TARGET)
endif

install: $(TARGET)
ifeq ($(DETECTED_OS),Windows)
	$(call MKDIR,$(INSTALL_DIR))
	$(CP) $(TARGET) "$(INSTALL_DIR)$(PATH_SEP)"
else
	$(CP) $(TARGET) $(INSTALL_DIR)/
endif

# Include automatically generated dependencies
-include $(OBJECTS:.o=.d)

# Dependencies
$(OBJDIR)/main.o: $(INCDIR)$(PATH_SEP)cli.h
$(OBJDIR)/cli.o: $(INCDIR)$(PATH_SEP)cli.h $(INCDIR)$(PATH_SEP)expression.h $(INCDIR)$(PATH_SEP)complex_number.h $(INCDIR)$(PATH_SEP)polynomial.h $(INCDIR)$(PATH_SEP)complex_number.h $(INCDIR)$(PATH_SEP)matrix.h $(INCDIR)$(PATH_SEP)calculus.h $(INCDIR)$(PATH_SEP)laplace.h $(INCDIR)$(PATH_SEP)fourier.h $(INCDIR)$(PATH_SEP)differential_equations.h $(INCDIR)$(PATH_SEP)numerical_methods.h
$(OBJDIR)/expression.o: $(INCDIR)$(PATH_SEP)expression.h
$(OBJDIR)/polynomial.o: $(INCDIR)$(PATH_SEP)polynomial.h
$(OBJDIR)/complex_number.o: $(INCDIR)$(PATH_SEP)complex_number.h
$(OBJDIR)/matrix.o: $(INCDIR)$(PATH_SEP)matrix.h
$(OBJDIR)/calculus.o: $(INCDIR)$(PATH_SEP)calculus.h $(INCDIR)$(PATH_SEP)expression.h $(INCDIR)$(PATH_SEP)polynomial.h
$(OBJDIR)/laplace.o: $(INCDIR)$(PATH_SEP)laplace.h $(INCDIR)$(PATH_SEP)expression.h $(INCDIR)$(PATH_SEP)complex_number.h
$(OBJDIR)/fourier.o: $(INCDIR)$(PATH_SEP)complex_number.h
$(OBJDIR)/differential_equations.o: $(INCDIR)$(PATH_SEP)differential_equations.h $(INCDIR)$(PATH_SEP)expression.h
$(OBJDIR)/numerical_methods.o: $(INCDIR)$(PATH_SEP)numerical_methods.h $(INCDIR)$(PATH_SEP)matrix.h $(INCDIR)$(PATH_SEP)polynomial.h
