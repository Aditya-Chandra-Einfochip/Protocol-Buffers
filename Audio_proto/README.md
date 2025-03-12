# Audio System 

## Overview

This project demonstrates how to use Protocol Buffers (protobuf) in C for serializing and deserializing structured audio system data. It includes different message types:

- `ConnectionDetails`: Stores device connection information.
- `AudioTrack`: Represents individual audio tracks with metadata.
- `AudioControl`: Handles audio control settings such as volume and equalizer settings.
- `DeviceInfo`: Contains manufacturer details and supported formats.
- `AudioSystem`: A composite message that includes all above structures.

## Prerequisites

To run this example, ensure you have:

- `protobuf-c` installed
- `protoc` compiler installed
- A C compiler (e.g., `gcc`)

## Generating C Code from `.proto`

Compile the `.proto` file into C source files using:

```sh
protoc --c_out=. audio_system.proto
```

This will generate `audio_system.pb-c.h` and `audio_system.pb-c.c`.

## Compilation

Compile the serialization and deserialization programs using:

```sh
gcc -o audio_serial audio_serial.c audio_system.pb-c.c -lprotobuf-c
gcc -o audio_deserial audio_deserial.c audio_system.pb-c.c -lprotobuf-c
```

## Running the Example

### 1. Serialize Data

```sh
./audio_serial
```

This creates `audio_data.bin` containing the serialized data.

### 2. Deserialize Data

To display specific messages:

```sh
./audio_deserial 1  # Connection Details
./audio_deserial 2  # Playlist
./audio_deserial 3  # Audio Control
./audio_deserial 4  # Device Info
```

## Expected Output

When deserializing, you should see output like:

```
Connection Details:
Device ID: 101
Device Name: AudioDeviceX
Is Connected: 1
IP Address: 19216811
```

Or for Audio Control:

```
Audio Control Settings:
Volume Level: 75
Is Muted: 0
Equalizer Bitmap: AA BB CC DD EE FF 11 22
```

## Error Handling

- If `audio_data.bin` is missing, deserialization will print:
  ```
  Failed to open file audio_data.bin
  ```
- If an invalid file is provided, deserialization will fail with:
  ```
  Error unpacking message.
  ```

## Notes

- Ensure the correct `.proto` file is compiled before running serialization and deserialization.
- Modify the `.proto` file as needed to extend functionality.
- The bitmap field is displayed in hexadecimal format.



