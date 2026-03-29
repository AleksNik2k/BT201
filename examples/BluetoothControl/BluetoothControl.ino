#include <BT201.h>

BT201 bt(10, 11); // RX, TX

void onCall(const char* number, void* userData) {
    Serial.print("Incoming call from: ");
    Serial.println(number);
}

void onConnection(bool connected, void* userData) {
    Serial.println(connected ? "Connected!" : "Disconnected.");
}

void setup() {
    Serial.begin(115200);
    bt.begin(115200);
    
    bt.setBluetoothName("MyAudioSystem");
    bt.setBluetoothPIN("1234");
    bt.setBLEName("MyBLEDevice");
    bt.setBluetoothBackground(true);
    bt.setCallCallback(onCall);
    bt.setConnectionCallback(onConnection);
}

void loop() {
    bt.processIncoming();
    
    if (Serial.available()) {
        char cmd = Serial.read();
        switch (cmd) {
            case 'c': bt.connectDevice(); break;
            case 'd': bt.disconnect(); break;
            case 'a': bt.answerCall(); break;
            case 'e': bt.endCall(); break;
            case 'r': bt.redial(); break;
            case '1': bt.dialNumber("10086"); break;
            case '2': bt.setBluetoothAudio(true); break;
            case '3': bt.setBLE(true); break;
            case '4': bt.setEDR(true); break;
            case 's': Serial.println(bt.getBluetoothStatus()); break;
            case 'n': Serial.println(bt.getCallNumber()); break;
        }
    }
}