<?
// ********************************************************************************************************
// Aplicaci�n WEB: ogAdmWebCon
// Autor: Jos� Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fecha Creaci�n: Diciembre-2003
// Fecha �ltima modificaci�n: Marzo-2005
// Nombre del fichero: barramenu.php
// Descripci�n :Este fichero implementa el menu general de la Aplicaci�n
// ********************************************************************************************************
if(isset($_SESSION)){ 	// Si existe algua sesi�n ...
	session_unset(); // Elimina variables
	session_destroy(); // Destruye sesi�n
}
$herror=0;
if (isset($_GET["herror"])) $herror=$_GET["herror"]; 
if (isset($_POST["herror"])) $herror=$_POST["herror"]; 
Header("Location: acceso_esp.php?herror=".$herror); // Redireccionamiento a la p�gina de inicio en el idioma por defecto
?>