<?
// *************************************************************************************************************************************************
// Aplicaci�n WEB: Hidra
// Copyright 200-2005 Jos� Manuel Alonso. Todos los derechos reservados.
// Fecha Creaci�n: A�o 2003-2004
// Fecha �ltima modificaci�n: Noviembre-2005
// Nombre del fichero: toma_mes.php
// Descripci�n :
//		Crea la tabla del mes y el a�o elegidos
// *************************************************************************************************************************************************

include_once("../includes/ctrlacc.php");
include_once("../idiomas/php/".$idioma."/clases/Almanaque_".$idioma.".php");
?>
<HTML>
<HEAD>
<BODY>
<?
// Toma parametros
$pidmes=0;
$pidanno=0;
$pvitem=0;

if (isset($_GET["idmes"]))	$pidmes=$_GET["idmes"];
if (isset($_GET["idanno"])) $pidanno=$_GET["idanno"];
if (isset($_GET["vitem"])) $pvitem=$_GET["vitem"];

$mialmanaque= new Almanaque("tabla_meses");
$retorno=$mialmanaque->MesAnno($pidmes,$pidanno);
?>
<p>
<span id="mesanno_retorno"><?=$mialmanaque->MesAnno($pidmes,$pidanno);?></span>
<span id="vitem_retorno"><?=$pvitem?></span></p>

<SCRIPT language="javascript">
	var objr=document.getElementById("mesanno_retorno");
	var objvitem=document.getElementById("vitem_retorno");
	cadecalendario=objr.innerHTML;
	vitem=objvitem.innerText;
	window.parent.cambia_mesanno(cadecalendario,vitem);
</SCRIPT>
</BODY>
</HTML>