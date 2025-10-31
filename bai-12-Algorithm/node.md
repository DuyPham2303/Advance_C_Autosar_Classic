<!-- 
    SORTING ALGORITHM
        + Bubble sort
        + selection sort
        + insertion sort
        + merge sort Vv...
    SEARCH ALGORITHM
        + Linear search
        + binary search
 -->

N = 6
-> số lần duyệt qua mảng i < N - 1 
-> số lần so sánh ở mỗi lượt duyệt j < N - i - 1
+ 5 lần duyệt 
    => lần duyệt thứ i = 0  -> j = 5 lần so sánh
                         1         4
                         2         3
                         3         2
                         4         1

tìm kiếm thông tin của user

input: nhập thông tin tên/sdt
output: thông tin đầy đủ của user 

các bước 
- đọc dữ liệu từ database (file excel -> .csv)
- phân tách dữ liệu 
- lưu vào trong bộ nhớ 
- xây dựng 2 linked list sorted phone và ten 
- xây dựng 2 binary search tree theo phone va ten

1 -> 2 -> 3 -> 4 -> 5

__so sanh name__
"Pham Cao Duy"
"Pham Cao Duy"

head -> "Pham Cao Duy" -> "Pham Cao Duy" -> "Pham Cao Duy"

"Dinh Anh Tuan"

__TH 1:__
'P' - 'D' > 0 : "Dinh Anh Tuan" < "Pham Cao Duy"
"Nguyen Huu Hung"

+ head -> "Dinh Anh Tuan" -> "Pham Cao Duy"
__TH 2:__
'P' - 'D' < 0 : "Dinh Anh Tuan" > "Pham Cao Duy"
+ head -> "Pham Cao Duy" -> "Dinh Anh Tuan" -> "Nguyen Huu Hung"

__so sanh phone__
"123456"
"789012"

head -> "123456" -> "789012"

------------------------------------
CÁC BƯỚC TRONG MAIN
- ghi file thành công
    + đọc và lưu trữ vào bộ nhớ
    + tạo ra 2 linked list sorted by name/phone
    __các bước chèn node trên linked list__
        + tạo node mới -> copy info vào node value
        + chèn node đầu  
            TH 1: nếu linked list empty
            TH 2: nếu node đầu > node new
        + chèn cở các vị trí tiếp theo
            -> duyệt qua các node ở trên linked list
- ghi file thất bại
    + return 0
