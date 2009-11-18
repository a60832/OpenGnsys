<?
// *************************************************************************
// Aplicación WEB: ogAdmWebCon
// Autor: José Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fecha Creación: Año 2003-2004
// Fecha Última modificación: Marzo-2006
// Nombre del fichero: aulas.php
// Descripción : 
//		Administra grupos,aulas y ordenadores de un determinado Centro
// ****************************************************************************
include_once("../includes/ctrlacc.php");
include_once("../clases/AdoPhp.php");
include_once("../clases/XmlPhp.php");
include_once("../clases/ArbolVistaXML.php");
include_once("../clases/MenuContextual.php");
include_once("../includes/constantes.php");
include_once("../includes/CreaComando.php");
include_once("../idiomas/php/".$idioma."/aulas_".$idioma.".php");
//________________________________________________________________________________________________________
$cmd=CreaComando($cadenaconexion);
if (!$cmd)
	Header('Location: '.$pagerror.'?herror=2'); // Error de conexióncon servidor B.D.
else
	$arbolXML=CreaArbol($cmd,$idcentro,$nombrecentro); // Crea el arbol XML con todos los datos de aulas del Centro
// Creación del árbol
$baseurlimg="../images/signos"; // Url de las im�enes de signo
$clasedefault="texto_arbol"; // Hoja de estilo (Clase por defecto) del árbol
$arbol=new ArbolVistaXML($arbolXML,0,$baseurlimg,$clasedefault,2,0,5); // Crea el árbol (formato XML)
//________________________________________________________________________________________________________
?>
<HTML>
<TITLE>Administración web de aulas</TITLE>
<HEAD>
	<meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
	<LINK rel="stylesheet" type="text/css" href="../estilos.css">
	<SCRIPT language="javascript" src="../clases/jscripts/ArbolVistaXML.js"></SCRIPT>
	<SCRIPT language="javascript" src="../clases/jscripts/MenuContextual.js"></SCRIPT>
	<SCRIPT language="javascript" src="../jscripts/aulas.js"></SCRIPT>
	<SCRIPT language="javascript" src="../jscripts/opciones.js"></SCRIPT>
	<SCRIPT language="javascript" src="../jscripts/constantes.js"></SCRIPT>
	<SCRIPT language="javascript" src="../jscripts/comunes.js"></SCRIPT>	
	<? echo '<SCRIPT language="javascript" src="../idiomas/javascripts/'.$idioma.'/comunes_'.$idioma.'.js"></SCRIPT>'?>
	<? echo '<SCRIPT language="javascript" src="../idiomas/javascripts/'.$idioma.'/aulas_'.$idioma.'.js"></SCRIPT>'?>
</HEAD>
<BODY OnContextMenu="return false">
<?
//________________________________________________________________________________________________________
echo $arbol->CreaArbolVistaXML();	// Crea árbol (HTML) a partir del XML
$flotante=new MenuContextual();			 // Crea objeto MenuContextual

// Crea contextual de los Centros y aulas
$XMLcontextual=ContextualXMLCentros(); // Centros
echo $flotante->CreaMenuContextual($XMLcontextual); 
$XMLcontextual=ContextualXMLGruposAulas(); //  Grupos de aulas
echo $flotante->CreaMenuContextual($XMLcontextual);
$XMLcontextual=ContextualXMLAulas();  // Aulas
echo $flotante->CreaMenuContextual($XMLcontextual);
$XMLcontextual=CreacontextualXMLUsuarios(); // Operadores
echo $flotante->CreaMenuContextual($XMLcontextual);
$XMLcontextual=ContextualXMLGruposOrdenadores();  // Grupos de ordenadores
echo $flotante->CreaMenuContextual($XMLcontextual);
$XMLcontextual=ContextualXMLOrdenadores();  // Ordenadores
echo $flotante->CreaMenuContextual($XMLcontextual);

// Crea contextual de los comandos para los distintos �bitos
$XMLcontextual=ContextualXMLComandos($LITAMBITO_CENTROS,$AMBITO_CENTROS);
echo $flotante->CreaMenuContextual($XMLcontextual);
$XMLcontextual=ContextualXMLComandos($LITAMBITO_GRUPOSAULAS,$AMBITO_GRUPOSAULAS);
echo $flotante->CreaMenuContextual($XMLcontextual);
$XMLcontextual=ContextualXMLComandos($LITAMBITO_AULAS,$AMBITO_AULAS);
echo $flotante->CreaMenuContextual($XMLcontextual);
$XMLcontextual=ContextualXMLComandos($LITAMBITO_GRUPOSORDENADORES,$AMBITO_GRUPOSORDENADORES);
echo $flotante->CreaMenuContextual($XMLcontextual);
$XMLcontextual=ContextualXMLComandos($LITAMBITO_ORDENADORES,$AMBITO_ORDENADORES);
echo $flotante->CreaMenuContextual($XMLcontextual);

