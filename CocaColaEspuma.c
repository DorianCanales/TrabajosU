#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define maxPlantasP 50
#define maxPedidos 100
#define maxProductos 100
#define MAXPLANTASD 100
#define MAX 100

struct CocaCola{
    struct PlantaProduccion **arregloPlantasP; // arreglo de plantas de produccion
    struct NodoRegiones *regiones;             // lista de regiones
    struct ArbolClientes *clientes;            // arbol de clientes
    struct NodoProductos *productos;           // lista de productos
    struct GrafoPlantasD **grafoPlantasD;
    char *pais;
    char *direccion;
    char *jefeNacional;
    int id, plibrePP;
};

struct GrafoPlantasD{
        struct Precio **precio;
};

struct Precio{
    int precio;
};

struct PlantaProduccion{
    int id;
    char *nombre;
    struct Region *region;
};

struct NodoRegiones{
    struct Region *region;
    struct NodoRegiones *sig;
};

struct Region{
    char *nombre;
    int idRegion, numeroRegion;
};

struct ArbolClientes{
    struct ArbolClientes *izq, *der;
    struct Cliente *datosCliente;
};

struct Cliente{
    char *nombre;
    int id, cantPedidos;
    struct Pedidos **pedidos;
    struct Region *viveRegion;
};

struct Pedidos{
    int id, cantidadProductos;
    char *fecha;
    struct PlantaProduccion *plantaProduccion;
    struct Producto **productosPedidos;
};

struct NodoProductos{
    struct NodoProductos *sig;
    struct Producto *datosProducto;
};

struct Producto{
    int id, precio;
    char *nombre;
};

//------------------------------------------------------------------------------------



//--------------------- FUNCIONES IMPRIMIR -------------------------------------------


/**
 *
 *          IMPRIMIR PRODUCTOS PEDIDOS ARRAY
 *
 * */

void imprimirProductosArray(struct Producto **productos)
{
    int i;

    for(i = 0; i < maxProductos; i++)
    {
        if(productos[i]!=NULL)
        {
            printf("\t\t\t\t- %s\n", productos[i]->nombre);
        }
    }
}

/**
 *
 *          IMPRIMIR PEDIDOS ARRAY
 *
 * */

void imprimirPedidosArray(struct Pedidos **pedidos)
{
    int i, cont = 0;

    for(i = 0; i<maxPedidos ;i++)
    {
        if(pedidos[i]!=NULL)
        {
            if(pedidos[i]->cantidadProductos>0)
            {
                cont++;
                printf("\t\tPEDIDO NUM = %d", cont);
                printf("\n\t\t\tFecha: %s\n\t\t\tID = %d\n\t\t\tPLANTA DE PRODUCCION ENCARGADA = %s\n\t\t\tCANT. PRODUCTOS = %d\n\t\t\tPRODUCTOS =\n", pedidos[i]->fecha, pedidos[i]->id, pedidos[i]->plantaProduccion->nombre, pedidos[i]->cantidadProductos);
                imprimirProductosArray(pedidos[i]->productosPedidos);
            }
        }
    }
    if(cont == 0)
    {
        printf("\nNO HAY PRODUCTOS REALIZADOS\nPOR FAVOR VERIFIQUE HABER INGRESADO ALMENOS 1 PRODUCTO DEL CLIENTE\n");
    }
}

/**
 *
 *          IMPRIMIR TODAS LOS CLIENTES
 *
 * */

void imprimirClientesPreOrden(struct ArbolClientes *cliente)
{
    if(cliente)
    {
        printf("\n-CLIENTE: %s\n\tPEDIDOS DEL CLIENTE:\n", cliente->datosCliente->nombre);
        if(cliente->datosCliente->cantPedidos == 0){
            printf("\t\t\tNO HAY PEDIDOS\n\n");
        }else{
            imprimirPedidosArray(cliente->datosCliente->pedidos);
        }
        imprimirClientesPreOrden(cliente->izq);
        imprimirClientesPreOrden(cliente->der);
    }
}

/**
 *
 *          IMPRIMIR TODAS LAS PLANTAS DE DISTRIBUCION ARRAY
 *
 * */

void imprimirPlantasDeProduccion(struct PlantaProduccion **PP, int pLibrePP)
{
    int i;
    if(pLibrePP == 0)
    {
        printf("\nNO HAY PLANTAS DE PRODUCCION REGISTRADAS\nPOR FAVOR VERIFIQUE HABER INGRESADO ALMENOS 1 PLANTA DE PRODUCCION\n");
        return;
    }
    for(i = 0; i<pLibrePP ;i++)
    {
        printf("\n- %s\n\tID = %d\n\tREGION %s\n\n", PP[i]->nombre, PP[i]->id,PP[i]->region->nombre);
    }
}

/**
 *
 *          IMPRIMIR TODOS LOS PRODUCTOS DE LA LISTA CIRCULAR
 *
 * */

void imprimirProductosLista(struct NodoProductos *producto)
{
    if(producto == NULL)
    {
        printf("\nNO HAY PRODUCTOS REGISTRADOS\nPOR FAVOR VERIFIQUE HABER REGISTRADO \n");
        return;
    }

    struct NodoProductos *rec = producto;
    do {
        printf("\n-NOMBRE: %s\n\tPRECIO : $%d\n\tID :%d\n\n\n", rec->datosProducto->nombre, rec->datosProducto->precio, rec->datosProducto->id);
        rec = rec->sig;
    }while(rec != producto);
    printf("\n");
}

/**
 *
 *          IMPRIMIR REGIONES LISTA
 *
 * */

void imprimirRegiones(struct NodoRegiones *Region)
{
    Region = Region->sig;

    if(Region == NULL)
    {
        printf("\n                  ERROR!!\nASEGUSERE DE HABER INGRESADO AL MENOS 1 REGION\n");
    }

    while(Region!=NULL){
        printf("\n- Region %s\n\tNumero:%d\n\tID: %d", Region->region->nombre, Region->region->numeroRegion, Region->region->idRegion);
        Region = Region->sig;
    }
}

//------------------------------------------------------------------------------------
/**
 *                  MODIFICAR
 * * CLIENTE                  ARBOL
 * PLANTA DE PRODUCCION     ARRAY (PLIBRE EN CCC)
 * REGIONES                 LISTA SIMPLEMENTE ENLAZADA (NODO FANTASMA)
 * PEDIDOS                  ARRAY (ARBOL CLIENTE)
 * PRODUCTOS                ARRAY (ARBOL CLIENTE)
 * PRODUCTOS                LISTA SIMPLEMENTE ENLAZADA CIRCULAR
 * GRAFO
 *
 * */
//--------------------- FUNCIONES MODIFICAR ------------------------------------------

/**
 *
 *          MODIFICAR EL NOMBRE DE ALGO (PATENTE, NOMBRE, ETC)
 *
 * */

char *modificarLetras()
{
    char *nuevo;
    nuevo = (char *)malloc(sizeof(char));
    scanf("%[^\n]", nuevo);
    return nuevo;
}

/**
 *
 *          MODIFICAR LOS NUMEROS (PRECIO, N°REGION, ETC)
 *
 * */

int modificarNumeros()
{
    int nuevo;
    scanf("%d", &nuevo);
    return nuevo;
}


//------------------------------------------------------------------------------------



//--------------------- FUNCIONES BUSCAR ---------------------------------------------

/**
 *
 *          BUSCAR PLANTA DE PRODUCCION : RETORNO STRUCT CON DATOS
 *
 * */

struct PlantaProduccion *buscarPlantaProduccion(struct PlantaProduccion **arregloPlantasP, int idPlantaP, int plibrePP)
{
    int i;
    for(i=0; i<plibrePP; i++){
        if(arregloPlantasP[i]->id == idPlantaP)
            return arregloPlantasP[i];
    }
    return NULL;
}

/**
 *
 *          BUSCAR REGION : RETORNO STRUCT CON DATOS
 *
 * */

struct Region *buscarRegion (struct NodoRegiones *region , int idRegion){
    struct NodoRegiones *rec = region->sig;
    while (rec != NULL){
        if(rec->region->idRegion == idRegion){
            return rec->region;
        }
        rec = rec->sig;
    }
    return NULL;
}

/**
 *
 *          BUSCAR PRODUCTO : RETORNO STRUCT CON DATOS
 *
 * */

struct Producto *buscarProducto (struct NodoProductos *producto, int idProducto)
{
    struct NodoProductos *rec = producto;
    if(producto){
        do{
            if(rec->datosProducto->id == idProducto){
                return rec->datosProducto;
            }
            rec = rec->sig;
        }while(rec!=producto);
    }
    return NULL;
}

/**
 *
 *          BUSCAR CLIENTE : RETORNO STRUCT CON DATOS
 *
 * */

struct Cliente *buscarCliente(struct ArbolClientes *cliente, struct ArbolClientes **ant, int id)
{
    int enc=0;
    while(!enc && (cliente))
    {
        if(cliente->datosCliente->id == id)
        {
            enc = 1;
        }else
        {
            *ant = cliente;
            if(cliente->datosCliente->id > id)
            {
                cliente = cliente->der;
            }else
            {
                cliente = cliente ->izq;
            }
        }
    }
    if(enc == 0)
        return NULL;
    return cliente->datosCliente;

}

/**
 *
 *          BUSCAR PEDIDO ARRAY: RETORNO STRUCT CON DATOS PEDIDO
 *
 * */

struct Pedidos *buscarPedidoArray(struct Pedidos **pedidos, int idPedido)
{
    int i;
    for(i=0; i < maxPedidos; i++)
    {
        if(pedidos[i]->id == idPedido)
            return pedidos[i];
    }
    return NULL;
}

/**
 *
 *          BUSCAR PRODUCTO ARRAY: RETORNO STRUCT CON DATOS DEL PRODUCTO
 *
 * */

struct Producto *buscarProductoArray(struct Producto **productos, int idProducto)
{
    int i;
    for(i=0; i < maxProductos; i++)
    {
        if(productos[i]->id == idProducto)
            return productos[i];
    }
    return NULL;
}

//------------------------------------------------------------------------------------




//--------------------- FUNCIONES ELIMINAR -------------------------------------------

