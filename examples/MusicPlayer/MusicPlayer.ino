#include <BT201.h>

BT201 bt(10, 11); // RX, TX

void onTrackChange(uint16_t index, void* userData) {
    Serial.print("Now playing: ");
    Serial.println(bt.getCurrentTrackName());
}

void setup() {
    Serial.begin(115200);
    bt.begin(115200);
    
    bt.setTrackCallback(onTrackChange);
    bt.setVolume(25);
    bt.playFolder("/MUSIC/ROCK");
}

void loop() {
    bt.processIncoming();
    
    if (Serial.available()) {
        char cmd = Serial.read();
        switch (cmd) {
            case '1': bt.playFolder("/MUSIC/ROCK"); break;
            case '2': bt.playFolder("/MUSIC/POP"); break;
            case '3': bt.playFolder("/MUSIC/CLASSICAL"); break;
        }
    }
}