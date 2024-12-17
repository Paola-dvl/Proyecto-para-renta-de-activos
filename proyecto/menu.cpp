// menu.cpp
#include "menu.h"
#include <fstream>

int Menu::obtenerEntrada() {
    int entrada;
    while (true) {
        cin >> entrada;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada no valida. Por favor, ingrese un numero: ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return entrada;
        }
    }
}
string Menu::obtenerEntradaString(){
    string entrada;
    while (true) {
        getline(cin, entrada);

        if (entrada.empty()) {
            cout << "Entrada no valida. Por favor, ingrese un texto: ";
        } else {
            return entrada;
        }
    }
}
void Menu::insertarUsuariosQuemados() {
    struct UsuarioData {
        std::string nombre;
        std::string nombre_completo;
        std::string contrasena;
        std::string departamento;
        std::string empresa;
    };

    UsuarioData usuarios[] = {
        {"elian_estrada", "Elian Estrada", "1234", "guatemala", "igss"},
        {"juanito", "Juan Perez", "4567", "jutiapa", "max"},
        {"pedrito", "Pedro Rodriguez", "48956", "jalapa", "usac"},
        {"mafer", "Maria Fernanda", "54312", "peten", "cinepolis"},
        {"juanma", "Juan Manuel", "32897", "guatemala", "usac"},
        {"casimiro", "Carlos Perez", "721896", "guatemala", "max"},
        {"fuego03", "Fernando Mendez", "891346", "jutiapa", "cinepolis"},
        {"azurdia", "Alejandra Guzman", "780145", "jutiapa", "usac"},
        {"incrediboy", "Iraldo Martinez", "201598", "jutiapa", "max"},
        {"alcachofa", "Antonio Lopez", "20435", "jalapa", "usac"}
    };

    for (const auto& u : usuarios) {
        Usuario* usuario = new Usuario(u.nombre, u.contrasena, u.nombre_completo);
        matriz->cabeza->insertar(matriz->cabeza, u.empresa, u.departamento, *usuario);
    }
}

