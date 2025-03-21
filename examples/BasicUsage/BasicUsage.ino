#include <BT201.h>

BT201 bt(10, 11); // RX, TX

void setup() {
    Serial.begin(115200);
    bt.begin(115200);
    
    bt.setVolume(20);
    bt.playFolder("/MUSIC");
}

void loop() {
    bt.processIncoming();
    
    if (Serial.available()) {
        char cmd = Serial.read();
        switch (cmd) {
            case 'p': bt.play(); break;
            case 's': bt.pause(); break;
            case 'n': bt.nextTrack(); break;
            case 'b': bt.prevTrack(); break;
            case '+': bt.volumeUp(); break;
            case '-': bt.volumeDown(); break;
        }
    }
}