/**
 *
 *          ELIMINAR REGION LISTA
 *
 * */

void eliminarRegion (struct NodoRegiones **region, struct Region *regionBuscada){
    if((*region)->sig->region == regionBuscada)
    {
        (*region)->sig = (*region)->sig->sig;
        return;
    }else
    {
        struct NodoRegiones *rec = (*region);
        rec = rec->sig;
        while (rec != NULL){
            if(rec->sig->region == regionBuscada)
            {
                rec->sig = rec->sig->sig;
                return;
            }
            rec = rec->sig;
        }
    }
    return;
}

/**
 *
 *          ELIMINAR PRODUCTO LISTA CIRCULAR
 *
 * */

void eliminarProducto(struct NodoProductos **producto, struct Producto *productoBuscado)
{
    struct NodoProductos *rec = (*producto);
    do
    {
        if((*producto)->sig->datosProducto != productoBuscado)
            (*producto) = (*producto)->sig;
    } while((*producto)->sig->datosProducto != productoBuscado && (*producto) != rec);
    if((*producto)->sig->datosProducto == productoBuscado)
    {
        if((*producto) == (*producto)->sig)
        {
            (*producto) = NULL;
        }
        else
        {
            rec = (*producto)->sig;
            (*producto)->sig = rec->sig;
        }
    }
}

/**
 *
 *          ELIMINAR CLIENTE
 *
 * */

void reemplazarCliente(struct ArbolClientes **cliente, struct ArbolClientes **aux)
{
    if(!((*cliente)->der))
    {
        (*aux)->datosCliente = (*cliente)->datosCliente;
        *aux = *cliente;
        *cliente = (*cliente)->izq;
    }else
    {
        reemplazarCliente(&(*cliente)->der, &(*aux));
    }
}

void eliminarCliente(struct ArbolClientes **cliente, int id)
{
    struct ArbolClientes *aux=NULL;
    if(!(*cliente))
        return;
    if((*cliente)->datosCliente->id < id)
        eliminarCliente(&(*cliente)->der, id);
    else
    {
        if((*cliente)->datosCliente->id > id)
            eliminarCliente(&(*cliente)->izq, id);
        else
        {
            if((*cliente)->datosCliente->id == id)
            {
                aux = *cliente;
                if(!((*cliente)->izq))
                    *cliente = (*cliente)->der;
                else
                {
                    if(!((*cliente)->der))
                        *cliente = (*cliente)->izq;
                    else
                        reemplazarCliente(&(*cliente)->izq, &aux);
                }
            }
        }
    }
}

/**
 *
 *          ELIMINAR PEDIDO ARRAY
 *
 * */

void eliminarPedidoArray(struct Pedidos **pedidosArray, struct Pedidos *pedido, int *cantPedidos){
    int i;
    for(i = 0 ; i < maxPedidos; i++)
    {
            if(pedidosArray[i] == pedido)
            {
                pedidosArray[i] = NULL;
                (*cantPedidos)--;
                printf("\nELIMINACION EXITOSA\n");
                return;
            }
    }
    return;
}

/**
 *
 *          ELIMINAR PRODUCTOS ARRAY
 *
 * */

void eliminarProductosArray(struct Producto **productoArray, struct Producto *producto){
    int i;
    for(i = 0 ; i < maxProductos; i++)
    {
        if(productoArray[i] != NULL){
            if(productoArray[i] == producto)
            {
                productoArray[i] = NULL;
            }
        }
    }
    printf("\nELIMINACION EXITOSA\n");
    return;
}

/**
 *
 *          ELIMINAR PLANTA DE PRODUCCION
 *
 * */

void eliminarPlantaDeProduccion(struct PlantaProduccion **plantasDeProduccion, struct PlantaProduccion *plantaDeProduccionEliminar, int *pLibrePP){
    int i;
    for(i = 0 ; i < (*pLibrePP); i++){
        if(plantasDeProduccion[i]==plantaDeProduccionEliminar)
        {
            plantasDeProduccion[i]=plantasDeProduccion[(*pLibrePP)-1];
            plantasDeProduccion[(*pLibrePP)-1] = NULL;
            (*pLibrePP)--;
            return;
        }
    }
}

//------------------------------------------------------------------------------------



//--------------------- FUNCIONES AGREGAR --------------------------------------------

/**
 *
 *          AGREGAR PRODUCTO LISTA CIRCULAR
 *
 * */

void agregarProducto(struct NodoProductos **productos , struct NodoProductos *nuevoProducto){
    if(!nuevoProducto)
    {
        printf("\nHA OCURRIDO UN ERROR CON EL NUEVO PRODUCTO\n NO EXISTE!\n");
        return;
    }
    if((*productos) == NULL)
    {
        (*productos) = nuevoProducto;
    }
    else
    {
        nuevoProducto->sig = (*productos)->sig;
    }
    (*productos)->sig = nuevoProducto;
}

/**
 *
 *          AGREGAR CLIENTE
 *
 * */

void insertarCliente(struct ArbolClientes **cliente, struct Cliente *datosNuevoCliente)
{
    if(!datosNuevoCliente)
    {
        printf("\nHA OCURRIDO UN ERROR CON EL NUEVO CLIENTE\nPOR FAVOR INTENTELO MAS TARDE\n");
        return;
    }
    if(!(*cliente))
    {
        (*cliente) = (struct ArbolClientes *) malloc(sizeof(struct ArbolCliente *));
        (*cliente)->datosCliente = datosNuevoCliente;
        (*cliente)->izq = NULL;
        (*cliente)->der = NULL;
    }else
    {
        if((*cliente)->datosCliente->id != datosNuevoCliente->id)
        {
            if((*cliente)->datosCliente->id > datosNuevoCliente->id)
                insertarCliente(&((*cliente)->der), datosNuevoCliente);
            else
            {
                insertarCliente(&((*cliente)->izq), datosNuevoCliente);
            }
        }
    }
}

/**
 *
 *          AGREGAR REGION
 *
 * */

void agregarRegion(struct NodoRegiones **regiones , struct NodoRegiones *nuevo){
    struct NodoRegiones *rec = *regiones;

    while(rec != NULL)
    {
        if(rec->sig==NULL)
        {
            rec->sig=nuevo;
            rec=nuevo;
        }
        rec = rec->sig;
    }
    printf("\nSE HA AÑADIDO CORRECTAMENTE LA REGION\n");
}

/**
 *
 *          AGREGAR PEDIDO
 *
 * */

void agregarPedido(struct Pedidos **pedidosArray , struct Pedidos *nuevo, int *cantPedidos){
    int i;
    if(!nuevo)
    {
        printf("\nHA OCURRIDO UN ERROR CON EL NUEVO PEDIDO\nPOR FAVOR INTENTELO MAS TARDE\n");
        return;
    }else
    {
        for(i = 0 ; i < maxPedidos ; i++){
            if(pedidosArray[i] == NULL){
                (*cantPedidos) = (*cantPedidos) + 1;
                pedidosArray[i] = nuevo;
                return;
            }
        }
    }
}

/**
 *
 *          AGREGAR PRODUCTOS ARRAY
 *
 * */

void agregarProductoArray(struct Producto **productosArray , struct Producto *nuevo){
    int i;
    if(!nuevo)
    {
        printf("\nHA OCURRIDO UN ERROR CON EL NUEVO PEDIDO\nPOR FAVOR INTENTELO MAS TARDE\n");
        return;
    }
    for(i = 0 ; i < maxPedidos ; i++){
        if(productosArray[i] == NULL){
            productosArray[i] = nuevo;
            return;
        }
    }
    printf("\nHAS LLEGADO AL LIMITE DE PRODUCTOS POR PEDIDO\n NO SE HA PODIDO AGREGAR MAS PRODUCTOS AL PEDIDO\n");
}

/**
 *
 *          AGREGAR PLANTA DE PRODUCCION ARRAY
 *
 * */

void agregarPlantaProduccion(struct PlantaProduccion **plantasDeProduccion, struct PlantaProduccion *nuevo, int *pLibrePP){
    if(!nuevo)
    {
        return;
    }
    int i;
    if((*pLibrePP)<maxPlantasP)
    {
        plantasDeProduccion[(*pLibrePP)] = nuevo;
        (*pLibrePP)++;
    }else
    {
        printf("\nHAS LLEGADO AL LIMITE DE PLANTAS DE PRODUCCION POR PAIS\n NO SE HA PODIDO AGREGAR MAS PLANTAS DE PRODICCION\n");
    }
}

//------------------------------------------------------------------------------------



//--------------------- FUNCIONES CREAR ----------------------------------------------

/**
 *
 *          CREAR REGION
 *
 * */

struct NodoRegiones *crearRegion(struct NodoRegiones *regionHead)
{
    struct NodoRegiones *nuevaRegion;
    nuevaRegion = ((struct NodoRegiones*)malloc(sizeof(struct NodoRegiones)));
    nuevaRegion->region = (struct Region*)malloc(sizeof(struct Region));
    printf("\nIndique los datos del producto que desea agregar:\nNOMBRE (UNICA PALABRA): ");
    nuevaRegion->region->nombre =(char*)malloc(sizeof(char));
    scanf("%s",nuevaRegion->region->nombre);
    do
    {
        printf("ID: ");
        scanf("%d", &nuevaRegion->region->idRegion);
        if(nuevaRegion->region->idRegion < 0 || (buscarRegion(regionHead, nuevaRegion->region->idRegion))!=NULL)
        {
            printf("\nERROR!!!\n\nIngrese una ID valida\n");
        }
    }while(nuevaRegion->region->idRegion < 0 || (buscarRegion(regionHead, nuevaRegion->region->idRegion))!=NULL);

    printf("NUMERO DE LA REGION :  ");

    do
    {
        scanf("%d", &nuevaRegion->region->numeroRegion);
        if(nuevaRegion->region->numeroRegion < 0)
        {
            printf("\nERROR!!!\n\nIngrese un PRECIO valido\n");
        }
    }while(nuevaRegion->region->numeroRegion < 0);

    nuevaRegion->sig = NULL;
    return nuevaRegion;
}

/**
 *
 *          CREAR CLIENTE
 *
 * */

