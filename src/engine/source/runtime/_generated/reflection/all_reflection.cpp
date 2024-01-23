#include "_generated/reflection/axis_aligned.gen.h"
#include "_generated/reflection/matrix4.gen.h"
#include "_generated/reflection/quaternion.gen.h"
#include "_generated/reflection/transform.gen.h"
#include "_generated/reflection/vector2.gen.h"
#include "_generated/reflection/vector3.gen.h"
#include "_generated/reflection/vector4.gen.h"
#include "_generated/reflection/actor.gen.h"
#include "_generated/reflection/controller.gen.h"
#include "_generated/reflection/player_controller.gen.h"
#include "_generated/reflection/game_mode.gen.h"
#include "_generated/reflection/level_setting.gen.h"
#include "_generated/reflection/pawn.gen.h"
#include "_generated/reflection/child_actor_component.gen.h"
#include "_generated/reflection/component.gen.h"
#include "_generated/reflection/input_component.gen.h"
#include "_generated/reflection/transform_component.gen.h"
#include "_generated/reflection/engine.gen.h"
#include "_generated/reflection/game_engine.gen.h"
#include "_generated/reflection/game_viewport_client.gen.h"
#include "_generated/reflection/game_instance.gen.h"
#include "_generated/reflection/level.gen.h"
#include "_generated/reflection/local_player.gen.h"
#include "_generated/reflection/player.gen.h"
#include "_generated/reflection/world.gen.h"

void ::GEngine::Reflection::autoRegisterAll() {
    ::GEngine::Reflection::Register::RegisterAxisAlignedBox()();
    ::GEngine::Reflection::Register::RegisterMatrix4x4_()();
    ::GEngine::Reflection::Register::RegisterQuaternion()();
    ::GEngine::Reflection::Register::RegisterTransform()();
    ::GEngine::Reflection::Register::RegisterVector2()();
    ::GEngine::Reflection::Register::RegisterVector3()();
    ::GEngine::Reflection::Register::RegisterVector4()();
    ::GEngine::Reflection::Register::RegisterActor()();
    ::GEngine::Reflection::Register::RegisterController()();
    ::GEngine::Reflection::Register::RegisterPlayerController()();
    ::GEngine::Reflection::Register::RegisterGameMode()();
    ::GEngine::Reflection::Register::RegisterLevelSetting()();
    ::GEngine::Reflection::Register::RegisterPawn()();
    ::GEngine::Reflection::Register::RegisterChildActorComponent()();
    ::GEngine::Reflection::Register::RegisterActorComponent()();
    ::GEngine::Reflection::Register::RegisterInputComponent()();
    ::GEngine::Reflection::Register::RegisterTransformComponent()();
    ::GEngine::Reflection::Register::RegisterEngine()();
    ::GEngine::Reflection::Register::RegisterGameEngine()();
    ::GEngine::Reflection::Register::RegisterGameViewportClient()();
    ::GEngine::Reflection::Register::RegisterGameInstance()();
    ::GEngine::Reflection::Register::RegisterLevel()();
    ::GEngine::Reflection::Register::RegisterLocalPlayer()();
    ::GEngine::Reflection::Register::RegisterPlayer()();
    ::GEngine::Reflection::Register::RegisterWorld()();
}
