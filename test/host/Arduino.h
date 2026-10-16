#pragma once
#include <cstddef>
#include <cstdint>
#include <cstdlib>

using TaskHandle_t = void *;
constexpr int pdPASS = 1;
constexpr uint32_t pdMS_TO_TICKS(uint32_t ms) { return ms; }
uint32_t millis();
void delay(uint32_t ms);
void vTaskDelay(uint32_t ticks);
void vTaskDelete(TaskHandle_t task);
int xTaskCreate(void (*entry)(void *), const char *, uint32_t, void *, int,
                TaskHandle_t *handle);
