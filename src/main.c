// src/main.c
#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "store.h"
#include "logic.h"

void print_menu() {
    printf("\n====== 📚 图书馆管理系统 ======\n");
    printf("1. ➕ 添加图书\n");
    printf("2. 📖 借阅图书\n");
    printf("3. 🔙 归还图书\n");
    printf("4. 🔍 搜索图书 (书名/作者)\n");
    printf("5. 📊 排序统计 (热度/库存)\n");
    printf("6. 📤 导出数据 (CSV)\n");
    printf("7. 🗑️ 删除图书\n");  
    printf("0. 🚪 退出系统\n");
    printf("==============================\n");
    printf("请选择操作: ");
}

int main() {
    // 1. 程序启动：加载数据
    printf("系统启动中...正在读取 books.json ...\n");
    store_load_json("books.json", &g_book_list);
    printf("数据加载完成。\n");

    int choice;

    while (1) {
        print_menu();
        
        // 处理非数字输入的健壮性检查
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); // 清空缓冲区
            continue;
        }

        if (choice == 0) break;

        switch (choice) {
            case 1:
                logic_add_book(&g_book_list);
                break;
            case 2:
                logic_borrow_book(g_book_list);
                break;
            case 3:
                logic_return_book(g_book_list);
                break;
            case 4: 
                {
                    char buffer[100];
                    printf("请输入搜索关键词: ");
                    scanf("%s", buffer);
                    logic_search_fuzzy(g_book_list, buffer);
                }
                break;
            case 5:
                printf("1. 按借阅量(热度)  2. 按库存量: ");
                int mode;
                scanf("%d", &mode);
                logic_sort_display(g_book_list, mode);
                break;
            case 6:
                store_export_csv("books_export.csv", g_book_list);
                break;
            case 7: 
                logic_delete_book(&g_book_list);
                break;
            default:
                printf("❌ 无效选项，请输入 0-7 之间的数字。\n");
        }
    }

    // 2. 程序退出：保存并释放内存
    printf("正在保存数据...\n");
    store_save_json("books.json", g_book_list);
    
    printf("正在清理内存...\n");
    data_free_all(&g_book_list);
    
    printf("👋 再见！\n");
    return 0;
}