// Fill out your copyright notice in the Description page of Project Settings.

#include "Debug/DebugLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/OutputDevice.h"
#include "HAL/PlatformStackWalk.h"

FString UDebugLibrary::GetScriptStackTrace()
{
    // 1. ��ȡԭʼ��ջ
    ANSICHAR StackTrace[65536];
    FPlatformStackWalk::StackWalkAndDump(StackTrace, UE_ARRAY_COUNT(StackTrace), 0, nullptr);
    FString StackStr = FString(UTF8_TO_TCHAR(StackTrace));

    // 2. ���д���
    TArray<FString> Lines;
    StackStr.ParseIntoArray(Lines, TEXT("\n"));

    // 3. ����������ֻ��������ĿDLL����ͼ��ص���
    TArray<FString> Filtered;
    for (const FString& Line : Lines)
    {
        // ���������Ŀģ�����ƻ���ͼǰ׺
        if (Line.Contains(TEXT("WhiteBox")) || Line.Contains(TEXT("_C_")))
        {
            Filtered.Add(Line);
        }
    }

    // 4. �ϲ����ַ���
    return FString::Join(Filtered, TEXT("\n"));
}