void Menu::mostrarMenuPrincipal() {
    int opcion;
    insertarUsuariosQuemados();
    do {
        usuarioActual = nullptr;
        cout << "\n=== Menu Principal ===\n";
        cout << "1. Menu de usuario\n";
        cout << "2. Menu de administrador\n";
        cout << "3. Salir\n";
        cout << "Seleccione una opcion: ";

        opcion = obtenerEntrada();

        switch (opcion) {
        case 1:
            loginUsuario();
            break;
        case 2:
            mostrarMenuAdministrador();
            break;
        case 3:
            cout << "Saliendo del programa...\n";
            break;
        default:
            cout << "Opcion no valida. Intente nuevamente.\n";
        }
    } while (opcion != 3);
}
void Menu::loginUsuario(){
    string nombre, contrasena;
    cout << "\n=== Login usuario ===\n";
    cout << "Ingrese el nombre del usuario:\n";
    nombre = obtenerEntradaString();
    cout << "Ingrese la contrasena:\n";
    contrasena = obtenerEntradaString();
    cout << "Ingrese el departamento:\n";
    departamentoActual = obtenerEntradaString();
    cout << "Ingrese la empresa:\n";
    empresaActual = obtenerEntradaString();
    usuarioActual = matriz->cabeza->buscarUsuario(matriz->cabeza, departamentoActual, empresaActual, nombre, contrasena);
    if(usuarioActual != nullptr) mostrarMenuUsuario();
}
void Menu::mostrarMenuUsuario() {
    int opcion;
    do {
        cout << "\n=== Menu de usuario ===\n";
        cout << "1. Agregar activo\n";
        cout << "2. Eliminar activo\n";
        cout << "3. Modificar activo\n";
        cout << "4. Rentar activo\n";
        cout << "5. Activos rentados\n";
        cout << "6. Mis activos rentados\n";
        cout << "7. Cerrar sesion\n";
        cout << "Seleccione una opcion: ";

        opcion = obtenerEntrada();

        switch (opcion) {
        case 1:
            agregarActivo();
            break;
        case 2:
            eliminarActivo();
            break;
        case 3:
            modificarActivo();
            break;
        case 4:
            rentarActivo();
            break;
        case 5:
            activosRentados();
            break;
        case 6:
            misActivosRentados();
            break;
        case 7:
            cout << "Cerrando sesion\nRegresando al menu principal...\n";
            break;
        default:
            cout << "Opcion no valida. Intente nuevamente.\n";
        }
    } while (opcion != 7);
}
void Menu::agregarActivo(){
    string nombre, descripcion;
    cout << "\n=== Agregar activo ===\n";
    cout << "Ingrese el nombre:\n";
    nombre = obtenerEntradaString();
    cout << "Ingrese la descripcion:\n";
    descripcion = obtenerEntradaString();
    Activo* activo = new Activo(nombre, descripcion);
    Activo* activoExistente = nullptr;
    string idUnico = "";
    do{
        idUnico = generarIdAleatorio(15);
        activoExistente = matriz->cabeza->buscarActivo(matriz->cabeza, idUnico);
    } while(activoExistente != nullptr);
    activo->setId(idUnico);
    usuarioActual->activos.insertar(*activo);
}
void Menu::eliminarActivo(){
    cout << "\n=== Eliminar activo ===\n";
    usuarioActual->activos.mostrarRentados(false);
    cout << "Ingrese el id del activo a eliminar:\n";
    string id = obtenerEntradaString();
    if(usuarioActual->activos.buscarPorId(id) == nullptr){
        cout << "Id del activo no encontrado\n";
    } else {
        usuarioActual->activos.eliminar(id);
        cout << "Activo eliminado\n";
    }
}
void Menu::modificarActivo(){
    cout << "\n=== Modificar activo ===\n";
    usuarioActual->activos.mostrarRentados(false);
    cout << "Ingrese el id del activo a modificar:\n";
    string id = obtenerEntradaString();
    Activo* activo = usuarioActual->activos.buscarPorId(id);
    if(activo == nullptr){
        cout << "Id del activo no encontrado\n";
    } else {
        cout << "Ingrese la nueva descripcion:\n";
        string descripcion = obtenerEntradaString();
        activo->setDescripcion(descripcion);
        Activo* activo = usuarioActual->activos.buscarPorId(id);
        cout << "Activo modificado\n";
        activo->mostrarActivo();
    }
}
void Menu::rentarActivo(){
    int opcion = 0;
    do{
        cout << "\n=== Rentar activo ===\n";
        matriz->cabeza->mostrarActivosDisponibles(matriz->cabeza, usuarioActual);
        cout << "Ingrese una opcion:\n";
        cout << "1. Rentar un activo:\n";
        cout << "2. Regresar al menu:\n";
        opcion = obtenerEntrada();
        if(opcion == 1){
            cout << "Ingrese el id del activo a rentar:\n";
            string id = obtenerEntradaString();
            Activo* activo = matriz->cabeza->buscarActivo(matriz->cabeza, id);
            if(activo == nullptr){
                cout << "Id del activo no encontrado\n";
            } else {
                activo->mostrarActivo();
                cout << "Ingrese el tiempo a rentar (ingrese 0 si desea regresar):\n";
                int tiempo = obtenerEntrada();
                if(tiempo > 0){
                    activo->rentarActivo();
                    Transaccion* transaccion = new Transaccion(id, usuarioActual->getNombre(),  
                        departamentoActual, empresaActual, time(nullptr), tiempo);
                    lista->insertar(*transaccion);
                    cout << "Activo rentado\n";
                }
            }
        } 
        else if (opcion == 2) cout << "Regresando...\n";
        else cout << "Ingrese una opcion valida.\n";
    }while(opcion != 2);
}
void Menu::activosRentados(){
    int opcion = 0;
    do{
        cout << "\n=== Activos rentados ===\n";
        lista->mostrarPorUsuario(usuarioActual->getNombre(), departamentoActual, empresaActual);
        cout << "Ingrese una opcion:\n";
        cout << "1. Registrar devolucion\n";
        cout << "2. Regresar al menu\n";
        opcion = obtenerEntrada();
        if(opcion == 1){
            cout << "Ingrese el id del activo a devolver:\n";
            string id = obtenerEntradaString();
            Activo* activo = matriz->cabeza->buscarActivo(matriz->cabeza, id);
            if(activo == nullptr){
                cout << "Id del activo no encontrado\n";
            } else {
                activo->mostrarActivo();
                activo->devolverActivo();
                lista->devolverPorUsuario(usuarioActual->getNombre(), departamentoActual, empresaActual);
                cout << "Activo devuelto\n";
            }
        } 
        else if (opcion == 2) cout << "Regresando...\n";
        else cout << "Ingrese una opcion valida.\n";
    }while(opcion != 2);
}
void Menu::misActivosRentados(){
    cout << "\n=== Mis ctivos rentados ===\n";
    usuarioActual->activos.mostrarRentados(true);
}

