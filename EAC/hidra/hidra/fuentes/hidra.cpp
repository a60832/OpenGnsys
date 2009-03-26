// ****************************************************************************************************************************************************
// Aplicacin HIDRA
// Copyright 2003-2005 JosnManuel Alonso. Todos los derechos reservados.
// Fichero: hidra.cpp
//	Descripcin:
//		Este proyecto implementa el servicio hidra en un ordenador con plataforma windows NT. Este fichero aporta las funciones de 
//		envn de comandos y recepcin de respuestas
// ****************************************************************************************************************************************************
#include "hidra.h"
#include "encriptacion.c"
// ________________________________________________________________________________________________________
// Funcin: RegistraLog
//
//		Descripción:
//			Esta funcin registra los evento de errores en un fichero log
//		Parametros:
//			- msg : Mensage de error
//			- swerrno: Switch que indica que recupere literal de error del sistema
// ________________________________________________________________________________________________________
void RegistraLog(char *msg,int swerrno)
{
	struct tm * timeinfo;
	timeinfo = TomaHora();

	FLog=fopen(szPathFileCfg,"at");
	if(swerrno)
		fprintf (FLog,"%02d/%02d/%d %02d:%02d ***%s:%s\n",timeinfo->tm_mday,timeinfo->tm_mon+1,timeinfo->tm_year+1900,timeinfo->tm_hour,timeinfo->tm_min,msg,strerror(errno));
	else
		fprintf (FLog,"%02d/%02d/%d %02d:%02d ***%s\n",timeinfo->tm_mday,timeinfo->tm_mon+1,timeinfo->tm_year+1900,timeinfo->tm_hour,timeinfo->tm_min,msg);
	fclose(FLog);
}

