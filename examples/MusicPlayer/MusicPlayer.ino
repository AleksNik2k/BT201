#include <BT201.h>

BT201 bt(10, 11); // RX, TX

void onTrackChange(uint16_t index, void* userData) {
    Serial.print("Now playing: ");
    Serial.println(bt.getCurrentTrackName());
    Serial.print("Track index: ");
    Serial.println(index);
    Serial.print("Total tracks: ");
    Serial.println(bt.getTotalTracks());
}

void setup() {
    Serial.begin(115200);
    bt.begin(115200);
    
    bt.setTrackCallback(onTrackChange);
    bt.setVolume(25);
    bt.setPlayMode(1); // Single loop
    bt.setMode(2); // U-disk mode
}

void loop() {
    bt.processIncoming();
    
    if (Serial.available()) {
        char cmd = Serial.read();
        switch (cmd) {
            case '1': bt.playFolder("/MUSIC/ROCK/*???"); break;
            case '2': bt.playFolder("/MUSIC/POP/*???"); break;
            case '3': bt.playFolder("/MUSIC/CLASSICAL/*???"); break;
            case 'p': bt.playTrack(1); break;
            case 'r': bt.startRecording(); break;
            case 's': bt.stopRecording(); break;
            case 'd': bt.deleteFile("/MUSIC/ROCK/001*???"); break;
            case 'm': Serial.println(bt.getMode()); break;
            case 't': Serial.println(bt.getCurrentTrackIndex()); break;
        }
    }
}