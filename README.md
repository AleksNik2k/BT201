# BT201 Arduino Library

[![Version](https://img.shields.io/badge/version-2.0.0-blue.svg)](https://github.com/AleksNik2k/BT201)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A comprehensive Arduino library for the BT201 Bluetooth audio module based on KT1025A/B chip. This library provides full control over audio playback, Bluetooth connectivity, BLE/SPP data transmission, recording capabilities, and file operations.

## Features

### 🎵 Audio Control
- Volume control (0-30 levels)
- Playback control: play, pause, stop, next/previous track
- Fast forward/backward, folder navigation
- Equalizer settings (limited support)
- DAC enable/disable

### 📱 Bluetooth Management
- Bluetooth Classic (EDR) and BLE support
- Device naming and PIN configuration
- Phone call control: dial, answer, end, redial
- Connection status monitoring
- MAC address configuration

### 🔄 Data Transmission
- BLE with custom UUID configuration
- SPP (Serial Port Profile) data transmission
- Transparent data mode

### 🎙️ Recording Features
- Audio recording with configurable bitrate
- Microphone gain control
- Recording to TF-card or U-disk
- Playback of recorded files

### 📁 File Operations
- Read file contents via serial port
- Delete files and folders
- Folder-based playback with wildcards

### ⚙️ System Control
- Multiple operation modes (Bluetooth, U-disk, TF-card, etc.)
- Baud rate configuration
- Power management and low-power mode
- Factory reset and firmware version query

### 📊 Status Queries
- Real-time status of all module parameters
- Track information and progress
- Connection and device status

### 🔔 Event Handling
- Callbacks for track changes, incoming calls, connections
- Device insertion/removal notifications
- Error handling and status updates

## Hardware Requirements

- **Arduino Board**: Any Arduino-compatible board with UART
- **BT201 Module**: Based on KT1025A/B chip
- **Connections**:
  - BT201 RX → Arduino TX pin
  - BT201 TX → Arduino RX pin
  - Power: 3.3V-5V
- **Storage**: TF-card or U-disk for music/recording (optional)

## Installation

### Method 1: Arduino IDE
1. Download the library as a ZIP file from [GitHub releases](https://github.com/AleksNik2k/BT201/releases)
2. Open Arduino IDE
3. Go to `Sketch > Include Library > Add .ZIP Library`
4. Select the downloaded ZIP file
5. Restart Arduino IDE

### Method 2: Manual Installation
1. Download or clone this repository
2. Copy the `BT201` folder to your Arduino libraries directory:
   - Windows: `Documents\Arduino\libraries\`
   - macOS: `~/Documents/Arduino/libraries/`
   - Linux: `~/Arduino/libraries/`
3. Restart Arduino IDE

## Quick Start

```cpp
#include <BT201.h>

BT201 bt(10, 11); // RX, TX pins

void setup() {
    Serial.begin(115200);
    bt.begin(115200); // Initialize with baud rate
    
    bt.setVolume(20);
    bt.setBluetoothName("MyBT201");
    bt.setMode(1); // Bluetooth mode
}

void loop() {
    bt.processIncoming(); // Handle module responses
    
    // Your code here
}
```

## API Reference

### Initialization
```cpp
BT201 bt(rxPin, txPin);           // Constructor
bool begin(unsigned long baud);   // Initialize module
```

### Audio Control
```cpp
bool setVolume(uint8_t level);     // 0-30
bool volumeUp();
bool volumeDown();
bool mute(bool enable);
bool setDAC(bool enable);          // Enable/disable DAC
bool setEQ(uint8_t eq);            // 0-5 (limited support)

bool play();
bool pause();
bool stop();
bool nextTrack();
bool prevTrack();
bool fastForward();
bool fastBackward();
bool nextFolder();
bool prevFolder();
bool deleteCurrentFile();
```

### Playback Control
```cpp
bool playTrack(uint16_t index);    // Play specific track by index
bool playFolder(const char* path); // Play folder with pattern (e.g., "/MUSIC/*???")
bool setPlayMode(uint8_t mode);    // 0: stop, 1: single loop, 2: all loop, 3: random, 4: folder loop
```

### Bluetooth Control
```cpp
// Naming and Security
bool setBluetoothName(const char* name);    // EDR name
bool setBluetoothPIN(const char* pin);      // EDR PIN
bool setBLEName(const char* name);          // BLE name
bool setBLEPIN(const char* pin);            // BLE PIN
bool setMACAddress(const char* mac);        // Set MAC address

// Calls
bool dialNumber(const char* number);
bool answerCall();
bool endCall();
bool redial();

// Settings
bool setBluetoothAudio(bool enable);
bool setBluetoothCalls(bool enable);
bool setEDR(bool enable);
bool setBLE(bool enable);
bool setSimplePassword(bool enable);
```

### BLE & SPP
```cpp
bool setBLEUUID(const char* service, const char* char1, const char* char2, const char* char3);
bool sendBLEData(const uint8_t* data, size_t length);
bool sendSPPData(const char* data);
```

### Recording
```cpp
bool setRecording(bool enable);             // Enable recording feature
bool startRecording();
bool pauseRecording();
bool stopRecording();
bool playRecording();
bool deleteRecording();
bool setRecordingPath(const char* path);    // e.g., "/KT_REC/TEST.MP3"
bool setRecordingBitrate(uint8_t bitrate);  // 0-8 (16kbps to 160kbps)
bool setMICGain(uint8_t gain);              // 0-63
```

### File Operations
```cpp
bool readFile(const char* path);   // Read file content to serial
bool deleteFile(const char* path);
```

### System Settings
```cpp
bool setMode(uint8_t mode);        // 0: idle, 1: BT, 2: U-disk, 3: TF-card, etc.
bool setBaudRate(unsigned long baud); // 9600, 19200, 38400, 57600, 115200
bool setPowerMode(uint8_t mode);
bool enableLowPowerMode(bool enable);
bool reset();
bool factoryReset();
String getFirmwareVersion();

// Advanced settings
bool setPowerOnMode(uint8_t mode);
bool setAutoReturn(bool enable);
bool setSingleTriggerPlay(bool enable);
bool setPrompt(bool enable);
bool setAutoSwitchToBluetooth(bool enable);
bool setBluetoothBackground(bool enable);
bool setKeyFunction(bool enable);
bool setActiveReturn(bool enable);
bool setBluetoothCallback(bool enable);
```

### Status Queries
```cpp
// Audio status
uint8_t getVolume();
bool isPlaying();
String getCurrentTrackName();
uint16_t getTrackPosition();
uint16_t getTrackDuration();
uint16_t getCurrentTrackIndex();
uint16_t getTotalTracks();
uint8_t getCurrentDevice();
uint8_t getOnlineDevices();

// System status
uint8_t getMode();
uint8_t getBaudRateIndex();
bool getPromptEnabled();
bool getAutoSwitchEnabled();
bool getBluetoothBackgroundEnabled();
bool getKeyFunctionEnabled();

// Bluetooth status
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
String getCharUUID(uint8_t index); // 0-2 for char1-char3
```

### Event Callbacks
```cpp
void setTrackCallback(void (*cb)(uint16_t index, void* userData), void* userData = nullptr);
void setCallCallback(void (*cb)(const char* number, void* userData), void* userData = nullptr);
void setConnectionCallback(void (*cb)(bool connected, void* userData), void* userData = nullptr);
void setDeviceCallback(void (*cb)(uint8_t event, void* userData), void* userData = nullptr);

// Event codes for device callback:
// 1: U-disk inserted, 2: U-disk removed
// 3: TF-card inserted, 4: TF-card removed
// 5: PC connected, 6: PC disconnected
```

### Processing
```cpp
void processIncoming(); // Must be called in loop() to handle responses
```

## Examples

The library includes several examples demonstrating different features:

### Basic Usage
```cpp
#include <BT201.h>

BT201 bt(10, 11);

void setup() {
    Serial.begin(115200);
    bt.begin(115200);
    bt.setVolume(20);
}

void loop() {
    bt.processIncoming();
    
    if (Serial.available()) {
        char cmd = Serial.read();
        switch (cmd) {
            case 'p': bt.play(); break;
            case 's': bt.pause(); break;
            case 'n': bt.nextTrack(); break;
            case '+': bt.volumeUp(); break;
        }
    }
}
```

### Bluetooth Control
```cpp
#include <BT201.h>

BT201 bt(10, 11);

void onCall(const char* number, void* userData) {
    Serial.print("Call from: ");
    Serial.println(number);
    bt.answerCall();
}

void setup() {
    Serial.begin(115200);
    bt.begin(115200);
    bt.setBluetoothName("MyDevice");
    bt.setCallCallback(onCall);
}

void loop() {
    bt.processIncoming();
}
```

### Recording
```cpp
#include <BT201.h>

BT201 bt(10, 11);

void setup() {
    Serial.begin(115200);
    bt.begin(115200);
    bt.setRecording(true);
    bt.setRecordingBitrate(5); // 128kbps
}

void loop() {
    bt.processIncoming();
    
    if (Serial.available()) {
        char cmd = Serial.read();
        if (cmd == 'r') {
            bt.setRecordingPath("/KT_REC/test.mp3");
            bt.startRecording();
        }
    }
}
```

See the `examples/` folder for complete working sketches.

## Command Reference

| Category | Command | Description |
|----------|---------|-------------|
| Audio | AT+CAxx | Set volume |
| Audio | AT+CB | Play/pause |
| Audio | AT+CC/CD | Next/previous track |
| Bluetooth | AT+BDname | Set BT name |
| Bluetooth | AT+BAxx | BT audio control |
| BLE | AT+U0-U3 | Set BLE UUID |
| Recording | AT+RCxx | Recording control |
| System | AT+CMxx | Set mode |
| Query | AT+QA | Get volume |

For complete command list, see [BT201 Manual](BT201%20Module%20_KT1025A_B_User%20Manual%20_V2.3.pdf).

## Troubleshooting

### Common Issues

**Module not responding**
- Check wiring: RX/TX pins must be crossed
- Verify power supply (3.3V-5V)
- Check baud rate settings

**No audio playback**
- Ensure TF-card or U-disk is inserted
- Check file format (MP3, WAV, etc.)
- Verify playback mode

**Bluetooth connection fails**
- Check device visibility
- Verify PIN/password settings
- Ensure BLE mode is enabled if needed

**Recording not working**
- Enable recording with `setRecording(true)`
- Insert storage device (TF-card/U-disk)
- Check available space

### Error Codes
- ER+1: Invalid data frame
- ER+2: Unknown command
- ER+3: Device not online
- ER+4: Invalid parameters
- ER+5: Device abnormal
- ER+6: Path not found

### Debug Tips
- Use `Serial.println(bt.getMode())` to check current mode
- Monitor serial output for module responses
- Test with simple commands first

## Compatibility

- **Arduino IDE**: 1.8.x and later
- **Boards**: Any with UART (Uno, Mega, ESP32, etc.)
- **Module**: BT201 with KT1025A/B chip
- **Firmware**: Compatible with V2.3 and later

## Contributing

Contributions are welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Add tests for new features
4. Submit a pull request

## License

This library is released under the MIT License. See [LICENSE](LICENSE) file for details.

## Credits

- Based on BT201 Module User Manual V2.3
- Developed for Arduino ecosystem
- Compatible with KT1025A/B Bluetooth audio modules

## Version History

- **v2.0.0**: Complete rewrite with full manual support
- **v1.0.0**: Initial release with basic functionality