void Menu::mostrarMenuAdministrador() {
    int opcion;
    do {
        cout << "\n=== Submenu 2 ===\n";
        cout << "1. Registrar usuario\n";
        cout << "2. Reporte matriz dispersa\n";
        cout << "3. Reporte activos disponibles de un departamento\n";
        cout << "4. Reporte activos disponibles de una empresa\n";
        cout << "5. Reporte transacciones\n";
        cout << "6. Reporte activos de un usuario\n";
        cout << "7. Activos rentados por un usuario\n";
        cout << "8. Ordenar transacciones\n";
        cout << "9. Volver al menu principal\n";
        cout << "Seleccione una opcion: ";

        opcion = obtenerEntrada();

        switch (opcion) {
        case 1:
            registrarUsuario();
            break;
        case 2:
            reporteMatrizDispersa();
            break;
        case 3:
            reporteActivosDisponiblesDepartamento();
            break;
        case 4:
            reporteActivosDisponiblesEmpresa();
            break;
        case 5:
            reporteTransacciones();
            break;
        case 6:
            reporteActivosUsuario();
            break;
        case 7:
            reporteActivosRentadosUsuario();
            break;
        case 8:
            ordenarTransacciones();
            break;
        default:
            cout << "Opcion no valida. Intente nuevamente.\n";
        }
    } while (opcion != 9);
}
void Menu::registrarUsuario(){
    string nombre, contrasena, nombre_completo, departamento, empresa;
    cout << "\n=== Crear usuario ===\n";
    cout << "Ingrese el nombre del usuario:\n";
    nombre = obtenerEntradaString();
    cout << "Ingrese la contrasena:\n";
    contrasena = obtenerEntradaString();
    cout << "Ingrese el nombre completo del usuario:\n";
    nombre_completo = obtenerEntradaString();
    cout << "Ingrese el departamento:\n";
    departamento = obtenerEntradaString();
    cout << "Ingrese la empresa:\n";
    empresa = obtenerEntradaString();
    Usuario* usuario = new Usuario(nombre, contrasena, nombre_completo);
    matriz->cabeza->insertar(matriz->cabeza, empresa, departamento, *usuario);
}
void Menu::reporteMatrizDispersa(){
    cout << "**********************************" << endl;
    cout << "**********************************" << endl;
    //cout << matriz->cabeza->graficarMatrizDispersa(matriz->cabeza) << endl;
    graficar(matriz->cabeza->graficarMatrizDispersa(matriz->cabeza), "ReporteMatrizDispersa");
    cout << "**********************************" << endl;
    cout << "**********************************" << endl;
}
void Menu::reporteActivosDisponiblesDepartamento(){
    cout << "\n=== Reporte activos disponibles por departamento ===\n";
    matriz->cabeza->listarDepartamentos(matriz->cabeza);
    cout << "Ingrese el departamento:\n";
    string departamento = obtenerEntradaString();
    cout << "**********************************" << endl;
    cout << "**********************************" << endl;
    //cout << matriz->cabeza->graficarPorDepartamento(matriz->cabeza, departamento) << endl;
    graficar(matriz->cabeza->graficarPorDepartamento(matriz->cabeza, departamento), "ReporteActivosPorDepartamento");
    cout << "**********************************" << endl;
    cout << "**********************************" << endl;
}
void Menu::reporteActivosDisponiblesEmpresa(){
    cout << "\n=== Reporte activos disponibles por empresa ===\n";
    matriz->cabeza->listarEmpresas(matriz->cabeza);
    cout << "Ingrese la empresa:\n";
    string empresa = obtenerEntradaString();
    cout << "**********************************" << endl;
    cout << "**********************************" << endl;
    //cout << matriz->cabeza->graficarPorEmpresa(matriz->cabeza, empresa) << endl;
    graficar(matriz->cabeza->graficarPorEmpresa(matriz->cabeza, empresa), "ReporteActivosPorEmpresa");
    cout << "**********************************" << endl;
    cout << "**********************************" << endl;
}
void Menu::reporteTransacciones(){
    cout << "**********************************" << endl;
    cout << "**********************************" << endl;
    //cout << lista->graficarTransacciones() << endl;
    graficar(lista->graficarTransacciones(), "ReporteTransacciones");
    cout << "**********************************" << endl;
    cout << "**********************************" << endl;
}
void Menu::reporteActivosUsuario(){
    cout << "\n=== Reporte activos de un usuario ===\n";
    cout << "Ingrese el departamento:\n";
    string departamento = obtenerEntradaString();
    cout << "Ingrese la empresa:\n";
    string empresa = obtenerEntradaString();
    cout << "Ingrese el nombre del usuario:\n";
    string nombre = obtenerEntradaString();
    Usuario* usuario = matriz->cabeza->buscarUsuarioReporte(matriz->cabeza, departamento, empresa, nombre);
    if(usuario != nullptr){
        cout << "**********************************" << endl;
        cout << "**********************************" << endl;
        //usuario->activos.graficarActivosUsuario();
        graficar(usuario->activos.graficarActivosUsuario(), "ReporteActivosPorUsuario");
        cout << "**********************************" << endl;
        cout << "**********************************" << endl;
    }
}
void Menu::reporteActivosRentadosUsuario(){
    cout << "\n=== Reporte activos rentados por un usuario ===\n";
    cout << "Ingrese el departamento:\n";
    string departamento = obtenerEntradaString();
    cout << "Ingrese la empresa:\n";
    string empresa = obtenerEntradaString();
    cout << "Ingrese el nombre del usuario:\n";
    string nombre = obtenerEntradaString();
    Usuario* usuario = matriz->cabeza->buscarUsuarioReporte(matriz->cabeza, departamento, empresa, nombre);
    if (usuario != nullptr){
        cout << "**********************************" << endl;
        cout << "**********************************" << endl;
        //lista->graficarTransaccionesPorUsuario(nombre, departamento, empresa);
        graficar(lista->graficarTransaccionesPorUsuario(nombre, departamento, empresa), "ReporteActivosRentadosPorUsuario");
        cout << "**********************************" << endl;
        cout << "**********************************" << endl;
    }
}
void Menu::ordenarTransacciones(){
    cout << "\n=== Ordenar transacciones ===\n";
    cout << "1. Orden Descendente.\n";
    cout << "2. Orden Ascendente.\n";
    cout << "Ingrese el tipo de orden a realizar:\n";
    int opcion = obtenerEntrada();
    if(opcion == 1) lista->ordenarDescendente();
    else if(opcion == 2) lista->ordenarAscendente();
    else cout << "Opcion no valida.\n";
}

void Menu::graficar(string dot, string nombre){
    ofstream archivo("reportes/" + nombre + ".dot");
    archivo << dot;
    archivo.close();
    string comando = "dot -Tpng reportes/" + nombre + ".dot -o reportes/" + nombre + ".png";
    if(nombre == "ReporteMatrizDispersa") comando = "neato -Tpng reportes/" + nombre + ".dot -o reportes/" + nombre + ".png";
    system(comando.c_str());
    cout << "Reporte generado con exito.\n";
}