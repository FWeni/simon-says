#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>  

#include <unistd.h>
#include <arpa/inet.h>
#include <asm-generic/socket.h>

#include <cjson/cJSON.h>

#define PORT 8080
#define BUFFER_SIZE 1024

/* Generates the colors that will be served to the client */ 

char** randomColorSelection(char *arr[], int arrSize, int numOfItems) {
    srand(time(NULL));

    char **selectedColors = malloc(numOfItems * sizeof(char*));
    for (int i = 0; i < numOfItems; i++)
    {
        int randInd = rand() % arrSize;
        selectedColors[i] = arr[randInd];
    }
    // Print the selected items
    // printf("Selected items:\n");
    // for (int i = 0; i < numOfItems; i++) {
    //     printf("%s\n", selectedColors[i]);
    // }
    return selectedColors;
}

// cJSON* simonResp(int level, char **colorCommands, int items) {
//     cJSON *json = cJSON_CreateObject();

//     if (json == NULL)
//     {
//         printf("Error creating a json object.");
//         return NULL;
//     }

//     cJSON_AddNumberToObject(json, "level", level);
    
//     cJSON *commands = cJSON_CreateArray();
//     if (commands == NULL) {
//         cJSON_Delete(json);
//         return NULL;
//     }

//     for (int i = 0; i < items; i++) {
//         cJSON *command = cJSON_CreateString(colorCommands[i]);
//         if (command == NULL) {
//             cJSON_Delete(json);
//             return NULL;
//         }
//         cJSON_AddItemToArray(commands, command);
//     }
//     cJSON_AddItemToObject(json, "commands", commands);
    
//     return json;
// }

void freeColorCommands(char **colorCommands, int numOfItems) {
    free(colorCommands);
}

void handle_client( int new_socket )
{
    const char* response_header = "HTTP/1.1 200 OK\r\n"
                                  "Content-Type: application/json\r\n"
                                  "Access-Control-Allow-Origin: *\r\n"
                                  "Connection: close\r\n\r\n";

    char *colorsArr[] = {"Red", "Green", "Blue", "Yellow"};
    int arrSize = sizeof(colorsArr) / sizeof(colorsArr[0]);
    int numOfItems = 4;

    cJSON *json = cJSON_CreateObject();
    if (json == NULL) {
        perror("Failed to create JSON object");
        return;
    }

    cJSON_AddStringToObject(json, "message", "Hello, World!");

    char *json_str = cJSON_Print(json);
    if (json_str == NULL) {
        perror("Failed to print JSON");
        cJSON_Delete(json);
        return;
    }

    // Send the response header
    send(new_socket, response_header, strlen(response_header), 0);

    // Send the JSON response
    send(new_socket, json_str, strlen(json_str), 0);

    free(json_str);
    cJSON_Delete(json);
    close(new_socket);
}

void server() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    
    if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd,3) < 0)
    {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    while (1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("Accept failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        handle_client(new_socket);
        
    }
    
    
    
}

int main() {
    server();
    
    
    // int level = 1;
    
    // Create the random selection of colors
    // char **selectedColors = randomColorSelection(colorsArr, arrSize, numOfItems);

    // printf("Selected items:\n");
    // for (int i = 0; i < numOfItems; i++) {
    //     printf("%s\n", selectedColors[i]);
    // }
    
    // Create the JSON response
    // cJSON *jsonResponse = simonResp(level, selectedColors, numOfItems);
    
    // Print the JSON response
    // if (jsonResponse != NULL) {
    //     char *jsonStr = cJSON_Print(jsonResponse);
    //     if (jsonStr != NULL) {
    //         printf("%s\n", jsonStr);
    //         free(jsonStr);
    //     }
    //     cJSON_Delete(jsonResponse);
    // }

    // freeColorCommands(selectedColors, numOfItems);
    
    return 0;
}