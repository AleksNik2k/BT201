#include <BT201.h>

BT201 bt(10, 11); // RX, TX

void setup() {
    Serial.begin(115200);
    bt.begin(115200);
    
    bt.setRecording(true); // Enable recording
    bt.setRecordingBitrate(5); // 128kbps
    bt.setMICGain(35); // Default gain
    Serial.println("Recording demo ready. Press 'r' to start recording, 's' to stop, 'p' to play.");
}

void loop() {
    bt.processIncoming();
    
    if (Serial.available()) {
        char cmd = Serial.read();
        switch (cmd) {
            case 'r': 
                bt.setRecordingPath("/KT_REC/TEST.MP3");
                bt.startRecording(); 
                Serial.println("Recording started...");
                break;
            case 's': 
                bt.stopRecording(); 
                Serial.println("Recording stopped.");
                break;
            case 'p': 
                bt.playRecording(); 
                Serial.println("Playing recording...");
                break;
            case 'd': 
                bt.deleteRecording(); 
                Serial.println("Recording deleted.");
                break;
        }
    }
}