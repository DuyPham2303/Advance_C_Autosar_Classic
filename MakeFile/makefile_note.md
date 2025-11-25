# Makefile Study Notes --- Full Summary

## 1. Biến tự động (Automatic Variables)

-   \*\*\$@*\*: Tên target hiện tại.
-   **\$\<**: Prerequisite đầu tiên (thường dùng cho quy tắc compile
    `.c → .o`).
-   **\$\^**: Danh sách tất cả prerequisites (loại trừ trùng lặp).
-   **\$?**: Các prerequisites mới hơn target.
-   **\$(@D)**: Thư mục chứa target.
-   **\$(@F)**: Tên file target (không kèm đường dẫn).

## 2. Ký tự `%` trong Makefile (Pattern Matching)

-   `%` đại diện cho *bất kỳ chuỗi ký tự nào*, dùng trong pattern rule.

Ví dụ:

    $(SOURCE_DIR)/%.o: $(SOURCE_DIR)/%.c

→ `.o` và `.c` phải có cùng tên gốc.

## 3. Sự khác nhau giữa `$<` và `$^`

  Biến   Ý nghĩa                                     Dùng khi
  ------ ------------------------------------------- -------------------
  `$<`   chỉ lấy *một file prerequisites đầu tiên*   compile `.c → .o`
  `$^`   lấy *toàn bộ danh sách prerequisites*       linking

Nếu dùng `$^` thay `$<` trong quy tắc compile thì sai vì compile chỉ
nhận 1 file input.

------------------------------------------------------------------------

## 4. Hàm `wildcard`

-   Lấy danh sách file thỏa mãn pattern.

Ví dụ:

    SOURCES = $(wildcard src/*.c)

------------------------------------------------------------------------

## 5. Substitution reference (`:.c=.o`)

Cách viết ngắn gọn, thay `.c` → `.o`.

    OBJECTS = $(SOURCES:.c=.o)

Tương đương:

    OBJECTS = $(patsubst %.c, %.o, $(SOURCES))

------------------------------------------------------------------------

## 6. Hàm `subst` -- thay thế chuỗi đơn giản

Cú pháp:

    $(subst FROM,TO,TEXT)

**TEXT** là phần cần thay thế nội dung.\
Make sẽ tìm chuỗi FROM và thay bằng TO.

Ví dụ:

    $(subst src,build,$(SOURCES))

------------------------------------------------------------------------

## 7. Hàm `patsubst` -- thay thế theo pattern

    $(patsubst %.c,%.o,$(SOURCES))

Cho phép dùng wildcard `%`.\
Mạnh hơn `subst`.

------------------------------------------------------------------------

## 8. Hàm `filter` và `filter-out`

### filter

Giữ lại các phần tử trùng với pattern.

    $(filter %.c, $(FILES))

### filter-out

Loại bỏ phần tử trùng với pattern.

    $(filter-out test%.c, $(FILES))

------------------------------------------------------------------------

## 9. Biến `vpath`

Cho phép Make tìm file `.c` trong nhiều thư mục.

    vpath %.c src utils drivers

------------------------------------------------------------------------

## 10. Pattern Rule

Dùng để viết quy tắc tổng quát:

    %.o: %.c
        $(CC) -c -o $@ $<

------------------------------------------------------------------------

## 11. Order-only prerequisites (`|`)

Make đảm bảo thư mục tồn tại trước khi build file:

    $(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)

------------------------------------------------------------------------

## 12. Lỗi thường gặp và cách sửa

### ❗ Wildcard sai dấu ngoặc

Sai:

    $(wildcard $(SOURCE_DIR/*.c))

Đúng:

    $(wildcard $(SOURCE_DIR)/*.c)

### ❗ Không tạo thư mục_OUTPUT

Fix:

    $(BINARY_DIR):
        mkdir -p $(BINARY_DIR)

### ❗ Circular dependency

Do sử dụng sai đường dẫn hoặc object file bị trùng tên.

------------------------------------------------------------------------

## 13. Best Practices

-   Tách rõ SOURCE_DIR, BUILD_DIR, HEADER_DIR.
-   Luôn dùng pattern rule thay vì viết từng rule thủ công.
-   Dùng `vpath` nếu có nhiều thư mục nguồn.
-   Luôn tạo thư mục output bằng order-only prerequisite.
-   Tránh object file trùng tên → dùng đầy đủ đường dẫn hoặc giữ cấu
    trúc thư mục.

------------------------------------------------------------------------

## 14. Template Makefile chuẩn

    TARGET = app
    SRC_DIR = src
    INC_DIR = include
    BUILD = build

    SOURCES = $(wildcard $(SRC_DIR)/*.c)
    OBJECTS = $(patsubst %.c,$(BUILD)/%.o,$(notdir $(SOURCES)))

    vpath %.c $(SRC_DIR)

    CC = gcc
    CFLAGS = -I$(INC_DIR) -Wall -Werror -g

    all: $(BUILD)/$(TARGET)

    $(BUILD):
        mkdir -p $(BUILD)

    $(BUILD)/$(TARGET): $(OBJECTS)
        $(CC) -o $@ $^

    $(BUILD)/%.o: %.c | $(BUILD)
        $(CC) $(CFLAGS) -c -o $@ $<

    clean:
        rm -rf $(BUILD)

    .PHONY: all clean

------------------------------------------------------------------------

Tài liệu này tổng hợp đầy đủ kiến thức đã trao đổi: automatic variables,
wildcard, subst, patsubst, filter-out, vpath, pattern rules, order-only
prerequisites, và lỗi phổ biến.
