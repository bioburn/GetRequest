// GetRequest.c : This file contains the 'main' function. Program execution begins and ends there.
//

#define CURL_STATICLIB
#include <stdio.h>
#include "curl/curl.h" //for http requests
#include <conio.h> //for keyboard input without blocking
#include <time.h> //for elapsed time
#include <string.h> //for the substring check


#ifdef _DEBUG
#pragma comment (lib, "curl/libcurl_a_debug.lib")
#else
#pragma comment (lib, "curl/libcurl_a.lib")
#endif

#pragma comment (lib, "Normaliz.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Wldap32.lib")
#pragma comment (lib, "Crypt32.lib")
#pragma comment (lib, "advapi32.lib")


struct memory {
    char* response;
    size_t size;
};

//boilerplate callback implementation from curl
static size_t cb(void* data, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;
    struct memory* mem = (struct memory*)userp;

    char* ptr = realloc(mem->response, mem->size + realsize + 1);
    if (ptr == NULL)
        return 0;  /* out of memory! */

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}

struct memory chunk = { 0 };

typedef enum { F, T } bool;
//https://previews.123rf.com/images/fordzolo/fordzolo1506/fordzolo150600296/41026708-example-white-stamp-text-on-red-backgroud.jpg

// main function -
// where the execution of program begins
int main(int argc, char* argv[])
{
    //If program wasn't supplied a url, we use example.com
    //Specifications may require us to supply one, but here I'll just default it until I get clarification on the specs
    char* string;
    if (argc > 1) {
        
        string = argv[1];
    }
    else string = "www.example.com";

    const char SPACE = ' ';
    const char PLUS = '+';
    const char MINUS = '-';
        

    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    int bytes_read;
    

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, string);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        
       
        //Here we check if the content type is text or not, if so we set our text flag to true
        boolean text = F;
        char* ct = NULL;
        res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
        if (!res && ct && (strstr(ct, "text") != NULL || strstr(ct, "UTF-8") != NULL || strstr(ct, "UTF-16") != NULL)) {
            //if content type is text, print normally
            
            text = T;
        }
        //debug
        //printf("Content-Type: %s\n", ct);
       
        //setting up our delta time for printing out X lines per second
        int lineCount = 1;
        float linesPerSecond = 1.0f;
        bool pause = F;

        float timePerLine = 1.0f / linesPerSecond;

        clock_t t;
        t = clock();
        
        //the printing and user input loop
        //program exits when done printing
        int i = 0;
        while ((text==T && chunk.response[i] != '\0') || (text==F && i < chunk.size))
        {
            
            //take user input in this area
            //if user hits + or - increase or decrease lines per second
            if (_kbhit()) {
                char input = _getch();

                if (input == SPACE) {
                    if (pause == T) {
                        pause = F;
                    }
                    else pause = T;
                }
                if (input == PLUS) {
                    linesPerSecond++;
                    timePerLine = 1.0f / linesPerSecond;
                    

                }
                if (input == MINUS) {
                    linesPerSecond--;
                    if (linesPerSecond < 1.0f) {
                        linesPerSecond = 1.0f;
                    }
                    timePerLine = 1.0f / linesPerSecond;
                    

                }
                //if + or - increase or decrease linesPerSecond
            }

            //if user had hit space bar to pause, don't print
            if (pause == T) {
                continue;
            }



            //Here we do the printing
            // 
            //take time now - previoustime = elapsed time
            //if elapsed time < timeperline (seconds per line = 1 second / number of lines per second
            //wait
            //if time to print, printline
            
            time_t now = time(NULL);
            float deltaTime = ((float)(clock() - (float)t) / CLOCKS_PER_SEC);
            if ( deltaTime >= timePerLine) {

                //if our resource was a text
                if (text == T) {
                    printf("%d", lineCount);

                    //This builds the line in the current iteration
                    while (chunk.response[i] != '\n') {
                        putchar(chunk.response[i]);
                        i++;
                    }
                    //classic trailing space/newline problem
                    putchar('\n');

                    lineCount++;
                    i++;
                    t = clock();
                }

                //if it wasn't a text
                else if (text == F) {
                    
                    while ((i + 1) % 16 != 0) {
                        printf("%02x", lineCount);
                        printf("%02x ", chunk.response[i]);
                        i++;
                    }
                    printf("%02x", lineCount);
                    printf("%02x ", chunk.response[i]);
                    putchar('\n');

                    lineCount++;
                    i++;
                    t = clock();
                    
                }
            }
            
            
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
