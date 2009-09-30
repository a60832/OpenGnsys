<?
// *************************************************************************************************************************************************
// Aplicaci�n WEB: ogAdmWebCon
// Autor: Jos� Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fecha Creaci�n: A�o 2003-2004
// Fecha �ltima modificaci�n: Marzo-2005
// Nombre del fichero: perfilessoft_eliminacion.php
// Descripci�n :
//	Elimina en cascada registros de la tabla perfilessoft 
//		Parametros: 
//		-	cmd:Una comando ya operativo (con conexi�n abierta)  
//		-	identificador: El identificador por el que se eliminar� el el perfil software
//		-	nombreid: Nombre del campo identificador del registro 
//		-	swid: Indica 0= El identificador es tipo alfanum�rico	1= EI identificador es tipo num�rico ( valor por defecto) *************************************************************************************************************************************************
function	EliminaPerfilessoft($cmd,$identificador,$nombreid,$swid=1){
	if (empty($identificador)) return(true);
	if($swid==0)
		$cmd->texto="SELECT  idperfilsoft  FROM  perfilessoft WHERE ".$nombreid."='".$identificador."'";
	else
		$cmd->texto='SELECT  idperfilsoft  FROM perfilessoft WHERE '.$nombreid.'='.$identificador;
	$rs=new Recordset; 
	$rs->Comando=&$cmd; 
	if (!$rs->Abrir()) return(false); // Error al abrir recordset
	if ($rs->numeroderegistros==0) return(true);
	$rs->Primero(); 
	while (!$rs->EOF){
		$cmd->texto="UPDATE imagenes SET idperfilsoft=0 WHERE idperfilsoft=".$rs->campos["idperfilsoft"];
		$resul=$cmd->Ejecutar();
		if ($resul){
			$cmd->texto="DELETE  FROM perfileshard_perfilessoft  WHERE idperfilsoft=".$rs->campos["idperfilsoft"];
			$resul=$cmd->Ejecutar();
		}
		if (!$resul){
			$rs->Cerrar();
			return(false);
		}
		$rs->Siguiente();
	}
	if($swid==0)
		$cmd->texto="DELETE  FROM perfilessoft WHERE ".$nombreid."='".$identificador."'";
	else
		$cmd->texto='DELETE  FROM perfilessoft  WHERE '.$nombreid.'='.$identificador;
	$resul=$cmd->Ejecutar();
	return($resul);
}
?>
