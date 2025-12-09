#ifndef STORE_H
#define STORE_H

#include "data.h"

// 保存图书数据到 JSON 文件
void store_save_json(const char* filename, BookNode* head);

// 从 JSON 文件加载图书数据
void store_load_json(const char* filename, BookNode** head);

// 追加二进制借阅记录
void store_append_binary_log(const char* filename, const char* isbn, int action);

// 导出图书数据到 CSV
void store_export_csv(const char* filename, BookNode* head);

#endif // STORE_H