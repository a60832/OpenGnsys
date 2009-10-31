<?
// *************************************************************************************************************************************************
// Aplicaci�n WEB: ogAdmWebCon
// Autor: Jos� Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla

// Fecha Creaci�n: Diciembre-2003
// Fecha �ltima modificaci�n: Febrero-2005
// Nombre del fichero: controlacceso.php
// Descripci�n :Este fichero implementa el control de acceso a la aplicaci�n
// *************************************************************************************************************************************************
include_once("./clases/AdoPhp.php");

$usu="";
$pss="";
if (isset($_POST["usu"])) $usu=$_POST["usu"]; 
if (isset($_POST["pss"])) $pss=$_POST["pss"]; 

include_once("./includes/controlacceso.inc");
?>
<HTML>
	<TITLE> Administraci�n web de aulas</TITLE>
	<HEAD>
		<LINK rel="stylesheet" type="text/css" href="hidra.css">
	</HEAD>
	<BODY>
		<DIV id="mensaje" style="Position:absolute;TOP:250;LEFT:330; visibility:visible">
		<SPAN  align=center class=subcabeceras>Acceso permitido. Espere por favor ...</SPAN></P>
		<SCRIPT LANGUAGE="JAVASCRIPT">
			var vez=0;
			setTimeout("acceso();",300);
			function acceso(){
				o=document.getElementById("mensaje");
				var s=o.style.visibility;
				if(s=="hidden")
					o.style.visibility="visible";
				else
					o.style.visibility="hidden";
				if(vez>5){
					var w=window.top;
					w.location="frames.php";
				}
				vez++;
				setTimeout("acceso();",300);
			}
	</SCRIPT>
 </BODY>
</HTML>
