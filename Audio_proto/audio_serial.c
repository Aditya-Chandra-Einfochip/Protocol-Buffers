#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "audio_system.pb-c.h"

void serialize_audio_system(const char *filename) {
    // Initialize messages
    AudioSystem audio_system = AUDIO_SYSTEM__INIT;

    ConnectionDetails connection = CONNECTION_DETAILS__INIT;
    connection.device_id = 101;
    connection.device_name = strdup("AudioDeviceX");
    connection.is_connected = 1;
    connection.has_ip_address = 1;
    connection.ip_address = 19216811;

    AudioTrack track1 = AUDIO_TRACK__INIT;
    track1.track_id = 1;
    track1.track_name = strdup("Song A");
    track1.duration = 3.5;
    track1.sample_rate = 44100;

    AudioControl control = AUDIO_CONTROL__INIT;
    control.volume_level = 75;
    control.has_is_muted = 1;
    control.is_muted = 0;
    uint8_t eq_bitmap[8] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};
    control.equalizer_bitmap.len = 8;
    control.equalizer_bitmap.data = eq_bitmap;

    DeviceInfo device_info = DEVICE_INFO__INIT;
    device_info.device_id = 101;
    device_info.manufacturer = strdup("AudioTech");
    device_info.n_supported_formats = 2;
    char *formats[] = {"MP3", "WAV"};
    device_info.supported_formats = formats;

    // Assign data to audio_system
    audio_system.connection = &connection;
    audio_system.n_playlist = 1;
    audio_system.playlist = malloc(sizeof(AudioTrack *));
    audio_system.playlist[0] = &track1;
    audio_system.control = &control;
    audio_system.device_info = &device_info;

    // Serialize
    size_t size = audio_system__get_packed_size(&audio_system);
    uint8_t *buffer = malloc(size);
    audio_system__pack(&audio_system, buffer);

    // Write to file
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file");
        free(buffer);
        return;
    }
    fwrite(buffer, size, 1, file);
    fclose(file);

    // Cleanup
    free(buffer);
    free(audio_system.playlist);
    free(connection.device_name);
    free(track1.track_name);
    free(device_info.manufacturer);

    printf("Serialized data saved to %s\n", filename);
}

int main() {
    serialize_audio_system("audio_data.bin");
    return 0;
}