// Crea submenu contextual de clas de acciones
$XMLcontextual=ContextualXMLColasAcciones();  // Crea submenu contextual de acciones
echo $flotante->CreaMenuContextual($XMLcontextual);
//________________________________________________________________________________________________________
include_once("../includes/iframecomun.php");
//________________________________________________________________________________________________________
?>
</BODY>
</HTML>
<?
// *************************************************************************************************************************************************
//	Devuelve una cadena con formato XML con toda la informaci� de aulas y ordenadores registrados en un Centro concreto
//	Parametros: 
//		- cmd:Una comando ya operativo ( con conexiónabierta)  
//		- idcentro: El identificador del centro
//		- nombrecentro: El nombre del centro
//________________________________________________________________________________________________________
function CreaArbol($cmd,$idcentro,$nombrecentro){
	global $TbMsg;
	global $LITAMBITO_CENTROS;
	$cadenaXML='<CENTRO';
	// Atributos
	$cadenaXML.=' imagenodo="../images/iconos/centros.gif"';
	$cadenaXML.=' nodoid='.$LITAMBITO_CENTROS."-".$idcentro;
	$cadenaXML.=' infonodo="'.$nombrecentro.'"';
	$cadenaXML.=' clickcontextualnodo="menu_contextual(this,' ."'flo_".$LITAMBITO_CENTROS."'" .')"';
	$cadenaXML.='>';
	$cadenaXML.=SubarbolXML_grupos_aulas($cmd,$idcentro,0);
	$cadenaXML.='</CENTRO>';
	return($cadenaXML);
}
//________________________________________________________________________________________________________
function SubarbolXML_grupos_aulas($cmd,$idcentro,$grupoid){
	global $TbMsg;
	global $LITAMBITO_GRUPOSAULAS;
	global $LITAMBITO_AULAS;
	global $AMBITO_GRUPOSAULAS;
	$cadenaXML="";
	$rs=new Recordset; 
	$cmd->texto="SELECT idgrupo,nombregrupo,grupoid FROM grupos WHERE grupoid=".$grupoid." AND idcentro=".$idcentro." AND tipo=".$AMBITO_GRUPOSAULAS." ORDER BY nombregrupo";
	$rs->Comando=&$cmd; 
	if (!$rs->Abrir()) return($cadenaXML); // Error al abrir recordset
	$rs->Primero(); 
	while (!$rs->EOF){
		$cadenaXML.='<GRUPOSAULAS';
		// Atributos
		$cadenaXML.=' clickcontextualnodo="menu_contextual(this,'. " 'flo_".$LITAMBITO_GRUPOSAULAS."'" .');"';
		$cadenaXML.=' imagenodo="../images/iconos/carpeta.gif"';
		$cadenaXML.=' infonodo="'.$rs->campos["nombregrupo"].'"';
		$cadenaXML.=' nodoid='.$LITAMBITO_GRUPOSAULAS.'-'.$rs->campos["idgrupo"];
		$cadenaXML.='>';
		$cadenaXML.=SubarbolXML_grupos_aulas($cmd,$idcentro,$rs->campos["idgrupo"]);
		$cadenaXML.='</GRUPOSAULAS>';
		$rs->Siguiente();
	}
	$rs->Cerrar();
	$cmd->texto="SELECT idaula,nombreaula FROM aulas WHERE grupoid=".$grupoid." AND idcentro=".$idcentro." order by nombreaula";
	$rs->Comando=&$cmd; 
	if (!$rs->Abrir()) return($cadenaXML); // Error al abrir recordset
	$rs->Primero(); 
	while (!$rs->EOF){
		$cadenaXML.='<AULA ';
		// Atributos		
		$cadenaXML.=' clickcontextualnodo="menu_contextual(this,' ."'flo_".$LITAMBITO_AULAS."'" .')"';
		$cadenaXML.=' imagenodo="../images/iconos/aula.gif"';
		$cadenaXML.=' infonodo="'.$rs->campos["nombreaula"].'"';
		$cadenaXML.=' nodoid='.$LITAMBITO_AULAS.'-'.$rs->campos["idaula"];
		$cadenaXML.='>';
		$cadenaXML.=SubarbolXML_aulas_ordenadores($cmd,$rs->campos["idaula"],0);
		$cadenaXML.=SubarbolXML_aulas_operadores($cmd,$rs->campos["idaula"],&$cc);
		$cadenaXML.='</AULA>';
		$rs->Siguiente();
	}
	$rs->Cerrar();
	return($cadenaXML);
}
//________________________________________________________________________________________________________
function SubarbolXML_aulas_operadores($cmd,$idaula,$cont){
	global $TbMsg;
	global $LITAMBITO_USUARIOS;
	global $cadenaXML;
	global $OPERADOR;
	$cadenaXML="";
	$rs=new Recordset; 
	$cmd->texto="SELECT idusuario,nombre FROM usuarios WHERE idtipousuario=".$OPERADOR." AND idambito=".$idaula." ORDER by nombre";
	$rs->Comando=&$cmd; 
	if (!$rs->Abrir()) return($cadenaXML); // Error al abrir recordset
	$rs->Primero(); 
	if ($rs->EOF) return("");
	while (!$rs->EOF){
		$cont++;
		$cadenaXML.='<USUARIO';
		// Atributos			
		$cadenaXML.=' imagenodo="../images/iconos/operadores.gif"';
		$cadenaXML.=' clickcontextualnodo="menu_contextual(this,' ."'flo_".$LITAMBITO_USUARIOS."'" .')"';
		$cadenaXML.=' infonodo="'.$rs->campos["nombre"].'"';
		$cadenaXML.=' nodoid='.$LITAMBITO_USUARIOS.'-'.$rs->campos["idusuario"];
		$cadenaXML.='></USUARIO>';
		$rs->Siguiente();
	}
	$rs->Cerrar();
	return($cadenaXML);
}
//________________________________________________________________________________________________________
function SubarbolXML_aulas_ordenadores($cmd,$idaula,$grupoid){
	global $TbMsg;
	global $LITAMBITO_GRUPOSORDENADORES;
	global $LITAMBITO_ORDENADORES;
	$cadenaXML="";
	$rs=new Recordset; 
	$cmd->texto="SELECT idgrupo,nombregrupoordenador,grupoid FROM gruposordenadores WHERE grupoid=".$grupoid." AND idaula=".$idaula." ORDER BY  nombregrupoordenador";
	$rs->Comando=&$cmd; 
	if (!$rs->Abrir()) return($cadenaXML); // Error al abrir recordset
	$rs->Primero(); 
	while (!$rs->EOF){
		$cadenaXML.='<GRUPOSORDENADORES';
		// Atributos	
		$cadenaXML.=' clickcontextualnodo="menu_contextual(this,' ."'flo_".$LITAMBITO_GRUPOSORDENADORES."'" .')"';
		$cadenaXML.=' imagenodo="../images/iconos/carpeta.gif"';
		$cadenaXML.=' infonodo="'.$rs->campos["nombregrupoordenador"].'"';
		$cadenaXML.=' nodoid='.$LITAMBITO_GRUPOSORDENADORES.'-'.$rs->campos["idgrupo"];
		$cadenaXML.='>';
		$cadenaXML.=SubarbolXML_aulas_ordenadores($cmd,$idaula,$rs->campos["idgrupo"]);
		$cadenaXML.='</GRUPOSORDENADORES>';
		$rs->Siguiente();
	}
	$rs->Cerrar();
	$cmd->texto="SELECT idordenador,nombreordenador FROM ordenadores WHERE grupoid=".$grupoid." AND idaula=".$idaula." order by nombreordenador desc";
	$rs->Comando=&$cmd; 
	if (!$rs->Abrir()) return($cadenaXML); // Error al abrir recordset
	$rs->Primero(); 
	while (!$rs->EOF){
		$cadenaXML.='<ORDENADOR';
		// Atributos			
		$cadenaXML.=' imagenodo="../images/iconos/ordenador.gif"';
		$cadenaXML.=' clickcontextualnodo="menu_contextual(this,' ."'flo_".$LITAMBITO_ORDENADORES."'" .')"';
		$cadenaXML.=' infonodo="'.$rs->campos["nombreordenador"].'"';
		$cadenaXML.=' nodoid='.$LITAMBITO_ORDENADORES.'-'.$rs->campos["idordenador"];
		$cadenaXML.='></ORDENADOR>';
		$rs->Siguiente();
	}
	$rs->Cerrar();
	return($cadenaXML);
}
//________________________________________________________________________________________________________
//
//	Mens Contextuales
//________________________________________________________________________________________________________
function ContextualXMLCentros(){
	global $TbMsg;
	global $AMBITO_AULAS;
	global $AMBITO_GRUPOSAULAS;
	global $LITAMBITO_GRUPOSAULAS;
	global $AMBITO_CENTROS;
	global $LITAMBITO_CENTROS;
	global $RESERVA_CONFIRMADA;

	$layerXML='<MENUCONTEXTUAL';
	$layerXML.=' idctx="flo_'.$LITAMBITO_CENTROS.'"';
	$layerXML.=' maxanchu=160';
	$layerXML.=' swimg=1';
	$layerXML.=' clase="menu_contextual"';
	$layerXML.='>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="ver_aulas()"';
	$layerXML.=' textoitem='.$TbMsg[1];
	$layerXML.=' imgitem="../images/iconos/ordenadores.gif"';
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' subflotante="flo_colasacciones"';
	$layerXML.=' imgitem="../images/iconos/acciones.gif"';
	$layerXML.=' textoitem='.$TbMsg[6];
	$layerXML.='></ITEM>';

	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="insertar_grupos('.$AMBITO_GRUPOSAULAS.',' ."'".$LITAMBITO_GRUPOSAULAS."',1". ')"';

	$layerXML.=' imgitem="../images/iconos/carpeta.gif"';
	$layerXML.=' textoitem='.$TbMsg[0];
	$layerXML.='></ITEM>';

	$wLeft=170;
	$wTop=80;
	$wWidth=480;
	$wHeight=480;
	$wpages="../propiedades/propiedades_aulas.php";
	$wParam=$wLeft .",".$wTop.",".$wWidth.",".$wHeight.",'". $wpages."'";
	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="insertar('.$wParam.',1)"';
	$layerXML.=' imgitem="../images/iconos/aula.gif"';
	$layerXML.=' textoitem='.$TbMsg[3];
	$layerXML.='></ITEM>';
	
	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="actualizar_ordenadores()"';
	$layerXML.=' imgitem="../images/iconos/actualizar.gif"';
	$layerXML.=' textoitem='.$TbMsg[4];
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="purgar_ordenadores()"';
	$layerXML.=' imgitem="../images/iconos/purgar.gif"';
	$layerXML.=' textoitem='.$TbMsg[2];
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="conmutar_ordenadores()"';
	$layerXML.=' imgitem="../images/iconos/hidra.gif"';
	$layerXML.=' textoitem='.$TbMsg[33];
	$layerXML.='></ITEM>';

	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>';

	$layerXML.='<ITEM';
	$layerXML.=' subflotante="flo_comandos_'.$LITAMBITO_CENTROS.'"';
	$layerXML.=' imgitem="../images/iconos/comandos.gif"';
	$layerXML.=' textoitem='.$TbMsg[5];
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="confirmarprocedimiento('.$AMBITO_CENTROS.')"';
	$layerXML.=' imgitem="../images/iconos/procedimiento.gif"';
	$layerXML.=' textoitem='.$TbMsg[28];
	$layerXML.='></ITEM>';

	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="cola_reservas('.$RESERVA_CONFIRMADA.')"';
	$layerXML.=' imgitem="../images/iconos/reservas.gif"';
	$layerXML.=' textoitem='.$TbMsg[29];
	$layerXML.='></ITEM>';

	$layerXML.='</MENUCONTEXTUAL>';
	return($layerXML);
}
//________________________________________________________________________________________________________
function ContextualXMLGruposAulas(){
	global $TbMsg;
	global $AMBITO_AULAS;
	global $AMBITO_GRUPOSAULAS;
	global $LITAMBITO_GRUPOSAULAS;
	global $RESERVA_CONFIRMADA;

	$layerXML='<MENUCONTEXTUAL';
	$layerXML.=' idctx="flo_'.$LITAMBITO_GRUPOSAULAS.'"';
	$layerXML.=' maxanchu=155';
	$layerXML.=' swimg=1';
	$layerXML.=' clase="menu_contextual"';
	$layerXML.='>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="ver_aulas()"';
	$layerXML.=' textoitem='.$TbMsg[1];
	$layerXML.=' imgitem="../images/iconos/ordenadores.gif"';
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' subflotante="flo_colasacciones"';
	$layerXML.=' imgitem="../images/iconos/acciones.gif"';
	$layerXML.=' textoitem='.$TbMsg[6];
	$layerXML.='></ITEM>';

	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>'; 

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="insertar_grupos('.$AMBITO_GRUPOSAULAS.',' ."'".$LITAMBITO_GRUPOSAULAS."'". ')"';
	$layerXML.=' imgitem="../images/iconos/carpeta.gif"';
	$layerXML.=' textoitem='.$TbMsg[0];
	$layerXML.='></ITEM>';
	
	$wLeft=170;
	$wTop=80;
	$wWidth=480;
	$wHeight=480;
	$wpages="../propiedades/propiedades_aulas.php";
	$wParam=$wLeft .",".$wTop.",".$wWidth.",".$wHeight.",'". $wpages."'";
	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="insertar('.$wParam.')"';
	$layerXML.=' imgitem="../images/iconos/aula.gif"';
	$layerXML.=' textoitem='.$TbMsg[3];
	$layerXML.='></ITEM>';

	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="actualizar_ordenadores()"';
	$layerXML.=' imgitem="../images/iconos/actualizar.gif"';
	$layerXML.=' textoitem='.$TbMsg[4];
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="purgar_ordenadores()"';
	$layerXML.=' imgitem="../images/iconos/purgar.gif"';
	$layerXML.=' textoitem='.$TbMsg[2];
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="conmutar_ordenadores()"';
	$layerXML.=' imgitem="../images/iconos/hidra.gif"';
	$layerXML.=' textoitem='.$TbMsg[33];
	$layerXML.='></ITEM>';

	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>';

	$layerXML.='<ITEM';
	$layerXML.=' subflotante="flo_comandos_'.$LITAMBITO_GRUPOSAULAS.'"';
	$layerXML.=' imgitem="../images/iconos/comandos.gif"';
	$layerXML.=' textoitem='.$TbMsg[5];
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="confirmarprocedimiento('.$AMBITO_GRUPOSAULAS.')"';
	$layerXML.=' imgitem="../images/iconos/procedimiento.gif"';
	$layerXML.=' textoitem='.$TbMsg[28];
	$layerXML.='></ITEM>';

	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>'; 

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="modificar_grupos()"';
	$layerXML.=' imgitem="../images/iconos/modificar.gif"';
	$layerXML.=' textoitem='.$TbMsg[13];
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="eliminar_grupos()"';
	$layerXML.=' imgitem="../images/iconos/eliminar.gif"';
	$layerXML.=' textoitem='.$TbMsg[7];
	$layerXML.='></ITEM>';

	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="cola_reservas('.$RESERVA_CONFIRMADA.')"';
	$layerXML.=' imgitem="../images/iconos/reservas.gif"';
	$layerXML.=' textoitem='.$TbMsg[29];
	$layerXML.='></ITEM>';

	$layerXML.='</MENUCONTEXTUAL>';
	return($layerXML);
}
//________________________________________________________________________________________________________
function ContextualXMLAulas(){
	global $TbMsg;
	global $AMBITO_AULAS;
	global $LITAMBITO_AULAS;
	global $RESERVA_CONFIRMADA;
	global $OPERADOR;

	$layerXML='<MENUCONTEXTUAL';
	$layerXML.=' idctx="flo_'.$LITAMBITO_AULAS.'"';
	$layerXML.=' maxanchu=190';
	$layerXML.=' swimg=1';
	$layerXML.=' clase="menu_contextual"';
	$layerXML.='>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="ver_aulas()"';
	$layerXML.=' textoitem='.$TbMsg[1];
	$layerXML.=' imgitem="../images/iconos/ordenadores.gif"';
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' subflotante="flo_colasacciones"';
	$layerXML.=' imgitem="../images/iconos/acciones.gif"';
	$layerXML.=' textoitem='.$TbMsg[6];
	$layerXML.='></ITEM>';

	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="insertar_grupos('.$AMBITO_AULAS.',' ."'".$LITAMBITO_AULAS."'". ')"';

	$layerXML.=' imgitem="../images/iconos/carpeta.gif"';
	$layerXML.=' textoitem='.$TbMsg[8];
	$layerXML.='></ITEM>';
	
	/*
	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="insertar_ordenadores(1)"';
	$layerXML.=' imgitem="../images/iconos/ordenador.gif"';
	$layerXML.=' textoitem='.$TbMsg[9];
	$layerXML.='></ITEM>';
*/
	$wLeft=170;
	$wTop=80;
	$wWidth=480;
	$wHeight=480;
	$wpages="../propiedades/propiedades_ordenadores.php";
	$wParam=$wLeft .",".$wTop.",".$wWidth.",".$wHeight.",'". $wpages."'";
	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="insertar('.$wParam.')"';
	$layerXML.=' imgitem="../images/iconos/ordenador.gif"';
	$layerXML.=' textoitem='.$TbMsg[9];
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="colocar_ordenador(1)"';
	$layerXML.=' imgitem="../images/iconos/colocar.gif"';
	$layerXML.=' textoitem='.$TbMsg[11];
	$layerXML.='></ITEM>';

	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="actualizar_ordenadores()"';
	$layerXML.=' imgitem="../images/iconos/actualizar.gif"';
	$layerXML.=' textoitem='.$TbMsg[4];
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="purgar_ordenadores()"';
	$layerXML.=' imgitem="../images/iconos/purgar.gif"';
	$layerXML.=' textoitem='.$TbMsg[2];
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="conmutar_ordenadores()"';
	$layerXML.=' imgitem="../images/iconos/hidra.gif"';
	$layerXML.=' textoitem='.$TbMsg[33];
	$layerXML.='></ITEM>';

	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>';

	$layerXML.='<ITEM';
	$layerXML.=' subflotante="flo_comandos_'.$LITAMBITO_AULAS.'"';
	$layerXML.=' imgitem="../images/iconos/comandos.gif"';
	$layerXML.=' textoitem='.$TbMsg[5];
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="confirmarprocedimiento('.$AMBITO_AULAS.')"';
	$layerXML.=' imgitem="../images/iconos/procedimiento.gif"';
	$layerXML.=' textoitem='.$TbMsg[28];
	$layerXML.='></ITEM>';

	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="incorporarordenador()"';
	$layerXML.=' imgitem="../images/iconos/aula.gif"';
	$layerXML.=' textoitem='.$TbMsg[27];
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="ordenador_estandar()"';
	$layerXML.=' imgitem="../images/iconos/ordenadores.gif"';
	$layerXML.=' textoitem='.$TbMsg[12];
	$layerXML.='></ITEM>';

	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="configuraciones('.$AMBITO_AULAS.')"';
	$layerXML.=' textoitem='.$TbMsg[10];
	$layerXML.=' imgitem="../images/iconos/configuraciones.gif"';
	$layerXML.='></ITEM>';

	$wLeft=170;
	$wTop=80;
	$wWidth=480;
	$wHeight=480;
	$wpages="../propiedades/propiedades_aulas.php";
	$wParam=$wLeft .",".$wTop.",".$wWidth.",".$wHeight.",'". $wpages."'";
	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="modificar('.$wParam.')"';	
	$layerXML.=' textoitem='.$TbMsg[13];
	$layerXML.=' imgitem="../images/iconos/propiedades.gif"';
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="eliminar('.$wParam.')"';	
	$layerXML.=' imgitem="../images/iconos/eliminar.gif"';
	$layerXML.=' textoitem='.$TbMsg[14];
	$layerXML.='></ITEM>';

	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>';

	// Crear operador
	$wLeft=140;
	$wTop=115;
	$wWidth=400;
	$wHeight=320;
	$wpages="../propiedades/propiedades_usuarios.php?idtipousuario=".$OPERADOR;
	$wParam=$wLeft .",".$wTop.",".$wWidth.",".$wHeight.",'". $wpages."'";
	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="insertar('.$wParam.',3)"';
	$layerXML.=' imgitem="../images/iconos/operadores.gif"';
	$layerXML.=' textoitem='.$TbMsg[37];
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="cola_reservas('.$RESERVA_CONFIRMADA.')"';
	$layerXML.=' imgitem="../images/iconos/reservas.gif"';
	$layerXML.=' textoitem='.$TbMsg[29];
	$layerXML.='></ITEM>';

	$layerXML.='</MENUCONTEXTUAL>';
	return($layerXML);
}
//________________________________________________________________________________________________________
function CreacontextualXMLUsuarios(){
	global $LITAMBITO_USUARIOS;
	global $TbMsg;

	$layerXML='<MENUCONTEXTUAL';
	$layerXML.=' idctx="flo_'.$LITAMBITO_USUARIOS.'"';
	$layerXML.=' maxanchu=130';
	$layerXML.=' swimg=1';
	$layerXML.=' clase="menu_contextual"';
	$layerXML.='>';

	// Modificar usuarios
	$wLeft=140;
	$wTop=115;
	$wWidth=400;
	$wHeight=320;
	$wpages="../propiedades/propiedades_usuarios.php";
	$wParam=$wLeft .",".$wTop.",".$wWidth.",".$wHeight.",'". $wpages."'";
	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="modificar('.$wParam.')"';	
	$layerXML.=' textoitem='.$TbMsg[13];
	$layerXML.=' imgitem="../images/iconos/propiedades.gif"';
	$layerXML.='></ITEM>';

	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="eliminar('.$wParam.')"';	
	$layerXML.=' imgitem="../images/iconos/eliminar.gif"';
	$layerXML.=' textoitem='.$TbMsg[36];
	$layerXML.='></ITEM>';

	$layerXML.='</MENUCONTEXTUAL>';
	return($layerXML);
}
//________________________________________________________________________________________________________
function ContextualXMLGruposOrdenadores(){
	global $TbMsg;
	global $AMBITO_GRUPOSORDENADORES;
	global $LITAMBITO_GRUPOSORDENADORES;
	$layerXML='<MENUCONTEXTUAL';

	$layerXML.=' idctx="flo_'.$LITAMBITO_GRUPOSORDENADORES.'"';
	$layerXML.=' maxanchu=195';
	$layerXML.=' swimg=1';
	$layerXML.=' clase="menu_contextual"';
	$layerXML.='>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="ver_aulas()"';
	$layerXML.=' textoitem='.$TbMsg[1];
	$layerXML.=' imgitem="../images/iconos/ordenadores.gif"';
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' subflotante="flo_colasacciones"';
	$layerXML.=' imgitem="../images/iconos/acciones.gif"';
	$layerXML.=' textoitem='.$TbMsg[6];
	$layerXML.='></ITEM>';

 	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="insertar_grupos('.$AMBITO_GRUPOSORDENADORES.',' ."'".$LITAMBITO_GRUPOSORDENADORES."'". ')"';
	$layerXML.=' imgitem="../images/iconos/carpeta.gif"';
	$layerXML.=' textoitem='.$TbMsg[8];
	$layerXML.='></ITEM>';
	
	$wLeft=170;
	$wTop=80;
	$wWidth=480;
	$wHeight=480;
	$wpages="../propiedades/propiedades_ordenadores.php";
	$wParam=$wLeft .",".$wTop.",".$wWidth.",".$wHeight.",'". $wpages."'";
	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="insertar('.$wParam.')"';
	$layerXML.=' imgitem="../images/iconos/ordenador.gif"';
	$layerXML.=' textoitem='.$TbMsg[9];
	$layerXML.='></ITEM>';

	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="actualizar_ordenadores()"';
	$layerXML.=' imgitem="../images/iconos/actualizar.gif"';
	$layerXML.=' textoitem='.$TbMsg[4];
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="purgar_ordenadores()"';
	$layerXML.=' imgitem="../images/iconos/purgar.gif"';
	$layerXML.=' textoitem='.$TbMsg[2];
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="conmutar_ordenadores()"';
	$layerXML.=' imgitem="../images/iconos/hidra.gif"';
	$layerXML.=' textoitem='.$TbMsg[33];
	$layerXML.='></ITEM>';

	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>';

	$layerXML.='<ITEM';
	$layerXML.=' subflotante="flo_comandos_'.$LITAMBITO_GRUPOSORDENADORES.'"';
	$layerXML.=' imgitem="../images/iconos/comandos.gif"';
	$layerXML.=' textoitem='.$TbMsg[5];
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="confirmarprocedimiento('.$AMBITO_GRUPOSORDENADORES.')"';
	$layerXML.=' imgitem="../images/iconos/procedimiento.gif"';
	$layerXML.=' textoitem='.$TbMsg[28];
	$layerXML.='></ITEM>';

	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="colocar_ordenador(0)"';
	$layerXML.=' imgitem="../images/iconos/colocar.gif"';
	$layerXML.=' textoitem='.$TbMsg[11];
	$layerXML.='></ITEM>';

	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>'; 
	
	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="configuraciones('.$AMBITO_GRUPOSORDENADORES.')"';
	$layerXML.=' textoitem='.$TbMsg[10];
	$layerXML.=' imgitem="../images/iconos/configuraciones.gif"';
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="modificar_grupos()"';
	$layerXML.=' imgitem="../images/iconos/modificar.gif"';
	$layerXML.=' textoitem='.$TbMsg[13];
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="eliminar_grupos()"';
	$layerXML.=' imgitem="../images/iconos/eliminar.gif"';
	$layerXML.=' textoitem='.$TbMsg[16];
	$layerXML.='></ITEM>';

	$layerXML.='</MENUCONTEXTUAL>';
	return($layerXML);
}
//________________________________________________________________________________________________________
function ContextualXMLOrdenadores(){
	global $TbMsg;
	global $AMBITO_ORDENADORES;
	global $LITAMBITO_ORDENADORES;

	$layerXML='<MENUCONTEXTUAL';
	$layerXML.=' idctx="flo_'.$LITAMBITO_ORDENADORES.'"';
	$layerXML.=' maxanchu=140';
	$layerXML.=' swimg=1';
	$layerXML.=' clase="menu_contextual"';
	$layerXML.='>';

	$layerXML.='<ITEM';
	$layerXML.=' subflotante="flo_colasacciones"';
	$layerXML.=' imgitem="../images/iconos/acciones.gif"';
	$layerXML.=' textoitem='.$TbMsg[6];
	$layerXML.='></ITEM>';

	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="mover_ordenador()"';
	$layerXML.=' imgitem="../images/iconos/mover.gif"';
	$layerXML.=' textoitem='.$TbMsg[17];
	$layerXML.='></ITEM>';

	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="actualizar_ordenadores()"';
	$layerXML.=' imgitem="../images/iconos/actualizar.gif"';
	$layerXML.=' textoitem='.$TbMsg[4];
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="purgar_ordenadores()"';
	$layerXML.=' imgitem="../images/iconos/purgar.gif"';
	$layerXML.=' textoitem='.$TbMsg[2];
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="conmutar_ordenadores()"';
	$layerXML.=' imgitem="../images/iconos/hidra.gif"';
	$layerXML.=' textoitem='.$TbMsg[33];
	$layerXML.='></ITEM>';

	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>';

	$layerXML.='<ITEM';
	$layerXML.=' subflotante="flo_comandos_'.$LITAMBITO_ORDENADORES.'"';
	$layerXML.=' imgitem="../images/iconos/comandos.gif"';
	$layerXML.=' textoitem='.$TbMsg[5];
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="confirmarprocedimiento('.$AMBITO_ORDENADORES.')"';
	$layerXML.=' imgitem="../images/iconos/procedimiento.gif"';
	$layerXML.=' textoitem='.$TbMsg[28];
	$layerXML.='></ITEM>';

	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="configuraciones('.$AMBITO_ORDENADORES.')"';
	$layerXML.=' textoitem='.$TbMsg[10];
	$layerXML.=' imgitem="../images/iconos/configuraciones.gif"';
	$layerXML.='></ITEM>';

	$wLeft=170;
	$wTop=80;
	$wWidth=480;
	$wHeight=400;
	$wpages="../propiedades/propiedades_ordenadores.php";
	$wParam=$wLeft .",".$wTop.",".$wWidth.",".$wHeight.",'". $wpages."'";
	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="modificar('.$wParam.')"';	

	$layerXML.=' textoitem='.$TbMsg[13];
	$layerXML.=' imgitem="../images/iconos/propiedades.gif"';
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="eliminar('.$wParam.')"';	
	$layerXML.=' imgitem="../images/iconos/eliminar.gif"';
	$layerXML.=' textoitem='.$TbMsg[18];
	$layerXML.='></ITEM>';

	$layerXML.='</MENUCONTEXTUAL>';
	return($layerXML);
}
//________________________________________________________________________________________________________
function ContextualXMLColasAcciones(){
	global $TbMsg;
	global $EJECUCION_COMANDO;
	global $EJECUCION_TAREA;
	global $EJECUCION_TRABAJO;

	$layerXML='<MENUCONTEXTUAL';
	$layerXML.=' idctx="flo_colasacciones"';
	$layerXML.=' maxanchu=90';
	$layerXML.=' swimg=1';
	$layerXML.=' clase="menu_contextual"';
	$layerXML.='>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="cola_acciones('.$EJECUCION_COMANDO.')"';
	$layerXML.=' imgitem="../images/iconos/comandos.gif"';
	$layerXML.=' textoitem='.$TbMsg[5];
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="cola_acciones('.$EJECUCION_TAREA.')"';
	$layerXML.=' imgitem="../images/iconos/tareas.gif"';
	$layerXML.=' textoitem='.$TbMsg[19];
	$layerXML.='></ITEM>';

	$layerXML.='<ITEM';
	$layerXML.=' alpulsar="cola_acciones('.$EJECUCION_TRABAJO.')"';
	$layerXML.=' imgitem="../images/iconos/trabajos.gif"';
	$layerXML.=' textoitem='.$TbMsg[20];
	$layerXML.='></ITEM>'; 
	
	$layerXML.='<SEPARADOR>';
	$layerXML.='</SEPARADOR>';

	$layerXML.='<ITEM';
	$layerXML.=' imgitem="../images/iconos/acciones.gif"';
	$layerXML.=' alpulsar="cola_acciones(0)"';
	$layerXML.=' textoitem='.$TbMsg[21];
	$layerXML.='></ITEM>';

	$layerXML.='</MENUCONTEXTUAL>';
	return($layerXML);
}
//________________________________________________________________________________________________________
function ContextualXMLComandos($litambito,$ambito){
	global $cmd;
 	$maxlongdescri=0;
	$rs=new Recordset; 
	$cmd->texto="SELECT idcomando,descripcion,interactivo FROM comandos WHERE activo=1 AND aplicambito & ".$ambito.">0 ORDER BY descripcion";
	$rs->Comando=&$cmd; 
	if ($rs->Abrir()){
		$layerXML="";
		$rs->Primero(); 
		while (!$rs->EOF){
			$layerXML.='<ITEM';
			$layerXML.=' alpulsar="confirmarcomando('."'".$ambito."'".','.$rs->campos["idcomando"].','.$rs->campos["interactivo" ]. ')"';
			$layerXML.=' textoitem="'.$rs->campos["descripcion"].'"';
			$layerXML.='></ITEM>';
			if($maxlongdescri<strlen($rs->campos["descripcion"])) // Toma la Descripción de mayor longitud
				$maxlongdescri=strlen($rs->campos["descripcion"]);
			$rs->Siguiente();
		}
	$layerXML.='</MENUCONTEXTUAL>';
	$prelayerXML='<MENUCONTEXTUAL';
	$prelayerXML.=' idctx="flo_comandos_'.$litambito.'"';
	$prelayerXML.=' maxanchu='.$maxlongdescri*7;
	$prelayerXML.=' clase="menu_contextual"';
	$prelayerXML.='>';
	$finallayerXML=$prelayerXML.$layerXML;
	return($finallayerXML);
	}
}
?>
