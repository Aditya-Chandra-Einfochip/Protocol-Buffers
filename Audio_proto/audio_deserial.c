#include <stdio.h>
#include <stdlib.h>
#include "audio_system.pb-c.h"

void deserialize_audio_system(const char *filename, int message_type) {
    // Read file
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        return;
    }
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    uint8_t *buffer = malloc(size);
    fread(buffer, size, 1, file);
    fclose(file);

    // Deserialize
    AudioSystem *audio_system = audio_system__unpack(NULL, size, buffer);
    if (!audio_system) {
        fprintf(stderr, "Error unpacking message.\n");
        free(buffer);
        return;
    }

    // Display requested message type
    switch (message_type) {
        case 1:  // Connection Details
            if (audio_system->connection) {
                printf("Connection Details:\n");
                printf("Device ID: %d\n", audio_system->connection->device_id);
                printf("Device Name: %s\n", audio_system->connection->device_name ? audio_system->connection->device_name : "N/A");
                printf("Is Connected: %d\n", audio_system->connection->is_connected);
                if (audio_system->connection->has_ip_address)
                    printf("IP Address: %u\n", audio_system->connection->ip_address);
            }
            break;

        case 2:  // Playlist
            printf("Playlist:\n");
            for (size_t i = 0; i < audio_system->n_playlist; i++) {
                printf("Track %zu: ID=%d, Name=%s, Duration=%.2f, Sample Rate=%d\n",
                       i + 1, audio_system->playlist[i]->track_id,
                       audio_system->playlist[i]->track_name ? audio_system->playlist[i]->track_name : "Unknown",
                       audio_system->playlist[i]->duration, audio_system->playlist[i]->sample_rate);
            }
            break;

        case 3:  // Audio Control
            if (audio_system->control) {
                printf("Audio Control Settings:\n");
                printf("Volume Level: %d\n", audio_system->control->volume_level);
                if (audio_system->control->has_is_muted)
                    printf("Is Muted: %d\n", audio_system->control->is_muted);

                printf("Equalizer Bitmap: ");
                for (size_t i = 0; i < audio_system->control->equalizer_bitmap.len; i++) {
                    printf("%02X ", audio_system->control->equalizer_bitmap.data[i]);
                }
                printf("\n");
            }
            break;

        case 4:  // Device Info
            if (audio_system->device_info) {
                printf("Device Info:\n");
                printf("Device ID: %d\n", audio_system->device_info->device_id);
                printf("Manufacturer: %s\n", audio_system->device_info->manufacturer ? audio_system->device_info->manufacturer : "Unknown");
                printf("Supported Formats: ");
                for (size_t i = 0; i < audio_system->device_info->n_supported_formats; i++) {
                    printf("%s ", audio_system->device_info->supported_formats[i]);
                }
                printf("\n");
            }
            break;

        default:
            printf("Invalid message type. Use:\n 1 - Connection Details\n 2 - Playlist\n 3 - Audio Control\n 4 - Device Info\n");
    }

    // Cleanup
    audio_system__free_unpacked(audio_system, NULL);
    free(buffer);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <message_type>\n", argv[0]);
        return 1;
    }
    int message_type = atoi(argv[1]);
    deserialize_audio_system("audio_data.bin", message_type);
    return 0;
}
