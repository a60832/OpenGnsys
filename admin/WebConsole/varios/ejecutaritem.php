<?
// *************************************************************************************************************************************************
// Aplicaci�n WEB: ogAdmWebCon
// Autor: Jos� Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fecha Creaci�n: A�o 2009
// Fecha �ltima modificaci�n: Octubre-2009
// Nombre del fichero: menumliente.php
// Descripci�n :
//		Pagina del menu del cliente. �ste la solicita a trav�s de su browser local
// *************************************************************************************************************************************************
include_once("../includes/ctrlacc.php");
include_once("../clases/AdoPhp.php");
include_once("../includes/CreaComando.php");
include_once("../includes/constantes.php");
include_once("../includes/opciones.php");
include_once("../includes/TomaDato.php");
//________________________________________________________________________________________________________
$iph="0.0.0.0";
$idt="0";

if (isset($_GET["iph"])) $iph=$_GET["iph"]; 
if (isset($_GET["idt"])) $idt=$_GET["idt"]; 
//________________________________________________________________________________________________________
$cmd=CreaComando($cadenaconexion);
if (!$cmd)
	Header('Location: '.$pagerror.'?herror=2');  // Error de conexi�n con servidor B.D.
//________________________________________________________________________________________________________
$rs=new Recordset; 
$cmd->texto="SELECT acciones_menus.tipoaccion, acciones_menus.idtipoaccion 
			FROM acciones_menus 
			WHERE acciones_menus.idaccionmenu=".$idt;
$rs->Comando=&$cmd; 
$resul=$rs->Abrir();
if (!$rs->Abrir()) die("NO SE HA PODIDO RECUEPARA EL ITEM PARA SER EJECUTADO");
if ($rs->EOF) die("EL ITEM PARA SER EJECUTADO NO EXISTE");
	
$tipoaccion=$rs->campos["tipoaccion"]; 
$idtipoaccion=$rs->campos["idtipoaccion"]; 
switch($tipoaccion){
	case $EJECUCION_PROCEDIMIENTO :
		$ambito=$AMBITO_ORDENADORES;
		$idambito=TomaDato($cmd,0,'ordenadores',$iph,'ip','idordenador',0);
		$wurl="../gestores/gestor_ejecutarprocedimientos.php";
		$wurl.="?swc=1&ambito=".$ambito."&idambito=".$idambito."&idprocedimiento=".$idtipoaccion;
		Header('Location: '.$wurl);  // Ejecuci�n procedimiento
		break;
	case $EJECUCION_TAREA :
		$wurl="../gestores/gestor_tareas.php?swc=1&opcion=".$op_ejecucion."&idtarea=".$idtipoaccion;
		Header('Location: '.$wurl);  // Ejecuci�n procedimiento
		break;
	case $EJECUCION_TRABAJO :
		$wurl="../gestores/gestor_trabajos.php?swc=1&opcion=".$op_ejecucion."&idtrabajo=".$idtipoaccion;
		Header('Location: '.$wurl);  // Ejecuci�n procedimiento
		break;
}
die("HA HABIDO ALG�N ERROR AL PROCESAR EL ITEM");
?>
