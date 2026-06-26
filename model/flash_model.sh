#!/bin/bash
# Flash ESPDet model to ESP32-P4 SPIFFS partition
#
# Usage:
#   ./flash_model.sh [path_to_espdl_file]
#
# Example:
#   ./flash_model.sh model/espdet_pico_416_416_logo.espdl

set -e

MODEL_FILE="${1:-model/espdet_pico_416_416_logo.espdl}"
SPIFFS_DIR="$(mktemp -d)"
SPIFFS_BIN="spiffs_model.bin"
PARTITION_SIZE=0x500000  # 5MB, matches partitions.csv
PARTITION_OFFSET=0xb00000
MODEL_BASENAME="$(basename "$MODEL_FILE")"

if [ ! -f "$MODEL_FILE" ]; then
    echo "Error: Model file not found: $MODEL_FILE"
    exit 1
fi

echo "=== ESPDet Model Flash Tool ==="
echo "Model: $MODEL_FILE"
echo "Size:  $(stat -c%s "$MODEL_FILE" 2>/dev/null || stat -f%z "$MODEL_FILE") bytes"

# Create SPIFFS image directory
cp "$MODEL_FILE" "$SPIFFS_DIR/$MODEL_BASENAME"

# Generate SPIFFS image
echo "Generating SPIFFS image..."
python3 "$IDF_PATH/components/spiffs/spiffsgen.py" $PARTITION_SIZE "$SPIFFS_DIR" "$SPIFFS_BIN"

# Flash to device
echo "Flashing SPIFFS image to offset $PARTITION_OFFSET..."
python3 -m esptool --chip esp32p4 write_flash $PARTITION_OFFSET "$SPIFFS_BIN"

# Cleanup
rm -rf "$SPIFFS_DIR" "$SPIFFS_BIN"

echo "=== Done! Model flashed successfully ==="
echo "Runtime path: /storage/$MODEL_BASENAME"
