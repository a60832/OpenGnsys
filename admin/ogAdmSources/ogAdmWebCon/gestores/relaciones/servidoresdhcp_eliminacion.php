<?
// *************************************************************************************************************************************************
// Aplicaci�n WEB: ogAdmWebCon
// Autor: Jos� Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fecha Creaci�n: A�o 2003-2004
// Fecha �ltima modificaci�n: Marzo-2005
// Nombre del fichero: servidoresdhcp_eliminacion.php
// Descripci�n :
//	Elimina en cascada registros de la tabla servidoresdhcp 
//		Parametros: 
//		-	cmd:Una comando ya operativo (con conexi�n abierta)  
//		-	identificador: El identificador por el que se eliminar� el servidor dhcp
//		-	nombreid: Nombre del campo identificador del registro 
//		-	swid: Indica 0= El identificador es tipo alfanum�rico	1= EI identificador es tipo num�rico ( valor por defecto) *************************************************************************************************************************************************
function	EliminaServidoresdhcp($cmd,$identificador,$nombreid,$swid=1){
	if (empty($identificador)) return(true);
	if($swid==0)
		$cmd->texto="SELECT  idservidordhcp  FROM  servidoresdhcp WHERE ".$nombreid."='".$identificador."'";
	else
		$cmd->texto='SELECT  idservidordhcp  FROM servidoresdhcp WHERE '.$nombreid.'='.$identificador;
	$rs=new Recordset; 
	$rs->Comando=&$cmd; 
	if (!$rs->Abrir()) return(false); // Error al abrir recordset
	if ($rs->numeroderegistros==0) return(true);
	$rs->Primero(); 
	while (!$rs->EOF){
		$cmd->texto="UPDATE ordenadores SET idservidordhcp=0 WHERE idservidordhcp=".$rs->campos["idservidordhcp"];
		$resul=$cmd->Ejecutar();
		if (!$resul){
			$rs->Cerrar();
			return(false);
		}
		$rs->Siguiente();
	}
	if($swid==0)
		$cmd->texto="DELETE  FROM servidoresdhcp WHERE ".$nombreid."='".$identificador."'";
	else
		$cmd->texto='DELETE  FROM servidoresdhcp  WHERE '.$nombreid.'='.$identificador;
	$resul=$cmd->Ejecutar();
	return($resul);
}
?>
