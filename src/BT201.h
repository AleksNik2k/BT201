#ifndef BT201_H
#define BT201_H

#include <Arduino.h>
#include <SoftwareSerial.h>

#define BT201_RESPONSE_BUFFER_SIZE 128
#define BT201_COMMAND_QUEUE_SIZE 10
#define BT201_DEFAULT_TIMEOUT 1000

class BT201 {
public:
    // Конструктор
    BT201(uint8_t rxPin, uint8_t txPort);
    
    // Инициализация
    bool begin(unsigned long baud = 115200);
    
    // Системные команды
    bool reset();
    bool factoryReset();
    bool setBaudRate(unsigned long baud);
    String getFirmwareVersion();
    
    // Управление питанием
    bool setPowerMode(uint8_t mode);
    bool enableLowPowerMode(bool enable);
    
    // Управление аудио
    bool setVolume(uint8_t level); // 0-30
    bool volumeUp();
    bool volumeDown();
    bool mute(bool enable);
    
    // Управление воспроизведением
    bool play();
    bool pause();
    bool nextTrack();
    bool prevTrack();
    bool playTrack(uint16_t index);
    bool playFolder(const char* path);
    bool setPlayMode(uint8_t mode);
    
    // Статус медиа
    bool isPlaying();
    String getCurrentTrackName();
    uint16_t getTrackPosition();
    uint16_t getTrackDuration();
    
    // Bluetooth управление
    bool setBluetoothName(const char* name);
    bool setBluetoothPIN(const char* pin);
    bool connectDevice();
    bool disconnect();
    bool answerCall();
    bool endCall();
    bool redial();
    
    // BLE функции
    bool setBLEUUID(const char* service, const char* char1, const char* char2, const char* char3);
    bool sendBLEData(const uint8_t* data, size_t length);
    
    // SPP функции
    bool sendSPPData(const char* data);
    
    // Коллбэки
    void setTrackCallback(void (*cb)(uint16_t index, void* userData), void* userData = nullptr);
    void setCallCallback(void (*cb)(bool incoming, void* userData), void* userData = nullptr);
    void setConnectionCallback(void (*cb)(bool connected, void* userData), void* userData = nullptr);
    
    // Обработка данных
    void processIncoming();

private:
    SoftwareSerial _serial;
    char _responseBuffer[BT201_RESPONSE_BUFFER_SIZE];
    String _commandQueue[BT201_COMMAND_QUEUE_SIZE];
    uint8_t _commandQueueHead = 0;
    uint8_t _commandQueueTail = 0;
    
    // Коллбэки
    void (*_trackCb)(uint16_t, void*);
    void* _trackUserData;
    void (*_callCb)(bool, void*);
    void* _callUserData;
    void (*_connCb)(bool, void*);
    void* _connUserData;
    
    // Вспомогательные методы
    bool sendCommand(const char* cmd);
    bool sendCommand(const String& cmd); // Перегруженный метод
    bool readResponse(char* buffer, size_t size, unsigned long timeout = BT201_DEFAULT_TIMEOUT);
    bool validateResponse(const char* response);
    void handleEvent(const char* event);
    bool enqueueCommand(const char* cmd);
    bool dequeueCommand();
};
#endif