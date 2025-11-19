# so sánh struct vs union 
                    **struct**                              **union**
`Bộ nhớ cấp phát`   từng member có vị trí bộ nhớ riêng      tất cả member đều share a common memory
`kích thước`        total size of members + padding         largest member + padding byte
`Truy cập`          có thể truy cập độc lập mỗi member      chỉ access dc 1 member tại 1 thời điểm
`Mục đích`          gộp nhiều datatype để access đồng thời  - optimize memory
                                                            - access từng member tại mỗi thời điểm
                                                            - access dữ liệu của member ở nhiều định dạng khác nhau
# trường hợp cụ thể sử dụng struct 
    - sinh vien (mssv,name,age,major,gpa) 
    - cảm biến (temp,humi)