struct Cliente *crearCliente(struct NodoRegiones *region, struct ArbolClientes *clientes)
{
    int id,i;
    struct Cliente *nuevo;
    struct ArbolClientes *ant = clientes;
    nuevo = (struct Cliente *)malloc(sizeof(struct Cliente));
    nuevo->nombre = (char*)malloc(sizeof(char));
    printf("Escriba el nombre del usuario : ");
    scanf("%s",nuevo->nombre);
    do
    {
        printf("INGRESE EL RUT DEL CLIENTE (SI SU DIGITO VERIFICADOR TERMINA EN K REMPLACE POR UN 0):");
        scanf("%d", &nuevo->id);
        if(nuevo->id < 0 || (buscarCliente(clientes, &ant, nuevo->id)!=NULL))
        {
            printf("HA OCURRIDO UN ERROR, VERIFIQUE QUE LA ID SEA MAYOR A 0 Y NUMEROS SOLAMENTE\n");
        }
    }while(nuevo->id < 0 || (buscarCliente(clientes, &ant, nuevo->id)!=NULL));

    printf("\nID REGION PERTENECIENTE: ");
    do
    {
        scanf("%d", &id);
        if(id < 0)
        {
            printf("HA OCURRIDO UN ERROR, VERIFIQUE QUE LA ID SEA MAYOR A 0 Y NUMEROS SOLAMENTE\n");
        }
    }while(id < 0);

    if(buscarRegion(region, id) == NULL)
    {
        printf("\nHA OCURRIDO UN ERROR, ID NO ENCONTRADA\nVerifica tener Regiones y haber ingresado una id de Region\n");
        return NULL;
    }else
    {
        nuevo->viveRegion = buscarRegion(region, id);
    }

    nuevo->pedidos = (struct Pedidos **)malloc(maxPedidos * sizeof(struct Pedidos *));
    for(i = 0; i<maxPedidos;i++)
    {
        nuevo->pedidos[i] = NULL;
    }
    nuevo->cantPedidos = 0;
    return nuevo;
}

/**
 *
 *          CREAR PLANTA DE PRODUCCION ARRAY
 *
 * */

struct PlantaProduccion *crearPP(struct CocaCola *CCC)
{
    int id;
    struct PlantaProduccion *nuevo;
    nuevo = (struct PlantaProduccion *)malloc(sizeof(struct PlantaProduccion ));
    nuevo->nombre = (char *)malloc(sizeof(char));
    printf("NOMBRE PLANTA DE PRODUCCION:");
    scanf("%s",nuevo->nombre);

    do
    {
        printf("\nID DE LA PLANTA DE PRODUCCION:");
        scanf("%d", &nuevo->id);
        if(nuevo->id < 0 || (buscarPlantaProduccion(CCC->arregloPlantasP, nuevo->id, CCC->plibrePP)))
        {
            printf("\nERROR!!!\n\nINGRESE UNA ID VALIDA POR FAVORn\n");
        }
    }while(nuevo->id < 0 || buscarPlantaProduccion(CCC->arregloPlantasP, nuevo->id, CCC->plibrePP));

    do
    {
        printf("\nREGION PERTENECIENTE :" );
        scanf("%d", &id);
        if(id < 0)
        {
            printf("\nERROR!!!\n\nINGRESE UNA ID VALIDA POR FAVORn\n");
        }
    }while(id < 0 );
    nuevo->region = buscarRegion(CCC->regiones, id);
    if(nuevo->region == NULL)
    {
        printf("\nHA OCURRIDO UN ERROR CON LA REGION\n NO EXISTE O NO SE PUDO ENCONTRAR\nPOR FAVOR, INTENTELO MAS TARDE\n");
        return NULL;
    }
    return nuevo;
}

/**
 *
 *          CREAR PRODUCTOS LISTA CIRCULAR
 *
 * */

struct NodoProductos *crearProducto(struct NodoProductos *productos)
{
    struct NodoProductos *nueva;
    nueva = ((struct NodoProductos*)malloc(sizeof(struct NodoProductos)));
    nueva->datosProducto = (struct Producto *)malloc(sizeof(struct Producto));
    nueva->datosProducto->nombre = (char *)malloc(sizeof(char));
    printf("\nIndique los datos del producto que desea agregar:\nNOMBRE: ");
    scanf("%s", nueva->datosProducto->nombre);
    printf("ID: ");
    do
    {
        scanf("%d", &nueva->datosProducto->id);
        if(nueva->datosProducto->id < 0 || buscarProducto(productos,nueva->datosProducto->id))
        {
            printf("\nERROR!!!\n\nIngrese una ID valida\n");
        }
    }while(nueva->datosProducto->id < 0 || buscarProducto(productos,nueva->datosProducto->id));


    do
    {
        printf("PRECIO : $ ");
        scanf("%d", &nueva->datosProducto->precio);
        if(nueva->datosProducto->precio < 0)
        {
            printf("\nERROR!!!\n\nIngrese un PRECIO valido\n");
        }
    }while(nueva->datosProducto->precio < 1);

    nueva->sig = NULL;
    return nueva;
}

/**
 *
 *          CREAR PEDIDO ARRAY
 *
 * */

struct Pedidos *crearPedido(struct CocaCola *CCC)
{
    int id, ciclo1;
    struct Pedidos *nuevo;
    struct Producto *productoAgregar;
    nuevo = (struct Pedidos *)malloc(sizeof(struct Pedidos));
    printf("\nID DEL PEDIDO: ");
    do
    {
        scanf("%d", &nuevo->id);
        printf("\n");
        if(nuevo->id < 0)
        {
            printf("\nERROR!!!\n\nIngrese una ID valida\n");
        }
    }while(nuevo->id < 0);

    printf("\nINGRESE LA PLANTA DE PRODUCCION ENCARGADA :");
    do
    {
        scanf("%d", &id);
        printf("\n");
        if(id < 0)
        {
            printf("\nERROR!!!\n\nIngrese una ID valida\n");
        }
    }while(id < 0);
    nuevo->plantaProduccion = buscarPlantaProduccion(CCC->arregloPlantasP,id,CCC->plibrePP);
    if(nuevo->plantaProduccion == NULL)
    {
        printf("\nHA OCURRIDO UN ERROR CON LA ID DE LA PLANTA DE PRODUCCION\nPOR FAVOR INTENTE MAS TARDE\n");
        return NULL;
    }

    nuevo->fecha = (char *)malloc(50 * sizeof(char *));
    printf("\nINGRESE LA FECHA DEL PEDIDO: ");
    scanf("%s",nuevo->fecha);

    nuevo->cantidadProductos = 0;
    nuevo->productosPedidos = (struct Producto **)malloc(maxProductos * sizeof(struct Producto*));
    for(int i=0; i<maxProductos;i++)
    {
        nuevo->productosPedidos[i] = NULL;
    }
    ciclo1 = 1;
    while(ciclo1 != 0 || nuevo->cantidadProductos > maxProductos) {
        do
        {
            printf("\nINGRESE LA ID DEL PRODUCTO\n\n");
            scanf("%d", &id);
            if(id < 0)
            {
                printf("\nERROR!!!\n\nIngrese una ID valida\n");
            }
        }while(id < 0);
        productoAgregar = buscarProducto(CCC->productos, id);
        if(productoAgregar == NULL)
        {
            printf("\nHA INGRESADO UNA ID NO VALIDA\n");
        }else
        {
            if(nuevo->cantidadProductos <= maxProductos)
            {
                nuevo->cantidadProductos = (nuevo->cantidadProductos) + 1;
                agregarProductoArray(nuevo->productosPedidos, productoAgregar);
            }
            else{
                printf("\nYA HA INGRESADO EN N° MAXIMO DE PRODUCTOS SOLICITADOS\n");
                return nuevo;
            }
        }

        printf("\nQUIERE AGREGAR ALGO MAS?\n1 = SI\n0 = NO\n");
        do
        {
            scanf("%d", &ciclo1);
            printf("\n");
            if(ciclo1 < 0 || ciclo1 > 1)
            {
                printf("\nERROR!!!\n\n1 = SI\n 0 = NO\n");
            }
        }while(ciclo1 < 0 || ciclo1 > 1);
    }
    return nuevo;
}

/**
 *
 *          CREAR GRAFO
 *
 * */

void llenarGrafosConPrecios(struct GrafoPlantasD **matriz)
{
    int i, j;

    for(i = 0 ; i < MAXPLANTASD ; i++){
        matriz[i] = (struct GrafoPlantasD *)malloc(sizeof(struct GrafoPlantasD));
        matriz[i]->precio = (struct Precio **)malloc(MAXPLANTASD * sizeof(struct Precio*));
        for(j=0;j<MAXPLANTASD;j++)
            matriz[i]->precio[j] = (struct Precio *)malloc( sizeof(struct Precio));
    }

    srand(time(0));
    for (i = 0; i < MAXPLANTASD; i++)
    {
        for (j = 0; j < MAXPLANTASD; j++)
        {
            if (j == i)
            {
                matriz[i]->precio[j]->precio = 0;
            }
            else
            {
                matriz[i]->precio[j]->precio = 1 + rand() % (100000 + 1 - 1);
                matriz[j]->precio[i]->precio = matriz[i]->precio[j]->precio;
            }
        }
    }
}

void mostrarGrafos(struct GrafoPlantasD **matriz){
    int i , j;
    for (i = 0; i < MAXPLANTASD; i++)
    {
        for (j = 0; j < MAXPLANTASD; j++){
            printf("%d\t" , matriz[i]->precio[j]->precio);
        }
        printf("\n");
    }
}

//------------------------------------------------------------------------------------



//--------------------- FUNCIONES ESPECIFICAS ----------------------------------------

/**
 *
 *          FUNCION CLIENTE CON MAS DINERO GASTADO
 *
 * */

int cantidadDeProductosComprados(struct Producto **productosArray){
    int i , sumaPrecio = 0;
    for(i = 0; i < maxProductos ; i++){
        if(productosArray[i]!= NULL){
            sumaPrecio += productosArray[i]->precio;
        }
    }
    return sumaPrecio;
}

