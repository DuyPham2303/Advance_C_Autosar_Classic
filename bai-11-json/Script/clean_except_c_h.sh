#!/bin/bash
# Convert Windows path to Unix path (sử dụng cygpath nếu có)
if command -v cygpath &> /dev/null; then
    FILE_PATH=$(cygpath "$1")
else
    # Fallback: tự chuyển C:\ → /c/ (thô sơ, không chính xác 100%)
    FILE_PATH="/$(echo "$1" | sed 's|\\|/|g' | sed 's|:||')"
fi

# Lấy thư mục chứa file
TARGET_DIR=$(dirname "$FILE_PATH")

# In debug ra cho chắc
echo "→ Jumping to: $TARGET_DIR"

# Nhảy vào thư mục đó
cd "$TARGET_DIR" || exit
echo "Cleaning files in: $(TARGET_DIR)"

# Xoá tất cả file (không phải folder), trừ .c và .h
find . -maxdepth 1 -type f ! -name "*.c" ! -name "*.h" -exec rm -f {} \;

