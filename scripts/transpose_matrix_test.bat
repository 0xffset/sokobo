@echo off
set EXE_NAME=transpose_matrix_test.exe

if exist %EXE_NAME% (
    del %EXE_NAME%
)

echo [BUILD] Compiling...
g++ -std=c++23 -Wall -Wextra -O3 ^
    -Itest -Isource -Isource/include ^
    test/matrix/transpose_matrix_test.cpp ^
    source/matrix.cpp ^
    source/complex_number.cpp ^
    -o transpose_matrix_test -lm

if %errorlevel% equ 0 (
    echo [BUILD] Success! Starting %EXE_NAME%...
    echo ---------------------------------------
    
    %EXE_NAME%
    
    echo ---------------------------------------
    echo [RUN] Program finished.
) else (
    echo.
    echo [ERROR] Compilation failed. Check the errors above.
)

pause