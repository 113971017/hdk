@echo off

:: get date
set DATE_STR=%DATE:~0,4%%DATE:~5,2%%DATE:~8,2%
:: set DATE_STR=20240103

set VERSION_MAJOR=00
:: ---Customer: HENDRICKSON
:: Customer ID - Version
::       x x x - x x x x x
set VERSION_MINOR=10
::set VERSION_MINOR=0F

set VERSION=V%VERSION_MAJOR%%VERSION_MINOR%
set HEX_FILE=BSIxx-SGA_mems

echo.
@echo ------------------------------------------
@echo      gmake clean
@echo ------------------------------------------
gmake clean


echo.
@echo ------------------------------------------  
@echo      gmake
@echo ------------------------------------------  
gmake


if "%errorlevel%" == "0" (
  if exist "%HEX_FILE%.hex" (
    echo.
    echo.
    echo.
    @echo ---------------------------------------------------------  
    @echo      Convert %HEX_FILE%.hex to output\mlx_memory_image.c
    @echo ---------------------------------------------------------  
    echo.
    echo.

    1_hex_converter.py
    copy %HEX_FILE%.hex output\%HEX_FILE%_%VERSION%_%DATE_STR%.hex

  )
)

::pause