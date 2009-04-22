// ************************************************************************
//	Libreria de scripts de Javascript
// Copyright 2005 Jos� Manuel Alonso. Todos los derechos reservados.
// Fichero: parametrostrabajos.js
// ************************************************************************
//____________________________________________________________________________
//	
//	Esta funci�n muestra una tabala de  parametros de comandos
//		Los par�metros recibidos son:
//			- id: identificador de la tarea
//____________________________________________________________________________
function abre_tablaparametros(id){
	var oTabla=document.getElementById("tablaparametros-"+id); // Toma objeto DIV
	if (oTabla.style.display=="block")
		oTabla.style.display="none";
	else
		oTabla.style.display="block"
}
