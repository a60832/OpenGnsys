<?
// *************************************************************************************************************************************************
// Aplicaci�n WEB: ogAdmWebCon
// Autor: Jos� Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fecha Creaci�n: Diciembre-2003
// Fecha �ltima modificaci�n: Febrero-2005
// Nombre del fichero: menucliente.php
// Descripci�n :Este fichero implementa el menu del browser de los clientes
// *************************************************************************************************************************************************
$iph=""; // Switch menu cliente
if (isset($_GET["iph"])) $iph=$_GET["iph"]; 

if(!empty($iph)){
	Header("Location:menubroser.php?iph= ".$iph); // Accede a la p�gina de menus
	exit;
}
?>
<HTML>
	<HEAD>
	</HEAD>
	<BODY>
	<P>Error de acceso al men� del cliente.</P>
</BODY>
</HTML>

