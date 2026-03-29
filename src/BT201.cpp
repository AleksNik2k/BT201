#include "BT201.h"

BT201::BT201(uint8_t rxPin, uint8_t txPin) 
    : _serial(rxPin, txPin) {
    _trackCb = nullptr;
    _callCb = nullptr;
    _connCb = nullptr;
    _deviceCb = nullptr;
    _trackUserData = nullptr;
    _callUserData = nullptr;
    _connUserData = nullptr;
    _deviceUserData = nullptr;
    _commandQueueHead = 0;
    _commandQueueTail = 0;
}

bool BT201::begin(unsigned long baud) {
    _serial.begin(baud);
    return sendCommand("AT") && readResponse(_responseBuffer, BT201_RESPONSE_BUFFER_SIZE) && 
           strstr(_responseBuffer, "OK") != nullptr;
}

bool BT201::reset() {
    return sendCommand("AT+CZ");
}

bool BT201::factoryReset() {
    return sendCommand("AT+CW");
}

bool BT201::setBaudRate(unsigned long baud) {
    const unsigned long baudRates[] = {9600, 19200, 38400, 57600, 115200};
    for (uint8_t i = 0; i < 5; i++) {
        if (baud == baudRates[i]) {
            char cmd[16];
            snprintf(cmd, sizeof(cmd), "AT+CT%02d", i);
            return sendCommand(cmd);
        }
    }
    return false;
}

String BT201::getFirmwareVersion() {
    if (sendCommand("AT+VER") && readResponse(_responseBuffer, BT201_RESPONSE_BUFFER_SIZE)) {
        return String(_responseBuffer + 3); // Пропускаем "VER"
    }
    return "";
}

bool BT201::setPowerMode(uint8_t mode) {
    char cmd[16];
    snprintf(cmd, sizeof(cmd), "AT+CP%02d", mode);
    return sendCommand(cmd);
}

bool BT201::enableLowPowerMode(bool enable) {
    return sendCommand(enable ? "AT+CL1" : "AT+CL0");
}

bool BT201::setVolume(uint8_t level) {
    if (level > 30) return false;
    char cmd[16];
    snprintf(cmd, sizeof(cmd), "AT+CA%02d", level);
    return sendCommand(cmd);
}

bool BT201::volumeUp() {
    return sendCommand("AT+CE");
}

bool BT201::volumeDown() {
    return sendCommand("AT+CF");
}

bool BT201::mute(bool enable) {
    return sendCommand(enable ? "AT+CU1" : "AT+CU0");
}

bool BT201::play() {
    return sendCommand("AT+AA1");
}

bool BT201::pause() {
    return sendCommand("AT+AA2");
}

bool BT201::nextTrack() {
    return sendCommand("AT+CC");
}

bool BT201::prevTrack() {
    return sendCommand("AT+CD");
}

bool BT201::playTrack(uint16_t index) {
    char cmd[16];
    snprintf(cmd, sizeof(cmd), "AT+AB%04d", index);
    return sendCommand(cmd);
}

bool BT201::playFolder(const char* path) {
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "AT+AF/%s/*???", path);
    return sendCommand(cmd);
}

bool BT201::setPlayMode(uint8_t mode) {
    char cmd[16];
    snprintf(cmd, sizeof(cmd), "AT+AC%02d", mode);
    return sendCommand(cmd);
}

bool BT201::isPlaying() {
    return sendCommand("AT+MP") && readResponse(_responseBuffer, BT201_RESPONSE_BUFFER_SIZE) && 
           strstr(_responseBuffer, "1") != nullptr;
}

String BT201::getCurrentTrackName() {
    if (sendCommand("AT+MF") && readResponse(_responseBuffer, BT201_RESPONSE_BUFFER_SIZE)) {
        return String(_responseBuffer + 3); // Пропускаем "MF+"
    }
    return "";
}

