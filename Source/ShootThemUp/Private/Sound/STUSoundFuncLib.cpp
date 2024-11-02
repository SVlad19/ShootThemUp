// Shoot Them Up Game. All Rights Reserved.

#include "Sound/STUSoundFuncLib.h"
#include "Sound/SoundClass.h"

void USTUSoundFuncLib::SetSoundClassVolume(USoundClass *SoundClass, float Volume)
{
    if (!SoundClass)
    {
        return;
    }

    SoundClass->Properties.Volume = FMath::Clamp(Volume, 0.f, 1.f);
}

void USTUSoundFuncLib::ToggleSoundClassVolume(USoundClass *SoundClass)
{
    if (!SoundClass)
    {
        return;
    }

    const auto NextVolume = SoundClass->Properties.Volume > 0.f ? 0.f : 1.f;
    SetSoundClassVolume(SoundClass, NextVolume);
}
