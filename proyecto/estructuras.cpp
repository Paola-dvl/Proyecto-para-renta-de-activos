// estructuras.cpp
#include "libs.h"
//******************************************** */
//******************************************** */
// Inician declaraciones de clases
//******************************************** */
//******************************************** */
//******************************************** */
//******************************************** */
// Inician entidades
//******************************************** */
//******************************************** */
class Activo {
    private:
        string id;
        string nombre;
        string descripcion;
        bool disponible;

    public:
        Activo(const string& nombre_, const string& descripcion_)
            : nombre(nombre_), descripcion(descripcion_), disponible(true) {}

        bool operator<(const Activo& other) const {
            return id < other.id; 
        }

        bool operator>(const Activo& other) const {
            return id > other.id; 
        }

        bool operator==(const Activo& other) const {
            return id == other.id;
        }

        string getId() const {
            return id;
        }

        string getNombre() const {
            return nombre;
        }

        bool getDisponible() const{
            return disponible;
        }

        string getDescripcion() const {
            return descripcion;
        }

        void setId(const string& id_) {
            id = id_;
        }

        void setNombre(const string& nombre_) {
            nombre = nombre_;
        }

        void setDescripcion(const string& descripcion_) {
            descripcion = descripcion_;
        }

        void mostrarActivo() const {
            cout << "ID: " << id << "\n";
            cout << "\tNombre: " << nombre << "\n";
            cout << "\tDescripcion: " << descripcion << "\n";
        }

        void rentarActivo(){
            disponible = false;
        }
        void devolverActivo(){
            disponible = true;
        }
};
class Departamento{
    private:
        string nombre;
    public:
        Departamento(string nombre): nombre(nombre){}
        string getNombre(){return nombre;}
};
class Empresa{
    private:
        string nombre;
    public:
        Empresa(string nombre): nombre(nombre){}
        string getNombre(){return nombre;}
};
//******************************************** */
class NodoAVL {
    public:
        Activo data;
        NodoAVL* izquierdo;
        NodoAVL* derecho;
        int altura;

        NodoAVL(Activo data) : data(data), izquierdo(nullptr), derecho(nullptr), altura(1) {}
};
class AVL {
    private:
        NodoAVL* raiz;

        int getAltura(NodoAVL* nodo){
            return nodo ? nodo->altura : 0;     
        }

        NodoAVL* rotarDerecha(NodoAVL* y){
            NodoAVL* x = y->izquierdo;
            NodoAVL* T2 = x->derecho;
            x->derecho = y;
            y->izquierdo = T2;
            y->altura = 1 + max(getAltura(y->izquierdo), getAltura(y->derecho));
            x->altura = 1 + max(getAltura(x->izquierdo), getAltura(x->derecho));
            return x;
        }

        NodoAVL* rotarIzquierda(NodoAVL* x){
            NodoAVL* y = x->derecho;
            NodoAVL* T2 = y->izquierdo;
            y->izquierdo = x;
            x->derecho = T2;
            x->altura = 1 + max(getAltura(x->izquierdo), getAltura(x->derecho));
            y->altura = 1 + max(getAltura(y->izquierdo), getAltura(y->derecho));
            return y;
        }

        int factorBalance(NodoAVL* nodo){
            return nodo ? getAltura(nodo->izquierdo) - getAltura(nodo->derecho) : 0;
        }

        NodoAVL* insertar(NodoAVL* nodo, Activo data){
            if(nodo == nullptr) return new NodoAVL(data);
            if(data.getId() < nodo->data.getId()){
                nodo->izquierdo = insertar(nodo->izquierdo, data);
            } else if(data.getId() > nodo->data.getId()){
                nodo->derecho = insertar(nodo->derecho, data);
            } else{
                return nodo;
            }
            nodo->altura = 1 + max(getAltura(nodo->izquierdo), getAltura(nodo->derecho));
            int factor = factorBalance(nodo);
            if(factor > 1 && data.getId() < nodo->izquierdo->data.getId())
                return rotarDerecha(nodo);
            if(factor < -1 && data.getId() > nodo->derecho->data.getId())
                return rotarIzquierda(nodo);
            if(factor > 1 && data.getId() > nodo->izquierdo->data.getId()){
                nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
                return rotarDerecha(nodo);
            }
            if(factor < -1 && data.getId() < nodo->derecho->data.getId()){
                nodo->derecho = rotarDerecha(nodo->derecho);
                return rotarIzquierda(nodo);
            }
            return nodo;
        }

