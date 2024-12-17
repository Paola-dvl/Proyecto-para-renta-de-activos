// menu.h
#ifndef MENU_H
#define MENU_H
#include "libs.h"
#include "estructuras.cpp"

class Menu {
public:
    void mostrarMenuPrincipal();

private:
    Matriz* matriz = new Matriz();
    ListaCircularDoble* lista = new ListaCircularDoble();
    Usuario* usuarioActual;
    string departamentoActual, empresaActual;
    int obtenerEntrada();
    string obtenerEntradaString();

    void insertarUsuariosQuemados();

    void loginUsuario();
    void mostrarMenuUsuario();
    void mostrarMenuAdministrador();
    void agregarActivo();
    void eliminarActivo();
    void modificarActivo();
    void rentarActivo();
    void activosRentados();
    void misActivosRentados();

    void registrarUsuario();
    void reporteMatrizDispersa();
    void reporteActivosDisponiblesDepartamento();
    void reporteActivosDisponiblesEmpresa();
    void reporteTransacciones();
    void reporteActivosUsuario();
    void reporteActivosRentadosUsuario();
    void ordenarTransacciones();

    void graficar(string dot, string nombre);
};

#endif // MENU_H