int gastoTotalDeUnCliente (struct Pedidos **pedidosArray){
    int i , cont = 0;
    for (i = 0 ; i < maxPedidos ; i++){
        if(pedidosArray[i] != NULL){
            cont += cantidadDeProductosComprados(pedidosArray[i]->productosPedidos);
        }
    }
    return cont;
}

void clienteConMasDineroGastado(struct ArbolClientes *abb, struct Cliente **mayor, int *mayorCantidad){
    if(abb){
        if(gastoTotalDeUnCliente(abb->datosCliente->pedidos)>(*mayorCantidad))
        {
            (*mayorCantidad) = gastoTotalDeUnCliente(abb->datosCliente->pedidos);
            (*mayor) = abb->datosCliente;
        }
        clienteConMasDineroGastado(abb->izq, &(*mayor), &(*mayorCantidad));
        clienteConMasDineroGastado(abb->der, &(*mayor), &(*mayorCantidad));
    }
}

/**
 *
 *          FUNCION REGION QUE MAS CONSUME
 *
 * */

int contarProductos(struct Producto **productosArray){
    int i , contador = 0;
    for(i = 0; i < maxProductos ; i++)
    {
        if(productosArray[i]!= NULL)
        {
            contador++;
        }
    }
    return contador;
}

int contarProductosCliente(struct Pedidos **pedidosArray){
    int i , contador = 0;
    for (i = 0 ; i < maxPedidos ; i++){
        if(pedidosArray[i] != NULL){
            contador += contarProductos(pedidosArray[i]->productosPedidos);
        }
    }
    return contador;
}

void contarClientesRegion(struct ArbolClientes *cliente, int *cantidadClientes, struct Region *region){
    if(cliente){
        if(cliente->datosCliente->viveRegion == region)
        {
            (*cantidadClientes) += contarProductosCliente(cliente->datosCliente->pedidos);
        }
        contarClientesRegion(cliente->izq, &(*cantidadClientes), region);
        contarClientesRegion(cliente->der, &(*cantidadClientes), region);
    }
}

struct Region *regionConsumidora(struct ArbolClientes *cliente, struct NodoRegiones *regiones,int *MayCantConsumidaRegion)
{
    struct Region *regionConsumidora = NULL;
    int mayor = 0;
    while(regiones!=NULL)
    {
        mayor = 0;
        contarClientesRegion(cliente,&mayor,regiones->region);
        if(mayor > (*MayCantConsumidaRegion))
        {
            (*MayCantConsumidaRegion) = mayor;
            regionConsumidora = regiones->region;
        }
        regiones = regiones->sig;
    }
    return regionConsumidora;
}

/**
 *
 *          FUNCION PRODUCTOS TOTAL VENDIDOS
 *
 * */

int productosTotalVendidos(struct ArbolClientes *abb){
    if(abb){
        return contarProductosCliente(abb->datosCliente->pedidos) + productosTotalVendidos(abb->izq) +productosTotalVendidos(abb->der);
    }
    return 0;
}

/**
 *
 *          FUNCION PRODUCTOS MAS VENDIDO
 *
 * */

int contarProductosEspecifico(struct Producto **productosArray, struct Producto *productoBuscado)
{
    int i , contador = 0;
    for(i = 0; i < maxProductos ; i++)
    {
        if(productosArray[i]!= NULL)
        {
            if(productosArray[i] == productoBuscado)
            {
                contador++;
            }
        }
    }
    return contador;
}

int contarProductosEspecificoCliente(struct Pedidos **pedidosArray, struct Producto *productoBuscado){
    int i , contador = 0;
    for (i = 0 ; i < maxPedidos ; i++){
        if(pedidosArray[i] != NULL){
            contador += contarProductosEspecifico(pedidosArray[i]->productosPedidos, productoBuscado);
        }
    }
    return contador;
}

int productosEspecificoVendidoClientes(struct ArbolClientes *cliente, struct Producto *productoBuscado){
    if(cliente){
        return contarProductosEspecificoCliente(cliente->datosCliente->pedidos, productoBuscado) + productosEspecificoVendidoClientes(cliente->izq, productoBuscado) +productosEspecificoVendidoClientes(cliente->der, productoBuscado);
    }
    return 0;
}

struct Producto *productoMasVendido(struct ArbolClientes *cliente, struct NodoProductos *producto)
{
    int mayor=0, prueba=0;
    struct NodoProductos *recProducto = producto;
    struct Producto *mayorProducto = NULL;
    if(producto)
    {
        do
        {
            prueba = productosEspecificoVendidoClientes(cliente, recProducto->datosProducto);
            if(prueba>mayor)
            {
                mayor = prueba;
                mayorProducto = recProducto->datosProducto;
            }
            recProducto = recProducto->sig;
        }while(recProducto!=producto);
    }
    return mayorProducto;
}

//------------------------------------------------------------------------------------



//--------------------- FUNCION DE ORDENAMIENTO ----------------------------

/**
 *
 *          ORDENAR PLANTAS DE PRODUCCION (SHELL)
 *
 * */

void ordenarPlantaProduccion(struct PlantaProduccion **arreglo, int plibre)
{
    int i, j, k, salto;
    struct PlantaProduccion *aux;
    salto = plibre / 2;
    while (salto > 0)
    {
        for (i = salto; i < plibre; i++)
        {
            j = i - salto;
            while (j >= 0)
            {
                k = j + salto;
                if (arreglo[j]->id <= arreglo[k]->id)
                    j = -1;
                else
                {
                    aux = arreglo[j];
                    arreglo[j] = arreglo[k];
                    arreglo[k] = aux;
                    j = j - salto;
                }
            }
        }
        salto = salto / 2;
    }
}

//------------------------------------------------------------------------------------

/**
 *
 *          OBTENER COSTO DE ORIGEN Y DESTINO CON GRAFO
 *
 * */

int dijkstra(int desde, int hasta, struct GrafoPlantasD **matriz)
{
    int i, j, posision[10000], ruta[10000], pos, min;
    for (i = 0; i < MAXPLANTASD; i++)
    {
        ruta[i] = 0;
        posision[i] = matriz[desde]->precio[i]->precio;    // Primero asuma la distancia desde el punto fuente a otros puntos
    }
    for (i = 0; i < MAXPLANTASD; ++i)
    {
        min = 99999999;  // registra la LAN mínima [i]
        for (j = 0; j < MAXPLANTASD; ++j)
        {
            if (!ruta[j] && min > posision[j])
            {
                pos = j;
                min = posision[j];
            }
        }
        ruta[pos] = 1;
        for (j = 0; j < MAXPLANTASD; ++j)
        {
            if (!ruta[j] && (posision[j] > (posision[pos] + matriz[pos]->precio[j]->precio)))
            {
                posision[j] = posision[pos] + matriz[pos]->precio[j]->precio;
            }
        }
    }
    return posision[hasta];
}
/**
 *
 *      FUNCION CREAR OBJETOS
 *
 * */

void crearObjeto(struct CocaCola **CCC)
{
    int opcion, id;
    printf("\nQUE DESEA CREAR?\n1 = REGION\n2 = PLANTA DE PRODUCCION\n3 = CLIENTE\n4 = PRODUCTO\n"
           "5 = PEDIDO\n0 = CANCELAR\n\n\n");
    scanf("%d",&opcion);
    switch (opcion) {
        case 1:
        {
            agregarRegion(&((*CCC)->regiones), crearRegion((*CCC)->regiones));
            return;
        }
        case 2:
        {
            agregarPlantaProduccion((*CCC)->arregloPlantasP, crearPP((*CCC)), &((*CCC)->plibrePP));
            ordenarPlantaProduccion((*CCC)->arregloPlantasP,(*CCC)->plibrePP);
            return;
        }
        case 3:
        {
            struct Cliente *auxCliente;
            auxCliente = crearCliente((*CCC)->regiones, (*CCC)->clientes);
            if(!auxCliente)
            {
                printf("\nNO SE HA PODIDO CREAR AL USUARIO\nPOR FAVOR VERIFIQUE LOS DATOS\n");
                return;
            }
            insertarCliente(&((*CCC)->clientes), auxCliente);
            printf("\nCLIENTE CREADO EXITOSAMENTE\n");
            return;
        }
        case 4:
        {
            agregarProducto(&((*CCC)->productos), crearProducto((*CCC)->productos));
            printf("\nHA SIDO AGREGADA CORRECTAMENTE\n");
            return;
        }
        case 5:
        {
            struct ArbolClientes *ant = ((*CCC)->clientes);
            struct Cliente *cliente = NULL;
            do
            {
                printf("\nINGRESE EL RUT DEL CLIENTE AL QUE QUIERE AGREGARLE UN PEDIDO (RECUERDE SI SU DIGITO TERMINA EN K, REMPLACELO POR UN 0) :");
                scanf("%d", &id);
                cliente = buscarCliente((*CCC)->clientes, &ant, id);
                if(id < 0 || cliente==NULL)
                {
                    printf("\nERROR!!!\n\nIngrese una ID valida\n");
                }
            }while(id < 0 || cliente==NULL);
            if(cliente)
            {
                if(id < ant->datosCliente->id)
                {
                    ant = ant->der;
                }else if(id > ant->datosCliente->id)
                {
                    ant = ant->izq;
                }
                if(ant->datosCliente->id == id)
                {
                    agregarPedido(ant->datosCliente->pedidos, crearPedido((*CCC)), &(ant->datosCliente->cantPedidos));
                }else
                {
                    printf("\nEL CLIENTE NO EXISTE\nPOR FAVOR INTENTE MAS TARDE\n");
                    return;
                }
                printf("\nHA SIDO AGREGADA CORRECTAMENTE\n");
            }else{
                printf("\n\n\n\nHA OCURRIDO UN ERROR CON EL ID DEL CLIENTE INGRESADO\n\n\n");
            }

            return;
        }
        case 0:
        {
            printf("\nCANCELADO\n");
            return;
        }
        default:
        {
            printf("\nHA OCURRIDO UN ERROR CON EL NUMERO, INTENTELO MAS TARDE :D GRACIAS\n");
            return;
        }
    }
}

/**
 *
 *      FUNCION BUSCAR OBJETOS
 *
 * */

