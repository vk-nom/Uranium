#pragma once

#include "Unreal.hpp"
#include "Globals.hpp"
#include "Structs.hpp"
#include "Offsets.hpp"
#include "enums.hpp"

namespace Functions
{


	static void InitConsole() {
		SpawnObjectParams params{ (UClass*)(Offsets::Engine::ConsoleClass), (UObject*)(Offsets::Engine::GameViewport) };
		UObject** ViewportConsole = reinterpret_cast<UObject**>(reinterpret_cast<__int64>(Offsets::Engine::GameViewport)+ 0x40);
		*ViewportConsole = params.ReturnValue;
	}
	static inline void SwitchLevel(FString URL)
	{
		static auto fn = FindObject("Function /Script/Engine.PlayerController.SwitchLevel");
		ProcessEvent(Globals::PlayerController, fn, &URL);
	}


	static UObject* Posess(UObject* Inpawn) {
		auto PossessFunc = FindObject("Function /Script/Engine.Controller.Possess");
		struct
		{
			class UObject* InPawn;                                                   // (Parm, ZeroConstructor, IsPlainOldData)
		} possesparams;
		possesparams.InPawn = Inpawn;
		ProcessEvent(Globals::PlayerController, PossessFunc, &possesparams);
	}



	static inline UObject* SpawnActor(UObject* ActorClass, FVector loc) {
		auto spawnfunction1 = FindObject("Function /Script/Engine.GameplayStatics.BeginDeferredActorSpawnFromClass");
		auto spawnfunction2 = FindObject("Function /Script/Engine.GameplayStatics.FinishSpawningActor");

		UGameplayStatics_BeginDeferredActorSpawnFromClass_Params bdasfc;
		UGameplayStatics_FinishSpawningActor_Params fsap;

		bdasfc.ActorClass = (UClass*)ActorClass;
		bdasfc.CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		bdasfc.SpawnTransform.Rotation = FQuat{ 0,0,0,0 };
		bdasfc.SpawnTransform.Scale3D = FVector{ 1,1,1 };
		bdasfc.SpawnTransform.Translation = loc;
		bdasfc.WorldContextObject = (UObject*)Offsets::GameViewportClient::World;
		bdasfc.Owner = nullptr;

		auto GameplayStatics = FindObject("Default__GameplayStatics");

		std::cout << GameplayStatics->GetFullName();
		std::cout << spawnfunction1->GetFullName();

		ProcessEvent(GameplayStatics, spawnfunction1, &bdasfc);

		fsap.Actor = (UObject*)bdasfc.ReturnValue;
		fsap.SpawnTransform = bdasfc.SpawnTransform;

		ProcessEvent(GameplayStatics, spawnfunction2, &fsap);

		return (UObject*)fsap.ReturnValue;
	}


}