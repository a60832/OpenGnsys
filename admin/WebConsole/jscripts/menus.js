// *************************************************************************************************************************************************
//	Libreria de scripts de Javascript
// Autor: Jos� Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fichero: menus.js
// Este fichero implementa las funciones javascript del fichero menus.php
// *************************************************************************************************************************************************
//___________________________________________________________________________________________________________
//	
//	Muestra informaci�n sobre los men�s y gestiona los items de cada uno de ellos
//___________________________________________________________________________________________________________
function muestra_informacion(){
	reset_contextual(-1,-1)
	var identificador=currentNodo.toma_identificador()
	var descripcionperfil=currentNodo.toma_infonodo()
	var whref="../varios/informacion_menus.php?idmenu="+identificador+"&descripcionmenu="+descripcionperfil
	window.open(whref,"frame_contenidos")
}
//____________________________________________________________________________
//
//	Recupera el navegador utilizado
//____________________________________________________________________________
var IE=(navigator.appName=="Microsoft Internet Explorer");
var NS=(navigator.appName=="Netscape");
