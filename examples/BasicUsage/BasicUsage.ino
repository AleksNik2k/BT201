#include <BT201.h>

BT201 bt(10, 11); // RX, TX

void setup() {
    Serial.begin(115200);
    bt.begin(115200);
    
    bt.setVolume(20);
    bt.setPrompt(false); // Disable prompts
    bt.setMode(1); // Bluetooth mode
}

void loop() {
    bt.processIncoming();
    
    if (Serial.available()) {
        char cmd = Serial.read();
        switch (cmd) {
            case 'p': bt.play(); break;
            case 's': bt.pause(); break;
            case 't': bt.stop(); break;
            case 'n': bt.nextTrack(); break;
            case 'b': bt.prevTrack(); break;
            case '+': bt.volumeUp(); break;
            case '-': bt.volumeDown(); break;
            case 'f': bt.fastForward(); break;
            case 'r': bt.fastBackward(); break;
            case 'm': bt.setMode(2); break; // U-disk mode
            case 'c': bt.setMode(3); break; // TF-card mode
            case 'q': Serial.println(bt.getCurrentTrackName()); break;
            case 'v': Serial.println(bt.getVolume()); break;
        }
    }
}