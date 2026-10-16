#pragma once
#include <cstdint>

enum class NimBLETxPowerType { Advertise };
struct NimBLEDevice {
    static bool isInitialized() { return true; }
    static bool init(const char *) { return true; }
    static void setPower(int8_t, NimBLETxPowerType) {}
    static bool setOwnAddr(uint8_t *) { return true; }
    static bool setOwnAddrType(int) { return true; }
};
constexpr int BLE_OWN_ADDR_RANDOM = 1;
constexpr int BLE_GAP_CONN_MODE_NON = 0;
constexpr int BLE_GAP_DISC_MODE_NON = 0;
constexpr int BLE_HS_FOREVER = -1;
struct ble_gap_adv_params {
    int conn_mode, disc_mode;
    uint16_t itvl_min, itvl_max;
};
inline int ble_gap_adv_stop() { return 0; }
inline int ble_gap_adv_set_data(uint8_t *, uint8_t) { return 0; }
inline int ble_gap_adv_start(int, void *, int, ble_gap_adv_params *, void *, void *) {
    return 0;
}
