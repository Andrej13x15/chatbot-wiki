#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <curl/curl.h>

// Bufor odpowiedzi z Wikipedii
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

// Funkcja pobierająca z Wikipedii 3-6 zdań
std::string getWikipediaSummary(const std::string& topic) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    std::string url = "https://en.wikipedia.org/api/rest_v1/page/summary/" + topic;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    size_t start = readBuffer.find("\"extract\":\"");
    if (start != std::string::npos) {
        start += 10;
        size_t end = readBuffer.find("\",", start);
        if (end != std::string::npos) {
            return readBuffer.substr(start, end - start);
        }
    }
    return "Nie znaleziono informacji.";
}

// Zapis do pamięci długoterminowej
void saveToMemory(const std::string& userInput, const std::string& response) {
    std::ofstream file("memory.txt", std::ios::app);
    file << "User: " << userInput << "\nBot: " << response << "\n";
    file.close();
}

// Zapis do pamięci kontekstowej (ostatnie 10 linii)
void saveContext(const std::string& userInput, const std::string& response) {
    std::ofstream file("context.txt", std::ios::app);
    file << "User: " << userInput << "\nBot: " << response << "\n";
    file.close();
}

int main() {
    std::string input;
    std::cout << "Witaj! Zapytaj mnie o coś (np. Einstein, Python, Polska):\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "exit") break;

        std::string formattedTopic = input;
        for (char& c : formattedTopic) {
            if (c == ' ') c = '_';
        }

        std::string response = getWikipediaSummary(formattedTopic);

        std::cout << "[AI] " << response << "\n";

        saveToMemory(input, response);
        saveContext(input, response);
    }

    return 0;
}
