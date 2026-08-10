@echo off
REM Flash ESPDet model to ESP32-P4 SPIFFS partition
REM Usage: flash_model.bat [path_to_espdl_file]
REM Example: flash_model.bat model\espdet_pico_416_416_logo.espdl

setlocal enabledelayedexpansion

if "%~1"=="" (
    set MODEL_FILE=model\espdet_pico_416_416_logo.espdl
) else (
    set MODEL_FILE=%~1
)
set SPIFFS_DIR=spiffs_image
set SPIFFS_BIN=spiffs_model.bin
set PARTITION_SIZE=5242880
set PARTITION_OFFSET=0xb00000
for %%F in ("%MODEL_FILE%") do set MODEL_BASENAME=%%~nxF

if not exist "%MODEL_FILE%" (
    echo Error: Model file not found: %MODEL_FILE%
    exit /b 1
)

echo === ESPDet Model Flash Tool ===
echo Model: %MODEL_FILE%

REM Create SPIFFS image directory
if exist "%SPIFFS_DIR%" rmdir /s /q "%SPIFFS_DIR%"
mkdir "%SPIFFS_DIR%"
copy "%MODEL_FILE%" "%SPIFFS_DIR%\%MODEL_BASENAME%" >nul

REM Generate SPIFFS image
echo Generating SPIFFS image...
python "%IDF_PATH%\components\spiffs\spiffsgen.py" %PARTITION_SIZE% "%SPIFFS_DIR%" "%SPIFFS_BIN%"
if errorlevel 1 (
    echo Error: spiffsgen failed
    exit /b 1
)

REM Flash to device
echo Flashing SPIFFS image to offset %PARTITION_OFFSET%...
python -m esptool --chip esp32p4 write_flash %PARTITION_OFFSET% "%SPIFFS_BIN%"
if errorlevel 1 (
    echo Error: Flash failed
    exit /b 1
)

REM Cleanup
rmdir /s /q "%SPIFFS_DIR%"
del "%SPIFFS_BIN%"

echo === Done! Model flashed successfully ===
echo Runtime path: /storage/%MODEL_BASENAME%