        void preOrder(NodoAVL* nodo){
            if(nodo != nullptr){
                nodo->data.mostrarActivo();
                preOrder(nodo->izquierdo);
                preOrder(nodo->derecho);
            }
        }

        void preOrderDisponibles(NodoAVL* nodo, bool rentados){
            if(nodo != nullptr){
                if(nodo->data.getDisponible() == rentados)
                    nodo->data.mostrarActivo();
                preOrderDisponibles(nodo->izquierdo, rentados);
                preOrderDisponibles(nodo->derecho, rentados);
            }
        }

        NodoAVL* nodoMinimo(NodoAVL* nodo) {
            NodoAVL* actual = nodo;
            while (actual->izquierdo)
                actual = actual->izquierdo;
            return actual;
        }

        NodoAVL* eliminar(NodoAVL* nodo, const string& id) {
            if (!nodo) return nodo;

            if (id < nodo->data.getId()) {
                nodo->izquierdo = eliminar(nodo->izquierdo, id);
            } else if (id > nodo->data.getId()) {
                nodo->derecho = eliminar(nodo->derecho, id);
            } else {
                if (!nodo->izquierdo || !nodo->derecho) {
                    NodoAVL* temp = nodo->izquierdo ? nodo->izquierdo : nodo->derecho;
                    if (!temp) {
                        temp = nodo;
                        nodo = nullptr;
                    } else {
                        *nodo = *temp;
                    }
                    delete temp;
                } else {
                    NodoAVL* temp = nodoMinimo(nodo->derecho);
                    nodo->data = temp->data;
                    nodo->derecho = eliminar(nodo->derecho, temp->data.getId());
                }
            }

            if (!nodo) return nodo;

            nodo->altura = 1 + max(getAltura(nodo->izquierdo), getAltura(nodo->derecho));

            int balance = factorBalance(nodo);

            if (balance > 1 && factorBalance(nodo->izquierdo) >= 0)
                return rotarDerecha(nodo);

            if (balance > 1 && factorBalance(nodo->izquierdo) < 0) {
                nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
                return rotarDerecha(nodo);
            }

            if (balance < -1 && factorBalance(nodo->derecho) <= 0)
                return rotarIzquierda(nodo);

            if (balance < -1 && factorBalance(nodo->derecho) > 0) {
                nodo->derecho = rotarDerecha(nodo->derecho);
                return rotarIzquierda(nodo);
            }

            return nodo;
        }

        Activo* buscarPorId(NodoAVL* nodo, const string& id) {
            if (!nodo) return nullptr;
            if (id < nodo->data.getId()) {
                return buscarPorId(nodo->izquierdo, id);
            } else if (id > nodo->data.getId()) {
                return buscarPorId(nodo->derecho, id);
            } else {
                return &nodo->data;
            }
        }
        void liberarMemoria(NodoAVL* nodo) {
            if (nodo) {
                liberarMemoria(nodo->izquierdo);
                liberarMemoria(nodo->derecho);
                delete nodo;
            }
        }

        string preOrderGraficar(NodoAVL* nodo, string activos){
            if(nodo != nullptr){
                activos += "\"" + nodo->data.getId() + "\" [label=\"ID: " + nodo->data.getId() + "\nNombre: " + nodo->data.getNombre() + "\nDescripcion: " + nodo->data.getDescripcion() + "\"]\n";
                if(nodo->izquierdo != nullptr) activos += "\"" + nodo->data.getId() + "\" -> \"" + nodo->izquierdo->data.getId() + "\"\n";
                if(nodo->derecho != nullptr) activos += "\"" + nodo->data.getId() + "\" -> \"" + nodo->derecho->data.getId() + "\"\n";
                activos = preOrderGraficar(nodo->izquierdo, activos);
                activos = preOrderGraficar(nodo->derecho, activos);
            }
            return activos;
        }

    public:
        AVL() : raiz(nullptr) {}
        ~AVL() {
            liberarMemoria(raiz);
        }

        void insertar(Activo data) {
            raiz = insertar(raiz, data);
            cout << "Activo insertado con exito\n";
        }

        void eliminar(const string& id) {
            raiz = eliminar(raiz, id);
            cout << "Activo eliminado con exito\n";
        }

        Activo* buscarPorId(const string& id) {
            return buscarPorId(raiz, id);
        }

        void preorden() {
            preOrder(raiz);
        }

        void mostrarRentados(bool rentados){ //True -> Mostrar rentados     False -> Mostrar disponibles
            preOrderDisponibles(raiz, !rentados);
        }