// ________________________________________________________________________________________________________
// Funcin: TomaHora
//
//		Descripcin:
//			Esta funcin toma la hora actual  del sistema y devuelve una estructura conlos datos
//		Parametros:
//			- msg : Mensage de error
//			- swerrno: Switch que indica que recupere literal de error del sistema
// ________________________________________________________________________________________________________
struct tm * TomaHora()
{
	time_t rawtime;
	time ( &rawtime );
	return(gmtime(&rawtime));
}
//________________________________________________________________________________________________________
//
// Función: TomaConfiguracion
//
//		Descripción:
//		Esta función lee el fichero de configuración del programa hidralinuxcli  y toma los parametros
//		Parametros:
//				- pathfilecfg : Ruta al fichero de configuración
//________________________________________________________________________________________________________
int TomaConfiguracion(char* pathfilecfg)
{
	long lSize;
	char * buffer,*lineas[100],*dualparametro[2];
	char ch[2];
	int i,numlin,resul;

	if(pathfilecfg==NULL) return(FALSE); // Nombre del fichero en blanco

	Fconfig = fopen ( pathfilecfg , "rb" );
	if (Fconfig==NULL)	return(FALSE);
	fseek (Fconfig , 0 , SEEK_END);  // Obtiene tamaño del fichero.
	lSize = ftell (Fconfig);
	rewind (Fconfig);
	buffer = (char*) malloc (lSize);  // Toma memoria para el buffer de lectura.
	if (buffer == NULL)	 	return(FALSE);
	fread (buffer,1,lSize,Fconfig); 	// Lee contenido del fichero
	fclose(Fconfig);

	//inicializar
	IPlocal[0]=(char)NULL;
	servidorhidra[0]=(char)NULL;
	Puerto[0]=(char)NULL;
	usuario[0]=(char)NULL;
	pasguor[0]=(char)NULL;
	datasource[0]=(char)NULL;
	catalog[0]=(char)NULL;
	
	
	strcpy(ch,"\n");// caracter delimitador ( salto de linea)
	numlin=split_parametros(lineas,buffer,ch);
	for (i=0;i<numlin;i++){
		strcpy(ch,"=");// caracter delimitador
		split_parametros(dualparametro,lineas[i],ch); // Toma primer nombre del parametros

		resul=strcmp(dualparametro[0],"IPhidra");
		if(resul==0) strcpy(IPlocal,dualparametro[1]);

		resul=strcmp(dualparametro[0],"IPhidra");
		if(resul==0) strcpy(servidorhidra,dualparametro[1]);

		resul=strcmp(dualparametro[0],"Puerto");
		if(resul==0) strcpy(Puerto,dualparametro[1]);
		
		resul=strcmp(dualparametro[0],"AulaUp");
		if(resul==0) strcpy(AulaUp,dualparametro[1]);
	
		resul=strcmp(dualparametro[0],"Usuario");
		if(resul==0) strcpy(usuario,dualparametro[1]);

		resul=strcmp(dualparametro[0],"PassWord");
		if(resul==0) strcpy(pasguor,dualparametro[1]);

		resul=strcmp(dualparametro[0],"DataSource");
		if(resul==0) strcpy(datasource,dualparametro[1]);

		resul=strcmp(dualparametro[0],"Catalog");
		if(resul==0) strcpy(catalog,dualparametro[1]);
	}
	if(IPlocal[0]==(char)NULL){
		RegistraLog("IPlocal, NO se ha definido este parámetro",false);
		return(FALSE);
	}
	if(servidorhidra[0]==(char)NULL){
		RegistraLog("IPhidra, NO se ha definido este parámetro",false);
		return(FALSE);
	}
	if(Puerto[0]==(char)NULL){
		RegistraLog("Puerto, NO se ha definido este parámetro",false);
		return(FALSE);
	}
	puerto=atoi(Puerto);
	
	if(AulaUp[0]==(char)NULL){
		RegistraLog("AulaUp, NO se ha definido este parámetro",false);
		return(FALSE);
	}	
	aulaup=atoi(AulaUp);
			
	if(usuario[0]==(char)NULL){
		RegistraLog("Usuario, NO se ha definido este parámetro",false);
		return(FALSE);
	}	
	if(pasguor[0]==(char)NULL){
		RegistraLog("PassWord, NO se ha definido este parámetro",false);
		return(FALSE);
	}	
	if(datasource[0]==(char)NULL){
		RegistraLog("DataSource, NO se ha definido este parámetro",false);
		return(FALSE);
	}	
	if(catalog[0]==(char)NULL){
		RegistraLog("Catalog, NO se ha definido este parámetro",false);
		return(FALSE);
	}	
	return(TRUE);
}
// ________________________________________________________________________________________________________
// Funcin: GestionaConexion
//
//		Descripcin:
//			Esta hebra es la encargada de comunicarse con los clientes  a travn del socket enviado como parnetro.
//		Parametros:
//			- lpParam : Socket usado
// ________________________________________________________________________________________________________
void * GestionaConexion(void* s)
{
	SOCKET socket_c=*(SOCKET*)s;
	TRAMA trama;		
	
	if (recibe_trama(socket_c,&trama)){
		if (strncmp(trama.identificador,"JMMLCAMDJ",9)==0) // Es una trama hidra
			gestiona_comando(socket_c,trama);
	}
	return(s);
}
// ________________________________________________________________________________________________________
// Funcin: gestiona_comando
//
//		Descripcin:
//			Analiza el comando enviado por el servidor web y lo reenvia al cliente rembo o lo ejecuta n mismo
//		Parametros:
//			- s : Socket usado
//			- trama : La trama con los parametrso del comando
// ________________________________________________________________________________________________________
void gestiona_comando(SOCKET s,TRAMA trama)
{
	int i,resul,idaccion,numipes,cont,estado_cliente;
	char *parametros,*nombrefuncion;
	char *iph,*ids,*coletilla;
	char pids[20],ipes[MAXLON_PARAMETROSIPH];

	parametros=&trama.parametros[0];
	
	if (trama.ejecutor=='1'){	// Debe ejecutar el servidor
		INTROaFINCAD(parametros);
		nombrefuncion=toma_parametro("nfn",parametros); 
		resul=strcmp(nombrefuncion,"inclusion_cliRMB");
		if(resul==0){
			if(!inclusion_cliRMB(s,parametros))
				respuesta_cortesia(s);
			return;
		}

		resul=strcmp(nombrefuncion,"inclusion_cliWINLNX");
		if(resul==0){
			inclusion_cliWINLNX(s,parametros);
			return;
		}
		
		resul=strcmp(nombrefuncion,"inclusion_REPO");
		if(resul==0){
			inclusion_REPO(s,parametros);
			return;
		}		

		resul=strcmp(nombrefuncion,"COMANDOSpendientes");
		if(resul==0){
			if(!COMANDOSpendientes(s,parametros))
				respuesta_cortesia(s);
			return;
		}

		resul=strcmp(nombrefuncion,"RecuperaItem");
		if(resul==0){
			if(!RecuperaItem(s,parametros))
				respuesta_cortesia(s);
			return;
		}

		resul=strcmp(nombrefuncion,"EjecutarItem");
		if(resul==0){
			if(!EjecutarItem(s,parametros))
				respuesta_cortesia(s);
			return;
		}
		resul=strcmp(nombrefuncion,"disponibilidadCOMANDOS");
		if(resul==0){
			disponibilidadCOMANDOS(s,parametros);
			respuesta_cortesia(s);
			return;
		}
		resul=strcmp(nombrefuncion,"Sondeo");
		if(resul==0){
			Sondeo(s,parametros);
			return;
		}
		resul=strcmp(nombrefuncion,"Arrancar");
		if(resul==0){
			Arrancar(parametros);
			return;
		}

		resul=strcmp(nombrefuncion,"Actualizar");
		if(resul==0){
			Actualizar(parametros);
			return;
		}

		resul=strcmp(nombrefuncion,"Conmutar");
		if(resul==0){
			Conmutar(parametros);
			return;
		}
		resul=strcmp(nombrefuncion,"Purgar");
		if(resul==0){
			PurgarTablaSockets(parametros);
			return;
		}
	
		resul=strcmp(nombrefuncion,"RESPUESTA_Arrancar");
		if(resul==0){
			RESPUESTA_Arrancar(s,parametros);
			respuesta_cortesia(s);
			return;
		}

		resul=strcmp(nombrefuncion,"RESPUESTA_Apagar");
		if(resul==0){
			RESPUESTA_Apagar(s,parametros);
			respuesta_cortesia(s);
			return;
		}

		resul=strcmp(nombrefuncion,"RESPUESTA_RemboOffline");
		if(resul==0){
			RESPUESTA_RemboOffline(s,parametros);
			respuesta_cortesia(s);
			return;
		}

		resul=strcmp(nombrefuncion,"RESPUESTA_Reiniciar");
		if(resul==0){
			RESPUESTA_Reiniciar(s,parametros);
			respuesta_cortesia(s);
			return;
		}
		resul=strcmp(nombrefuncion,"RESPUESTA_EjecutarScript");
		if(resul==0){
			RESPUESTA_EjecutarScript(s,parametros);
			respuesta_cortesia(s);
			return;
		}
		resul=strcmp(nombrefuncion,"RESPUESTA_CrearPerfilSoftware");
		if(resul==0){
			RESPUESTA_CrearPerfilSoftware(s,parametros);
			respuesta_cortesia(s);
			return;
		}

		resul=strcmp(nombrefuncion,"RESPUESTA_CrearSoftwareIncremental");
		if(resul==0){
			RESPUESTA_CrearSoftwareIncremental(s,parametros);
			respuesta_cortesia(s);
			return;
		}
		resul=strcmp(nombrefuncion,"RESPUESTA_RestaurarImagen");
		if(resul==0){
			RESPUESTA_RestaurarImagen(s,parametros);
			respuesta_cortesia(s);
			return;
		}
		resul=strcmp(nombrefuncion,"RESPUESTA_ParticionaryFormatear");
		if(resul==0){
			RESPUESTA_ParticionaryFormatear(s,parametros);
			respuesta_cortesia(s);
			return;
		}
		resul=strcmp(nombrefuncion,"RESPUESTA_Configurar");
		if(resul==0){
			RESPUESTA_Configurar(s,parametros);
			respuesta_cortesia(s);
			return;
		}
		resul=strcmp(nombrefuncion,"RESPUESTA_TomaConfiguracion");
		if(resul==0){
			RESPUESTA_TomaConfiguracion(s,parametros);
			respuesta_cortesia(s);
			return;
		}
		resul=strcmp(nombrefuncion,"RESPUESTA_TomaHardware");
		if(resul==0){
			RESPUESTA_TomaHardware(s,parametros);
			respuesta_cortesia(s);
			return;
		}		
	}
	else{	// Debe ejecutar elcliente rembo
		coletilla=corte_iph(parametros); // toma el puntero al comienzo del parametros iph
		INTROaFINCAD(coletilla);
		iph=toma_parametro("iph",coletilla); // Toma ipes
		ids=toma_parametro("ids",coletilla); // Toma identificador de la accion
		coletilla[0]='\0';// Corta la trama en la ip
		strcpy(ipes,iph); // Copia la cadena de ipes
		if(ids!=NULL){
			idaccion=atoi(ids);
			sprintf(pids,"ids=%d\r",idaccion);
			strcat(parametros,pids); // Le ande el identificador de la accion
		}
		numipes=cuenta_ipes(ipes); // Numero de ipes a los que enviar las tramas
		cont=0;
		DesmarcaServidoresRembo();
		for (i=0;i<MAXIMOS_SOCKETS;i++){
			if (strncmp(tbsockets[i].ip,"\0",1)!=0){ // Si es un cliente activo
				if (IgualIP(ipes,tbsockets[i].ip)){ // Si existe la IP en la cadena
					estado_cliente=strcmp(tbsockets[i].estado,CLIENTE_REMBO);
					if(estado_cliente==0){ // Cliente Rembo ...
						strcpy(tbsockets[i].estado,CLIENTE_OCUPADO); 
						MarcaServidoresRembo(tbsockets[i].ipsrvrmb,tbsockets[i].ip);
					}
					else{
						estado_cliente=strcmp(tbsockets[i].estado,CLIENTE_OCUPADO);
						if(estado_cliente!=0){ // Cliente Windows(Windows98,Windows2000,windows XP...) y Linux
							strcpy(tbsockets[i].estado,CLIENTE_OCUPADO); 
							manda_comando(tbsockets[i].sock,parametros);
						}
					}
					cont++; // Contador de envios de tramas a  ordenadores
					if(cont==numipes) break;
				}
			}
		}
		EnviaServidoresRembo(parametros);
	}
}
// ________________________________________________________________________________________________________
// Funcin: manda_comando
//
//		Descripcin:
//			Esta funcin envia un comando por la red (TCP) desde el servidor hidra al servidor rembo que controla al cliente que lo ejecutarn
//		Parametros:
//			- sock : El socket del cliente
//			- parametros: El contenido del comando
// ________________________________________________________________________________________________________
int manda_comando(SOCKET sock,char* parametros)
{
	TRAMA trama;		
	int resul;

	trama.arroba='@';
	strncpy(trama.identificador,"JMMLCAMDJ",9);
	trama.ejecutor='0';
	strcpy(trama.parametros,parametros);
	resul=manda_trama(sock,&trama);
	return(resul);
}
// ________________________________________________________________________________________________________
// Funcin: manda_trama
//
//		Descripcin:
//			Esta funcin envia una trama por la red (TCP) 
//		Parametros:
//			- sock : El socket del host al que se dirige la trama
//			- trama: El contenido de la trama
// ________________________________________________________________________________________________________
int manda_trama(SOCKET sock,TRAMA* trama)
{
	int nLeft,idx,ret;
	Encriptar((char*)trama);
	nLeft = strlen((char*)trama);
	idx = 0;
	while(nLeft > 0){
		ret = send(sock,(char*)&trama[idx], nLeft, 0);
		
		if (ret == 0){
			break;
		}
		else
			if (ret == SOCKET_ERROR){
				RegistraLog("***send() fallo en hebra cliente",true);
				return(FALSE);
			}
		nLeft -= ret;
		idx += ret;
	}
	return(TRUE);
}
// ________________________________________________________________________________________________________
// Funcin: recibe_trama
//
//		Descripcin:
//			Esta funcin recibe una trama por la red (TCP)
//		Parametros:
//			- sock : El socket del cliente
//			- trama: El buffer para recibir la trama
// ________________________________________________________________________________________________________
int recibe_trama(SOCKET sock,TRAMA* trama)
{
	int ret;

	while(1){ // Bucle para recibir datos del cliente 
		ret = recv(sock,(char*)trama,LONGITUD_TRAMA,0);
		if (ret == 0) // Conexin cerrada por parte del cliente (Graceful close)
			break;
		else{ 
			if (ret == SOCKET_ERROR){
				RegistraLog("***recv() fallo en recepcion trama",true);
				return (FALSE);
			}
			else // Datos recibidos
				break;
		}
	}
 	Desencriptar((char*)trama);
	trama->parametros[ret-11]=(char)NULL; // Coloca caracter fin de cadena en trama
	return(TRUE);
}
// ________________________________________________________________________________________________________
// Funcin: hay_hueco
// 
// 		Descripcin:
// 			Esta funcin devuelve true o false dependiendo de que haya hueco en la tabla de sockets para un nuevo cliente.
// 			Parametros:
// 				- idx:   Primer indice libre que se podrn utilizar
// ________________________________________________________________________________________________________
int hay_hueco(int *idx)
{
	int i;

	for (i=0;i<MAXIMOS_SOCKETS;i++){
		if (strncmp(tbsockets[i].ip,"\0",1)==0){ // Hay un hueco
			*idx=i;
			return(TRUE);
		}
	}
	return(FALSE);
}
// ________________________________________________________________________________________________________
// Funcin: cliente_existente
//
//		 Descripcin:
// 			Esta funcin devuelve true o false dependiendo de si el cliente estnya registrado en  la tabla de sockets
//		Parametros:
//				- ip : La ip del cliente a buscar
//				- idx:   Indice que ocuparn el cliente, de estar ya registrado
// ________________________________________________________________________________________________________
BOOLEAN cliente_existente(char *ip,int* idx)
{
	int i;
	for (i=0;i<MAXIMOS_SOCKETS;i++){
		if (strcmp(ip,tbsockets[i].ip)==0){ // Si existe la IP ...
			*idx=i;
			return(TRUE);
		}
	}
	return(FALSE);
}
// ________________________________________________________________________________________________________
// Funcin: hay_huecoservidorrembo
// 
// 		Descripcin:
// 			Esta funcin devuelve true o false dependiendo de que haya hueco en la tabla de sockets para un nuevo servidor rembo.
// 		Parametros:
// 			- idx:   Primer indice libre que se podrn utilizar
// ________________________________________________________________________________________________________
int hay_huecoservidorrembo(int *idx)
{
	int i;
	for (i=0;i<MAXIMOS_SRVRMB;i++){
		if (strncmp(tbsocketsSRVRMB[i].ip,"\0",1)==0){ // Hay un hueco
			*idx=i;
			return(TRUE);
		}
	}
	return(FALSE);
}
// ________________________________________________________________________________________________________
// Funcin: servidorrembo_existente
//
//		Descripcin:
// 			Esta funcin devuelve true o false dependiendo de si el servidor estnya registrado en  la tabla de sockets
//		Parametros:
//				- ip : La ip delcliente a buscar
//				- idx   Indice que ocuparn el servidor, de existir
// ________________________________________________________________________________________________________
BOOLEAN servidorrembo_existente(char *ip,int* idx)
{
	int i;
	for (i=0;i<MAXIMOS_SRVRMB;i++){
		if (strcmp(ip,tbsocketsSRVRMB[i].ip)==0){ // Si existe la IP ...
			*idx=i;
			return(TRUE);
		}
	}
	return(FALSE);
}
// ________________________________________________________________________________________________________
// Funcin: INTROaFINCAD
//
//		Descripcin:
// 			Cambia INTROS por caracteres fin de cadena ('\0') en una cadena
//		Parametros:
//				- parametros : La cadena a explorar
// ________________________________________________________________________________________________________
void INTROaFINCAD(char* parametros)
{
	int lon,i;
	lon=strlen(parametros);
	for(i=0;i<lon;i++){
		if(parametros[i]=='\r') parametros[i]='\0';
	}
}
// ________________________________________________________________________________________________________
// Funcinn: FINCADaINTRO
//
//		Descripcinn?:
// 			Cambia caracteres fin de cadena ('\0') por INTROS en una cadena
//		Parametros:
//				- parametros : La cadena a explorar
// ________________________________________________________________________________________________________
void FINCADaINTRO(char* a,char *b)
{
	char *i;
	for(i=a;i<b;i++){ // Cambia los NULOS por INTROS
		if(*i=='\0') *i='\r';
	}
}
// ________________________________________________________________________________________________________
// Funcin: cuenta_ipes
//
//		Descripcin:
// 			Cuenta las comas (caracter de separacion) de las cadenas de ipes
//		Parnetros:
//			- parametros : La cadena a explorar
// ________________________________________________________________________________________________________
int cuenta_ipes(char* iph)
{
	int lon,i,cont=1;
	lon=strlen(iph);
	for(i=0;i<lon;i++){ 
		if(iph[i]==';') cont++;
	}
	return(cont);
}
// ________________________________________________________________________________________________________
// Funcin: toma_parametro
// 
//		Descripcin:
// 			Esta funcin devuelve el valor de un parametro incluido en la trama.
// 			El formato del protocolo es: "nombre_parametro=valor_parametro"
// 		Parnetros:
// 			- nombre_parametro: Es el nombre del parnetro a recuperar
// 			- parametros: Es la matriz que contiene todos los parnetros
// ________________________________________________________________________________________________________
char * toma_parametro(char* nombre_parametro,char *parametros)
{
	int i=0;
	char* pos;

	for(i=0;i<LONGITUD_PARAMETROS-4;i++){ 
		if(parametros[i]==nombre_parametro[0]){
			if(parametros[i+1]==nombre_parametro[1]){
				if(parametros[i+2]==nombre_parametro[2]){
					if(parametros[i+3]=='='){
						pos=&parametros[i+4];
						return(pos);
					}
				}
			}
		}
	}
	return(NULL);
}
// ________________________________________________________________________________________________________
// Funcin: split_parametros
//
//		Descripcin:
//			Esta funcin trocea una cadena segn un carnter delimitador, Devuelve el nmero de trozos
// 		Parnetros:
// 			- trozos: Array de punteros a cadenas
// 			- cadena: Cadena a trocear
// 			- ch: Carnter delimitador
// ________________________________________________________________________________________________________
int split_parametros(char **trozos,char *cadena, char * ch){
	int w=0;
	char* token;

	token= strtok(cadena,ch); // Trocea segn delimitador
	while( token != NULL ){
		trozos[w++]=token;
		token=strtok(NULL,ch); // Siguiente token
	}
	trozos[w++]=token; 
	return(w-1); // Devuelve el numero de trozos
}
// ________________________________________________________________________________________________________
// Funcin: corte_iph
// 
//		Descripcin:
// 			Esta funcin devuelve el valor del parametro iph incluido en la trama que debe ser el ltimo parnetro de la trama.
// 	  Parnetros:
// 			- parametros: Parnetros de la trama
// ________________________________________________________________________________________________________
char* corte_iph(char *parametros)
{
	int i=0;
	char nombre_parametro[5];

	strcpy(nombre_parametro,"iph=");
	for(i=0;i<LONGITUD_PARAMETROS-4;i++){ 
		if(parametros[i]==nombre_parametro[0]){
			if(parametros[i+1]==nombre_parametro[1]){
				if(parametros[i+2]==nombre_parametro[2]){
					if(parametros[i+3]=='='){
						return(&parametros[i]); //Devuelve la posicion de comienzo de la iph
					}
				}
			}
		}
	}
	return(NULL);
}
// ________________________________________________________________________________________________________
// Funcin: respuesta_cortesia
//
//	 Descripcin:
//		Envn respuesta de cortesn al cliente rembo
// 	  Parnetros:
// 			- s: Socket usado por el cliente para comunicarse con el servidor HIDRA
// ________________________________________________________________________________________________________
int respuesta_cortesia(SOCKET s)
{
	char nwparametros[100];

	nwparametros[0]='\0';
	strcat(nwparametros,"nfn=Cortesia");
	strcat(nwparametros,"\r"); 
	return(manda_comando(s,nwparametros));
}
// ________________________________________________________________________________________________________
// Funcin: NoComandosPendientes
//
//		Descripcin:
//			Envn respuesta de cortesn al cliente rembo
// 	  Parnetros:
// 			- s: Socket usado por el cliente para comunicarse con el servidor HIDRA
// ________________________________________________________________________________________________________
int NoComandosPendientes(SOCKET s)
{
	char nwparametros[100];

	nwparametros[0]='\0';
	strcat(nwparametros,"nfn=NoComandosPtes");
	strcat(nwparametros,"\r"); 
	return(manda_comando(s,nwparametros));
}
// ________________________________________________________________________________________________________
// Funcin: Iinclusion_cliRMB
//
//		Descripcin:
//			Esta funcin incorpora el socket de un nuevo cliente a la tabla de sockets y le devuelve alguna de sus propiedades: nombre, 
//			dentificador, perfil hardware , mens...
//		Parnetros:
//			- s: Socket del cliente
//			- parametros: Parnetros de la trama recibida
// ________________________________________________________________________________________________________
int inclusion_cliRMB(SOCKET s,char *parametros)
{
	char ErrStr[200],sqlstr[1000];
	Database db;
	Table tbl;

	char *iph,*cfg,*mac,*nau,*nor,*ipr,*ipd;
	int i,lon,glon,idx,resul,puertorepo;
	char nwparametros[LONGITUD_PARAMETROS];
	char ipservidordhcp[16],ipservidorrembo[16],nombreordenador[100];
	int idordenador,idaula,idconfiguracion,idparticion,idperfilhard,idmenu,cache;

	// Toma parnetros
	iph=toma_parametro("iph",parametros); // Toma ip
	mac=toma_parametro("mac",parametros); // Toma mac
	cfg=toma_parametro("cfg",parametros); // Toma configuracion
	nau=toma_parametro("nau",parametros); // Toma nombre del grupo em el           fichero config de rembo
	nor=toma_parametro("nor",parametros); // Toma nombre del ordenador en el  fichero config de rembo
	ipd=toma_parametro("ipd",parametros); // Toma ip del servidor dhcpd
	ipr=toma_parametro("ipr",parametros); // Toma ip del servidor rembo

	// Toma las propiedades del ordenador
	if(!db.Open(usuario,pasguor,datasource,catalog)){ // error de conexion
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	// Recupera los datos del ordenador
	sprintf(sqlstr,"SELECT ordenadores.idordenador,ordenadores.idaula,ordenadores.nombreordenador, ordenadores.idperfilhard, ordenadores.idconfiguracion, idparticion,servidoresdhcp.ip AS ipservidordhcp, servidoresrembo.ip AS ipservidorrembo,servidoresrembo.puertorepo, ordenadores.idmenu,ordenadores.cache FROM ordenadores INNER JOIN servidoresdhcp ON ordenadores.idservidordhcp = servidoresdhcp.idservidordhcp INNER JOIN  servidoresrembo ON ordenadores.idservidorrembo = servidoresrembo.idservidorrembo  WHERE ordenadores.ip = '%s'",iph);

	if(!db.Execute(sqlstr,tbl)){ // Error al consultar
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(tbl.ISEOF()){ // Si No existe registro
		if(aulaup==AUTOINCORPORACION_OFF) // No estnactivada la incorporacin automnica
			return(false);
		if(!cuestion_nuevoordenador(db,tbl,&idordenador,nau,nor,iph,mac,cfg,ipd,ipr)) // Ha habido algn error en la incorporacin automnica
			return(false);
		// Valores por defecto del nuevo ordenador
		strcpy(nombreordenador,nor);
		idperfilhard=0;
		strcpy(ipservidordhcp,ipd);
		strcpy(ipservidorrembo,ipr);
		idmenu=0;
	}
	else{
		if(!tbl.Get("idordenador",idordenador)){ // Toma dato
				tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
				return(false);
		}
		if(!tbl.Get("nombreordenador",nombreordenador)){ // Toma dato 
				tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
				return(false);
		}	
		if(!tbl.Get("idaula",idaula)){ // Toma dato
				tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
				return(false);
		}

		if(!tbl.Get("idconfiguracion",idconfiguracion)){ // Toma dato
				tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
				return(false);
		}
		if(!tbl.Get("idparticion",idparticion)){ // Toma dato
				tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
				return(false);
		}
		if(!tbl.Get("idperfilhard",idperfilhard)){ // Toma dato
				tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
				return(false);
		}
		if(!tbl.Get("ipservidordhcp",ipservidordhcp)){ // Toma dato
				tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
				return(false);
		}
		lon=strlen(ipservidordhcp);
		for (i=0;i<lon;i++){
			if(ipservidordhcp[i]==' ') {
				ipservidordhcp[i]='\0';
				break;
			}
		}
		if(!tbl.Get("ipservidorrembo",ipservidorrembo)){ // Toma dato
				tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
				return(false);
		}
		lon=strlen(ipservidorrembo);
		for (i=0;i<lon;i++){
			if(ipservidorrembo[i]==' ') {
				ipservidorrembo[i]='\0';
				break;
			}
		}
		if(!tbl.Get("puertorepo",puertorepo)){ // Toma dato
				tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
				return(false);
		}		
		
		if(!tbl.Get("idmenu",idmenu)){ // Toma dato
				tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
				return(false);
		}
		if(!tbl.Get("cache",cache)){ // Toma dato
				tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
				return(false);
		}					
		resul=actualiza_configuracion(db,tbl,cfg,idconfiguracion,idparticion,iph); // Actualiza la configuracin del ordenador
		if(!resul){
			pthread_mutex_unlock(&guardia); 
			return(false); 
		}
	}
	// Incluyendo al cliente en la tabla de sokets
	if (cliente_existente(iph,&i)){ // Si ya existe la IP ...
		idx=i;
		//close(tbsockets[idx].sock);
	}
	else{ 
		if (hay_hueco(&i)){ // Busca hueco para el nuevo cliente 
			idx=i;
			strcpy(tbsockets[idx].ip,iph);// Copia IP
		}
		else
			return(false); // No hay huecos
	}
	strcpy(tbsockets[idx].estado,CLIENTE_INICIANDO); // Actualiza el estado del cliente 
	tbsockets[idx].sock=s; // Guarda el socket 
	strcpy(tbsockets[idx].ipsrvdhcp,ipservidordhcp);// Guarda IP servidor dhcp
	strcpy(tbsockets[idx].ipsrvrmb,ipservidorrembo);// Guarda IP servidor rembo

	inclusion_srvRMB(ipservidorrembo,puertorepo); // Actualiza tabla de servidores rembo

	// Prepara la trama
	lon=sprintf(nwparametros,"nfn=RESPUESTA_inclusion_cliRMB\r");
	lon+=sprintf(nwparametros+lon,"ido=%d\r",idordenador);
	lon+=sprintf(nwparametros+lon,"npc=%s\r",nombreordenador);
	lon+=sprintf(nwparametros+lon,"ida=%d\r",idaula);
	lon+=sprintf(nwparametros+lon,"hrd=%s\r",servidorhidra);
	lon+=sprintf(nwparametros+lon,"prt=%d\r",puerto);
	lon+=sprintf(nwparametros+lon,"ifh=%d\r",idperfilhard);
	lon+=sprintf(nwparametros+lon,"che=%d\r",cache);
	lon+=sprintf(nwparametros+lon,"ipr=%s\r",ipservidorrembo);
	lon+=sprintf(nwparametros+lon,"rep=%d\r",puertorepo);
	glon=lon;
	if(!Toma_menu(db,tbl,nwparametros,idmenu,lon)) nwparametros[glon]=(char)NULL;
	db.Close();
	return(manda_comando(s,nwparametros));
}
// ________________________________________________________________________________________________________
// Función: Toma menu
//
//		Descripcin:
//			Esta funcin toma los parametros del menu inicial del cliente rembo y se los envn en el proceso de inclusin
//		Parnetros:
//			- nwparametros: Cadena con los parnetros a enviar al cliente
//			- idmenu: Identificador del men
//			- lon : Longitud inicial de la cadena de parnetros
// ________________________________________________________________________________________________________
int Toma_menu(Database db, Table tbl,char* nwparametros,int idmenu,int lon)
{
	Table littbl;

	char sqlstr[1000],ErrStr[200],titulo[250],descripitem[250],urlimg[250];	
	int idaccionmenu,idtipoaccion,coorx,coory,idurlimg;
	int modalidad,resolucion,tipoaccion,tipoitem;
	char htmlmenupub[250],htmlmenupri[250];

	sprintf(sqlstr,"SELECT menus.resolucion,menus.titulo,menus.coorx,menus.coory,menus.modalidad,menus.scoorx,menus.scoory,menus.smodalidad,menus.htmlmenupub,menus.htmlmenupri,acciones_menus.tipoaccion,acciones_menus.idaccionmenu,acciones_menus.idtipoaccion,acciones_menus.tipoitem,acciones_menus.descripitem,acciones_menus.idurlimg FROM acciones_menus INNER JOIN menus ON acciones_menus.idmenu = menus.idmenu WHERE menus.idmenu=%d order by acciones_menus.orden",idmenu);
		
	if(!db.Execute(sqlstr,tbl)){ // Error al leer
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if (tbl.ISEOF()) return(true);
	
	if(!tbl.Get("titulo",titulo)){ // Toma dato
		tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
		return(false);
	}
	if(!tbl.Get("coorx",coorx)){ // Toma dato
		tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
		return(false);
	}
	if(!tbl.Get("coory",coory)){ // Toma dato
		tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
		return(false);
	}
	if(!tbl.Get("modalidad",modalidad)){ // Toma dato
		tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
		return(false);
	}
	lon+=sprintf(nwparametros+lon,"cmn=%s&%d&%d&%d&",titulo,coorx,coory,modalidad); // Cabecera de menu

	if(!tbl.Get("scoorx",coorx)){ // Toma dato
		tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
		return(false);
	}
	if(!tbl.Get("scoory",coory)){ // Toma dato
		tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
		return(false);
	}
	if(!tbl.Get("smodalidad",modalidad)){ // Toma dato
		tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
		return(false);
	}
	lon+=sprintf(nwparametros+lon,"%d&%d&%d",coorx,coory,modalidad); // Cabecera de menu

	if(!tbl.Get("resolucion",resolucion)){ // Toma dato
		tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
		return(false);
	}
	lon+=sprintf(nwparametros+lon,"&%d\r",resolucion); // Resolucion de la pantalla

	if(!tbl.Get("htmlmenupub",htmlmenupub)){ // Toma dato
		tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
		return(false);
	}
	if(!tbl.Get("htmlmenupri",htmlmenupri)){ // Toma dato
		tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
		return(false);
	}
	lon+=sprintf(nwparametros+lon,"htm=%s;%s\r",htmlmenupub,htmlmenupri); // Html de menu

	lon+=sprintf(nwparametros+lon,"mnu=");
	while(!tbl.ISEOF()){ // Recorre acciones del menu
		if(!tbl.Get("tipoaccion",tipoaccion)){ // Toma dato
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}
		if(!tbl.Get("tipoitem",tipoitem)){ // Toma dato
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}
		if(!tbl.Get("idtipoaccion",idtipoaccion)){ // Toma dato
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}
		if(!tbl.Get("idaccionmenu",idaccionmenu)){ // Toma dato
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}
		if(!tbl.Get("descripitem",descripitem)){ // Toma dato
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}
		if(!tbl.Get("idurlimg",idurlimg)){ // Toma dato
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}
	
		sprintf(sqlstr,"SELECT urlicono FROM iconos WHERE idicono=%d",idurlimg);
		if(!db.Execute(sqlstr,littbl)){ // Error al leer
			db.GetErrorErrStr(ErrStr);
			return(false);
		}
		if (!littbl.ISEOF()){
			if(!littbl.Get("urlicono",urlimg)){ // Toma dato
				littbl.GetErrorErrStr(ErrStr); // error al acceder al registro
				return(false);
			}
		}
		else
			sprintf(urlimg,"itemdefault.pcx");

		lon+=sprintf(nwparametros+lon,"%d&%s&%s&%d&%d\?",idaccionmenu,urlimg,descripitem,tipoitem,tipoaccion);
		tbl.MoveNext();
	}
	nwparametros[lon-1]='\r';
	nwparametros[lon]=(char)NULL;
	return(true);
}
// ________________________________________________________________________________________________________
// Funcin:RecuperaItem
//
//		Descripcin:
//			Esta funcin busca en la base de datos, los parametros de un items de un menu 
//		Parnetros:
//			- s: Socket del cliente
//			- parametros: Parnetros de la trama recibida
// ________________________________________________________________________________________________________
int RecuperaItem(SOCKET s,char *parametros)
{
	char ErrStr[200],sqlstr[1000];
	Database db;
	Table tbl;
	char *ida;
	int idtipoaccion,tipoaccion;

	// Toma parnetros
	ida=toma_parametro("ida",parametros); // Toma identificador de la accin

	//  Abre conexin con la base de datos
	if(!db.Open(usuario,pasguor,datasource,catalog)){ // error de conexion
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	sprintf(sqlstr,"SELECT tipoaccion,idtipoaccion FROM acciones_menus  WHERE idaccionmenu=%s",ida);
	if(!db.Execute(sqlstr,tbl)){ // Error al leer
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if (tbl.ISEOF()) return(false);
	
	if(!tbl.Get("tipoaccion",tipoaccion)){ // Toma tipo de accin
		tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
		return(false);
	}
	if(!tbl.Get("idtipoaccion",idtipoaccion)){ // Toma identificador del tipo de accin
		tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
		return(false);
	}
	switch(tipoaccion){
		case EJECUCION_PROCEDIMIENTO :
			sprintf(sqlstr,"SELECT  procedimientos_comandos.parametros FROM procedimientos_comandos  WHERE procedimientos_comandos.idprocedimiento=%d",idtipoaccion);
			if(!db.Execute(sqlstr,tbl)){ // Error al leer
				db.GetErrorErrStr(ErrStr);
				return(false);
			}
			if(tbl.ISEOF()) // No existe procedimiento
				return(false);

			while(!tbl.ISEOF()){ 
				if(!tbl.Get("parametros",parametros)){ // Toma dato
					tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
					return(false);
				}
				tbl.MoveNext();
			}
			break;
		case EJECUCION_TAREA :
			//Las tareas no se recuperan como fichero de items;
			break;
		case EJECUCION_TRABAJO :
			//Los t rabajos no se recuperan como fichero de items;
			break;
	}
	db.Close();
	return(manda_comando(s,parametros));
}

// ________________________________________________________________________________________________________
// Funcin: actualiza_configuracion
//
//		Descripcin:
//			Esta funcin actualiza la base de datos con la configuracion de sistemas operativos y particiones de un ordenador
//		Parnetros:
//			- db: Objeto base de datos (ya operativo)
//			- tbl: Objeto tabla
//			- cfg: cadena con una configuracin
//			- idcfgo: Identificador de la configuracin actual del ordenador
//			- ipho: Identificador de la configuracin actual de las particiones del ordenador
//			- ipho: Ipe del ordenador
// ________________________________________________________________________________________________________
int actualiza_hardware(Database db, Table tbl,char* hrd,char* ip,char*ido)
{
	int pci,idtipohardware,codigo1,codigo2;
	int i,lon=0;
	char *tbHardware[MAXHARDWARE]; 
	int tbidhardware[MAXHARDWARE]; 
	char *dualHardware[2]; 
	char *codigos[2]; 	
	char ch[2]; // Carnter delimitador
	char sqlstr[1000],ErrStr[200],descripcion[250],nombreordenador[250];
	int idcentro;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ACCESO atnico A TRAVEZ DE OBJETO MUTEX a este trozo de cnigo 
	pthread_mutex_lock(&guardia); 
	
	// Toma Centro 
	sprintf(sqlstr,"SELECT aulas.idcentro,ordenadores.nombreordenador FROM aulas INNER JOIN ordenadores ON aulas.idaula=ordenadores.idaula WHERE ordenadores.idordenador=%s",ido);
	if(!db.Execute(sqlstr,tbl)){ // Error al leer
		db.GetErrorErrStr(ErrStr);
		pthread_mutex_unlock(&guardia); 
		return(false);
	}		
	if(!tbl.Get("idcentro",idcentro)){ // Toma dato 
		tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
		pthread_mutex_unlock(&guardia); 
		return(false);
	}			
	if(!tbl.Get("nombreordenador",nombreordenador)){ // Toma dato 
		tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
		pthread_mutex_unlock(&guardia); 
		return(false);
	}			
	
	if(lon>MAXHARDWARE) lon=MAXHARDWARE;
		// Trocea la cadena de configuracin
	strcpy(ch,"\n");// caracter delimitador 
	lon=split_parametros(tbHardware,hrd,ch);
	
	// Trocea las cadenas de parametros de particin
	for (i=0;i<lon;i++){
		strcpy(ch,"=");// caracter delimitador "="
		split_parametros(dualHardware,tbHardware[i],ch); // Nmero de particin
		sprintf(sqlstr,"SELECT idtipohardware,pci,descripcion FROM tipohardwares WHERE nemonico='%s'",dualHardware[0]);
		if(!db.Execute(sqlstr,tbl)){ // Error al leer
			db.GetErrorErrStr(ErrStr);
			pthread_mutex_unlock(&guardia); 
			return(false);
		}		
		if(tbl.ISEOF()){ //  Tipo de Hardware NO existente
			pthread_mutex_unlock(&guardia); 
			return(false);
		}
		else{  //  Tipo de Hardware Existe
			if(!tbl.Get("idtipohardware",idtipohardware)){ // Toma dato si es hardware pci
				tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
				pthread_mutex_unlock(&guardia); 
				return(false);
			}
			if(!tbl.Get("descripcion",descripcion)){ // Toma dato si es hardware pci
				tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
				pthread_mutex_unlock(&guardia); 
				return(false);
			}			
			if(!tbl.Get("pci",pci)){ // Toma dato si es hardware pci
				tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
				pthread_mutex_unlock(&guardia); 
				return(false);
			}			
			if(pci){ // Hardware pci
				strcpy(ch,":");// caracter delimitador "="
				split_parametros(codigos,dualHardware[1],ch); // Nmero de particin
				sprintf(sqlstr,"SELECT idhardware FROM hardwares WHERE idtipohardware=%d AND codigo1=0x%s AND codigo2=0x%s",idtipohardware,codigos[0],codigos[1]);
			}
			else // Hardware NO pci
				sprintf(sqlstr,"SELECT idhardware FROM hardwares WHERE idtipohardware=%d AND descripcion='%s'",idtipohardware,dualHardware[1]);
			
			// EJecuta consulta
			if(!db.Execute(sqlstr,tbl)){ // Error al leer
				db.GetErrorErrStr(ErrStr);
				pthread_mutex_unlock(&guardia); 
				return(false);
			}	
			if(tbl.ISEOF()){ //  Hardware NO existente
				if(pci){ // Hardware pci
					sprintf(sqlstr,"SELECT * FROM pcifabricantes WHERE codigo1=0x%s AND codigo2=0x%s",codigos[0],codigos[1]);
					if(!db.Execute(sqlstr,tbl)){ // Error al leer
						db.GetErrorErrStr(ErrStr);
						pthread_mutex_unlock(&guardia); 
						return(false);
					}	
					if(tbl.ISEOF()){ //  Hardware NO existente
						strcat(descripcion,"(Desconocido)");
						sprintf(sqlstr,"INSERT pcifabricantes (descripcion,codigo1,codigo2) VALUES('%s',0x%s,0x%s)",descripcion,codigos[0],codigos[1]);
						if(!db.Execute(sqlstr,tbl)){ // Error al insertar
							db.GetErrorErrStr(ErrStr);
							pthread_mutex_unlock(&guardia); 
							return(false);
						}
						sprintf(sqlstr,"INSERT hardwares (idtipohardware,descripcion,idcentro,grupoid,codigo1,codigo2) VALUES(%d,'%s',%d,0,0x%s,0x%s)",idtipohardware,descripcion,idcentro,codigos[0],codigos[1]);
					}							
					else{				
						if(!tbl.Get("codigo1",codigo1)){ // Toma dato
							tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
							pthread_mutex_unlock(&guardia); 
							return(false);
						}			
						if(!tbl.Get("codigo2",codigo2)){ // Toma dato
							tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
							pthread_mutex_unlock(&guardia); 
							return(false);
						}		
						if(!tbl.Get("descripcion",descripcion)){ // Toma dato
							tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
							pthread_mutex_unlock(&guardia); 
							return(false);
						}	
						sprintf(sqlstr,"INSERT hardwares (idtipohardware,descripcion,idcentro,grupoid,codigo1,codigo2) VALUES(%d,'%s',%d,0,%d,%d)",idtipohardware,descripcion,idcentro,codigo1,codigo2);
					}															
				}
				else
					sprintf(sqlstr,"INSERT hardwares (idtipohardware,descripcion,idcentro,grupoid,codigo1,codigo2) VALUES(%d,'%s',%d,0,0,0)",idtipohardware,dualHardware[1],idcentro);
				if(!db.Execute(sqlstr,tbl)){ // Error al insertar
					db.GetErrorErrStr(ErrStr);
					pthread_mutex_unlock(&guardia); 
					return(false);
				}		
				// Recupera el identificador del hardware	
				sprintf(sqlstr,"SELECT LAST_INSERT_ID() as identificador");
				if(!db.Execute(sqlstr,tbl)){ // Error al leer
					db.GetErrorErrStr(ErrStr);
					pthread_mutex_unlock(&guardia); 
					return(false);
				}
				if(!tbl.ISEOF()){ // Si existe registro
					if(!tbl.Get("identificador",tbidhardware[i])){
							tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
							pthread_mutex_unlock(&guardia); 
							return(false);
					}
				}					
			}
			else{
				if(!tbl.Get("idhardware",tbidhardware[i])){ // Toma dato si es hardware pci
					tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
					pthread_mutex_unlock(&guardia); 
					return(false);
				}							
			}
		}	// Fin for 
	}
	 // Comprueba existencia de perfil hardware y actualización de éste para el ordenador
	if(!CuestrionPerfilHardware(db, tbl,idcentro,ido,tbidhardware,i,nombreordenador)){
		tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
		pthread_mutex_unlock(&guardia); 
		return(false);
	}	
	pthread_mutex_unlock(&guardia); 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
	return(true);
}
// ________________________________________________________________________________________________________
// Funcin: CuestrionPerfilHardware
//________________________________________________________________________________________________________/
int CuestrionPerfilHardware(Database db, Table tbl,int idcentro,char* ido,int *tbidhardware,int i,char *nombreordenador){
	char sqlstr[1000],ErrStr[200];
	int tbidhardwareperfil[MAXHARDWARE]; 
	int j=0;
	int idperfilhard;
	// Busca perfil hard del ordenador
	sprintf(sqlstr,"SELECT perfileshard_hardwares.idhardware FROM ordenadores INNER JOIN perfileshard ON ordenadores.idperfilhard = perfileshard.idperfilhard	INNER JOIN perfileshard_hardwares ON perfileshard_hardwares.idperfilhard = perfileshard.idperfilhard WHERE ordenadores.idordenador =%s",ido);
	// EJecuta consulta
	if(!db.Execute(sqlstr,tbl)){ // Error al leer
		db.GetErrorErrStr(ErrStr);
		return(false);
	}		
	while(!tbl.ISEOF()){ // Recorre acciones del menu
		if(!tbl.Get("idhardware",tbidhardwareperfil[j++])){ // Toma dato si es hardware pci
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}		
		tbl.MoveNext();
	}
	// Comprueba si el perfil del ordenador contiene todo el hardware enviado
	int k,q,sw=false;
	for(k=0;k<i;k++){ // Elemento hardware
		for(q=0;q<j;q++){
			if(tbidhardware[k]==tbidhardwareperfil[q]){
				sw=true;
				break;
			}
		}
		if(!sw)	break;			
	}
	// La variable sw contiene false si se ha encontrado algún hardware que no está en el perfil hardware del ordenador
	if(sw) return(true); // Todo el hardware está en el perfil actual
	
	// Crea perfil nuevo con todo el hardware inventariado
	sprintf(sqlstr,"INSERT perfileshard  (descripcion,idcentro,grupoid) VALUES('Perfil (%s)',%d,0)",nombreordenador,idcentro);
	if(!db.Execute(sqlstr,tbl)){ // Error al insertar
		db.GetErrorErrStr(ErrStr);
		return(false);
	}		
	// Recupera el identificador del hardware	
	sprintf(sqlstr,"SELECT LAST_INSERT_ID() as identificador");
	if(!db.Execute(sqlstr,tbl)){ // Error al leer
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(!tbl.ISEOF()){ // Si existe registro
		if(!tbl.Get("identificador",idperfilhard)){
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}
	}	
	for(k=0;k<i;k++){ // relaciona elementos hardwares con el nuevo perfil hardware
		sprintf(sqlstr,"INSERT perfileshard_hardwares  (idperfilhard,idhardware) VALUES(%d,%d)",idperfilhard,tbidhardware[k]);
		if(!db.Execute(sqlstr,tbl)){ // Error al insertar
			db.GetErrorErrStr(ErrStr);
			return(false);
		}		
	}				
	sprintf(sqlstr,"UPDATE 	ordenadores SET idperfilhard=%d WHERE idordenador=%s",idperfilhard,ido);
	if(!db.Execute(sqlstr,tbl)){ // Error al insertar
		db.GetErrorErrStr(ErrStr);
		return(false);
	}			
	return(true);
}
// ________________________________________________________________________________________________________
// Funcin: actualiza_configuracion
//
//		Descripcin:
//			Esta funcin actualiza la base de datos con la configuracion de sistemas operativos y particiones de un ordenador
//		Parnetros:
//			- db: Objeto base de datos (ya operativo)
//			- tbl: Objeto tabla
//			- cfg: cadena con una configuracin
//			- idcfgo: Identificador de la configuracin actual del ordenador
//			- ipho: Identificador de la configuracin actual de las particiones del ordenador
//			- ipho: Ipe del ordenador
// ________________________________________________________________________________________________________
int actualiza_configuracion(Database db, Table tbl,char* cfg,int idcfgo,int idprto,char* ipho)
{
	char sqlstr[1000],ErrStr[200];	
	int idconfiguracion,idparticion,lon;
	char * part;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ACCESO atnico A TRAVEZ DE OBJETO MUTEX a este trozo de cnigo 
	pthread_mutex_lock(&guardia); 
	sprintf(sqlstr,"SELECT idconfiguracion FROM configuraciones WHERE configuracion LIKE '%s'",cfg);
	if(!db.Execute(sqlstr,tbl)){ // Error al leer
		db.GetErrorErrStr(ErrStr);
				pthread_mutex_unlock(&guardia); 
		return(false);
	}
	if(!tbl.ISEOF()){ // Configuracin ya existente
		if(!tbl.Get("idconfiguracion",idconfiguracion)){ // Toma dato
				tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
						pthread_mutex_unlock(&guardia); 
				return(false);
		}
	}
	else{ // Nueva configuracin
			sprintf(sqlstr,"INSERT configuraciones (configuracion) VALUES('%s')",cfg);
			if(!db.Execute(sqlstr,tbl)){ // Error al insertar
				db.GetErrorErrStr(ErrStr);
				pthread_mutex_unlock(&guardia); 
				return(false);
			}
			sprintf(sqlstr,"SELECT LAST_INSERT_ID() as identificador");
			if(!db.Execute(sqlstr,tbl)){ // Error al leer
				db.GetErrorErrStr(ErrStr);
				pthread_mutex_unlock(&guardia); 
				return(false);
			}
			if(!tbl.ISEOF()){ // Si existe registro
				if(!tbl.Get("identificador",idconfiguracion)){
						tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
						pthread_mutex_unlock(&guardia); 
						return(false);
				}
			}
	}
	// Genera cadena de particiones
	lon=strlen(cfg);
	part=(char*)malloc(lon); 
	TomaParticiones(cfg,part,lon);
	sprintf(sqlstr,"SELECT idparticion FROM particiones WHERE particion LIKE '%s'",part);
	if(!db.Execute(sqlstr,tbl)){ // Error al leer
		db.GetErrorErrStr(ErrStr);
		pthread_mutex_unlock(&guardia); 
		return(false);
	}
	if(!tbl.ISEOF()){ // Configuracin ya existente
		if(!tbl.Get("idparticion",idparticion)){ // Toma dato
				tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
						pthread_mutex_unlock(&guardia); 
				return(false);
		}
	}
	else{ // Nueva particion
			sprintf(sqlstr,"INSERT particiones (particion) VALUES('%s')",part);
			if(!db.Execute(sqlstr,tbl)){ // Error al insertar
				db.GetErrorErrStr(ErrStr);
						pthread_mutex_unlock(&guardia); 
				return(false);
			}
			sprintf(sqlstr,"SELECT LAST_INSERT_ID() as identificador");
			if(!db.Execute(sqlstr,tbl)){ // Error al leer
				db.GetErrorErrStr(ErrStr);
						pthread_mutex_unlock(&guardia); 
				return(false);
			}
			if(!tbl.ISEOF()){ // Si existe registro
				if(!tbl.Get("identificador",idparticion)){
						tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
								pthread_mutex_unlock(&guardia); 
						return(false);
				}
			}
	}			
	if(idconfiguracion!=idcfgo ||  idparticion!=idprto){ // Si el odenador tiene una configuracin distinta ...
		sprintf(sqlstr,"Update ordenadores set idconfiguracion=%d, idparticion=%d WHERE ip='%s'",idconfiguracion,idparticion,ipho);
		if(!db.Execute(sqlstr,tbl)){ // Error al actualizar
			db.GetErrorErrStr(ErrStr);
					pthread_mutex_unlock(&guardia); 
			return(false);
		}
	}
			pthread_mutex_unlock(&guardia); 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return(true);
}
// ________________________________________________________________________________________________________
// Funcin: TomaParticiones
//
//		Descripcin:
//			Esta funcin compone basndose en la cadena de configuracin que devuelve el ordenador, una cadena de particiones con 
//			los valores "n0=PPPP;n1=PPPP..."  con las duplas:el nmero de particin y el tipo, separados por coma
//		Parnetros:
//			- cfg: Cadena de configuracin
//			- parts: Cadena devuelta con el formato anterior descrito 
//			- lonprt: Longitud mnmima para las cadenas 
// ________________________________________________________________________________________________________
void TomaParticiones(char* cfg, char* parts,int lonprt)
{
	int i;
	int lon=0;
	char *tbParticiones[10]; // Para albergar hasta 10 particiones ( Normalmente Mnimo 8);
	char *tbParticion[8]; // Para albergar hasta 8 parnetros de particin;
	char *tbIgualdad[2]; // Para albergar hasta 8 parnetros de particin;
	char ch[2]; // Carnter delimitador
	char *apun;

	// Toma memoria para cada elemento de particin
	for(i=0;i<10;i++)
		tbParticiones[i]=(char*)malloc(lonprt); 

	// Toma memoria para cada parametro de particin
	for(i=0;i<8;i++)
		tbParticion[i]=(char*)malloc(lonprt); 

	// Toma memoria para cada igualdad
	for(i=0;i<2;i++)
		tbIgualdad[i]=(char*)malloc(20);  

	// Trocea la cadena de configuracin
	strcpy(ch,"\t");// caracter delimitador (tabulador)
	lonprt=split_parametros(tbParticiones,cfg,ch);
	// Trocea las cadenas de parametros de particin
	for (i=0;i<lonprt;i++){
		strcpy(ch,"\n");// caracter delimitador (salto de linea)
		split_parametros(tbParticion,tbParticiones[i],ch);
		strcpy(ch,"=");// caracter delimitador "="
		split_parametros(tbIgualdad,tbParticion[4],ch); // Nmero de particin
		lon+=sprintf(parts+lon,"%s=",tbIgualdad[1]);
		split_parametros(tbIgualdad,tbParticion[2],ch); // Tipo de particion
		apun=tbIgualdad[1];
		//if(apun[0]=='H') apun++; // Si es oculta ...
		lon+=sprintf(parts+lon,"%s;",apun);
	}
	lon+=sprintf(parts+lon,"@prt");
}
// ________________________________________________________________________________________________________
// Funcin: COMANDOSpendientes
//
//		Descripcin:
//			Esta funcin busca en la base de datos,comandos pendientes de ejecutar por un  ordenador  concreto
//		Parnetros:
//			- s: Socket del cliente
//			- parametros: Parnetros de la trama recibida
// ________________________________________________________________________________________________________
int COMANDOSpendientes(SOCKET s,char *parametros)
{
	char *iph,*ido,*coletilla;
	int ids;
	char pids[20],ipe[16],idord[16];

	iph=toma_parametro("iph",parametros); // Toma ip
	ido=toma_parametro("ido",parametros); // Toma identificador del ordenador
	strcpy(ipe,iph);
	strcpy(idord,ido);

	if(busca_comandos(ipe,idord,parametros,&ids)){
		Coloca_estado(ipe,CLIENTE_OCUPADO,s);
		//Manda el comando pendiente
		coletilla=corte_iph(parametros);
		coletilla[0]='\0';// Corta la trama en la ip
		sprintf(pids,"ids=%d\r",ids);
		strcat(parametros,pids); // Le ande el identificador de la accion
		return(manda_comando(s,parametros));
	}
	NoComandosPendientes(s);  // Indica al cliente rembo que ya no hay mn comandos pendientes
	return(true);
}
// ________________________________________________________________________________________________________
// Funcin: EjecutarItem
//
//		Descripcin:
//			Esta funcin ejecuta un item de un men concreto solicitado por algn cliente rembo
//		Parnetros:
//			- s: Socket del cliente
//			- parametros: Parnetros de la trama recibida
// ________________________________________________________________________________________________________
int EjecutarItem(SOCKET s,char *parametros)
{
	char sqlstr[1000],ErrStr[200];
	Database db;
	Table tbl,tbln;
	int idtipoaccion,lon,cont_comandos=0,i,puertorepo;
	char tipoaccion,*iph,*idt,ipe[16];
	char *tbComandosparametros[100];

	iph=toma_parametro("iph",parametros); // Toma ip
	idt=toma_parametro("idt",parametros); // Toma idemtificador del item
	strcpy(ipe,iph);

	if(!db.Open(usuario,pasguor,datasource,catalog)){ // error de conexion
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	sprintf(sqlstr,"SELECT acciones_menus.tipoaccion, acciones_menus.idtipoaccion FROM acciones_menus WHERE acciones_menus.idaccionmenu=%s",idt);
	if(!db.Execute(sqlstr,tbl)){ // Error al leer
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(tbl.ISEOF()){
		return(false);  // No hay comandos pendientes
	}

	if(!tbl.Get("tipoaccion",tipoaccion)){ // Toma dato
		tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
		return(false);
	}

	if(!tbl.Get("idtipoaccion",idtipoaccion)){ // Toma dato
		tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
		return(false);
	}
	
	switch(tipoaccion){
		case EJECUCION_PROCEDIMIENTO :
			sprintf(sqlstr,"SELECT  procedimientos_comandos.parametros  FROM  procedimientos_comandos  WHERE procedimientos_comandos.idprocedimiento=%d",idtipoaccion);
			if(!db.Execute(sqlstr,tbl)){ // Error al leer
				db.GetErrorErrStr(ErrStr);
				return(false);
			}
			if(tbl.ISEOF()) // No existe procedimiento
				return(false);

			while(!tbl.ISEOF()){ 
				if(!tbl.Get("parametros",parametros)){ // Toma dato
					tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
					return(false);
				}
				lon=strlen(parametros);
				tbComandosparametros[cont_comandos]=(char*)malloc(lon);
				if(tbComandosparametros[cont_comandos]==NULL)
					return(false); // No hay memoria bastante
				strcpy(tbComandosparametros[cont_comandos++],parametros);
				tbl.MoveNext();
			}
			strcpy(parametros,tbComandosparametros[0]);
			strcat(parametros,"iph=");
			strcat(parametros,ipe);
			strcat(parametros,"\r");
			for(i=1;i<cont_comandos;i++){
				strcat(parametros,"\n");
				strcat(parametros,tbComandosparametros[i]);
				strcat(parametros,"iph=");
				strcat(parametros,ipe);
				strcat(parametros,"\r");
			}
			if(TomaIPServidorRembo(ipe,&puertorepo))
				return(manda_trama_servidorrembo(ipe,parametros,puertorepo));
			break;
		case EJECUCION_TAREA :
			EjecutarTarea(idtipoaccion,0,0,0,db,parametros);
			break;
		case EJECUCION_TRABAJO :
			EjecutarTrabajo(idtipoaccion,db,parametros); // Es una programacin de un trabajo
			break;
	}
	db.Close();
	return(true);
}
// ________________________________________________________________________________________________________
// Funcin: disponibilidadCOMANDOS
//
//		Descripcin:
//			Esta funcin habilita a un clinte rembo para recibir o no, comandos iteractivos
//		Parnetros:
//			- s: Socket del cliente
//			- parametros: Parmetros de la trama recibida
// ________________________________________________________________________________________________________
int disponibilidadCOMANDOS(SOCKET s,char *parametros)
{
	char *iph,*swd;
	int resul=0,i;

	iph=toma_parametro("iph",parametros); // Toma ip
	swd=toma_parametro("swd",parametros); // Toma switch de diponibilidad

	if(strcmp(swd,"1")==0) // Cliente disponible
		resul=Coloca_estado(iph,CLIENTE_REMBO,s);
	else{
		if (cliente_existente(iph,&i)) // Si ya existe la IP ...
			resul=borra_entrada(i); // Cliente apagado
	}
	return(resul);
}
// ________________________________________________________________________________________________________
// Funcin: Coloca_estado
//
//		Descripcin: 
//			Esta funcin coloca el estado de un ordenador en la tabla de sockets
//		Parnetros:
//			- iph: Ip del ordenador
//			- e: Nuevo estado
//			- s: Socket usado por el cliente para comunicarse con el servidor HIDRA
// ________________________________________________________________________________________________________
int Coloca_estado(char *iph,char *e,SOCKET s)
{
	int i;
	for (i=0;i<MAXIMOS_SOCKETS;i++){
		if (strncmp(tbsockets[i].ip,"\0",1)!=0){	 // Si es un cliente activo
			if (IgualIP(iph,tbsockets[i].ip)){	// Si existe la IP en la cadena
				strcpy(tbsockets[i].estado,e);	// Cambia el estado 
				tbsockets[i].sock=s;		// Guarda el socket 
				return(true);
			}
		}
	}
	return(false);
}
// ________________________________________________________________________________________________________
// Funcin: IgualIP
//
//		 Descripcin: 
//			Comprueba si una cadena con una ipe estnincluidad en otra que  contienen varias direcciones ipes separas por punto y coma
//		Parnetros:
//			- cadenaiph: Cadena de IPes
//			- ipcliente: Cadena de la ip a buscar
// ________________________________________________________________________________________________________
BOOLEAN IgualIP(char *cadenaiph,char *ipcliente)
{
	char *posa,*posb;
	int lon;

	posa=strstr(cadenaiph,ipcliente);
	if(posa==NULL) return(FALSE); // No existe la IP en la cadena
	posb=posa; // Iguala direcciones
	while(TRUE){
		posb++;
		if(*posb==';') break;
		if(*posb=='\0') break;
		if(*posb=='\r') break;
	}
	lon=strlen(ipcliente);
	if((posb-posa)==lon) return(TRUE); // IP encontrada !!!!
		
	return(FALSE);
}
// ________________________________________________________________________________________________________
// Funcin: inclusion_srvRMB
//
//		Descripcin:
//			Esta funcin incorpora el socket de un nuevo servidor rembo a la tabla de sockets
//		Parnetros:
//			- s: Socket del servidor rembo
//			- parametros: Parnetros de la trama recibida
// ________________________________________________________________________________________________________
int inclusion_srvRMB(char *iphsrvrmb,int puertorepo)
{
	int i,idx;
	
	// Incluyendo al cliente en la tabla de sokets
	if (servidorrembo_existente(iphsrvrmb,&i)){ // Si ya existe la IP ...
		idx=i;
	}
	else{ 
		if (hay_huecoservidorrembo(&i)){ // Busca hueco para el nuevo cliente 
			idx=i;
			strcpy(tbsocketsSRVRMB[idx].ip,iphsrvrmb);// Copia IP
			tbsocketsSRVRMB[idx].puertorepo=puertorepo;
		}
		else
			return(false); // No hay huecos
	}
	return(true);
}
// ________________________________________________________________________________________________________
// Funcin: inclusion_cliWINLNX
//
//		 Descripcin:
//			Esta funcin incorpora el socket de un nuevo cliente rembo a la tabla de sockets
//		Parnetros:
//			- s: Socket del servidor rembo
//			- parametros: Parnetros de la trama recibida
// ________________________________________________________________________________________________________
int inclusion_cliWINLNX(SOCKET s,char *parametros)
{
	char *iph,*tso;
	int i,idx;

	// Toma parnetros
	iph=toma_parametro("iph",parametros); // Toma ip
	tso=toma_parametro("tso",parametros); // Toma ip
	// Incluyendo al cliente en la tabla de sokets
	if (cliente_existente(iph,&i)){ // Si ya existe la IP ...
		idx=i;
		close(tbsockets[idx].sock);
	}
	else{ 
		if (hay_hueco(&i)){ // Busca hueco para el nuevo cliente 
			idx=i;
			strcpy(tbsockets[idx].ip,iph);// Copia IP
		}
		else
			return(false); // No hay huecos
	}
	tbsockets[idx].sock=s; // Guarda el socket 
	strcpy(tbsockets[idx].estado,tso);
	return(true);
}
// ________________________________________________________________________________________________________
// Funcin: inclusion_REPO
//
//		 Descripcin:
//			Esta funcin incorpora el socket de un nuevo repositorio hidra
// ________________________________________________________________________________________________________
int inclusion_REPO(SOCKET s,char *parametros)
{
	char ErrStr[200],sqlstr[1000];
	Database db;
	Table tbl;
	
	char *iph;
	char PathHidra[1024]; // path al directorio base de Hidra
	int puertorepo,lon;
	
	// Toma parnetros
	iph=toma_parametro("iph",parametros); // Toma ip
	
	// Toma las propiedades del ordenador
	if(!db.Open(usuario,pasguor,datasource,catalog)){ // error de conexion
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	// Recupera los datos del ordenador
	sprintf(sqlstr,"SELECT puertorepo,pathrembod FROM servidoresrembo WHERE ip = '%s'",iph);

	if(!db.Execute(sqlstr,tbl)){ // Error al consultar
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(tbl.ISEOF()) // Si No existe registro
		return(false);

	if(!tbl.Get("puertorepo",puertorepo)){ // Toma dato
		tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
		return(false);
	}
	if(!tbl.Get("pathrembod",PathHidra)){ // Toma dato
		tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
		return(false);
	}
	
	inclusion_srvRMB(iph,puertorepo); // Actualiza tabla de servidores rembo
	
	TRAMA *trama=(TRAMA*)malloc(LONGITUD_TRAMA);
	if(!trama)
		return(false);
	// Envia la trama
	trama->arroba='@';
	strncpy(trama->identificador,"JMMLCAMDJ",9);
	trama->ejecutor='1';
	lon=sprintf(trama->parametros,"nfn=RESPUESTA_inclusionREPO\r");
	lon+=sprintf(trama->parametros+lon,"prp=%d\r",puertorepo);
	lon+=sprintf(trama->parametros+lon,"pth=%s\r",PathHidra);
	lon+=sprintf(trama->parametros+lon,"usu=%s\r",usuario);	
	lon+=sprintf(trama->parametros+lon,"pwd=%s\r",pasguor);
	lon+=sprintf(trama->parametros+lon,"dat=%s\r",datasource);
	lon+=sprintf(trama->parametros+lon,"cat=%s\r",catalog);			
	return(manda_trama(s,trama));
}
// ________________________________________________________________________________________________________
// Funcin: Sondeo
//
//		Descripcin: 
//			Esta funcin recupera el estado de los ordenadores solicitados
//		Parnetros:
//			- s: Socket del servidor web que envn el comando
//			- parametros: Parnetros de la trama enviada por nte
// ________________________________________________________________________________________________________
int Sondeo(SOCKET s,char *parametros)
{
	char *iph;
	char nwparametros[LONGITUD_PARAMETROS];
	int j;

	iph=toma_parametro("iph",parametros); // Toma ip
    nwparametros[0]='\0';
	strcat(nwparametros,"tso="); // Compone retorno tso ( sistemas operativos de los clientes )
    for (j=0;j<MAXIMOS_SOCKETS;j++){
		if (strncmp(tbsockets[j].ip,"\0",1)!=0){ // Si es un cliente activo
			if (IgualIP(iph,tbsockets[j].ip)){ // Si existe la IP en la cadena
				 strcat( nwparametros, tbsockets[j].ip); // Compone retorno
				 strcat( nwparametros,"/");			// "ip=sistemaoperatico;"
				 strcat( nwparametros, tbsockets[j].estado);
				 strcat( nwparametros,";");
			}
		}
	}
	return(manda_comando(s,nwparametros));
}
// ________________________________________________________________________________________________________
// Funcin: Actualizar
//
//		Descripcin:
//			Esta funcin actualiza la vista de ordenadores
//		Parnetros:
//			- parametros: parametros del comando
// ________________________________________________________________________________________________________
int Actualizar(char *parametros)
{
	TRAMA *trama=(TRAMA*)malloc(LONGITUD_TRAMA);
	if(!trama)return(false);
	int i,estado_cliente,lon;
	char *iph,*rmb;

	iph=toma_parametro("iph",parametros); // Toma ip
	rmb=toma_parametro("rmb",parametros); // Toma ipe del servidor rembo
	for (i=0;i<MAXIMOS_SOCKETS;i++){
		if (strncmp(tbsockets[i].ip,"\0",1)!=0){ // Si es un cliente activo
			if (IgualIP(iph,tbsockets[i].ip)){ // Si existe la IP en la cadena
				estado_cliente=strcmp(tbsockets[i].estado,CLIENTE_OCUPADO);
				if(estado_cliente!=0){ // Cliente NO OCUPADO ...
					estado_cliente=strcmp(tbsockets[i].estado,CLIENTE_INICIANDO);
					if(estado_cliente!=0){ // Cliente NO INICIANDO ...
						estado_cliente=strcmp(tbsockets[i].estado,CLIENTE_REMBO);
						if(estado_cliente!=0){ // Cliente windows o linux ...
							lon=sprintf(trama->parametros,"nfn=Actualizar\r");
							manda_comando(tbsockets[i].sock,(char*)trama->parametros);
						}
						borra_entrada(i);
					}
				}
			}
		}
	}
	int j;
	for (j=0;j<MAXIMOS_SRVRMB;j++){
		if (strcmp(rmb,tbsocketsSRVRMB[j].ip)==0){ // Si existe la IP ...
			FINCADaINTRO(parametros,iph);
			return(manda_trama_servidorrembo(rmb,parametros,tbsocketsSRVRMB[j].puertorepo));
		}
	}
	return(false);
}
// ________________________________________________________________________________________________________
// Funcin: FicheroOperador
//
//		Descripcin:
//			Esta funcin envia al servidor datos de un operador para crear fichero de login
//		Parnetros:
//			- parametros: parametros del comando
// ________________________________________________________________________________________________________
int FicheroOperador(char *parametros)
{
	TRAMA trama;		
	SOCKET s;
	char *rmb,*amb,*usu,*psw,*ida;
	int resul,lon;

	rmb=toma_parametro("rmb",parametros); // Toma ipe del servidor rembo

	// Abre conexion con el servidor rembo y envia trama
	s=AbreConexion(rmb,puerto+1);
	if(!s){
		RegistraLog("Fallo al conectar con el servidor rembo para envio de tramas",true);
		return(FALSE);
	}

	amb=toma_parametro("amb",parametros); // Toma tipo de operacion
	usu=toma_parametro("usu",parametros); // Toma usuario
	psw=toma_parametro("psw",parametros); // Toma passwrod
	ida=toma_parametro("ida",parametros); // Toma identificador del aula

	// Envia la trama
	trama.arroba='@';
	strncpy(trama.identificador,"JMMLCAMDJ",9);
	trama.ejecutor='1';
	lon=sprintf(trama.parametros,"nfn=FicheroOperador\r");
	lon+=sprintf(trama.parametros+lon,"amb=%s\r",amb);
	lon+=sprintf(trama.parametros+lon,"usu=%s\r",usu);
	lon+=sprintf(trama.parametros+lon,"psw=%s\r",psw);
	lon+=sprintf(trama.parametros+lon,"ida=%s\r",ida);
	resul=(manda_trama(s,&trama));
	if(!resul)
		RegistraLog("Fallo en el envio de trama al servidor rembo",true);
	return(resul);
}
// ________________________________________________________________________________________________________
// Funcin: Conmutar
//
//		Descripcin:
//			Esta funcin conmuta un cliente rembo del modo NO administrado al modo admnistrado
//		Parnetros:
//			- parametros: parametros del comando
// ________________________________________________________________________________________________________
int Conmutar(char *parametros)
{
	TRAMA trama;		
	SOCKET s;
	int i,estado_cliente,lon,resul;
	char *iph,*rmb;

	iph=toma_parametro("iph",parametros); // Toma ip
	rmb=toma_parametro("rmb",parametros); // Toma ipe del servidor rembo
	for (i=0;i<MAXIMOS_SOCKETS;i++){
		if (strncmp(tbsockets[i].ip,"\0",1)!=0){ // Si es un cliente activo
			if (IgualIP(iph,tbsockets[i].ip)){ // Si existe la IP en la cadena
				estado_cliente=strcmp(tbsockets[i].estado,CLIENTE_OCUPADO);
				if(estado_cliente!=0){ // Cliente NO OCUPADO ...
					estado_cliente=strcmp(tbsockets[i].estado,CLIENTE_INICIANDO);
					if(estado_cliente!=0){ // Cliente NO INICIANDO ...
						estado_cliente=strcmp(tbsockets[i].estado,CLIENTE_REMBO);
						if(estado_cliente!=0){ // Cliente windows o linux ...
							lon=sprintf(trama.parametros,"nfn=Conmutar\r");
							manda_comando(tbsockets[i].sock,trama.parametros);
						}
					}
				}
			}
		}
	}

	// Abre conexion con el servidor rembo y envia trama
	s=AbreConexion(rmb,puerto+1);
	if(!s){
		RegistraLog("Fallo al conectar con el servidor rembo para envio de tramas",true);
		resul=FALSE;
	}
	else{
		// Envia la trama
		trama.arroba='@';
		strncpy(trama.identificador,"JMMLCAMDJ",9);
		trama.ejecutor='2';
		lon=sprintf(trama.parametros,"nfn=Conmutar\r");
		lon+=sprintf(trama.parametros+lon,"iph=%s\r",iph);
		resul=(manda_trama(s,&trama));
		if(!resul){
			RegistraLog("Fallo en el envio de trama al servidor rembo",true);
		}
	}
	return(resul);
}
// ________________________________________________________________________________________________________
// Funcin: PurgarTablaSockets
//
//		Descripcin:
//			Borra ordenadores de la tabla de sockets
//		Parnetros:
//			- parametros: parametros del comando
// ________________________________________________________________________________________________________
void PurgarTablaSockets(char *parametros)
{
	int i;
	char *iph;

	iph=toma_parametro("iph",parametros); // Toma ip
    for (i=0;i<MAXIMOS_SOCKETS;i++){
		if (strncmp(tbsockets[i].ip,"\0",1)!=0){ // Si es un cliente activo
			if (IgualIP(iph,tbsockets[i].ip)){ // Si existe la IP en la cadena
				borra_entrada(i);
			}
		}
	}
}
// _____________________________________________________________________________________________________________
// Función: Arrancar
//
//		Descripción:
//			Esta función arranca los ordenadores solicitados. PAra ello le envía el comando arrancar al servidor rembo que lo controla y
//			es éste el que le envía la trama de wake-up
//		Parámetros:
//			- mac: Dirección mac del cliente rembo
//			- iph: Dirección ip del cliente rembo
//			- rmb: ip del servidor rembo
// _____________________________________________________________________________________________________________
int Arrancar(char *parametros)
{
	TRAMA *trama=(TRAMA*)malloc(LONGITUD_TRAMA);
	if(!trama)return(false);
	char *iph,*rmb,*mac;
	int j;

	rmb=toma_parametro("rmb",parametros); 
	mac=toma_parametro("mac",parametros);
	iph=toma_parametro("iph",parametros); 

	for (j=0;j<MAXIMOS_SRVRMB;j++){
		if (strcmp(rmb,tbsocketsSRVRMB[j].ip)==0){ // Si existe la IP ...
			FINCADaINTRO(parametros,iph);
			return(manda_trama_servidorrembo(rmb,parametros,tbsocketsSRVRMB[j].puertorepo));
		}
	}
	return(false);
}
// ________________________________________________________________________________________________________
// Funcin: RESPUESTA_Arrancar
//
//		Descripcin:
//			Responde al comando Apagar 
//		Parnetros:
//			- s: Socket que el cliente rembo usa para comunicarse con el servidor HIDRA
//			- parametros: parametros del comando
// ________________________________________________________________________________________________________
int RESPUESTA_Arrancar(SOCKET s,char *parametros)
{
	char ErrStr[200];
	Database db;
	Table tbl;
	
	char *res,*der,*iph,*ido,*ids;

	res=toma_parametro("res",parametros); // Toma resultado
	der=toma_parametro("der",parametros); // Toma descripcin del error ( si hubiera habido)
	iph=toma_parametro("iph",parametros); // Toma ip
	ido=toma_parametro("ido",parametros); // Toma identificador del ordenador
	ids=toma_parametro("ids",parametros); // Toma identificador de la accin

	if(!db.Open(usuario,pasguor,datasource,catalog)){ // error de conexion
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(!RespuestaEstandar(res,der,ids,ido,db,tbl)){
		return(false); // Error al registrar notificacion
	}
	db.Close();
	return(true);
}
// ________________________________________________________________________________________________________
// Funcin: RESPUESTA_Apagar
//
//		Descripcin:
//			Responde al comando Apagar 
//		Parnetros:
//			- s: Socket que el cliente rembo usa para comunicarse con el servidor HIDRA
//			- parametros: parametros del comando
// ________________________________________________________________________________________________________
int RESPUESTA_Apagar(SOCKET s,char *parametros)
{
	char ErrStr[200];
	Database db;
	Table tbl;
	int i;
	char *res,*der,*iph,*ido,*ids;

	res=toma_parametro("res",parametros); // Toma resultado
	der=toma_parametro("der",parametros); // Toma descripcin del error ( si hubiera habido)
	iph=toma_parametro("iph",parametros); // Toma ip
	ido=toma_parametro("ido",parametros); // Toma identificador del ordenador
	ids=toma_parametro("ids",parametros); // Toma identificador de la accin

	if(!db.Open(usuario,pasguor,datasource,catalog)){ // error de conexion
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(!RespuestaEstandar(res,der,ids,ido,db,tbl)){
		return(false); // Error al registrar notificacion
	}

	if(strcmp(res,ACCION_FALLIDA)==0) return(TRUE); // Error en la ejecucin de la accin en el cliente rembo

	if (cliente_existente(iph,&i)) // Si ya existe la IP ...
		borra_entrada(i);
	db.Close();
	return(true);
}
// ________________________________________________________________________________________________________
// Funcin:RESPUESTA_RemboOffline
//
//		Descripcin:
//			Responde al comando Apagar 
//		Parnetros:
//			- s: Socket que el cliente rembo usa para comunicarse con el servidor HIDRA
//			- parametros: parametros del comando
// ________________________________________________________________________________________________________
int RESPUESTA_RemboOffline(SOCKET s,char *parametros)
{
	char ErrStr[200];
	Database db;
	Table tbl;
	int i;

	char *res,*der,*iph,*ido,*ids;

	res=toma_parametro("res",parametros); // Toma resultado
	der=toma_parametro("der",parametros); // Toma descripcin del error ( si hubiera habido)
	iph=toma_parametro("iph",parametros); // Toma ip
	ido=toma_parametro("ido",parametros); // Toma identificador del ordenador
	ids=toma_parametro("ids",parametros); // Toma identificador de la accin

	if(!db.Open(usuario,pasguor,datasource,catalog)){ // error de conexion
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(!RespuestaEstandar(res,der,ids,ido,db,tbl)){
		return(false); // Error al registrar notificacion
	}

	if(strcmp(res,ACCION_FALLIDA)==0) return(TRUE); // Error en la ejecucin de la accin en el cliente rembo

	if (cliente_existente(iph,&i)) // Si ya existe la IP ...
		borra_entrada(i);
	db.Close();
	return(true);
}
// ________________________________________________________________________________________________________
// Funcin: RESPUESTA_Reiniciar
//
//		Descripcin:
//			Responde al comando Reiniciar 
//		Parnetros:
//			- s: Socket que el cliente rembo usa para comunicarse con el servidor HIDRA
//			- parametros: parametros del comando
// ________________________________________________________________________________________________________
int RESPUESTA_Reiniciar(SOCKET s,char *parametros)
{
	int i;
	char ErrStr[200];
	Database db;
	Table tbl;
	
	char *res,*der,*iph,*ido,*ids;

	res=toma_parametro("res",parametros); // Toma resultado
	der=toma_parametro("der",parametros); // Toma descripcin del error ( si hubiera habido)
	iph=toma_parametro("iph",parametros); // Toma ip
	ido=toma_parametro("ido",parametros); // Toma identificador del ordenador
	ids=toma_parametro("ids",parametros); // Toma identificador de la accin

	if(!db.Open(usuario,pasguor,datasource,catalog)){ // error de conexion
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(!RespuestaEstandar(res,der,ids,ido,db,tbl)){
		return(false); // Error al registrar notificacion
	}
	if(strcmp(res,ACCION_FALLIDA)==0) return(TRUE); // Error en la ejecucin de la accin en el cliente rembo

	if (cliente_existente(iph,&i)) // Si ya existe la IP ...
		borra_entrada(i);
	db.Close();
	return(true);
}
// ________________________________________________________________________________________________________
//
// Funcin: borra_entrada
//
//		Descripcin:
//			 Borra la entrada de un ordenador en la tabla de socket
//		Parnetros:
//			- i: Indice dentro de la tabla
// ________________________________________________________________________________________________________
int borra_entrada(int i)
{
	tbsockets[i].ip[0]=(char)NULL; 
	tbsockets[i].estado[0]=(char)NULL;
	if(!tbsockets[i].sock)
			close(tbsockets[i].sock);
	tbsockets[i].sock=INVALID_SOCKET;
	tbsockets[i].ipsrvdhcp[0]=(char)NULL; 
	tbsockets[i].ipsrvrmb[0]=(char)NULL; 

	return(true);
}
// ________________________________________________________________________________________________________
// Funcin: RESPUESTA_EjecutarScript
//
//		Descripcin:
//			Responde al comando Ejecutar script
//		Parnetros:
//			- s: Socket que el cliente rembo usa para comunicarse con el servidor HIDRA
//			- parametros: parametros del comando
// ________________________________________________________________________________________________________
int RESPUESTA_EjecutarScript(SOCKET s,char *parametros)
{
	char ErrStr[200];
	Database db;
	Table tbl;
	
	char *res,*der,*ids,*iph,*ido,*cfg;

	res=toma_parametro("res",parametros); // Toma resultado
	der=toma_parametro("der",parametros); // Toma descripcin del error ( si hubiera habido)
	ids=toma_parametro("ids",parametros); // Toma idperfilsoft
	iph=toma_parametro("iph",parametros); // Toma ip
	ido=toma_parametro("ido",parametros); // Toma identificador del ordenador
	cfg=toma_parametro("cfg",parametros); // Toma configuracin
	
	if(!db.Open(usuario,pasguor,datasource,catalog)){ // error de conexion
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(!RespuestaEstandar(res,der,ids,ido,db,tbl)){
		return(false); // Error al registrar notificacion
	}

	if(strcmp(res,ACCION_FALLIDA)!=0) { // Ha habido algn error en la ejecucin de la accin del cliente rembo
		if(!actualiza_configuracion(db,tbl,cfg,0,0,iph)) // El ordenador ha cambiado de configuracin
			return(false);
	}
	db.Close();
	return(true);
}
// ________________________________________________________________________________________________________
// Funcin: RespuestaEstandar
//
//		Descripcin:
//			Esta funcin actualiza la base de datos con el resultado de la ejecucin de un comando con seguimiento
//		Parnetros:
//			- res: resultado de la ejecucin del comando
//			- der: Descripcin del error si hubiese habido
//			- ids: identificador de la accin notificada
//			- ido: Identificador del ordenador que notifica
//			- db: Objeto base de datos (operativo)
//			- tbl: Objeto tabla
// ________________________________________________________________________________________________________
int RespuestaEstandar(char *res,char *der,char *ids,char* ido,Database db,Table tbl)
{
	char ErrStr[200],sqlstr[1000];
	char parametros[LONGITUD_PARAMETROS];
	char fechareg[100];
	int i,resul;
	int idaccion,accionid,idnotificador;
	char *iph;
    struct tm* st;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ACCESO atnico A TRAVEZ DE OBJETO MUTEX a este trozo de cnigo 
	pthread_mutex_lock(&guardia); 

	sprintf(sqlstr,"Select * from acciones WHERE idaccion=%s",ids);
	if(!db.Execute(sqlstr,tbl)){ // Error al consultar
		db.GetErrorErrStr(ErrStr);
				pthread_mutex_unlock(&guardia); 
		return(false);
	}
	if(tbl.ISEOF()){ // No existe registro de acciones
				pthread_mutex_unlock(&guardia); 
		return(true);
	}
	if(!tbl.Get("parametros",parametros)){ // Toma parametros de la accin
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
					pthread_mutex_unlock(&guardia); 
			return(false);
	}
	char resultado[2]; // comprueba si ya ha fallado la accin
	if(!tbl.Get("resultado",resultado)){ // Toma resultado actual de la accin
			tbl.GetErrorErrStr(ErrStr); // error al recuperar el campo resultado
					pthread_mutex_unlock(&guardia); 
			return(false);
	}
	if(!tbl.Get("idaccion",idaccion)){ // Toma el identificador de la accin para tener el dato en formato "int"
			tbl.GetErrorErrStr(ErrStr); // error al recuperar el campo resultado
					pthread_mutex_unlock(&guardia); 
			return(false);
	}
	if(!tbl.Get("accionid",accionid)){ // Toma la accion padre
			tbl.GetErrorErrStr(ErrStr); // error al recuperar el campo resultado
					pthread_mutex_unlock(&guardia); 
			return(false);
	}
	if(!tbl.Get("idnotificador",idnotificador)){ // Toma el identificador del notificador
			tbl.GetErrorErrStr(ErrStr); // error al recuperar el campo resultado
					pthread_mutex_unlock(&guardia); 
			return(false);
	}
	
    st=TomaHora();
	sprintf(fechareg,"%d/%d/%d %d:%d:%d",st->tm_year+1900,st->tm_mon+1,st->tm_mday,st->tm_hour,st->tm_min,st->tm_sec);

	// Graba notificacin
	sprintf(sqlstr,"INSERT INTO notificaciones (accionid,idnotificador,fechahorareg,resultado,descrinotificacion) VALUES (%s,%s,'%s','%s','%s')",ids,ido,fechareg,res,der);
	if(!db.Execute(sqlstr)){ // Error al insertar
		db.GetErrorErrStr(ErrStr);
				pthread_mutex_unlock(&guardia); 
		return(false);
	}
 
	if(strcmp(res,ACCION_FALLIDA)==0 && strcmp(resultado,ACCION_SINERRORES)==0){ // Accion fallida en el cliente rembo
		sprintf(sqlstr,"Update acciones set resultado='%s' WHERE idaccion=%s",ACCION_CONERRORES,ids);
		strcpy(resultado,ACCION_CONERRORES);
		if(!db.Execute(sqlstr)){ // Error al actualizar
			db.GetErrorErrStr(ErrStr);
					pthread_mutex_unlock(&guardia); 
			return(false);
		}
	}
	// Comprueba si la accin se ejecutncorrectamente para el ambito sumando notificaciones
	INTROaFINCAD(parametros);
	iph=toma_parametro("iph",parametros); // Toma cadenaip
	int tbnumipes=0,totalipes=1,lon;

	lon=strlen(iph);
	for (i=0;i<lon;i++){
		if(iph[i]==';') 
			totalipes++; // ip detectada
	}
	
	sprintf(sqlstr,"SELECT COUNT(*) AS tbnumipes FROM notificaciones WHERE accionid=%s",ids);
	if(!db.Execute(sqlstr,tbl)){ // Error al insertar
				pthread_mutex_unlock(&guardia); 
		db.GetErrorErrStr(ErrStr);
				pthread_mutex_unlock(&guardia); 
		return(false);
	}
		
	if(!tbl.Get("tbnumipes",tbnumipes)){ // Recupera el numero de ordenadores que ya han notificado
			tbl.GetErrorErrStr(ErrStr); // error al recuperar el campo 
					pthread_mutex_unlock(&guardia); 
			return(false);
	}
	if(tbnumipes!=totalipes){
				pthread_mutex_unlock(&guardia); 
		return(true); // No es el ultimo ordenador en notificar
	}

    st=TomaHora();
	sprintf(fechareg,"%d/%d/%d %d:%d:%d",st->tm_year+1900,st->tm_mon+1,st->tm_mday,st->tm_hour,st->tm_min,st->tm_sec);

	// Actualizacion despues de que todos los ordenadores han notificado
	if(strcmp(resultado,ACCION_SINERRORES)==0){ // Accion finalizada con exito
		sprintf(sqlstr,"Update acciones set estado='%s',resultado='%s',fechahorafin='%s' WHERE idaccion=%s",ACCION_FINALIZADA,ACCION_EXITOSA,fechareg,ids);
		if(!db.Execute(sqlstr,tbl)){ // Error al actualizar
			db.GetErrorErrStr(ErrStr);
					pthread_mutex_unlock(&guardia); 
			return(false);
		}
	}
	if(strcmp(resultado,ACCION_CONERRORES)==0){ // Accion finalizada con errores
		sprintf(sqlstr,"Update acciones set estado='%s',resultado='%s',fechahorafin='%s' WHERE idaccion=%s",ACCION_FINALIZADA,ACCION_FALLIDA,fechareg,ids);
		if(!db.Execute(sqlstr,tbl)){ // Error al actualizar
			db.GetErrorErrStr(ErrStr);
					pthread_mutex_unlock(&guardia); 
			return(false);
		}
	}
	resul=true;
	if(accionid>0){ // Existe accion padre que hay que actualizar
		resul=InsertaNotificaciones(idaccion,idnotificador,accionid,resultado,db);
		if(resul)
			resul=comprueba_resultados(accionid,db);
	}
			pthread_mutex_unlock(&guardia); 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return(resul);
}
// ________________________________________________________________________________________________________
// Funcin: RESPUESTA_CrearPerfilSoftware
//
//		Descripcin:
//			Responde al comando Crear Perfil Software
//		Parnetros:
//			- s: Socket que el cliente rembo usa para comunicarse con el servidor HIDRA
//			- parametros: parametros del comando
// ________________________________________________________________________________________________________
int RESPUESTA_CrearPerfilSoftware(SOCKET s,char *parametros)
{
	char ErrStr[200],sqlstr[1000];
	char *res,*der,*ids,*ifh,*ifs,*iph,*ido;
	Database db;
	Table tbl;
	
	if(!db.Open(usuario,pasguor,datasource,catalog)){ // error de conexion
		db.GetErrorErrStr(ErrStr);
		return(false);
	}

	res=toma_parametro("res",parametros); // Toma resultado
	der=toma_parametro("der",parametros); // Toma descripcin del error ( si hubiera habido)
	ids=toma_parametro("ids",parametros); // Toma idperfilsoft
	iph=toma_parametro("iph",parametros); // Toma ip
	ido=toma_parametro("ido",parametros); // Toma dentificador del ordenador
	ifh=toma_parametro("ifh",parametros); // Toma idperfilhard
	ifs=toma_parametro("ifs",parametros); // Toma idperfilsoft

	if(!RespuestaEstandar(res,der,ids,ido,db,tbl)){
		return(false); // Error al registrar notificacion
	}

	if(strcmp(res,ACCION_FALLIDA)==0) { // Ha habido algn error en la ejecucin de la accin en el cliente rembo
		db.Close();
		return(false);
	}

	sprintf(sqlstr,"Select * from perfileshard_perfilessoft WHERE idperfilhard=%s AND idperfilsoft=%s",ifh,ifs);
	if(!db.Execute(sqlstr,tbl)){ // Error al consultar
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(!tbl.ISEOF()){ // Si ya existe el registro ... no hace falta insertarlo
		db.Close();
		return(false);	
	}
	sprintf(sqlstr,"INSERT INTO perfileshard_perfilessoft (idperfilhard,idperfilsoft) VALUES(%s,%s)",ifh,ifs);
	if(!db.Execute(sqlstr,tbl)){ // Error al insertar
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	db.Close();
	return(true);
}
// ________________________________________________________________________________________________________
// Funcin: RESPUESTA_CrearSoftwareIncremental
//
//		Descripcin:
//			Esta funcin responde a un comando de creacin de un software incremental. Ademn actualiza  la base de datos insertando
//			en su caso la nueva combinacin de perfil software con incremental.
//		Parnetros:
//			- s: Socket que el cliente rembo usa para comunicarse con el servidor HIDRA
//			- parametros: parametros del comando
// ________________________________________________________________________________________________________
int RESPUESTA_CrearSoftwareIncremental(SOCKET s,char *parametros)
{
	char ErrStr[200],sqlstr[1000];
	char *res,*der,*ids,*ifh,*ifs,*icr,*iph,*ido;
	int idphardidpsoft;
	Database db;
	Table tbl;
	
	if(!db.Open(usuario,pasguor,datasource,catalog)){ // error de conexion
		db.GetErrorErrStr(ErrStr);
		return(false);
	}

	res=toma_parametro("res",parametros); // Toma resultado
	der=toma_parametro("der",parametros); // Toma descripcin del error ( si hubiera habido)
	ids=toma_parametro("ids",parametros); // Toma idperfilsoft
	iph=toma_parametro("iph",parametros); // Toma ip
	ido=toma_parametro("ido",parametros); // Toma dentificador del ordenador
	ifh=toma_parametro("ifh",parametros); // Toma idperfilhard
	ifs=toma_parametro("ifs",parametros); // Toma idperfilsoft
	icr=toma_parametro("icr",parametros); // Toma idsoftincremental

	if(!RespuestaEstandar(res,der,ids,ido,db,tbl)){
		return(false); // Error al registrar notificacion
	}

	if(strcmp(res,ACCION_FALLIDA)==0) { // Ha habido algn error en la ejecucin de la accin en el cliente rembo
		db.Close();
		return(false);
	}

	sprintf(sqlstr,"Select idphardidpsoft from perfileshard_perfilessoft WHERE idperfilhard=%s AND idperfilsoft=%s",ifh,ifs);
	if(!db.Execute(sqlstr,tbl)){ // Error al consultar
		db.GetErrorErrStr(ErrStr);
		return(false);
	}

	if(tbl.ISEOF()){ // Si no existe el registro ... 
		db.Close();
		return(false);	
	}

	if(!tbl.Get("idphardidpsoft",idphardidpsoft)){ // Recupera el identificador de la combinacin de perfiles
		tbl.GetErrorErrStr(ErrStr); // error al recuperar el campo 
		return(false);
	}

	sprintf(sqlstr,"Select * from phard_psoft_softincremental WHERE idphardidpsoft=%d AND idsoftincremental=%s",idphardidpsoft,icr);
	if(!db.Execute(sqlstr,tbl)){ // Error al consultar
		db.GetErrorErrStr(ErrStr);
		return(false);
	}

	if(!tbl.ISEOF()){ // Si ya existe el registro ... 
		db.Close();
		return(false);	
	}

	sprintf(sqlstr,"INSERT INTO phard_psoft_softincremental (idphardidpsoft,idsoftincremental) VALUES(%d,%s)",idphardidpsoft,icr);
	if(!db.Execute(sqlstr,tbl)){ // Error al insertar
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	db.Close();	
	return(true);
}
// ________________________________________________________________________________________________________
// Funcin: RESPUESTA_RestaurarImagen
//
//		Descripcin:
//			Esta funcin responde a un comando de restauracin de una imagen. Ademn actualiza  la base de datos.
//		Parnetros:
//			- s: Socket que el cliente rembo usa para comunicarse con el servidor HIDRA
//			- parametros: parametros del comando
// ________________________________________________________________________________________________________
int RESPUESTA_RestaurarImagen(SOCKET s,char *parametros)
{
	char ErrStr[200],gido[20];
	char *res,*der,*ids,*iph,*ido,*idi,*par,*cfg;
	Database db;
	Table tbl;

	if(!db.Open(usuario,pasguor,datasource,catalog)){ // error de conexion
		db.GetErrorErrStr(ErrStr);
		return(false);
	}

	INTROaFINCAD(parametros);

	res=toma_parametro("res",parametros); // Toma resultado
	der=toma_parametro("der",parametros); // Toma descripcin del error ( si hubiera habido)
	ids=toma_parametro("ids",parametros); // Toma identificador de la accion
	iph=toma_parametro("iph",parametros); // Toma ip
	ido=toma_parametro("ido",parametros); // Toma identificador del ordenador
	cfg=toma_parametro("cfg",parametros); // Toma configuracin
	par=toma_parametro("par",parametros); // particion
	idi=toma_parametro("idi",parametros); // identificador de la imagen

	strcpy(gido,ido); // Guarda el identificador del ordenador

	if(!RespuestaEstandar(res,der,ids,ido,db,tbl)){
		return(false); // Error al registrar notificacion
	}
	if(strcmp(res,ACCION_FALLIDA)==0) { // Ha habido algn error en la ejecucin de la accin del cliente rembo
		db.Close();
		return(false);
	}

	if(!actualiza_configuracion(db,tbl,cfg,0,0,iph)) 	return(false); // Erro al actualiza la configuracin
	if(!Actualiza_ordenador_imagen(par,idi,gido,db)) return(false); 
	db.Close();
	return(true);	
}
// ________________________________________________________________________________________________________
// Funcin: Actualiza_ordenador_imagen
//
//		Descripcin:
//			Esta funcin actualiza la tabla ordenador_imagen
//		Parnetros:
//			- par: particion
//			- idi: identificador de la imagen ( 0 ninguna )
//			- ido: identificador del ordenador
//			- db: Conexin ADO operativa
// ________________________________________________________________________________________________________
int Actualiza_ordenador_imagen(char *par,char *idi,char *ido,Database db)
{
	char ErrStr[200],sqlstr[1000];
	Table tbl;
	int idimagen,idimagenres;

	idimagenres=atoi(idi);
	if(idimagenres==0){ // Se ha formateado la particin y se ha borrado la imagen por tanto
		sprintf(sqlstr,"DELETE FROM ordenador_imagen WHERE idordenador=%s AND particion=%s",ido,par);
		if(!db.Execute(sqlstr)){ // Error al insertar
			db.GetErrorErrStr(ErrStr);
			return(false);
		}
		return(true);
	}

	sprintf(sqlstr,"SELECT idimagen FROM ordenador_imagen INNER JOIN ordenadores ON ordenador_imagen.idordenador = ordenadores.idordenador WHERE ordenadores.idordenador = %s AND ordenador_imagen.particion = %s",ido,par);
	if(!db.Execute(sqlstr,tbl)){ // Error al consultar
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(!tbl.ISEOF()){ // Existe registro
		if(!tbl.Get("idimagen",idimagen)){
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}
		else{
			if (idimagenres!=idimagen){
				sprintf(sqlstr,"Update ordenador_imagen set idimagen=%s WHERE idordenador=%s AND particion=%s",idi,ido,par);
				if(!db.Execute(sqlstr)){ // Error al actualizar
					db.GetErrorErrStr(ErrStr);
					return(false);
				}
			}
		}
	}
	else{ // No existe el registro
			sprintf(sqlstr,"INSERT INTO ordenador_imagen (idordenador,particion,idimagen) VALUES(%s,%s,%s)",ido,par,idi);
			if(!db.Execute(sqlstr)){ // Error al insertar
				db.GetErrorErrStr(ErrStr);
				return(false);
			}
	}
	return(true);
}
// ________________________________________________________________________________________________________
// Funcin: RESPUESTA_ParticionaryFormatear
//
//		Descripcin:
//			Esta funcin responde a un comando de particionar y formatear.  Ademn actualiza  la base de datos.
//		Parnetros:
//			- s: Socket que el cliente rembo usa para comunicarse con el servidor HIDRA
//			- parametros: parametros del comando
// ________________________________________________________________________________________________________
int RESPUESTA_ParticionaryFormatear(SOCKET s,char *parametros)
{
	char sqlstr[1000],ErrStr[200],gido[20];
	Database db;
	Table tbl;
	char *res,*der,*ids,*iph,*ido,*cfg;

	res=toma_parametro("res",parametros); // Toma resultado
	der=toma_parametro("der",parametros); // Toma descripcin del error ( si hubiera habido)
	ids=toma_parametro("ids",parametros); // Toma identificador de la accin
	iph=toma_parametro("iph",parametros); // Toma ip
	ido=toma_parametro("ido",parametros); // Toma identificador del ordenador
	cfg=toma_parametro("cfg",parametros); // Toma configuracin
	
	strcpy(gido,ido); // Guarda el identificador del ordenador

	if(!db.Open(usuario,pasguor,datasource,catalog)){ // error de conexion
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(!RespuestaEstandar(res,der,ids,ido,db,tbl)){
		return(false); // Error al registrar notificacion
	}
	if(strcmp(res,ACCION_FALLIDA)==0){
		db.Close(); 
		 return(true); // Ha habido algn error en la ejecucin de la accin del cliente rembo
	}
	if(!actualiza_configuracion(db,tbl,cfg,0,0,iph)) 	return(false); // Erro al actualiza la configuracin

	// Elimina informacin sobre imagenes en este ordenador, al haber sido formateado
	sprintf(sqlstr,"DELETE FROM ordenador_imagen WHERE idordenador=%s",gido);
	if(!db.Execute(sqlstr)){ // Error al insertar
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	db.Close(); 
	return(true);
}
// ________________________________________________________________________________________________________
// Funcin: RESPUESTA_Configurar
//
//		Descripcin:
//			Esta funcin responde a un comando de Configurar.  Ademn actualiza  la base de datos.
//		Parnetros:
//			- s: Socket que el cliente rembo usa para comunicarse con el servidor HIDRA
//			- parametros: parametros del comando
// ________________________________________________________________________________________________________
int RESPUESTA_Configurar(SOCKET s,char *parametros)
{
	char ErrStr[200],gids[20],gido[20];
	Database db;
	Table tbl;
	int lon,resul,i;
	char *res,*der,*ids,*iph,*ido,*cfg,*hdc;

	res=toma_parametro("res",parametros); // Toma resultado
	der=toma_parametro("der",parametros); // Toma descripcin del error ( si hubiera habido)
	ids=toma_parametro("ids",parametros); // Toma idperfilsoft
	iph=toma_parametro("iph",parametros); // Toma ip
	ido=toma_parametro("ido",parametros); // Toma identificador del ordenador
	cfg=toma_parametro("cfg",parametros); // Toma configuracin
	hdc=toma_parametro("hdc",parametros); // Toma participaciones a formatear

	strcpy(gids,ids); // Guarda el identificador de la accin
	strcpy(gido,ido); // Guarda el identificador del ordenador

	if(!db.Open(usuario,pasguor,datasource,catalog)){ // error de conexion
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(!RespuestaEstandar(res,der,ids,ido,db,tbl)){
		return(false); // Error al registrar notificacion
	}

	if(strcmp(res,ACCION_FALLIDA)==0){
		db.Close();
		 return(true); // Ha habido algn error en la ejecucin de la accin del cliente rembo
	}
	if(!actualiza_configuracion(db,tbl,cfg,0,0,iph)) 	return(false); // Erro al actualiza la configuracin
	
	lon=strlen(hdc);
	for(i=0;i<lon;i++){
		if(hdc[i]==';') hdc[i]='\0';
	}
	for(i=0;i<lon;i++){
		if(*hdc!='\0'){
			resul=Actualiza_ordenador_imagen(hdc,"0",gido,db);
			if(!resul){
				db.Close();
				 return(false);
			}
		}
		hdc++;
	}
	db.Close();
	return(true);
}
// ________________________________________________________________________________________________________
// Funcin: RESPUESTA_TomaConfiguracion
//
//		Descripcin:
//			Esta funcin responde a un comando de Toma Comfiguracin.  Ademn actualiza  la base de datos.
//		Parnetros:
//			- s: Socket que el cliente rembo usa para comunicarse con el servidor HIDRA
//			- parametros: parametros del comando
// ________________________________________________________________________________________________________
int RESPUESTA_TomaConfiguracion(SOCKET s,char *parametros)
{
	char ErrStr[200];
	Database db;
	Table tbl;
	
	char *res,*der,*ids,*iph,*ido,*cfg;

	res=toma_parametro("res",parametros); // Toma resultado
	der=toma_parametro("der",parametros); // Toma descripcin del error ( si hubiera habido)
	ids=toma_parametro("ids",parametros); // Toma identificador de la accin
	iph=toma_parametro("iph",parametros); // Toma ip
	ido=toma_parametro("ido",parametros); // Toma identificador del ordenador
	cfg=toma_parametro("cfg",parametros); // Toma configuracin
	
	if(!db.Open(usuario,pasguor,datasource,catalog)){ // error de conexion
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(!RespuestaEstandar(res,der,ids,ido,db,tbl)){
		return(false); // Error al registrar notificacion
	}
	if(strcmp(res,ACCION_FALLIDA)!=0) { // Ha habido algn error en la ejecucin de la accin del cliente rembo
		if(!actualiza_configuracion(db,tbl,cfg,0,0,iph)) // El ordenador ha cambiado de configuracin
			return(false);
	}
	db.Close();
	return(true);
}
// ________________________________________________________________________________________________________
// Funcin: RESPUESTA_TomaHardware
//
//		Descripcin:
//			Esta funcin responde a un comando de Toma HArdware.  Ademn actualiza  la base de datos.
//		Parnetros:
//			- s: Socket que el cliente rembo usa para comunicarse con el servidor HIDRA
//			- parametros: parametros del comando
// ________________________________________________________________________________________________________
int RESPUESTA_TomaHardware(SOCKET s,char *parametros)
{
	char ErrStr[200];
	Database db;
	Table tbl;
	
	char *res,*der,*ids,*iph,*ido,*hrd;

	res=toma_parametro("res",parametros); // Toma resultado
	der=toma_parametro("der",parametros); // Toma descripcin del error ( si hubiera habido)
	ids=toma_parametro("ids",parametros); // Toma identificador de la accin
	iph=toma_parametro("iph",parametros); // Toma ip
	ido=toma_parametro("ido",parametros); // Toma identificador del ordenador
	
	hrd=toma_parametro("hrd",parametros); // Toma configuracin
	
	if(!db.Open(usuario,pasguor,datasource,catalog)){ // error de conexion
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(!RespuestaEstandar(res,der,ids,ido,db,tbl)){
		return(false); // Error al registrar notificacion
	}
	if(strcmp(res,ACCION_FALLIDA)!=0) { // Ha habido algn error en la ejecucin de la accin del cliente rembo
		if(!actualiza_hardware(db,tbl,hrd,iph,ido)) // El ordenador ha cambiado de configuracin
			return(false);
	}
	db.Close();
	return(true);
}
// ________________________________________________________________________________________________________
// Funcin: busca_comandos
//
//		Descripcin:
//			Esta funcin busca en la base de datos,comandos pendientes de ejecutar	para el ordenador cocreto
//		Parnetros:
//			- iph: Direccin IP del ordenador
//			- ido: Identificador del ordenador
//			- parametros: parametros de la accin buscada
//			- ids: Identificador de la accin
// ________________________________________________________________________________________________________
int busca_comandos(char* iph,char *ido,char *parametros,int *ids)
{
	char sqlstr[1000],ErrStr[200];
	Database db;
	Table tbl,tbln;

	if(!db.Open(usuario,pasguor,datasource,catalog)){ // error de conexion
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	sprintf(sqlstr,"SELECT idaccion,resultado,estado,parametros FROM acciones WHERE tipoaccion=%d AND estado = '%s' AND (resultado='%s' OR resultado='%s') AND parametros LIKE '%c%s%c' ORDER BY idaccion",EJECUCION_COMANDO,ACCION_INICIADA,ACCION_SINERRORES,ACCION_CONERRORES,37,iph,37);
	if(!db.Execute(sqlstr,tbl)){ // Error al leer
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(tbl.ISEOF()){
		db.Close();
		return(false);  // No hay comandos pendientes
	}

	while(!tbl.ISEOF()){ // Busca entre todas las acciones de diversos ambitos

		if(!tbl.Get("parametros",parametros)){ // Toma parametros
				tbl.GetErrorErrStr(ErrStr); // error al recuperar el campo parametros
				return(false);
		}

		if(IgualIP(parametros,iph)){ // Si existe la IP en la cadena
			if(!tbl.Get("idaccion",*ids)){ // Toma identificador de la accin
				tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
				return(false);
			}

			// Comprueba que aunque el resultado es ACCION_INICIADA, este ordenador an no ha notificado
			sprintf(sqlstr,"SELECT idnotificador FROM notificaciones WHERE accionid=%d AND idnotificador=%s",*ids,ido);
			if(!db.Execute(sqlstr,tbln)){ // Error al leer
				db.GetErrorErrStr(ErrStr);
				return(false);
			}
			if(tbln.ISEOF()){
				db.Close();				
				return(true);  // No ha notificado este ordenador
			}
		}
		tbl.MoveNext();
	}
	db.Close();
	return(false);  // No hay mn acciones 
}
// ________________________________________________________________________________________________________
int InsertaNotificaciones(int idaccion,int idnotificador, int accionid, char *resultado,Database db){
	
    struct tm* st;
	char ErrStr[200],sqlstr[1000];
	char fechahorareg[100];
	char descrinotificacion[100];


    st=TomaHora();
	sprintf(fechahorareg,"%d/%d/%d %d:%d:%d",st->tm_year+1900,st->tm_mon+1,st->tm_mday,st->tm_hour,st->tm_min,st->tm_sec);

	strcpy(descrinotificacion," ");

	if(strcmp(resultado,ACCION_CONERRORES)==0) {
		strcpy(descrinotificacion,"Ha ocurrido algn error en la ejecucin de esta tarea.");
		strcpy(resultado,ACCION_FALLIDA);
	}
	if(strcmp(resultado,ACCION_SINERRORES)==0) 
		strcpy(resultado,ACCION_EXITOSA);
	
	sprintf(sqlstr,"INSERT INTO notificaciones (accionid,idnotificador,fechahorareg,resultado,descrinotificacion,idaccion) VALUES (%d,%d,'%s','%s','%s',%d)",accionid,idnotificador,fechahorareg,resultado,descrinotificacion,idaccion);
	if(!db.Execute(sqlstr)){ // Error al insertar
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	return(true);
}
// ________________________________________________________________________________________________________
int comprueba_resultados(int idaccion,Database db){

	char ErrStr[200],sqlstr[1000];
	int numfallidas;
	char finalaccion[2];
	Table tbl;

	sprintf(sqlstr,"SELECT COUNT(*) as numfallidas FROM notificaciones WHERE resultado='%s' AND accionid=%d",ACCION_FALLIDA,idaccion);
	if(!db.Execute(sqlstr,tbl)){ // Error al consultar
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(tbl.ISEOF()) return(false); // No existe registro de acciones
		
	if(!tbl.Get("numfallidas",numfallidas)){ // Toma dato
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
	}

	if(numfallidas>0)
		strcpy(finalaccion,ACCION_CONERRORES);
	else
		strcpy(finalaccion,ACCION_SINERRORES);

	sprintf(sqlstr,"UPDATE acciones SET resultado='%s' WHERE idaccion=%d",finalaccion,idaccion);
	if(!db.Execute(sqlstr,tbl)){ // Error al actualizar
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	// Comprueba si ha finalizado esta accin e inserta su notificador correspondiente
	return(comprueba_finalizada(idaccion,finalaccion,db));
}
// ________________________________________________________________________________________________________
int comprueba_finalizada(int idaccion,char *resultado,Database db){

	char ErrStr[200],sqlstr[1000];
	int numnotificaciones,tipoaccion,idnotificador;
	char parametros[LONGITUD_PARAMETROS],*cadenanot;
	char fechareg[100];
	int accionid,cont,i,resul,lon;
	Table tbl;
    struct tm* st;

	sprintf(sqlstr,"SELECT COUNT(*) as numnotificaciones FROM notificaciones WHERE accionid=%d",idaccion);
	if(!db.Execute(sqlstr,tbl)){ // Error al consultar
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(tbl.ISEOF()) return(false); // No existe registro de acciones

	if(!tbl.Get("numnotificaciones",numnotificaciones)){ // Toma dato
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
	}

	sprintf(sqlstr,"SELECT tipoaccion,parametros,idnotificador,accionid FROM acciones WHERE idaccion=%d",idaccion);
	if(!db.Execute(sqlstr,tbl)){ // Error al consultar
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(tbl.ISEOF()) return(true); // No existe registro de acciones
		
	if(!tbl.Get("tipoaccion",tipoaccion)){ // Toma dato
			tbl.GetErrorErrStr(ErrStr); // error al recuperar el campo resultado
			return(false);
	}
	if(!tbl.Get("parametros",parametros)){ // Toma dato
			tbl.GetErrorErrStr(ErrStr); // error al recuperar el campo resultado
			return(false);
	}
	if(!tbl.Get("idnotificador",idnotificador)){ // Toma dato
			tbl.GetErrorErrStr(ErrStr); // error al recuperar el campo resultado
			return(false);
	}
	if(!tbl.Get("accionid",accionid)){ // Toma dato
				tbl.GetErrorErrStr(ErrStr); // error al recuperar el campo resultado
				return(false);
	}

	INTROaFINCAD(parametros);
	switch(tipoaccion){
		case EJECUCION_COMANDO :
			cadenanot=toma_parametro("iph",parametros); // Toma cadenaip
			break;
		case EJECUCION_TAREA :
			cadenanot=toma_parametro("cmd",parametros); // Toma comandos
 			break;
		case EJECUCION_TRABAJO :
			cadenanot=toma_parametro("tsk",parametros); // Toma tareas
			break;
		default:
			return(false);
	}
	cont=1;
	lon=strlen(cadenanot);
	for (i=0;i<lon;i++){
		if(cadenanot[i]==';') cont++;
	}
	resul=true;
	if(numnotificaciones==cont){
		st=TomaHora();
		sprintf(fechareg,"%d/%d/%d %d:%d:%d",st->tm_year+1900,st->tm_mon+1,st->tm_mday,st->tm_hour,st->tm_min,st->tm_sec);

		if(strcmp(resultado,ACCION_CONERRORES)==0) 
			sprintf(sqlstr,"UPDATE acciones SET resultado='%s',estado='%s',fechahorafin='%s' WHERE idaccion=%d",ACCION_FALLIDA,ACCION_FINALIZADA,fechareg,idaccion);
		else
			sprintf(sqlstr,"UPDATE acciones SET resultado='%s',estado='%s',fechahorafin='%s' WHERE idaccion=%d",ACCION_EXITOSA,ACCION_FINALIZADA,fechareg,idaccion);

		if(!db.Execute(sqlstr)){ // Error al actualizar
			db.GetErrorErrStr(ErrStr);
			return(false);
		}

		if(accionid>0){ // Esto no se ejecutarnsi la tarea tiene un trabajo padre
			resul=InsertaNotificaciones(idaccion,idnotificador,accionid,resultado,db);
			if(resul)
				return(comprueba_resultados(accionid,db));
		}
	}
	return(resul);
}
// ________________________________________________________________________________________________________
// Funcin: EnviaServidoresRembo
//
//		Descripcin:
//			Esta funcin envia una  trama a un servidor rembo para que sus clientes ejecuten un comando
//		Parnetros:
//			- parametros: parametros del comando
// ________________________________________________________________________________________________________
void EnviaServidoresRembo(char * parametros)
{
	int i,lon;
	for (i=0;i<MAXIMOS_SRVRMB;i++){
		if (tbsocketsSRVRMB[i].swenv==1){ // El switch de envio estna uno hay que enviar al servidor trama ...
			strcat(parametros,"iph=");
			strcat(parametros,tbsocketsSRVRMB[i].ipes);
			lon=strlen(parametros);
			parametros[lon-1]='\r'; // Quita la coma final
			manda_trama_servidorrembo(tbsocketsSRVRMB[i].ip,parametros,tbsocketsSRVRMB[i].puertorepo);
		}
	}
}
// ________________________________________________________________________________________________________
// Funcin: manda_comando_servidorrembo
//
//		Descripcin:
//			Esta funcin envia una  trama a un servidor rembo para que sus clientes ejecuten un comando
//		Parnetros:
//			- ip_srvrbm: Direccin IP del servidor REMBO
//			- parametros: parametros del comando
// ________________________________________________________________________________________________________
int manda_trama_servidorrembo(char* ip_srvrbm,char *parametros,int puertorepo)
{	int ret;
	TRAMA *trama=(TRAMA*)malloc(LONGITUD_TRAMA);
	if(!trama)
		return(false);
	strcpy(trama->parametros,parametros);
	SOCKET udpsock;
	udpsock=UDPConnect(IPlocal);
	if (udpsock == INVALID_SOCKET) return(false);
	ret=envia_comandos(udpsock,trama,ip_srvrbm,puertorepo);
	close(udpsock);
	return(ret);
}
//_______________________________________________________________________________________________________________
//
// Crea un socket en un puerto determinado para la conversacin UDP con el repositorio
// 
//_______________________________________________________________________________________________________________
SOCKET UDPConnect(char *ips)
{
	SOCKET socket_c; // Socket para hebras (UDP)
    struct sockaddr_in cliente;
	int puerto;

	socket_c = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // Crea socket para UDP

	if (socket_c == SOCKET_ERROR)
		return (INVALID_SOCKET);

	cliente.sin_addr.s_addr = inet_addr(ips); // selecciona interface
	cliente.sin_family = AF_INET;
	puerto=PUERTOMINUSER;
	while(puerto<PUERTOMAXUSER){ // Busca puerto libre
		cliente.sin_port = htons(puerto); // Puerto asignado
		if (bind(socket_c,(struct sockaddr *)&cliente,sizeof(cliente)) == SOCKET_ERROR)
			puerto++;
		else
			break;
	}
	if(puerto>=PUERTOMAXUSER){ // No hay puertos libres
		return(INVALID_SOCKET);
	}
	return(socket_c);
}
//________________________________________________________________________________________________________
// Funcin: envia_comandos
//
//		Descripcin: 
//			Enva trama UDP
// ________________________________________________________________________________________________________
int envia_comandos(SOCKET s,TRAMA* trama, char* ipsrv,int puerto)
{
	int ret,lon;
	struct sockaddr_in addrRepo;
	 
	trama->arroba='@';										// cabecera de la trama
	strcpy(trama->identificador,"JMMLCAMDJ");	// identificador de la trama
	trama->ejecutor='1';										// ejecutor de la trama 1=el servidor hidra  2=el cliente hidra
	
	addrRepo.sin_family = AF_INET;
    addrRepo.sin_port = htons((short)puerto);
    addrRepo.sin_addr.s_addr = inet_addr(ipsrv); //  Direccin IP repositorio
    Encriptar((char*)trama);
	ret = sendto(s,(char *)trama,lon+11,0,(struct sockaddr *)&addrRepo, sizeof(addrRepo));
    if (ret == SOCKET_ERROR){
    	RegistraLog("***send() fallo en envío al repositorio",true);
		return(FALSE);
    }
	return true;
}
// ________________________________________________________________________________________________________
// Funcin: DesmarcaServidoresRembo
//
//	 Descripcin:
//		Esta funcin desmarca la tabla completa de servidores rembo para iniciar la cuesation de envio
// ________________________________________________________________________________________________________
void DesmarcaServidoresRembo(void)
{
	int i;
	for (i=0;i<MAXIMOS_SRVRMB;i++){
		tbsocketsSRVRMB[i].swenv=0;
		tbsocketsSRVRMB[i].ipes[0]=(char)NULL;
	}
}
// ________________________________________________________________________________________________________
// Funcin: MarcaServidoresRembo
//
//		Descripcin:
//			Esta funcin marca la tabla de servidores Rembo y coloca la ip del cliente en el buffer
//		Parnetros:
//			- ipsrvrmb: ip del servidor rembo
//			- ipclrmb: ip del cliente rembo
// ________________________________________________________________________________________________________
void MarcaServidoresRembo(char* ipsrvrmb,char*ipclrmb)
{
	int i,resul;
	for (i=0;i<MAXIMOS_SRVRMB;i++){
		resul=strcmp(tbsocketsSRVRMB[i].ip,ipsrvrmb);
		if(resul==0) {// servidor rembo encontrado
			strcat(tbsocketsSRVRMB[i].ipes,ipclrmb);
			strcat(tbsocketsSRVRMB[i].ipes,";");
			tbsocketsSRVRMB[i].swenv=1;
			return;
		}
	}
}
// ________________________________________________________________________________________________________
// Funcin: TomaIPServidorRembo
//
//		Descripcin:
//			Esta funcin devuelve true o false dependiendo si el Servidor REMBO estnen la tabla  de servidores.
//		Parnetros:
//			- ip : La ip del servidor a buscar
// ________________________________________________________________________________________________________
BOOLEAN TomaIPServidorRembo(char *ip,int *p)
{
	int i,j;
	for (i=0;i<MAXIMOS_SOCKETS;i++){
		if (strcmp(ip,tbsockets[i].ip)==0){ // Si existe la IP ...
			strcpy(ip,tbsockets[i].ipsrvrmb);
			for (j=0;j<MAXIMOS_SRVRMB;j++){
				if (strcmp(ip,tbsocketsSRVRMB[j].ip)==0){ // Si existe la IP ...
					*p=tbsocketsSRVRMB[j].puertorepo;
					return(TRUE);
				}
			}
		}
	}			
	return(FALSE);
}
// ________________________________________________________________________________________________________
// Funcin: AbreConexion
//
//		Descripcin: 
//			Crea un socket y lo conecta a una interface de red. Devuelve el socket
//		Parnetros:
//			- ips : La direccin IP con la que se comunicarnel socket
//			- port : Puerto para la  comunicacin
// ________________________________________________________________________________________________________
SOCKET AbreConexion(char *ips,int port)
{
    struct sockaddr_in server;
	SOCKET s;

	// Crea el socket y se intenta conectar
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == SOCKET_ERROR){
		RegistraLog("Error en la creacin del socket. Modulo: AbreConexion()",true);
		return INVALID_SOCKET;
	}

	server.sin_family = AF_INET;
	server.sin_port = htons((short)port);
	server.sin_addr.s_addr = inet_addr(ips);

	if (connect(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR){
		RegistraLog("connect() fallo",true);
		return INVALID_SOCKET;
	}
	return(s);
	
}
// ________________________________________________________________________________________________________
// Funcin: EjecutarTarea
//
//		Descripcin: 
//			Registra una accin (Tarea) y la envn para su ejecucin 
//		Parnetros:
//			- idtarea : Identificador de la tarea
//			- accionid: identificador del trabajo padre (si existe)
//			- idnotificador:  identificador del trabajo_tarea incluido en trabajo padre (si existe)
//			- idcentro: Centro propietario del trabjo padre (si existe este trabajo)
//			- Database: una conexion ADO operativa
//			- parametros: parnetros de la accin
// ________________________________________________________________________________________________________
int EjecutarTarea(int idtarea,int accionid,int idnotificador,int idcentro,Database db,char* parametros )
{
	char sqlstr[1000],ErrStr[200],ambito;
	Table tbl;
	int cont_comandos=0,lon;
	int  idcomando,idambito,idtareacomando,accionidcmd;
	char wambitarea[20],ambitarea[4000];
	char wparamtarea[20],paramtarea[1000],pids[20];
	int  tblon[100],tbComandosidcomando[100],tbComandosambito[100],tbComandosidnotificador[100],tbComandosidambito[100];
	char *tbComandosparametros[100];

	ambitarea[0]=(char)NULL; // Inicializacin
	strcpy(paramtarea,"cmd="); // Inicializacin
	if(idcentro==0){
		// recupera el identificador del Centro propietario de la tarea
		sprintf(sqlstr,"SELECT idcentro FROM tareas WHERE idtarea=%d",idtarea);
		if(!db.Execute(sqlstr,tbl)){ // Error al leer
			db.GetErrorErrStr(ErrStr);
			return(false);
		}
		if(tbl.ISEOF()) return(true);
		if(!tbl.Get("idcentro",idcentro)){ // Toma dato
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}
	}
	// Recupera los comandos que forman parte de la tarea
	sprintf(sqlstr,"SELECT * FROM tareas_comandos WHERE idtarea=%d ORDER by orden",idtarea);
	if(!db.Execute(sqlstr,tbl)){ // Error al leer
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(tbl.ISEOF()) return(true);
		
	// Recorre tareas-comandos
	while(!tbl.ISEOF()){ 
		if(!tbl.Get("idcomando",idcomando)){ // Toma dato
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}
		tbComandosidcomando[cont_comandos]=idcomando;

		if(!tbl.Get("ambito",ambito)){ // Toma dato
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}
		tbComandosambito[cont_comandos]=ambito;

		if(!tbl.Get("idambito",idambito)){ // Toma dato
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}
		tbComandosidambito[cont_comandos]=idambito;


		if(!tbl.Get("parametros",parametros)){ // Toma dato
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}

		lon=strlen(parametros);
		tblon[cont_comandos]=lon;
		tbComandosparametros[cont_comandos]=(char*)malloc(lon+20);
		if(tbComandosparametros[cont_comandos]==NULL)
			return(false); // No hay memoria bastante

		strcpy(tbComandosparametros[cont_comandos],parametros);
		
		if(!tbl.Get("idtareacomando",idtareacomando)){ // Toma dato
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}
		tbComandosidnotificador[cont_comandos]=idtareacomando;

		sprintf(wambitarea,"%d:%d;",ambito,idambito);
		strcat(ambitarea,wambitarea);

		sprintf(wparamtarea,"%d;",idtareacomando);
		strcat(paramtarea,wparamtarea);

		cont_comandos++;
		tbl.MoveNext();
	}
	lon=strlen(ambitarea);
	ambitarea[lon-1]=(char)NULL; // Quita la coma final

	lon=strlen(paramtarea);
	paramtarea[lon-1]=(char)NULL; // Quita la coma final

	char _fechahorareg[100];
    struct tm* st;
    st=TomaHora();
	sprintf(_fechahorareg,"%d/%d/%d %d:%d:%d",st->tm_year+1900,st->tm_mon+1,st->tm_mday,st->tm_hour,st->tm_min,st->tm_sec);

	sprintf(sqlstr,"INSERT INTO acciones (tipoaccion,idtipoaccion,cateaccion,ambito,idambito,ambitskwrk,fechahorareg,estado,resultado,idcentro,parametros,accionid,idnotificador) VALUES (%d,%d,%d,0,0,'%s','%s','%s','%s',%d,'%s',%d,%d)",EJECUCION_TAREA,idtarea,PROCESOS,ambitarea,_fechahorareg,ACCION_INICIADA,ACCION_SINERRORES,idcentro,paramtarea,accionid,idnotificador);
	if(!db.Execute(sqlstr)){ // Error al insertar
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	accionid=0;
	// Toma identificador dela accin
	sprintf(sqlstr,"SELECT LAST_INSERT_ID() as identificador");
	if(!db.Execute(sqlstr,tbl)){ // Error al leer
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(!tbl.ISEOF()){ // Si existe registro
		if(!tbl.Get("identificador",accionid)){
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}
	}
	int i;
	// Insertar acciones:comandos
	for (i=0;i<cont_comandos;i++){
	    st=TomaHora();
		sprintf(_fechahorareg,"%d/%d/%d %d:%d:%d",st->tm_year+1900,st->tm_mon+1,st->tm_mday,st->tm_hour,st->tm_min,st->tm_sec);
		sprintf(sqlstr,"INSERT INTO acciones (tipoaccion,idtipoaccion,cateaccion,ambito,idambito,fechahorareg,estado,resultado,idcentro,parametros,accionid,idnotificador) VALUES (%d,%d,%d,%d,%d,'%s','%s','%s',%d,'%s',%d,%d)",EJECUCION_COMANDO,tbComandosidcomando[i],PROCESOS,tbComandosambito[i],tbComandosidambito[i],_fechahorareg,ACCION_EXITOSA,ACCION_SINERRORES,idcentro,tbComandosparametros[i],accionid,tbComandosidnotificador[i]);	
		if(!db.Execute(sqlstr)){ // Error al insertar
			db.GetErrorErrStr(ErrStr);
			 free(tbComandosparametros[i]);
			return(false);
		}
		// Toma identificador dela accin
		sprintf(sqlstr,"SELECT LAST_INSERT_ID() as identificador");
		if(!db.Execute(sqlstr,tbl)){ // Error al leer
			db.GetErrorErrStr(ErrStr);
			return(false);
		}
		if(!tbl.ISEOF()){ // Si existe registro
			if(!tbl.Get("identificador",accionidcmd)){
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
			}
		}
		sprintf(pids,"ids=%d\r",accionidcmd);
		strcat((char*)tbComandosparametros[i],pids); // Le ande el identificador de la accion
		envia_tarea(tbComandosparametros[i]);
		free(tbComandosparametros[i]);
	}
	return(true);
}
// ________________________________________________________________________________________________________
// Funcin: manda peticin de inclusion
//
//		Descripcin:
//			Esta funcin envia una tarea  por la red.
//		Parnetros:
//			- parametros: El contenido de la tarea
// ________________________________________________________________________________________________________
void envia_tarea(char* parametros)
{
	TRAMA trama;	

	trama.arroba='@';
	strncpy(trama.identificador,"JMMLCAMDJ",9);
	trama.ejecutor=parametros[0];
	strcpy(trama.parametros,(char*)&parametros[1]);
	gestiona_comando(INVALID_SOCKET,trama);
}
// ________________________________________________________________________________________________________
// Funcin: EjecutarTrabajo
//
//		Descripcin: 
//			Registra una accin (Trabajo y la envn para su ejecucin 
//		Parnetros:
//			- idtrabajo : Identificador del trabajo
//			- Database: una conexion ADO operativa
//			- parametros: parnetros de la accin
// ________________________________________________________________________________________________________
int EjecutarTrabajo(int idtrabajo,Database db,char*parametros )
{
	char sqlstr[1000],ErrStr[200];
	Table tbl;
	int cont_tareas=0,lon;
	int  idtarea,idtrabajotarea,idcentro;
	char wambitrabajo[500],ambitrabajo[4000];
	char wparamtrabajo[20],paramtrabajo[1000];
	int  tbTareasidtarea[100],tbTareasidnotificador[100];
	char ambitskwrk[500];

	ambitrabajo[0]=(char)NULL; // Inicializacin
	strcpy(paramtrabajo,"tsk="); // Inicializacin

	// recupera el identificador del Centro propietario de la tarea
	sprintf(sqlstr,"SELECT idcentro FROM trabajos WHERE idtrabajo=%d",idtrabajo);
	if(!db.Execute(sqlstr,tbl)){ // Error al leer
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(tbl.ISEOF()) return(true);
	if(!tbl.Get("idcentro",idcentro)){ // Toma dato
		tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
		return(false);
	}
	// Recupera las tareas que forman parte del trabajo
	sprintf(sqlstr,"SELECT * FROM trabajos_tareas WHERE idtrabajo=%d ORDER by orden",idtrabajo);
	if(!db.Execute(sqlstr,tbl)){ // Error al leer
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(tbl.ISEOF()) return(true);
	// Recorre trabajos-tareas
	while(!tbl.ISEOF()){ 	
		if(!tbl.Get("idtrabajotarea",idtrabajotarea)){ // Toma dato
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}
		tbTareasidnotificador[cont_tareas]=idtrabajotarea;

		if(!tbl.Get("idtarea",idtarea)){ // Toma dato
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}
		tbTareasidtarea[cont_tareas]=idtarea;

		if(!tbl.Get("parametros",parametros)){ // Toma dato
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}
		
		if(!tbl.Get("ambitskwrk",ambitskwrk)){ // Toma dato
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}
		sprintf(wambitrabajo,"%s;",ambitskwrk);
		strcat(ambitrabajo,wambitrabajo);

		sprintf(wparamtrabajo,"%d;",idtrabajotarea);
		strcat(paramtrabajo,wparamtrabajo);

		cont_tareas++;
		tbl.MoveNext();
	}
	lon=strlen(ambitrabajo);
	ambitrabajo[lon-1]=(char)NULL; // Quita la coma final

	lon=strlen(paramtrabajo);
	paramtrabajo[lon-1]=(char)NULL; // Quita la coma final

	char _fechahorareg[100];
    struct tm* st;
    st=TomaHora();
	sprintf(_fechahorareg,"%d/%d/%d %d:%d:%d",st->tm_year+1900,st->tm_mon+1,st->tm_mday,st->tm_hour,st->tm_min,st->tm_sec);

	sprintf(sqlstr,"INSERT INTO acciones (tipoaccion,idtipoaccion,cateaccion,ambito,idambito,ambitskwrk,fechahorareg,estado,resultado,idcentro,parametros,accionid,idnotificador) VALUES (%d,%d,%d,0,0,'%s','%s','%s','%s',%d,'%s',0,0)",EJECUCION_TRABAJO,idtrabajo,PROCESOS,ambitrabajo,_fechahorareg,ACCION_INICIADA,ACCION_SINERRORES,idcentro,paramtrabajo);
	if(!db.Execute(sqlstr)){ // Error al insertar
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	int accionid=0;
	// Toma identificador dela accin
	sprintf(sqlstr,"SELECT LAST_INSERT_ID() as identificador");
	if(!db.Execute(sqlstr,tbl)){ // Error al leer
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(!tbl.ISEOF()){ // Si existe registro
		if(!tbl.Get("identificador",accionid)){
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}
	}
	int i;
	// Insertar acciones:tareas
	for (i=0;i<cont_tareas;i++){
		if(!EjecutarTarea(tbTareasidtarea[i],accionid,tbTareasidnotificador[i],idcentro,db,parametros)){
			return(false);
		}
	}
	return(true);
}
// ________________________________________________________________________________________________________
// Funcin: cuestion_nuevoordenador
//
//	 Descripcin:
//		Esta funcin da de alta un ordenador  y un aula si el sistema estnconfigurado para ello
//		Parnetros:
//			- db: Objeto base de datos (ya operativo)
//			- tbl: Objeto tabla
//			- ido: identificador del ordenador que se darnde alta automnicamente( se devuelve)
//			- nau: Nombre del grupo donde estnel ordenador( rembo.conf)
//			- nor: Nombre del ordenador dado por rembo(dhcpd)
//			- iph: IP del ordenador
//			- mac: MAC del ordenador
//			- cfg: configuracin
//			- ipd: ip del servidor dhcp
//			- ipr: ip del servidor rembo
// ________________________________________________________________________________________________________
int cuestion_nuevoordenador(Database db,Table tbl,int*ido,char *nau,char *nor,char *iph,char *mac,char*cfg,char*ipd,char*ipr)
{
	char sqlstr[1000],ErrStr[200];
	int ida,isd,isr;

	// Recupera los datos del aula
	sprintf(sqlstr,"SELECT idaula FROM aulas  WHERE nombreaula= '%s'",nau);

	if(!db.Execute(sqlstr,tbl)){ // Error al consultar
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(tbl.ISEOF()){ // Si NO existe el aula
		return(false); 
	}
	else{
		if(!tbl.Get("idaula",ida)){ // Toma dato
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}
	}
	if(!Toma_idservidorres(db,tbl,ipd,ipr,&isd,&isr)) return(false);
	if(!alta_ordenador(db,tbl,ido,nor,iph,mac,ida,isd,isr)) return(false); // Alta del ordenador
	if(!actualiza_configuracion(db,tbl,cfg,0,0,iph)){ // Actualiza la configuracin del ordenador
		return(false); 
	}
	return(true);
}
// ________________________________________________________________________________________________________
// Funcin: alta_ordenador
//
//		Descripcin:
//			Esta funcin da de alta un ordenador
//		Parnetros:
//			- db: Objeto base de datos (ya operativo)
//			- tbl: Objeto tabla
//			- mac: MAC del ordenador
//			- ida: Identificador del aula
//			- isd: Identificador del servidor dhcp
//			- isr: Identificador del servidor rembo
// ________________________________________________________________________________________________________
int alta_ordenador(Database db,Table tbl,int* ido,char *nor,char *iph,char*mac,int ida,int isd,int isr)
{
	char sqlstr[1000],ErrStr[200],strmac[20];
	int idordenador,lon,i,p;

	// Prepara mac
	lon=strlen(mac);
	p=0;
	for (i=0;i<lon;i++){
		if(mac[i]!=' ') // Si no es espacio
			strmac[p++]=mac[i];
	}
	strmac[p]=(char)NULL;

	sprintf(sqlstr,"INSERT INTO ordenadores(nombreordenador,ip,mac,idperfilhard,idservidordhcp,idservidorrembo,idmenu,idaula,grupoid,idconfiguracion) VALUES ('%s','%s','%s',0,%d,%d,0,%d,0,0)",nor,iph,strmac,isd,isr,ida);
	if(!db.Execute(sqlstr)){ // Error al insertar
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	idordenador=0;
	// Toma identificador dela accin
	sprintf(sqlstr,"SELECT LAST_INSERT_ID() as identificador");
	if(!db.Execute(sqlstr,tbl)){ // Error al leer
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(!tbl.ISEOF()){ // Si existe registro
		if(!tbl.Get("identificador",idordenador)){
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}
	}
	*ido=idordenador;
	return(true);
}
// ________________________________________________________________________________________________________
// Funcin: Toma_idservidorres
//
//		Descripcin:
//			Esta funcin devuelve los identificadores de los servidores rembo y dhcp de un determinado ordenador
//		Parnetros:
//				db: Objeto base de datos (ya operativo)
//				tbl: Objeto tabla
//				ipd: ip del servidor dhcp
//				ipr: ip del servidor rembo
//				isd: identificador del servidor dhcp
//				isr: identificador del servidor rembo
// ________________________________________________________________________________________________________
int Toma_idservidorres(Database db,Table tbl,char*ipd,char*ipr,int*isd,int*isr)
{
	char sqlstr[1000],ErrStr[200];
	int identificador_dhcp,identificador_rembo;
	
	// Servidor dhcp
	sprintf(sqlstr,"SELECT idservidordhcp FROM servidoresdhcp where ip='%s'",ipd);
	if(!db.Execute(sqlstr,tbl)){ // Error al leer
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(!tbl.ISEOF()){ // Si existe registro
		if(!tbl.Get("idservidordhcp",identificador_dhcp)){
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}
	}
	// Servidor rembo
	sprintf(sqlstr,"SELECT idservidorrembo FROM servidoresrembo where ip='%s'",ipr);
	if(!db.Execute(sqlstr,tbl)){ // Error al leer
		db.GetErrorErrStr(ErrStr);
		return(false);
	}
	if(!tbl.ISEOF()){ // Si existe registro
		if(!tbl.Get("idservidorrembo",identificador_rembo)){
			tbl.GetErrorErrStr(ErrStr); // error al acceder al registro
			return(false);
		}
	}
	*isd=identificador_dhcp;
	*isr=identificador_rembo;
	return(true);
}
//************************************************************************************************************************************************
// PROGRAMA PRINCIPAL ( SERVICIO)
//***************************************************************************************************************************************************
int main (int argc, char *argv[])
{
    SOCKET	socket_s; // Socket donde escucha el servidor
    SOCKET	socket_c; // Socket de los clientes que se conectan
    int 	i;// Tamao de la estructura de direccionamiento IP del cliente
    socklen_t	iAddrSize;
    struct	sockaddr_in local,cliente;
	//pthread_t hThread;
	//void *resul
	// Validación de parámetros
    strcpy (szPathFile, "hidra.cfg");
    strcpy (szPathFileCfg, "hidra.log");
    for(i = 1; (i+1) < argc; i+=2){
        if (argv[i][0] == '-'){
            switch (tolower(argv[i][1])){
                case 'f':
                    if (argv[i+1]!=NULL)
                        strcpy(szPathFile, argv[i+1]);
                    else{
                        RegistraLog("Fallo en los parámetros: Debe especificar el fichero de configuración del servicio",false);
                        exit(EXIT_FAILURE);
                        }
                    break;
                case 'l':
                    if (argv[i+1]!=NULL)
                        strcpy(szPathFileCfg, argv[i+1]);
                    else{
                        RegistraLog("Fallo en los parámetros: Debe especificar el fichero de log para el servicio",false);
                        exit(EXIT_FAILURE);
                        }
                    break;
                default:
                    	RegistraLog("Fallo de sintaxis en los parámetros: Debe especificar -f nombre_del_fichero_de_configuración_del_servicio",false);
                        exit(EXIT_FAILURE);
                    break;
            }
        }
    }
	if (szPathFile==NULL)
		{printf ("error\n");exit(EXIT_FAILURE);}
	if(!TomaConfiguracion(szPathFile)){ // Toma parametros de configuracion
			RegistraLog("NO existe fichero de configuración o contiene un error de sintaxis",false);
			exit(EXIT_FAILURE);
	}

	pthread_mutex_init(&guardia,NULL); // Creación del mutex para control de hebras

	for (i=0;i<MAXIMOS_SOCKETS;i++){
		tbsockets[i].ip[0]='\0'; // Inicializa IP
		tbsockets[i].sock=INVALID_SOCKET; // Inicializa Socket
	}
	RegistraLog("***Inicio de sesion***",false);

	socket_s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Crea socket
	if (socket_s == SOCKET_ERROR){
		RegistraLog("***socket() fallo:",true);
	}
	local.sin_addr.s_addr = htonl(INADDR_ANY); // selecciona interface
	local.sin_family = AF_INET;	
	local.sin_port = htons(puerto); // Puerto

	if (bind(socket_s,(struct sockaddr *)&local, // Enlaza socket
		sizeof(local)) == SOCKET_ERROR){
		RegistraLog("***bind() fallo:",true);
	}

	listen(socket_s,250); // Pone a escuchar al socket
	iAddrSize = sizeof(cliente);

	while (true) {  // Bucle para escuchar peticiones de clientes
		socket_c = accept(socket_s, (struct sockaddr *)&cliente,&iAddrSize);    
		if (socket_c == INVALID_SOCKET){ 
			RegistraLog("***accept() fallo:",true);
		    break;
		}
		//resul=pthread_create(&hThread,NULL,GestionaConexion,(void*)&socket_c);
		GestionaConexion(&socket_c);
		/*if(resul!=0){
			RegistraLog("***Fallo al crear la hebra cliente",false);
		    break;
        }
        */
		//pthread_detach(hThread);
		close(socket_c); // Cierra la conexion con el servidor hidra
	}
	close(socket_s);
	exit(EXIT_SUCCESS); 
}

