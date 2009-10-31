<?
// *************************************************************************************************************************************************
// Aplicaci�n WEB: ogAdmWebCon
// Autor: Jos� Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fecha Creaci�n: A�o 2003-2005
// Fecha �ltima modificaci�n: Abril-2005
// Nombre del fichero: ejecutarprocedimientos.php
// Descripci�n :
//		Administra procedimientos,tareas y trabajos de un determinado Centro
// *************************************************************************************************************************************************
include_once("../includes/ctrlacc.php");
include_once("../clases/AdoPhp.php");
include_once("../clases/XmlPhp.php");
include_once("../clases/ArbolVistaXML.php");
include_once("../clases/MenuContextual.php");
include_once("../includes/constantes.php");
include_once("../includes/CreaComando.php");
include_once("../idiomas/php/".$idioma."/ejecutarprocedimientos_".$idioma.".php");
//________________________________________________________________________________________________________
$ambito=0;
$idambito=0;
$nombreambito="";

if (isset($_GET["ambito"]))	$ambito=$_GET["ambito"]; 
if (isset($_GET["idambito"]))	$idambito=$_GET["idambito"]; 
if (isset($_GET["nombreambito"]))	$nombreambito=$_GET["nombreambito"]; 

//________________________________________________________________________________________________________
$cmd=CreaComando($cadenaconexion);
if (!$cmd)
	Header('Location: '.$pagerror.'?herror=2');  // Error de conexi�n con servidor B.D.
else
	$arbolXML=CreaArbol($cmd,$idcentro); // Crea el arbol XML con todos los datos de las acciones registradas en el Centro

// Creaci�n del �rbol
$baseurlimg="../images/tsignos"; // Url de las im�genes de signo
$clasedefault="tabla_listados_sin";
$titulotabla=$TbMsg[8];  
$arbol=new ArbolVistaXml($arbolXML,0,$baseurlimg,$clasedefault,1,20,100,1,$titulotabla);

//________________________________________________________________________________________________________
?>
<HTML>
<TITLE>Administraci�n web de aulas</TITLE>
<HEAD>
	<LINK rel="stylesheet" type="text/css" href="../estilos.css">
	<SCRIPT language="javascript" src="../clases/jscripts/ArbolVistaXML.js"></SCRIPT>
	<SCRIPT language="javascript" src="../clases/jscripts/MenuContextual.js"></SCRIPT>
	<SCRIPT language="javascript" src="../jscripts/comunes.js"></SCRIPT>
	<SCRIPT language="javascript" src="../jscripts/ejecutarprocedimientos.js"></SCRIPT>
	<? echo '<SCRIPT language="javascript" src="../idiomas/javascripts/'.$idioma.'/comunes_'.$idioma.'.js"></SCRIPT>'?>
	<? echo '<SCRIPT language="javascript" src="../idiomas/javascripts/'.$idioma.'/ejecutarprocedimientos_'.$idioma.'.js"></SCRIPT>'?>
</HEAD>
<BODY OnContextMenu="return false">
<FORM  name="fdatos"> 
	<INPUT type=hidden name=ambito value=<?=$ambito?>>
	<INPUT type=hidden name=idambito value=<?=$idambito?>>
</FORM>

<?
switch($ambito){
		case $AMBITO_CENTROS :
			$urlimg='../images/iconos/centros.gif';
			$textambito=$TbMsg[0];
			break;
		case $AMBITO_GRUPOSAULAS :
			$urlimg='../images/iconos/carpeta.gif';
			$textambito=$TbMsg[1];
			break;
		case $AMBITO_AULAS :
			$urlimg='../images/iconos/aula.gif';
			$textambito=$TbMsg[2];
			break;
		case $AMBITO_GRUPOSORDENADORES :
			$urlimg='../images/iconos/carpeta.gif';
			$textambito=$TbMsg[3];
			break;
		case $AMBITO_ORDENADORES :
			$urlimg='../images/iconos/ordenador.gif';
			$textambito=$TbMsg[4];
			break;
	}
	echo '<p align=center><span class=cabeceras>'.$TbMsg[5].'&nbsp;</span><br>';
	echo '<IMG src="'.$urlimg.'">&nbsp;&nbsp;<span align=center class=subcabeceras><U>'.$TbMsg[6].': '.$textambito.','.$nombreambito.'</U></span>&nbsp;&nbsp;</span></p>';
?>
<BR>
<BR>
<?
//________________________________________________________________________________________________________
echo $arbol->CreaArbolVistaXML();	 // Crea �rbol (HTML) a partir del XML
$flotante=new MenuContextual();			 // Crea objeto MenuContextual

