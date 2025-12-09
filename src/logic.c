#include "logic.h"
#include "store.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 辅助：清除输入缓冲区
static void clear_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void logic_add_book(BookNode** head) {
    Book b;
    printf("请输入 ISBN: ");
    scanf("%19s", b.isbn);
    clear_input(); // 清除换行符

    // 冲突检测
    if (data_find_by_isbn(*head, b.isbn)) {
        printf("错误: ISBN %s 已存在！\n", b.isbn);
        return;
    }

    printf("书名: ");
    scanf("%[^\n]", b.title); clear_input();
    printf("作者: ");
    scanf("%[^\n]", b.author); clear_input();
    printf("库存数量: ");
    scanf("%d", &b.total_stock); clear_input();

    b.current_stock = b.total_stock;
    b.borrow_count = 0;

    data_append_node(head, data_create_node(&b));
    store_save_json("books.json", *head); // 自动保存
    printf("图书添加成功并保存。\n");
}

void logic_borrow_book(BookNode* head) {
    char isbn[20];
    printf("输入借阅 ISBN: ");
    scanf("%19s", isbn);

    BookNode* node = data_find_by_isbn(head, isbn);
    if (!node) {
        printf("未找到该图书。\n");
        return;
    }

    if (node->data.current_stock > 0) {
        node->data.current_stock--;
        node->data.borrow_count++;
        // 1. 更新 JSON
        store_save_json("books.json", head);
        // 2. 写入二进制日志
        store_append_binary_log("history.dat", isbn, 1); // 1 = Borrow
        printf("借阅成功！\n");
    } else {
        printf("库存不足，无法借阅。\n");
    }
}

void logic_return_book(BookNode* head) {
    char isbn[20];
    printf("输入归还 ISBN: ");
    scanf("%19s", isbn);

    BookNode* node = data_find_by_isbn(head, isbn);
    if (!node) {
        printf("未找到该图书。\n");
        return;
    }

    if (node->data.current_stock < node->data.total_stock) {
        node->data.current_stock++;
        store_save_json("books.json", head);
        store_append_binary_log("history.dat", isbn, 2); // 2 = Return
        printf("归还成功！\n");
    } else {
        printf("库存已满，无法归还（检查是否输入错误）。\n");
    }
}

void logic_search_fuzzy(BookNode* head, const char* keyword) {
    BookNode* curr = head;
    int count = 0;
    printf("\n%-15s %-30s %-20s %s\n", "ISBN", "Title", "Author", "Stock");
    printf("--------------------------------------------------------------------\n");
    
    while (curr) {
        // strstr 实现模糊匹配
        if (strstr(curr->data.title, keyword) || strstr(curr->data.author, keyword)) {
            printf("%-15s %-30s %-20s %d/%d\n", 
                curr->data.isbn, curr->data.title, curr->data.author, 
                curr->data.current_stock, curr->data.total_stock);
            count++;
        }
        curr = curr->next;
    }
    printf("共找到 %d 本相关图书。\n", count);
}

// qsort 比较函数
static int cmp_borrow_desc(const void* a, const void* b) {
    const Book* b1 = *(const Book**)a;
    const Book* b2 = *(const Book**)b;
    return b2->borrow_count - b1->borrow_count; // 降序
}

static int cmp_stock_asc(const void* a, const void* b) {
    const Book* b1 = *(const Book**)a;
    const Book* b2 = *(const Book**)b;
    return b1->current_stock - b2->current_stock; // 升序
}

void logic_sort_display(BookNode* head, int mode) {
    // 1. 转为数组
    int count = 0;
    BookNode* curr = head;
    while(curr) { count++; curr = curr->next; }
    if(count == 0) return;

    Book** arr = malloc(sizeof(Book*) * count);
    curr = head;
    for(int i=0; i<count; i++) {
        arr[i] = &curr->data;
        curr = curr->next;
    }

    // 2. 排序
    if (mode == 1) qsort(arr, count, sizeof(Book*), cmp_borrow_desc);
    else qsort(arr, count, sizeof(Book*), cmp_stock_asc);

    // 3. 显示
    printf("\n%-15s %-30s %-10s %s\n", "ISBN", "Title", "Borrows", "Stock");
    for(int i=0; i<count; i++) {
        printf("%-15s %-30s %-10d %d\n", 
            arr[i]->isbn, arr[i]->title, arr[i]->borrow_count, arr[i]->current_stock);
    }

    free(arr); // 仅释放指针数组，不释放节点
}
/* * [新增] 删除图书的完整业务逻辑 
 * 包含：用户输入 -> 删除节点 -> 自动保存 -> 结果反馈
 */
void logic_delete_book(BookNode** head) {
    char isbn[20];
    printf("请输入要删除的图书 ISBN: ");
    scanf("%19s", isbn);
    
    // 调用 data.c 中的删除功能
    if (data_delete_node(head, isbn)) {
        printf("✅ 成功: 图书 (ISBN: %s) 已删除。\n", isbn);
        
        // 关键：删除后立即更新 JSON 文件，实现持久化
        store_save_json("books.json", *head); 
        printf("💾 数据文件已自动更新。\n");
    } else {
        printf("❌ 失败: 未找到 ISBN 为 %s 的图书。\n", isbn);
    }
}