void buscarObjeto(struct CocaCola **CCC)
{
    char buffer[MAX];
    int opcion1, opcion2, opcion3, opcion4, opcion5, ciclo1, id, i;
    struct Region *auxRegion = NULL;
    struct Producto *auxProducto = NULL;
    struct PlantaProduccion *auxPP = NULL;
    struct Cliente *auxCliente = NULL;
    struct NodoRegiones *modRegion = (*CCC)->regiones;
    struct NodoProductos *modProducto = (*CCC)->productos;
    struct ArbolClientes *anteriorCliente = (*CCC)->clientes;
    printf("\nQue desea buscar?\n1 = REGION\n2 = PLANTA DE PRODUCCION\n3 = CLIENTE\n4 = PRODUCTO\n"
           "5 = COCA-COLA COMPANY\n0 = CANCELAR\n");
    scanf("%d", &opcion1);
    switch (opcion1){
        case 1:
        {
            if((*CCC)->regiones->sig == NULL)
            {
                printf("\n\n\n\n\n------NO HA INGRESADO REGIONES AUN------\n\n\n\n\n");
            }else{
                printf("\nQUE DESEA HACER EN LAS REGIONES?\n1 = MOSTRAR DATOS DE LAS REGIONES\n2 = MODIFICAR UNA REGION\n3 = ELIMINAR UNA REGION\n0 = CANCELAR\n");
                scanf("%d", &opcion2);
                switch (opcion2) {
                    case 1:
                    {
                        imprimirRegiones((*CCC)->regiones);
                        return;
                    }
                    case 2:
                    {
                        printf("\nINGRESE LA ID DE LA REGION QUE DESEA MODIFICAR:");
                        do{
                            scanf("%d", &id);
                            printf("\n");
                            if(id<0 || !buscarRegion((*CCC)->regiones, id))
                            {
                                printf("\nHA OCURRIDO UN ERROR CON LA ID INGRESADA\nPOR FAVOR, INTENTE NUEVAMENTE\n");
                            }
                        }while(id<0 && !buscarRegion((*CCC)->regiones, id));
                        auxRegion = buscarRegion((*CCC)->regiones, id);
                        modRegion = modRegion->sig;
                        while(modRegion->region != auxRegion)
                        {
                            modRegion = modRegion->sig;
                        }
                        printf("\nQue deseas modificar?\n1 = NOMBRE\n2 = N°REGION\n0 = CANCELAR\n");
                        scanf("%d",&opcion3);
                        switch (opcion3) {
                            case 1:
                            {
                                printf("\nIngrese el nombre nuevo que le quiere colocar a la REGION:");
                                scanf("%s",buffer);
                                strcpy(modRegion->region->nombre,buffer);
                                printf("\n");
                                return;
                            }
                            case 2:
                            {
                                printf("\nIngrese el nuevo N° de REGION\n");
                                modRegion->region->numeroRegion = modificarNumeros();
                                return;

                            }
                            case 0:
                            {
                                printf("\nCANCELADO\n");
                                return;
                            }
                            default:
                            {
                                printf("\nHA OCURRIDO UN ERROR CON LA OPCION INDICADA\n");
                                return;
                            }
                        }
                        return;
                    }
                    case 3:
                    {
                        printf("\nINGRESE LA ID DE LA REGION QUE DESEA MODIFICAR: ");
                        do{
                            scanf("%d", &id);
                            printf("\n");
                            if(id<0 || !buscarRegion((*CCC)->regiones, id))
                            {
                                printf("\nHA OCURRIDO UN ERROR CON LA ID INGRESADA\nPOR FAVOR, INTENTE NUEVAMENTE\n");
                            }
                        }while(id<0 && buscarRegion((*CCC)->regiones, id));
                        auxRegion = buscarRegion((*CCC)->regiones, id);
                        eliminarRegion(&((*CCC)->regiones), auxRegion);
                        return;
                    }
                    case 0:
                    {
                        printf("\nCANCELADO\n");
                        return;
                    }
                    default:
                    {
                        printf("\nHA OCURRIDO UN ERROR CON EL NUMERO INGRESADO, POR FAVOR INTENTELO MAS TARDE\n");
                        return;
                    }
                }
            }

            return;
        }
        case 2:
        {
            if((*CCC)->plibrePP ==0)
            {
                printf("\n\n\n\n\n------NO HA INGRESADO NINGUNA PLANTA DE PRODUCCION------\n\n\n\n\n");
            }else
            {
                printf("\nQUE DESEA HACER EN LAS PLANTA DE PRODUCCION?\n1 = MOSTRAR DATOS DE LAS PLANTAS\n2 = MODIFICAR UNA PLANTA PRODUCTORA\n3 = ELIMINAR UNA PLANTA PRODUCTORA\n0 = CANCELAR\n");
                scanf("%d", &opcion2);
                switch (opcion2) {
                    case 1:
                    {
                        imprimirPlantasDeProduccion((*CCC)->arregloPlantasP, (*CCC)->plibrePP);
                        return;
                    }
                    case 2:
                    {
                        printf("\nINGRESE LA ID DE LA PLANTA PRODUCTORA QUE DESEA MODIFICAR:");
                        do{
                            scanf("%d", &id);
                            printf("\n");
                            if(id<0 || !buscarPlantaProduccion((*CCC)->arregloPlantasP, id,(*CCC)->plibrePP))
                            {
                                printf("\nHA OCURRIDO UN ERROR CON LA ID INGRESADA\nPOR FAVOR, INTENTE NUEVAMENTE\n");
                            }
                        }while(id<0 && !buscarPlantaProduccion((*CCC)->arregloPlantasP, id,(*CCC)->plibrePP));
                        auxPP = buscarPlantaProduccion((*CCC)->arregloPlantasP, id,(*CCC)->plibrePP);
                        for(i=0;i < (*CCC)->plibrePP; i++)
                        {
                            if((*CCC)->arregloPlantasP[i] == auxPP)
                            {
                                printf("\nQue deseas modificar?\n1 = NOMBRE\n2 = REGION\n0 = CANCELAR\n");
                                scanf("%d",&opcion3);
                                switch (opcion3) {
                                    case 1:
                                    {
                                        printf("\nIngrese el nombre nuevo que le quiere colocar a la planta:");
                                        scanf("%s", buffer);
                                        strcpy((*CCC)->arregloPlantasP[i]->nombre,buffer);
                                        printf("\n\n\n\n----- NOMBRE CAMBIADO CORRECTAMENTE ------\n\n\n\n\n");
                                        return;
                                    }
                                    case 2:
                                    {
                                        do{
                                            printf("\nINGRESE LA ID DE LA REGION A DONDE QUIERE MOVER LA PLANTA: ");
                                            scanf("%d", &id);
                                            if(id<0 || !buscarRegion((*CCC)->regiones, id))
                                            {
                                                printf("\nHA OCURRIDO UN ERROR CON LA ID INGRESADA\nPOR FAVOR, INTENTE NUEVAMENTE\n");
                                            }
                                        }while(id<0 && !buscarRegion((*CCC)->regiones, id));
                                        (*CCC)->arregloPlantasP[i]->region = buscarRegion((*CCC)->regiones, id);
                                        return;
                                    }
                                    case 0:
                                    {
                                        printf("\nCANCELADO\n");
                                        return;
                                    }
                                    default:
                                    {
                                        printf("\nHA OCURRIDO UN ERROR CON LA OPCION INDICADA\n");
                                        return;
                                    }
                                }
                            }
                        }
                        return;
                    }
                    case 3:
                    {
                        printf("\nINGRESE LA ID DE LA PLANTA DE PRODUCCION QUE DESEA ELIMINAR: ");
                        do{
                            scanf("%d", &id);
                            printf("\n");
                            if(id<0 || !buscarPlantaProduccion((*CCC)->arregloPlantasP, id,(*CCC)->plibrePP))
                            {
                                printf("\nHA OCURRIDO UN ERROR CON LA ID INGRESADA\nPOR FAVOR, INTENTE NUEVAMENTE\n");
                            }
                        }while(id<0 && !buscarPlantaProduccion((*CCC)->arregloPlantasP, id,(*CCC)->plibrePP));
                        auxPP = buscarPlantaProduccion((*CCC)->arregloPlantasP, id,(*CCC)->plibrePP);
                        for(i=0;i < (*CCC)->plibrePP; i++)
                        {
                            if((*CCC)->arregloPlantasP[i] == auxPP)
                            {
                                eliminarPlantaDeProduccion((*CCC)->arregloPlantasP, auxPP, &((*CCC)->plibrePP));
                                ordenarPlantaProduccion((*CCC)->arregloPlantasP, (*CCC)->plibrePP);
                            }
                        }
                        printf("\n\n\n\n\nELIMINADO EXITOSAMENTE\n\n\n\n\n");
                        return;
                    }
                    case 0:
                    {
                        printf("\nCANCELADO\n");
                        return;
                    }
                    default:
                    {
                        printf("\nHA OCURRIDO UN ERROR CON EL NUMERO INGRESADO, POR FAVOR INTENTELO MAS TARDE\n");
                        return;
                    }
                }
            }

            return;
        }
        case 3:
        {
            if(((*CCC)->clientes->datosCliente)==NULL)
            {
                printf("\nNO HA INGRESADO CLIENTES AUN\n");
            }else
            {
                printf("\nQUE DESEA HACER CON LOS CLIENTES?\n1 = MOSTRAR DATOS DE LOS CLIENTES\n2 = MODIFICAR UN CLIENTE\n3 = ELIMINAR UN CLIENTE\n0 = CANCELAR\n");
                scanf("%d", &opcion2);
                switch (opcion2) {
                    case 1:
                    {
                        imprimirClientesPreOrden((*CCC)->clientes);
                        return;
                    }
                    case 2:
                    {
                        printf("\nINGRESE EL RUT DEL CLIENTE QUE DESEA MODIFICAR:");
                        do{
                            scanf("%d", &id);
                            if(id<0 || !buscarCliente((*CCC)->clientes, &anteriorCliente, id))
                            {
                                printf("\nHA OCURRIDO UN ERROR CON LA ID INGRESADA\nPOR FAVOR, INTENTE NUEVAMENTE\n");
                            }
                        }while(id<0 || !(buscarCliente((*CCC)->clientes, &anteriorCliente, id)));
                        if(id<anteriorCliente->datosCliente->id)
                        {
                            anteriorCliente = anteriorCliente->izq;
                        }
                        else if(id>anteriorCliente->datosCliente->id)
                        {
                            anteriorCliente= anteriorCliente->der;
                        }
                        if(id == anteriorCliente->datosCliente->id)
                        {
                            printf("\nQue deseas modificar?\n1 = NOMBRE\n2 = REGION\n3 = PEDIDO\n0 = CANCELAR\n\n\n");
                            scanf("%d",&opcion3);
                            switch (opcion3) {
                                case 1:
                                {
                                    printf("\n\n\n\nINGRESE EL NUEVO NOMBRE DEL CLIENTE:");
                                    scanf("%s",buffer);
                                    strcpy(anteriorCliente->datosCliente->nombre,buffer);
                                    printf("\n\n\n\n------CAMBIO REALIZADO CON EXITO!!------\n\n\n\n");
                                    return;
                                }
                                case 2:
                                {
                                    printf("\nIngrese la id de la region a donde esta el cliente\n");
                                    do{
                                        scanf("%d", &id);
                                        printf("\n");
                                        if(id<0 || !buscarRegion((*CCC)->regiones, id))
                                        {
                                            printf("\nHA OCURRIDO UN ERROR CON LA ID INGRESADA\nPOR FAVOR, INTENTE NUEVAMENTE\n");
                                        }
                                    }while(id<0 || !buscarRegion((*CCC)->regiones, id));
                                    anteriorCliente->datosCliente->viveRegion = buscarRegion((*CCC)->regiones, id);
                                    printf("\n\n\n\n------CAMBIO REALIZADO CON EXITO!!------\n\n\n\n");
                                    return;
                                }
                                case 3:
                                {
                                    if(anteriorCliente->datosCliente->cantPedidos == 0)
                                    {
                                        printf("\nNO EXISTEN PEDIDOS EN ESTE CLIENTE\n");
                                    }else{
                                        printf("\nIngrese la id del pedido que desea modificar: ");
                                        do{
                                            scanf("%d", &id);
                                            if(id<0 || !buscarPedidoArray(anteriorCliente->datosCliente->pedidos, id))
                                            {
                                                printf("\nHA OCURRIDO UN ERROR CON LA ID INGRESADA\nPOR FAVOR, INTENTE NUEVAMENTE\n");
                                            }
                                        }while(id<0 || !(buscarPedidoArray(anteriorCliente->datosCliente->pedidos, id)));
                                        for(i=0; i<maxPedidos;i++)
                                        {
                                            if(anteriorCliente->datosCliente->pedidos[i]->id == id)
                                            {
                                                printf("\nQUE DESEA HACER CON EL PEDIDO?\n1 = MODIFICAR FECHA\n2 = CAMBIAR PLANTA DE PRODUCCION\n3 = MODIFICAR PRODUCTOS\n0 = CANCELAR\n\n\n");
                                                scanf("%d", &opcion4);
                                                switch (opcion4) {
                                                    case 1:
                                                    {
                                                        printf("\n\nINGRESE LA NUEVA FECHA (todo junto):");
                                                        scanf("%s", buffer);
                                                        strcpy(anteriorCliente->datosCliente->pedidos[i]->fecha, buffer);
                                                        printf("\n\n\n\n------CAMBIO REALIZADO CON EXITO!!------\n\n\n\n");
                                                        return;
                                                    }
                                                    case 2:
                                                    {
                                                        printf("\nINGRESE LA ID DE LA PLANTA DE PRODUCCION QUE SE ENCARGARA DEL PEDIDO: ");
                                                        do{
                                                            scanf("%d", &id);
                                                            printf("\n");
                                                            if(id<0 || !buscarPlantaProduccion((*CCC)->arregloPlantasP, id, (*CCC)->plibrePP))
                                                            {
                                                                printf("\nHA OCURRIDO UN ERROR CON LA ID INGRESADA\nPOR FAVOR, INTENTE NUEVAMENTE\n");
                                                            }
                                                        }while(id<0 || !buscarPlantaProduccion((*CCC)->arregloPlantasP, id, (*CCC)->plibrePP));
                                                        anteriorCliente->datosCliente->pedidos[i]->plantaProduccion = buscarPlantaProduccion((*CCC)->arregloPlantasP, id, (*CCC)->plibrePP);
                                                        printf("\n\n\n\n------CAMBIO REALIZADO CON EXITO!!------\n\n\n\n");
                                                        return;
                                                    }
                                                    case 3:
                                                    {
                                                        printf("\nQUE DESEA HACER CON LOS PRODUCTOS?\n1 = AGREGAR UN PRODUCTO\n2 = ELIMINAR UN PRODUCTO\n0 = CANCELAR\n");
                                                        scanf("%d", &opcion5);
                                                        switch (opcion5) {
                                                            case 1:
                                                            {
                                                                ciclo1 = 1;
                                                                while(ciclo1)
                                                                {
                                                                    if(anteriorCliente->datosCliente->pedidos[i]->cantidadProductos >= maxProductos)
                                                                    {
                                                                        ciclo1 = 0;
                                                                        printf("\nHA ALCANZADO EL MAXIMO DE PRODUCTOS POR PEDIDO\n");
                                                                    }else
                                                                    {
                                                                        printf("\nINGRESE LA ID DEL PRODUCTO QUE DESEA AGREGAR\n");
                                                                        do{
                                                                            scanf("%d", &id);
                                                                            printf("\n");
                                                                            if(id<0 || !buscarProducto((*CCC)->productos, id))
                                                                            {
                                                                                printf("\nHA OCURRIDO UN ERROR CON LA ID INGRESADA\nPOR FAVOR, INTENTE NUEVAMENTE\n");
                                                                            }
                                                                        }while(id<0 && !buscarProducto((*CCC)->productos, id));
                                                                        agregarProductoArray(anteriorCliente->datosCliente->pedidos[i]->productosPedidos, buscarProducto((*CCC)->productos, id));
                                                                        printf("\n\n\n\nAGREGADO CORRECTAMENTE\n\n\n\n");
                                                                        do {
                                                                            printf("DESEAS AGREGAR ALGO MAS?\n1 = SI\n0 = NO\n\n\n");
                                                                            scanf("%d", &ciclo1);
                                                                            if(ciclo1 < 0 || ciclo1 > 1)
                                                                                printf("\ningrese un digito VALIDO por favor\n");
                                                                        }while(ciclo1 < 0 || ciclo1 > 1);
                                                                    }
                                                                }
                                                                return;
                                                            }
                                                            case 2:
                                                            {
                                                                ciclo1 = 1;
                                                                while(ciclo1)
                                                                {
                                                                    if(anteriorCliente->datosCliente->pedidos[i]->cantidadProductos <= 0)
                                                                    {
                                                                        ciclo1 = 0;
                                                                        eliminarPedidoArray(anteriorCliente->datosCliente->pedidos,anteriorCliente->datosCliente->pedidos[i], &(anteriorCliente->datosCliente->cantPedidos));
                                                                        printf("\nNO HAY PRODUCTOS MAS POR ELIMINAR, POR ENDE SE ELIMINARA EL PEDIDO\n");
                                                                    }else
                                                                    {
                                                                        printf("\nINGRESE LA ID DEL PRODUCTO QUE DESEA ELIMINAR\n");
                                                                        do{
                                                                            scanf("%d", &id);
                                                                            printf("\n");
                                                                            if(id<0 || !buscarProducto((*CCC)->productos, id))
                                                                            {
                                                                                printf("\nHA OCURRIDO UN ERROR CON LA ID INGRESADA\nPOR FAVOR, INTENTE NUEVAMENTE\n");
                                                                            }
                                                                        }while(id<0 && !buscarProducto((*CCC)->productos, id));
                                                                        eliminarProductosArray(anteriorCliente->datosCliente->pedidos[i]->productosPedidos, buscarProducto((*CCC)->productos, id));
                                                                        (anteriorCliente->datosCliente->pedidos[i]->cantidadProductos)--;
                                                                        printf("\nELIMINADO CORRECTAMENTE\n");
                                                                        do {
                                                                            printf("DESEAS AGREGAR ALGO MAS?\n1 = SI\n0 = NO\n");
                                                                            scanf("%d", &ciclo1);
                                                                            if(ciclo1 < 0 || ciclo1 > 1)
                                                                                printf("\ningrese un digito VALIDO por favor\n");
                                                                        }while(ciclo1 < 0 || ciclo1 > 1);
                                                                    }
                                                                }
                                                                return;
                                                            }
                                                            case 0:
                                                            {
                                                                printf("\nSE HA CANCELADO EXITOSAMENTE\n");
                                                                return;
                                                            }
                                                            default:
                                                            {
                                                                printf("\nHA INGRESADO UN NUMERO NO VALIDO\nPOR FAVOR INTENTE MAS TARDE\n");
                                                                return;
                                                            }

                                                        }
                                                        return;
                                                    }
                                                    case 0:
                                                    {
                                                        printf("\nSE HA CANCELADO EXITOSAMENTE\n");
                                                        return;
                                                    }
                                                    default:
                                                    {
                                                        printf("\nHA INGRESADO UN NUMERO NO VALIDO\nPOR FAVOR INTENTE MAS TARDE\n");
                                                        return;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    return;
                                }
                                case 0:
                                {
                                    printf("\nCANCELADO\n");
                                    return;
                                }
                                default:
                                {
                                    printf("\nHA OCURRIDO UN ERROR CON LA OPCION INDICADA\n");
                                    return;
                                }
                            }
                        }
                        return;
                    }
                    case 3:
                    {
                        printf("\nINGRESE EL RUT DEL CLIENTE QUE DESEA ELIMINAR: ");
                        do{
                            scanf("%d", &id);
                            printf("\n");
                            if(id<0 || !buscarCliente((*CCC)->clientes, &anteriorCliente, id))
                            {
                                printf("\nHA OCURRIDO UN ERROR CON LA ID INGRESADA\nPOR FAVOR, INTENTE NUEVAMENTE\n");
                            }
                        }while(id<0 && !buscarCliente((*CCC)->clientes, &anteriorCliente, id));
                        eliminarCliente(&((*CCC)->clientes), id);
                        printf("\nCLIENTE ELIMINADO EXITOSAMENTE\n");
                    }
                    case 0:
                    {
                        printf("\nCANCELADO\n");
                        return;
                    }
                    default:
                    {
                        printf("\nHA OCURRIDO UN ERROR CON EL NUMERO INGRESADO, POR FAVOR INTENTELO MAS TARDE\n");
                        return;
                    }
                }
            }
            return;
        }
        case 4:
        {
            if(!((*CCC)->productos))
            {
                printf("\n\n\n\n\n------NO HAY PRODUCTOS EN LA EMPRESA------\n\n\n\n\n");
            }else
            {
                printf("\nQUE DESEA HACER CON LOS PRODUCTOS?\n1 = MOSTRAR DATOS DE LOS PRODUCTOS\n2 = MODIFICAR UN PRODUCTO\n3 = ELIMINAR UN PRODUCTO\n0 = CANCELAR\n");
                scanf("%d", &opcion2);
                switch (opcion2) {
                    case 1:
                    {
                        imprimirProductosLista((*CCC)->productos);
                        return;
                    }
                    case 2:
                    {
                        printf("\nINGRESE EL ID DEL PRODUCTO QUE DESEA MODIFICAR:");
                        do{
                            scanf("%d", &id);
                            printf("\n");
                            if(id<0 || !buscarProducto((*CCC)->productos,id))
                            {
                                printf("\nHA OCURRIDO UN ERROR CON LA ID INGRESADA\nPOR FAVOR, INTENTE NUEVAMENTE\n");
                            }
                        }while(id<0 && !buscarProducto((*CCC)->productos,id));
                        do {
                            if(modProducto->datosProducto->id == id)
                            {
                                printf("\nQUE DESEA MODIFICAR DEL PRODUCTO?\n1 = NOMBRE\n2 = PRECIO \n0 = CANCELAR\n");
                                scanf("%d", &opcion3);
                                switch (opcion3) {
                                    case 1:
                                    {
                                        printf("INGRESE EL NUEVO NOMBRE: ");
                                        scanf("%s", buffer);
                                        strcpy(modProducto->datosProducto->nombre, buffer);
                                        printf("\n\n\n\n------CAMBIO REALIZADO CON EXITO!!------\n\n\n\n");
                                        return;
                                    }
                                    case 2:
                                    {

                                        do
                                        {
                                            printf("\nNUEVO PRECIO : $ ");
                                            scanf("%d", &modProducto->datosProducto->precio);
                                            printf("\n");
                                            if(modProducto->datosProducto->precio < 1)
                                            {
                                                printf("\nERROR!!!\n\nIngrese un PRECIO valido\n\n\n\n");
                                            }
                                        }while(modProducto->datosProducto->precio < 1);
                                        printf("CAMBIO REALIZADO CON EXITO!!\n\n\n\n");
                                        return;
                                    }
                                    case 0:
                                    {
                                        printf("\nCANCELADO\n");
                                        return;
                                    }
                                    default:
                                    {
                                        printf("\nHA OCURRIDO UN ERROR CON EL NUMERO INGRESADO, POR FAVOR INTENTELO MAS TARDE\n\n\n\n");
                                        return;
                                    }
                                }
                            }
                            modProducto = modProducto->sig;
                        }while(modProducto->datosProducto!=(*CCC)->productos->datosProducto);
                        return;
                    }
                    case 3:
                    {
                        do{
                            printf("\nINGRESE EL ID DEL PRODUCTO QUE DESEA ELIMINAR:");
                            scanf("%d", &id);
                            printf("\n");
                            if(id<0 || !buscarProducto((*CCC)->productos,id))
                            {
                                printf("\nHA OCURRIDO UN ERROR CON LA ID INGRESADA\nPOR FAVOR, INTENTE NUEVAMENTE\n\n\n\n");
                            }
                        }while(id<0 || !(buscarProducto((*CCC)->productos,id)));
                        eliminarProducto(&((*CCC)->productos), buscarProducto((*CCC)->productos,id));
                        printf("\n\n\n\nPRODUCTO ELIMINADO CORRECTAMENTE\n\n\n\n\n");
                        return;
                    }
                    case 0:
                    {
                        printf("\n\n\n\n\nCANCELADO\n\n\n\n\n");
                        return;
                    }
                    default:
                    {
                        printf("\nHA OCURRIDO UN ERROR CON EL NUMERO INGRESADO, POR FAVOR INTENTELO MAS TARDE\n\n\n\n");
                        return;
                    }
                }
            }
            return;
        }
        case 5:
        {
            printf("QUE DESEAS HACER?\n\n1 = MOSTRAR DATOS COCA-COLA COMPANY\n2 = MODIFICAR DATOS COCA-COLA COMPANY\n0 = CANCELAR\n\n\n");
            scanf("%d", &opcion2);
            switch (opcion2) {
                case 1:
                {
                    printf("PAIS: %s\n", (*CCC)->pais);
                    printf("DIRECCION: %s\n", (*CCC)->direccion);
                    printf("PRESIDENTE NACIONAL COCA-COLA COMPANY: %s\n", (*CCC)->jefeNacional);
                    return;
                }
                case 2:
                {
                    printf("QUE DESEAS MODIFICAR?\n\n1 = PAIS\n2 = DIRECCION\n3 = PRESIDENTE NACIONAL CCC\n0 = CANCELAR\n");
                    scanf("%d", &opcion3);
                    switch (opcion3) {
                        case 1:
                        {
                            printf("INGRESE EL NOMBRE DEL NUEVO PAIS : ");
                            scanf("%s", buffer);
                            strcpy((*CCC)->pais, buffer);
                            printf("\n\n\n\n------CAMBIO REALIZADO CON EXITO!!------\n\n\n\n");
                            return;
                        }
                        case 2:
                        {
                            printf("INGRESE LA NUEVA DIRECCION : ");
                            scanf("%s", buffer);
                            strcpy((*CCC)->direccion, buffer);
                            printf("\n\n\n\n------CAMBIO REALIZADO CON EXITO!!------\n\n\n\n");
                            return;
                        }
                        case 3:
                        {
                            printf("INGRESE EL NOMBRE DEL NUEVO JEFE NACIONAL : ");
                            scanf("%s", buffer);
                            strcpy((*CCC)->jefeNacional, buffer);
                            printf("\n\n\n\n------CAMBIO REALIZADO CON EXITO!!------\n\n\n\n");
                            return;
                        }
                        case 0:
                        {
                            printf("\nCANCELADO\n");
                            return;
                        }
                        default:
                        {
                            printf("\nHA OCURRIDO UN ERROR CON EL NUMERO INGRESADO, POR FAVOR INTENTELO MAS TARDE\n");
                            return;
                        }
                    }
                    return;
                }
                case 0:
                {
                    printf("\nCANCELADO\n");
                    return;
                }
                default:
                {
                    printf("\nHA OCURRIDO UN ERROR CON EL NUMERO INGRESADO, POR FAVOR INTENTELO MAS TARDE\n");
                    return;
                }
            }
            return;
        }
        case 0:
        {
            printf("\nCANCELADO\n");
            return;
        }
        default:
        {
            printf("\nHA OCURRIDO UN ERROR CON EL NUMERO, INTENTELO MAS TARDE :D GRACIAS\n");
            return;
        }

    }
    return;
}

/**
 *
 *      FUNCION LLAMAR F(X) ESPECIFICAS
 *
 * */

void funcionesEspecificar(struct CocaCola **CCC)
{
    int opcion, mayor = 0, cantidad = 0;
    struct Cliente *cliente = NULL;
    struct Region *region = NULL;
    struct Producto *producto = NULL;

    printf("\nQUE DESEA VER?\n1 = CLIENTE CON MAS DINERO GASTADO\n2 = REGION QUE MAS CONSUME\n3 = PRODUCTOS TOTAL VENDIDOS\n4 = PRODUCTO MAS VENDIDO\n5 = MENOR GASTO ECONOMINO ENTRE PLANTA DE PRODUCCION CON CLIENTE\n\n");
    scanf("%d", &opcion);
    switch (opcion) {
        case 1:
        {
            clienteConMasDineroGastado((*CCC)->clientes, &cliente, &mayor);
            if(cliente && mayor > 0)
            {
                printf("\n\nel cliente %s ha gastado $%d  !!\nSE HA GANADO UN TALADRO PERCUTOR BAUKER!!\n\n", cliente->nombre, mayor);
            }else
            {
                printf("\nHA OCURRIDO UN ERROR CON EL CLIENTE\n\n\n");
            }
            return;
        }
        case 2:
        {
            region = regionConsumidora((*CCC)->clientes, (*CCC)->regiones, &mayor);
            if(region)
            {
                printf("\nLa region que mas consume es %s, es por ello que se creara una planta de produccion en dicha region\nConsumen un total de: %d\n\n\n", region->nombre, mayor);
            }else
            {
                printf("\nHA OCURRIDO UN ERROR CON LOS DATOS INGRESADOS\nVERIFIQUE QUE TODO ESTE EN ORDEN PRIMERAMENTE\n\n\n");
            }
            return;
        }
        case 3:
        {
            printf("\nLA EMPRESA HA VENDIDO UN TOTAL DE %d PRODUCTOS\n\n\n", productosTotalVendidos((*CCC)->clientes));
            return;
        }
        case 4:
        {
            producto = productoMasVendido((*CCC)->clientes, (*CCC)->productos);
            if(producto)
            {
                printf("\nEl producto %s es el mas consumido, es por ello que debemos enfocarnos en este producto por sobre los demas\n\n\n", producto->nombre);
            }else
            {
                printf("\nHA OCURRIDO UN ERROR CON LOS DATOS INGRESADOS\nVERIFIQUE QUE TODO ESTE EN ORDEN PRIMERAMENTE\n\n\n");
            }
            return;
        }
        case 5:
        {
            int id, i, posicionCliente, posicionPP, gastoCliente, gastoEmpresa;
            char nombrePP[100],nombreCliente[100];
            struct Pedidos *pedidoCliente;
            struct PlantaProduccion *plantaInicio;
            struct ArbolClientes *anteriorCliente=(*CCC)->clientes;
            struct Cliente *clienteFinal= NULL;
            if((*CCC)->plibrePP < 1)
            {
                printf("\n\n\n\nNECESITA DE AL MENOS UNA PLANTA DE PRODUCCION PARA PODER DISTRIBUIR UN PEDIDO\n\n\n\n");
                return;
            }else
            {
                if((*CCC)->clientes)
                {
                    do{
                        printf("\n\nINGRESE LA ID DE LA PLANTA DE PRODUCCION :");
                        scanf("%d", &id);
                        if(id<0 || !buscarPlantaProduccion((*CCC)->arregloPlantasP,id,(*CCC)->plibrePP))
                        {
                            printf("\nHA OCURRIDO UN ERROR CON LA ID INGRESADA\nPOR FAVOR, INTENTE NUEVAMENTE\n");
                        }
                    }while(id<0 || !buscarPlantaProduccion((*CCC)->arregloPlantasP,id,(*CCC)->plibrePP));
                    plantaInicio = buscarPlantaProduccion((*CCC)->arregloPlantasP,id,(*CCC)->plibrePP);
                    do{
                        printf("\n\nINGRESE LA ID DEL CLIENTE AL QUE SE LE REALIZARA LA ENTREGA DEL PEDIDO : ");
                        scanf("%d", &id);
                        printf("\n");
                        if(id<0 || !buscarCliente((*CCC)->clientes, &anteriorCliente, id))
                        {
                            printf("\nHA OCURRIDO UN ERROR CON LA ID INGRESADA\nPOR FAVOR, INTENTE NUEVAMENTE\n");
                        }
                    }while(id<0 && !buscarCliente((*CCC)->clientes, &anteriorCliente, id));
                    if(id<anteriorCliente->datosCliente->id)
                    {
                        anteriorCliente = anteriorCliente->izq;
                    }
                    else if(id>anteriorCliente->datosCliente->id)
                    {
                        anteriorCliente= anteriorCliente->der;
                    }
                    if(id == anteriorCliente->datosCliente->id)
                    {
                        clienteFinal = anteriorCliente->datosCliente;
                        if(clienteFinal->cantPedidos>0)
                        {
                            if(plantaInicio->region == clienteFinal->viveRegion)
                            {
                                do {
                                    printf("\n\nINGRESE LA POSICION DEL CLIENTE(COMUNA DEL 1 AL 100) = ");
                                    scanf("%d", &posicionPP);
                                    if(posicionPP<1||posicionPP>100)
                                    {
                                        printf("\n\nHA INGRESADO UNA POSICION INCORRECTA\n\n\n");
                                    }
                                }while(posicionPP<1||posicionPP>100);

                                do {
                                    printf("\n\nINGRESE LA POSICION DEL CLIENTE(COMUNA DEL 1 AL 100) = ");
                                    scanf("%d", &posicionCliente);
                                    if(posicionCliente<1||posicionCliente>100)
                                    {
                                        printf("\n\nHA INGRESADO UNA POSICION INCORRECTA\n\n\n");
                                    }
                                }while(posicionCliente<1||posicionCliente>100);

                                do{
                                    printf("\n\nINGRESE LA ID DEL PEDIDO QUE DESEA ENTREGAR : ");
                                    scanf("%d", &id);
                                    if(id<0 || !buscarPedidoArray(anteriorCliente->datosCliente->pedidos,id))
                                    {
                                        printf("\n\n\n\nNO EXISTE DICHA ID PRODUCTO DEL CLIENTE\nPOR FAVOR INTENTE NUEVAMENTE\n\n\n\n");
                                    }
                                }while(id<0 || !buscarPedidoArray(anteriorCliente->datosCliente->pedidos,id));
                                pedidoCliente = buscarPedidoArray(anteriorCliente->datosCliente->pedidos,id);
                                gastoCliente = cantidadDeProductosComprados(pedidoCliente->productosPedidos);
                                gastoEmpresa = dijkstra(posicionPP, posicionCliente, (*CCC)->grafoPlantasD);
                                if(gastoCliente>gastoEmpresa)
                                {
                                    printf("\n\n\nPARA ENTREGAR EL PEDIDO %d\nDESDE LA PLANTA %s\nHASTA EL CLIENTE: %s\nHEMOS GENERADO UNA GANANCIA DE = $%d\n\n\n",pedidoCliente->id, plantaInicio->nombre,clienteFinal->nombre, gastoCliente-gastoEmpresa);
                                }else
                                {
                                    if(gastoCliente < gastoEmpresa)
                                    {
                                        printf("\n\n\n\n\n\nPARA ENTREGAR EL PEDIDO %d\n DESDE LA PLANTA %s\nHASTA EL CLIENTE: %s\nHEMOS PERDIDO UN TOTAL DE = -$%d\n\n\n\n\n\n",pedidoCliente->id, plantaInicio->nombre,clienteFinal->nombre, gastoEmpresa-gastoCliente);
                                    }else
                                    {
                                        printf("\n\n\n\n\n\nPARA ENTREGAR EL PEDIDO %d\n DESDE LA PLANTA %s\nHASTA EL CLIENTE: %s\nNO HEMOS GENERADO GANANCIAS\n\n\n\n\n\n",pedidoCliente->id, plantaInicio->nombre,clienteFinal->nombre);
                                    }
                                }
                                eliminarPedidoArray(anteriorCliente->datosCliente->pedidos,pedidoCliente,&(anteriorCliente->datosCliente->cantPedidos));
                                printf("\nSE HA REMOVIDO EL PEDIDO POR SER ENTREGADO\n");
                                return;
                            }else
                            {
                                printf("\n\n\n\n\nEL CLIENTE Y LA PLANTA DE PRODUCCION SON DE DISTINTAS REGIONES\nCOMPRUEBE PRIMERO TENER EL CLIENTE EN LA MISMA REGION A LA PLANTA DE PRODUCCION\n\n\n\n\n");
                                return;
                            }
                        }else
                        {
                            printf("\n\n\n\n\nEL CLIENTE NO TIENE PEDIDOS PARA ENVIARLE\n\n\n\n");
                            return;
                        }
                    }
                }else
                {
                    printf("\n\n\n\nNECESITA DE AL MENOS UN CLIENTE PARA PODER DISTRIBUIR UN PEDIDO\n\n\n\n");
                    return;
                }
            }
        }
        case 0:
        {
            printf("\nCANCELADO\n");
            return;
        }
        default:
        {
            printf("\nHA OCURRIDO UN ERROR CON EL NUMERO, INTENTELO MAS TARDE :D GRACIAS\n");
            return;
        }
    }
}

/**
 *
 *          FUNCION MAIN
 *
 * */

int main(){
    int ciclo1, opcion1;
    struct CocaCola *CCC;
    CCC = (struct CocaCola *) malloc(sizeof (struct CocaCola));
    CCC->pais = (char *)malloc(sizeof(char));
    CCC->direccion = (char *)malloc(sizeof(char));
    CCC->jefeNacional = (char *)malloc(sizeof(char));
    CCC->arregloPlantasP = (struct PlantaProduccion **)malloc(maxPlantasP * sizeof(struct PlantaProduccion *));
    CCC->plibrePP = 0;
    CCC->regiones = (struct NodoRegiones *)malloc(sizeof(struct NodoRegiones));
    CCC->regiones->region = NULL;
    CCC->regiones->sig = NULL;
    CCC->productos = NULL;
    CCC->clientes = NULL;
    CCC->grafoPlantasD = (struct GrafoPlantasD**)malloc(MAXPLANTASD *sizeof(struct GrafoPlantasD*));
    llenarGrafosConPrecios(CCC->grafoPlantasD);
    printf("\n");
    printf("Ingrese el pais:");
    gets(CCC->pais);
    printf("Ingrese la direccion:");
    gets(CCC->direccion);
    printf("Ingrese el jefe Nacional:");
    gets(CCC->jefeNacional);
    do
    {
        printf("Ingrese la id : ");
        scanf("%d",&CCC->id);
        if(CCC->id<1)
        {
            printf("\n\n\nIngrese una id valida por favor!!!\n\n\n");
        }
    }while(CCC->id<1);

    printf("\n\nBienvenido a Coca-Cola Company %s\n\n", CCC->pais);
    do {
        printf("\n\n\n\n------------------------------------------\n\n");
        printf("\nDESEA HACER ALGO MAS?\n\n1 = SI\n\n0 = NO\n\n\n");
        printf("------------------------------------------\n\n");
        scanf("%d", &ciclo1);
        if(ciclo1 < 0 || ciclo1 > 1)
            printf("\n\ningrese un digito VALIDO por favor\n");
    }while(ciclo1 < 0 || ciclo1 > 1);
    while(ciclo1)
    {
        printf("\nQue desea hacer?\n\n1 = CREAR\n\n2 = BUSCAR\n\n3 = FUNCIONES ESPECIFICAS\n\n0 = CANCELAR\n\n\n");
        scanf("%d", &opcion1);
        switch (opcion1) {
            case 1:
            {
                crearObjeto(&CCC);
                break;
            }
            case 2:
            {
                buscarObjeto(&CCC);
                break;
            }
            case 3:
            {
                funcionesEspecificar(&CCC);
                break;
            }
            case 0:
            {
                printf("\nSE HA CANCELADO LA ACCION\n\n\n");
                break;
            }
            default:
            {
                printf("\nHA OCURRIDO UN ERROR!\nASEGURESE DE INGRESAR NUMEROS VALIDOS!!\n\n\n");
                break;
            }
        }
        do {
            printf("\n\n\n\n------------------------------------------\n\n");
            printf("\nDESEA HACER ALGO MAS?\n\n1 = SI\n\n0 = NO\n\n\n");
            printf("------------------------------------------\n\n");
            scanf("%d", &ciclo1);
            if(ciclo1 < 0 || ciclo1 > 1)
                printf("\ningrese un digito VALIDO por favor\n");
        }while(ciclo1 < 0 || ciclo1 > 1);
    }
    return 0;
}