        string graficarActivosUsuario(){
            string codigo = "digraph{\nnode [shape=box];\n";
            string activos = "\n#Activos\n";
            activos = preOrderGraficar(raiz, activos);
            return codigo + activos + "}";
        }

        string graficarActivosUsuarioArbol(){
            return preOrderGraficar(raiz, "");
        }
};
//******************************************** */
class Usuario{
    private:
        string nombre;
        string contrasena;
        string nombre_completo;
    public:
        AVL activos;
        Usuario(string nombre, string contrasena, string nombre_completo)
            : nombre(nombre), contrasena(contrasena), nombre_completo(nombre_completo){}

        string getNombre(){return nombre;}
        string getContrasena(){return contrasena;}

        bool operator==(const Usuario& other) const {
            return nombre == other.nombre;
        }
};
class Transaccion{
    private:
        string id;
        string id_activo;
        string usuario;
        string departamento;
        string empresa;
        time_t fecha_inicio;
        int tiempo_renta;
        bool devuelto = false;
    public:
        Transaccion(string id_activo, string usuario, string departamento, string empresa, time_t fecha_inicio, int tiempo_renta)
            : id_activo(id_activo), usuario(usuario), departamento(departamento), empresa(empresa), fecha_inicio(fecha_inicio), tiempo_renta(tiempo_renta) {}
        void setId(const string& id_) {
            id = id_;
        }
        string getId() const { return id; }
        string getIdActivo() const { return id_activo; }
        string getUsuario() const { return usuario; }
        string getDepartamento() const { return departamento; }
        string getEmpresa() const { return empresa; }
        string getFechaInicio() const { 
            struct tm* timeInfo = localtime(&fecha_inicio); 
            ostringstream oss;
            oss << std::setfill('0') << std::setw(2) << timeInfo->tm_mday << "-"
                << std::setfill('0') << std::setw(2) << timeInfo->tm_mon + 1 << "-"
                << timeInfo->tm_year + 1900 << " "
                << std::setfill('0') << std::setw(2) << timeInfo->tm_hour << ":"
                << std::setfill('0') << std::setw(2) << timeInfo->tm_min << ":"
                << std::setfill('0') << std::setw(2) << timeInfo->tm_sec;

            return oss.str();
            }
        int getTiempoRenta() const { return tiempo_renta; }
        void devolver() { devuelto = true; }
        bool getDevuelto() { return devuelto; }
};
//******************************************** */
//******************************************** */
// Inician estructuras
//******************************************** */
//******************************************** */
class ListaCircular{
    public:
        Transaccion data;
        ListaCircular* izquierda;
        ListaCircular* derecha;
        ListaCircular(Transaccion data) : data(data), izquierda(nullptr), derecha(nullptr) {}
};
class ListaCircularDoble {
private:
    ListaCircular* cabeza;

public:
    ListaCircularDoble() : cabeza(nullptr) {}

    void insertar(Transaccion data) {
        string idUnico = "";
        Transaccion* existente = nullptr;
        do{
            idUnico = generarIdAleatorio(15);
            existente = buscarPorId(idUnico);
        } while(existente != nullptr);
        data.setId(idUnico);

        ListaCircular* nuevoNodo = new ListaCircular(data);

        if (!cabeza) {
            cabeza = nuevoNodo;
            cabeza->izquierda = cabeza;
            cabeza->derecha = cabeza;
        } else {
            ListaCircular* actual = cabeza;
            
            do {
                if (data.getId() < actual->data.getId()) {
                    ListaCircular* anterior = actual->izquierda;
                    anterior->derecha = nuevoNodo;
                    nuevoNodo->izquierda = anterior;
                    nuevoNodo->derecha = actual;
                    actual->izquierda = nuevoNodo;
                    if (actual == cabeza) {
                        cabeza = nuevoNodo; 
                    }
                    return;
                }
                actual = actual->derecha;
            } while (actual != cabeza);

            ListaCircular* ultimo = cabeza->izquierda;
            ultimo->derecha = nuevoNodo;
            nuevoNodo->izquierda = ultimo;
            nuevoNodo->derecha = cabeza;
            cabeza->izquierda = nuevoNodo;
        }
    }

    Transaccion* buscarPorId(const string& id) {
        if (!cabeza) {
            return nullptr; 
        }
        ListaCircular* actual = cabeza;
        do {
            if (actual->data.getId() == id) {
                return &actual->data;
            }
            actual = actual->derecha;
        } while (actual != cabeza);  
        return nullptr; 
    }

