#include <stdio.h>
#include <stdint.h>
#include "unistd.h"
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Constants
#define HEADER_BYTE 0x55
#define END_BYTE 0xAA
#define UART_DEVICE_NAME "/dev/ttyS0"
#define BAUD_RATE 115200

// Helper functions prototypes
int uart_open(const char* device_name, int baud_rate);
int uart_write(uint8_t* data, int data_size);
int uart_read(uint8_t* data, int data_size);
int uart_close();
void register_status_callback(void (*callback)(int connected, int actived));
unsigned int brightness, contrast;
// Structure to represent data packet from module to screen
typedef struct {
    uint8_t header;    // Start byte indicating the beginning of a command
    uint8_t connected; // Number of connected devices
    uint16_t reserved; // Reserved field
    uint8_t actived;   // Is there a device that is casting
    uint8_t end;       // End byte indicating the end of a command
} ModuleToScreenPacket;

// Structure to represent data packet from screen to module
typedef struct {
    uint8_t header;     // Start byte indicating the beginning of a response
    uint8_t brightness; // Brightness of the screen
    uint8_t contrast;   // Contrast of the screen
    uint8_t end;        // End byte indicating the end of a response
} ScreenToModulePacket;

// Function to send status update to the screen
void send_status_update() {
    ModuleToScreenPacket packet;
    packet.header = HEADER_BYTE;
    packet.connected = 5;  // Number of connected devices (example value)
    packet.reserved = 0;   // Reserved field (not used)
    packet.actived = 1;    // Casting is active (example value)
    packet.end = END_BYTE;

    // Convert packet to byte array
    uint8_t data[sizeof(ModuleToScreenPacket)];
    memcpy(data, &packet, sizeof(ModuleToScreenPacket));

    // Write data to UART
    uart_write(data, sizeof(data));
}

// Function to receive configuration from the screen
void receive_configuration() {
    ScreenToModulePacket packet;

    // Read data from UART
    uint8_t data[sizeof(ScreenToModulePacket)];
    uart_read(data, sizeof(data));

    // Parse the received data directly into the structure
    packet.header = data[0];
    packet.brightness = data[1];
    packet.contrast = data[2];
    packet.end = data[3];

    // Print brightness and contrast values
    printf("HEADER: %02x\n", packet.header);
    printf("Brightness: %02x\n", packet.brightness);
    printf("Contrast: %02x\n", packet.contrast);
    printf("END: %02x\n", packet.end);
}

int main(int argc, char* argv[]) {

    int i=0;
    printf("argc = %d\n", argc);
    for(i=0; i<argc; ++i)
       printf("argv[%d] = %s\n", i, argv[i]);
    
    
    sscanf_s(argv[1], "%02x", &brightness);
    sscanf_s(argv[2], "%02x", &contrast);

    // Open UART communication  
    if (uart_open(UART_DEVICE_NAME, BAUD_RATE) == -1) {
        printf("Error: Unable to open UART communication\n");
        return -1;
    }

    // Send status update every second
    while (1) {
        send_status_update();
        register_status_callback(send_status_update);
        receive_configuration();
        Sleep(1000); // Update status every second
    }

    // Close UART communication
    uart_close();

    return 0;
}

int uart_open(const char* device_name, int baud_rate)
{
    printf("UART communication opened successfully.\n");
    return 0;
}

int uart_write(uint8_t* data, int data_size)
{
    // Print the bytes being written to UART
    printf("\n");
    printf("UART write successfully.\n");
    printf("Header: %02x\n", data[0]);
    printf("Connected: %d\n", data[1]);
    printf("Reserved: %d\n", (data[2] << 8) | data[3]); // Assuming reserved is 16 bits
    printf("Actived: %d\n", data[4]);
    printf("End: %02x\n", data[5]);
 
    return 0;
}

int uart_read(uint8_t* data, int data_size)
{
    printf("\n");
    printf("UART read successfully.\n");
    // Simulate reading data
    // This part should be replaced with actual UART reading code
    uint8_t received_data[] = { 0x55, brightness, contrast, 0xAA }; // Example data
    memcpy(data, received_data, sizeof(received_data));
    return 0;
}

int uart_close()
{
    printf("\n");
    printf("UART closed successfully.\n");
    return 0;
}

void register_status_callback(void (*callback)(int connected, int actived)) {
    // This function should register the callback function to get the latest status
    printf("Status callback registered successfully.\n");
    printf("\n");
    printf("\n");
}