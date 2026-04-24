#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 64
#define MAX_DATA_LENGTH 32

// Thread-safe circular buffer
typedef struct {
    char data[BUFFER_SIZE];
    int head;
    int tail;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} CircularBuffer;

// UART device structure
typedef struct {
    CircularBuffer rx_buffer;
    CircularBuffer tx_buffer;
    bool tx_active;
} UARTDevice;

// Initialize a circular buffer
void buffer_init(CircularBuffer *buf) {
    buf->head = 0;
    buf->tail = 0;
    buf->count = 0;
    pthread_mutex_init(&buf->mutex, NULL);
    pthread_cond_init(&buf->not_empty, NULL);
    pthread_cond_init(&buf->not_full, NULL);
}

// Check if buffer is empty
bool buffer_empty(CircularBuffer *buf) {
    return buf->count == 0;
}

// Check if buffer is full
bool buffer_full(CircularBuffer *buf) {
    return buf->count == BUFFER_SIZE;
}

// Write to buffer (thread-safe)
void buffer_write(CircularBuffer *buf, char data) {
    pthread_mutex_lock(&buf->mutex);
    while (buffer_full(buf)) {
        pthread_cond_wait(&buf->not_full, &buf->mutex);
    }
    buf->data[buf->tail] = data;
    buf->tail = (buf->tail + 1) % BUFFER_SIZE;
    buf->count++;
    pthread_cond_signal(&buf->not_empty);
    pthread_mutex_unlock(&buf->mutex);
}

// Read from buffer (thread-safe)
char buffer_read(CircularBuffer *buf) {
    pthread_mutex_lock(&buf->mutex);
    while (buffer_empty(buf)) {
        pthread_cond_wait(&buf->not_empty, &buf->mutex);
    }
    char data = buf->data[buf->head];
    buf->head = (buf->head + 1) % BUFFER_SIZE;
    buf->count--;
    pthread_cond_signal(&buf->not_full);
    pthread_mutex_unlock(&buf->mutex);
    return data;
}

// UART device initialization
void uart_init(UARTDevice *uart) {
    buffer_init(&uart->rx_buffer);
    buffer_init(&uart->tx_buffer);
    uart->tx_active = false;
}

// Simulate UART transmission (worker thread)
void* uart_tx_thread(void *arg) {
    UARTDevice *uart = (UARTDevice *)arg;
    while (1) {
        if (!uart->tx_active) {
            sleep(1); // Simulate delay
            continue;
        }
        char data = buffer_read(&uart->tx_buffer);
        printf("TX: Sent byte: %c\n", data);
        // Simulate transmission delay
        usleep(100000); // 100ms
    }
    return NULL;
}

// Simulate UART reception (worker thread)
void* uart_rx_thread(void *arg) {
    UARTDevice *uart = (UARTDevice *)arg;
    while (1) {
        // Simulate receiving a byte (e.g., from another device)
        char received = 'A' + (rand() % 26); // Random ASCII letter
        buffer_write(&uart->rx_buffer, received);
        printf("RX: Received byte: %c\n", received);
        // Simulate reception delay
        usleep(150000); // 150ms
    }
    return NULL;
}

int main() {
    UARTDevice device1, device2;
    uart_init(&device1);
    uart_init(&device2);

    // Simulate UART connection
    device1.tx_active = true;
    device2.tx_active = true;

    // Create threads for TX and RX
    pthread_t tx_thread1, rx_thread1, tx_thread2, rx_thread2;
    pthread_create(&tx_thread1, NULL, uart_tx_thread, &device1);
    pthread_create(&rx_thread1, NULL, uart_rx_thread, &device1);
    pthread_create(&tx_thread2, NULL, uart_tx_thread, &device2);
    pthread_create(&rx_thread2, NULL, uart_rx_thread, &device2);

    // Simulate data exchange
    for (int i = 0; i < 10; i++) {
        char msg[] = "Hello UART!";
        for (int j = 0; j < strlen(msg); j++) {
            buffer_write(&device1.tx_buffer, msg[j]);
        }
        sleep(2); // Wait before sending next message
    }

    // Cleanup (in a real app, you'd join threads properly)
    device1.tx_active = false;
    device2.tx_active = false;
    printf("Simulation complete.\n");
    return 0;
}
