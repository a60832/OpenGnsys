<?
// *************************************************************************************************************************************************
// Aplicaci�n WEB: Hidra
// Copyright 2003-2005  Jos� Manuel Alonso. Todos los derechos reservados.
// Fecha Creaci�n: A�o 2003-2004
// Fecha �ltima modificaci�n: Mayo-2005
// Nombre del fichero: gestor_procedimientos.php
// Descripci�n :
//		Gestiona la ejecuci�n de procedimientos
// *************************************************************************************************************************************************
include_once("../includes/ctrlacc.php");
include_once("../clases/AdoPhp.php");
include_once("../clases/XmlPhp.php");
include_once("../clases/ArbolVistaXML.php");
include_once("../clases/SockHidra.php");
include_once("../includes/CreaComando.php");
include_once("../includes/constantes.php");
include_once("../includes/RecopilaIpesMacs.php");
//________________________________________________________________________________________________________
$opcion=0; // Inicializa parametros

$idprocedimiento=0; 
$ambito=0; 
$idambito=0; 

if (isset($_GET["idprocedimiento"])) $idprocedimiento=$_GET["idprocedimiento"];
if (isset($_GET["ambito"])) $ambito=$_GET["ambito"]; 
if (isset($_GET["idambito"])) $idambito=$_GET["idambito"]; 

$cadenaip="";
$cadenamac="";

$cmd=CreaComando($cadenaconexion); // Crea objeto comando
$resul=false;
if ($cmd){
	$resul=Gestiona();
	$cmd->Conexion->Cerrar();
}
// *************************************************************************************************************************************************
?>
<HTML>
<HEAD>
<BODY>
<?
$literal="resultado_ejecutar_procedimiento";
if ($resul){
	echo '<SCRIPT language="javascript">'.chr(13);
	echo 'window.parent.'.$literal."(1,'".$cmd->DescripUltimoError()." ',".$idprocedimiento.");".chr(13);
	echo '</SCRIPT>';
}
else{
	echo '<SCRIPT language="javascript">';
	echo "	window.parent.".$literal."(0,'".$cmd->DescripUltimoError()."',".$idprocedimiento.")";
	echo '</SCRIPT>';
}
?>
</BODY>
</HTML>	
<?
/**************************************************************************************************************************************************
	Ejecutar un procedimiento para un ambito concreto
________________________________________________________________________________________________________*/
function Gestiona(){
	global $EJECUCION_PROCEDIMIENTO;

	global	$cmd;
	global	$opcion;

	global	$idcentro;
	global	$idprocedimiento;
	global	$ambito;
	global	$idambito;

	global $EJECUCION_COMANDO;
	global $EJECUCION_TAREA;
	global $PROCESOS;
	global $ACCION_INICIADA;
	global $ACCION_SINERRORES; 
	global $idcentro;
	global $servidorhidra;
	global $hidraport;
	global $idprocedimiento;
	global $cmd;

	global $cadenaip;

	$tbComandos="";
	$tabla_comandos="";
	$cont_comandos=0;

	RecopilaIpesMacs($cmd,$ambito,$idambito); // Recopila Ipes para la variable par�metros

	$rs=new Recordset; 
	$cmd->texto="SELECT * FROM procedimientos_comandos WHERE idprocedimiento=".$idprocedimiento;
	$cmd->texto.=" ORDER by procedimientos_comandos.orden";
	$rs->Comando=&$cmd; 
	if (!$rs->Abrir()) return(false); // Error al abrir recordset
	$rs->Primero(); 
	// Recorre tareas-comandos
	while (!$rs->EOF){
			$tbComandos["idcomando"]=$rs->campos["idcomando"];
			$tbComandos["ambito"]=$ambito;
			$tbComandos["idambito"]=$idambito;
			$tbComandos["parametros"]=$rs->campos["parametros"];
			$tbComandos["parametros"].="iph=".$cadenaip.chr(13);
			$tbComandos["idnotificador"]=0;
			$tabla_comandos[$cont_comandos]=$tbComandos;
			$cont_comandos++;
			$rs->Siguiente();
	}
	$rs->Cerrar();

	//Creaci�n parametros para inserci�n
	$cmd->CreaParametro("@tipoaccion","",1);
	$cmd->CreaParametro("@idtipoaccion",0,1);
	$cmd->CreaParametro("@cateaccion",$PROCESOS,1);
	$cmd->CreaParametro("@ambito",0,1);
	$cmd->CreaParametro("@idambito",0,1);
	$cmd->CreaParametro("@ambitskwrk","",0);
	$cmd->CreaParametro("@fechahorareg","",0);
	$cmd->CreaParametro("@estado",$ACCION_INICIADA,0);
	$cmd->CreaParametro("@resultado",$ACCION_SINERRORES,0);
	$cmd->CreaParametro("@idcentro",$idcentro,1);
	$cmd->CreaParametro("@parametros","",0);	
	$cmd->CreaParametro("@accionid",0,1);	
	$cmd->CreaParametro("@idnotificador",0,1);	

	// Insertar acciones:comandos
	for ($i=0;$i<$cont_comandos;$i++){
		$tbComandos=$tabla_comandos[$i];
		$cmd->ParamSetValor("@tipoaccion",$EJECUCION_COMANDO);
		$cmd->ParamSetValor("@idtipoaccion",$tbComandos["idcomando"]);
		$cmd->ParamSetValor("@ambito",$tbComandos["ambito"]);
		$cmd->ParamSetValor("@idambito",$tbComandos["idambito"]);
		$cmd->ParamSetValor("@ambitskwrk","");
		$cmd->ParamSetValor("@fechahorareg",date("y/m/d H:i:s"));
		$cmd->ParamSetValor("@parametros",$tbComandos["parametros"]);
		$cmd->ParamSetValor("@accionid",0);
		$cmd->ParamSetValor("@idnotificador",$tbComandos["idnotificador"]);
		$cmd->texto="INSERT INTO acciones (tipoaccion,idtipoaccion,cateaccion,ambito,idambito,ambitskwrk,fechahorareg,estado,resultado,idcentro,parametros,accionid,idnotificador) VALUES (@tipoaccion,@idtipoaccion,@cateaccion,@ambito,@idambito,@ambitskwrk,@fechahorareg,@estado,@resultado,@idcentro,@parametros,@accionid,@idnotificador)";
		$resul=$cmd->Ejecutar();
		if(!$resul) return(false);
		$tbComandos["parametros"].="ids=".$cmd->Autonumerico().chr(13);

		$shidra=new SockHidra($servidorhidra,$hidraport); 
		if ($shidra->conectar()){ // Se ha establecido la conexi�n con el servidor hidra
			$shidra->envia_comando($tbComandos["parametros"]);
			$shidra->desconectar();
		}
	}
	return(true);
}
?>