uint16_t BT201::getTrackPosition() {
    if (sendCommand("AT+MK") && readResponse(_responseBuffer, BT201_RESPONSE_BUFFER_SIZE)) {
        return atoi(_responseBuffer + 3); // Пропускаем "MK+"
    }
    return 0;
}

uint16_t BT201::getTrackDuration() {
    if (sendCommand("AT+MT") && readResponse(_responseBuffer, BT201_RESPONSE_BUFFER_SIZE)) {
        return atoi(_responseBuffer + 3); // Пропускаем "MT+"
    }
    return 0;
}

bool BT201::setBluetoothName(const char* name) {
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "AT+BD%s", name);
    return sendCommand(cmd);
}

bool BT201::setBluetoothPIN(const char* pin) {
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "AT+BE%s", pin);
    return sendCommand(cmd);
}

bool BT201::connectDevice() {
    return sendCommand("AT+BA5");
}

bool BT201::disconnect() {
    return sendCommand("AT+BA1");
}

bool BT201::answerCall() {
    return sendCommand("AT+BA4");
}

bool BT201::endCall() {
    return sendCommand("AT+BA3");
}

bool BT201::redial() {
    return sendCommand("AT+BA6");
}

bool BT201::setBLEUUID(const char* service, const char* char1, const char* char2, const char* char3) {
    bool success = true;
    success &= sendCommand(String("AT+U0") + service);
    success &= sendCommand(String("AT+U1") + char1);
    success &= sendCommand(String("AT+U2") + char2);
    success &= sendCommand(String("AT+U3") + char3);
    return success;
}

bool BT201::sendBLEData(const uint8_t* data, size_t length) {
    String cmd = "AT+BLE";
    for (size_t i = 0; i < length; i++) {
        cmd += String(data[i], HEX);
    }
    return sendCommand(cmd); // Используем перегруженный метод
}

bool BT201::sendSPPData(const char* data) {
    return sendCommand(String("AT+SPP") + data);
}

void BT201::setTrackCallback(void (*cb)(uint16_t, void*), void* userData) {
    _trackCb = cb;
    _trackUserData = userData;
}

void BT201::setCallCallback(void (*cb)(const char*, void*), void* userData) {
    _callCb = cb;
    _callUserData = userData;
}

void BT201::setConnectionCallback(void (*cb)(bool, void*), void* userData) {
    _connCb = cb;
    _connUserData = userData;
}

void BT201::setDeviceCallback(void (*cb)(uint8_t, void*), void* userData) {
    _deviceCb = cb;
    _deviceUserData = userData;
}

// Новые методы для управления аудио
bool BT201::stop() {
    return sendCommand("AT+AA00");
}

bool BT201::fastForward() {
    return sendCommand("AT+AA04");
}

bool BT201::fastBackward() {
    return sendCommand("AT+AA05");
}

bool BT201::nextFolder() {
    return sendCommand("AT+AA06");
}

bool BT201::prevFolder() {
    return sendCommand("AT+AA07");
}

bool BT201::deleteCurrentFile() {
    return sendCommand("AT+AA08");
}

bool BT201::setDAC(bool enable) {
    return sendCommand(enable ? "AT+CS01" : "AT+CS00");
}

bool BT201::setEQ(uint8_t eq) {
    if (eq > 5) return false;
    char cmd[16];
    snprintf(cmd, sizeof(cmd), "AT+CQ%02d", eq);
    return sendCommand(cmd);
}

// Новые методы для статуса медиа
uint16_t BT201::getCurrentTrackIndex() {
    String resp = queryCommand("AT+M1");
    if (resp.startsWith("M1+")) {
        return strtol(resp.substring(3).c_str(), nullptr, 16);
    }
    return 0;
}

uint16_t BT201::getTotalTracks() {
    String resp = queryCommand("AT+M2");
    if (resp.startsWith("M2+")) {
        return strtol(resp.substring(3).c_str(), nullptr, 16);
    }
    return 0;
}