// Crea contextual de los procedimientos
$XMLcontextual=CreacontextualXMLProcedimiento(); // Procedimientos
echo $flotante->CreaMenuContextual($XMLcontextual);
//________________________________________________________________________________________________________
include_once("../includes/iframecomun.php");
//________________________________________________________________________________________________________
?>
</BODY>
</HTML>
<?
// *************************************************************************************************************************************************
//	Devuelve una cadena con formato XML con toda la informaci�n de las acciones registradas en un Centro concreto
//	Parametros: 
//		- cmd:Una comando ya operativo ( con conexi�n abierta)  
//		- idcentro: El identificador del centro
//________________________________________________________________________________________________________
function CreaArbol($cmd,$idcentro){
	global $TbMsg;
	global $LITAMBITO_PROCEDIMIENTOS;

	$cadenaXML='<PROCEDIMIENTOS';
	// Atributos
	$cadenaXML.=' imagenodo="../images/iconos/carpeta.gif"';
	$cadenaXML.=' infonodo="'.$TbMsg[7].'"';
	$cadenaXML.=' nodoid=Raizpro'.$LITAMBITO_PROCEDIMIENTOS;
	$cadenaXML.='>';
	$cadenaXML.=SubarbolXML_grupos_procedimientos($cmd,$idcentro,0);
	$cadenaXML.='</PROCEDIMIENTOS>';

	return($cadenaXML);
}
//________________________________________________________________________________________________________
function SubarbolXML_grupos_procedimientos($cmd,$idcentro,$grupoid){
	global $AMBITO_GRUPOSPROCEDIMIENTOS;
	global $LITAMBITO_GRUPOSPROCEDIMIENTOS;
	$cadenaXML="";
	$rs=new Recordset; 
	$cmd->texto="SELECT idgrupo,nombregrupo,grupoid FROM grupos WHERE grupoid=".$grupoid." AND idcentro=".$idcentro." AND tipo=".$AMBITO_GRUPOSPROCEDIMIENTOS." ORDER BY nombregrupo";
	$rs->Comando=&$cmd; 
	if (!$rs->Abrir()) return($cadenaXML); // Error al abrir recordset
	$rs->Primero(); 
	while (!$rs->EOF){
		$cadenaXML.='<GRUPOSPROCEDIMIENTOS';
		// Atributos
		$cadenaXML.=' imagenodo="../images/iconos/carpeta.gif"';
		$cadenaXML.=' infonodo="'.$rs->campos["nombregrupo"].'"';
		$cadenaXML.=' nodoid='.$LITAMBITO_GRUPOSPROCEDIMIENTOS.'-'.$rs->campos["idgrupo"];
		$cadenaXML.='>';
		$cadenaXML.=SubarbolXML_grupos_procedimientos($cmd,$idcentro,$rs->campos["idgrupo"]);
		$cadenaXML.='</GRUPOSPROCEDIMIENTOS>';
		$rs->Siguiente();
	}
	$rs->Cerrar();
	$cadenaXML.=SubarbolXML_Procedimientos($cmd,$idcentro,$grupoid);
	return($cadenaXML);
}
//________________________________________________________________________________________________________
function SubarbolXML_Procedimientos($cmd,$idcentro,$grupoid){
	global $LITAMBITO_PROCEDIMIENTOS;
	$cadenaXML="";
	$rs=new Recordset; 
	$cmd->texto="SELECT idprocedimiento,descripcion  FROM procedimientos WHERE idcentro=".$idcentro." AND grupoid=".$grupoid." ORDER BY descripcion";
	$rs->Comando=&$cmd; 
	if (!$rs->Abrir()) return($cadenaXML); // Error al abrir recordset
	$rs->Primero(); 
	while (!$rs->EOF){
		$cadenaXML.='<PROCEDIMIENTO';
		// Atributos
		$cadenaXML.=' imagenodo="../images/iconos/procedimiento.gif"';
		$cadenaXML.=' infonodo="'.$rs->campos["descripcion"].'"';
		$cadenaXML.=' nodoid='.$LITAMBITO_PROCEDIMIENTOS.'-'.$rs->campos["idprocedimiento"];
		$cadenaXML.=' clickcontextualnodo="menu_contextual(this,' ."'flo_".$LITAMBITO_PROCEDIMIENTOS."'" .')"';
		$cadenaXML.='>';
		$cadenaXML.='</PROCEDIMIENTO>';
		$rs->Siguiente();
	}
	$rs->Cerrar();
	return($cadenaXML);
}
//________________________________________________________________________________________________________
//
//	Men�s Contextuales
//________________________________________________________________________________________________________
function CreacontextualXMLProcedimiento(){
	global $EJECUCION_PROCEDIMIENTO;
	global $AMBITO_PROCEDIMIENTOS;
	global $LITAMBITO_PROCEDIMIENTOS;
	global $TbMsg;

	$layerXML='<MENUCONTEXTUAL';
	$layerXML.=' idctx="flo_'.$LITAMBITO_PROCEDIMIENTOS.'"';
	$layerXML.=' maxanchu=170';
	$layerXML.=' swimg=1';
	$layerXML.='>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="EjecutarProcedimiento()"';
	$layerXML.=' imgitem="../images/iconos/comandos.gif"';
	$layerXML.=' textoitem='.$TbMsg[9];
	$layerXML.='></ITEM>';
	
	$layerXML.='</MENUCONTEXTUAL>';
	return($layerXML);
}
?>
