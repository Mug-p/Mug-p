#include "data.h"
#include <stdlib.h>
#include <string.h>

// 全局链表初始化
BookNode* g_book_list = NULL;

/*
 * 创建图书节点
 * 参数: book - 图书数据
 * 返回: 新分配的节点指针，失败返回NULL
 */
BookNode* data_create_node(const Book* book) {
    BookNode* node = (BookNode*)malloc(sizeof(BookNode));
    if (node) {
        node->data = *book;
        node->next = NULL;
    }
    return node;
}

/*
 * 添加节点到链表末尾
 * 遍历链表直到找到 next 为 NULL 的节点
 */
void data_append_node(BookNode** head, BookNode* new_node) {
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    BookNode* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

/*
 * 精确查找
 * 遍历链表比对 ISBN
 */
BookNode* data_find_by_isbn(BookNode* head, const char* isbn) {
    BookNode* current = head;
    while (current != NULL) {
        if (strcmp(current->data.isbn, isbn) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/*
 * 删除节点
 * 处理头节点删除和中间节点删除的情况
 */
bool data_delete_node(BookNode** head, const char* isbn) {
    BookNode *current = *head, *prev = NULL;
    
    while (current != NULL) {
        if (strcmp(current->data.isbn, isbn) == 0) {
            if (prev == NULL) {
                // 删除的是头节点
                *head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

/*
 * 释放所有内存
 * 用于程序退出前，防止内存泄漏
 */
void data_free_all(BookNode** head) {
    BookNode* current = *head;
    while (current != NULL) {
        BookNode* temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}