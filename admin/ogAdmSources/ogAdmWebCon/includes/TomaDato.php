<?
// *************************************************************************************************************************************************
// Aplicaci�n WEB: ogAdmWebCon.
// Autor: Jos� Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fecha Creaci�n:2003-2004
// Fecha �ltima modificaci�n: Febrero-2005
// Nombre del fichero: TomaDato.php
// Descripci�n :
//			Recupera un dato de una tabla
//	Parametros: 
//		- cmd:Un comando ya operativo (con conexi�n abierta)  
//		- idcentro:Centro al que pertene el registro donde se encuentra el dato a recuperar, ser� 0 para no contemplar este dato
//		- nombretabla: Nombre de la tabla origen de los datos
//		- identificador: Valor del campo identificador del registro (cadena separada por comas si hay varios)
//		- nombreid: Nombre del campo identificador del registro (cadena separada por comas si hay varios)
//		- nombreliteral: Nombre del campo que se quiere recuperar
//		- swid: Indica 0= El identificador es tipo alfanum�rico	1= EI identificador es tipo num�rico ( valor por defecto)
//	 (*) En el caso de haber varios identificadores todos deben ser del mismo tipo  ( num�rico o alfanum�rico)
//*************************************************************************************************************************************************
function TomaDato($cmd,$idcentro,$nombretabla,$identificador,$nombreid,$nombreliteral,$swid=1){
	$Dato="";
	if (empty($identificador)) return($Dato);

	if($swid==0)	$ch='"';	else $ch=""; // Caracter comillas para campos alfanum�ricos
	$auxidentificador=split(";",$identificador);
	$auxnombreid=split(";",$nombreid);

	$clausulaWhere=" WHERE  ".$auxnombreid[0]."=".$ch.$auxidentificador[0].$ch;
	for ($i=1;$i<sizeof($auxidentificador);$i++)
		$clausulaWhere.=" AND ".$auxnombreid[$i]."=".$ch.$auxidentificador[$i].$ch;
	$cmd->texto="SELECT  *  FROM ".$nombretabla.$clausulaWhere;
	if (!empty($idcentro)) 
			$cmd->texto.=" AND idcentro=".$idcentro;
	$rs=new Recordset; 
	$rs->Comando=&$cmd; 
	if (!$rs->Abrir()) return(""); // Error al abrir recordset
	if (!$rs->EOF)
		$Dato.=$rs->campos[$nombreliteral];
	$rs->Cerrar();
	return($Dato);
}
