#!/bin/bash
export PATH="/c/Users/16604/AppData/Local/anyui/environment/mingw/bin:/c/Users/16604/AppData/Local/anyui/environment/mingw/libexec/gcc/mingw32/9.2.0:/c/Program Files/Git/usr/bin:$PATH"

ROOT="/c/Users/16604/Desktop/anyui_test/test12"
SIM="$ROOT/simulator"
LVGL="$ROOT/lvgl"
GEN="$ROOT/generated"
SDK="$ROOT/sdk"
SDL2_INC="C:/Users/16604/AppData/Local/anyui/environment/template/SDL2/i686-w64-mingw32/include"
SDL2_LIB="C:/Users/16604/AppData/Local/anyui/environment/template/SDL2/i686-w64-mingw32/lib"
CFLAGS="-O0 -g0 -Wno-implicit-function-declaration -Wno-unused-function -Wno-unused-variable -I$ROOT -I$LVGL -I$SIM -I$GEN -I$SDK -I$SDL2_INC -DSIMULATOR=1 -DLV_BUILD_TEST=0 -MMD -MP"
LDFLAGS="-mconsole -L$ROOT/lib/x86 -L$SDL2_LIB"
LDLIBS="-lSDL2 -lm -llvgl_thorvg -lstdc++"
GCC="/c/Users/16604/AppData/Local/anyui/environment/mingw/bin/gcc.exe"

echo "=== Compiling simulator files ==="
for f in "$SIM"/*.c; do
  name=$(basename "$f" .c)
  echo "  $name"
  $GCC -c $CFLAGS "$f" -o "$SIM/build/obj/simulator/$name.o" 2>&1
done

echo "=== Compiling generated files ==="
find "$GEN" -name '*.c' | {
  while read f; do
    rel=${f#$ROOT/}
    dir=$(dirname "$rel")
    mkdir -p "$SIM/build/obj/$dir"
    echo "  $rel"
    $GCC -c $CFLAGS "$f" -o "$SIM/build/obj/$rel.o" 2>&1
  done
}

echo "=== Compiling sdk files ==="
for f in "$SDK"/*.c; do
  name=$(basename "$f" .c)
  echo "  $name"
  $GCC -c $CFLAGS "$f" -o "$SIM/build/obj/sdk/$name.o" 2>&1
done

echo "=== Compiling lvgl files ==="
LVGL_COUNT=0
find "$LVGL/src" -name '*.c' | {
  while read f; do
    rel=${f#$ROOT/}
    dir=$(dirname "$rel")
    mkdir -p "$SIM/build/obj/$dir"
    $GCC -c $CFLAGS "$f" -o "$SIM/build/obj/$rel.o" 2>&1
    LVGL_COUNT=$((LVGL_COUNT + 1))
    if [ $((LVGL_COUNT % 50)) -eq 0 ]; then
      echo "  compiled $LVGL_COUNT lvgl files..."
    fi
  done
  echo "=== LVGL compile done ($LVGL_COUNT files) ==="
}

echo "=== Collecting simulator objects ==="
SIM_OBJS=$(find "$SIM/build/obj/simulator" -name '*.o')
LVGL_OBJS=$(find "$SIM/build/obj/lvgl" -name '*.o')
echo "  found $(echo "$LVGL_OBJS" | wc -l) lvgl objects"

echo "=== Collecting generated objects ==="
GEN_OBJS=$(find "$SIM/build/obj/generated" -name '*.o')

echo "=== Collecting sdk objects ==="
SDK_OBJS=$(find "$SIM/build/obj/sdk" -name '*.o')

echo "=== Linking ==="
echo "  Creating lvgl archive..."
find "$SIM/build/obj/lvgl" -name '*.o' | xargs ar rcs "$SIM/build/bin/liblvgl.a" 2>&1
echo "  Creating generated archive..."
find "$SIM/build/obj/generated" -name '*.o' | xargs ar rcs "$SIM/build/bin/libgen.a" 2>&1
echo "  Creating sdk archive..."
find "$SIM/build/obj/sdk" -name '*.o' | xargs ar rcs "$SIM/build/bin/libsdk.a" 2>&1

echo "  Linking..."
$GCC -v -o "$SIM/build/bin/simulator" \
  $SIM_OBJS \
  -L"$SIM/build/bin" \
  -Wl,--start-group \
  -llvgl -lgen -lsdk \
  $LDLIBS \
  -Wl,--end-group \
  $LDFLAGS 2>&1

echo "=== Done ==="