    void mostrar() {
        if (!cabeza) {
            cout << "La lista esta vacia." << endl;
            return;
        }

        ListaCircular* actual = cabeza;
        do {
            cout << "*************************************" << endl;
            cout << "ID: " << actual->data.getId() << ", Id activo: " << actual->data.getIdActivo() << ", Usuario: " << actual->data.getUsuario()
                    << ", Departamento: " << actual->data.getDepartamento() << ", Empresa:" << actual->data.getEmpresa()
                    << ", Fecha de inicio: " << actual->data.getFechaInicio() << ", Tiempo de renta:" << actual->data.getTiempoRenta() << endl;
            actual = actual->derecha;
        } while (actual != cabeza);
    }

    void mostrarPorUsuario(string usuario, string departamento, string empresa) {
        if (!cabeza) {
            cout << "La lista esta vacia." << endl;
            return;
        }

        ListaCircular* actual = cabeza;
        do {
            Transaccion transaccion = actual->data;
            if(transaccion.getUsuario() == usuario && transaccion.getDepartamento() == departamento && transaccion.getEmpresa() == empresa && transaccion.getDevuelto() == false){
                cout << "*************************************" << endl;
                cout << "ID: " << actual->data.getId() << ", Id activo: " << actual->data.getIdActivo() << ", Usuario: " << actual->data.getUsuario()
                        << ", Departamento: " << actual->data.getDepartamento() << ", Empresa:" << actual->data.getEmpresa()
                        << ", Fecha de inicio: " << actual->data.getFechaInicio() << ", Tiempo de renta:" << actual->data.getTiempoRenta() << endl;
            }
            actual = actual->derecha;
        } while (actual != cabeza);
    }
    void devolverPorUsuario(string usuario, string departamento, string empresa) {
        ListaCircular* actual = cabeza;
        do {
            Transaccion transaccion = actual->data;
            if(transaccion.getUsuario() == usuario && transaccion.getDepartamento() == departamento && transaccion.getEmpresa() == empresa && transaccion.getDevuelto() == false){
                transaccion.devolver();
                break;
            }
            actual = actual->derecha;
        } while (actual != cabeza);
    }

    void ordenarDescendente() {
        if (!cabeza) {
            cout << "La lista está vacía." << endl;
            return;
        }

        bool ordenado; 
        do {
            ordenado = true;  
            ListaCircular* actual = cabeza;

            do {
                ListaCircular* siguiente = actual->derecha;

                if (siguiente != cabeza && actual->data.getId() < siguiente->data.getId()) {
                    auto temp = actual->data;
                    actual->data = siguiente->data;
                    siguiente->data = temp;

                    ordenado = false;  
                }

                actual = actual->derecha; 
            } while (actual != cabeza);  

        } while (!ordenado); 

        cout << "La lista ha sido ordenada en orden descendente." << endl;
    }

    void ordenarAscendente() {
    if (!cabeza) {
        cout << "La lista está vacía." << endl;
        return;
    }

    bool ordenado; 
    do {
        ordenado = true; 
        ListaCircular* actual = cabeza;

        do {
            ListaCircular* siguiente = actual->derecha;

            if (siguiente != cabeza && actual->data.getId() > siguiente->data.getId()) {
                auto temp = actual->data;
                actual->data = siguiente->data;
                siguiente->data = temp;

                ordenado = false; 
            }

            actual = actual->derecha; 
        } while (actual != cabeza); 

    } while (!ordenado); 

    cout << "La lista ha sido ordenada en orden ascendente." << endl;
}



