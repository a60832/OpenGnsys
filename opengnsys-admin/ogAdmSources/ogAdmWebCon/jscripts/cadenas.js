// *************************************************************************************************************************************************
//	Libreria de scripts de Javascript
// Copyright 2003-2005  Jos� Manuel Alonso. Todos los derechos reservados.
// Fecha Creaci�n:2003-2004
// Fecha �ltima modificaci�n: Noviembre-2005
// Nombre del fichero: cadenas.js
// Descripci�n : 
//		Este fichero implementa funciones de uso com�n para cadenas
// *************************************************************************************************************************************************
function TrimLeft( str ) {
	var resultStr = "";
	var i = len = 0;
	if (str+"" == "undefined" || str == null) 	return null;
	str += "";
	if (str.length == 0) 
		resultStr = "";
	else { 
		len = str.length;
		while ((i <= len) && (str.charAt(i) == " "))	i++;
		resultStr = str.substring(i, len);
	}
	return resultStr;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
function TrimRight( str ) {
	var resultStr = "";
	var i = 0;
	if (str+"" == "undefined" || str == null) 	return null;
	str += "";
	if (str.length == 0) 
		resultStr = "";
	else {
		i = str.length - 1;
		while ((i >= 0) && (str.charAt(i) == " "))	i--;
		resultStr = str.substring(0, i + 1);
	}
	return resultStr; 
	}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
function Trim( str ) {
	var resultStr = "";
	resultStr = TrimLeft(str);
	resultStr = TrimRight(resultStr);
	return resultStr;
}