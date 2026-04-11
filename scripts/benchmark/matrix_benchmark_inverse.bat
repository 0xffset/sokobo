@echo off
set EXE_NAME=matrix_benchmark_inverse.exe

if exist %EXE_NAME% (
    del %EXE_NAME%
)

echo [BUILD] Compiling Benchmark...
g++ -std=c++23 -Wall -Wextra -O3 ^
    -Isource -Isource/include ^
    benchmark/matrix/matrix_inverse.cpp ^
    -o %EXE_NAME% -lbenchmark -lpthread

if %errorlevel% equ 0 (
    echo [BUILD] Success! Starting %EXE_NAME%...
    echo ---------------------------------------

    %EXE_NAME%
    
    echo ---------------------------------------
    echo [RUN] Benchmark finished.
) else (
    echo.
    echo [ERROR] Compilation failed. Check the errors above.
)

pause