    string graficarTransacciones(){
        string codigo = "digraph{\nnode [shape=box];\n";
        string transacciones = "\n#Transacciones\n";
        ListaCircular* actual = cabeza;
        do {
            transacciones += "\"" + actual->data.getId() + "\" [label=\"ID: " + actual->data.getId() + "\nID Activo: " + actual->data.getIdActivo() + "\nUsuario: " + actual->data.getUsuario() + "\nDepartamento: " + actual->data.getDepartamento() + "\nEmpresa: " + actual->data.getEmpresa() + "\nFecha de inicio: " + actual->data.getFechaInicio() + "\nTiempo de renta: " + to_string(actual->data.getTiempoRenta()) +"\nDevuelto: " + to_string(actual->data.getDevuelto()) + "\"]\n";
            transacciones += "\"" + actual->data.getId() + "\" -> \"" + actual->derecha->data.getId() + "\"\n";
            transacciones += "\"" + actual->derecha->data.getId() + "\" -> \"" + actual->data.getId() + "\"\n";
            actual = actual->derecha;
        } while (actual != cabeza);
        return codigo + transacciones + "}";
    }
    string graficarTransaccionesPorUsuario(string usuario, string departamento, string empresa){
        string codigo = "digraph{\nnode [shape=box];\n";
        string transacciones = "\n#Transacciones por usuario\n";
        ListaCircular* actual = cabeza;
        string id_ant = "", id_primero = "";
        do {
            if(actual->data.getUsuario() == usuario && actual->data.getDepartamento() == departamento && actual->data.getEmpresa() == empresa){
                id_ant = actual->data.getId();
                if(id_primero == "") id_primero = id_ant;
                transacciones += "\"" + actual->data.getId() + "\" [label=\"ID: " + actual->data.getId() + "\nID Activo: " + actual->data.getIdActivo() + "\nUsuario: " + actual->data.getUsuario() + "\nDepartamento: " + actual->data.getDepartamento() + "\nEmpresa: " + actual->data.getEmpresa() + "\nFecha de inicio: " + actual->data.getFechaInicio() + "\nTiempo de renta: " + to_string(actual->data.getTiempoRenta()) +"\nDevuelto: " + to_string(actual->data.getDevuelto()) + "\"]\n";
                if(id_ant != "") {
                    transacciones += "\"" + actual->data.getId() + "\" -> \"" + id_ant + "\"\n";
                    transacciones += "\"" + id_ant + "\" -> \"" + actual->data.getId() + "\"\n";
                }
                actual = actual->derecha;
                id_ant = actual->data.getId();
            }
        } while (actual != cabeza);
        if(id_ant != id_primero) {
            transacciones += "\"" + id_primero + "\" -> \"" + id_ant + "\"\n";
            transacciones += "\"" + id_ant + "\" -> \"" + id_primero + "\"\n";
        }
        return codigo + transacciones + "}";
    }
    ~ListaCircularDoble() {
        if (!cabeza) return;

        ListaCircular* actual = cabeza;
        do {
            ListaCircular* siguiente = actual->derecha;
            delete actual;
            actual = siguiente;
        } while (actual != cabeza);

        cabeza = nullptr;
    }
};
//******************************************** */
class Usuarios{
    public:
        Usuario usuario;
        Usuarios* siguiente;
        Usuarios(Usuario data) : usuario(data), siguiente(nullptr) {}
        ~Usuarios() { delete siguiente; }
};
class Celda{
    private:
        Usuarios* usuarios;
        Celda* derecha;
        Celda* izquierda;
        Celda* arriba;
        Celda* abajo;
        Departamento* departamento;
        Empresa* empresa;
        int fila, columna;

    public:    
        Celda(): derecha(nullptr), izquierda(nullptr), arriba(nullptr), abajo(nullptr), fila(0), columna(0), usuarios(nullptr), departamento(nullptr), empresa(nullptr) {}
        Celda(Empresa* empresa, int fila): fila(fila), columna(0), derecha(nullptr), izquierda(nullptr), arriba(nullptr), abajo(nullptr), usuarios(nullptr), departamento(nullptr), empresa(empresa) {}
        Celda(Departamento* departamento, int columna): fila(0), columna(columna), derecha(nullptr), izquierda(nullptr), arriba(nullptr), abajo(nullptr), usuarios(nullptr), departamento(departamento), empresa(nullptr) {}
        Celda(Usuarios* usuarios, int fila, int columna): fila(fila), columna(columna), derecha(nullptr), izquierda(nullptr), arriba(nullptr), abajo(nullptr), usuarios(usuarios), departamento(nullptr), empresa(nullptr) {}

        ~Celda() {
            delete usuarios;
            delete derecha;
            delete izquierda;
            delete abajo;
            delete arriba;
        } 

        bool insertarUsuario(Usuario data){
            Usuarios* actual = usuarios;
            while(actual != nullptr){
                if(data.getNombre() == actual->usuario.getNombre()) break;
                actual = actual->siguiente;
            }
            if (actual == nullptr){
                Usuarios* usuarios_new = new Usuarios(data);
                usuarios_new->siguiente = usuarios;
                usuarios = usuarios_new;
                cout << "Usuario insertado con exito." << endl;
                return true;
            }
            cout << "Ya existe el usuario " << data.getNombre() << " en esta empresa y departamento." << endl; 
            return false;
        }

