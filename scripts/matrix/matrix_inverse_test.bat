@echo off
set EXE_NAME=matrix_inverse_test.exe

if exist %EXE_NAME% (
    del %EXE_NAME%
)

echo [BUILD] Compiling...
g++ -std=c++23 -Wall -Wextra -O3 ^
    -Itest -Isource -Isource/include ^
    test/matrix/matrix_inverse_test.cpp ^
    source/matrix.cpp ^
    source/complex_number.cpp ^
    -o matrix_inverse_test -lm

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