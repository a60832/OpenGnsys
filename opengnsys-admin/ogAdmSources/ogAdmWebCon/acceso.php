<?
// *************************************************************************************************************************************************
// Aplicaci�n WEB: Hidra
// Copyright 2003-2005 Jos� Manuel Alonso. Todos los derechos reservados.
// Fecha Creaci�n: Diciembre-2003
// Fecha �ltima modificaci�n: Marzo-2005
// Nombre del fichero: acceso.php
// Descripci�n : Redirecciona a la p�gia de inicio del idioma que se quiere tener por defecto
// *************************************************************************************************************************************************
if(isset($_SESSION)){ 	// Si existe algua sesi�n ...
	session_unset(); // Elimina variables
	session_destroy(); // Destruye sesi�n
}
$herror=0;
if (isset($_GET["herror"])) $herror=$_GET["herror"]; 
if (isset($_POST["herror"])) $herror=$_POST["herror"]; 
Header("Location: acceso_esp.php?herror=".$herror); // Redireccionamiento a la p�gina de inicio en el idioma por defecto
?>