        Celda* buscarEmpresa(Celda* cabeza, string nombre){
            Celda* filaActual = cabeza->abajo;
            Celda* filaAnterior = cabeza;
            while(filaActual != nullptr){
                if(filaActual->empresa->getNombre() == nombre) return filaActual;
                filaAnterior = filaActual;
                filaActual = filaActual->abajo;
            }
            Empresa* empresa = new Empresa(nombre);
            Celda* filaNueva = new Celda(empresa, filaAnterior->fila + 1);
            filaNueva->arriba = filaAnterior;
            filaAnterior->abajo = filaNueva;
            return filaNueva;
        }
        Celda* buscarDepartamento(Celda* cabeza, string nombre){
            Celda* columnaActual = cabeza->derecha;
            Celda* columnaAnterior = cabeza;
            while(columnaActual != nullptr){
                if(columnaActual->departamento->getNombre() == nombre) return columnaActual;
                columnaAnterior = columnaActual;
                columnaActual = columnaActual->derecha;
            }
            Departamento* departamento = new Departamento(nombre);
            Celda* columnaNueva = new Celda(departamento, columnaAnterior->columna + 1);
            columnaNueva->izquierda = columnaAnterior;
            columnaAnterior->derecha = columnaNueva;
            return columnaNueva;
        }
        void insertarCeldaNueva(Celda* fila, Celda* columna, Usuario data){
            Celda* celda = new Celda(nullptr, fila->fila, columna->columna);
            if (celda->insertarUsuario(data)){
                celda->derecha = fila->derecha;
                celda->abajo = columna->abajo;
                fila->derecha = celda;
                celda->izquierda = fila;
                columna->abajo = celda;
                celda->arriba = columna;
            }
        }
        void insertar(Celda* cabeza, string empresa, string departamento, Usuario usuario){
            Celda* fila = buscarEmpresa(cabeza, empresa);
            Celda* columna = buscarDepartamento(cabeza, departamento);
            if(fila->derecha == nullptr){
                if (columna->abajo == nullptr){ 
                    // Fila vacia - Columna vacia
                    insertarCeldaNueva(fila, columna, usuario);
                } else {
                    //Fila vacia - Columna con datos
                    Celda* colActual = columna->abajo;
                    Celda* colAnterior = columna;
                    while(colActual != nullptr){
                        if(colActual->fila == fila->fila){ //Hay celda
                            colActual->insertarUsuario(usuario);
                            break;
                        }
                        if(colActual->fila > fila->fila){ //No hay celda
                            insertarCeldaNueva(fila, colAnterior, usuario);
                            break;
                        }
                        colAnterior = colActual;
                        colActual = colActual->abajo;
                    }
                    if(colActual == nullptr){ // No hay celda
                        insertarCeldaNueva(fila, colAnterior, usuario);
                    }
                }
            }else {
                Celda* filActual = fila->derecha;
                while(columna->abajo != nullptr){
                    if(columna->abajo->fila > fila->fila){
                        break;
                    }
                    columna = columna->abajo;
                }
                Celda* filAnterior = fila;
                while(filActual != nullptr){
                    if(filActual->columna == columna->columna){ //Hay celda
                        filActual->insertarUsuario(usuario);
                        break;
                    }
                    if(filActual->columna > columna->columna){ //No hay celda
                        insertarCeldaNueva(filAnterior, columna, usuario);
                        break;
                    }
                    filAnterior = filActual;
                    filActual = filActual->derecha;
                }
                if(filActual == nullptr){ // No hay celda
                    insertarCeldaNueva(filAnterior, columna, usuario);
                }
            }
        }

        Usuario* buscarUsuario(Celda* cabeza, string departamento, string empresa,  string nombre, string contrasena){
            int columna = 0, fila = 0;
            Celda* filaActual = cabeza->abajo;
            while(filaActual != nullptr){
                if(filaActual->empresa->getNombre() == empresa){
                    fila = filaActual->fila;
                    break;
                }
                filaActual = filaActual->abajo;
            }
            if(filaActual != nullptr){
                Celda* colActual = cabeza->derecha;
                while(colActual != nullptr){
                    if(colActual->departamento->getNombre() == departamento){
                        columna = colActual->columna;
                        break;
                    }
                    colActual = colActual->derecha;
                }
                if(colActual != nullptr){
                    Celda* celdaActual = filaActual->derecha;
                    while(celdaActual != nullptr){
                        if(celdaActual->columna == columna){
                            Usuarios* usuarios = celdaActual->usuarios;
                            while(usuarios != nullptr){
                                if(usuarios->usuario.getNombre() == nombre && usuarios->usuario.getContrasena() == contrasena) 
                                    return &usuarios->usuario;
                                usuarios = usuarios->siguiente;
                            }
                            break;
                        }
                        celdaActual = celdaActual->derecha;
                    }
                }
            }
            cout << "Usuario no encontrado.\n";
            return nullptr;
        }
        
