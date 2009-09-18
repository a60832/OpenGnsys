// *****************************************************************************************************************************************************
//	Libreria de scripts de Javascript
// Copyright 2003-2005  Jos� Manuel Alonso. Todos los derechos reservados.
// Fecha Creaci�n:2003-2004
// Fecha �ltima modificaci�n: Marzo-2005
// Nombre del fichero: propiedades_softincrementales.js
// Descripci�n : 
//		Este fichero implementa las funciones javascript del fichero propiedades_softincrementales.php
// *****************************************************************************************************************************************************
var wpadre=window.parent; // Toma frame padre
var farbol=wpadre.frames["frame_arbol"];
//________________________________________________________________________________________________________
//	
//	Cancela la edici�n 
//________________________________________________________________________________________________________
function cancelar(){
	selfclose();
}
//________________________________________________________________________________________________________
// Devuelve el resultado de insertar un registro
// Especificaciones:
//		Los par�metros recibidos son:
//			- resul: resultado de la operaci�n de inserci�n (true si tuvo �xito)
//			- descrierror: Descripci�n del error en su caso
//			- nwid: Identificador asignado al nuevo registro
//			- tablanodo: Tabla nodo generada para el nuevo registro (�rbol de un s�lo un elemento)
//________________________________________________________________________________________________________
function resultado_insertar_softincrementales(resul,descrierror,nwid,tablanodo){
	farbol.resultado_insertar(resul,descrierror,nwid,tablanodo);
	selfclose();
}
//________________________________________________________________________________________________________
//	
//		Devuelve el resultado de modificar alg�n dato de un registro
//		Especificaciones:
//		Los par�metros recibidos son:
//			- resul: resultado de la operaci�n de inserci�n ( true si tuvo �xito)
//			- descrierror: Descripci�n del error en su caso
//			- lit: Nuevo nombre del grupo
//________________________________________________________________________________________________________
function resultado_modificar_softincrementales(resul,descrierror,lit){
	farbol.resultado_modificar(resul,descrierror,lit);
	selfclose();
}
//________________________________________________________________________________________________________
//	
//		Devuelve el resultado de eliminar un registro
//		Especificaciones:
//		Los par�metros recibidos son:
//			- resul: resultado de la operaci�n de inserci�n ( true si tuvo �xito)
//			- descrierror: Descripci�n del error en su caso
//			- id: Identificador del registro que se quiso modificar
//________________________________________________________________________________________________________
function resultado_eliminar_softincrementales(resul,descrierror,id){
	farbol.resultado_eliminar(resul,descrierror,id);
	selfclose();
}
//________________________________________________________________________________________________________
function selfclose(){
	document.location.href="../nada.php";
}
//___________________________________________________________________________________________________________
//	
//	Confirma la edici�n 
//___________________________________________________________________________________________________________
function confirmar(op){
	var opcion=op;
	ifr=document.getElementById("iframes_comodin"); // Toma objeto Iframe
	var wurl="../gestores/gestor_softincrementales.php?opcion="+opcion
	if (opcion!=op_eliminacion){
		if(!comprobar_datos()) return
		var valor
		var o
		var nit=document.forms[0].elements.length // Prepara la cadena de par�metros metodo get
		for (i=0;i<nit;i++){
			o=document.forms[0].elements[i]
			valor=o.value
			if (valor=="on") {
					if(o.checked ) 
						valor=1; 
					else 
						valor=0
			}
			wurl+="&"+o.name+"="+valor
		}
	}
	else{
		wurl+="&idsoftincremental="+document.fdatos.idsoftincremental.value
	}
	ifr.src=wurl; // LLama a la p�gina gestora
}
//___________________________________________________________________________________________________________
//	
//	Comprobar_datos 
//___________________________________________________________________________________________________________
function comprobar_datos(){
	if (document.fdatos.descripcion.value=="") {
		alert(TbMsg[0]);
		document.fdatos.descripcion.focus();
		return(false);
	}
	return(true);
}
