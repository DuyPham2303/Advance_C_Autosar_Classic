#include "search.h"

int stringCompare(const char *str1, const char *str2)
{
    // kiểm tra ký tự str1 có khác null không và ký tự hiện tại của 2 chuỗi có giống nhau khôngkhông
    while (*str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
    }
    return (*(const unsigned char *)str1 - *(const unsigned char *)str2);
}
static CenterPoint *binarySearch(CenterPoint *current, const char *search)
{
    if (current == NULL)
    {
        return NULL;
    }
    // kiểm tra và trả về thông tin name hoặc sdt dựa trên search
    char *current_info = isdigit(*search) ? current->user->info.phone : current->user->info.name;

    // if name is matched then return full info
    if (strcmp(current_info, search) == 0) // compare current info and search info
    {
        return current;
    }
    // if search < current -> find the left side of tree
    if (stringCompare(current_info, search) > 0)
        return binarySearch(current->left, search);
    // if search > current -> find the right side of tree
    else
        return binarySearch(current->right, search);
}
CenterPoint *buildTree(User_node *head, int start, int end)
{
    // kiểm tra vị trí các node trái và phải có hợp lệ hay không 
    if (head == NULL || start > end)
    {
        return NULL;
    }
    // xác định vị trí node giữa 
    int mid = (start + end) / 2;

    //con trỏ tạm để lặp qua từng node
    User_node *node = head;

    // lặp cho đến khi gặp node giữa
    for (int i = start; i < mid; i++)
    {
        if (node->next_user == NULL)
        {
            break;
        }
        node = node->next_user;
    }

    // cấp phát bộ nhớ cho từng node (mỗi lần gọi đệ quy)
    CenterPoint *root = (CenterPoint *)malloc(sizeof(CenterPoint));

    //cập nhật node trên root hiện tại của cây nhị phân 
    root->user = node;  
    // tạo ra các nhánh 2 trái phải từ nhánh gốc, bằng cách gọi đệ quy
    root->left = buildTree(head, start, mid - 1);
    root->right = buildTree(node->next_user, mid + 1, end);

    return root;
}
CenterPoint *centerPoint(User_node *head)
{
    int length = 0;
    User_node *node = head;
    while (node != NULL)
    {
        node = node->next_user;
        length++; // xác định số lượng các node trong danh sáchsách
    }
    return buildTree(head, 0, length - 1); // trả về cây nhị phân
}
CenterPoint *Search_Info(CenterPoint **root, const char *search)
{
    // kiểm tra cây nhị phân có tồn tại không
    if (root == NULL || search == NULL)
    {
        return NULL;
    }
    // xác định cây nhị phân sẽ sử dụng tìm kiếm dựa trên chuỗi search -> nếu phát hiện ký tự số thì trả về cây nhị phân tìm kiếm dựa trên sdt
    CenterPoint *search_info = isdigit(*search) ? root[1] : root[0];
    // trả về kết quả tìm kiếm
    return binarySearch(search_info, search);
}
void print_Search_byPhone(CenterPoint *root)
{
    User_node *search_user = root->user;
    printf("Tên:%s\n"
           "Tuổi:%d\n"
           "sdt:%s\n"
           "Địa chỉ:%s\n",
           search_user->info.name,
           search_user->info.age,
           search_user->info.phone,
           search_user->info.address);
}
void print_Search_byName(CenterPoint *root, User_node *head)
{
    User_node *search_user = root->user;
    User_node *current = head;
    // tìm và in ra tất cả thông tin của user có tên trùng nhau
    while (current != NULL)
    {
        // lặp cho đến khi user hiện tại và tiếp theo có name trùng nhau
        if (strcmp(current->info.name, search_user->info.name))
        {
            current = current->next_user;
        }
        else
        {
            //note 
            /* 
                - bỏ qua trường hợp tìm thấy sdt của user trong danh sách trùng với thông tin của user đang tìm kiếm theo name
                - tránh trường hợp in thông tin của user muốn tìm kiếm 2 lần
            */

            if (!strcmp(current->info.phone, search_user->info.phone))
                current = current->next_user;
            // in thông tin của tất cả user có name giống nhưng khác phone
            else
            {
                printf("Tên:%s\n"
                       "Tuổi:%d\n"
                       "sdt:%s\n"
                       "Địa chỉ:%s\n",
                       current->info.name,
                       current->info.age,
                       current->info.phone,
                       current->info.address);
                // tiếp tục lặp cho đến cuối danh sách
                current = current->next_user;
            }
        }
    }
    //note 
    /* 
        - Nếu đã kiểm tra toàn bộ node, chứng tỏ không có đối tượng nào trùng tên
        - In ra thông tin của đối tượng có tên không trùng với bất kỳ ai
    */
    printf("Tên:%s\n"
           "Tuổi:%d\n"
           "sdt:%s\n"
           "Địa chỉ:%s\n",
           search_user->info.name,
           search_user->info.age,
           search_user->info.phone,
           search_user->info.address);
}



void freeBST(CenterPoint **proot)
{
    if (proot == NULL || *proot == NULL){
        return;
    }
    CenterPoint *root = *proot;
    freeBST(&root->left);
    freeBST(&root->right);

    free(root);
    *proot = NULL; // tránh để lại con trỏ treo ở phía gọi
}
