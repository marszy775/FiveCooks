## Temat projektu
Zaimplementowany został problem Pięciu Kucharzy.

Zadanie oparte jest na problemie ucztujących filozofów, przy czym:
  * Tutaj kucharz może albo przygotować porcję do zjedzenia, albo skonsumować porcję przygotowaną wcześniej (być może przez innego kucharza).
  * Zarówno do skonsumowania porcji, jak i do jej przygotowania kucharz potrzebuje dwóch widelców.
  * Porcje przygotowane przez kucharzy gromadzone są na stole, który ma ograniczoną pojemność — K oraz dopuszczalne obciążenie — W.
  * Każda przygotowana porcja zajmuje jedno miejsce na stole, a wagi przygotowywanych porcji są zróżnicowane.
  * Zawartość stołu jest reprezentowana przez kolejkę komunikatów.

## Implementacja Mechanizmów Synchronizacji w Programie
Program został zaimplementowany w języku C++ i wykorzystuje różne mechanizmy synchronizacji, takie jak:
* std::binary_semaphore,
* std::mutex,
* std::condition_variable,
* std::thread.

Do komunikacji między wątkami użyto kolejki komunikatów z użyciem std::vector.

## Instrukcja kompilacji oraz uruchomienia programu
Aby skompilować i uruchomić program, należy skopiować kod do środowiska programistycznego, takiego jak Visual Studio. 

W przypadku chęci zmiany pojemności stołu lub dopuszczalnego obciążenia, konieczne jest dokonanie odpowiednich modyfikacji bezpośrednio w kodzie źródłowym. (w linijkach kodu 12 oraz 13)
