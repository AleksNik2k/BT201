#include <BT201.h>

BT201 bt(10, 11); // RX, TX

void onCall(bool incoming) {
    Serial.println(incoming ? "Incoming call!" : "Call ended.");
}

void onConnection(bool connected) {
    Serial.println(connected ? "Connected!" : "Disconnected.");
}

void setup() {
    Serial.begin(115200);
    bt.begin(115200);
    
    bt.setBluetoothName("MyAudioSystem");
    bt.setBluetoothPIN("1234");
    bt.setCallCallback(onCall);
    bt.setConnectionCallback(onConnection);
}

void loop() {
    bt.processIncoming();
    bt.play();
    if (bt.isPlaying()){
      bt.pause();
    }
    
    if (Serial.available()) {
        char cmd = Serial.read();
        switch (cmd) {
            case 'c': bt.connectDevice(); break;
            case 'd': bt.disconnect(); break;
            case 'a': bt.answerCall(); break;
            case 'e': bt.endCall(); break;
            case 'r': bt.redial(); break;
        }
    }
}