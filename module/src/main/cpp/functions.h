#ifndef ZYCHEATS_SGUYS_FUNCTIONS_H
#define ZYCHEATS_SGUYS_FUNCTIONS_H
// here you can define variables for the patches
typedef struct {
    bool isSp= false;
    bool isCost= false;
    int AtkValue=1;
    int DefValue=1;
    int AttackSpeedValue=1;
    bool is_blockCnt= false;
    bool isUndeadable= false;
} SharedData;

SharedData readdata;
std::string path = "/data/data/" + std::string(GamePackageName) + "/files/sharedata.bin";
void readSharedDataFromFile(const char* filename, SharedData* data) {
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

monoString *CreateIl2cppString(const char *str) {
    monoString *(*String_CreateString)(void *instance, const char *str) = (monoString*(*)(void*, const char*)) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x2596B20")));
    return String_CreateString(NULL, str);
}

void (*PurchaseRealMoney) (void* instance, monoString* itemId, monoString* receipt, void* callback);

void Pointers() {
    PurchaseRealMoney = (void(*)(void*, monoString*, monoString*, void*)) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0xE7AADC")));
}




// declare your hooks here



enum SideType
{
    // Token: 0x04009076 RID: 36982
    NONE,
    // Token: 0x04009077 RID: 36983
    ALLY,
    // Token: 0x04009078 RID: 36984
    ENEMY,
    // Token: 0x04009079 RID: 36985
    BOTH_ALLY_AND_ENEMY,
    // Token: 0x0400907A RID: 36986
    NEUTRAL,
    // Token: 0x0400907B RID: 36987
    ALL = 7
};
void * (*old_op_Implicit_int)(void *_this,int value);
void * op_Implicit_int(void *_this,int value)
{
    return old_op_Implicit_int(_this,value);
}

void * (*old_op_Implicit_float)(void *_this,float value);
void * op_Implicit_float(void *_this,float value)
{
    return old_op_Implicit_float(_this,value);
}


int (*old_ToInt)(void*_this);
int ToInt(void*_this)
{
    return old_ToInt(_this);
}


float (*old_ToFloat)(void*_this);
float ToFloat(void*_this)
{
    return old_ToFloat(_this);
}




bool (*old_canCastWithNoSp)(void *instance);
bool canCastWithNoSp(void *instance) {
    if(instance!=NULL&&readdata.isSp)
    {
        return true;
    }
    return old_canCastWithNoSp(instance);
}


bool (*old_readyToSpawn)(void *instance);
bool readyToSpawn(void *instance)
{
    readSharedDataFromFile(path.c_str(),&readdata);
    if(instance!=NULL&&readdata.isCost)
    {
        return true;
    }
    return old_readyToSpawn(instance);
}



void* (*old_Atk)(void *instance);
void * Atk(void *instance)
{
    if(instance!=NULL&&readdata.AtkValue!=1)
    {
        if(*(SideType *)((uint64_t)instance + 0x1c)==(SideType)1)
        {
            int a = ToInt(old_Atk(instance));
            return op_Implicit_int(old_Atk(instance),a*readdata.AtkValue);
        }
    }
    return old_Atk(instance);
}


void* (*old_Def)(void *instance);
void * Def(void *instance)
{
    if(instance!=NULL&&readdata.DefValue!=1)
    {
        if(*(SideType *)((uint64_t)instance + 0x1c)==(SideType)1)
        {
            int a = ToInt(old_Def(instance));
            return op_Implicit_int(old_Def(instance),a*readdata.DefValue);
        }
    }
    return old_Def(instance);
}


void* (*old_AttackSpeed)(void *instance);
void * AttackSpeed(void *instance)
{
    if(instance!=NULL&&readdata.AttackSpeedValue!=1)
    {
        if(*(SideType *)((uint64_t)instance + 0x1c)==(SideType)1)
        {
            float a = ToFloat(old_AttackSpeed(instance));
            return op_Implicit_float(old_AttackSpeed(instance),a*(float)readdata.AttackSpeedValue);
        }
    }
    return old_AttackSpeed(instance);
}
void* (*old_attackTime)(void *instance);
void * attackTime(void *instance)
{
    if(instance!=NULL&&readdata.AttackSpeedValue!=1)
    {
        if(*(SideType *)((uint64_t)instance + 0x1c)==(SideType)1)
        {
            float a = ToFloat(old_attackTime(instance));
            return op_Implicit_float(old_attackTime(instance),a/(float)readdata.AttackSpeedValue);
        }
    }
    return old_attackTime(instance);
}


int (*old_blockCnt)(void *instance);
int  blockCnt(void *instance)
{
    if(instance!=NULL&&readdata.is_blockCnt)
    {
        if(*(SideType *)((uint64_t)instance + 0x1c)==(SideType)1)
        {
            return 99;
        }
    }
    return old_blockCnt(instance);
}



bool (*old_Undeadable)(void *_this);
bool Undeadable(void *_this)
{
    if(_this!=NULL&&readdata.isUndeadable)
    {
        if(*(SideType *)((uint64_t)_this + 0x1c)==(SideType)1)
        {
            return true;
        }
    }
    return old_Undeadable(_this);
}


void (*old_DoCheck)(void* _this);
void DoCheck(void* _this)
{
    if(_this!=NULL)
    {
        return;
    }
    return old_DoCheck(_this);
}


void Hooks() {

    HOOK("0x2e3af54", DoCheck, old_DoCheck);
    HOOK( "0x518ffc8 ", ToInt, old_ToInt);
    HOOK( "0x518ff58 ", ToFloat, old_ToFloat);
    HOOK( "0x518f02c", op_Implicit_int, old_op_Implicit_int);
    HOOK("0x518fb78", op_Implicit_float, old_op_Implicit_float);
    HOOK( "0x35e20c4", Atk, old_Atk);
    HOOK( "0x35e2150", Def, old_Def);
    HOOK( "0x35e1df4", AttackSpeed, old_AttackSpeed);
    HOOK("0x35e1cb0", attackTime, old_attackTime);
    HOOK( "0x35e2524", blockCnt, old_blockCnt);
    HOOK("0x2F0D0B0", canCastWithNoSp, old_canCastWithNoSp);
    HOOK("0x2e5515c", readyToSpawn, old_readyToSpawn);//SpawnCharacterOrToken
    HOOK( "0x35e32ec", Undeadable, old_Undeadable);
}

void Patches() {
    PATCH_SWITCH("0x2E519B8", "08008052", readdata.isCost);
}
#endif //ZYCHEATS_SGUYS_FUNCTIONS_H
