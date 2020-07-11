// dllmain.cpp : Define el punto de entrada de la aplicación DLL.
#include "pch.h"
#include "layers_scenes_transitions_nodes/CCScene.h""
#include "CCDirector.h"
#include "sprite_nodes/CCSpriteFrameCache.h"
#include "sprite_nodes/CCSpriteFrame.h"
//#include "cocos2d.h"
#include "sprite_nodes/CCSprite.h"
#include<iostream>
#include <math.h>

using namespace cocos2d;


void placeJMP(BYTE* address, DWORD jumpTo, DWORD length)
{
    DWORD oldProtect, newProtect, relativeAddress;
    VirtualProtect(address, length, PAGE_EXECUTE_READWRITE, &oldProtect);
    relativeAddress = (DWORD)(jumpTo - (DWORD)address) - 5;
    *address = 0xE9;
    *((DWORD*)(address + 0x1)) = relativeAddress;
    for (DWORD x = 0x5; x < length; x++)
    {
        *(address + x) = 0x90;
    }
    VirtualProtect(address, length, oldProtect, &newProtect);
}    
auto scene = cocos2d::CCScene::create();



auto a = CCDirector::sharedDirector();


cocos2d::CCScene* director;
void test() {
    auto spfc = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("GJ_garageBtn_001.png");
    spfc->retain();
    auto s = CCSprite::createWithSpriteFrameName("GJ_garageBtn_001.png");
    s->setPosition(CCPoint(rand() % (255 - 0 + 1) + 0, rand() % (255 - 0 + 1) + 0));
    director->addChild(s);
   // scene->addChild(box);
}

__declspec(naked) void hacc() {
    __asm pushad
        // MessageBoxA(NULL, (LPCSTR)"Editor Hack Disabled!", (LPCSTR)"EditorHack Disabled!", MB_OK);
    director = cocos2d::CCDirector::sharedDirector()->getRunningScene();

    test();

    __asm popad
    __asm ret 4
}

DWORD base = (DWORD)GetModuleHandleA(0);
DWORD haccc = (DWORD)hacc;
DWORD retJMP = base + 0x190C28;

__declspec(naked) void myMidfuncHook() {

    __asm {

        push haccc
        jmp[retJMP]
    }


}


void mainHack() {

    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    std::cout << "Base: " << base << std::endl;
 
  //  labbel->setPosition(cocos2d::CCPoint(50, 50));
    placeJMP((BYTE*)base + 0x190C23, (DWORD)myMidfuncHook, 0x5);

}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)mainHack, hModule, 0, nullptr);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

