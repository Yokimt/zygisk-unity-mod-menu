//
// Created by Perfare on 2020/7/4.
//

#include "il2cpp_hook.h"
#include <dlfcn.h>
#include <cstdlib>
#include <cstring>
#include <cinttypes>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include "xdl.h"
#include "il2cpp-tabledefs.h"
#include "il2cpp-class.h"

#define DO_API(r, n, p) r (*n) p

#include "il2cpp-api-functions.h"
#include "dobby.h"

#undef DO_API

static uint64_t il2cpp_base = 0;

void init_il2cpp_api(void *handle) {
#define DO_API(r, n, p) {                      \
    n = (r (*) p)xdl_sym(handle, #n, nullptr); \
    if(!n) {                                   \
        LOGW("api not found %s", #n);          \
    }                                          \
}

#include "il2cpp-api-functions.h"
#undef DO_API
}
void il2cpp_api_init(void *handle) {
    LOGI("il2cpp_handle: %p", handle);
    init_il2cpp_api(handle);
    if (il2cpp_domain_get_assemblies) {
        Dl_info dlInfo;
        if (dladdr((void *) il2cpp_domain_get_assemblies, &dlInfo)) {
            il2cpp_base = reinterpret_cast<uint64_t>(dlInfo.dli_fbase);
        }
        LOGI("il2cpp_base: %" PRIx64"", il2cpp_base);
    } else {
        LOGE("Failed to initialize il2cpp api.");
        return;
    }
    while (!il2cpp_is_vm_thread(nullptr)) {
        LOGI("Waiting for il2cpp_init...");
        sleep(1);
    }
    auto domain = il2cpp_domain_get();
    il2cpp_thread_attach(domain);
}



void *(*org_get_BattleSvtData)(void * _this);
void *replace_get_BattleSvtData(void * _this)
{
    return org_get_BattleSvtData(_this);
}


void* (*org_getSelfSkillInfo)(void * _this,int index);
void* replace_getSelfSkillInfo(void * _this,int index)
{
    return  org_getSelfSkillInfo(_this,index);
}




int (*org_getNowHp)(void * _this);
int replace_getNowHp(void * _this)
{
    if(!*(bool *)((uint64_t)_this+0x1c7)&&readdata.isHp)
    {
        return org_getNowHp(_this)*readdata.Hp;
    }

    return org_getNowHp(_this);
}


int (*org_getMaxHp)(void * _this);
int replace_getMaxHp(void * _this)
{
    if(!*(bool *)((uint64_t)_this+0x1c7)&&readdata.isHp)
    {
        return org_getMaxHp(_this)*readdata.Hp;
    }
    return org_getMaxHp(_this);
}

int (*org_getAttackType)(void* _this,void *command);
int replace_getAttackType(void* _this,void *command)
{
    if(_this!= nullptr&&readdata.kill)
    {
        attack= true;
    }
    return org_getAttackType(_this,command);
}


int (*org_turnProgressing)(void* _this,void *fieldIndiv,void *logic,bool isAlive,void *actiondata);
int replace_turnProgressing(void* _this,void *fieldIndiv,void *logic,bool isAlive,void *actiondata)
{
    if(_this!= nullptr)
    {
        attack= false;
    }
    return org_turnProgressing(_this,fieldIndiv,logic,isAlive,actiondata);
}



void (*org_Update)(void * _this);
void replace_Update(void * _this)
{
    readSharedDataFromFile(path.c_str(),&readdata);
    if(_this!=NULL)
    {
        if(!*(bool *)((uint64_t)org_get_BattleSvtData(_this)+0x1c7))
        {
            if(readdata.isNP)
            {
                *(int *)((uint64_t)org_get_BattleSvtData(_this)+0x154) = 30000;
            }
            if(readdata.isCD)
            {
                for (int i = 0; i < 3; ++i) {
                    *(int*)((uint64_t)org_getSelfSkillInfo(org_get_BattleSvtData(_this),i)+0x28) = 0;
                }
            }
        }

        if(*(bool *)((uint64_t)org_get_BattleSvtData(_this)+0x1c7)&&attack)
        {
            *(int *)((uint64_t)org_get_BattleSvtData(_this)+0x13c) = 0;
        }
    }
    return org_Update(_this);
}




void hook_function()
{
    auto domain = il2cpp_domain_get();
    size_t size;
    auto assemblies = il2cpp_domain_get_assemblies(domain, &size);
    int assemble_csharp_index = 0;
    for(int i = 0; i < size; ++i) {
        auto image = il2cpp_assembly_get_image(assemblies[i]);
        if(strcmp(il2cpp_image_get_name(image), "Assembly-CSharp.dll") == 0) {
            assemble_csharp_index = i;
            LOGD("assemble_csharp_index : %d",assemble_csharp_index);
            break;
        }
    }
    auto image_game = il2cpp_assembly_get_image(assemblies[assemble_csharp_index]);
    Il2CppClass* BattleActorControl = il2cpp_class_from_name(image_game, "", "BattleActorControl");
    Il2CppClass* BattleServantData = il2cpp_class_from_name(image_game, "", "BattleServantData");

    DobbyHook((void *)(il2cpp_class_get_method_from_name(BattleActorControl, "get_BattleSvtData",0)->methodPointer),
              (void*)replace_get_BattleSvtData,
              (void**)&org_get_BattleSvtData);
    DobbyHook((void *)(il2cpp_class_get_method_from_name(BattleServantData, "getSelfSkillInfo",1)->methodPointer),
              (void*)replace_getSelfSkillInfo,
              (void**)&org_getSelfSkillInfo);
    DobbyHook((void *)(il2cpp_class_get_method_from_name(BattleServantData, "getAttackType",1)->methodPointer),
              (void*)replace_getAttackType,
              (void**)&org_getAttackType);
    DobbyHook((void *)(il2cpp_class_get_method_from_name(BattleServantData, "turnProgressing",4)->methodPointer),
              (void*)replace_turnProgressing,
              (void**)&org_turnProgressing);
    DobbyHook((void *)(il2cpp_class_get_method_from_name(BattleServantData, "getNowHp",0)->methodPointer),
              (void*)replace_getNowHp,
              (void**)&org_getNowHp);
    DobbyHook((void *)(il2cpp_class_get_method_from_name(BattleServantData, "getMaxHp",0)->methodPointer),
              (void*)replace_getMaxHp,
              (void**)&org_getMaxHp);
    DobbyHook((void *)(il2cpp_class_get_method_from_name(BattleActorControl, "Update",0)->methodPointer),
              (void*)replace_Update,
              (void**)&org_Update);

}