        Usuario* buscarUsuarioReporte(Celda* cabeza, string departamento, string empresa,  string nombre){
            int columna = 0, fila = 0;
            Celda* filaActual = cabeza->abajo;
            cout << "Buscando... \n";
            while(filaActual != nullptr){
                cout << "Fila: " << filaActual->empresa->getNombre() << endl;
                if(filaActual->empresa->getNombre() == empresa){
                    fila = filaActual->fila;
                    break;
                }
                filaActual = filaActual->abajo;
            }
            if(filaActual != nullptr){
                cout << "Fila: " << filaActual->empresa->getNombre() << " == " << empresa << endl;
                Celda* colActual = cabeza->derecha;
                while(colActual != nullptr){
                    cout << "Columna: " << colActual->departamento->getNombre() << endl;
                    if(colActual->departamento->getNombre() == departamento){
                        columna = colActual->columna;
                        break;
                    }
                    colActual = colActual->derecha;
                }
                if(colActual != nullptr){
                    cout << "Columna: " << colActual->departamento->getNombre() << " == " << departamento << endl;
                    Celda* celdaActual = filaActual->derecha;
                    while(celdaActual != nullptr){
                        if(celdaActual->columna == columna){
                            Usuarios* usuarios = celdaActual->usuarios;
                            while(usuarios != nullptr){
                                if(usuarios->usuario.getNombre() == nombre) 
                                    return &usuarios->usuario;
                                usuarios = usuarios->siguiente;
                            }
                            break;
                        }
                        celdaActual = celdaActual->derecha;
                    }
                }
            }
            cout << "Usuario no encontrado.\n";
            return nullptr;
        }
        Activo* buscarActivo(Celda* cabeza, string id){
            Celda* filaActual = cabeza->abajo;
            while(filaActual != nullptr){
                Celda* celdaActual = filaActual->derecha;
                while(celdaActual != nullptr){
                    Usuarios* usuarios = celdaActual->usuarios;
                    while(usuarios != nullptr){
                        Activo* activo = usuarios->usuario.activos.buscarPorId(id);
                        if(activo != nullptr) return activo;
                        usuarios = usuarios->siguiente;
                    }
                    celdaActual = celdaActual->derecha;
                }
                filaActual = filaActual->abajo;
            }
            return nullptr;
        }
        void listarDepartamentos(Celda* cabeza){
            Celda* colActual = cabeza->derecha;
            while(colActual != nullptr){
                cout << colActual->departamento->getNombre() << endl;
                colActual = colActual->derecha;
            }
        }
        void listarEmpresas(Celda* cabeza){
            Celda* filaActual = cabeza->abajo;
            while(filaActual != nullptr){
                cout << filaActual->empresa->getNombre() << endl;
                filaActual = filaActual->abajo;
            }
        }
        void mostrarActivosDisponibles(Celda* cabeza, Usuario* usuario){
            Celda* filaActual = cabeza->abajo;
            while(filaActual != nullptr){
                Celda* celdaActual = filaActual->derecha;
                while(celdaActual != nullptr){
                    Usuarios* usuarios = celdaActual->usuarios;
                    while(usuarios != nullptr){
                        Usuario* usuarioActual = &usuarios->usuario;
                        if (usuarioActual != usuario)
                        usuarios->usuario.activos.mostrarRentados(false);
                        usuarios = usuarios->siguiente;
                    }
                    celdaActual = celdaActual->derecha;
                }
                filaActual = filaActual->abajo;
            }
        }
        
