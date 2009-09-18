// *************************************************************************************************************************************************
//	Libreria de scripts de Javascript
// Copyright 2003-2005  Jos� Manuel Alonso. Todos los derechos reservados.
// Fecha Creaci�n:2003-2005
// Fecha �ltima modificaci�n: abril-2005
// Nombre del fichero: ejecutarprocedimientos.js
// Descripci�n : 
//		Este fichero implementa las funciones javascript del fichero ejecutarprocedimientos.php
// *************************************************************************************************************************************************
//________________________________________________________________________________________________________
//	
//  Env�a un comando para su ejecuci�n o incorporaci�n a procedimientos o tareas
//________________________________________________________________________________________________________
function EjecutarProcedimiento(){
	reset_contextual(-1,-1); // Oculta menu contextual
	var resul=window.confirm(TbMsg[0]);
	if (!resul) return
	var ambito=document.fdatos.ambito.value
	var idambito=document.fdatos.idambito.value
	var idprocedimiento=currentNodo.toma_identificador() // identificador del ambito
	var wurl="../gestores/gestor_ejecutarprocedimientos.php?ambito="+ambito+"&idambito="+idambito+"&idprocedimiento="+idprocedimiento
	var ifr=document.getElementById("iframes_comodin"); // Toma objeto Iframe
	ifr.src=wurl; // LLama a la p�gina gestora
}
//________________________________________________________________________________________________________
//	
//	Devuelve el resultado de ejecutar un procedimiento sobre un �mbito
//	Par�metros:
//			- resul: resultado de la operaci�n( true si tuvo �xito)
//			- descrierror: Descripci�n del error en su caso
//			- idt: Identificador de la tarea
//________________________________________________________________________________________________________
function resultado_ejecutar_procedimiento(resul,descrierror,idt){
	if (!resul){ // Ha habido alg�n error en la ejecuci�n
		alert(descrierror)
		return
	}
	alert(TbMsg[1])
}
