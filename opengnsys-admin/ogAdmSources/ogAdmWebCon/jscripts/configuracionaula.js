// ************************************************************************
//	Libreria de scripts de Javascript
// Copyright 2003-2005 -2005  Jos� Manuel Alonso. Todos los derechos reservados.
// Fichero: configuracionaula.js
// Este fichero implementa las funciones javascript del fichero configuracionaula.php
// ************************************************************************

var currentOrdenador=null // Ordenador elegido
var currentNombreordenador=null // Nombre del ordenador

// C�digo de los ambitos

var AMBITO_ORDENADORES=0x10;

// Literales de los ambitos

var LITAMBITO_ORDENADORES="ordenadores";
//____________________________________________________________________________
//	
//	Esta funci�n muestra un menu contextual 
//____________________________________________________________________________
function resalta(id,nom,nomflo){
	currentOrdenador=id
	currentNombreordenador=nom
	var menuctx=document.getElementById(nomflo); // El menu contextual
	muestra_contextual(event.clientX,event.clientY,menuctx) // muestra menu
	event.returnValue=false // Cancela efectos del evento
}
//____________________________________________________________________________
//	
//	Esta funci�n muestra los comandos a elegir para su ejecuci�n
//____________________________________________________________________________
function confirmarcomando(ambito,idc,interac){
	reset_contextual(-1,-1);
	var ambito;
	var idambito;
	var nombreambito;
	
	ambito=AMBITO_ORDENADORES
	idambito=currentOrdenador;
	nombreambito=currentNombreordenador
	
	var identificador=idc // identificador del comando
	var tipotrama='CMD'
	var wurl="../principal/dialogostramas.php?identificador="+identificador+"&tipotrama="+tipotrama+"&ambito="+ambito+"&idambito="+idambito+"&nombreambito="+nombreambito
	if(interac==0){
	   ifr=document.getElementById("iframes_comodin"); // Toma objeto Iframe
		ifr.src=wurl; // LLama a la p�gina gestora
	}
	else
		window.open(wurl,"frame_contenidos")
}
//____________________________________________________________________________
//	
//	Esta funci�n muestra las colas de acciones
//____________________________________________________________________________
function cola_acciones(tipoaccion){
	reset_contextual(-1,-1);
	var ambito;
	var idambito;
	var nombreambito;

	ambito=AMBITO_ORDENADORES
	idambito=currentOrdenador;
	nombreambito=currentNombreordenador;

	var wurl="../principal/colasacciones.php?ambito="+ambito+"&idambito="+idambito+"&nombreambito="+nombreambito+"&tipocola="+tipoaccion
	window.open(wurl,"frame_contenidos")
}
//____________________________________________________________________________
//	
//	Esta funci�n  muestra la configuraci�n de los ordenadores de un configuracionaula
// objeto de argumentos de eventos:
//	Especificaciones:
//		El objeto e tiene las siguientes atributos o propiedades:
//			- literalnodo: El literal del nodo
//			- idnodo: El identificador del nodo
//____________________________________________________________________________
function configuraciones(ambito){
		var identificador=currentOrdenador;
		switch(ambito){
			case AMBITO_configuracionaulaS:
					var identificador=currentconfiguracionaula;
					wurl="configuracionconfiguracionaula.php?idconfiguracionaula="+identificador
					 window.open(wurl,"frame_contenidos")
					break;
			case AMBITO_ORDENADORES:
					var identificador=currentOrdenador;
					wurl="configuracionordenador.php?idordenador="+identificador
					 window.open(wurl,"frame_contenidos")
					break;
		}
}	
