# AI Chatbot C++

Prosty chatbot AI w C++, który:
- Wyszukuje wiedzę z Wikipedii (3–6 zdań),
- Posiada pamięć długoterminową (do pliku `memory.txt`),
- Zapamiętuje kontekst rozmowy (`context.txt`),
- Działa lokalnie w terminalu.

## Uruchomienie

Wymagania: `g++`, `libcurl`

```bash
g++ main.cpp -o chatbot -lcurl
./chatbot
