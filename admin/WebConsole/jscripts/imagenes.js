// *************************************************************************************************************************************************
//	Libreria de scripts de Javascript
// Autor: Jos� Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fichero: imagenes.js
// Este fichero implementa las funciones javascript del fichero imagenes.php
// *************************************************************************************************************************************************
//___________________________________________________________________________________________________________
//	
//	Muestra informaci�n sobre las im�genes
//___________________________________________________________________________________________________________
function muestra_informacion(){
	reset_contextual(-1,-1)
	var identificador=currentNodo.toma_identificador()
	var descripcionimagen=currentNodo.toma_infonodo()
	var whref="../varios/informacion_imagenes.php?idimagen="+identificador+"&descripcionimagen="+descripcionimagen
	window.open(whref,"frame_contenidos")
}
//________________________________________________________________________________________________________
//	
//	Muestra formulario para gestionar el software incremental incluido en una imagen
//________________________________________________________________________________________________________
function insertar_imagenincremental(){
	reset_contextual(-1,-1)
	var identificador=currentNodo.toma_identificador()
	var descripcionimagen=currentNodo.toma_infonodo()
	var whref="../varios/imagenincremental.php?idimagen="+identificador+"&descripcionimagen="+descripcionimagen
	window.open(whref,"frame_contenidos")
}
