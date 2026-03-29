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
    BT201(uint8_t rxPin, uint8_t txPin);
    
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
    bool setDAC(bool enable);
    bool setEQ(uint8_t eq);
    
    // Управление воспроизведением
    bool play();
    bool pause();
    bool stop();
    bool nextTrack();
    bool prevTrack();
    bool playTrack(uint16_t index);
    bool playFolder(const char* path);
    bool setPlayMode(uint8_t mode);
    bool fastForward();
    bool fastBackward();
    bool nextFolder();
    bool prevFolder();
    bool deleteCurrentFile();
    
    // Статус медиа
    bool isPlaying();
    String getCurrentTrackName();
    uint16_t getTrackPosition();
    uint16_t getTrackDuration();
    uint16_t getCurrentTrackIndex();
    uint16_t getTotalTracks();
    uint8_t getCurrentDevice();
    uint8_t getOnlineDevices();
    
    // Bluetooth управление
    bool setBluetoothName(const char* name);
    bool setBluetoothPIN(const char* pin);
    bool setBLEName(const char* name);
    bool setBLEPIN(const char* pin);
    bool setMACAddress(const char* mac);
    bool connectDevice();
    bool disconnect();
    bool answerCall();
    bool endCall();
    bool redial();
    bool dialNumber(const char* number);
    bool setBluetoothAudio(bool enable);
    bool setBluetoothCalls(bool enable);
    bool setEDR(bool enable);
    bool setBLE(bool enable);
    bool setSimplePassword(bool enable);
    
    // BLE функции
    bool setBLEUUID(const char* service, const char* char1, const char* char2, const char* char3);
    bool sendBLEData(const uint8_t* data, size_t length);
    
    // SPP функции
    bool sendSPPData(const char* data);
    
    // Режимы и настройки
    bool setMode(uint8_t mode);
    bool setPowerOnMode(uint8_t mode);
    bool setAutoReturn(bool enable);
    bool setSingleTriggerPlay(bool enable);
    bool setRecording(bool enable);
    bool setPrompt(bool enable);
    bool setAutoSwitchToBluetooth(bool enable);
    bool setBluetoothBackground(bool enable);
    bool setKeyFunction(bool enable);
    bool setActiveReturn(bool enable);
    bool setBluetoothCallback(bool enable);
    
    // Запись
    bool startRecording();
    bool pauseRecording();
    bool stopRecording();
    bool playRecording();
    bool deleteRecording();
    bool setRecordingPath(const char* path);
    bool setRecordingBitrate(uint8_t bitrate);
    bool setMICGain(uint8_t gain);
    
    // Файловые операции
    bool readFile(const char* path);
    bool deleteFile(const char* path);
    
    // Запросы
    uint8_t getVolume();
    uint8_t getBaudRateIndex();
    uint8_t getMode();
    bool getPromptEnabled();
    bool getAutoSwitchEnabled();
    bool getBluetoothBackgroundEnabled();
    bool getKeyFunctionEnabled();
    String getBluetoothPassword();
    String getBluetoothNameEDR();
    String getBluetoothAddressEDR();
    String getBluetoothNameBLE();
    String getBluetoothAddressBLE();
    bool getIOSConnect();
    uint8_t getBluetoothStatus();
    String getCallNumber();
    bool getSimplePasswordEnabled();
    bool getCallsEnabled();
    bool getAudioEnabled();
    bool getBLEEnabled();
    bool getEDREnabled();
    String getServiceUUID();
    String getCharUUID(uint8_t index);
    
    // Коллбэки
    void setTrackCallback(void (*cb)(uint16_t index, void* userData), void* userData = nullptr);
    void setCallCallback(void (*cb)(const char* number, void* userData), void* userData = nullptr);
    void setConnectionCallback(void (*cb)(bool connected, void* userData), void* userData = nullptr);
    void setDeviceCallback(void (*cb)(uint8_t event, void* userData), void* userData = nullptr);
    
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
    void (*_callCb)(const char*, void*);
    void* _callUserData;
    void (*_connCb)(bool, void*);
    void* _connUserData;
    void (*_deviceCb)(uint8_t, void*);
    void* _deviceUserData;
    
    // Вспомогательные методы
    bool sendCommand(const char* cmd);
    bool sendCommand(const String& cmd);
    bool readResponse(char* buffer, size_t size, unsigned long timeout = BT201_DEFAULT_TIMEOUT);
    bool validateResponse(const char* response);
    void handleEvent(const char* event);
    bool enqueueCommand(const char* cmd);
    bool dequeueCommand();
    String queryCommand(const char* cmd);
};
#endif