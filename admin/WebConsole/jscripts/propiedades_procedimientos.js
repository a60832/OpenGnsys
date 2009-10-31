﻿// *************************************************************************************************************************************************
//	Libreria de scripts de Javascript
// Autor: José Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fecha Creación:2003-2004
// Fecha Última modificación: Febrero-2005
// Nombre del fichero: propiedades_procedimientos.js
// Descripción : 
//		Este fichero implementa las funciones javascript del fichero propiedades_procedimientos.php
// *************************************************************************************************************************************************
var wpadre=window.parent; // Toma frame padre
var farbol=wpadre.frames["frame_arbol"];
//________________________________________________________________________________________________________
//	
//	Cancela la edición 
//________________________________________________________________________________________________________
function cancelar(){
	selfclose();
}
//________________________________________________________________________________________________________
// Devuelve el resultado de insertar un registro
// Especificaciones:
//		Los parámetros recibidos son:
//			- resul: resultado de la operación de inserción (true si tuvo éxito)
//			- descrierror: Descripción del error en su caso
//			- nwid: Identificador asignado al nuevo registro
//			- tablanodo: Tabla nodo generada para el nuevo registro (árbol de un sólo un elemento)
//________________________________________________________________________________________________________
function resultado_insertar_procedimientos(resul,descrierror,nwid,tablanodo){
	farbol.resultado_insertar(resul,descrierror,nwid,tablanodo);
	selfclose();
}
//________________________________________________________________________________________________________
//	
//		Devuelve el resultado de modificar algún dato de un registro
//		Especificaciones:
//		Los parámetros recibidos son:
//			- resul: resultado de la operación de inserción ( true si tuvo éxito)
//			- descrierror: Descripción del error en su caso
//			- lit: Nuevo nombre del grupo
//________________________________________________________________________________________________________
function resultado_modificar_procedimientos(resul,descrierror,lit){
	farbol.resultado_modificar(resul,descrierror,lit);
	selfclose();
}
//________________________________________________________________________________________________________
//	
//		Devuelve el resultado de eliminar un registro
//		Especificaciones:
//		Los parámetros recibidos son:
//			- resul: resultado de la operación de inserción ( true si tuvo éxito)
//			- descrierror: Descripción del error en su caso
//			- id: Identificador del registro que se quiso modificar
//________________________________________________________________________________________________________
function resultado_eliminar_procedimientos(resul,descrierror,id){
	farbol.resultado_eliminar(resul,descrierror,id);
	selfclose();
}
//________________________________________________________________________________________________________
function selfclose(){
	document.location.href="../nada.php";
}
//________________________________________________________________________________________________________
//	
//	Confirma la edición 
//________________________________________________________________________________________________________
function confirmar(op){
	var opcion=op;
	ifr=document.getElementById("iframes_comodin"); // Toma objeto Iframe
	var wurl="../gestores/gestor_procedimientos.php?opcion="+opcion
	if (opcion!=op_eliminacion){
		if(!comprobar_datos()) return
		var valor
		var o
		var nit=document.forms[0].elements.length // Prepara la cadena de parámetros metodo get
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
		wurl+="&idprocedimiento="+document.fdatos.idprocedimiento.value
	}
	ifr.src=wurl; // LLama a la página gestora
}
//________________________________________________________________________________________________________
//	
//	Comprobar_datos 
//________________________________________________________________________________________________________
function comprobar_datos(){
	if (document.fdatos.descripcion.value=="") {
		alert(TbMsg[0]);
		document.fdatos.descripcion.focus();
		return(false);
	}
	return(true);
}
