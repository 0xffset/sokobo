# ==========================================
# 1. OS Detection
# ==========================================
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
else
    DETECTED_OS := $(shell uname -s)
endif

# ==========================================
# 2. Compiler and Flags
# ==========================================
CXX = g++
# -MMD -MP: Generates dependency files (.d) to track header changes
CXXFLAGS = -std=c++20 -MMD -MP -Wall -Wextra -O3  -I$(SRCDIR) -I$(INCDIR)
LDFLAGS = -lm

# ==========================================
# 3. Directories
# ==========================================
SRCDIR = source
INCDIR = source/include/
OBJDIR = obj
TESTDIR = test
TESTOBJDIR = obj/test
PATH_SEP = /

# ==========================================
# 4. File Discovery and Exclusions
# ==========================================

# Main application sources and objects
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

# $(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/$(INCDIR)%.h

OBJECTS_NO_MAIN = $(filter-out $(OBJDIR)/main.o, $(OBJECTS))

# --- TEST EXCLUSION SECTION ---
# Add any test files you want to skip here (relative path)
EXCLUDE_TESTS := test/matrix/dot_product_test.cpp

# Find all .cpp files in /test and subdirectories
ifeq ($(DETECTED_OS),Windows)
    TEST_SOURCES_ALL := $(wildcard $(TESTDIR)/*.cpp) $(wildcard $(TESTDIR)/**/*.cpp)
    LDFLAGS += -mconsole
else
    TEST_SOURCES_ALL := $(shell find $(TESTDIR) -name "*.cpp")
endif

# Apply the exclusion filter
TEST_SOURCES := $(filter-out $(EXCLUDE_TESTS), $(TEST_SOURCES_ALL))

# Map test sources to objects and binaries
TEST_OBJECTS = $(patsubst $(TESTDIR)/%.cpp, $(TESTOBJDIR)/%.o, $(TEST_SOURCES))

ifeq ($(DETECTED_OS),Windows)
    TEST_BINARIES = $(patsubst $(TESTDIR)/%.cpp, $(TESTOBJDIR)/%.exe, $(TEST_SOURCES))
else
    TEST_BINARIES = $(patsubst $(TESTDIR)/%.cpp, $(TESTOBJDIR)/%, $(TEST_SOURCES))
endif

# ==========================================
# 5. OS-Specific Commands
# ==========================================
ifeq ($(DETECTED_OS),Windows)
    TARGET = sokobo.exe
    RM = rmdir /s /q
    # Windows mkdir requires backslashes and a check if exists
    MKDIR = if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
    # CMD loop: converts paths to backslashes for execution
    TEST_RUN = @for %%t in ($(subst /,\,$(TEST_BINARIES))) do (echo Running %%t... && %%t || exit /b 1)
else
    TARGET = sokobo
    RM = rm -rf
    MKDIR = mkdir -p $(1)
    # Bash loop
    TEST_RUN = @for test_bin in $(TEST_BINARIES); do echo "Running $$test_bin..."; ./$$test_bin || exit 1; done
endif

# ==========================================
# 6. Build Rules
# ==========================================
.PHONY: all clean test check-paths

$(OBJDIR):
	@$(call MKDIR,$@)

all:$(OBJDIR) $(TARGET)

# Link main application
$(TARGET): $(OBJECTS)
	@$(call MKDIR,$(dir $@))
	$(CXX) $^ -o $@ $(LDFLAGS) -mconsole

# Compile application objects
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@$(call MKDIR,$(dir $@))
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --- Test Rules ---

# Build and run all tests
test: $(TEST_BINARIES)
	@echo ------------------------------------------
	@echo Starting Test Execution
	@echo ------------------------------------------
	$(TEST_RUN)

# Compile test objects
$(TESTOBJDIR)/%.o: $(TESTDIR)/%.cpp | $(OBJDIR)
	@$(call MKDIR,$(dir $@))
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link each test (test file + all app code except main.o)
$(TEST_BINARIES): $(TESTOBJDIR)/%$(if $(findstring Windows,$(DETECTED_OS)),.exe): $(TESTOBJDIR)/%.o $(OBJECTS_NO_MAIN) | $(TESTOBJDIR)
	@$(call MKDIR,$(dir $@))
	$(CXX) $^ -o $@ $(LDFLAGS)

# ==========================================
# 7. Utilities
# ==========================================

clean:
	$(RM) $(OBJDIR) $(TARGET) 2>nul || echo Clean.
    # Include auto-generated dependencies (.d files)
    -include $(OBJECTS:.o=.d)
    -include $(TEST_OBJECTS:.o=.d)

# Use this to verify which files are being picked up
check-paths:
	@echo "OS: $(DETECTED_OS)"
	@echo "Included Tests: $(TEST_SOURCES)"
	@echo "Excluded: $(EXCLUDE_TESTS)"\
    @echo "Source Dir: $(SRCDIR)"
	@echo "Include Dir: $(INCDIR)"
	@echo "Combined path: $(SRCDIR)/$(INCDIR)calculus.h"




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