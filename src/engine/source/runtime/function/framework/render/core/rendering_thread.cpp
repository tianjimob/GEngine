#include "rendering_thread.h"

#include "core/misc/config_cache_ini.h"

#include <thread>

namespace GEngine {
uint32_t RendringThread::run() {
  if (init()) {
    //   check(
    //       Queue(QueueIndex).StallRestartEvent);  // make sure we are started
    //       up
    //   Queue(QueueIndex).QuitForReturn = false;
    //   verify(++Queue(QueueIndex).RecursionGuard == 1);
    //   const bool bIsMultiThread = FTaskGraphInterface::IsMultithread();
    //   do {
    //     const bool bAllowStall = bIsMultiThread;
    //     // 具体处理逻辑
    //     ProcessTasksNamedThread(QueueIndex, bAllowStall);
    //   } while (!Queue(QueueIndex).QuitForReturn &&
    //            !Queue(QueueIndex).QuitForShutdown &&
    //            bIsMultiThread);  // @Hack - quit now when  running with only
    //            one
    //                              // thread.
    //   verify(!--Queue(QueueIndex).RecursionGuard);

    // 核心逻辑如下
    // while (!Queue(QueueIndex).QuitForReturn) {
    //   //...
    //   // 从队伍中取出Task
    //   FBaseGraphTask *Task =
    //       Queue(QueueIndex).StallQueue.Pop(0, bStallQueueAllowStall);
    //   // 执行任务，会调用到DoTask
    //   Task->Execute(
    //       NewTasks,
    //       ENamedThreads::Type(ThreadId |
    //                           (QueueIndex << ENamedThreads::QueueIndexShift)));
    // }

    exit();
    return 0;
  } else
    return 1;
}

uint32_t RHIThread::run()
{
  if (init()) {
    //   check(
    //       Queue(QueueIndex).StallRestartEvent);  // make sure we are started
    //       up
    //   Queue(QueueIndex).QuitForReturn = false;
    //   verify(++Queue(QueueIndex).RecursionGuard == 1);
    //   const bool bIsMultiThread = FTaskGraphInterface::IsMultithread();
    //   do {
    //     const bool bAllowStall = bIsMultiThread;
    //     // 具体处理逻辑
    //     ProcessTasksNamedThread(QueueIndex, bAllowStall);
    //   } while (!Queue(QueueIndex).QuitForReturn &&
    //            !Queue(QueueIndex).QuitForShutdown &&
    //            bIsMultiThread);  // @Hack - quit now when  running with only
    //            one
    //                              // thread.
    //   verify(!--Queue(QueueIndex).RecursionGuard);

    // 核心逻辑如下
    // while (!Queue(QueueIndex).QuitForReturn) {
    //   //...
    //   // 从队伍中取出Task
    //   FBaseGraphTask *Task =
    //       Queue(QueueIndex).StallQueue.Pop(0, bStallQueueAllowStall);
    //   // 执行任务，会调用到DoTask
    //   Task->Execute(
    //       NewTasks,
    //       ENamedThreads::Type(ThreadId |
    //                           (QueueIndex <<
    //                           ENamedThreads::QueueIndexShift)));
    // }

    exit();
    return 0;
  } else
    return 1;
}

bool gUseRenderingThread = false;
thread_local bool gIsRenderingThread = false;

bool gUseRHIThread = false;
thread_local bool gIsRHIThread = false;

static Runnable *gRenderingRunable = nullptr;
static Runnable* gRHIRunable = nullptr;

void startRenderingThread() {
  ConfigCacheIni &config = ConfigCacheIni::instance();
  auto getRenderSettingBool = [&config](const std::string &key,
                                       const bool &defaultValue) -> bool {
    auto res = defaultValue;
    config.getBool("Render", key, "game.ini", res);
    return res;
  };

  gUseRenderingThread = getRenderSettingBool("use_rendering_thread", true);
  if (!gUseRenderingThread)
    return;

  gRenderingRunable = new RendringThread;
  std::thread([]() { gRenderingRunable->run(); }).detach();

  gUseRHIThread = getRenderSettingBool("use_rhi_thread", true);
  if (!gUseRHIThread)
    return;

  gRHIRunable = new RHIThread;
  std::thread([]() { gRHIRunable->run(); }).detach();
}
}