#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define MAX_STR 100

// 图书实体结构
typedef struct {
    char isbn[20];          // 唯一标识
    char title[MAX_STR];    // 书名
    char author[MAX_STR];   // 作者
    int total_stock;        // 总库存
    int current_stock;      // 当前库存
    int borrow_count;       // 借阅次数(用于热度排序)
} Book;

// 链表节点结构
typedef struct BookNode {
    Book data;
    struct BookNode* next;
} BookNode;

// 借阅日志结构 (用于二进制存储)
typedef struct {
    char isbn[20];
    time_t timestamp;
    int action_type; // 1: 借出, 2: 归还
} BorrowLog;

// 全局链表头指针
extern BookNode* g_book_list;

// === 函数声明 ===

// 创建新节点
BookNode* data_create_node(const Book* book);

// 添加节点到链表尾部
void data_append_node(BookNode** head, BookNode* new_node);

// 根据ISBN查找节点
BookNode* data_find_by_isbn(BookNode* head, const char* isbn);

// 删除指定ISBN的节点
bool data_delete_node(BookNode** head, const char* isbn);

// 释放所有内存
void data_free_all(BookNode** head);

#endif // DATA_H