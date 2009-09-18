// *************************************************************************************************************************************************
//	Libreria de scripts de Javascript
// Copyright 2003-2005  Jos� Manuel Alonso. Todos los derechos reservados.
// Fecha Creaci�n:2003-2004
// Fecha �ltima modificaci�n: Marzo-2005
// Nombre del fichero: imagenincremental.js
// Descripci�n : 
//		Este fichero implementa las funciones javascript del fichero imagenincremental.php
// *************************************************************************************************************************************************
//________________________________________________________________________________________________________
function gestion_componente(id,o){
	idimagen=document.fdatos.idimagen.value
	if (o.checked)
		var wurl="../gestores/gestor_imagenincremental.php?opcion="+op_alta+"&idimagen="+idimagen+"&idsoftincremental="+id
	else
		var wurl="../gestores/gestor_imagenincremental.php?opcion="+op_eliminacion+"&idimagen="+idimagen+"&idsoftincremental="+id
	ifr=document.getElementById("iframes_comodin"); // Toma objeto Iframe
	ifr.src=wurl; // LLama a la p�gina gestora
}
//________________________________________________________________________________________________________
function resultado_insertar_imagenincremental(resul,descrierror,nwid,tablanodo){
	if (!resul){ // Ha habido alg�n error en la inserci�n
		alert(descrierror)
		return
	}
	alert(TbMsg[0])
}
//________________________________________________________________________________________________________
function resultado_eliminar_imagenincremental(resul,descrierror,idp,idh){
	if (!resul){ // Ha habido alg�n error en la eliminaci�n
		alert(descrierror)
		return
	}
	alert(TbMsg[1])
}
