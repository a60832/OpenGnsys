// *************************************************************************************************************************************************
//	Libreria de scripts de Javascript
// Copyright 2003-2005  Jos� Manuel Alonso. Todos los derechos reservados.
// Fecha Creaci�n:2003-2004
// Fecha �ltima modificaci�n: Marzo-2006
// Nombre del fichero: propiedades_grupos.js
// Descripci�n : 
//		Este fichero implementa las funciones javascript del fichero propiedades_grupos.php
// *************************************************************************************************************************************************
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
//	
//	Confirma la edici�n 
//________________________________________________________________________________________________________
function confirmar(op){
	var opcion=op;
	ifr=document.getElementById("iframes_comodin"); // Toma objeto Iframe
	if(document.fdatos.literaltipo.value==LITAMBITO_GRUPOSORDENADORES){
		var wurl="../gestores/gestor_gruposordenadores.php?opcion="+opcion;
	}
	else
		var wurl="../gestores/gestor_grupos.php?opcion="+opcion
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
		wurl+="&idgrupo="+document.fdatos.idgrupo.value+"&literaltipo="+document.fdatos.literaltipo.value
	}
	ifr.src=wurl; // LLama a la p�gina gestora
}
//________________________________________________________________________________________________________
//	
//	Comprobar_datos 
//________________________________________________________________________________________________________
function comprobar_datos(){
	if (document.fdatos.nombregrupo.value=="") {
		alert(TbMsg[0]);
		document.fdatos.nombregrupo.focus();
		return(false);
	}
	return(true);
}
//________________________________________________________________________________________________________
//	
// Devuelve el resultado de insertar una nueva grupo en la base de datos a traves del IFRAME
// Especificaciones:
//		Los par�metros recibidos son:
//			- resul: resultado de la operaci�n de inserci�n (true si tuvo �xito)
//			- descrierror: Descripci�n del error en su caso
//			- nwid: Identificador asignado al nuevo registro
//			- tablanodo: Tabla nodo generada para el nuevo registro (�rbol de un s�lo un elemento)
//________________________________________________________________________________________________________
function resultado_insertar_grupos(resul,descrierror,nwid,tablanodo){
	farbol.resultado_insertar_grupos(resul,descrierror,nwid,tablanodo);
	selfclose();
}
//________________________________________________________________________________________________________
//	
//		Devuelve el resultado de modificar alg�n dato de  un grupo a traves del IFRAME
//		Especificaciones:
//		Los par�metros recibidos son:
//			- resul: resultado de la operaci�n de inserci�n ( true si tuvo �xito)
//			- descrierror: Descripci�n del error en su caso
//			- lit: Nuevo nombre del grupo
//________________________________________________________________________________________________________
function resultado_modificar_grupos(resul,descrierror,lit){
	farbol.resultado_modificar_grupos(resul,descrierror,lit);
	selfclose();
}
//________________________________________________________________________________________________________
//	
//		Devuelve el resultado de eliminar un grupo a traves del IFRAME
//		Especificaciones:
//		Los par�metros recibidos son:
//			- resul: resultado de la operaci�n de inserci�n ( true si tuvo �xito)
//			- descrierror: Descripci�n del error en su caso
//			- id: Identificador del registro que se quiso modificar
//________________________________________________________________________________________________________
function resultado_eliminar_grupos(resul,descrierror,id){
	farbol.resultado_eliminar_grupos(resul,descrierror,id);
	selfclose();
}
//________________________________________________________________________________________________________
function selfclose(){
	document.location.href="../nada.php";
}