uint8_t BT201::getCurrentDevice() {
    String resp = queryCommand("AT+MD");
    if (resp.startsWith("MD+")) {
        return atoi(resp.substring(3).c_str());
    }
    return 0;
}

uint8_t BT201::getOnlineDevices() {
    String resp = queryCommand("AT+MV");
    if (resp.startsWith("MV+")) {
        return atoi(resp.substring(3).c_str());
    }
    return 0;
}

// Новые методы для Bluetooth
bool BT201::setBLEName(const char* name) {
    String cmd = "AT+BM" + String(name);
    return sendCommand(cmd);
}

bool BT201::setBLEPIN(const char* pin) {
    String cmd = "AT+BN" + String(pin);
    return sendCommand(cmd);
}

bool BT201::setMACAddress(const char* mac) {
    String cmd = "AT+BS" + String(mac);
    return sendCommand(cmd);
}

bool BT201::dialNumber(const char* number) {
    String cmd = "AT+BT" + String(number);
    return sendCommand(cmd);
}

bool BT201::setBluetoothAudio(bool enable) {
    return sendCommand(enable ? "AT+B301" : "AT+B300");
}

bool BT201::setBluetoothCalls(bool enable) {
    return sendCommand(enable ? "AT+B201" : "AT+B200");
}

bool BT201::setEDR(bool enable) {
    return sendCommand(enable ? "AT+B501" : "AT+B500");
}

bool BT201::setBLE(bool enable) {
    return sendCommand(enable ? "AT+B401" : "AT+B400");
}

bool BT201::setSimplePassword(bool enable) {
    return sendCommand(enable ? "AT+B101" : "AT+B100");
}

// Новые методы для режимов
bool BT201::setMode(uint8_t mode) {
    char cmd[16];
    snprintf(cmd, sizeof(cmd), "AT+CM%02d", mode);
    return sendCommand(cmd);
}

bool BT201::setPowerOnMode(uint8_t mode) {
    char cmd[16];
    snprintf(cmd, sizeof(cmd), "AT+CP%02d", mode);
    return sendCommand(cmd);
}

bool BT201::setAutoReturn(bool enable) {
    return sendCommand(enable ? "AT+CR01" : "AT+CR00");
}

bool BT201::setSingleTriggerPlay(bool enable) {
    return sendCommand(enable ? "AT+CJ01" : "AT+CJ00");
}

bool BT201::setRecording(bool enable) {
    return sendCommand(enable ? "AT+RE01" : "AT+RE00");
}

bool BT201::setPrompt(bool enable) {
    return sendCommand(enable ? "AT+CN01" : "AT+CN00");
}

bool BT201::setAutoSwitchToBluetooth(bool enable) {
    return sendCommand(enable ? "AT+CK01" : "AT+CK00");
}

bool BT201::setBluetoothBackground(bool enable) {
    return sendCommand(enable ? "AT+CG01" : "AT+CG00");
}

bool BT201::setKeyFunction(bool enable) {
    return sendCommand(enable ? "AT+C101" : "AT+C100");
}

bool BT201::setActiveReturn(bool enable) {
    return sendCommand(enable ? "AT+C201" : "AT+C200");
}

bool BT201::setBluetoothCallback(bool enable) {
    return sendCommand(enable ? "AT+C401" : "AT+C400");
}

// Новые методы для записи
bool BT201::startRecording() {
    return sendCommand("AT+RC01");
}

bool BT201::pauseRecording() {
    return sendCommand("AT+RC02");
}

bool BT201::stopRecording() {
    return sendCommand("AT+RC03");
}

bool BT201::playRecording() {
    return sendCommand("AT+RC04");
}

bool BT201::deleteRecording() {
    return sendCommand("AT+RC07");
}

bool BT201::setRecordingPath(const char* path) {
    String cmd = "AT+RP" + String(path);
    return sendCommand(cmd);
}