        string graficarMatrizDispersa(Celda* cabeza){
            string codigo = "digraph{\nnode [shape=box width=1.5];\n";
            string columnas , celdaId, posicion, celdaUsuarios, conexiones = "";
            columnas = "\n#Columnas\n";
            Celda* colActual = cabeza;
            Celda* celdaActual;
            while(colActual != nullptr){
                celdaActual = colActual;
                while(celdaActual != nullptr){
                    celdaId = "\"" + to_string(celdaActual->columna) + "_" + to_string(celdaActual->fila) + "\"";
                    posicion = "\"" + to_string(2*celdaActual->columna) + ",-" + to_string(celdaActual->fila) + "!\"";
                    columnas += celdaId;
                    if(celdaActual->empresa != nullptr) columnas += " [label=\"Empresa:\\n" + celdaActual->empresa->getNombre() + "\" pos = " + posicion + " style = filled, fillcolor = lightskyblue]\n";
                    else if(celdaActual->departamento != nullptr) columnas += " [label=\"Departamento:\\n" + celdaActual->departamento->getNombre() + "\" pos = " + posicion + " style = filled, fillcolor = bisque1]\n";
                    else if (celdaActual->usuarios != nullptr){
                        celdaUsuarios = "Usuarios: ";
                        Usuarios* usuarios = celdaActual->usuarios;
                        while(usuarios != nullptr){
                            celdaUsuarios += usuarios->usuario.getNombre();
                            if(usuarios->siguiente != nullptr) celdaUsuarios += ", \\n";
                            usuarios = usuarios->siguiente;
                        }
                        columnas += " [label=\"" + celdaUsuarios + "\" pos = " + posicion + "]\n";
                    } 
                    else columnas += " [label=\"Matriz\" pos = " + posicion + " style = filled, fillcolor = firebrick1]\n";
                    if(celdaActual->abajo != nullptr){
                        conexiones += celdaId  + " -> \"" + to_string(celdaActual->abajo->columna) + "_" + to_string(celdaActual->abajo->fila) + "\"\n";
                        conexiones += "\"" + to_string(celdaActual->abajo->columna) + "_" + to_string(celdaActual->abajo->fila) + "\" -> " + celdaId + "\n";
                    }
                    celdaActual = celdaActual->abajo;
                }
                colActual = colActual->derecha;
            }
            Celda* filActual = cabeza;
            while(filActual != nullptr){
                celdaActual = filActual;
                while(celdaActual != nullptr){
                    celdaId = "\"" + to_string(celdaActual->columna) + "_" + to_string(celdaActual->fila) + "\"";
                    if(celdaActual->derecha != nullptr){
                        conexiones += celdaId  + "-> \"" + to_string(celdaActual->derecha->columna) + "_" + to_string(celdaActual->derecha->fila) + "\"\n";
                        conexiones += "\"" + to_string(celdaActual->derecha->columna) + "_" + to_string(celdaActual->derecha->fila) + "\" -> " + celdaId + "\n";
                    }
                    celdaActual = celdaActual->derecha;
                }
                filActual = filActual->abajo;
            }
            return codigo + columnas + conexiones + "\n}";
        }
        string graficarPorDepartamento(Celda* cabeza, string departamento){
            string codigo = "digraph{\nnode [shape=box];\n";
            Celda* colActual = cabeza->derecha;
            Celda* celdaActual;
            int cluster = 0;
            while(colActual != nullptr){
                if(colActual->departamento->getNombre() == departamento){
                    celdaActual = colActual->abajo;
                    while(celdaActual != nullptr){
                        if (celdaActual->usuarios != nullptr){
                            Usuarios* usuarios = celdaActual->usuarios;
                            while(usuarios != nullptr){
                                codigo += "subgraph cluster_" + to_string(cluster) + "{\n";
                                codigo += usuarios->usuario.activos.graficarActivosUsuarioArbol();
                                codigo += "\nlabel=\"Usuario: " + usuarios->usuario.getNombre() + "\"}\n";
                                usuarios = usuarios->siguiente;
                                cluster++;
                            }
                        } 
                        celdaActual = celdaActual->abajo;
                    }
                }
                colActual = colActual->derecha;
            }
            return codigo + "\n}";
        }
        string graficarPorEmpresa(Celda* cabeza, string empresa){
            string codigo = "digraph{\nnode [shape=box];\n";
            Celda* celdaActual;
            Celda* filActual = cabeza->abajo;
            int cluster = 0;
            while(filActual != nullptr){
                if(filActual->empresa->getNombre() == empresa){       
                    celdaActual = filActual->derecha;
                    while(celdaActual != nullptr){
                        if(celdaActual->usuarios != nullptr){
                            Usuarios* usuarios = celdaActual->usuarios;
                            while(usuarios != nullptr){
                                codigo += "subgraph cluster_" + to_string(cluster) + "{\n";
                                codigo += usuarios->usuario.activos.graficarActivosUsuarioArbol();
                                codigo += "\nlabel=\"Usuario: " + usuarios->usuario.getNombre() + "\"}\n";
                                usuarios = usuarios->siguiente;
                                cluster++;
                            }
                        } 
                        celdaActual = celdaActual->derecha;
                    }
                }
                filActual = filActual->abajo;
            }
            return codigo + "\n}";
        }

};
class Matriz{
    public:
        Celda* cabeza;
        Matriz() : cabeza(new Celda()) {}
        ~Matriz() { delete cabeza;}
};
//******************************************** */