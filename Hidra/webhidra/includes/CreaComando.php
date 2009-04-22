<?
// *************************************************************************************************************************************************
// Aplicaci�n WEB: Hidra.
// Copyright 2003-2005  Jos� Manuel Alonso. Todos los derechos reservados.
// Fecha Creaci�n:2003-2004
// Fecha �ltima modificaci�n: Febrero-2005
// Nombre del fichero: CreaComando.php
// Descripci�n :
//		Devuelve una objeto comando totalmente operativo (con la conexi�n abierta)
//	Parametros: 
//		- cadenaconexion: Una cadena con los datos necesarios para la conexi�n: nombre del servidor
//		usuario,password,base de datos,etc separados por coma
// *************************************************************************************************************************************************
function CreaComando($cadenaconexion){
	$strcn=split(";",$cadenaconexion);
	$cn=new Conexion; 
	$cmd=new Comando;	
	$cn->CadenaConexion($strcn[0],$strcn[1],$strcn[2],$strcn[3],$strcn[4]);
	if (!$cn->Abrir()) return (false); 
	$cmd->Conexion=&$cn; 
	return($cmd);
}
?>