bool BT201::setRecordingBitrate(uint8_t bitrate) {
    if (bitrate > 8) return false;
    char cmd[16];
    snprintf(cmd, sizeof(cmd), "AT+RB%02d", bitrate);
    return sendCommand(cmd);
}

bool BT201::setMICGain(uint8_t gain) {
    if (gain > 63) return false;
    char cmd[16];
    snprintf(cmd, sizeof(cmd), "AT+RV%02d", gain);
    return sendCommand(cmd);
}

// Новые методы для файловых операций
bool BT201::readFile(const char* path) {
    String cmd = "AT+AR" + String(path);
    return sendCommand(cmd);
}

bool BT201::deleteFile(const char* path) {
    String cmd = "AT+AL" + String(path);
    return sendCommand(cmd);
}

// Новые методы для запросов
uint8_t BT201::getVolume() {
    String resp = queryCommand("AT+QA");
    if (resp.startsWith("QA+")) {
        return atoi(resp.substring(3).c_str());
    }
    return 0;
}

uint8_t BT201::getBaudRateIndex() {
    String resp = queryCommand("AT+QT");
    if (resp.startsWith("QT+")) {
        return atoi(resp.substring(3).c_str());
    }
    return 0;
}

uint8_t BT201::getMode() {
    String resp = queryCommand("AT+QM");
    if (resp.startsWith("QM+")) {
        return atoi(resp.substring(3).c_str());
    }
    return 0;
}

bool BT201::getPromptEnabled() {
    String resp = queryCommand("AT+QN");
    return resp.startsWith("QN+01");
}

bool BT201::getAutoSwitchEnabled() {
    String resp = queryCommand("AT+QK");
    return resp.startsWith("QK+01");
}

bool BT201::getBluetoothBackgroundEnabled() {
    String resp = queryCommand("AT+QG");
    return resp.startsWith("QG+01");
}

bool BT201::getKeyFunctionEnabled() {
    String resp = queryCommand("AT+Q1");
    return resp.startsWith("Q1+01");
}

String BT201::getBluetoothPassword() {
    String resp = queryCommand("AT+TE");
    if (resp.startsWith("TE+")) {
        return resp.substring(3);
    }
    return "";
}

String BT201::getBluetoothNameEDR() {
    String resp = queryCommand("AT+TD");
    if (resp.startsWith("TD+")) {
        return resp.substring(3);
    }
    return "";
}

String BT201::getBluetoothAddressEDR() {
    String resp = queryCommand("AT+TA");
    if (resp.startsWith("TA+")) {
        return resp.substring(3);
    }
    return "";
}

String BT201::getBluetoothNameBLE() {
    String resp = queryCommand("AT+TM");
    if (resp.startsWith("TM+")) {
        return resp.substring(3);
    }
    return "";
}

String BT201::getBluetoothAddressBLE() {
    String resp = queryCommand("AT+TB");
    if (resp.startsWith("TB+")) {
        return resp.substring(3);
    }
    return "";
}

bool BT201::getIOSConnect() {
    String resp = queryCommand("AT+TI");
    return resp.startsWith("TI+01");
}

uint8_t BT201::getBluetoothStatus() {
    String resp = queryCommand("AT+TS");
    if (resp.startsWith("TS+")) {
        return atoi(resp.substring(3).c_str());
    }
    return 0;
}

String BT201::getCallNumber() {
    String resp = queryCommand("AT+TT");
    if (resp.startsWith("TT+")) {
        return resp.substring(3);
    }
    return "";
}

bool BT201::getSimplePasswordEnabled() {
    String resp = queryCommand("AT+T1");
    return resp.startsWith("T1+01");
}

bool BT201::getCallsEnabled() {
    String resp = queryCommand("AT+T2");
    return resp.startsWith("T2+01");
}

bool BT201::getAudioEnabled() {
    String resp = queryCommand("AT+T3");
    return resp.startsWith("T3+01");
}

