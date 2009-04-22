// *************************************************************************************************************************************************
//	Libreria de scripts de Javascript
// Copyright 2003-2005  Jos� Manuel Alonso. Todos los derechos reservados.
// Fecha Creaci�n:2003-2004
// Fecha �ltima modificaci�n: Marzo-2005
// Nombre del fichero: incrementalcomponente_soft.js
// Descripci�n : 
//		Este fichero implementa las funciones javascript del fichero incrementalcomponente_soft.php
// *************************************************************************************************************************************************
//________________________________________________________________________________________________________
function gestion_componente(id,o){
	idincremental=document.fdatos.idsoftincremental.value
	if (o.checked)
		var wurl="../gestores/gestor_incrementalcomponente_soft.php?opcion="+op_alta+"&idsoftincremental="+idincremental+"&idsoftware="+id
	else
		var wurl="../gestores/gestor_incrementalcomponente_soft.php?opcion="+op_eliminacion+"&idsoftincremental="+idincremental+"&idsoftware="+id
	ifr=document.getElementById("iframes_comodin"); // Toma objeto Iframe
	ifr.src=wurl; // LLama a la p�gina gestora
}
//________________________________________________________________________________________________________
function resultado_insertar_incrementalcomponente_soft(resul,descrierror,nwid,tablanodo){
	if (!resul){ // Ha habido alg�n error en la inserci�n
		alert(descrierror)
		return
	}
	alert(TbMsg[0])
}
//________________________________________________________________________________________________________
function resultado_eliminar_incrementalcomponente_soft(resul,descrierror,idp,idh){
	if (!resul){ // Ha habido alg�n error en la eliminaci�n
		alert(descrierror)
		return
	}
	alert(TbMsg[1])
}
