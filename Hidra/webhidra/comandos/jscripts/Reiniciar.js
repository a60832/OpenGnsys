// *************************************************************************************************************************************************
//	Libreria de scripts de Javascript
// Copyright 2003-2005 Jos� Manuel Alonso. Todos los derechos reservados.
// Fecha Creaci�n:2003-2004
// Fecha �ltima modificaci�n: Marzo-2005
// Nombre del fichero: Reiniciar.js
// Descripci�n : 
//		Este fichero implementa las funciones javascript del fichero Reiniciar.php (Comandos)
// *************************************************************************************************************************************************
 function confirmar(){
	if (comprobar_datos()){
		var wurl="./gestores/gestor_Reiniciar.php?" +compone_urlejecucion();
		ifr=document.getElementById("iframes_comodin"); // Toma objeto Iframe
		ifr.src=wurl; // LLama a la p�gina gestora
	}
 }
//__________________________________________________________________________________________________
  function cancelar(){
	alert(CTbMsg[0]);
	location.href="../nada.php"
}
//__________________________________________________________________________________________________
  function comprobar_datos(){
			return(comprobar_datosejecucion())
}
//__________________________________________________________________________________________________
//	
//	Comprobar retorno
//__________________________________________________________________________________________________
function resultado_reiniciar(resul){
	if (!resul){
		alert(CTbMsg[1]);	
		return
	}
	alert(CTbMsg[2]);	
}
