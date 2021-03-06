<?php
// *************************************************************************************************************************************************
// Aplicación WEB: ogAdmWebCon
// Autor: José Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fecha Creación: Año 2009-2010
// Fecha Última modificación: Agosto-2010
// Nombre del fichero: gestor_procedimientoscomandos.php
// Descripción :
//		Gestiona el mantenimiento de la tabla de procedimientos_comandos
// *************************************************************************************************************************************************
include_once("../includes/ctrlacc.php");
include_once("../clases/AdoPhp.php");
include_once("../includes/CreaComando.php");
include_once("../includes/opciones.php");
//________________________________________________________________________________________________________
$opcion=0; // Inicializa parametros
$idprocedimientocomando=0; 
$orden=0; 

if (isset($_POST["opcion"])) $opcion=$_POST["opcion"]; // Recoge parametros
if (isset($_POST["idprocedimientocomando"])) $idprocedimientocomando=$_POST["idprocedimientocomando"];
if (isset($_POST["orden"])) $orden=$_POST["orden"];

$cmd=CreaComando($cadenaconexion); // Crea objeto comando
$resul=false;
if ($cmd){
	$resul=Gestiona();
	$cmd->Conexion->Cerrar();
}
$literal="";
switch($opcion){
	case $op_eliminacion :
		$literal="resultado_eliminar_procedimientocomando";
		break;
	case $op_modificacion :
		$literal="resultado_modificar_procedimientocomando";
		break;
	default:
		break;
}
if ($resul){
	echo $literal."(1,'".$cmd->DescripUltimoError()." ',".$idprocedimientocomando.");";
}
else{
	echo $literal."(0,'".$cmd->DescripUltimoError()."',".$idprocedimientocomando.");";
}
// *************************************************************************************************************************************************
function Gestiona(){
	global	$cmd;
	global	$opcion;
	global	$op_modificacion;
	global	$op_eliminacion;
	global $idprocedimientocomando;
	global$orden;

	$cmd->CreaParametro("@orden",$orden,1);
	switch($opcion){
		case $op_modificacion :
			$cmd->texto='UPDATE procedimientos_comandos set orden=@orden WHERE idprocedimientocomando='.$idprocedimientocomando;
			$resul=$cmd->Ejecutar();
			break;
		case $op_eliminacion :
			$cmd->texto='DELETE  FROM procedimientos_comandos WHERE idprocedimientocomando='.$idprocedimientocomando;
			$resul=$cmd->Ejecutar();
			break;
		default:
			break;
	}
	return($resul);
}

