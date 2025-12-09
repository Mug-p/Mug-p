#include "store.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 辅助函数：简单的行级 JSON 解析
 * 仅用于解析本项目生成的标准格式
 */
static void parse_json_line(char* line, Book* book) {
    char key[50], val_str[MAX_STR];
    char* colon = strchr(line, ':');
    if (!colon) return;

    // 解析 Key
    sscanf(line, " \"%[^\"]\"", key);

    // 解析 Value (区分字符串和数字)
    if (strchr(colon, '\"')) {
        sscanf(colon + 1, " \"%[^\"]\"", val_str); // 字符串
    } else {
        sscanf(colon + 1, " %[^,\n]", val_str);     // 数字
    }

    if (strcmp(key, "isbn") == 0) strcpy(book->isbn, val_str);
    else if (strcmp(key, "title") == 0) strcpy(book->title, val_str);
    else if (strcmp(key, "author") == 0) strcpy(book->author, val_str);
    else if (strcmp(key, "total") == 0) book->total_stock = atoi(val_str);
    else if (strcmp(key, "current") == 0) book->current_stock = atoi(val_str);
    else if (strcmp(key, "borrowed") == 0) book->borrow_count = atoi(val_str);
}

void store_save_json(const char* filename, BookNode* head) {
    FILE* fp = fopen(filename, "w");
    if (!fp) return;

    fprintf(fp, "[\n");
    BookNode* curr = head;
    while (curr) {
        Book* b = &curr->data;
        fprintf(fp, "  {\n");
        fprintf(fp, "    \"isbn\": \"%s\",\n", b->isbn);
        fprintf(fp, "    \"title\": \"%s\",\n", b->title);
        fprintf(fp, "    \"author\": \"%s\",\n", b->author);
        fprintf(fp, "    \"total\": %d,\n", b->total_stock);
        fprintf(fp, "    \"current\": %d,\n", b->current_stock);
        fprintf(fp, "    \"borrowed\": %d\n", b->borrow_count);
        fprintf(fp, "  }%s\n", curr->next ? "," : "");
        curr = curr->next;
    }
    fprintf(fp, "]\n");
    fclose(fp);
}

void store_load_json(const char* filename, BookNode** head) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return; // 文件不存在则跳过

    char line[256];
    Book temp_book;
    bool in_obj = false;

    // 加载前先清空旧数据
    data_free_all(head);

    while (fgets(line, sizeof(line), fp)) {
        if (strchr(line, '{')) {
            in_obj = true;
            memset(&temp_book, 0, sizeof(Book));
        } else if (strchr(line, '}')) {
            if (in_obj) {
                data_append_node(head, data_create_node(&temp_book));
                in_obj = false;
            }
        } else if (in_obj) {
            parse_json_line(line, &temp_book);
        }
    }
    fclose(fp);
}

/*
 * 二进制日志记录
 * 使用 "ab" 模式追加写入，非内存模拟
 */
void store_append_binary_log(const char* filename, const char* isbn, int action) {
    FILE* fp = fopen(filename, "ab");
    if (!fp) {
        perror("无法打开日志文件");
        return;
    }
    BorrowLog log;
    strcpy(log.isbn, isbn);
    log.timestamp = time(NULL);
    log.action_type = action;

    fwrite(&log, sizeof(BorrowLog), 1, fp);
    fclose(fp);
}

void store_export_csv(const char* filename, BookNode* head) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("导出 CSV 失败\n");
        return;
    }
    fprintf(fp, "ISBN,Title,Author,Total,Current,Borrows\n");
    BookNode* curr = head;
    while (curr) {
        fprintf(fp, "%s,%s,%s,%d,%d,%d\n",
            curr->data.isbn, curr->data.title, curr->data.author,
            curr->data.total_stock, curr->data.current_stock, curr->data.borrow_count);
        curr = curr->next;
    }
    fclose(fp);
    printf("CSV 已导出至 %s\n", filename);
}