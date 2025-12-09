#ifndef LOGIC_H
#define LOGIC_H

#include "data.h"

// 添加图书（含冲突检测）
void logic_add_book(BookNode** head);

// 借书操作
void logic_borrow_book(BookNode* head);

// 还书操作
void logic_return_book(BookNode* head);

// 模糊搜索
void logic_search_fuzzy(BookNode* head, const char* keyword);

// 排序显示 (1:借阅量降序, 2:库存量升序)
void logic_sort_display(BookNode* head, int mode);

// [新增] 删除图书逻辑封装
void logic_delete_book(BookNode** head);

#endif // LOGIC_H