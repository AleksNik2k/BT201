#include "BT201.h"

BT201::BT201(uint8_t rxPin, uint8_t txPort) 
    : _serial(rxPin, txPort) {
    _trackCb = nullptr;
    _callCb = nullptr;
    _connCb = nullptr;
    _trackUserData = nullptr;
    _callUserData = nullptr;
    _connUserData = nullptr;
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
    success &= sendCommand("AT+U0" + String(service)); // Используем String
    success &= sendCommand("AT+U1" + String(char1));   // Используем String
    success &= sendCommand("AT+U2" + String(char2));   // Используем String
    success &= sendCommand("AT+U3" + String(char3));   // Используем String
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
    return sendCommand("AT+SPP" + String(data)); // Используем String
}

void BT201::setTrackCallback(void (*cb)(uint16_t, void*), void* userData) {
    _trackCb = cb;
    _trackUserData = userData;
}

void BT201::setCallCallback(void (*cb)(bool, void*), void* userData) {
    _callCb = cb;
    _callUserData = userData;
}

void BT201::setConnectionCallback(void (*cb)(bool, void*), void* userData) {
    _connCb = cb;
    _connUserData = userData;
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
        _callCb(true, _callUserData);
    } else if (strstr(event, "TS+") && _connCb) {
        _connCb(strstr(event + 3, "01") != nullptr, _connUserData);
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