bool BT201::getBLEEnabled() {
    String resp = queryCommand("AT+T4");
    return resp.startsWith("T4+01");
}

bool BT201::getEDREnabled() {
    String resp = queryCommand("AT+T5");
    return resp.startsWith("T5+01");
}

String BT201::getServiceUUID() {
    String resp = queryCommand("AT+T6");
    if (resp.startsWith("T6+")) {
        return resp.substring(3);
    }
    return "";
}

String BT201::getCharUUID(uint8_t index) {
    if (index < 1 || index > 3) return "";
    char cmd[16];
    snprintf(cmd, sizeof(cmd), "AT+T%d", 6 + index);
    String resp = queryCommand(cmd);
    if (resp.startsWith(String("T") + String(6 + index) + "+")) {
        return resp.substring(3);
    }
    return "";
}

void BT201::processIncoming() {
    while (_serial.available()) {
        char c = _serial.read();
        if (c == '\n') {
            handleEvent(_responseBuffer);
            _responseBuffer[0] = '\0'; // Очистка буфера
        } else if (strlen(_responseBuffer) < BT201_RESPONSE_BUFFER_SIZE - 1) {
            _responseBuffer[strlen(_responseBuffer)] = c;
            _responseBuffer[strlen(_responseBuffer) + 1] = '\0';
        }
    }
    
    // Обработка очереди команд
    if (_commandQueueHead != _commandQueueTail) {
        dequeueCommand();
    }
}

void BT201::handleEvent(const char* event) {
    if (strstr(event, "MV+") && _trackCb) {
        _trackCb(atoi(event + 3), _trackUserData);
    } else if (strstr(event, "TT+") && _callCb) {
        _callCb(event + 3, _callUserData);
    } else if (strstr(event, "TS+") && _connCb) {
        _connCb(strstr(event + 3, "01") != nullptr, _connUserData);
    } else if (strstr(event, "MU+") && _deviceCb) {
        _deviceCb(atoi(event + 3), _deviceUserData);
    }
}

bool BT201::sendCommand(const char* cmd) {
    return enqueueCommand(cmd);
}

bool BT201::sendCommand(const String& cmd) {
    return enqueueCommand(cmd.c_str()); // Преобразуем String в const char*
}

bool BT201::enqueueCommand(const char* cmd) {
    if ((_commandQueueHead + 1) % BT201_COMMAND_QUEUE_SIZE == _commandQueueTail) {
        return false; // Очередь переполнена
    }
    _commandQueue[_commandQueueHead] = String(cmd);
    _commandQueueHead = (_commandQueueHead + 1) % BT201_COMMAND_QUEUE_SIZE;
    return true;
}

bool BT201::dequeueCommand() {
    if (_commandQueueHead == _commandQueueTail) {
        return false; // Очередь пуста
    }
    String cmd = _commandQueue[_commandQueueTail];
    _commandQueueTail = (_commandQueueTail + 1) % BT201_COMMAND_QUEUE_SIZE;
    _serial.print(cmd + "\r\n");
    return true;
}

bool BT201::readResponse(char* buffer, size_t size, unsigned long timeout) {
    unsigned long start = millis();
    size_t index = 0;
    while (millis() - start < timeout && index < size - 1) {
        if (_serial.available()) {
            char c = _serial.read();
            buffer[index++] = c;
            if (c == '\n') {
                buffer[index] = '\0';
                return true;
            }
        }
    }
    buffer[index] = '\0';
    return false;
}

bool BT201::validateResponse(const char* response) {
    return strstr(response, "OK") != nullptr && 
           strstr(response, "ER") == nullptr;
}

String BT201::queryCommand(const char* cmd) {
    if (sendCommand(cmd) && readResponse(_responseBuffer, BT201_RESPONSE_BUFFER_SIZE)) {
        return String(_responseBuffer);
    }
    return "";
}