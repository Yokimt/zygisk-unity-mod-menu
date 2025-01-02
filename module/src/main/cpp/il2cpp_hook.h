//
// Created by Perfare on 2020/7/4.
//

#ifndef ZYGISK_IL2CPPDUMPER_IL2CPP_HOOK_H
#define ZYGISK_IL2CPPDUMPER_IL2CPP_HOOK_H
#include "game.h"
#include "string"
#include "log.h"


typedef struct {
    bool isNP= false;
    bool isCD= false;
    bool isHp = false;
    int Hp = 1;
    bool kill = false;

} SharedData;

inline SharedData readdata;
inline SharedData writedata;
inline bool attack = false;
static std::string path = "/data/data/" + std::string(GamePackageName) + "/files/sharedata.bin";
static void readSharedDataFromFile(const char* filename, SharedData* data) {
    FILE* file = fopen(filename, "rb"); // 打开文件用于二进制读取
    if (file == NULL) {
        LOGI("Error opening file");
        return;
    }
    // 从文件读取数据
    fread(data, sizeof(SharedData), 1, file);
    // 关闭文件
    fclose(file);
}
static void writeSharedDataToFile(const char* filename, const SharedData* data) {
    FILE* file = fopen(filename, "wb"); // 打开文件用于二进制写入
    if (file == NULL) {
        LOGI("Error opening file");
        return;
    }

    // 写入数据到文件
    fwrite(data, sizeof(SharedData), 1, file);

    // 关闭文件
    fclose(file);
}

void il2cpp_api_init(void *handle);
void hook_function();

#endif //ZYGISK_IL2CPPDUMPER_IL2CPP_HOOK_H
