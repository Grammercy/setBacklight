#include <stdio.h>
#include <stdlib.h>

#define MAX_PATH_LENGTH 256

int main() {
    // Check if the program is run as root
    if (geteuid() != 0) {
        fprintf(stderr, "Please run as root (sudo).\n");
        exit(EXIT_FAILURE);
    }

    // Path to the Intel backlight control directory
    const char *BACKLIGHT_DIR = "/sys/class/backlight/intel_backlight";

    // Check if the backlight directory exists
    if (access(BACKLIGHT_DIR, F_OK) == -1) {
        fprintf(stderr, "Intel backlight directory not found: %s\n", BACKLIGHT_DIR);
        exit(EXIT_FAILURE);
    }

    // Get the current brightness value
    char brightnessFilePath[MAX_PATH_LENGTH];
    snprintf(brightnessFilePath, sizeof(brightnessFilePath), "%s/brightness", BACKLIGHT_DIR);

    FILE *brightnessFile = fopen(brightnessFilePath, "r");
    if (!brightnessFile) {
        perror("Error opening brightness file");
        exit(EXIT_FAILURE);
    }

    int currentBrightness;
    fscanf(brightnessFile, "%d", &currentBrightness);
    fclose(brightnessFile);

    // Get the maximum brightness value
    char maxBrightnessFilePath[MAX_PATH_LENGTH];
    snprintf(maxBrightnessFilePath, sizeof(maxBrightnessFilePath), "%s/max_brightness", BACKLIGHT_DIR);

    FILE *maxBrightnessFile = fopen(maxBrightnessFilePath, "r");
    if (!maxBrightnessFile) {
        perror("Error opening max_brightness file");
        exit(EXIT_FAILURE);
    }

    int maxBrightness;
    fscanf(maxBrightnessFile, "%d", &maxBrightness);
    fclose(maxBrightnessFile);

    printf("Current brightness: %d out of %d\n", currentBrightness, maxBrightness);

    // Prompt the user to set a new brightness value
    int newBrightness;
    printf("Enter the new brightness level (1-%d): ", maxBrightness);
    scanf("%d", &newBrightness);

    // Validate user input
    if (newBrightness < 1 || newBrightness > maxBrightness) {
        fprintf(stderr, "Invalid input. Please enter a value between 1 and %d.\n", maxBrightness);
        exit(EXIT_FAILURE);
    }

    // Set the brightness
    FILE *newBrightnessFile = fopen(brightnessFilePath, "w");
    if (!newBrightnessFile) {
        perror("Error opening brightness file for writing");
        exit(EXIT_FAILURE);
    }

    fprintf(newBrightnessFile, "%d", newBrightness);
    fclose(newBrightnessFile);

    printf("Brightness set to %d out of %d.\n", newBrightness, maxBrightness);

    return 0;
}
