// *************************************************************************************************************************************************
//	Libreria de scripts de Javascript
// Copyright 2003-2005 -2005  Jos� Manuel Alonso. Todos los derechos reservados.
// Fichero: softwares.js
// Este fichero implementa las funciones javascript del fichero softwares.php
// *************************************************************************************************************************************************
//________________________________________________________________________________________________________
//	
//	Muestra formulario para gestionar los componentes incluidos en un perfil software 
//________________________________________________________________________________________________________
function insertar_perfilcomponente(){
	reset_contextual(-1,-1)
	var identificador=currentNodo.toma_identificador()
	var descripcionperfil=currentNodo.toma_infonodo();
	var whref="../varios/perfilcomponente_soft.php?idperfilsoft="+identificador+"&descripcionperfil="+descripcionperfil
	window.open(whref,"frame_contenidos")
}
//________________________________________________________________________________________________________
//	
//	Muestra informaci�n sobre los perfiles software
//________________________________________________________________________________________________________
function informacion_perfiles(){
	reset_contextual(-1,-1)
	var identificador=currentNodo.toma_identificador()
	var descripcionperfil=currentNodo.toma_infonodo();
	var whref="../varios/informacion_perfilessoftware.php?idperfil="+identificador+"&descripcionperfil="+descripcionperfil
	window.open(whref,"frame_contenidos")
}
//________________________________________________________________________________________________________
//	
//	Muestra formulario para gestionar los componentes incluidos en un software incremental
//________________________________________________________________________________________________________
function insertar_incrementalcomponente(){
	reset_contextual(-1,-1)
	var identificador=currentNodo.toma_identificador()
	var descripcionincremental=currentNodo.toma_infonodo();
	var whref="../varios/incrementalcomponente_soft.php?idsoftincremental="+identificador+"&descripcionincremental="+descripcionincremental
	window.open(whref,"frame_contenidos")
}
//________________________________________________________________________________________________________
//	
//	Muestra informaci�n sobre los perfiles software
//________________________________________________________________________________________________________
function informacion_incrementales(){
	reset_contextual(-1,-1)
	var identificador=currentNodo.toma_identificador();
	var descripcionincremental=currentNodo.toma_infonodo();
	var whref="../varios/informacion_incrementales.php?idsoftincremental="+identificador+"&descripcionincremental="+descripcionincremental
	window.open(whref,"frame_contenidos")
}
	