// libs.cpp
#include "libs.h"

string generarIdAleatorio(int longitud) {
    const string caracteres = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    mt19937 generador(static_cast<unsigned int>(time(0)));
    uniform_int_distribution<> distribucion(0, caracteres.size() - 1);
    string idAleatorio;
    for (int i = 0; i < longitud; ++i) {
        idAleatorio += caracteres[distribucion(generador)];
    }
    return idAleatorio;
}