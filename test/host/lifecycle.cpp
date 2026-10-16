// Compile the actual implementation; only hardware/FreeRTOS calls are faked.
#include <cstdio>
#include "../../src/FindMyAdv.cpp"

// Exit normally on failure, avoiding Windows assertion-dialog side effects.
#define CHECK(condition) do { if (!(condition)) { \
    std::fprintf(stderr, "Lifecycle check failed at line %d: %s\n", __LINE__, #condition); \
    std::exit(1); \
} } while (false)

static uint32_t clockMs = 0;
static unsigned liveTasks = 0, createdTasks = 0, overlappingTasks = 0;
static unsigned forcedDeletes = 0;
static bool stalledTask = false;

uint32_t millis() { return clockMs; }
void delay(uint32_t ms) {
    clockMs += ms;
    // Model the old task seeing end()'s stop request on its next wakeup.
    if (!stalledTask && stopRequested && schedulerTask) {
        --liveTasks;
        schedulerTask = nullptr;
    }
}
void vTaskDelay(uint32_t ticks) { delay(ticks); }
void vTaskDelete(TaskHandle_t task) {
    CHECK(task != nullptr);  // The test never actually enters a scheduler loop.
    CHECK(liveTasks > 0);
    --liveTasks;
    ++forcedDeletes;
}
int xTaskCreate(void (*)(void *), const char *, uint32_t, void *, int,
                TaskHandle_t *handle) {
    if (liveTasks) ++overlappingTasks;
    ++liveTasks;
    *handle = reinterpret_cast<void *>(static_cast<uintptr_t>(++createdTasks));
    return pdPASS;
}

int main() {
    FindMyAdvConfig config;
    config.googleAdvertisementEidHex = "0000000000000000000000000000000000000000";
    CHECK(FindMyAdv.begin(config));
    CHECK(liveTasks == 1);
    const unsigned initialCreated = createdTasks;
    CHECK(FindMyAdv.begin(config));  // Already running is still a no-op.
    CHECK(createdTasks == initialCreated);
    for (int i = 0; i < 5; ++i) {
        FindMyAdv.end();
        CHECK(FindMyAdv.begin(config));
        CHECK(overlappingTasks == 0);
        CHECK(liveTasks == 1);
    }
    CHECK(FindMyAdv.reconfigure(config));
    CHECK(liveTasks == 1 && overlappingTasks == 0);
    FindMyAdv.end();
    config.backgroundTask = false;
    CHECK(FindMyAdv.begin(config));
    CHECK(liveTasks == 0);  // Background -> manual must drain the old task.
    FindMyAdv.poll();
    FindMyAdv.end();
    config.backgroundTask = true;
    CHECK(FindMyAdv.begin(config));
    FindMyAdv.end();
    stalledTask = true;
    CHECK(FindMyAdv.begin(config));
    CHECK(forcedDeletes == 1);  // Preserve the bounded shutdown fallback.
    CHECK(liveTasks == 1 && overlappingTasks == 0);
    stalledTask = false;
    FindMyAdv.end();
    delay(100);
    CHECK(liveTasks == 0);
}
